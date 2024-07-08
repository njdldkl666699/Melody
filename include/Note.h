#pragma once

#include<QKeySequence>
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
	
	virtual QString getType()const = 0;

protected:
	int startTime;
	QKeySequence key;
	QSoundEffect sound;
};
