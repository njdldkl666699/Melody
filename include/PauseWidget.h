#pragma once

#include <QWidget>
#include <QMovie>
#include "ui_PauseWidget.h"

class PauseWidget : public QWidget
{
	Q_OBJECT
		     
public:
	PauseWidget(QWidget* parent = nullptr);
	~PauseWidget();
	
private:
	void initBackgroundGIF();

signals:
	void signalBackMenu();
	void signalRestart();
	void signalContinue();

private:
	Ui::PauseWidgetClass ui;
	QMovie* backgroundGIF;
};
