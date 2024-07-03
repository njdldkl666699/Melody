#pragma once

#include <QWidget>
#include "ui_ChartSelectWidget.h"

class ChartSelectWidget : public QWidget
{
	Q_OBJECT

public:
	ChartSelectWidget(QWidget *parent = nullptr);
	~ChartSelectWidget();

private:
	Ui::ChartSelectWidgetClass ui;
};
