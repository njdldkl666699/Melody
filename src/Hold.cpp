#include "Hold.h"

Hold::Hold(int strtTime, int edTime, const QString& picturePath, 
	const QSize& size, QWidget* parent)
	:Note(strtTime, picturePath, size, parent), endTime(edTime), state(None)
{}

Hold::~Hold()
{}

void Hold::paintEvent(QPaintEvent* event)
{
	//####need to be implemented####
	if (state == Perfect || state == Good)
	{
		int bottom = this->y() + this->height();
		int distance = bottom - 625;
		if (distance < 0)
		{
			this->move(this->x(), this->y() - distance);
		}
		else if (distance >= 0)
		{
			//int newHeight = 625 - this->y();
			int newHeight = this->height() - distance;
			if (newHeight > 0)
			{
				this->setFixedHeight(newHeight);
				this->setPixmap(this->pixmap().scaled(this->size(),
					Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
			}
		}
	}
	return Note::paintEvent(event);
}