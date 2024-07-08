#pragma once

#include <QObject>
#include "SettingsWidget.h"
#include"Tap.h"
#include"Hold.h"
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QQueue>
#include<QFile>
#include<QTextStream>

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

private:
	//init functions
	void initnoteTracks();
	void initMediaPlayer();

	//get Unbiased Note Time (ms)
	int getNoteTime(const QString& rawTimeData)const;

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
