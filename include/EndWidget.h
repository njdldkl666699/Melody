#pragma once

#include <QWidget>
#include"GameController.h"
#include "ui_EndWidget.h"
#include<QMovie>
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QDateTime>

class EndWidget : public QWidget
{
	Q_OBJECT

public:
	EndWidget(const GameController* game, QWidget* parent = nullptr);
	~EndWidget();

signals:
	void signalBackMenu();
	void signalRestart();

private:
	void initWindow();
	void initUI();
	void initScore();
	void initRank();

	// ###implement later###
	void playAnimation();

	void playMusic();

private:
	struct HistoryList
	{
		QString dateTime;
		QString score;
		QString acc;
		QString rank;
		QString maxCombo;
	};

	void historyOn();
	void getHistory();
	void writeHistory();
	void setHistoryList();

private:
	const GameController* gameController;
	uint perfectNum, goodNum, badNum, missNum;
	uint score, maxCombo;
	float acc;

	enum Rank
	{
		phi, //All Perfect
		BlueV, //Full Combo, regardless of accuracy
		V, //acc >= 99.00% , delta = 1.0%
		S, //97.50% <= acc < 99.00% , delta = 2.5%
		A, //95.00% <= acc < 97.50% , delta = 2.5%
		B, //90.00% <= acc < 95.00% , delta = 5.0%
		C, //85.00% <= acc < 90.00% , delta = 5.0%
		F  //acc < 85.00%
	};
	Rank rank;
	QString rankStr;

private:
	Ui::EndWidgetClass ui;
	QMovie* backgroundGIF;

	QMediaPlayer* player;
	QAudioOutput* audio;

	QString filename;
	bool ifHistoryOn;
	QDir dir;
	QVector<HistoryList> historyList;
};
