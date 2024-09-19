#include "Note.h"

Note::Note(int strtTime, const QString& picturePath, 
	const QSize& size, QWidget* parent)
	: QLabel(parent), startTime(strtTime)
{
	this->setFixedSize(size);
	QPixmap picture(picturePath);
	picture = picture.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	this->setPixmap(picture);
}

Note::~Note()
{}
