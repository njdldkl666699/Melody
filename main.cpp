#include "menuwidget.h"

#include <QApplication>
#include <QString>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    menuWidget w;
    QString str = "hello.\n";
    qDebug() << str;



    w.show();
    return a.exec();//end
}
