#pragma once

#include"Note.h"

class Tap : public Note
{
	Q_OBJECT

public:
	Tap(int strtTime, const QKeySequence& ky, const QString& picturePath,
		const QSize& size, QLabel* parent = nullptr);
	~Tap();

	QString getType()const override { return "Tap"; }

protected:
};
