#include "ButtonClickSound.h"
#include<QObject>

QSoundEffect ButtonClickSound::sound = QSoundEffect();
QSoundEffect ButtonClickSound::sound2 = QSoundEffect();

void ButtonClickSound::buttonClickSound(QPushButton* button)
{
	ButtonClickSound::sound.setSource(QUrl::fromLocalFile("./res/click.wav"));
	ButtonClickSound::sound.setVolume(1);
	connect(button, &QPushButton::clicked, &sound, &QSoundEffect::play);
	//qDebug() << "sound";
}

void ButtonClickSound::buttonClickSound(QComboBox* button)
{
	ButtonClickSound::sound2.setSource(QUrl::fromLocalFile("./res/click2.WAV"));
	ButtonClickSound::sound2.setVolume(0.6);
	connect(button, &QComboBox::highlighted, &sound2, &QSoundEffect::play);
	//qDebug() << "sound";
}

void ButtonClickSound::buttonClickSound(QSlider* button)
{
	ButtonClickSound::sound2.setSource(QUrl::fromLocalFile("./res/click2.wav"));
	ButtonClickSound::sound2.setVolume(0.6);
	connect(button, &QSlider::sliderMoved, &sound2, &QSoundEffect::play);
	//qDebug() << "sound";
}

void ButtonClickSound::buttonClickSound(QKeySequenceEdit* button)
{
	ButtonClickSound::sound.setSource(QUrl::fromLocalFile("./res/click.wav"));
	ButtonClickSound::sound.setVolume(1);
	connect(button, &QKeySequenceEdit::keySequenceChanged, &sound, &QSoundEffect::play);

 }
