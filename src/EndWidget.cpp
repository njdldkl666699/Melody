#include "EndWidget.h"
#include "MenuWidget.h"
#include "UIController.h"
#include<QFile>
#include<QTreeWidget>
#include<QJsonArray>
#include<QTextStream>
#include<QDir>

EndWidget::EndWidget(const GameController* game, QWidget* parent)
	: gameController(game), QWidget(parent)
{
	ui.setupUi(this);
	initWindow();
	initUI();

	dir.setPath(QString("./history"));
	if (dir.exists())
	{
		qDebug() << "have folder";
	}
	else
	{
		bool success = dir.mkpath(QString("./history"));
		if (success == 0) qDebug() << "fail to create";
		qDebug() << "folder create";
	}

	//void setScore(int bestNum, int goodNum, int missNum, int comboNum, int accNum, int score);
	setScore(game->getPerfectCount(), game->getGoodCount(), game->getMissCount(), game->getMaxCombo(), game->getAccuracy(), game->getScore());
	//setScore(1123, 123, 0, 1333, 100, 114514);
	showScore();
	showRank();
	musicSet();
	musicPlay();
	toolTips();

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

	filename = "./history/" + game->getSongName() + "_" + game->getChartName() + ".dat";

	ui.treeWidget_history->hide();
	connect(ui.pushButton_history, &QPushButton::clicked, this, &EndWidget::historyOn);
	writeHistory();
	getHistory();
	setHistoryList();
}

EndWidget::~EndWidget()
{
	delete backgroundGIF;
}

void EndWidget::initWindow()
{
	using namespace UICtrl;
	setWindowTitle(QString::fromLocal8Bit("音灵幻章Meolide"));
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
	ui.label_songPic->setPixmap(gameController->getSongPicture());

	// init chart Intro
	QString chartIntro = gameController->getSongName() +
		" / Chart:" + gameController->getChartName();
	ui.label_chartIntro->setText(chartIntro);
}

void EndWidget::setScore(int b, int g, int m, int combo, int a, int s)
{
	bestNum = b; 
	goodNum = g; missNum = m;
	comboNum = combo; accNum = a;
	score = s;
}

void EndWidget::showScore()
{
	ui.label_scoreFrame->setScaledContents(true);
	ui.label_scoreFrame->setPixmap(QString("./res/icon/scoreFrame.png"));

	ui.label_scoreListFrame->setScaledContents(true);
	QPixmap listFrame(QString("./res/icon/scoreListFrame.png"));
	ui.label_scoreListFrame->setPixmap(listFrame);




	ui.label_bestNum->setText(QString::number(bestNum));
	ui.label_goodNum->setText(QString::number(goodNum));
	ui.label_missNum->setText(QString::number(missNum));
	ui.label_comboNum->setText(QString::number(comboNum));
	ui.bar_accNum->setValue(accNum);
	ui.label_scoreNum->setText(QString::number(score));


	QPropertyAnimation* animation = new QPropertyAnimation(ui.widget_scoreList, "pos", this);
	animation->setDuration(1000); 
	animation->setStartValue(QPoint(400, -1000));
	animation->setEndValue(QPoint(400,420));
	animation->setEasingCurve(QEasingCurve::OutBack);
	// 启动动画
	animation->start();

	QPropertyAnimation* animation1 = new QPropertyAnimation(ui.widget_acc, "pos", this);
	animation1->setDuration(2000);
	animation1->setStartValue(QPoint(-1000,500));
	animation1->setEndValue(QPoint(480, 500));
	animation1->setEasingCurve(QEasingCurve::OutBack);
	// 启动动画
	animation1->start();

	QPropertyAnimation* animation2 = new QPropertyAnimation(ui.label_scoreNum, "pos", this);
	animation2->setDuration(3000);
	animation2->setStartValue(QPoint(-1000, 410));
	animation2->setEndValue(QPoint(140, 410));
	animation2->setEasingCurve(QEasingCurve::OutQuad);
	// 启动动画
	animation2->start();
}

void EndWidget::showRank()
{
	


	//此处应计算rank 在此先随便设计一个测试
	if (accNum == 100) rank = 1;
	else if (missNum == 0) rank = 2;
	else if (accNum >= 98.5 && accNum < 100)  rank = 3;
	else if (accNum >= 97.5 && accNum < 98.5) rank = 4;
	else if (accNum >= 95 && accNum < 97.5) rank = 5;
	else if (accNum >= 90 && accNum < 95) rank = 6;
	else if (accNum >= 85 && accNum < 90) rank = 7;
	else if (accNum >= 0 && accNum < 85) rank = 8;


	QLabel* label_rank = new QLabel(this);
	label_rank->setStyleSheet("QLabel{"
		"min-width:400px;"
		"max-width:400px;"
		"min-height:400px;"
		"max-height:400px;"
		"}");
	label_rank->move(25, 20);

	ui.pushButton_restart->setStyleSheet("QToolTip { color: black; background-color: white; border: 1px solid black; }");


	QString rankPath;
	switch (rank)
	{
	case 1:
		rankPath = "./res/icon/rankFai"; label_rank->setToolTip("Genius!Perfect!"); rankk = "φ "; break;
	case 2:
		rankPath = "./res/icon/rankBV"; label_rank->setToolTip("No Miss!? unbelievable"); rankk = "Blue V"; break;
	case 3:
		rankPath = "./res/icon/ranWV";  label_rank->setToolTip("nothing can stop you "); rankk = "White V"; break;
	case 4:
		rankPath = "./res/icon/rankS";  label_rank->setToolTip("so good at it"); rankk = "S"; break;
	case 5:
		rankPath = "./res/icon/rankA";  label_rank->setToolTip("so easy");  rankk = "A"; break;
	case 6:
		rankPath = "./res/icon/rankB";  label_rank->setToolTip("just a little miss");  rankk = "B"; break;
	case 7:
		rankPath = "./res/icon/rankC"; label_rank->setToolTip("need practice");  rankk = "C"; break;
	case 8:
		rankPath = "./res/icon/rankF";  label_rank->setToolTip("quite bad");  rankk = "F"; break;
	}

	QPixmap rankPic(rankPath);
	label_rank->setScaledContents(true);
	label_rank->setPixmap(rankPic);


	QPropertyAnimation* animation1 = new QPropertyAnimation(label_rank, "pos", this);
	animation1->setDuration(1000);  // 动画持续500毫秒
	animation1->setStartValue(QPoint(25, -900));
	animation1->setEndValue(QPoint(25,20)); 
	animation1->setEasingCurve(QEasingCurve::OutQuad);  // 使用OutQuad曲线
	// 启动动画
	animation1->start();

}

void EndWidget::historyOn()
{
	if (ifHistoryOn == 0)
	{
		ui.treeWidget_history->show();
		ifHistoryOn = 1;
	}
	else
	{
		ui.treeWidget_history->hide();
		ifHistoryOn = 0;
	}
}

void EndWidget::musicSet()
{

	player = new QMediaPlayer(this);
	audio = new QAudioOutput(this);
	player->setAudioOutput(audio);
	audio->setVolume(0.5);
	//播放列表
	musicList = {
		QUrl::fromLocalFile("./res/video/endMusic/Fai.mp3"),
		QUrl::fromLocalFile("./res/video/endMusic/BV.mp3"),
		QUrl::fromLocalFile("./res/video/endMusic/WV.mp3"),
		QUrl::fromLocalFile("./res/video/endMusic/S.mp3"),
		QUrl::fromLocalFile("./res/video/endMusic/A.mp3"),
		QUrl::fromLocalFile("./res/video/endMusic/B.mp3"),
		QUrl::fromLocalFile("./res/video/endMusic/C.mp3"),
		QUrl::fromLocalFile("./res/video/endMusic/F.mp3")
	};
	player->play();
}

void EndWidget::musicPlay()
{
	QUrl musicc(musicList.at(rank-1));
	player->setSource(musicc);
	//player->setVolume(100);
	player->play();
}

void EndWidget::toolTips()
{
	ui.pushButton_restart->setToolTip("Restart");
	ui.pushButton_restart->setStyleSheet("QToolTip { color: black; background-color: white; border: 1px solid black; }");

	ui.pushButton_backMenu->setToolTip("Back To Menu");
	ui.pushButton_backMenu->setStyleSheet("QToolTip { color: black; background-color: white; border: 1px solid black; }");

	

}

void EndWidget::writeHistory()
{
	QFile file(filename);
	if (!file.open(QIODevice::Append))
	{
		qDebug() << "cannotwrite";
		return;
	}
	else qDebug() << "write";
	QDataStream fout(&file);
	//fout.setAutoDetectUnicode(true);
	fout << datetime;
	fout<< QString::number(score);
	fout << QString::number(accNum);
	fout << QString::number(missNum);
	fout << rankk;
	file.close();
}

void EndWidget::getHistory()
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
		return;
	QDataStream fin(&file);
	HistoryList historyTmp;
	//fin.setAutoDetectUnicode(true);
	while (!fin.atEnd())
	{
		fin >> historyTmp.time;
		fin >> historyTmp.score;
		fin >> historyTmp.acc;
		fin >> historyTmp.miss;
		fin>> historyTmp.rank;
		historyList.push_back(historyTmp);
	}
	file.close();
}

void EndWidget::setHistoryList()
{
	int num = historyList.size();
	for (int i = 0; i < num; i++)
	{
		QTreeWidgetItem* childItem = new QTreeWidgetItem(ui.treeWidget_history);
		childItem->setText(0, historyList[i].time);
		childItem->setText(1, historyList[i].rank);
		childItem->setText(2, historyList[i].score);
		childItem->setText(3, historyList[i].acc+"%");
		childItem->setText(4, historyList[i].miss);

	}

}


