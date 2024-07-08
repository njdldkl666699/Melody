#pragma once

#include"Note.h"

class Hold : public Note
{
	Q_OBJECT

public:
	Hold(int strtTime, int edTime, QKeySequence ky, QLabel* parent = nullptr);
	~Hold();

	bool judge() override;

private:
	int endTime;
};
