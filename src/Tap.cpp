#include "Tap.h"

Tap::Tap(int strtTime, QKeySequence ky, QLabel* parent)
	: Note(strtTime, ky, parent)
{

}

Tap::~Tap()
{}

bool Tap::judge()
{
	//to be implemented
	return true;
}

void Tap::keyPressEvent(QKeyEvent* ev)
{
	qDebug() << key.toString() << '\n' << ev->text();
	if (key.toString() == ev->text())
	{
		qDebug() << "Key Pressed!\n";
	}
	else
	{
		return QWidget::keyPressEvent(ev);
	}
}
