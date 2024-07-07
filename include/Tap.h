#pragma once

#include"Note.h"

class Tap : public Note
{
	Q_OBJECT

public:
	Tap(int strtTime, QKeySequence ky, QObject* parent = nullptr);
	~Tap();

	bool judge() override;

private:
};
