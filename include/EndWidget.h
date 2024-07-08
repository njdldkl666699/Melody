#pragma once

#include <QWidget>
#include"GameController.h"
#include "ui_EndWidget.h"
#include<QMovie>

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
	const GameController* gameController;
	QMovie* backgroundGIF;
};
