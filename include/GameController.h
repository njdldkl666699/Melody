/*
	<Header file of class GameController>
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

#pragma once

#include "SettingsWidget.h"
#include "Hold.h"
#include "Tap.h"

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>

#include <QQueue>
#include <QTimer>
#include <QParallelAnimationGroup>

class GameController : public QObject
{
	Q_OBJECT

public:
	GameController(const QString& songFilePth, const QString& chartFilePth,
		QWidget* noteParent, QObject* parent = nullptr);
	~GameController();
	//## Only Use in PlayWidget.cpp ##
	void reset();
	void wait();
	void chartPlay();

public:
	//get functions
	uint getPerfectCount() const { return perfectCount; }
	uint getGoodCount() const { return goodCount; }
	uint getBadCount()const { return badCount; }
	uint getMissCount() const { return missCount; }
	float getAccuracy() const { return accuracy; } //0~100, without the '%'
	uint getScore() const { return score; }
	uint getCombo() const { return combo; }
	uint getMaxCombo() const { return maxCombo; }
	QString getSongName()const;
	QString getChartName()const;
	QPixmap getSongPicture()const;

signals:
	void gameEnded();
	void signalShowEndButton();
	void signalUpdate();
	void judgeResult(const QString& comment);

public slots:
	// Judge related
	/* Judge time(ms) table:
	Perfect: -50 ~ 50
	Good: -100 ~ -51, 51 ~ 100
	Bad: 100 ~ 120 (only Tap)
	Miss: < -100, not hit
	*/
	void judgeKeyPress(QKeyEvent* event);
	void judgeKeyRelease(QKeyEvent* event);
	//game related
	void gamePause();

private slots:
	void judgeNoHitMiss();
	void judgeNoReleaseHold();
	//music related
	void musicEnd(QMediaPlayer::MediaStatus status);

private:
	//init functions
	void initVals();
	/* func initNoteTracks
	1. Read chart file
	2. Create Note objects
	3. Set Animation
	*/
	void initNoteTracks();
	void initMusicPlayer();

	void calculateAccAndScore();
	
	void resetNoteTracks();

private:
	uint perfectCount, goodCount, badCount, missCount;
	float accuracy;
	uint score;
	uint combo, maxCombo;

	float velocity;	//pixel per ms
	int deltaTime;
	int waitTime;
	QString key[4];

	const SettingsWidget* settings;
	const QString songFilePath;
	const QString chartFilePath;

	QWidget* playWidget;
	/* Explain noteTracks:
		In one game, notes are initialized and stored in noteTracks.
		When a note is judged, it will be moved out
		AND moved in noteOutTracks, in that case we don't need
		to construct every note again.
	*/
	QQueue<Note*>noteTracks[4];
	QQueue<Note*>noteOutTracks[4];

	QParallelAnimationGroup* noteAnimationGroup;

	QMediaPlayer musicPlayer;
	QAudioOutput audioOutput;
	QSoundEffect noteSound;

	QTimer timer;
	QTimer musicCountdownTimer;
};
