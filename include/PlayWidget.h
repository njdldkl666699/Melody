/*
	<Header file of class PlayWidget>
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

#include"EndWidget.h"
#include"GameController.h"
#include"PauseWidget.h"
#include"SettingsWidget.h"
#include "ui_PlayWidget.h"

class PlayWidget : public QWidget
{
	Q_OBJECT

public:
	PlayWidget(const QString& songFilePth, 
		const QString& chartFilePth, QWidget* parent = nullptr);
	~PlayWidget();

signals:
	void signalBackMenu();
	void signalKeyPressed(QKeyEvent* event);
	void signalKeyReleased(QKeyEvent* event);

protected:
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

private:
	void initPlayWidget();

private slots:
	void gamePause();
	void gameContinue();
	void gameRestart();
	void gameClose();
	void gameEnd();
	void updateUI();
	void updateComment(const QString& comment);
	void updateCountDown();

private:
	Ui::PlayWidgetClass ui;
	GameController* gameController;
	PauseWidget* pauseWidget;
	EndWidget* endWidget;

	const SettingsWidget* settings;
	const QString& songFilePath;
	const QString& chartFilePath;

	QTimer commentTimer;
	QTimer countDownTimer;
	int countDownNum;
};
