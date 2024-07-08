#include "GameController.h"

GameController::GameController(const QString& songFilePth, const QString& chartFilePth,
	const SettingsWidget* settingsWidget, QObject* parent)
	: QObject(parent), settings(settingsWidget), perfectCount(0), goodCount(0),
	missCount(0), accuracy(0), score(0), combo(0), maxCombo(0),
	songFilePath(songFilePth), chartFilePath(chartFilePth)
{
	initnoteTracks();
	initMediaPlayer();
	//qDebug() << getSongName();
}

GameController::~GameController()
{
	for (int i = 0; i < 4; i++)
		while (!noteTracks[i].isEmpty())
			delete noteTracks[i].dequeue();
}

QString GameController::getSongName()const
{
	QDir songDir(songFilePath);
	songDir.cdUp();
	//qDebug() << songDir.dirName();
	return songDir.dirName();
}

QString GameController::getChartName()const
{
	QFileInfo fileInfo(chartFilePath);
	return fileInfo.baseName();
}

QPixmap GameController::getSongPicture()const
{
	QDir songDir(songFilePath);
	QStringList songPicFilters;
	songPicFilters << "*.jpg" << "*.png";
	QString songPicName = songDir.entryList(songPicFilters, QDir::Files).at(0);
	QString songPicPath = songDir.path() + "/" + songPicName;
	return QPixmap(songPicPath);
}

void GameController::initnoteTracks()
{
	//read chart file
	QFile chartFile(chartFilePath);
	if (!chartFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Open chart file failed!";
		return;
	}
	QTextStream in(&chartFile);

	//read bpm and offset
	for (int i = 0; i < 2; i++)
	{
		QStringList bpmOrOffsetLine = in.readLine().split(",");
		int bpmOrOffset = bpmOrOffsetLine.last().toInt();

		if (bpmOrOffsetLine.first() == "bpm")
			bpm = bpmOrOffset;
		else if (bpmOrOffsetLine.first() == "offset")
			offset = bpmOrOffset;
		else
		{
			qDebug() << "Invalid chart file!";
			return;
		}
	}

	//read notes
	while (!in.atEnd())
	{
		QString noteLine = in.readLine();
		QStringList noteData = noteLine.split(";");
		bool isHold = noteData.first().toInt();
		QString startTimeRaw = noteData.at(1);
		int startTime = getNoteTime(startTimeRaw);

		//get KeySequence
		int key = noteData.last().toInt();
		QKeySequence keySequence;
		switch (key)
		{
		case 0:
			keySequence = settings->getKey_1();
			break;
		case 1:
			keySequence = settings->getKey_2();
			break;
		case 2:
			keySequence = settings->getKey_3();
			break;
		case 3:
			keySequence = settings->getKey_4();
			break;
		default:
			break;
		}

		//construct note
		Note* note = nullptr;
		if (!isHold)
		{
			//Tap note
			note = new Tap(startTime, keySequence);
		}
		else
		{
			//Hold note
			QString endTimeRaw = noteData.at(2);
			int endTime = getNoteTime(endTimeRaw);
			note = new Hold(startTime, endTime, keySequence);
		}

		//push note to noteTracks
		/*
		we use queue, because we just need to 
		check the first note once at a time in each track
		*/
		noteTracks[key].enqueue(note);
	}

	//close file
	chartFile.close();
}

int GameController::getNoteTime(const QString& rawTimeData)const
{
	QStringList timeData = rawTimeData.split(",");
	int beat = timeData.at(0).toInt();
	int pos = timeData.at(1).toInt();
	double totalPos = timeData.at(2).toInt();
	int miliseconds = settings->getBiasVal() - offset + (beat + pos / totalPos) * 60000 / bpm;
	return miliseconds;
}

void GameController::initMediaPlayer()
{
	//to be implemented
	musicPlayer.setSource(QUrl::fromLocalFile(songFilePath));
	audioOutput.setVolume(settings->getMusicVal() / 100.0f);
}