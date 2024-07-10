#pragma once

#include <QWidget>
#include"GameController.h"
#include "ui_EndWidget.h"
#include<QMovie>
#include<QMediaPlayer>
#include<QVideoWidget>
#include<QAudioOutput>


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
	void initBackgroundGIF();
	void initScoreList();
	void initChartIntro(const GameController*);
	void setScore(int bestNum,  int goodNum, int missNum, int comboNum, int accNum, int score);
	void showScore();
	void showRank();
	void musicSet();
	void musicPlay();
	void toolTips();
private slots:
private:
	int bestNum;
	int coolNum;
	int goodNum;
	int missNum;
	int comboNum;
	int accNum;
	int score;
	int rank; //1:Fai 2:BV£¨À¶V 3:WV(°×V 4:A 5: B 6:C 7£ºF

	QList<QUrl> musicList;
	QMediaPlayer* player;
	QMediaPlayer* music;
	QAudioOutput* audio;

	Ui::EndWidgetClass ui;
	const GameController* gameController;
	QMovie* backgroundGIF;
};
