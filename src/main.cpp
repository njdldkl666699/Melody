#include <QApplication>
#include<QStyleFactory>
#include "StartWidget.h"
//#include "MenuWidget.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	a.setStyle(QStyleFactory::create("Fusion"));
    StartWidget s;
	s.show();
	/*MenuWidget w;
	w.show();*/

	//qt.multimedia.plugin: Qt Multimedia requires a QCoreApplication instance
	//������ȫ��ButtonClickSound������

	return a.exec();
}
