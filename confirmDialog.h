#pragma once

#include <QDialog>
#include "ui_confirmDialog.h"

class ConfirmDialog : public QDialog
{
	Q_OBJECT

public:
	ConfirmDialog(QWidget *parent = nullptr);
	~ConfirmDialog();

signals:
	void backToMenu();
	void exitGame();

private slots:
	void on_pushButton_backMenu_clicked();
	void on_pushButton_exit_clicked();
	
	
private:
	Ui::confirmDialogClass ui;

private:
	void initConfirmDialog(QWidget*parent);
};
