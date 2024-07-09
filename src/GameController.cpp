#include "GameController.h"

GameController::GameController(const QString& songFilePth, const QString& chartFilePth,
	const SettingsWidget* settingsWidget, QObject* parent)
	: settings(settingsWidget), QObject(parent),
	songFilePath(songFilePth), chartFilePath(chartFilePth)
{
	initData();
	initnoteTracks();
	initMusicPlayer();
	connect(&musicPlayer, &QMediaPlayer::positionChanged, this, &GameController::judgeNoHitMiss);
	connect(&musicPlayer, &QMediaPlayer::mediaStatusChanged, this, &GameController::isMusicEnd);
}

GameController::~GameController()
{
	for (int i = 0; i < 4; i++)
		while (!noteTracks[i].isEmpty())
			delete noteTracks[i].dequeue();

	for (auto& i : notesOutQueue)
		delete i;
}

void GameController::reset()
{
	initData();
	initnoteTracks();
	musicPlayer.setPosition(0);
	musicPlayer.play();
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
	QDir songDir(songFilePath);
	songDir.cdUp();
	QStringList songPicFilters;
	songPicFilters << "*.jpg" << "*.png";
	QString songPicName = songDir.entryList(songPicFilters, QDir::Files).at(0);
	QString songPicPath = songDir.path() + "/" + songPicName;
	return QPixmap(songPicPath);
}

void GameController::initData()
{
	perfectCount = 0;
	goodCount = 0;
	missCount = 0;
	accuracy = 0;
	score = 0;
	combo = 0;
	maxCombo = 0;
	key[0] = settings->getKey_1().toString();
	key[1] = settings->getKey_2().toString();
	key[2] = settings->getKey_3().toString();
	key[3] = settings->getKey_4().toString();
	//Will be set in initnoteTracks()
	bpm = 0.0;
	offset = 0;
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

	////read bpm and offset
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
		QString soundPath = "./res/note/sound.wav";
		QString picturePath;
		if (!isHold)
		{
			//Tap note
			if (key == 0 || key == 3)		//Outer Track
				picturePath = "./res/note/note_blue.png";
			else if (key == 1 || key == 2)	//Inner Track
				picturePath = "./res/note/note_pink.png";
			else							//not happen	
				return;
			note = new Tap(startTime, keySequence, soundPath, picturePath, QSize(10, 10));
		}
		else
		{
			//Hold note
			QString endTimeRaw = noteData.at(2);
			int endTime = getNoteTime(endTimeRaw);
			if (key == 0 || key == 3)		//Outer Track
				picturePath = "./res/note/hold_blue.png";
			else if (key == 1 || key == 2)	//Inner Track
				picturePath = "./res/note/hold_pink.png";
			else							//not happen	
				return;
			note = new Hold(startTime, endTime, keySequence, soundPath, picturePath, QSize(10, 10));
		}

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

void GameController::initMusicPlayer()
{
	musicPlayer.setSource(QUrl::fromLocalFile(songFilePath));
	musicPlayer.setAudioOutput(&audioOutput);
	audioOutput.setVolume(settings->getMusicVal() / 100.0f);
	musicPlayer.play();
}

void GameController::isMusicEnd(QMediaPlayer::MediaStatus status)
{
	if (status == QMediaPlayer::EndOfMedia)
		emit musicEnded();
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

void GameController::judgeKeyPress(QKeyEvent* event)
{
	for (int i = 0; i < 4; i++)
	{
		QString eventKey = event->text().toUpper();
		qDebug() << key[i] << '\n' << eventKey;
		if (key[i] == eventKey)
		{
			qDebug() << "Hit!";
			const auto& headNote = noteTracks[i].head();
			int musicCurrentTime = getMusicCurrentTime();
			int noteStartTime = headNote->getStartTime();
			/*
			notes:	1¡¢musicCurrentTime is current time you press the key
					2¡¢difference < 0 means you press the key late, > 0 means early
			*/
			int difference = noteStartTime - musicCurrentTime;

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
					//pop note and delete
					delete noteTracks[i].dequeue();
				}
				//part of case Miss,it means press too early.
				//another part is in Function judgeNoHitMiss()
				else if (difference > 100 && difference < 120)
				{
					missCount++;
					combo = 0;
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
		if (key[i] == event->text())
		{
			qDebug() << "Released!";
			const auto& head = noteTracks[i].head();
			if (head->getType() == "Hold")
			{
				//always to be Hold, because getType() tells us.
				assert(dynamic_cast<Hold*>(head));
				Hold* hold = dynamic_cast<Hold*>(head);

				//judge
				int noteEndTime = hold->getEndTime();
				int musicCurrentTime = getMusicCurrentTime();
				int difference = noteEndTime - musicCurrentTime;

				//judgement of ending, > 50ms means release too early
				if (difference >= -50 && difference <= 50)
				{
					//case Perfect
					if (hold->getState() == Hold::Perfect)
					{
						perfectCount++;
						combo++;
						if (combo > maxCombo)
							maxCombo = combo;
						score += 100;
						//pop note and delete
						delete noteTracks[i].dequeue();
					}
					//case Good
					else if (hold->getState() == Hold::Good)
					{
						goodCount++;
						combo++;
						if (combo > maxCombo)
							maxCombo = combo;
						score += 50;
						//pop note and delete
						delete noteTracks[i].dequeue();
					}
				}
				//case Miss, release too early
				else if (difference > 50)
				{
					missCount++;
					combo = 0;
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
		const auto& head = noteTracks[i].head();
		int musicCurrentTime = getMusicCurrentTime();
		int noteStartTime = head->getStartTime();
		int difference = noteStartTime - musicCurrentTime;
		if (head->getType() == "Tap")
		{
			//case Miss, too late
			if (difference < -100)
			{
				missCount++;
				combo = 0;
				//pop note and delete
				delete noteTracks[i].dequeue();
			}
		}
		else if (head->getType() == "Hold")
		{
			//always to be Hold, because getType() tells us.
			assert(dynamic_cast<Hold*>(head));
			Hold* hold = dynamic_cast<Hold*>(head);

			//case Miss, too late
			if (hold->getState() != Hold::Miss && difference < -100)
			{
				missCount++;
				combo = 0;
				//dequeue the Hold and push into notesOutQueue,
				//to draw the left part. 
				notesOutQueue.push_back(noteTracks[i].dequeue());
			}
		}
	}
}

void GameController::drawNote()
{
	//draw notes on Queue

	//draw notes out Queue
}