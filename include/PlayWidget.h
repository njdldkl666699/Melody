#pragma once

#include <QWidget>
#include "ui_PlayWidget.h"
#include"EndWidget.h"
#include"PauseWidget.h"
#include"SettingsWidget.h"
#include"GameController.h"

class PlayWidget : public QWidget
{
	Q_OBJECT

public:
	PlayWidget(const QString& songFileName, const QString& chartFileName,
		const SettingsWidget* settings, QWidget* parent = nullptr);

	~PlayWidget();

signals:
	void signalBackMenu();

private:
	void initPlayWidget();

private slots:
	void gamePause();
	void gameContinue();
	void gameClose();

private:
	Ui::PlayWidgetClass ui;
	PauseWidget* pauseWidget;
	EndWidget* endWidget;

	GameController* gameController;
};
