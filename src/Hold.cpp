/*
	<Source file of class Hold>
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

#include "Hold.h"

Hold::Hold(int strtTime, int edTime, const QString& picturePth,
	const QSize& sz, QWidget* parent)
	:Note(strtTime, picturePth, sz, parent), endTime(edTime),
	state(None), picturePath(picturePth), startSize(sz)
{}

Hold::~Hold()
{}

void Hold::paintEvent(QPaintEvent* event)
{
	/*
	If the hold is pressed, only draw the y < 625 part.
	If Hold is up the judge line, do nothing.
	Method: judge the bottom, if larger than 625, 
	resize the height and scale the pixmap.
	*/
	if (state == Perfect || state == Good)
	{
		int bottom = this->y() + this->height();
		int distance = bottom - 625;
		if (distance > 0)
		{
			// It means part of Hold is under the judge line.
			int newHeight = this->height() - distance;
			//or newHeight = 625 - this->y();
			if (newHeight > 0)
			{
				this->setFixedHeight(newHeight);
				this->setPixmap(this->pixmap().scaled(this->size(),
					Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
			}
			else
			{
				//the Hold is too low to draw, don't draw it.
				return;
			}
		}
	}
	return Note::paintEvent(event);
}

void Hold::reset()
{
	state = None;
	this->setFixedSize(startSize);
	QPixmap picture(picturePath);
	picture = picture.scaled(this->size(),
		Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	this->setPixmap(picture);
}