#pragma once

#include <QObject>
#include "SettingsWidget.h"
#include"Tap.h"
#include"Hold.h"
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QQueue>
#include<QDir>
#include<QFile>
#include<QFileInfo>
#include<QTextStream>

class PlayWidget;
class GameController : public QObject
{
	Q_OBJECT

public:
	GameController(const QString& songFilePth, const QString& chartFilePth,
		const SettingsWidget* settingsWidget, QObject* parent = nullptr);
	~GameController();

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

private:
	//init functions
	void initnoteTracks();
	void initMediaPlayer();

	//get Unbiased Note Time (ms)
	int getNoteTime(const QString& rawTimeData)const;
	//get Music Current Time (ms)
	int getMusicCurrentTime()const;

private slots:
	//void judge();

private:
	uint perfectCount, goodCount, missCount;
	float accuracy;
	uint score;
	uint combo, maxCombo;

	int bpm, offset;

	const SettingsWidget* settings;
	const QString songFilePath;
	const QString chartFilePath;
	QQueue<Note*>noteTracks[4];
	QMediaPlayer musicPlayer;
	QAudioOutput audioOutput;
};
