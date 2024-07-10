#include "Hold.h"

Hold::Hold(int strtTime, int edTime, const QKeySequence& ky, const QString& soundPath,
		const QString& picturePath, const QSize& size, QLabel* parent)
	:Note(strtTime, ky, soundPath, picturePath, size, parent), endTime(edTime), state(None)
{}

Hold::~Hold()
{}

void Hold::paintEvent(QPaintEvent* event)
{
	if (state != None && state != Miss)
	{
		int bottom = this->y() + this->height();
		if (bottom > 625)
		{
			int newHeight = 625 - this->y();
			if (newHeight > 0)
			{
				this->setFixedHeight(625 - this->y());
				this->setPixmap(this->pixmap().scaled(this->size(),
					Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
			}
		}
	}
	return Note::paintEvent(event);
}