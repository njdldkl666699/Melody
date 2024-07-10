#pragma once
#include<QSoundEffect>
#include<QPushButton>
#include <QObject>
#include<QComboBox>
#include<QSlider>
#include<QKeySequenceEdit>


class ButtonClickSound :public QObject
{
	Q_OBJECT
public:
    static void buttonClickSound(QPushButton* button);
	static void buttonClickSound(QComboBox* button);
	static void buttonClickSound(QSlider* button);
	static void buttonClickSound(QKeySequenceEdit* button);

	static QSoundEffect sound;

};

