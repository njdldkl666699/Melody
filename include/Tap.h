#pragma once

#include"Note.h"

class Tap : public Note
{
	Q_OBJECT

public:
	Tap(int strtTime, QKeySequence ky, QLabel* parent = nullptr);
	~Tap();

	QString getType()const override { return "Tap"; }

protected:
};
