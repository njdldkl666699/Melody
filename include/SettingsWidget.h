#pragma once

#include <QWidget>
#include <QIcon>
#include <QPixmap>
#include <QBitmap>
#include <QPalette>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSize>
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
	QPushButton* pushButton_backMenu;
	
	

	QLabel* Label_background_settings;		//”√”⁄œ‘ æ±≥æ∞Õº∆¨

	
};
