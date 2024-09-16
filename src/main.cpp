#include <QApplication>
#include<QStyleFactory>
#include<QFontDatabase>
#include<QFile>
#include"SettingsWidget.h"

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
	QFontDatabase::addApplicationFont("./res/font/SourceHanSansSC-Regular-2.otf");

#ifdef _DEBUG
	MenuWidget w;
	w.show();
#else
    StartWidget s;
	s.show();
#endif // _DEBUG

	return a.exec();
}
