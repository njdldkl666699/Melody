#pragma once

#include<QLabel>

class Note : public QLabel
{
	Q_OBJECT

public:
	Note(int strtTime, const QString& picturePath,
		const QSize& size, QWidget* parent = nullptr);
	virtual ~Note();

	int getStartTime()const { return startTime; }
	virtual QString getType()const = 0;
	int getAnimationIndex()const { return animationIndex; }
	void setAnimationIndex(int index) { animationIndex = index; }

protected:
	int startTime;
	int animationIndex;
};
