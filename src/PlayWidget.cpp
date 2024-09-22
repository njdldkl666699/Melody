#include "PlayWidget.h"
#include "UIController.h"
#include<QGraphicsBlurEffect>
#include<QPropertyAnimation>
#include<QKeyEvent>

PlayWidget::PlayWidget(const QString& songFilePth,
	const QString& chartFilePth, QWidget* parent)
	: endWidget(nullptr), pauseWidget(nullptr), QWidget(parent),
	songFilePath(songFilePth), chartFilePath(chartFilePth),
	gameController(nullptr), countDownNum(3)
{
	ui.setupUi(this);
	initPlayWidget();
	commentTimer.setSingleShot(true);
	countDownTimer.setTimerType(Qt::PreciseTimer);

	//create GameController
	gameController = new GameController(songFilePath, chartFilePath, this);

	/* set labels' level
	Explain: the z-order is determined by thier construction order 
		and raise() function, so call raise() here can make those labels
		above notes, and don't need to call it in updateUI() function.
	*/
	ui.label_acc->raise();
	ui.label_combo->raise();
	ui.label_score->raise();
	ui.label_comment->raise();

	//create PauseWidget
	pauseWidget = new PauseWidget(this);
	pauseWidget->move((this->width() - pauseWidget->width()) / 2,
		(this->height() - pauseWidget->height()) / 2);

	//connect PauseWidget related
	connect(pauseWidget, &PauseWidget::signalBackMenu, this, &PlayWidget::gameClose);
	connect(pauseWidget, &PauseWidget::signalRestart, this, &PlayWidget::gameRestart);
	connect(pauseWidget, &PauseWidget::signalContinue, this, &PlayWidget::gameContinue);

	//connect GameController related
	connect(this, &PlayWidget::signalKeyPressed, gameController, &GameController::judgeKeyPress);
	connect(this, &PlayWidget::signalKeyReleased, gameController, &GameController::judgeKeyRelease);
	connect(gameController, &GameController::gameEnded, this, &PlayWidget::gameEnd);

	//connect UI related
	connect(gameController, &GameController::signalUpdate, this, &PlayWidget::updateUI);
	connect(gameController, &GameController::judgeResult, this, &PlayWidget::updateComment);
	connect(&commentTimer, &QTimer::timeout, ui.label_comment, &QLabel::clear);
	connect(&countDownTimer, &QTimer::timeout, this, &PlayWidget::updateCountDown);

	//connect button related
	connect(ui.pushButton_pause, &QPushButton::clicked, this, &PlayWidget::gamePause);
	connect(ui.pushButton_end, &QPushButton::clicked, this, &PlayWidget::gameEnd);
	connect(gameController, &GameController::signalShowEndButton, ui.pushButton_end, &QPushButton::show);
#ifdef NDEBUG
	ui.pushButton_end->hide();
#endif // NDEBUG
}

PlayWidget::~PlayWidget()
{
	if (gameController != nullptr)
	{
		delete gameController;
		gameController = nullptr;
	}
	if (pauseWidget != nullptr)
	{
		delete pauseWidget;
		pauseWidget = nullptr;
	}
	if (endWidget != nullptr)
	{
		delete endWidget;
		endWidget = nullptr;
	}
}

void PlayWidget::initPlayWidget()
{
	//set window & sound
	setWindowTitle(QString::fromLocal8Bit("ÒôÁé»ÃÕÂMeolide"));
	setWindowIcon(QIcon("./res/icon/icon.ico"));
	using namespace UICtrl;
	auto sw = SettingsWidget::instance();
	setIfFullscreen(this, sw->getFullscreen());
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
	//in PlayWidget() constructor
}

void PlayWidget::updateUI()
{
	//set labels
	QString accuracy = QString::number(gameController->getAccuracy(), 'f', 2) + '%';
	ui.label_acc->setText(accuracy);
	ui.label_score->setText(QString::number(gameController->getScore()));
	ui.label_combo->setText(QString::number(gameController->getCombo()));
}

void PlayWidget::updateComment(const QString& comment)
{
	ui.label_comment->setText(comment);
	commentTimer.start(1000);
}

void PlayWidget::updateCountDown()
{
	//initialized to 3, then count down to 0
	if (countDownNum > 0)
	{
		ui.label_countDown->setText(QString::number(countDownNum));
		countDownNum--;

		// label_countDown animation
		//If you don't interpret this part,
		//just run the pregram and see it.  ;)

		QPropertyAnimation* inAnimation =
			new QPropertyAnimation(ui.label_countDown, "geometry", this);
		inAnimation->setDuration(250);
		inAnimation->setStartValue(QRect(710, 360, 0, 0));
		inAnimation->setEndValue(QRect(610, 320, 60, 80));
		inAnimation->setEasingCurve(QEasingCurve::OutQuad);

		QPropertyAnimation* outAnimation =
			new QPropertyAnimation(ui.label_countDown, "geometry", this);
		outAnimation->setDuration(250);
		outAnimation->setStartValue(QRect(610, 320, 60, 80));
		outAnimation->setEndValue(QRect(510, 360, 0, 0));
		outAnimation->setEasingCurve(QEasingCurve::OutQuad);

		inAnimation->start();
		connect(inAnimation, &QPropertyAnimation::finished, [=]()
			{
				QTimer::singleShot(500, [=]() { outAnimation->start(); });
			});
	}
	else
	{
		countDownTimer.stop();
		ui.label_countDown->clear();
		countDownNum = 3;
	}
}

void PlayWidget::keyPressEvent(QKeyEvent* event)
{
	//qDebug() << "keyPressEvent: " << event->text();
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
	/*
	the long press is interpreted as a series of
	press and release events, but we can distinguish
	them by the isAutoRepeat() function.
	The first press event is not an auto repeat event,
	and the last release event is not an auto repeat event.
	*/
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
	// stop the count down timer
	if (countDownTimer.isActive())
	{
		countDownTimer.stop();
		ui.label_countDown->clear();
		countDownNum = 3;
	}
	// game Pause
	gameController->gamePause();
	// blur the background
	QGraphicsBlurEffect* blurEffect = new QGraphicsBlurEffect(this);
	blurEffect->setBlurRadius(20);
	this->setGraphicsEffect(blurEffect);
	// Show pause menu
	pauseWidget->show();
}

void PlayWidget::gameContinue()
{
	pauseWidget->close();
	// remove blur effect
	this->setGraphicsEffect(nullptr);
	gameController->wait();
	//3 seconds wait, start a 3 time 1 second timer
	//in updateCountDown() function
	countDownTimer.start(1000);
	this->updateCountDown();
}

void PlayWidget::gameRestart()
{
	pauseWidget->close();
	// remove blur effect
	this->setGraphicsEffect(nullptr);
	gameController->reset();
	this->updateUI();
	this->show();
}

void PlayWidget::gameClose()
{
	gameController->gamePause();
	emit signalBackMenu();
	pauseWidget->close();
	this->close();
}

void PlayWidget::gameEnd()
{
	gameController->gamePause();
	//create EndWidget
	if (endWidget != nullptr)
	{
		delete endWidget;
		endWidget = nullptr;
	}
	endWidget = new EndWidget(gameController);
	//connect EndWidget related
	connect(endWidget, &EndWidget::signalBackMenu, this, &PlayWidget::gameClose);
	connect(endWidget, &EndWidget::signalRestart, this, &PlayWidget::gameRestart);
	endWidget->show();
	this->hide();
}