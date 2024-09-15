#pragma once

#include"MenuWidget.h"
#include <QWidget>
#include<QMediaPlayer>
#include<QVideoWidget>
#include<QKeyEvent>
#include <QList>
#include <QUrl>
#include<QAudioOutput>

namespace Ui {
class StartWidget;
}

class StartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StartWidget(QWidget *parent = nullptr );
    void mediaSet();
    void mediaPlay();
    ~StartWidget();
    void next();
    virtual void keyPressEvent(QKeyEvent* event);
    void musicLoop();
    void gameStart();

private slots:

private:
    Ui::StartWidget *ui;
    QMediaPlayer* player;
    QMediaPlayer* music;
    QAudioOutput* audio;
    QVideoWidget* videoWidget;
    QList<QUrl> mediaList;

    QAudioOutput* audioLoop;
    int currentMediaIndex;
    bool ifLoopStart;
	MenuWidget* w;
};


