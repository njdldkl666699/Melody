#include "Note.h"

Note::Note(int strtTime, const QKeySequence& ky, const QString& soundPath,
		const QString& picturePath, const QSize& size, QLabel* parent)
	: QLabel(parent), startTime(strtTime), key(ky), picture(picturePath)
{
	this->setFixedSize(size);
	sound.setSource(QUrl::fromLocalFile(soundPath));
	picture = picture.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	this->setPixmap(picture);
}

Note::~Note()
{}
