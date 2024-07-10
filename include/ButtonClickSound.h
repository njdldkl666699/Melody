#pragma once
#include<QSoundEffect>
#include<QPushButton>
#include <QObject>
#include<QComboBox>

class ButtonClickSound :public QObject
{
	Q_OBJECT
public:
    static void buttonClickSound(QPushButton* button);
	static void buttonClickSound(QComboBox* button);

	static QSoundEffect sound;

};

