#include "PauseWidget.h"

PauseWidget::PauseWidget(QWidget* parent)
	:QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton_backMenu, &QPushButton::clicked, this, [this]()
		{
			emit signalBackMenu();
			this->hide();
		});
	connect(ui.pushButton_restart, &QPushButton::clicked, this, [this]()
		{
			emit signalRestart();
			this->hide();
		});
	connect(ui.pushButton_continue, &QPushButton::clicked, this, [this]()
		{
			emit signalContinue();
			this->hide();
		});
}

PauseWidget::~PauseWidget()
{}