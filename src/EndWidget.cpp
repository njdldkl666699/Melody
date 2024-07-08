#include "EndWidget.h"
#include "MenuWidget.h"
#include<QPixmap>
#include<QPainter>
#include<QPropertyAnimation>


EndWidget::EndWidget(const GameController* game, QWidget* parent)
	: gameController(game), QWidget(parent)
{
	ui.setupUi(this);
	initBackgroundGIF();
	initScoreList();
	initChartIntro(game);
	//void setScore(int bestNum, int goodNum, int missNum, int comboNum, int accNum, int score);
	setScore(game->getPerfectCount(), game->getGoodCount(), game->getMissCount(), game->getMaxCombo(), game->getAccuracy(), game->getScore());
	//setScore(1123, 123, 0, 1333, 100, 114514);
	showScore();
	showRank();
	ui.pushButton_restart->setToolTip("Restart");
	ui.pushButton_restart->setStyleSheet("QToolTip { color: black; background-color: white; border: 1px solid black; }");
	ui.pushButton_backMenu->setToolTip("Back To Menu");
	ui.pushButton_backMenu->setStyleSheet("QToolTip { color: black; background-color: white; border: 1px solid black; }");
	connect(ui.pushButton_backMenu, &QPushButton::clicked, this, [this]()
		{
			emit signalBackMenu();
			this->close();
		});
	connect(ui.pushButton_restart, &QPushButton::clicked, this, [this]()
		{
			emit signalRestart();
			this->close();
		});
}

EndWidget::~EndWidget()
{
	delete backgroundGIF;
}

void EndWidget::initBackgroundGIF()
{
	ui.background->setGeometry(0, 0, 1200, 675);
	backgroundGIF = new QMovie("./res/background/end.gif");
	ui.background->setMovie(backgroundGIF);
	backgroundGIF->start();

	ui.pushButton_restart->setIcon(QIcon("./res/icon/restart_black_64x.png"));
	ui.pushButton_backMenu->setIcon(QIcon("./res/icon/home_black_64x.png"));
}

void EndWidget::initScoreList()
{
	ui.bar_accNum->setStyleSheet("QProgressBar {"
							   "border: 1px solid grey;"
							   "background-color: white;" // 背景色
							   "color: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
							   "stop:0 #ff0000, stop:1 #0000ff);" // 渐变色从红色到蓝色
							   "text-align: center;"
							   "border-radius: 5px;"
							   "min-height: 0px;"
							   "}");
	ui.label_bestNum->setStyleSheet("QLabel{"
	"color:rgba(98,34,28,1);"
	"text-align:center;"
	"font-family:Arial;font-size:20px;font-weight:bold;"
	"min-width:100px;"
	"max-width:100px;"
	"min-height:40px;"
	"max-height:40px;"
	"}");
	ui.label_goodNum->setStyleSheet("QLabel{"
		"color:rgba(98,34,28,1);"
		"text-align:center;"
		"font-family:Arial;font-size:20px;font-weight:bold;"
		"min-width:60px;"
		"max-width:60px;"
		"min-height:40px;"
		"max-height:40px;"
		"}");

	ui.label_missNum->setStyleSheet("QLabel{"
	"color:rgba(103,55,52,1);"
	"text-align:center;"
	"font-family:Arial;font-size:20px;font-weight:bold;"
	"min-width:60px;"
	"max-width:60px;"
	"min-height:40px;"
	"max-height:40px;"
	"}");

	ui.label_comboNum->setStyleSheet("QLabel{"
	"color:rgba(98,22,22,1);"
	"text-align:center;"
	"font-family:Arial;font-size:20px;font-weight:bold;"
	"min-width:60px;"
	"max-width:60px;"
	"min-height:40px;"
	"max-height:40px;"
	"}");

	ui.label_scoreNum->setStyleSheet("QLabel{"
		"color:rgba(0,0,0,1);"
		"text-align:center;"
		"font-family:cursive;font-size:48px;font-weight:bold;"
		"min-width:300px;"
		"max-width:300px;"
		"min-height:200px;"
		"max-height:200px;"
		"}");
}

void EndWidget::initChartIntro(const GameController* game) //初始化歌曲的图片和铺面的介绍（如果需要的话）
{
	QLabel* label_songPicture = new QLabel(this);
	label_songPicture->setStyleSheet("QLabel{"
		"border:2px solid white;"
		"border-radius:10px;"
		"min-width:300px;"
		"max-width:300px;"
		"min-height:300px;"
		"max-height:300px;"
		"}");
	label_songPicture->move(450, 60);

	//此处需要获取 MenuWidget中铺面图片 用 路径 暂时替代
	//QString songPicPath = "./beatmap/Mujinku-Vacuum Track#ADD8E6-/bg.jpg";

	label_songPicture->setScaledContents(true);
    //songPic = songPic.scaled(label_songPicture->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

	label_songPicture->setPixmap(game->getSongPicture());


	//铺面信息 
	ui.label_chartIntro->setStyleSheet("QLabel{"
		"color:rgba(0,85,255,1);"
		"}");
	QString chartIntro;
	chartIntro = game->getSongName() + "/Chart:" + game->getChartName();
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
	int rank; //1:Fai 2:BV（蓝V 3:WV(白V 4:A 5: B 6:C 7：F


	//此处应计算rank 在此先随便设计一个测试
	if (accNum == 100) rank = 1;
	else if ((accNum >= 98.5 && accNum < 100)||missNum==0) rank = 2;
	else if (accNum >= 97.5 && accNum < 98.5) rank = 3;
	else if (accNum >= 95 && accNum < 97.5) rank = 4;
	else if (accNum >= 90 && accNum < 95) rank = 5;
	else if (accNum >= 85 && accNum < 90) rank = 6;
	else if (accNum >= 0 && accNum < 85) rank = 7;


	QLabel* label_rank = new QLabel(this);
	label_rank->setStyleSheet("QLabel{"
		"min-width:400px;"
		"max-width:400px;"
		"min-height:400px;"
		"max-height:400px;"
		"}");
	label_rank->move(25, 20);
	QString rankPath;
	switch (rank)
	{
	case 1:
		rankPath = "./res/icon/rankFai"; break;
	case 2:
		rankPath = "./res/icon/rankBV";  break;
	case 3:
		rankPath = "./res/icon/ranWV"; break;
	case 4:
		rankPath = "./res/icon/rankA";  break;
	case 5:
		rankPath = "./res/icon/rankB";  break;
	case 6:
		rankPath = "./res/icon/rankC"; break;
	case 7:
		rankPath = "./res/icon/rankF"; break;
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

