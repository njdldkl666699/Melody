#pragma once

#include <QDialog>
#include <QGraphicsBlurEffect>	//import blur model to process the confirm widget
#include <QGraphicsScene>		//manage 2D graphics
#include <QGraphicsPixmapItem>	//add image element
#include <QPainter>				//draw the bakcground
#include <QPalette>
#include <QPixmap>
#include <QImage>
#include "ui_confirmDialog.h"

class ConfirmDialog : public QDialog
{
	Q_OBJECT

public:
	ConfirmDialog(QWidget *parent = nullptr,const QPixmap&bkg=QPixmap());
	~ConfirmDialog();

signals:
	//void backToMenu();
	void exitGame();

private slots:
	void on_pushButton_backMenu_clicked();
	void on_pushButton_exit_clicked();
	
	
private:
	Ui::confirmDialogClass ui;
	QPixmap background;

private:
	void initConfirmDialog(QWidget*parent);
};
