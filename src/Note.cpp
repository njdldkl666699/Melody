#include "Note.h"

Note::Note(int strtTime, const QKeySequence& ky, const QString& soundPath,
		const QString& picturePath, const QSize&size,QLabel* parent)
	: QLabel(parent), startTime(strtTime), key(ky)
{
	this->setFixedSize(size);
	sound.setSource(QUrl::fromLocalFile(soundPath));
	QPixmap picture(picturePath);
	picture = picture.scaled(this->size());
	this->setPixmap(QPixmap(picturePath));
}

Note::~Note()
{}
