#pragma once

#include"Note.h"

class Tap : public Note
{
	Q_OBJECT

public:
	Tap(int strtTime, QKeySequence ky, QLabel* parent = nullptr);
	~Tap();

	bool judge() override;

protected:
	void keyPressEvent(QKeyEvent* event)override;
};
