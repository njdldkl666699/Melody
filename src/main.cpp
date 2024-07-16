#include <QApplication>
#include<QStyleFactory>
#include "StartWidget.h"
#include"ButtonClickSound.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	a.setStyle(QStyleFactory::create("Fusion"));

	//qt.multimedia.plugin: Qt Multimedia requires a QCoreApplication instance
	//fix
	ButtonClickSound::sound = new QSoundEffect();
	ButtonClickSound::sound2 = new QSoundEffect();

    StartWidget s;
	s.show();

	return a.exec();
}
