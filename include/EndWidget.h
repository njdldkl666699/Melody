/*
	<Header file of class EndWidget>
	Copyright (C) <2024>  <Meolide Team>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include"GameController.h"
#include "ui_EndWidget.h"
#include <QWidget>
#include<QMovie>
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QDir>

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
	void initHistory();

	void playAnimation();
	void playMusic();

	void historyOn();

private:
	const GameController* gameController;
	uint perfectNum, goodNum, badNum, missNum;
	int score, maxCombo;
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
};
