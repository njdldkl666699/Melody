#pragma once

#include <QObject>
#include "SettingsWidget.h"
#include"Tap.h"
#include"Hold.h"
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QQueue>
#include<QVector>
#include<QDir>
#include<QFile>
#include<QFileInfo>
#include<QTextStream>
#include<QKeyEvent>

class GameController : public QObject
{
	Q_OBJECT
		
public:
	GameController(const QString& songFilePth, const QString& chartFilePth,
		const SettingsWidget* settingsWidget, QObject* parent = nullptr);
	~GameController();
	//## Only Use in PlayWidget.cpp ##
	void reset();

public:
	//get functions
	uint getPerfectCount() const { return perfectCount; }
	uint getGoodCount() const { return goodCount; }
	uint getMissCount() const { return missCount; }
	float getAccuracy() const { return accuracy; }
	uint getScore() const { return score; }
	uint getMaxCombo() const { return maxCombo; }
	QString getSongName()const;
	QString getChartName()const;
	QPixmap getSongPicture()const;

signals:
	void musicEnded();

public slots:
	//judge related
	void judgeKeyPress(QKeyEvent* event);
	void judgeKeyRelease(QKeyEvent* event);
	//music related
	void isMusicEnd(QMediaPlayer::MediaStatus status);
	void pauseMusic() { musicPlayer.pause(); }
	void playMusic() { musicPlayer.play(); }
	void stopMusic() { musicPlayer.stop(); }

private:
	//init functions
	void initData();
	void initnoteTracks();
	void initMusicPlayer();

	//get Unbiased Note Time (ms)
	int getNoteTime(const QString& rawTimeData)const;
	//get Music Current Time (ms)
	int getMusicCurrentTime()const { return musicPlayer.position(); }

private slots:
	void judgeNoHitMiss();
	void drawNote();

private:
	uint perfectCount, goodCount, missCount;
	float accuracy;
	uint score;
	uint combo, maxCombo;

	float bpm;
	int offset;

	const SettingsWidget* settings;
	const QString songFilePath;
	const QString chartFilePath;
	QQueue<Note*>noteTracks[4];
	QString key[4];
	QMediaPlayer musicPlayer;
	QAudioOutput audioOutput;

	QVector<Note*>notesOutQueue;
};
