#pragma once

#include<QKeySequence>
#include<QLabel>
#include<QSoundEffect>

class Note : public QLabel
{
	Q_OBJECT

public:
	Note(int strtTime, const QKeySequence& ky, const QString& soundPath,
		const QString& picturePath, const QSize& size, QLabel* parent = nullptr);
	virtual ~Note();

	int getStartTime()const { return startTime; }
	QKeySequence getKey()const { return key; }
	virtual QString getType()const = 0;

protected:
	int startTime;
	QKeySequence key;
	QSoundEffect sound;
	QPixmap picture;
};
