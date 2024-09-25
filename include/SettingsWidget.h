/*
	<Header file of class SettingsWidget>
	Copyright (C) <2024>  <Meolide Team>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <QWidget>
#include<QPixmap>
#include<QFile>
#include<QTextStream>
#include "ui_SettingsWidget.h"

//Singleton Class
class SettingsWidget : public QWidget
{
	Q_OBJECT

private:
	SettingsWidget(QWidget* parent = nullptr);
	SettingsWidget(const SettingsWidget&) = delete;
	SettingsWidget& operator=(const SettingsWidget&) = delete;
	~SettingsWidget(){}

	class Destructor
	{
	public:
		~Destructor()
		{
			if (settingsWidget != nullptr)
			{
				delete settingsWidget;
				settingsWidget = nullptr;
			}
		}
	};

public:
	//Singleton Getter
	static SettingsWidget* instance()
	{
		if (settingsWidget == nullptr)
		{
			settingsWidget = new SettingsWidget();
		}
		return settingsWidget;
	}

public:
	//Data Members' Getters
	constexpr const int getMusicVal() const { return musicVal; }
	constexpr const int getSoundVal() const { return soundVal; }
	constexpr const int getSpeedVal() const { return speedVal; }
	constexpr const int getBiasVal() const { return biasVal; }
	constexpr const int getFpsVal() const { return fpsVal; }
	const QKeySequence getKey_1() const { return key_1; }
	const QKeySequence getKey_2() const { return key_2; }
	const QKeySequence getKey_3() const { return key_3; }
	const QKeySequence getKey_4() const { return key_4; }
	constexpr const bool getFullscreen() const { return isFullscreen; }

signals:
	void pushButtonBackMenuClicked();

private:
	void initBackground();
	void initSettings();
	void initUIValues();
	void initWindow();

private slots:
	void saveSettings();
	void onKeyEdited(const QKeySequence& keySequence);

private:
	static SettingsWidget* settingsWidget;
	static Destructor destructor;

	Ui::SettingsWidgetClass ui;
	QPixmap backgroundPic;

	//Settings Data Members
	int musicVal, soundVal, speedVal, biasVal, fpsVal;
	QKeySequence key_1, key_2, key_3, key_4;
	bool isFullscreen;
};
