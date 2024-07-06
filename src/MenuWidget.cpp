#include "MenuWidget.h"
#include<QPixmap>


MenuWidget::MenuWidget(QWidget* parent)
	: QWidget(parent), settingsWidget(new SettingsWidget(parent)), playWidget(nullptr)
{
	ui.setupUi(this);
	initBackgroundGIF();

	// SongComboBox related
	initSongComboBox();
	connect(ui.comboBox_song, &QComboBox::currentTextChanged, this, &MenuWidget::comboBoxSongSelected);

	// connect SettingsWidget related
	connect(ui.pushButton_settings, &QPushButton::clicked, this, [this]()
		{
			settingsWidget->show();
			this->hide();
		});
	connect(settingsWidget, &SettingsWidget::pushButtonBackMenuClicked, this, &MenuWidget::show);

	// connect PlayWidget related
	connect(ui.pushButton_play, &QPushButton::clicked, this, &MenuWidget::pushButtonPlayClicked);
}

void MenuWidget::initSongComboBox()
{
	beatmapDir = QDir::currentPath() + "/beatmap";
	QStringList beatmapFilters;
	beatmapFilters << "*";
	QDir::Filters beatFilter = QDir::Dirs | QDir::NoDotAndDotDot;
	QStringList beatmapSubDirs = beatmapDir.entryList(beatmapFilters, beatFilter);
	foreach(const QString & subDir, beatmapSubDirs)
	{
		qDebug() << subDir;
	}
	ui.comboBox_song->addItem("[Select Song]");
	ui.comboBox_song->addItems(beatmapSubDirs);
	ui.comboBox_song->setCurrentText("[Select Song]");
}

void MenuWidget::initBackgroundGIF()
{
	backgroundGIF = new QMovie("./res/background/menu.gif");
	ui.background->setMovie(backgroundGIF);
	backgroundGIF->start();
}

MenuWidget::~MenuWidget()
{}

void MenuWidget::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		qDebug() << "Escape key pressed!";
		// 这里可以添加你的处理逻辑  
		this->close();
	}
	else
	{
			// 调用基类的keyPressEvent来处理其他按键  
		return QWidget::keyPressEvent(event);
	}
}

void MenuWidget::comboBoxSongSelected(const QString& songName)
{
	// Update chart comboBox
	ui.comboBox_chart->clear();
	ui.comboBox_chart->addItem("[Select Chart]");
	ui.comboBox_chart->setCurrentText("[Select Chart]");
	QStringList chartFilters;
	chartFilters << "*.txt";
	songDir = beatmapDir.path() + "/" + songName;
	QStringList chartFiles = songDir.entryList(chartFilters, QDir::Files);
	for(auto &chart: chartFiles)
	{
		int suffixPos = chart.lastIndexOf(".txt");
		chart = chart.left(suffixPos);
		qDebug() << chart;
	}
	ui.comboBox_chart->addItems(chartFiles);

	// Update song picture
	if (ui.comboBox_song->currentText() == "[Select Song]")
	{
		ui.songPicture->clear();
		return;
	}
	QStringList songPicFilters;
	songPicFilters << "*.jpg" << "*.png";
	QString songPicName = songDir.entryList(songPicFilters, QDir::Files).at(0);
	QString songPicPath = songDir.path() + "/" + songPicName;
	QPixmap songPic(songPicPath);
	songPic = songPic.scaled(ui.songPicture->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui.songPicture->setPixmap(songPic);

	// Update songFile and chartFile
	QStringList songFileFilters;
	songFileFilters << "*.mp3" << "*.ogg" << "*.wav" << "*.flac" << "*.m4a";
	QString songFileName = songDir.entryList(songFileFilters, QDir::Files).at(0);
	songFilePath = songDir.path() + "/" + songName;
	chartFilePath = songDir.path() + "/" + ui.comboBox_chart->currentText() + ".txt";
}

void MenuWidget::pushButtonPlayClicked()
{
	// Check if song and chart are selected
	if (ui.comboBox_song->currentText() == "[Select Song]" ||
		ui.comboBox_chart->currentText() == "[Select Chart]")
	{
		return;
	}
	else
	{
		// Initialize PlayWidget
		playWidget = new PlayWidget(songFilePath, chartFilePath, settingsWidget);
		connect(playWidget, &PlayWidget::signalBackMenu, this, [this]()
			{
				this->show();
			});
		playWidget->resize(this->size());
		playWidget->show();
		this->hide();
	}
}