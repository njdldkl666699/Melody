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


