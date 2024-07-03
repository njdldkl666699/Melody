#pragma once

#include <QWidget>
#include "ui_GameController.h"

//note class 
/*
* Tap
* Hold
*/

class GameController : public QWidget
{
	Q_OBJECT

public:
	GameController(QWidget* parent = nullptr);
	~GameController();

private:
	Ui::GameControllerClass ui;
};
