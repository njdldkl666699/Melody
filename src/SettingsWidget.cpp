/*
	<Source file of class SettingsWidget>
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

#include "SettingsWidget.h"
#include "UIController.h"
#include<QSlider>
#include<QDesktopServices>

#ifdef _DEBUG
#include<cassert>
#endif // _DEBUG

SettingsWidget* SettingsWidget::settingsWidget = nullptr;

SettingsWidget::SettingsWidget(QWidget* parent)
	: QWidget(parent), backgroundPic("./res/background/settings.png"),
	musicVal(100), soundVal(100), speedVal(0), biasVal(0), fpsVal(60),
	key_1(Qt::Key_S), key_2(Qt::Key_F), key_3(Qt::Key_J), key_4(Qt::Key_L),
	isFullscreen(false)
{
	ui.setupUi(this);

	initBackground();
	initSettings();
	initUIValues();
	initWindow();

	connect(ui.pushButton_backMenu, &QPushButton::clicked, this, [this]()
		{
			emit pushButtonBackMenuClicked();
			this->saveSettings();
			this->hide();
		});
	connect(ui.pushButton_license, &QPushButton::clicked, this, []()
		{
			QDesktopServices::openUrl(QUrl::fromLocalFile("./LICENSE.txt"));
		});

	//check if keys are valid
	connect(ui.keySequenceEdit_1, &QKeySequenceEdit::keySequenceChanged, this, &SettingsWidget::onKeyEdited);
	connect(ui.keySequenceEdit_2, &QKeySequenceEdit::keySequenceChanged, this, &SettingsWidget::onKeyEdited);
	connect(ui.keySequenceEdit_3, &QKeySequenceEdit::keySequenceChanged, this, &SettingsWidget::onKeyEdited);
	connect(ui.keySequenceEdit_4, &QKeySequenceEdit::keySequenceChanged, this, &SettingsWidget::onKeyEdited);
}

void SettingsWidget::initBackground()
{
	backgroundPic = backgroundPic.scaled(this->size(), Qt::KeepAspectRatioByExpanding);
	ui.background->setPixmap(backgroundPic);
	ui.background->show();
}

void SettingsWidget::initWindow()
{
	setWindowTitle(QString::fromLocal8Bit("“Ù¡Èª√’¬Meolide"));
	setWindowIcon(QIcon("./res/icon/icon.ico"));
	using namespace UICtrl;
	setIfFullscreen(this, isFullscreen);
	setObjectSound(ui.slider_bias, &QSlider::sliderMoved, gear, soundVal);
	setObjectSound(ui.slider_music, &QSlider::sliderMoved, gear, soundVal);
	setObjectSound(ui.slider_sound, &QSlider::sliderMoved, gear, soundVal);
	setObjectSound(ui.slider_speed, &QSlider::sliderMoved, gear, soundVal);
	setObjectSound(ui.comboBox_fps, &QComboBox::highlighted, gear, soundVal);
	setObjectSound(ui.pushButton_backMenu, &QPushButton::clicked, ber, soundVal);
	setObjectSound(ui.checkBox_fullscreen, &QCheckBox::checkStateChanged, ber, soundVal);
	setObjectSound(ui.keySequenceEdit_1, &QKeySequenceEdit::keySequenceChanged, ber, soundVal);
	setObjectSound(ui.keySequenceEdit_2, &QKeySequenceEdit::keySequenceChanged, ber, soundVal);
	setObjectSound(ui.keySequenceEdit_3, &QKeySequenceEdit::keySequenceChanged, ber, soundVal);
	setObjectSound(ui.keySequenceEdit_4, &QKeySequenceEdit::keySequenceChanged, ber, soundVal);
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
			"key_4: " << key_4.toString() << "\n" <<
			"isFullscreen: " << isFullscreen << "\n";
		newFile.close();
		return;
	}
	//load Settings
	QTextStream in(&file);
	QString key, val;
	while (!in.atEnd())
	{
		in >> key >> val;
		if (key == "music:")
			musicVal = val.toInt();
		else if (key == "sound:")
			soundVal = val.toInt();
		else if (key == "speed:")
			speedVal = val.toInt();
		else if (key == "bias:")
			biasVal = val.toInt();
		else if (key == "fps:")
			fpsVal = val.toInt();
		else if (key == "key_1:")
			key_1 = QKeySequence(val);
		else if (key == "key_2:")
			key_2 = QKeySequence(val);
		else if (key == "key_3:")
			key_3 = QKeySequence(val);
		else if (key == "key_4:")
			key_4 = QKeySequence(val);
		else if (key == "isFullscreen:")
			isFullscreen = val.toInt();

		//qDebug() << line;
		//if (line.indexOf("music: ") != -1)
		//	musicVal = line.mid(7).toInt();
		//else if (line.indexOf("sound: ") != -1)
		//	soundVal = line.mid(7).toInt();
		//else if (line.indexOf("speed: ") != -1)
		//	speedVal = line.mid(7).toInt();
		//else if (line.indexOf("bias: ") != -1)
		//	biasVal = line.mid(6).toInt();
		//else if (line.indexOf("fps: ") != -1)
		//	fpsVal = line.mid(5).toInt();
		//else if (line.indexOf("key_1: ") != -1)
		//	key_1 = QKeySequence(line.mid(7));
		//else if (line.indexOf("key_2: ") != -1)
		//	key_2 = QKeySequence(line.mid(7));
		//else if (line.indexOf("key_3: ") != -1)
		//	key_3 = QKeySequence(line.mid(7));
		//else if (line.indexOf("key_4: ") != -1)
		//	key_4 = QKeySequence(line.mid(7));
		//else if (line.indexOf("isFullscreen: ") != -1)
		//	isFullscreen = line.mid(14).toInt();
		//line = in.readLine();

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
	//set fullscreen check box
	ui.checkBox_fullscreen->setChecked(isFullscreen);
	ui.label_restart->hide();
	ui.label_resetKey->hide();
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
	isFullscreen = ui.checkBox_fullscreen->isChecked();

	//save settings to file
	QFile file("./settings.cfg");
	//It can always open the file for writing, 
	//because it is always created in the constructor
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "Could not open the file for writing";
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
		"key_4: " << key_4.toString() << "\n" <<
		"isFullscreen: " << isFullscreen << "\n";
	file.close();
}

void SettingsWidget::onKeyEdited(const QKeySequence& keySequence)
{
	bool isValid = true;
	const QKeyCombination& comb = keySequence[0];
	qDebug() << comb.key() << comb.keyboardModifiers();
	if (comb.key() == 0)
	{
		//I don't know why the console prints Qt::Key(0)
		qDebug() << "Invalid Key";
		return;
	}
	if (comb.keyboardModifiers() != Qt::NoModifier ||
		comb.key() == Qt::Key_Escape)
		isValid = false;

	if (!isValid)
	{
		ui.pushButton_backMenu->hide();
		ui.label_resetKey->show();
        auto keySequenceEdit = qobject_cast<QKeySequenceEdit*>(sender());
		if (keySequenceEdit != nullptr)
			keySequenceEdit->clear();
	}
	else
	{
		ui.pushButton_backMenu->show();
		ui.label_resetKey->hide();
	}
}