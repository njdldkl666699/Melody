#include "PlayWidget.h"

PlayWidget::PlayWidget(const QString& songFilePath, const QString& chartFilePath,
	const SettingsWidget* settingsWidget, QWidget* parent)
	: gameController(new GameController(songFilePath, chartFilePath, settingsWidget)),
	pauseWidget(new PauseWidget()), endWidget(nullptr),
	QWidget(parent), settings(settingsWidget)
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
			endWidget = new EndWidget(gameController);
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
	QPixmap backgroundPNG("./res/background/play.png");
	backgroundPNG = backgroundPNG.scaled(ui.background->size(),
		Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui.background->setPixmap(backgroundPNG);
	this->show();
}

void PlayWidget::keyPressEvent(QKeyEvent* event)
{
	emit signalKeyPressed(event);
	//QString key[4] = { settings->getKey_1().toString(), settings->getKey_2().toString(),
	//	settings->getKey_3().toString(), settings->getKey_4().toString() };
	//for (int i = 0; i < 4; i++)
	//{
	//	qDebug() << key[i] << '\n' << event->text();
	//	if (key[i] == event->text())
	//	{
	//		qDebug()<< "Hit!";
	//		int musicCurrentTime = gameController->getMusicCurrentTime();
	//		int noteStartTime=gameController.
	//		//case perfect
	//		
	//		//case good
	//		//case miss
	//	}
	//}
}

void PlayWidget::keyReleaseEvent(QKeyEvent* event)
{
	emit signalKeyReleased(event);
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
