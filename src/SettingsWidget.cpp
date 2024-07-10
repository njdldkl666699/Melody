#include "SettingsWidget.h"
#include "ButtonClickSound.h"
#ifdef _DEBUG
#include<cassert>
#endif // _DEBUG

SettingsWidget::SettingsWidget(QWidget* parent)
	: QWidget(parent), backgroundPic("./res/background/settings.png"),
	musicVal(100), soundVal(100), speedVal(0), biasVal(0), fpsVal(60),
	key_1(Qt::Key_S), key_2(Qt::Key_F), key_3(Qt::Key_J), key_4(Qt::Key_L)
{
	ui.setupUi(this);
	initBackground();
	initSettings();
	initUIValues();
	setWindowTitle("Meolide");

	ButtonClickSound::buttonClickSound(ui.pushButton_backMenu);
	ButtonClickSound::buttonClickSound(ui.slider_bias);
	ButtonClickSound::buttonClickSound(ui.slider_music);
	ButtonClickSound::buttonClickSound(ui.slider_sound);
	ButtonClickSound::buttonClickSound(ui.slider_speed);
	ButtonClickSound::buttonClickSound(ui.comboBox_fps);
	ButtonClickSound::buttonClickSound(ui.keySequenceEdit_1);
	ButtonClickSound::buttonClickSound(ui.keySequenceEdit_2);
	ButtonClickSound::buttonClickSound(ui.keySequenceEdit_3);
	ButtonClickSound::buttonClickSound(ui.keySequenceEdit_4);

	connect(ui.pushButton_backMenu, &QPushButton::clicked, this, [this]()
		{
			emit pushButtonBackMenuClicked();
			this->saveSettings();
			this->close();
		});

}

SettingsWidget::~SettingsWidget()
{}

void SettingsWidget::initBackground()
{
	backgroundPic = backgroundPic.scaled(this->size(), Qt::KeepAspectRatioByExpanding);
    ui.background->setPixmap(backgroundPic);
    ui.background->show();
}

void SettingsWidget::initSettings()
{
	QFile file("./settings.cfg");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Could not open the file for reading, create a new";
		//save Default Settings
		QFile newFile("./settings.cfg");
		newFile.open(QIODevice::WriteOnly | QIODevice::Text);
		QTextStream out(&newFile);
		out << "music: " << musicVal << "\n" <<
			"sound: " << soundVal << "\n" <<
			"speed: " << speedVal << "\n" <<
			"bias: " << biasVal << "\n" <<
			"fps: " << fpsVal << "\n" <<
			"key_1: " << key_1.toString() << "\n" <<
			"key_2: " << key_2.toString() << "\n" <<
			"key_3: " << key_3.toString() << "\n" <<
			"key_4: " << key_4.toString() << "\n";
		newFile.close();
		return;
	}
	//load Settings
	QTextStream in(&file);
	QString line = in.readLine();
	while (!line.isEmpty())
	{
		qDebug() << line;
		if (line.indexOf("music: ") != -1)
			musicVal = line.mid(7).toInt();
		else if (line.indexOf("sound: ") != -1)
			soundVal = line.mid(7).toInt();
		else if (line.indexOf("speed: ") != -1)
			speedVal = line.mid(7).toInt();
		else if (line.indexOf("bias: ") != -1)
			biasVal = line.mid(6).toInt();
		else if (line.indexOf("fps: ") != -1)
			fpsVal = line.mid(5).toInt();
		else if (line.indexOf("key_1: ") != -1)
			key_1 = QKeySequence(line.mid(7));
		else if (line.indexOf("key_2: ") != -1)
			key_2 = QKeySequence(line.mid(7));
		else if (line.indexOf("key_3: ") != -1)
			key_3 = QKeySequence(line.mid(7));
		else if (line.indexOf("key_4: ") != -1)
			key_4 = QKeySequence(line.mid(7));
		line = in.readLine();
	}
	file.close();
}

void SettingsWidget::initUIValues()
{
	//set values
	ui.slider_music->setValue(musicVal);
	ui.slider_sound->setValue(soundVal);
	ui.slider_speed->setValue(speedVal);
	ui.slider_bias->setValue(biasVal);
	ui.comboBox_fps->setCurrentText(QString::number(fpsVal));
	ui.keySequenceEdit_1->setKeySequence(key_1);
	ui.keySequenceEdit_2->setKeySequence(key_2);
	ui.keySequenceEdit_3->setKeySequence(key_3);
	ui.keySequenceEdit_4->setKeySequence(key_4);
	//set value texts
	ui.label_musicValue->setText(QString::number(musicVal));
	ui.label_soundValue->setText(QString::number(soundVal));
	ui.label_speedValue->setText(QString::number(speedVal));
	ui.label_biasValue->setText(QString::number(biasVal));
}

void SettingsWidget::saveSettings()
{
	//save settings to variables
	musicVal = ui.slider_music->value();
	soundVal = ui.slider_sound->value();
	speedVal = ui.slider_speed->value();
	biasVal = ui.slider_bias->value();
	fpsVal = ui.comboBox_fps->currentText().toInt();
	key_1 = ui.keySequenceEdit_1->keySequence();
	key_2 = ui.keySequenceEdit_2->keySequence();
	key_3 = ui.keySequenceEdit_3->keySequence();
	key_4 = ui.keySequenceEdit_4->keySequence();

	//save settings to file
	QFile file("./settings.cfg");
	//It can always open the file for writing, 
	//because it is always created in the constructor
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug()<<"Could not open the file for writing";
		return;
	}
	QTextStream out(&file);
	out << "music: " << musicVal << "\n" <<
		"sound: " << soundVal << "\n" <<
		"speed: " << speedVal << "\n" <<
		"bias: " << biasVal << "\n" <<
		"fps: " << fpsVal << "\n" <<
		"key_1: " << key_1.toString() << "\n" <<
		"key_2: " << key_2.toString() << "\n" <<
		"key_3: " << key_3.toString() << "\n" <<
		"key_4: " << key_4.toString() << "\n";
	file.close();
}
