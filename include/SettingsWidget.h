#pragma once

#include <QWidget>
#include "ui_SettingsWidget.h"

class SettingsWidget : public QWidget
{
	Q_OBJECT

public:
	SettingsWidget(QWidget* parent = nullptr);
	~SettingsWidget();

signals:
	void pushButtonBackMenuClicked();

private:
	Ui::SettingsWidgetClass ui;
};
