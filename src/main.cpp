#include <QApplication>
#include<QStyleFactory>
#include "StartWidget.h"
#include"ButtonClickSound.h"
//#include "MenuWidget.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	a.setStyle(QStyleFactory::create("Fusion"));

	//qt.multimedia.plugin: Qt Multimedia requires a QCoreApplication instance
	//怀疑是全局ButtonClickSound的问题
	//fix
	ButtonClickSound::sound = new QSoundEffect();
	ButtonClickSound::sound2 = new QSoundEffect();

    StartWidget s;
	s.show();
	/*MenuWidget w;
	w.show();*/

	return a.exec();
}
