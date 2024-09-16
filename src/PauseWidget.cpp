#include "PauseWidget.h"
#include "UIController.h"
#include "SettingsWidget.h"

PauseWidget::PauseWidget(QWidget* parent)
	:QWidget(parent)
{
	ui.setupUi(this);
	setWindowTitle(QString::fromLocal8Bit("ÒôÁé»ÃÕÂMeolide"));
	setWindowIcon(QIcon("./res/icon/icon.ico"));
	initBackgroundGIF();
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

	/*UIController::buttonClickSound(ui.pushButton_backMenu);
	UIController::buttonClickSound(ui.pushButton_restart);
	UIController::buttonClickSound(ui.pushButton_continue);*/
	using namespace UICtrl;
	auto sw = SettingsWidget::instance();
    setIfFullscreen(this, sw->getFullscreen());
	setObjectSound(ui.pushButton_backMenu, &QPushButton::clicked, ber, sw->getSoundVal());
	setObjectSound(ui.pushButton_restart, &QPushButton::clicked, ber, sw->getSoundVal());
	setObjectSound(ui.pushButton_continue, &QPushButton::clicked, ber, sw->getSoundVal());
}

PauseWidget::~PauseWidget()
{}

void PauseWidget::initBackgroundGIF()
{
	backgroundGIF = new QMovie("./res/background/Pause2.gif");
	ui.background->setMovie(backgroundGIF);
	backgroundGIF->start();
}