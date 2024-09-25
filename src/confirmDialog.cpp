/*
	<Source file of class ConfirmDialog>
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

#include "ConfirmDialog.h"
#include "UIController.h"
#include "SettingsWidget.h"

ConfirmDialog::ConfirmDialog(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
	setWindowModality(Qt::WindowModal);
	this->setModal(true);

	using namespace UICtrl;
	const int soundVal = SettingsWidget::instance()->getSoundVal();
	setObjectSound(ui.pushButton_exit, &QPushButton::clicked, ber, soundVal);
	setObjectSound(ui.pushButton_backMenu, &QPushButton::clicked, gear, soundVal);

	QPixmap backgroundPNG("./res/background/exitDialog.png");
	backgroundPNG = backgroundPNG.scaled(this->size(),
		Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	ui.background->setPixmap(backgroundPNG);

	connect(ui.pushButton_exit, &QPushButton::clicked,
		this, &ConfirmDialog::on_pushButton_exit_clicked);
	connect(ui.pushButton_backMenu, &QPushButton::clicked,
		this, &ConfirmDialog::on_pushButton_backMenu_clicked);
}

ConfirmDialog::~ConfirmDialog()
{}

void ConfirmDialog::on_pushButton_backMenu_clicked()
{
	//emit backToMenu();
	reject();
}

void ConfirmDialog::on_pushButton_exit_clicked()
{
	//accept();
	accept();
}