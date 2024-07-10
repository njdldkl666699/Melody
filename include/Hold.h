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
	Hold(int strtTime, int edTime, const QKeySequence& ky, const QString& soundPath,
		const QString& picturePath, const QSize& size, QLabel* parent = nullptr);
	~Hold();

	QString getType()const override { return "Hold"; }
	int getEndTime()const { return endTime; }
	ToBeState getState()const { return state; }
	void setState(ToBeState stat) { state = stat; }

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	int endTime;
	ToBeState state;
};
