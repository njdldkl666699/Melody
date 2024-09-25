/*
	<Header file of class StartWidget>
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

#include "MenuWidget.h"
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QAudioOutput>

class StartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StartWidget(QWidget *parent = nullptr );
    ~StartWidget();

private:
    void initMedia();
    void nextMedia();
	void playMedia0();
	void playMedia1();
    void gameStart();

protected:
    virtual void keyPressEvent(QKeyEvent* event)override;
    virtual void mousePressEvent(QMouseEvent* event)override;

private:
	MenuWidget* w;

    QMediaPlayer* player;
	QMediaPlayer* loopAudioPlayer;
	QMediaPlayer* loopVideoPlayer;

    QVideoWidget* videoWidget;
    QAudioOutput* audio;

    QTimer nextTimer;
    int nextIndex;
};


