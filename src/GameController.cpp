/*
	<Source file of class GameController>
	Copyright (C) <2024>  <Meolide Team>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "GameController.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QKeyEvent>
#include <QPropertyAnimation>

#include <QtAssert>

GameController::GameController(const QString& songFilePth,
	const QString& chartFilePth, QWidget* noteParent, QObject* parent)
	: settings(SettingsWidget::instance()), songFilePath(songFilePth),
	chartFilePath(chartFilePth), playWidget(noteParent), QObject(parent),
	noteAnimationGroup(new QParallelAnimationGroup(noteParent))
{
	initVals();
	initNoteTracks();
	initMusicPlayer();

	//Init timers
	chartCountdownTimer.setTimerType(Qt::PreciseTimer);
	chartCountdownTimer.setSingleShot(true);
	chartCountdownTimer.setInterval(waitTime);

	musicCountdownTimer.setTimerType(Qt::PreciseTimer);
	musicCountdownTimer.setSingleShot(true);

	wait();

	//connect chart related
	connect(&chartCountdownTimer, &QTimer::timeout, this, &GameController::chartPlay);
	connect(noteAnimationGroup, &QParallelAnimationGroup::finished,
		this, &GameController::signalShowEndButton);

	//connect music related
	connect(&musicCountdownTimer, &QTimer::timeout, &musicPlayer, &QMediaPlayer::play);
	connect(&musicPlayer, &QMediaPlayer::mediaStatusChanged, this, &GameController::musicEnd);

	//connect timer related
	connect(&timer, &QTimer::timeout, this, &GameController::judgeNoHitMiss);
	connect(&timer, &QTimer::timeout, this, &GameController::judgeNoReleaseHold);
	connect(&timer, &QTimer::timeout, this, &GameController::signalUpdate);
}

GameController::~GameController()
{
	if (noteAnimationGroup != nullptr)
	{
		delete noteAnimationGroup;
		noteAnimationGroup = nullptr;
	}
}

void GameController::initVals()
{
	perfectCount = 0;
	goodCount = 0;
	badCount = 0;
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
}

void GameController::initMusicPlayer()
{
	musicPlayer.setSource(QUrl::fromLocalFile(songFilePath));
	musicPlayer.setAudioOutput(&audioOutput);
	audioOutput.setVolume(settings->getMusicVal() / 100.0f);
	noteSound.setSource(QUrl::fromLocalFile("./res/note/sound.wav"));
	noteSound.setVolume(settings->getSoundVal() / 100.0f);
}

void GameController::initNoteTracks()
{
	//read chart file and write it to JsonObject
	QFile chartFile(chartFilePath);
	if (!chartFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Open chart file failed!";
		return;
	}
	const auto& chartDoc = QJsonDocument::fromJson(chartFile.readAll());
	if (!chartDoc.isObject())
	{
		qDebug() << "Chart is not a JsonObject!";
		return;
	}
	const auto& chartObj = chartDoc.object();
	const auto& chartArray = chartObj["note"].toArray();
	offset = chartObj["offset"].toDouble();

	//parse chartArray
	for (qsizetype i = 0; i < chartArray.size(); ++i)
	{
		const auto& noteVal = chartArray[i];
		const auto& noteObj = noteVal.toObject();
		bool isHold = noteObj.contains("endTime");

		//init note
		Note* note = nullptr;
		QString picturePath;
		int startTime = noteObj["time"].toInt();
		int endTime;
		int column = noteObj["column"].toInt();
		if (!isHold) //case Tap
		{
			const QSize noteSize(145, 50);
			endTime = startTime + 50 / velocity;
			if (column == 0 || column == 3)
				picturePath = "./res/note/tap_blue.png";
			else if (column == 1 || column == 2)
				picturePath = "./res/note/tap_pink.png";
			else	//not happen, but if happened, just send a warning
				qDebug() << "Warning: note column is invalid!";
			note = new Tap(startTime, picturePath, noteSize, playWidget);
		}
		else //case Hold
		{
			endTime = noteObj["endTime"].toInt();
			int height = (endTime - startTime) * velocity;
			const QSize noteSize(145, height);
			if (column == 0 || column == 3)
				picturePath = "./res/note/hold_blue.png";
			else if (column == 1 || column == 2)
				picturePath = "./res/note/hold_pink.png";
			else	//not happen, but if happened, just skip
				qDebug() << "Warning: note column is invalid!";
			note = new Hold(startTime, endTime, picturePath, noteSize, playWidget);
		}

		//set note (x, y)
		int xPos = 345 + column * 150;
		int yPos = 625 - note->height() - velocity * startTime;
		note->move(xPos, yPos);

		//init note animation
		auto* animation = new QPropertyAnimation(note, "pos", playWidget);
		animation->setStartValue(QPoint(xPos, yPos));
		int endY = 625 + velocity * 150;
		animation->setEndValue(QPoint(xPos, endY));

		//duration = (endY - yPos) / velocity = 150 + endTime;
		//You can calculate it by yourself.
		int duration = 150 + endTime;
		animation->setDuration(duration);
		animation->setEasingCurve(QEasingCurve::Linear);
		noteAnimationGroup->addAnimation(animation);
		note->setAnimationIndex(i);

		//push note to noteTracks
		noteTracks[column].enqueue(note);
	}

	//close file
	chartFile.close();
}

void GameController::wait()
{
	gamePause();
	int musicWaitTime = waitTime + offset;
	if (musicPlayer.duration() == 0)
	{
		// if this function is called first time in one game,
		// we should wait a BIAS time more than chartCountDownTimer
		// then start musicPlayer.
		musicWaitTime += settings->getBiasVal();
	}
	musicCountdownTimer.setInterval(musicWaitTime);
	//start countdown almost at the same time
	chartCountdownTimer.start();
	musicCountdownTimer.start();
}

void GameController::chartPlay()
{
	timer.start(deltaTime);
	auto& g = noteAnimationGroup;
	if (g->state() == QParallelAnimationGroup::Paused)
		g->resume();
	else
		g->start();
}

void GameController::gamePause()
{
	timer.stop();
	auto& g = noteAnimationGroup;
	if (g->state() == QParallelAnimationGroup::Running)
		g->pause();
	musicPlayer.pause();
	chartCountdownTimer.stop();
	musicCountdownTimer.stop();
}

void GameController::musicEnd(QMediaPlayer::MediaStatus status)
{
	if (status == QMediaPlayer::EndOfMedia)
	{
		timer.stop();
		emit gameEnded();
	}
}

void GameController::reset()
{
	noteAnimationGroup->stop();
	musicPlayer.setPosition(0);
	initVals();
	resetNoteTracks();
	wait();
}

void GameController::resetNoteTracks()
{
	//reset Hold in noteOutTracks
	for (int i = 0; i < 4; i++)
	{
		for (auto note : noteOutTracks[i])
		{
			if (note->getType() == "Tap")
			{
				note->show();
			}
			else if (note->getType() == "Hold")
			{
				Q_ASSERT(dynamic_cast<Hold*>(note));
				Hold* hold = dynamic_cast<Hold*>(note);
				hold->reset();
			}
		}
	}
	//push the rest notes from noteTracks to noteOutTracks
	for (int i = 0; i < 4; i++)
	{
		while (!noteTracks[i].isEmpty())
		{
			Note* note = noteTracks[i].dequeue();
			noteOutTracks[i].enqueue(note);
		}
	}
	//swap noteTracks and noteOutTracks
	for (int i = 0; i < 4; i++)
	{
		// The following "note" is from QQueue header file.   :)
		// compiler-generated special member functions are fine!
		noteTracks[i].swap(noteOutTracks[i]);
	}
}

void GameController::judgeKeyPress(QKeyEvent* event)
{
	QString eventKey = event->text().toUpper();
	for (int i = 0; i < 4; i++)
	{
		if (noteTracks[i].isEmpty())
			continue;
		//qDebug() << key[i] << '\n' << eventKey;
		if (key[i] == eventKey)
		{
			Note* headNote = noteTracks[i].head();

			//#######Judge By the difference of time#######
			/*	Notes:
			1.	noteCurTime is the difference of time
					between the start of the note and the current time.
			2.	difference < 0 means you press the key late, > 0 means early.
			3.	the calculate of "difference":
					difference = (625 - note->height() - note->y()) / velocity,
					note->y() = note_start_Y + noteCurTime * velocity
					note_start_Y = 625 - note->height() - noteStartTime * velocity
				Then you can prove the formula.  :)
			4.	If you don't want to calculate this, you can judge by the distance
					of note and judgeline, but it may be not accurate.
			*/
			int noteStartTime = headNote->getStartTime();
			int index = headNote->getAnimationIndex();
			auto animation = noteAnimationGroup->animationAt(index);
			int noteCurTime = animation->currentTime();
			int difference = noteStartTime - noteCurTime;
			//qDebug() << "Hit!\tx: " << headNote->x()
			//	<< "\ty: " << headNote->y()
			//	<< "\tnoteStartTime: " << noteStartTime
			//	<< "\tnoteCurTime: " << noteCurTime
			//	<< "\tdifference: " << difference;
			//###############################################

			//case Tap note
			if (headNote->getType() == "Tap")
			{
				//case Perfect
				if (difference >= -50 && difference <= 50)
				{
					perfectCount++;
					combo++;
					maxCombo = qMax(combo, maxCombo);
					noteSound.play();
					calculateAccAndScore();
					emit judgeResult("Perfect");
					//Stop animation, hide note, move note
					animation->stop();
					headNote->hide();
					//see Explain in GameController.h
					noteOutTracks[i].enqueue(noteTracks[i].dequeue());
				}
				//case Good
				else if (difference >= -100 && difference <= 100)
				{
					goodCount++;
					combo++;
					maxCombo = qMax(maxCombo, combo);
					calculateAccAndScore();
					noteSound.play();
					emit judgeResult("Good");
					animation->stop();
					headNote->hide();
					noteOutTracks[i].enqueue(noteTracks[i].dequeue());
				}
				//case Bad
				else if (difference > 100 && difference < 120)
				{
					badCount++;
					combo = 0;
					calculateAccAndScore();
					noteSound.play();
					emit judgeResult("Bad");
					animation->stop();
					headNote->hide();
					noteOutTracks[i].enqueue(noteTracks[i].dequeue());
				}
			}
			//case Hold note
			else if (headNote->getType() == "Hold")
			{
				//always to be Hold, because getType() tells us.
				Q_ASSERT(dynamic_cast<Hold*>(headNote));
				Hold* hold = dynamic_cast<Hold*>(headNote);

				//if have been Miss, don't judge again
				//acctually it's impossible to reach here, because
				//if Hold is Miss, it will be transfered to notesOutQueue
				if (hold->getState() == Hold::Miss)
					continue;

				//qDebug() << "Hold: Pressed!";

				//judge
				//case Perfect
				if (difference >= -50 && difference <= 50)
				{
					//mark as to be Perfect
					hold->setState(Hold::Perfect);
					emit judgeResult("Perfect");
					noteSound.play();
				}
				//case Good
				else if (difference >= -100 && difference <= 100)
				{
					//mark as to be Good
					hold->setState(Hold::Good);
					emit judgeResult("Good");
					noteSound.play();
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
	QString eventKey = event->text().toUpper();
	for (int i = 0; i < 4; i++)
	{
		if (noteTracks[i].isEmpty())
			continue;
		if (key[i] == eventKey)
		{
			Note* headNote = noteTracks[i].head();
			//Only Hold note need to judge release
			if (headNote->getType() == "Hold")
			{
				//qDebug() << "Release: In Hold: key[" << i << "] "
				//	<< key[i] << " eventKey: " << eventKey;
				Q_ASSERT(dynamic_cast<Hold*>(headNote));
				Hold* hold = dynamic_cast<Hold*>(headNote);

				////#######Judge By the difference of time#######
				int noteEndTime = hold->getEndTime();
				int index = hold->getAnimationIndex();
				auto animation = noteAnimationGroup->animationAt(index);
				int noteCurTime = animation->currentTime();

				//NOTE: use noteEndTime, NOT noteStartTime!!!
				int difference = noteEndTime - noteCurTime;
				//qDebug() << "Released!\tx: " << hold->x()
				//	<< "\ty: " << hold->y()
				//	<< "\tnoteEndTime: " << noteEndTime
				//	<< "\tnoteCurTime: " << noteCurTime
				//	<< "\tdifference: " << difference;
				////#############################################

				//judgement of ending, > 100ms means release too early
				Hold::ToBeState state = hold->getState();
				//qDebug() << "state: " << state;
				//case None, not press, don't judge
				if (state == Hold::None)
					continue;
				if (difference <= 100)
				{
					//if difference < 0, means release too late, but no problem
					//case Perfect
					if (state == Hold::Perfect)
					{
						perfectCount++;
						combo++;
						maxCombo = qMax(combo, maxCombo);
						calculateAccAndScore();
						emit judgeResult("Perfect");
						noteOutTracks[i].enqueue(noteTracks[i].dequeue());
					}
					//case Good
					else if (state == Hold::Good)
					{
						goodCount++;
						combo++;
						maxCombo = qMax(combo, maxCombo);
						calculateAccAndScore();
						emit judgeResult("Good");
						noteOutTracks[i].enqueue(noteTracks[i].dequeue());
					}
				}
				//case Miss, release too early
				else if (difference > 100)
				{
					hold->setState(Hold::Miss);
					missCount++;
					combo = 0;
					calculateAccAndScore();
					emit judgeResult("Miss");
					// hold->setState(Hold::Miss);	//had set in Hold() constructor

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

					//move note
					noteOutTracks[i].enqueue(noteTracks[i].dequeue());
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

		//#######Judge By the difference of time#######
		int index = headNote->getAnimationIndex();
		auto animation = noteAnimationGroup->animationAt(index);
		int noteCurTime = animation->currentTime();
		//qDebug() << "No Hit!\tx: " << headNote->x()
		//	<< "\ty: " << headNote->y()
		//	<< "\tnoteStartTime: " << noteStartTime
		//	<< "\tnoteCurTime: " << noteCurTime
		//	<< "\tdifference: " << difference;
		//#############################################

		if (headNote->getType() == "Tap")
		{
			int noteStartTime = headNote->getStartTime();
			int difference = noteStartTime - noteCurTime;
			//case Miss, too late
			if (difference < -100)
			{
				qDebug() << "Tap Miss!\tx: " << headNote->x()
					<< "\ty: " << headNote->y()
					<< "\tnoteStartTime: " << noteStartTime
					<< "\tnoteCurTime: " << noteCurTime
					<< "\tdifference: " << difference;
				missCount++;
				combo = 0;
				calculateAccAndScore();
				emit judgeResult("Miss");
				//Hide note, move note
				animation->stop();
				headNote->hide();
				noteOutTracks[i].enqueue(noteTracks[i].dequeue());
			}
		}
		else if (headNote->getType() == "Hold")
		{
			//always to be Hold, because getType() tells us.
			Q_ASSERT(dynamic_cast<Hold*>(headNote));
			Hold* hold = dynamic_cast<Hold*>(headNote);

			int noteStartTime = headNote->getStartTime();
			int difference = noteStartTime - noteCurTime;

			//case Miss, too late to hit
			if (difference < -100)
			{
				Hold::ToBeState state = hold->getState();
				if (state == Hold::Miss)
					continue;
				//case No Hit Miss
				if (state == Hold::None)
				{
					//qDebug() << "Hold No Hit Miss!\tx: " << headNote->x()
					//	<< "\ty: " << headNote->y()
					//	<< "\tnoteStartTime: " << noteStartTime
					//	<< "\tnoteCurTime: " << noteCurTime
					//	<< "\tdifference: " << difference;

					hold->setState(Hold::Miss);
					missCount++;
					combo = 0;
					calculateAccAndScore();
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

					noteOutTracks[i].enqueue(noteTracks[i].dequeue());
				}
				//Miss from release too early is in Function judgeKeyRelease()
			}
		}
	}
}

/* Explain:
	Judge a Hold which is pressed all the time
	but not released, which should be moved to
	noteOutTracks and be judged as what it should be.
*/
void GameController::judgeNoReleaseHold()
{
	for (int i = 0; i < 4; i++)
	{
		if (noteTracks[i].isEmpty())
			continue;
		Note* headNote = noteTracks[i].head();
		if (headNote->getType() == "Hold")
		{
			//always to be Hold, because getType() tells us.
			Q_ASSERT(dynamic_cast<Hold*>(headNote));
			Hold* hold = dynamic_cast<Hold*>(headNote);
			Hold::ToBeState state = hold->getState();

			/* Explain:
				There are only two cases, Perfect and Good,
				beacause if the Hold is Miss, it has been judged
				in judgeNoHitMiss() function and moved to noteOutTracks;
				if is None, it is not a Pressed Hold, so don't judge too.
			*/
			if (state == Hold::Miss || state == Hold::None)
				continue;

			//#######Judge By the difference of time#######
			int noteEndTime = hold->getEndTime();
			int index = hold->getAnimationIndex();
			auto animation = noteAnimationGroup->animationAt(index);
			int noteCurTime = animation->currentTime();
			int difference = noteEndTime - noteCurTime;
			//qDebug() << "No Release Hold!\tx: " << headNote->x()
			//	<< "\ty: " << headNote->y()
			//	<< "\tnoteEndTime: " << noteEndTime
			//	<< "\tnoteCurTime: " << noteCurTime
			//	<< "\tdifference: " << difference;
			////#############################################

			/* difference <= 100
			Example:
				noteEndTime		noteCurTime		difference
				1800			1750			50
				1800			1850			-50
			*/
			if (difference <= 100)
			{
				if (state == Hold::Perfect)
				{
					perfectCount++;
					emit judgeResult("Perfect");
				}
				else if (state == Hold::Good)
				{
					goodCount++;
					emit judgeResult("Good");
				}
				combo++;
				maxCombo = qMax(combo, maxCombo);
				calculateAccAndScore();
				noteOutTracks[i].enqueue(noteTracks[i].dequeue());
			}
		}
	}
}

void GameController::calculateAccAndScore()
{
	uint total = perfectCount + goodCount + badCount + missCount;
	if (total == 0)
		accuracy = 0;
	else
		accuracy = (perfectCount * 100.0f + goodCount * 65) / total;
	score = perfectCount * 100 + goodCount * 65;
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
	int suffixPos = chartName.lastIndexOf(".meo");
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
