/*
	<Source file of function main()>
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

#include <QApplication>
#include<QStyleFactory>
#include<QFontDatabase>
#include<QFile>
#include"SettingsWidget.h"

#ifdef _DEBUG
#include"MenuWidget.h"
#else 
#include"StartWidget.h"
#endif // _DEBUG

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	a.setStyle(QStyleFactory::create("Fusion"));

	QFontDatabase::addApplicationFont("./res/font/Saira-Regular.ttf");
	QFontDatabase::addApplicationFont("./res/font/SourceHanSansSC-Regular-2.otf");

#ifdef _DEBUG
	MenuWidget w;
	w.show();
#else
    StartWidget s;
	s.show();
#endif // _DEBUG

	return a.exec();
}
