#pragma once

#include "SettingsWidget.h"
#include "Hold.h"
#include "Tap.h"
#include <QKeyEvent>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
#include <QTimer>

#include <QQueue>
#include <QVector>

class GameController : public QObject
{
	Q_OBJECT

public:
	GameController(const QString& songFilePth, const QString& chartFilePth,
		const SettingsWidget* settingsWidget, QObject* parent = nullptr);
	~GameController();
	//## Only Use in PlayWidget.cpp ##
	void reset();
	void wait();
	void setNoteParent(QWidget* parent);

public:
	//get functions
	uint getPerfectCount() const { return perfectCount; }
	uint getGoodCount() const { return goodCount; }
	uint getMissCount() const { return missCount; }
	float getAccuracy() const { return accuracy; }
	uint getScore() const { return score; }
	uint getCombo() const { return combo; }
	uint getMaxCombo() const { return maxCombo; }
	QString getSongName()const;
	QString getChartName()const;
	QPixmap getSongPicture()const;

signals:
	void gameEnded();
	void signalUpdate();
	void judgeResult(const QString& comment);

public slots:
	//judge related
	void judgeKeyPress(QKeyEvent* event);
	void judgeKeyRelease(QKeyEvent* event);
	//game related
	void gamePause();
	void gamePlay();

private:
	//init functions
	void initVals();
	void initnoteTracks();
	void initMusicPlayer();

	//get Unbiased Note Time (ms)
	int getNoteTime(const QString& rawTimeData)const;
	//get Music Current Time (ms)
	int getMusicCurrentTime()const { return musicPlayer.position(); }
	void calculateAcc();
	//music related
	void isMusicEnd(QMediaPlayer::MediaStatus status);
	//clear note memory
	void clear();

private slots:
	void judgeNoHitMiss();
	void updateNote();

private:
	uint perfectCount, goodCount, missCount;
	float accuracy;
	uint score;
	uint combo, maxCombo;

	float bpm;
	int offset;
	float velocity;	//pixel per ms
	int deltaTime;
	int waitTime;

	const SettingsWidget* settings;
	const QString songFilePath;
	const QString chartFilePath;
	QString key[4];

	QQueue<Note*>noteTracks[4];
	QVector<Note*>notesOutQueue;
	QWidget* noteParent;

	QMediaPlayer musicPlayer;
	QAudioOutput audioOutput;
	QSoundEffect tapSound;

	QTimer timer;
	QTimer countdownTimer;
};
