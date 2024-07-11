#include "confirmDialog.h"

ConfirmDialog::ConfirmDialog(QWidget *parent,const QPixmap&bkg)
	: QDialog(parent)
	,background(bkg)
{
	ui.setupUi(this);
	
	initConfirmDialog(parent);

}

ConfirmDialog::~ConfirmDialog()
{}

void ConfirmDialog::on_pushButton_backMenu_clicked()
{
	//emit backToMenu();
	this->close();
}

void ConfirmDialog::on_pushButton_exit_clicked()
{
	//accept();
	emit exitGame();
	this->close();
}

void ConfirmDialog::initConfirmDialog(QWidget*parent)
{

	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
	this->setModal(true);
	// set half-transparent :reserve
	//setWindowOpacity(0.5);
	//setAttribute(Qt::WA_TranslucentBackground);
	//setStyleSheet("background:rgba(255, 255, 255,0);"); // 透明度为200，0完全透明，255完全不透明
	
	ui.label_background->raise();
	ui.pushButton_backMenu->raise();
	ui.pushButton_exit->raise();

	
	this->connect(ui.pushButton_exit, &QPushButton::clicked, this, &ConfirmDialog::on_pushButton_exit_clicked);
	this->connect(ui.pushButton_backMenu, &QPushButton::clicked, this, &ConfirmDialog::on_pushButton_backMenu_clicked);

}
