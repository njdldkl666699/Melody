#pragma once

#include <QObject>
#include<QKeySequence>
#include<QTimer>

class Note  : public QObject
{
	Q_OBJECT

public:
	Note(int strtTime, QKeySequence ky, QObject* parent = nullptr);
	~Note();

	int getStartTime()const { return startTime; }
	QKeySequence getKey()const { return key; }

	// judge if the note is hit, pure virtual function
	virtual bool judge() = 0;

protected:
	int startTime;
	QKeySequence key;
};
