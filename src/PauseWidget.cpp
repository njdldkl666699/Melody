#include "PauseWidget.h"
#include "SettingsWidget.h"
#include "UIController.h"

PauseWidget::PauseWidget(QWidget* parent)
	:QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
	setWindowModality(Qt::WindowModal);
	setModal(true);
	initBackgroundGIF();

	connect(ui.pushButton_backMenu, &QPushButton::clicked, this, &PauseWidget::signalBackMenu);
	connect(ui.pushButton_restart, &QPushButton::clicked, this, &PauseWidget::signalRestart);
	connect(ui.pushButton_continue, &QPushButton::clicked, this, &PauseWidget::signalContinue);

	/*UIController::buttonClickSound(ui.pushButton_backMenu);
	UIController::buttonClickSound(ui.pushButton_restart);
	UIController::buttonClickSound(ui.pushButton_continue);*/
	using namespace UICtrl;
	auto sw = SettingsWidget::instance();
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