#include "EndWidget.h"
#include "MenuWidget.h"
#include "UIController.h"

#include<QFile>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>

#include<QDate>
#include<QTreeWidget>
#include<QPropertyAnimation>

EndWidget::EndWidget(const GameController* game, QWidget* parent)
	: gameController(game), player(nullptr), audio(nullptr),
	ifHistoryOn(false), QWidget(parent)
{
	ui.setupUi(this);
	initWindow();
	initUI();
	initScore();
	initRank();
	initHistory();
	playAnimation();
	playMusic();

	//connect button
	connect(ui.pushButton_backMenu, &QPushButton::clicked, this, [this]()
		{
			player->stop();
			emit signalBackMenu();
			this->close();
		});
	connect(ui.pushButton_restart, &QPushButton::clicked, this, [this]()
		{
			player->stop();
			emit signalRestart();
			this->close();
		});
	connect(ui.pushButton_history, &QPushButton::clicked, this, &EndWidget::historyOn);
}

EndWidget::~EndWidget()
{
	if (backgroundGIF != nullptr)
	{
		delete backgroundGIF;
		backgroundGIF = nullptr;
	}
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}
	if (audio != nullptr)
	{
		delete audio;
		audio = nullptr;
	}
}

void EndWidget::initWindow()
{
	using namespace UICtrl;
	setWindowTitle(QString::fromLocal8Bit("ÒôÁé»ÃÕÂMeolide"));
	setWindowIcon(QIcon("./res/icon/icon.ico"));
	auto sw = SettingsWidget::instance();
	setIfFullscreen(this, sw->getFullscreen());
	setObjectSound(ui.pushButton_backMenu, &QPushButton::clicked, ber, sw->getSoundVal());
	setObjectSound(ui.pushButton_history, &QPushButton::clicked, ber, sw->getSoundVal());
	setObjectSound(ui.pushButton_restart, &QPushButton::clicked, ber, sw->getSoundVal());
}

void EndWidget::initUI()
{
	// init Background GIF
	backgroundGIF = new QMovie("./res/background/end.gif");
	ui.background->setMovie(backgroundGIF);
	backgroundGIF->start();

	// init song Picture
	QPixmap songPic = gameController->getSongPicture();
	songPic = songPic.scaled(ui.label_songPic->size(),
		Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
	ui.label_songPic->setPixmap(songPic);

	// init chart Intro
	QString chartIntro = gameController->getSongName() +
		" / " + gameController->getChartName();
	ui.label_chartIntro->setText(chartIntro);

	//init score Frame
	ui.label_scoreFrame->setPixmap(QString("./res/icon/scoreFrame.png"));
	ui.label_scoreListFrame->setPixmap(QString("./res/icon/scoreListFrame.png"));

	//init history Frame
	ui.treeWidget_history->hide();
}

void EndWidget::initScore()
{
	//get scores from gameController
	perfectNum = gameController->getPerfectCount();
	goodNum = gameController->getGoodCount();
	badNum = gameController->getBadCount();
	missNum = gameController->getMissCount();
	score = gameController->getScore();
	maxCombo = gameController->getMaxCombo();
	acc = gameController->getAccuracy();

	//set scores to UI
	ui.label_perfectNum->setText(QString::number(perfectNum));
	ui.label_goodNum->setText(QString::number(goodNum));
	ui.label_badNum->setText(QString::number(badNum));
	ui.label_missNum->setText(QString::number(missNum));
	ui.label_scoreNum->setText(QString::number(score));
	ui.label_comboNum->setText(QString::number(maxCombo));
	ui.label_accNum->setText(QString::number(acc, 'f', 2) + "%");
}

void EndWidget::initRank()
{
	QString rankPath = "./res/icon/rank";
	const QString rankPathEnd[] = { "Phi", "BV", "V", "S", "A", "B", "C", "F" };
	const QString rankRecord[] = { "¦Õ AP", "V FC", "V", "S", "A", "B", "C", "F" };
	const QString rankComment[] =
	{
		"Genius!Perfect!",
		"No Miss!? unbelievable",
		"Nothing can stop you",
		"So good at it",
		"So easy",
		"Just a little miss",
		"Need practice",
		"Quite bad"
	};

	if (acc == 100)
		rank = phi;
	else if (badNum + missNum == 0 && score != 0)
	{
		//in debug, if you click skip 
		//at the beginning, you will wrongly get a BV,
		//so I add (score != 0) to prevent this
		rank = BlueV;
	}
	else if (acc >= 99)
		rank = V;
	else if (acc >= 97.5)
		rank = S;
	else if (acc >= 95)
		rank = A;
	else if (acc >= 90)
		rank = B;
	else if (acc >= 85)
		rank = C;
	else
		rank = F;

	rankStr = rankRecord[rank];
	ui.label_rank->setToolTip(rankComment[rank]);

	rankPath += rankPathEnd[rank];
	//ui.label_rank->setScaledContents(true);
	ui.label_rank->setPixmap(rankPath);
}

void EndWidget::playAnimation()
{
	//label_rank animation
	QPropertyAnimation* rankAnimation = 
		new QPropertyAnimation(ui.label_rank, "pos", this);
	rankAnimation->setDuration(1000);
	rankAnimation->setStartValue(QPoint(-600, 70));
	rankAnimation->setEndValue(QPoint(40, 70));
	rankAnimation->setEasingCurve(QEasingCurve::OutQuad);
	rankAnimation->start();

	//label_scoreFrame animation
	QPropertyAnimation* scoreFrameAnimation = 
		new QPropertyAnimation(ui.label_scoreFrame, "pos", this);
	scoreFrameAnimation->setDuration(1000);
	scoreFrameAnimation->setStartValue(QPoint(-600, 510));
	scoreFrameAnimation->setEndValue(QPoint(40, 510));
	scoreFrameAnimation->setEasingCurve(QEasingCurve::OutQuad);
	scoreFrameAnimation->start();

	//label_scoreNum animation
	QPropertyAnimation* scoreNumAnimation = 
		new QPropertyAnimation(ui.label_scoreNum, "pos", this);
	scoreNumAnimation->setDuration(1000);
	scoreNumAnimation->setStartValue(QPoint(-600, 590));
	scoreNumAnimation->setEndValue(QPoint(90, 590));
	scoreNumAnimation->setEasingCurve(QEasingCurve::OutQuad);
	scoreNumAnimation->start();

	//label_scoreListFrame animation
	QPropertyAnimation* scoreListFrameAnimation =
		new QPropertyAnimation(ui.label_scoreListFrame, "pos", this);
	scoreListFrameAnimation->setDuration(1000);
	scoreListFrameAnimation->setStartValue(QPoint(1720, 490));
	scoreListFrameAnimation->setEndValue(QPoint(400, 490));
	scoreListFrameAnimation->setEasingCurve(QEasingCurve::OutQuad);
	scoreListFrameAnimation->start();

	//widget_scoreList animation
	QPropertyAnimation* scoreListAnimation =
		new QPropertyAnimation(ui.widget_scoreList, "pos", this);
	scoreListAnimation->setDuration(1000);
	scoreListAnimation->setStartValue(QPoint(1680, 520));
	scoreListAnimation->setEndValue(QPoint(440, 520));
	scoreListAnimation->setEasingCurve(QEasingCurve::OutQuad);
	scoreListAnimation->start();
}

void EndWidget::playMusic()
{
	// Music List
	QString musicName[] = { "Phi", "BV", "V", "S", "A", "B", "C", "F" };
	QString musicPath = "./res/music/" + musicName[rank] + ".mp3";

	// Set Music Player
	player = new QMediaPlayer(this);
	audio = new QAudioOutput(this);
	player->setAudioOutput(audio);
	player->setSource(QUrl::fromLocalFile(musicPath));
	audio->setVolume(SettingsWidget::instance()->getMusicVal() / 100.f);
	player->play();
}

void EndWidget::historyOn()
{
	if (!ifHistoryOn)
	{
		ui.treeWidget_history->show();
		ifHistoryOn = true;
	}
	else
	{
		ui.treeWidget_history->hide();
		ifHistoryOn = false;
	}
}

void EndWidget::initHistory()
{
	//create history folder
	dir.setPath(QString("./history"));
	if (dir.exists())
	{
		qDebug() << "have folder";
	}
	else
	{
		bool success = dir.mkpath(QString("./history"));
		if (!success)
		{
			qDebug() << "fail to create";
		}
		else
		{
			qDebug() << "folder create";
		}
	}

	//set filename
	filename = "./history/" + gameController->getSongName() +
		"_" + gameController->getChartName() + ".txt";

	//create json obj
	QJsonObject recordObj;
	recordObj["Date"] = QDate::currentDate().toString("yyyy-MM-dd");
	recordObj["Score"] = QString::number(score);
	recordObj["Accurancy"] = QString::number(acc, 'f', 2);
	recordObj["Rank"] = rankStr;
	recordObj["MaxCombo"] = QString::number(maxCombo);

	//read history from file
	//If file not exist, create new one
	QFile file(filename);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
	{
		qDebug() << "Error: File cannot read and write";
		return;
	}
	QJsonDocument fileDoc = QJsonDocument::fromJson(file.readAll());
	QJsonObject fileObj = fileDoc.object();
	QJsonArray fileArr = fileObj["History"].toArray();
	fileArr.append(recordObj);

	//write history to file
	fileObj["History"] = fileArr;
	fileDoc.setObject(fileObj);
	file.resize(0); //clear file content
	file.write(fileDoc.toJson());
	file.close();

	//init treeWidget_history
	for (const auto& hisVal : fileArr)
	{
		QJsonObject hisObj = hisVal.toObject();
		QTreeWidgetItem* childItem = new QTreeWidgetItem(ui.treeWidget_history);
		childItem->setText(0, hisObj["Date"].toString());
		childItem->setText(1, hisObj["Score"].toString());
		childItem->setText(2, hisObj["Accurancy"].toString() + "%");
		childItem->setText(3, hisObj["Rank"].toString());
		childItem->setText(4, hisObj["MaxCombo"].toString());
	}
}