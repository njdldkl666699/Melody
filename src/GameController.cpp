#include "GameController.h"

GameController::GameController(const QString& songFilePth, const QString& chartFilePth,
	const SettingsWidget* settingsWidget, QObject* parent)
	: settings(settingsWidget), QObject(parent),
	songFilePath(songFilePth), chartFilePath(chartFilePth)
{
	initVals();
	initnoteTracks();
	initMusicPlayer();
	timer.setTimerType(Qt::PreciseTimer);
	wait();

	connect(&musicPlayer, &QMediaPlayer::mediaStatusChanged, this, &GameController::isMusicEnd);
	connect(&countdownTimer, &QTimer::timeout, this, &GameController::gamePlay);
	connect(&timer, &QTimer::timeout, this, &GameController::judgeNoHitMiss);
	connect(&timer, &QTimer::timeout, this, &GameController::updateNote);
	connect(&timer, &QTimer::timeout, this, &GameController::signalUpdate);
	connect(&musicPlayer, &QMediaPlayer::positionChanged, this, &GameController::amendNotePos);
}

GameController::~GameController()
{
	clear();
}

void GameController::reset()
{
	clear();
	initVals();
	initnoteTracks();
	setNoteParent(noteParent);
	musicPlayer.setPosition(0);
	wait();
}

void GameController::clear()
{
	for (int i = 0; i < 4; i++)
		while (!noteTracks[i].isEmpty())
			delete noteTracks[i].dequeue();
	for (int i = 0; i < 4; i++)
		noteTracks[i].clear();

	for (auto& i : notesOutQueue)
	{
		if (i != nullptr)
		{
			delete i;
			i = nullptr;
		}
	}
	notesOutQueue.clear();
}

void GameController::setNoteParent(QWidget* parent)
{
	noteParent = parent;
	for (int i = 0; i < 4; i++)
		for (auto& note : noteTracks[i])
			note->setParent(parent);
	for (auto& note : notesOutQueue)
		note->setParent(parent);
}

QString GameController::getSongName()const
{
	QDir songDir(songFilePath);
	songDir.cdUp();
	//qDebug() << songDir.dirName();
	return songDir.dirName();
}

QString GameController::getChartName()const
{
	QFileInfo fileInfo(chartFilePath);
	QString chartName = fileInfo.fileName();
	int suffixPos = chartName.lastIndexOf(".txt");
	chartName = chartName.left(suffixPos);
	return chartName;
}

QPixmap GameController::getSongPicture()const
{
	//get song picture
	QDir songDir(songFilePath);
	songDir.cdUp();
	QStringList songPicFilters;
	songPicFilters << "*.jpg" << "*.png";
	QString songPicName = songDir.entryList(songPicFilters, QDir::Files).at(0);
	QString songPicPath = songDir.path() + "/" + songPicName;
	QPixmap songPic(songPicPath);

	//process the picture & return
	int width = songPic.width();
	int height = songPic.height();
	int sideLength = qMin(width, height);
	int x = (width - sideLength) / 2;
	int y = (height - sideLength) / 2;
	songPic = songPic.copy(x, y, sideLength, sideLength);
	return songPic;
}

void GameController::initVals()
{
	perfectCount = 0;
	goodCount = 0;
	missCount = 0;
	accuracy = 0;
	score = 0;
	combo = 0;
	maxCombo = 0;
	// speedVal is -20 ~ 20, because velocity should in 1 ~ 20.25
	velocity = 1.1 + settings->getSpeedVal() / 20.0;
	deltaTime = 1000 / settings->getFpsVal();
	waitTime = 3000;
	key[0] = settings->getKey_1().toString();
	key[1] = settings->getKey_2().toString();
	key[2] = settings->getKey_3().toString();
	key[3] = settings->getKey_4().toString();
	//Will be set in initnoteTracks()
	bpm = 0.0;
	offset = 0;
}

void GameController::initMusicPlayer()
{
	musicPlayer.setSource(QUrl::fromLocalFile(songFilePath));
	musicPlayer.setAudioOutput(&audioOutput);
	audioOutput.setVolume(settings->getMusicVal() / 100.0f);
	tapSound.setSource(QUrl::fromLocalFile("./res/note/sound.wav"));
	tapSound.setVolume(settings->getSoundVal() / 100.0f);
}

void GameController::wait()
{
	gamePause();
	countdownTimer.setSingleShot(true);
	countdownTimer.setInterval(waitTime);
	countdownTimer.start();
}

void GameController::isMusicEnd(QMediaPlayer::MediaStatus status)
{
	if (status == QMediaPlayer::EndOfMedia)
	{
		//if chart isn't end before music end,
		//show the end button
		for (int i = 0; i < 4; i++)
		{
			if (!noteTracks[i].isEmpty())
			{
				emit signalShowEndButton();
				return;
			}
		}

		timer.stop();
		musicPlayer.stop();
		emit gameEnded();
	}
}

void GameController::gamePause()
{
	countdownTimer.stop();
	velocity = 0;
	musicPlayer.pause();
	timer.stop();
}

void GameController::gamePlay()
{
	velocity = 1.1 + settings->getSpeedVal() / 20.0;
	timer.start(deltaTime);
	musicPlayer.play();
}

void GameController::initnoteTracks()
{
	//read chart file
	QFile chartFile(chartFilePath);
	if (!chartFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Open chart file failed!";
		return;
	}
	QTextStream in(&chartFile);

	//read bpm and offset
	QStringList bpmLine = in.readLine().split(",");
	if (bpmLine.first() == "bpm")
		bpm = bpmLine.last().toFloat();
	else //error
		return;
	QStringList offsetLine = in.readLine().split(",");
	if (offsetLine.first() == "offset")
		offset = offsetLine.last().toInt();
	else //error
		return;

	//read notes
	while (!in.atEnd())
	{
		QString noteLine = in.readLine();
		QStringList noteData = noteLine.split(";");
		bool isHold = noteData.first().toInt();
		QString startTimeRaw = noteData.at(1);
		int startTime = getNoteTime(startTimeRaw);

		//get KeySequence
		int key = noteData.last().toInt();
		QKeySequence keySequence;
		switch (key)
		{
		case 0:
			keySequence = settings->getKey_1();
			break;
		case 1:
			keySequence = settings->getKey_2();
			break;
		case 2:
			keySequence = settings->getKey_3();
			break;
		case 3:
			keySequence = settings->getKey_4();
			break;
		default:
			break;
		}

		//construct note
		Note* note = nullptr;
		QString picturePath;
		QSize noteSize(145, 50);
		if (!isHold)
		{
			//Tap note
			if (key == 0 || key == 3)		//Outer Track
				picturePath = "./res/note/tap_blue.png";
			else if (key == 1 || key == 2)	//Inner Track
				picturePath = "./res/note/tap_pink.png";
			else							//not happen	
				return;
			note = new Tap(startTime, keySequence, picturePath, noteSize);
		}
		else
		{
			//Hold note
			QString endTimeRaw = noteData.at(2);
			int endTime = getNoteTime(endTimeRaw);
			int height = (endTime - startTime) * velocity;
			noteSize.setHeight(height);
			if (key == 0 || key == 3)		//Outer Track
				picturePath = "./res/note/hold_blue.png";
			else if (key == 1 || key == 2)	//Inner Track
				picturePath = "./res/note/hold_pink.png";
			else							//not happen	
				return;
			note = new Hold(startTime, endTime,
				keySequence, picturePath, noteSize);
		}
		//set note x, y
		int xPos = 345 + key * 150;
		int yPos = 625 - noteSize.height() - velocity * startTime;
		note->move(xPos, yPos);

		//push note to noteTracks
		/*
		we use queue, because we just need to
		check the first note once at a time in each track
		*/
		noteTracks[key].enqueue(note);
	}

	//close file
	chartFile.close();
}

int GameController::getNoteTime(const QString& rawTimeData)const
{
	QStringList timeData = rawTimeData.split(",");
	int beat = timeData.at(0).toInt();
	int pos = timeData.at(1).toInt();
	double totalPos = timeData.at(2).toInt();
	int miliseconds = settings->getBiasVal() - offset + (beat + pos / totalPos) * 60000 / bpm;
	return miliseconds;
}

void GameController::calculateAcc()
{
	uint total = perfectCount + goodCount + missCount;
	if (total == 0)
		accuracy = 0;
	accuracy = (perfectCount * 100.0f + goodCount * 50) / total;
}

void GameController::judgeKeyPress(QKeyEvent* event)
{
	for (int i = 0; i < 4; i++)
	{
		if (noteTracks[i].isEmpty())
			continue;
		QString eventKey = event->text().toUpper();
		//qDebug() << key[i] << '\n' << eventKey;
		if (key[i] == eventKey)
		{
			Note* headNote = noteTracks[i].head();

			////#######Judge By the difference of time#######
			//int musicCurrentTime = getMusicCurrentTime();
			//int noteStartTime = headNote->getStartTime();
			///*
			//notes:	1¡¢musiccurrenttime is current time you press the key
			//		2¡¢difference < 0 means you press the key late, > 0 means early
			//*/
			//int difference = noteStartTime - musicCurrentTime;
			//qDebug() << "hit!\tx: " << headNote->x() << "\ty: " << headNote->y()
			//<< "\tnoteStartTime: " << noteStartTime
			//	<< "\tmusicCurrentTime: " << musicCurrentTime
			//	<< "\tdifference: " << difference; 
			////###############################################

			//######Judge By the distance of note and judgeline######
			int currentY = headNote->y() + headNote->height();
			int deltaY = 625 - currentY;
			float difference = deltaY / velocity;
			/*qDebug() << "hit!\tx:" << headNote->x() << "\ty: " << headNote->y()
				<< "\tdeltaY: " << deltaY << "\tdifference: " << difference;*/
			//#######################################################

			//case Tap note
			if (headNote->getType() == "Tap")
			{
				//case Perfect
				if (difference >= -50 && difference <= 50)
				{
					perfectCount++;
					combo++;
					if (combo > maxCombo)
						maxCombo = combo;
					score += 100;
					calculateAcc();
					tapSound.play();
					emit judgeResult("Perfect");
					//pop note and delete
					delete noteTracks[i].dequeue();
				}
				//case Good
				else if (difference >= -100 && difference <= 100)
				{
					goodCount++;
					combo++;
					if (combo > maxCombo)
						maxCombo = combo;
					score += 50;
					calculateAcc();
					tapSound.play();
					emit judgeResult("Good");
					//pop note and delete
					delete noteTracks[i].dequeue();
				}
				//part of case Miss,it means press too early.
				//another part is in Function judgeNoHitMiss()
				else if (difference > 100 && difference < 120)
				{
					missCount++;
					combo = 0;
					calculateAcc();
					emit judgeResult("Miss");
					//pop note and delete
					delete noteTracks[i].dequeue();
				}
			}
			//case Hold note
			else if (headNote->getType() == "Hold")
			{
				//always to be Hold, because getType() tells us.
				assert(dynamic_cast<Hold*>(headNote));
				Hold* hold = dynamic_cast<Hold*>(headNote);

				//if have been Miss, don't judge again
				//acctually it's impossible to reach here, because
				//if Hold is Miss, it will be transfered to notesOutQueue
				if (hold->getState() == Hold::Miss)
					continue;

				qDebug() << "Pressed!\tx:" << headNote->x() << "\tcurrentY: " << currentY
					<< "\tdeltaY: " << deltaY << "\tdifference: " << difference;

				//judge
				//case Perfect
				if (difference >= -50 && difference <= 50)
				{
					//mark as to be Perfect
					hold->setState(Hold::Perfect);
				}
				//case Good
				else if (difference >= -100 && difference <= 100)
				{
					//mark as to be Good
					hold->setState(Hold::Good);
				}
				//case Miss is in Function judgeKeyRelease() and judgeNoHitMiss(),
				//because Hold don't have "Bad"(too early) state.
			}
			else
			{
				//don't expect to reach here
				qDebug() << "Invalid note type!";
				return;
			}
		}
	}
}

void GameController::judgeKeyRelease(QKeyEvent* event)
{
	for (int i = 0; i < 4; i++)
	{
		if (noteTracks[i].isEmpty())
			continue;
		QString eventKey = event->text().toUpper();
		if (key[i] == eventKey)
		{
			Note* headNote = noteTracks[i].head();
			if (headNote->getType() == "Hold")
			{
				qDebug() << "Release: In Hold: key[" << i << "] "
					<< key[i] << " eventKey: " << eventKey;
				//always to be Hold, because getType() tells us.
				assert(dynamic_cast<Hold*>(headNote));
				Hold* hold = dynamic_cast<Hold*>(headNote);

				////#######Judge By the difference of time#######
				//int noteEndTime = hold->getEndTime();
				//int musicCurrentTime = getMusicCurrentTime();
				//int difference = noteEndTime - musicCurrentTime;
				////#############################################

				//######Judge By the distance of note and judgeline######
				int currentY = hold->y();
				int deltaY = 625 - currentY;
				float difference = deltaY / velocity;
				qDebug() << "Released!\tx:" << hold->x() << "\tcurrentY: " << currentY
					<< "\tdeltaY: " << deltaY << "\tdifference: " << difference;
				//#######################################################

				//judgement of ending, > 50ms means release too early
				Hold::ToBeState state = hold->getState();
				qDebug() << "state: " << state;
				//case None, not press, don't judge
				if (state == Hold::None)
					continue;
				if (difference <= 50)
				{
					//if difference < 0, means release too late, but no problem
					//case Perfect
					if (state == Hold::Perfect)
					{
						perfectCount++;
						combo++;
						if (combo > maxCombo)
							maxCombo = combo;
						score += 100;
						calculateAcc();
						emit judgeResult("Perfect");
						//pop note and delete
						delete noteTracks[i].dequeue();
					}
					//case Good
					else if (state == Hold::Good)
					{
						goodCount++;
						combo++;
						if (combo > maxCombo)
							maxCombo = combo;
						score += 50;
						calculateAcc();
						emit judgeResult("Good");
						//pop note and delete
						delete noteTracks[i].dequeue();
					}
				}
				//case Miss, release too early
				else if (difference > 50)
				{
					hold->setState(Hold::Miss);
					missCount++;
					combo = 0;
					calculateAcc();
					emit judgeResult("Miss");
					// hold->setState(Hold::Miss);	//had set in Hold()

					// set Hold picture miss version
					QString missPicPath;
					if (i == 0 || i == 3)
						missPicPath = "./res/note/hold_blue_miss.png";
					else if (i == 1 || i == 2)
						missPicPath = "./res/note/hold_pink_miss.png";
					QPixmap missPic(missPicPath);
					missPic = missPic.scaled(hold->size(),
						Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
					hold->setPixmap(missPic);

					//dequeue the Hold and push into notesOutQueue,
					//to draw the left part. 
					notesOutQueue.push_back(noteTracks[i].dequeue());
				}
			}
		}
	}
}

void GameController::judgeNoHitMiss()
{
	for (int i = 0; i < 4; i++)
	{
		if (noteTracks[i].isEmpty())
			continue;
		Note* headNote = noteTracks[i].head();

		////#######Judge By the difference of time#######
		//int musicCurrentTime = getMusicCurrentTime();
		//int noteStartTime = headNote->getStartTime();
		//int difference = noteStartTime - musicCurrentTime;
		////#############################################

		//######Judge By the distance of note and judgeline######
		int currentY = headNote->y() + headNote->height();
		int deltaY = 625 - currentY;
		float difference = deltaY / velocity;
		//#######################################################

		if (headNote->getType() == "Tap")
		{
			//case Miss, too late
			if (difference < -100)
			{
				/*qDebug() << "Miss!\tx: " << headNote->x() << "\ty: " << headNote->y()
					<< "\tnoteStartTime: " << noteStartTime
					<< "\tmusicCurrentTime: " << musicCurrentTime
					<< "\tdifference: " << difference;*/

				/*qDebug() << "Miss!\tx:" << headNote->x() << "\ty: " << headNote->y()
					<< "\tdeltaY: " << deltaY << "\tdifference: " << difference;*/

				missCount++;
				combo = 0;
				calculateAcc();
				emit judgeResult("Miss");
				//pop note and delete
				delete noteTracks[i].dequeue();
			}
		}
		else if (headNote->getType() == "Hold")
		{
			//always to be Hold, because getType() tells us.
			assert(dynamic_cast<Hold*>(headNote));
			Hold* hold = dynamic_cast<Hold*>(headNote);

			//too late or don't press the whole time
			if (difference < -100 || hold->y() > 625)
			{
				qDebug() << "NoHit!\tx:" << hold->x() << "\ty: " << hold->y()
					<< "\tdeltaY: " << deltaY << "\tdifference: " << difference;

				Hold::ToBeState state = hold->getState();
				//case Perfect
				if (state == Hold::Perfect)
				{
					perfectCount++;
					combo++;
					if (combo > maxCombo)
						maxCombo = combo;
					score += 100;
					calculateAcc();
					emit judgeResult("Perfect");
					//pop note and delete
					delete noteTracks[i].dequeue();
				}
				//case Good
				else if (state == Hold::Good)
				{
					goodCount++;
					combo++;
					if (combo > maxCombo)
						maxCombo = combo;
					score += 50;
					calculateAcc();
					emit judgeResult("Good");
					//pop note and delete
					delete noteTracks[i].dequeue();
				}
				//None->Miss
				else if (state == Hold::None)
				{
					qDebug() << "In Miss";
					hold->setState(Hold::Miss);
					missCount++;
					combo = 0;
					calculateAcc();
					emit judgeResult("Miss");
					// hold->setState(Hold::Miss);	//had set in Hold()

					// set Hold picture miss version
					QString missPicPath;
					if (i == 0 || i == 3)
						missPicPath = "./res/note/hold_blue_miss.png";
					else if (i == 1 || i == 2)
						missPicPath = "./res/note/hold_pink_miss.png";
					QPixmap missPic(missPicPath);
					missPic = missPic.scaled(hold->size(),
						Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
					hold->setPixmap(missPic);

					//dequeue the Hold and push into notesOutQueue,
					//to draw the left part. 
					notesOutQueue.push_back(noteTracks[i].dequeue());
				}
				//Miss from release too early is in Function judgeKeyRelease()
			}
		}
	}
}

void GameController::updateNote()
{
	// [DELETED] no use
	// //##new move function##
	// /*it seems that call getMusicCurrentTime() 
	// frequently is not a good solution. :( 
	// it seeeeeems that getMusicCurrentTime() 
	// only return the last state it updated*/

	//##legacy move function##
	//move notes in queue
	for (int i = 0; i < 4; i++)
	{
		for (Note* note : noteTracks[i])
		{
			QPoint pos = note->pos();
			pos.setY(pos.y() + velocity * deltaTime);
			note->move(pos);
		}
	}
	//move notes in vector
	for (Note* note : notesOutQueue)
	{
		if (note != nullptr)
		{
			QPoint pos = note->pos();
			pos.setY(pos.y() + velocity * deltaTime);
			note->move(pos);
		}
	}
}

void GameController::amendNotePos(qint64 musicCurrentTime)
{
	//##improvement on the legacy function##
	//everytime positionChanged() is emitted, amend the position.
	//qDebug() << "amendNotePos() called; time:" << musicCurrentTime;
	for (int i = 0; i < 4; i++)
	{
		for (Note* note : noteTracks[i])
		{
			bool needMove = true;
			if (note->getType() == "Hold")
			{
				Hold* hold = dynamic_cast<Hold*>(note);
				if (hold->getState() != Hold::None)
					needMove = false;
			}
			if (needMove)
			{
				int noteStartTime = note->getStartTime();
				int difference = noteStartTime - musicCurrentTime;
				int yPos = 625 - note->height() - difference * velocity;
				note->move(note->x(), yPos);
			}
		}
	}
	for (Note* note : notesOutQueue)
	{
		if (note != nullptr)
		{
			int noteStartTime = note->getStartTime();
			int difference = noteStartTime - musicCurrentTime;
			int yPos = 625 - note->height() - difference * velocity;
			note->move(note->x(), yPos);
		}
	}
}