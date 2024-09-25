/*
	<Source file of class MenuWidget>
	Copyright (C) <2024>  <Meolide Team>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "MenuWidget.h"
#include "UIController.h"
#include"EndWidget.h"
#include<QPixmap>
#include<QGraphicsBlurEffect>

MenuWidget::MenuWidget(QWidget* parent)
	: QWidget(parent), playWidget(nullptr), confirmDialog(nullptr)
{
	ui.setupUi(this);
	setWindowTitle(QString::fromLocal8Bit("ÒôÁé»ÃÕÂMeolide"));
	setWindowIcon(QIcon("./res/icon/icon.ico"));
	using namespace UICtrl;
	setIfFullscreen(this, SettingsWidget::instance()->getFullscreen());

	initBackgroundGIF();
	initLogoGIF();

	// SongComboBox related
	initSongComboBox();
	connect(ui.comboBox_song, &QComboBox::currentTextChanged, this, &MenuWidget::onComboBoxSongSelected);

	// connect SettingsWidget related
	connect(ui.pushButton_settings, &QPushButton::clicked, this, [this]()
		{
			SettingsWidget::instance()->show();
			this->hide();
		});
	connect(SettingsWidget::instance(), &SettingsWidget::pushButtonBackMenuClicked, this, &MenuWidget::show);

	// connect PlayWidget related
	connect(ui.pushButton_play, &QPushButton::clicked, this, &MenuWidget::onPushButtonPlayClicked);

	// connect buttonClickSound
	const int soundVal = SettingsWidget::instance()->getSoundVal();
	setObjectSound(ui.comboBox_chart, &QComboBox::highlighted, gear, soundVal);
	setObjectSound(ui.comboBox_song, &QComboBox::highlighted, gear, soundVal);
	setObjectSound(ui.pushButton_play, &QPushButton::clicked, ber, soundVal);
	setObjectSound(ui.pushButton_settings, &QPushButton::clicked, ber, soundVal);
}

MenuWidget::~MenuWidget()
{
	if (playWidget != nullptr)
	{
		delete playWidget;
		playWidget = nullptr;
	}
	if (backgroundGIF != nullptr)
	{
		delete backgroundGIF;
		backgroundGIF = nullptr;
	}
	if (logoGIF != nullptr)
	{
		delete logoGIF;
		logoGIF = nullptr;
	}
	if (confirmDialog != nullptr)
	{
		delete confirmDialog;
		confirmDialog = nullptr;
	}
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

		// blur the background
		QGraphicsBlurEffect* blurEffect = new QGraphicsBlurEffect(this);
		blurEffect->setBlurRadius(20);
		this->setGraphicsEffect(blurEffect);

		// show confirmDialog
		if (confirmDialog == nullptr)
		{
			confirmDialog = new ConfirmDialog(this);
		}
		if (confirmDialog->exec() == QDialog::Accepted)
		{
			event->accept();
			this->close();
		}
		else
		{
			event->ignore();
		}
		this->setGraphicsEffect(nullptr);
	}
	else
	{
		//do nothing
		return QWidget::keyPressEvent(event);
	}
}

void MenuWidget::onComboBoxSongSelected(const QString& songName)
{
	// Check if song is selected
	if (songName == "[Select Song]")
	{
		ui.comboBox_chart->clear();
		ui.comboBox_chart->addItem("[Select Chart]");
		ui.comboBox_chart->setCurrentText("[Select Chart]");
		ui.songPicture->clear();
		menuMusicPlayer->stop();
		return;
	}

	// Update chart comboBox
	ui.comboBox_chart->clear();
	ui.comboBox_chart->addItem("[Select Chart]");
	ui.comboBox_chart->setCurrentText("[Select Chart]");
	QStringList chartFilters;
	chartFilters << "*.meo";
	songDir = beatmapDir.path() + "/" + songName;
	QStringList chartFiles = songDir.entryList(chartFilters, QDir::Files);
	for (auto& chart : chartFiles)
	{
		int suffixPos = chart.lastIndexOf(".meo");
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

	// Process the picture & display
	int width = songPic.width();
	int height = songPic.height();
	int sideLength = qMin(width, height);
	int x = (width - sideLength) / 2;
	int y = (height - sideLength) / 2;
	songPic = songPic.copy(x, y, sideLength, sideLength);
	songPic = songPic.scaled(ui.songPicture->size(),
		Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
	ui.songPicture->setPixmap(songPic);

	// Update songFilePath
	QStringList songFileFilters;
	songFileFilters << "*.mp3" << "*.ogg" << "*.wav" << "*.flac" << "*.m4a";
	QString songSuffixName = songDir.entryList(songFileFilters, QDir::Files).at(0);
	songFilePath = songDir.path() + "/" + songSuffixName;

	//// Play the music
	menuMusicPlayer = new QMediaPlayer(this);
	menuMusicPlayer->setSource(QUrl::fromLocalFile(songFilePath));
	menuAudio = new QAudioOutput(this);
	menuAudio->setVolume(SettingsWidget::instance()->getMusicVal());
	menuMusicPlayer->setAudioOutput(menuAudio);
	menuMusicPlayer->play();
}

void MenuWidget::onPushButtonPlayClicked()
{
	// Check if song and chart are selected
	if (ui.comboBox_song->currentText() == "[Select Song]" ||
		ui.comboBox_chart->currentText() == "[Select Chart]")
	{
		return;
	}
	else
	{
		// Stop menuMusicPlayer
		menuMusicPlayer->stop();

		// Update chartFile
		chartFilePath = songDir.path() + "/" + ui.comboBox_chart->currentText() + ".meo";
		qDebug() << songFilePath << "\n" << chartFilePath;

		// Initialize PlayWidget
		if (playWidget != nullptr)
		{
			delete playWidget;
			playWidget = nullptr;
		}
		playWidget = new PlayWidget(songFilePath, chartFilePath);
		connect(playWidget, &PlayWidget::signalBackMenu, this, &MenuWidget::show);
		playWidget->resize(this->size());
		playWidget->show();
		this->hide();
	}
}