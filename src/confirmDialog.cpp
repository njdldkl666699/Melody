#include "confirmDialog.h"

ConfirmDialog::ConfirmDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
	this->setModal(true);
	// set half-transparent :reserve
	//setWindowOpacity(0.5);
	//setAttribute(Qt::WA_TranslucentBackground);
	//setStyleSheet("background:rgba(255, 255, 255,0);"); // ͸����Ϊ200��0��ȫ͸����255��ȫ��͸��

	ui.label_background->raise();
	ui.pushButton_backMenu->raise();
	ui.pushButton_exit->raise();

	this->connect(ui.pushButton_exit, &QPushButton::clicked, this, &ConfirmDialog::on_pushButton_exit_clicked);
	this->connect(ui.pushButton_backMenu, &QPushButton::clicked, this, &ConfirmDialog::on_pushButton_backMenu_clicked);
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