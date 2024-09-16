#include "PlayWidget.h"
#include "UIController.h"

PlayWidget::PlayWidget(const QString& songFilePth,
	const QString& chartFilePth, QWidget* parent)
	: endWidget(nullptr), QWidget(parent), pauseWidget(new PauseWidget()),
	songFilePath(songFilePth), chartFilePath(chartFilePth),
	gameController(new GameController(songFilePth, chartFilePth))
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
	connect(gameController, &GameController::judgeResult, this, &PlayWidget::updateComment);
	connect(&commentTimer, &QTimer::timeout, ui.label_comment, &QLabel::clear);

	//connect button related
	connect(ui.pushButton_end, &QPushButton::clicked, this, &PlayWidget::gameEnd);
	connect(gameController, &GameController::signalShowEndButton, ui.pushButton_end, &QPushButton::show);
#ifdef NDEBUG
	ui.pushButton_end->hide();
#endif // NDEBUG
}

PlayWidget::~PlayWidget()
{
	delete pauseWidget;
	delete endWidget;
	delete gameController;
}

void PlayWidget::initPlayWidget()
{
	//set window & sound
	setWindowTitle(QString::fromLocal8Bit("ÒôÁé»ÃÕÂMeolide"));
	setWindowIcon(QIcon("./res/icon/icon.ico"));
	using namespace UICtrl;
	auto sw = SettingsWidget::instance();
	setIfFullscreen(this,sw->getFullscreen());
	setObjectSound(ui.pushButton_pause, &QPushButton::clicked, ber, sw->getSoundVal());
	setObjectSound(ui.pushButton_end, &QPushButton::clicked, ber, sw->getSoundVal());

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
	ui.label_acc->setText("0.00%");
	ui.label_combo->setText("0");
	ui.label_score->setText("0");
	ui.label_comment->setText(" ");
	/*this used to be ui.label_comment->setText("");
	but it caused a lag when the first note falls down near the judge line,
	and I suspect that it is because this label is not well-initialized (font, size, etc.),
	so I add a space to force it to init early so the lag disappeared.*/

	//set labels' level
	ui.label_acc->raise();
	ui.label_combo->raise();
	ui.label_score->raise();
	ui.label_comment->raise();
}

void PlayWidget::updateUI()
{
	//set labels
	QString accuracy = QString::number(gameController->getAccuracy(), 'f', 2) + '%';
	ui.label_acc->setText(accuracy);
	ui.label_score->setText(QString::number(gameController->getScore()));
	ui.label_combo->setText(QString::number(gameController->getCombo()));
	//set labels' level
	ui.label_acc->raise();
	ui.label_combo->raise();
	ui.label_score->raise();
	ui.label_comment->raise();
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
	else if (event->key() == Qt::Key_Space)
	{
		return;
	}
	else if (event->isAutoRepeat() == false)
	{
		emit signalKeyPressed(event);
	}
	else
	{
		return QWidget::keyPressEvent(event);
	}
}

void PlayWidget::keyReleaseEvent(QKeyEvent* event)
{
	//received some event that should't be received,
	//but I can't fix it
	//qDebug() << "keyReleaseEvent";

	//now fix it
	if (event->isAutoRepeat() == false)
	{
		emit signalKeyReleased(event);
	}
	else
	{
		return QWidget::keyReleaseEvent(event);
	}
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
	//3 seconds wait, auto continue
	gameController->wait();
	this->show();
}

void PlayWidget::gameRestart()
{
	gameController->reset();
	this->updateUI();
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