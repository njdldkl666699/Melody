#include "menuwidget.h"
#include "ui_menuwidget.h"

menuWidget::menuWidget(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::menuWidget)
{
	ui->setupUi(this);
}

menuWidget::~menuWidget()
{
	delete ui;
}
