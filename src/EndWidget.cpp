#include "EndWidget.h"

EndWidget::EndWidget(GameController* game, QWidget *parent)
	: gameController(game),QWidget(parent)
{
	ui.setupUi(this);

	//固定窗口大小
	QSize windowSize(1440, 819);
	this->setFixedSize(windowSize);


	connect(ui.pushButton_backMenu, &QPushButton::clicked, this, [this]()
		{
			emit signalBackMenu();
			this->close();
		});
	connect(ui.pushButton_restart, &QPushButton::clicked, this, [this]()
		{
			emit signalRestart();
			this->close();
		});
}

EndWidget::~EndWidget()
{}