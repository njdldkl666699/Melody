#pragma once

#include "SettingsWidget.h"
#include "Hold.h"
#include "Tap.h"

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>

#include <QVector>
#include <QQueue>

#include <QTimer>
#include <QKeyEvent>

#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

class GameController : public QObject
{
	Q_OBJECT

public:
	GameController(const QString& songFilePth,
		const QString& chartFilePth, QObject* parent = nullptr);
	~GameController();
	//## Only Use in PlayWidget.cpp ##
	void reset();
	void wait();
	void setNoteParent(QWidget* parent) { playWidget = parent; }

public:
	//get functions
	uint getPerfectCount() const { return perfectCount; }
	uint getGoodCount() const { return goodCount; }

	// ####implement later###
	uint getBadCount()const { return 0; }

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

	/* Judge time(ms) table:
	Perfect: -50 ~ 50
	Good: -100 ~ -51, 51 ~ 100
	Bad: 100 ~ 120 (only Tap)
	Miss: < -100
	*/

	//judge related
	void judgeKeyPress(QKeyEvent* event);
	void judgeKeyRelease(QKeyEvent* event);
	//game related
	void gamePause();
	void gamePlay();

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

	//get Music Current Time (ms)
	int getMusicCurrentTime()const { return musicPlayer.position(); }
	void calculateAccAndScore();
	//music related
	void isMusicEnd(QMediaPlayer::MediaStatus status);
	//clear note memory
	void clear();

private slots:
	void judgeNoHitMiss();
	void updateNote();
	void amendNotePos(qint64 position);

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
	QQueue<Note*>noteTracks[4];
	QVector<Note*>notesOutQueue;

	QParallelAnimationGroup* noteInTracksAnimationGroup,
		* noteOutQueueAnimationGroup;

	QMediaPlayer musicPlayer;
	QAudioOutput audioOutput;
	QSoundEffect tapSound;

	QTimer timer;
	QTimer countdownTimer;
};
