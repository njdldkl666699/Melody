#pragma once

#include <QWidget>
#include<QKeyEvent>
#include <QDir>
#include<QMovie>
#include<QFile>
#include"SettingsWidget.h"
#include"PlayWidget.h"
#include "ui_MenuWidget.h"

class MenuWidget : public QWidget
{
	Q_OBJECT

public:
	MenuWidget(QWidget* parent = nullptr);
	~MenuWidget();
	QString getChartFilePath() const
	{
		return chartFilePath;
	}


protected:
	void keyPressEvent(QKeyEvent* event) override;

private slots:
	void comboBoxSongSelected(const QString& songName);

	void pushButtonPlayClicked();

private:
	void initSongComboBox();
	void initMenuWidget();
	void initBackgroundGIF();

private:
	Ui::MenuWidgetClass ui;
	QMovie* backgroundGIF;

	SettingsWidget* settingsWidget;
	PlayWidget* playWidget;

	QDir beatmapDir, songDir;

	// Used to init PlayWidget
	QString songFilePath, chartFilePath;
};
