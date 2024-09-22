#pragma once

#include <QWidget>
#include<QKeyEvent>
#include <QDir>
#include<QMovie>
#include<QFile>
#include"SettingsWidget.h"
#include"PlayWidget.h"
#include "confirmDialog.h"
#include "ui_MenuWidget.h"

class MenuWidget : public QWidget
{
	Q_OBJECT

public:
	MenuWidget(QWidget* parent = nullptr);
	~MenuWidget();

protected:
	void keyPressEvent(QKeyEvent* event) override;

private slots:
	void onComboBoxSongSelected(const QString& songName);
	void onPushButtonPlayClicked();

private:
	void initSongComboBox();
	void initBackgroundGIF();
	void initLogoGIF();

private:
	Ui::MenuWidgetClass ui;
	QMovie* backgroundGIF;
	QMovie* logoGIF;

	PlayWidget* playWidget;
	ConfirmDialog* confirmDialog;

	QMediaPlayer* menuMusicPlayer;
	QAudioOutput* menuAudio;

	QDir beatmapDir, songDir;
	// Used to init PlayWidget
	QString songFilePath, chartFilePath;
};
