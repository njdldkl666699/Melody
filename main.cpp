#include "menuwidget.h"

#include <QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	MenuWidget w;
	w.show();
	return a.exec();
}
