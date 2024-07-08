#pragma once

#include <QWidget>
#include<QKeyEvent>
#include "ui_PlayWidget.h"
#include"EndWidget.h"
#include"PauseWidget.h"
#include"SettingsWidget.h"
#include"GameController.h"

class PlayWidget : public QWidget
{
	Q_OBJECT

public:
	PlayWidget(const QString& songFilePth, const QString& chartFilePth,
		const SettingsWidget* settingsWidget, QWidget* parent = nullptr);

	~PlayWidget();

signals:
	void signalBackMenu();
	void signalKeyPressed(QKeyEvent* event);
	void signalKeyReleased(QKeyEvent* event);

protected:
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

private:
	void initPlayWidget();

private slots:
	void gamePause();
	void gameContinue();
	void gameRestart();
	void gameClose();
	void gameEnd();

private:
	Ui::PlayWidgetClass ui;
	GameController* gameController;
	PauseWidget* pauseWidget;
	EndWidget* endWidget;

	const SettingsWidget* settings;
	const QString& songFilePath;
	const QString& chartFilePath;
};
