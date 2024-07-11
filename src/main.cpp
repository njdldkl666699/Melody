#include <QApplication>
#include<QStyleFactory>
#include "StartWidget.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	a.setStyle(QStyleFactory::create("Fusion"));
	StartWidget s;
	//s.installEventFilter(&s); // 安装事件过滤器
	s.show();
	//w.show();
	return a.exec();
}
