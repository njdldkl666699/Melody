/*
	<Header file of class Hold>
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

#pragma once

#include"Note.h"
#include<QPainter>

class Hold : public Note
{
	Q_OBJECT

public:
	enum ToBeState
	{
		Perfect,
		Good,
		Miss,
		None
	};

public:
	Hold(int strtTime, int edTime,const QString& picturePth,
		const QSize& sz, QWidget* parent = nullptr);
	~Hold();

	QString getType()const override { return "Hold"; }
	int getEndTime()const { return endTime; }
	ToBeState getState()const { return state; }
	void setState(ToBeState stat) { state = stat; }
	void reset();

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	int endTime;
	ToBeState state;
	const QString picturePath;
	const QSize startSize;
};
