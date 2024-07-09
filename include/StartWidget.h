#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>
#include<QMediaPlayer>
#include<QVideoWidget>
#include<QKeyEvent>
#include <QList>
#include <QUrl>
#include<QAudioOutput>
#include "MenuWidget.h"

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
    QVideoWidget* videoWidget;
    QList<QUrl> mediaList;
    QAudioOutput* audio;
    QAudioOutput* audioLoop;
    int currentMediaIndex;
    MenuWidget* w;
    bool ifLoopStart;
};

#endif // STARTWIDGET_H
