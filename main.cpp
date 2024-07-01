#include "menuwidget.h"

#include <QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	menuWidget w;
	w.show();
	return a.exec();
}
