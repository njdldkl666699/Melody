#include "PauseWidget.h"

PauseWidget::PauseWidget(QWidget* parent)
	:QWidget(parent)
{
	ui.setupUi(this);
	initBackgroundGIF();
	initPauseWidget();




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

void PauseWidget::initBackgroundGIF()
{
	backgroundGIF = new QMovie("./res/background/Pause2.gif");
	ui.background->setMovie(backgroundGIF);
	backgroundGIF->start();

	//显示按钮图层
	ui.pushButton_backMenu->raise();
	ui.pushButton_continue->raise();
	ui.pushButton_restart->raise();

}

void PauseWidget::initPauseWidget()
{

	setFixedSize(1200, 675);
	//连接restart
	
}
