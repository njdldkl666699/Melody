#pragma once

#include <QWidget>
#include <QDir>
#include<QFile>
#include "ui_MenuWidget.h"

class MenuWidget : public QWidget
{
	Q_OBJECT

public:
	MenuWidget(QWidget *parent = nullptr);
	~MenuWidget();

	void initSongComboBox();

public slots:
	void comboBoxSongSelected(const QString& songName);

	void pushButtonPlayClicked();

private:
	Ui::MenuWidgetClass ui;
	QDir beatmapDir, songDir;

	// Used to init PlayWidget
	QString songFilePath, chartFilePath;
};
