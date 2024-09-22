#pragma once

#include "ui_PauseWidget.h"
#include <QDialog>
#include <QMovie>
#include <QKeyEvent>

class PauseWidget : public QDialog
{
	Q_OBJECT
		     
public:
	PauseWidget(QWidget* parent = nullptr);
	~PauseWidget();

protected:
	void keyPressEvent(QKeyEvent* event) override;
	
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
