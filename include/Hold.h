#pragma once

#include"Note.h"

class Hold : public Note
{
	Q_OBJECT

public:
	enum ToBeState
	{
		Perfect,
		Good,
		Miss
	};

public:
	Hold(int strtTime, int edTime, const QKeySequence& ky, const QString& soundPath,
		const QString& picturePath, const QSize& size, QLabel* parent = nullptr);
	~Hold();

	QString getType()const override { return "Hold"; }
	int getEndTime()const { return endTime; }
	ToBeState getState()const { return state; }
	void setState(ToBeState stat) { state = stat; }

private:
	int endTime;
	ToBeState state;
};
