#pragma once

#include <QWidget>
#include "ui_EndWidget.h"


class EndWidget : public QWidget
{
	Q_OBJECT

public:
	EndWidget(QWidget *parent = nullptr);
	~EndWidget();

private:
	Ui::EndWidgetClass ui;
};
