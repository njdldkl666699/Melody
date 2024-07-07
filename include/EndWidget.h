#pragma once

#include <QWidget>
#include"GameController.h"
#include "ui_EndWidget.h"

class EndWidget : public QWidget
{
	Q_OBJECT

public:
	EndWidget(GameController* game, QWidget* parent = nullptr);
	~EndWidget();

signals:
	void signalBackMenu();
	void signalRestart();

private:
	void initEndWidget();
private slots:

private:
	Ui::EndWidgetClass ui;
	GameController* gameController;
};
