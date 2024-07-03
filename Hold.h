#pragma once

#include <QObject>

class Hold  : public QObject
{
	Q_OBJECT

public:
	Hold(QObject *parent);
	~Hold();
};
