#pragma once

#include"Note.h"
#include<QPainter>

class Hold : public Note
{
	Q_OBJECT

public:
	enum ToBeState
	{
		Perfect,
		Good,
		Miss,
		None
	};

public:
	Hold(int strtTime, int edTime,const QString& picturePth,
		const QSize& sz, QWidget* parent = nullptr);
	~Hold();

	QString getType()const override { return "Hold"; }
	int getEndTime()const { return endTime; }
	ToBeState getState()const { return state; }
	void setState(ToBeState stat) { state = stat; }
	void reset();

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	int endTime;
	ToBeState state;
	const QString picturePath;
	const QSize startSize;
};
