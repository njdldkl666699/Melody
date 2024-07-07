#pragma once

#include <QWidget>
#include"GameController.h"
#include "ui_EndWidget.h"
#include<QMovie>

class EndWidget : public QWidget
{
	Q_OBJECT

public:
	EndWidget(GameController* game, QWidget* parent = nullptr);
	~EndWidget();

signals:
	void signalBackMenu();
	void signalRestart();

private:
	void initBackgroundGIF();
	void initScoreList();
	void initChartIntro();
	void setScore(int bestNum, int coolNum, int goodNum, int missNum, int comboNum, int accNum, int score);
	void showScore();
	void showRank();

private slots:
private:
	int bestNum;
	int coolNum;
	int goodNum;
	int missNum;
	int comboNum;
	int accNum;
	int score;

	Ui::EndWidgetClass ui;
	GameController* gameController;
	QMovie* backgroundGIF;
};
