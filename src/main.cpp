#include <QApplication>
#include<QStyleFactory>
#include<QFontDatabase>
#include"ButtonClickSound.h"

#ifdef _DEBUG
#include"MenuWidget.h"
#else 
#include"StartWidget.h"
#endif // _DEBUG

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	a.setStyle(QStyleFactory::create("Fusion"));
	a.setAttribute(Qt::AA_EnableHighDpiScaling);
	QFontDatabase::addApplicationFont("./res/font/Saira-Regular.ttf");

	//qt.multimedia.plugin: Qt Multimedia requires a QCoreApplication instance
	//fix
	ButtonClickSound::sound = new QSoundEffect();
	ButtonClickSound::sound2 = new QSoundEffect();

#ifdef _DEBUG
	MenuWidget w;
	w.show();
#else
    StartWidget s;
	s.show();
#endif // _DEBUG

	return a.exec();
}
