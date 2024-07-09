#include "PlayWidget.h"

PlayWidget::PlayWidget(const QString& songFilePth, const QString& chartFilePth,
	const SettingsWidget* settingsWidget, QWidget* parent)
	: endWidget(nullptr), QWidget(parent),
	pauseWidget(new PauseWidget()), settings(settingsWidget),
	songFilePath(songFilePth), chartFilePath(chartFilePth),
	gameController(new GameController(songFilePth, chartFilePth, settingsWidget))
{
	ui.setupUi(this);
	initPlayWidget();
	gameController->setNoteParent(this);

	//connect GameController related
	connect(this, &PlayWidget::signalKeyPressed, gameController, &GameController::judgeKeyPress);
	connect(this, &PlayWidget::signalKeyReleased, gameController, &GameController::judgeKeyRelease);
	connect(gameController, &GameController::musicEnded, this, &PlayWidget::gameEnd);

	//connect PauseWidget related
	connect(ui.pushButton_pause, &QPushButton::clicked, this, &PlayWidget::gamePause);
	connect(pauseWidget, &PauseWidget::signalBackMenu, this, &PlayWidget::gameClose);
	connect(pauseWidget, &PauseWidget::signalRestart, this, &PlayWidget::gameRestart);
	connect(pauseWidget, &PauseWidget::signalContinue, this, &PlayWidget::gameContinue);

	//Debug
	connect(ui.pushButton_debug, &QPushButton::clicked, this, &PlayWidget::gameEnd);
}

PlayWidget::~PlayWidget()
{
	delete pauseWidget;
	delete endWidget;
	delete gameController;
}

void PlayWidget::initPlayWidget()
{
	//set background
	QPixmap backgroundPNG("./res/background/play.png");
	backgroundPNG = backgroundPNG.scaled(ui.background->size(),
		Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui.background->setPixmap(backgroundPNG);
	//set judgeline
	QPixmap judgeLinePNG("./res/note/judgeLine.png");
	judgeLinePNG = judgeLinePNG.scaled(ui.judgeline->size(),
		Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ui.judgeline->setPixmap(judgeLinePNG);
}

void PlayWidget::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		gamePause();
		return;
	}
	else
		emit signalKeyPressed(event);
}

void PlayWidget::keyReleaseEvent(QKeyEvent* event)
{
	emit signalKeyReleased(event);
}

void PlayWidget::gamePause()
{
	// Pause music
	gameController->pauseMusic();
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
	gameController->playMusic();
	this->show();
}

void PlayWidget::gameRestart()
{
	gameController->reset();
	this->show();
}

void PlayWidget::gameClose()
{
	emit signalBackMenu();
	this->close();
}

void PlayWidget::gameEnd()
{
	gameController->stopMusic();
	endWidget = new EndWidget(gameController);
	//connect EndWidget related
	connect(endWidget, &EndWidget::signalBackMenu, this, &PlayWidget::gameClose);
	connect(endWidget, &EndWidget::signalRestart, this, &PlayWidget::gameRestart);
	endWidget->show();
	this->hide();
}