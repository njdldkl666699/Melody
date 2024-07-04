#pragma once

#include <QObject>

class Tap  : public QObject
{
	Q_OBJECT

public:
	Tap(QObject *parent);
	~Tap();

private:
};
