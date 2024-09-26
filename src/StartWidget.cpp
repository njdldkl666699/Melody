/*
	<Source file of class StartWidget>
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

#include "StartWidget.h"
#include "UIController.h"
#include <QTimer>

#ifdef _DEBUG
#include<QDebug>
#endif // _DEBUG

StartWidget::StartWidget(QWidget* parent)
	: QWidget(parent)
{
	w = new MenuWidget(nullptr);
	setWindowTitle(QString::fromLocal8Bit("ÒôÁé»ÃÕÂMeolide"));
	setWindowIcon(QIcon("./res/icon/icon.ico"));
	setFixedSize(1280, 720);
	UICtrl::setIfFullscreen(this, SettingsWidget::instance()->getFullscreen());

	initMedia();
	playMedia0();
}

StartWidget::~StartWidget()
{
	delete w;
}

void StartWidget::initMedia()
{
	//Set Video Output
	videoWidget = new QVideoWidget(this);
	videoWidget->resize(this->size());
	videoWidget->show();

	//Init Audio Output
	audio = new QAudioOutput(this);
	audio->setVolume(SettingsWidget::instance()->getMusicVal() / 100.f);

	//Init Media Player
	player = new QMediaPlayer(this);

	//Init Loop Audio Player
	loopAudioPlayer = new QMediaPlayer(this);
	loopAudioPlayer->setVideoOutput(nullptr);
	loopAudioPlayer->setLoops(QMediaPlayer::Infinite);

	//Init Loop Video Player
	loopVideoPlayer = new QMediaPlayer(this);
	loopVideoPlayer->setAudioOutput(nullptr);
	loopVideoPlayer->setLoops(QMediaPlayer::Infinite);

	//Init Next Related
	nextIndex = 0;
	nextTimer.setInterval(1000);
	nextTimer.setSingleShot(true);
}

void StartWidget::playMedia0()
{
	// The "Strange" video, with its audio
	player->setSource(QUrl::fromLocalFile("./res/video/gameStart1.mp4"));
	player->setVideoOutput(videoWidget);
	player->setAudioOutput(audio);
	player->play();

	connect(player, &QMediaPlayer::mediaStatusChanged, this, 
		[this](QMediaPlayer::MediaStatus status) {
			if (status == QMediaPlayer::EndOfMedia)
				playMedia1();
		});

	nextTimer.start();
	nextIndex++;
}

void StartWidget::playMedia1()
{
	// The transition video, with looped music
	player->stop();
	player = new QMediaPlayer(this);
	player->setSource(QUrl::fromLocalFile("./res/video/gameStart2.mp4"));
	player->setVideoOutput(videoWidget);
	player->setAudioOutput(nullptr);
	player->play();

	loopAudioPlayer->setSource(QUrl::fromLocalFile("./res/video/startMusic.mp3"));
	loopAudioPlayer->setAudioOutput(audio);
	loopAudioPlayer->play();

	// The Meolide video, with looped music already playing
	connect(player, &QMediaPlayer::mediaStatusChanged, this, 
		[this](QMediaPlayer::MediaStatus status){
			if (status == QMediaPlayer::EndOfMedia)
			{
				player->stop();
				loopVideoPlayer->setSource(QUrl::fromLocalFile("./res/video/gameStart3.mp4"));
				loopVideoPlayer->setVideoOutput(videoWidget);
				loopVideoPlayer->play();
			}
		});

	nextTimer.start(1000);
	nextIndex++;
}

void StartWidget::gameStart()
{
	loopAudioPlayer->stop();
	loopVideoPlayer->stop();
	this->close();
	w->show();
}

void StartWidget::nextMedia()
{
	qDebug() << "nextIndex: " << nextIndex;
	// Always wait for 1s to change audio & video
	if (nextTimer.isActive())
	{
		return;
	}
	// If press any key, jump to next a&v.
	switch (nextIndex)
	{
	case 1:
		playMedia1();
		break;
	case 2:
		gameStart();
		break;
	default:
		break;
	}
}

void StartWidget::keyPressEvent(QKeyEvent* event)
{
	this->nextMedia();
	return QWidget::keyPressEvent(event);
}

void StartWidget::mousePressEvent(QMouseEvent* event)
{
	this->nextMedia();
	return QWidget::mousePressEvent(event);
}
