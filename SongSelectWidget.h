#pragma once

#include <QWidget>
#include "ui_SongSelectWidget.h"

class SongSelectWidget : public QWidget
{
	Q_OBJECT

public:
	SongSelectWidget(QWidget *parent = nullptr);
	~SongSelectWidget();

private:
	Ui::SongSelectWidgetClass ui;
};
