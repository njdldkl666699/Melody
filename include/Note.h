#pragma once

#include<QKeySequence>
#include<QKeyEvent>
#include<QTimer>
#include<QTime>
#include<QLabel>
#include<QSoundEffect>

class Note  : public QLabel
{
	Q_OBJECT

public:
	Note(int strtTime, QKeySequence ky, QLabel* parent = nullptr);
	~Note();

	int getStartTime()const { return startTime; }
	QKeySequence getKey()const { return key; }

	// judge if the note is hit, pure virtual function
	virtual bool judge() = 0;

protected:
	int startTime;
	QKeySequence key;
	QSoundEffect sound;
};
