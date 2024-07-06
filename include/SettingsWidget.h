#pragma once

#include <QWidget>
#include<QPixmap>
#include<QFile>
#include<QTextStream>
#include "ui_SettingsWidget.h"

class SettingsWidget : public QWidget
{
	Q_OBJECT

public:
	SettingsWidget(QWidget* parent = nullptr);
	~SettingsWidget();

	//Data Members' Getters
	int getMusicVal() const { return musicVal; }
	int getSoundVal() const { return soundVal; }
	int getSpeedVal() const { return speedVal; }
	int getBiasVal() const { return biasVal; }
	int getFpsVal() const { return fpsVal; }
	QKeySequence getKey_1() const { return key_1; }
	QKeySequence getKey_2() const { return key_2; }
	QKeySequence getKey_3() const { return key_3; }
	QKeySequence getKey_4() const { return key_4; }

signals:
	void pushButtonBackMenuClicked();

private:
	void initBackground();
	void initSettings();
	void initUISettings();

private slots:
	void saveSettings();

private:
	Ui::SettingsWidgetClass ui;
	QPixmap backgroundPic;

	//Settings Data Members
	int musicVal, soundVal, speedVal, biasVal, fpsVal;
	QKeySequence key_1, key_2, key_3, key_4;
};
