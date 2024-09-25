/*
	<Header file of class MenuWidget>
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

#include <QWidget>
#include<QKeyEvent>
#include <QDir>
#include<QMovie>
#include<QFile>
#include"SettingsWidget.h"
#include"PlayWidget.h"
#include "confirmDialog.h"
#include "ui_MenuWidget.h"

class MenuWidget : public QWidget
{
	Q_OBJECT

public:
	MenuWidget(QWidget* parent = nullptr);
	~MenuWidget();

protected:
	void keyPressEvent(QKeyEvent* event) override;

private slots:
	void onComboBoxSongSelected(const QString& songName);
	void onPushButtonPlayClicked();

private:
	void initSongComboBox();
	void initBackgroundGIF();
	void initLogoGIF();

private:
	Ui::MenuWidgetClass ui;
	QMovie* backgroundGIF;
	QMovie* logoGIF;

	PlayWidget* playWidget;
	ConfirmDialog* confirmDialog;

	QMediaPlayer* menuMusicPlayer;
	QAudioOutput* menuAudio;

	QDir beatmapDir, songDir;
	// Used to init PlayWidget
	QString songFilePath, chartFilePath;
};
