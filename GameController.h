#pragma once

#include <QObject>

class GameController  : public QObject
{
	Q_OBJECT

public:
	GameController(QObject *parent);
	~GameController();
};
