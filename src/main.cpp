#include <QApplication>
#include<QStyleFactory>
#include "StartWidget.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	a.setStyle(QStyleFactory::create("Fusion"));
	StartWidget s;
	//s.installEventFilter(&s); // ��װ�¼�������
	s.show();
	//w.show();
	return a.exec();
}
