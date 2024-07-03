#pragma once

#include <QWidget>
#include "ui_PauseWidget.h"

class PauseWidget : public QWidget
{
	Q_OBJECT

public:
	PauseWidget(QWidget *parent = nullptr);
	~PauseWidget();

private:
	Ui::PauseWidgetClass ui;
};
