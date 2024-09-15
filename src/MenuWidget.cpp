#include "MenuWidget.h"
#include<QPixmap>
#include "ButtonClickSound.h"

MenuWidget::MenuWidget(QWidget* parent)
	: QWidget(parent), settingsWidget(new SettingsWidget(parent)),
	playWidget(nullptr), confirmDialog(new ConfirmDialog(parent))
{
	ui.setupUi(this);
	setWindowTitle(QString::fromLocal8Bit("ÒôÁé»ÃÕÂMeolide"));
	setWindowIcon(QIcon("./res/icon/icon.ico"));
	setWindowState(Qt::WindowFullScreen);

	initBackgroundGIF();
	initLogoGIF();

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

	//connect confirmDialog related
	//connect(confirmDialog, &ConfirmDialog::backToMenu, this,&MenuWidget::show);
	connect(confirmDialog, &ConfirmDialog::exitGame, this, &MenuWidget::close);

	//connect buttonClickSound
	ButtonClickSound::buttonClickSound(ui.pushButton_settings);
	ButtonClickSound::buttonClickSound(ui.pushButton_play);
	ButtonClickSound::buttonClickSound(ui.comboBox_chart);
	ButtonClickSound::buttonClickSound(ui.comboBox_song);
}

MenuWidget::~MenuWidget()
{
	delete settingsWidget;
	delete playWidget;
	delete backgroundGIF;
}

void MenuWidget::initSongComboBox()
{
	beatmapDir = QDir::currentPath() + "/beatmap";
	QStringList beatmapFilters;
	beatmapFilters << "*";
	QDir::Filters beatFilter = QDir::Dirs | QDir::NoDotAndDotDot;
	QStringList beatmapSubDirs = beatmapDir.entryList(beatmapFilters, beatFilter);
	for (auto& beatmap : beatmapSubDirs)
	{
		qDebug() << beatmap;
	}
	ui.comboBox_song->addItem("[Select Song]");
	ui.comboBox_song->addItems(beatmapSubDirs);
	ui.comboBox_song->setCurrentText("[Select Song]");
}

void MenuWidget::initBackgroundGIF()
{
	backgroundGIF = new QMovie("./res/background/menu.gif");
	backgroundGIF->setScaledSize(this->size());
	ui.background->setMovie(backgroundGIF);
	backgroundGIF->start();
}

void MenuWidget::initLogoGIF()
{
	logoGIF = new QMovie("./res/icon/logo.gif");
	logoGIF->setScaledSize(ui.logo->size());
	ui.logo->setMovie(logoGIF);
	logoGIF->start();
}

void MenuWidget::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		qDebug() << "Escape key pressed!";
		// confirm to close the window by escape key
		//ConfirmDialog confirmDialog(this);

		if (confirmDialog->exec() == QDialog::Accepted)
		{
			event->accept();
		}
		else
		{
			event->ignore();
		}
		confirmDialog->close();
	}
	else
	{
		//do nothing
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
	for (auto& chart : chartFiles)
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

	//process the picture & display
	int width = songPic.width();
	int height = songPic.height();
	int sideLength = qMin(width, height);
	int x = (width - sideLength) / 2;
	int y = (height - sideLength) / 2;
	songPic = songPic.copy(x, y, sideLength, sideLength);
	songPic = songPic.scaled(ui.songPicture->size(), 
		Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
	ui.songPicture->setPixmap(songPic);
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
		// Update songFile and chartFile
		QStringList songFileFilters;
		songFileFilters << "*.mp3" << "*.ogg" << "*.wav" << "*.flac" << "*.m4a";
		QString songFileName = songDir.entryList(songFileFilters, QDir::Files).at(0);
		songFilePath = songDir.path() + "/" + songFileName;
		chartFilePath = songDir.path() + "/" + ui.comboBox_chart->currentText() + ".txt";
		qDebug() << songFilePath << "\n" << chartFilePath;

		// Initialize PlayWidget
		if (playWidget != nullptr)
		{
			delete playWidget;
			playWidget = nullptr;
		}
		playWidget = new PlayWidget(songFilePath, chartFilePath, settingsWidget);
		connect(playWidget, &PlayWidget::signalBackMenu, this, &MenuWidget::show);
		playWidget->resize(this->size());
		playWidget->show();
		this->hide();
	}
}

