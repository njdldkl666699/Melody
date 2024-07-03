#pragma once

#include <QWidget>
#include "ui_MenuWidget.h"

class MenuWidget : public QWidget
{
	Q_OBJECT

public:
	MenuWidget(QWidget *parent = nullptr);
	~MenuWidget();

private:
	Ui::MenuWidgetClass ui;
};
