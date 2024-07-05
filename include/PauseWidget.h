#pragma once

#include <QWidget>
#include "ui_PauseWidget.h"

class PauseWidget : public QWidget
{
	Q_OBJECT

public:
	PauseWidget(QWidget* parent = nullptr);
	~PauseWidget();

signals:
	void signalBackMenu();
	void signalRestart();
	void signalContinue();

private:
	Ui::PauseWidgetClass ui;
};
