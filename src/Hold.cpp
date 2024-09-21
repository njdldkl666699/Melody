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