#include "confirmDialog.h"

ConfirmDialog::ConfirmDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
	setWindowModality(Qt::WindowModal);
	this->setModal(true);

	QPixmap backgroundPNG("./res/background/exitDialog.png");
	backgroundPNG = backgroundPNG.scaled(this->size(),
		Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ui.background->setPixmap(backgroundPNG);

	this->connect(ui.pushButton_exit, &QPushButton::clicked, this, &ConfirmDialog::on_pushButton_exit_clicked);
	this->connect(ui.pushButton_backMenu, &QPushButton::clicked, this, &ConfirmDialog::on_pushButton_backMenu_clicked);
}

ConfirmDialog::~ConfirmDialog()
{}

void ConfirmDialog::on_pushButton_backMenu_clicked()
{
	//emit backToMenu();
	reject();
}

void ConfirmDialog::on_pushButton_exit_clicked()
{
	//accept();
	accept();
}