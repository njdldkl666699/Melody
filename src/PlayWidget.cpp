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
	commentTimer.setSingleShot(true);
	gameController->setNoteParent(this);

	//connect GameController related
	connect(this, &PlayWidget::signalKeyPressed, gameController, &GameController::judgeKeyPress);
	connect(this, &PlayWidget::signalKeyReleased, gameController, &GameController::judgeKeyRelease);
	connect(gameController, &GameController::gameEnded, this, &PlayWidget::gameEnd);

	//connect PauseWidget related
	connect(ui.pushButton_pause, &QPushButton::clicked, this, &PlayWidget::gamePause);
	connect(pauseWidget, &PauseWidget::signalBackMenu, this, &PlayWidget::gameClose);
	connect(pauseWidget, &PauseWidget::signalRestart, this, &PlayWidget::gameRestart);
	connect(pauseWidget, &PauseWidget::signalContinue, this, &PlayWidget::gameContinue);

	//connect UI related
	connect(gameController, &GameController::signalUpdate, this, &PlayWidget::updateUI);
	connect(gameController, &GameController::judgeResult,this,&PlayWidget::updateComment);
	connect(&commentTimer, &QTimer::timeout, ui.label_comment, &QLabel::clear);

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
	//set labels
	ui.label_acc->setText("0%");
	ui.label_combo->setText("0");
	ui.label_score->setText("0");
	ui.label_comment->setText("");
}

void PlayWidget::updateUI()
{
	QString accuracy = QString::number(gameController->getAccuracy(), 'g', 4) + '%';
	ui.label_acc->setText(accuracy);
	ui.label_score->setText(QString::number(gameController->getScore()));
	ui.label_combo->setText(QString::number(gameController->getCombo()));
}

void PlayWidget::updateComment(const QString& comment)
{
	ui.label_comment->setText(comment);
	commentTimer.start(1000);
}

void PlayWidget::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		this->gamePause();
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
	// game Pause
	gameController->gamePause();
	// Show pause menu
	pauseWidget->show();
	this->hide();
}

void PlayWidget::gameContinue()
{
	// Continue the game
	gameController->gamePlay();
	this->show();
}

void PlayWidget::gameRestart()
{
	gameController->reset();
	this->show();
}

void PlayWidget::gameClose()
{
	gameController->gamePause();
	emit signalBackMenu();
	this->close();
}

void PlayWidget::gameEnd()
{
	gameController->gamePause();
	endWidget = new EndWidget(gameController);
	//connect EndWidget related
	connect(endWidget, &EndWidget::signalBackMenu, this, &PlayWidget::gameClose);
	connect(endWidget, &EndWidget::signalRestart, this, &PlayWidget::gameRestart);
	endWidget->show();
	this->hide();
}