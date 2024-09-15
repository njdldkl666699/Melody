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
	// set half-transparent :reserve
	//setWindowOpacity(0.5);
	//setAttribute(Qt::WA_TranslucentBackground);
	//setStyleSheet("background:rgba(255, 255, 255,0);"); // 透明度为200，0完全透明，255完全不透明

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
	emit exitGame();
	accept();
}