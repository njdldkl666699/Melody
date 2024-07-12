#include "StartWidget.h"
#include "ui_StartWidget.h"
#include<QMediaPlayer>
#include<QVideoWidget>
#include<QAudioOutput>
#ifdef _DEBUG
#include<QDebug>
#endif // _DEBUG
#include <QList>
#include <QUrl>
#include<QTimer>

StartWidget::StartWidget(QWidget* parent)
	: QWidget(parent), ui(new Ui::StartWidget)
{
	ui->setupUi(this);
	w = new MenuWidget(nullptr);
	setWindowTitle("Meolide");

	mediaSet();
	mediaPlay();

}

StartWidget::~StartWidget()
{
	delete ui;
	delete w;
}

void StartWidget::mediaSet()
{
	ifLoopStart = 0;
	player = new QMediaPlayer(this);
	videoWidget = new QVideoWidget(this);
	audio = new QAudioOutput(this);
	player->setAudioOutput(audio);
	audio->setVolume(0.5);
	videoWidget->resize(ui->label->size());
	videoWidget->show();
	//视频播放列表
	mediaList = {
		QUrl::fromLocalFile("./res/video/gameStart1.mp4"),
		QUrl::fromLocalFile("./res/video/gameStart 21.mp4"),
		QUrl::fromLocalFile("./res/video/gameStart 22.mp4"),
		QUrl::fromLocalFile("./res/video/gameStart23.mp4")
	};
	  // 初始化播放列表索引
	currentMediaIndex = 0;
	  // 连接信号以在视频结束时自动播放下一个视频
	connect(player, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status)
		{
			if (status == QMediaPlayer::EndOfMedia)
			{
				if (currentMediaIndex < 2)
				{
					currentMediaIndex++;
					mediaPlay();
				}
				else if (currentMediaIndex < 3)
				{
					currentMediaIndex--;
					mediaPlay();
				}
			}
		 });

	// 播放视频
	player->play();
}

void StartWidget::mediaPlay()
{
	QUrl currentMedia(mediaList.at(currentMediaIndex));
	player->setSource(currentMedia);
	player->setVideoOutput(videoWidget);

	//player->setVolume(100);
	player->play();
	if (currentMediaIndex > 0)
	{
		musicLoop();
	}
}

void StartWidget::next()
{
	qDebug() << "next";
}

void StartWidget::keyPressEvent(QKeyEvent* event)
{
	qDebug() << "next";
	if (player->position() > 3000)
	{
		if (currentMediaIndex == 0)
		{
			currentMediaIndex++;
			mediaPlay();
		}
	}
	else if (currentMediaIndex == 1 || currentMediaIndex == 2)
	{
		currentMediaIndex = 3;
		mediaPlay();
		QTimer::singleShot(1000, this, &StartWidget::gameStart);

	}
}

void StartWidget::gameStart()
{
	this->close();
	music->stop();
	w->show();
}

void StartWidget::musicLoop()
{
	if (ifLoopStart == 0)
	{
		music = new QMediaPlayer(this);
		audioLoop = new QAudioOutput(this);
		music->setAudioOutput(audioLoop);
		audioLoop->setVolume(0.5);
		music->setSource(QUrl::fromLocalFile("./res/video/gameStart2music.MP3"));
		music->play();
		ifLoopStart = 1;
	}
}