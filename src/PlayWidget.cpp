#include "PlayWidget.h"

PlayWidget::PlayWidget(const QString& songFilePath, const QString& chartFilePath,
	const SettingsWidget* settings, QWidget* parent)
	: pauseWidget(new PauseWidget()), QWidget(parent), 
	gameController(nullptr), endWidget(new EndWidget(gameController))
{
	ui.setupUi(this);

	auto songName = songFilePath;
	auto chartName = chartFilePath;

	initPlayWidget();

	//connect PauseWidget related
	connect(ui.pushButton_pause, &QPushButton::clicked, this, &PlayWidget::gamePause);
	connect(pauseWidget, &PauseWidget::signalBackMenu, this, &PlayWidget::gameClose);
	connect(pauseWidget, &PauseWidget::signalRestart, this, &PlayWidget::initPlayWidget);
	connect(pauseWidget, &PauseWidget::signalContinue, this, &PlayWidget::gameContinue);

	//connect EndWidget related
	connect(endWidget, &EndWidget::signalBackMenu, this, &PlayWidget::gameClose);
	connect(endWidget, &EndWidget::signalRestart, this, &PlayWidget::initPlayWidget);

	//connect([MusicPlayer],[musicEnd],this,&PlayWidget::gameEndSlot);
	//Debug
	connect(ui.pushButton_debug, &QPushButton::clicked, this, [this]()
		{
			endWidget->show();
			this->hide();
		});
}

PlayWidget::~PlayWidget()
{
	delete pauseWidget;
	delete endWidget;
	delete gameController;
}

void PlayWidget::initPlayWidget()
{
	this->show();
}

void PlayWidget::gamePause()
{
	// Pause music

	// Pause game

	// Show pause menu
	pauseWidget->show();
	this->hide();
}


void PlayWidget::gameContinue()
{
	//3 seconds wait

	// Continue the game

	// Continue the music
	this->show();
}

void PlayWidget::gameClose()
{
	emit signalBackMenu();
	this->hide();
}
