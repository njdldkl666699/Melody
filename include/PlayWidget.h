#pragma once

#include <QWidget>
#include "ui_PlayWidget.h"

class PlayWidget : public QWidget
{
	Q_OBJECT

public:
	PlayWidget(QWidget *parent = nullptr);
	PlayWidget(const QString& songFileName, const QString& chartFileName, QWidget* parent = nullptr);
	~PlayWidget();

private:
	Ui::PlayWidgetClass ui;
};
