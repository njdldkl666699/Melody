#include "EndWidget.h"

EndWidget::EndWidget(GameController* game, QWidget *parent)
	: gameController(game),QWidget(parent)
{
	ui.setupUi(this);
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