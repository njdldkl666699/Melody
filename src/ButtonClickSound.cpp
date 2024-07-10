#include "ButtonClickSound.h"
#include<QObject>


QSoundEffect ButtonClickSound::sound = QSoundEffect();

void ButtonClickSound::buttonClickSound(QPushButton* button)
{
	ButtonClickSound::sound.setSource(QUrl::fromLocalFile("./res/click.wav"));
	ButtonClickSound::sound.setVolume(1);
	connect(button, &QPushButton::clicked, &sound, &QSoundEffect::play);
	//qDebug() << "sound";
}

void ButtonClickSound::buttonClickSound(QComboBox* button)
{
	ButtonClickSound::sound.setSource(QUrl::fromLocalFile("./res/click.wav"));
	ButtonClickSound::sound.setVolume(1);
	connect(button, &QComboBox::highlighted, &sound, &QSoundEffect::play);
	//qDebug() << "sound";
}

void ButtonClickSound::buttonClickSound(QSlider* button)
{
	ButtonClickSound::sound.setSource(QUrl::fromLocalFile("./res/click.wav"));
	ButtonClickSound::sound.setVolume(1);
	connect(button, &QSlider::sliderMoved, &sound, &QSoundEffect::play);
	//qDebug() << "sound";
}

void ButtonClickSound::buttonClickSound(QKeySequenceEdit* button)
{
	ButtonClickSound::sound.setSource(QUrl::fromLocalFile("./res/click.wav"));
	ButtonClickSound::sound.setVolume(1);
	connect(button, &QKeySequenceEdit::keySequenceChanged, &sound, &QSoundEffect::play);

 }
