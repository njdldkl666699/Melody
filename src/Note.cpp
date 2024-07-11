#include "Note.h"

Note::Note(int strtTime, const QKeySequence& ky, const QString& picturePath, 
	const QSize& size, QLabel* parent)
	: QLabel(parent), startTime(strtTime), key(ky)
{
	this->setFixedSize(size);
	QPixmap picture(picturePath);
	picture = picture.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	this->setPixmap(picture);
}

Note::~Note()
{}
