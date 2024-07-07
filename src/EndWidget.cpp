#include "EndWidget.h"
#include "MenuWidget.h"
#include<QPixmap>

EndWidget::EndWidget(GameController* game, QWidget* parent)
	: gameController(game), QWidget(parent)
{
	ui.setupUi(this);
	initBackgroundGIF();
	//initScoreList();
	initChartIntro();
	setScore(12, 23, 345, 678, 999, 45, 114514);
	showScore();
	showRank();

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
	"color:rgba(255,170,0,1);"
	"text-align:center;"
	"font-family:Arial;font-size:20px;font-weight:bold;"
	"min-width:60px;"
	"max-width:60px;"
	"min-height:40px;"
	"max-height:40px;"
	"}");
	ui.label_coolNum->setStyleSheet("QLabel{"
		"color:rgba(255,170,0,1);"
		"text-align:center;"
		"font-family:Arial;font-size:20px;font-weight:bold;"
		"min-width:60px;"
		"max-width:60px;"
		"min-height:40px;"
		"max-height:40px;"
		"}");
	ui.label_goodNum->setStyleSheet("QLabel{"
		"color:rgba(255,170,0,1);"
		"text-align:center;"
		"font-family:Arial;font-size:20px;font-weight:bold;"
		"min-width:60px;"
		"max-width:60px;"
		"min-height:40px;"
		"max-height:40px;"
		"}");

	ui.label_missNum->setStyleSheet("QLabel{"
	"color:rgba(255,170,0,1);"
	"text-align:center;"
	"font-family:Arial;font-size:20px;font-weight:bold;"
	"min-width:60px;"
	"max-width:60px;"
	"min-height:40px;"
	"max-height:40px;"
	"}");

	ui.label_comboNum->setStyleSheet("QLabel{"
	"color:rgba(255,170,0,1);"
	"text-align:center;"
	"font-family:Arial;font-size:20px;font-weight:bold;"
	"min-width:60px;"
	"max-width:60px;"
	"min-height:40px;"
	"max-height:40px;"
	"}");

	ui.label_scoreNum->setStyleSheet("QLabel{"
		"color:rgba(255,170,0,1);"
		"text-align:center;"
		"font-family:Arial;font-size:48px;font-weight:bold;"
		"min-width:300px;"
		"max-width:300px;"
		"min-height:200px;"
		"max-height:200px;"
		"}");
}

void EndWidget::initChartIntro() //初始化歌曲的图片和铺面的介绍（如果需要的话）
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
	QString songPicPath = "./beatmap/Mujinku-Vacuum Track#ADD8E6-/bg.jpg";


	QPixmap songPic(songPicPath);
	label_songPicture->setScaledContents(true);
	//songPic = songPic.scaled(label_songPicture->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	label_songPicture->setPixmap(songPic);

	//如果需要加铺面信息 需要接口
	//QString chartIntro;
	//chartIntro = songName + "/Chart:" + chartName;
	//ui.label_chartIntro->setText(chartIntro);
}

void EndWidget::setScore(int b, int c, int g, int m, int combo, int a, int s)
{
	bestNum = b; coolNum = c;
	goodNum = g; missNum = m;
	comboNum = combo; accNum = a;
	score = s;
}

void EndWidget::showScore()
{
	ui.label_bestNum->setStyleSheet("QLabel{"
		"color:rgba(255,170,0,1);"
		"text-align:center;"
		"font-family:Arial;font-size:20px;font-weight:bold;"
		"min-width:60px;"
		"max-width:60px;"
		"min-height:40px;"
		"max-height:40px;"
		"}");
	ui.label_bestNum->setStyleSheet("QLabel{"
	"color:rgba(255,170,0,1);"
	"text-align:center;"
	"font-family:Arial;font-size:20px;font-weight:bold;"
	"min-width:60px;"
	"max-width:60px;"
	"min-height:40px;"
	"max-height:40px;"
	"}");



	ui.label_bestNum->setText(QString::number(bestNum));
	ui.label_coolNum->setText(QString::number(coolNum));
	ui.label_goodNum->setText(QString::number(goodNum));
	ui.label_missNum->setText(QString::number(missNum));
	ui.label_comboNum->setText(QString::number(comboNum));
	ui.bar_accNum->setValue(accNum);
	ui.label_scoreNum->setText(QString::number(score));
}

void EndWidget::showRank()
{
	int rank; //1:Fai 2:BV（蓝V 3:WV(白V 4:A 5: B 6:C 7：F


	//此处应计算rank 在此先随便设计一个测试
	if (accNum == 100) rank = 1;
	else if (accNum >= 80 && accNum < 100) rank = 2;
	else if (accNum >= 60 && accNum < 80) rank = 3;
	else if (accNum >= 40 && accNum < 60) rank = 4;
	else if (accNum >= 20 && accNum < 40) rank = 5;
	else if (accNum >= 0 && accNum < 20) rank = 6;


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
	label_rank->setPixmap(rankPic);
	label_rank->setScaledContents(true);


}

