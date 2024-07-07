#pragma once

#include <QObject>
#include "SettingsWidget.h"
#include"Tap.h"
#include"Hold.h"
#include<QMediaPlayer>
#include<QQueue>
#include<QFile>
#include<QTextStream>

class GameController : public QObject
{
	Q_OBJECT

public:
	GameController(const QString& songFilePath, const QString& chartFilePath,
		const SettingsWidget* settings, QObject* parent = nullptr);
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
	uint perfectCount, goodCount, missCount;
	float accuracy;
	uint score;
	uint combo, maxCombo;

	QFile chartFile;
	QFile songFile;
	QTextStream in;
};
