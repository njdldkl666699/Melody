/*
	<Source file of class PauseWidget>
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

#include "PauseWidget.h"
#include "SettingsWidget.h"
#include "UIController.h"

PauseWidget::PauseWidget(QWidget* parent)
	:QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
	setWindowModality(Qt::WindowModal);
	setModal(true);
	initBackgroundGIF();

	connect(ui.pushButton_backMenu, &QPushButton::clicked, this, &PauseWidget::signalBackMenu);
	connect(ui.pushButton_restart, &QPushButton::clicked, this, &PauseWidget::signalRestart);
	connect(ui.pushButton_continue, &QPushButton::clicked, this, &PauseWidget::signalContinue);

	using namespace UICtrl;
	auto sw = SettingsWidget::instance();
	setObjectSound(ui.pushButton_backMenu, &QPushButton::clicked, ber, sw->getSoundVal());
	setObjectSound(ui.pushButton_restart, &QPushButton::clicked, ber, sw->getSoundVal());
	setObjectSound(ui.pushButton_continue, &QPushButton::clicked, ber, sw->getSoundVal());
}

PauseWidget::~PauseWidget()
{}

void PauseWidget::initBackgroundGIF()
{
	backgroundGIF = new QMovie("./res/background/Pause2.gif");
	ui.background->setMovie(backgroundGIF);
	backgroundGIF->start();
}

void PauseWidget::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		emit signalContinue();
	}
	else if (event->key() == Qt::Key_Space)
	{
		//do nothing
	}
	else
		return QDialog::keyPressEvent(event);
}