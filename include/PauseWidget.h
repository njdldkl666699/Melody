#pragma once

#include "ui_PauseWidget.h"
#include <QDialog>
#include <QMovie>

class PauseWidget : public QDialog
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
