#include "confirmDialog.h"

ConfirmDialog::ConfirmDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	initConfirmDialog(this);

}

ConfirmDialog::~ConfirmDialog()
{}

void ConfirmDialog::on_pushButton_backMenu_clicked()
{
	emit backToMenu();
	close();
}

void ConfirmDialog::on_pushButton_exit_clicked()
{
	//accept();
	emit exitGame();
}

void ConfirmDialog::initConfirmDialog(QWidget*parent)
{
	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
	this->setModal(true);

	 // set half-transparent :reserve
	setAttribute(Qt::WA_TranslucentBackground);
	setStyleSheet("background:rgba(255, 255, 255, 50);"); // 透明度为200，0完全透明，255完全不透明

	ui.pushButton_backMenu->raise();
	ui.pushButton_exit->raise();

	
	this->connect(ui.pushButton_exit, &QPushButton::clicked, this, &ConfirmDialog::on_pushButton_exit_clicked);
	this->connect(ui.pushButton_backMenu, &QPushButton::clicked, this, &ConfirmDialog::on_pushButton_backMenu_clicked);

}
