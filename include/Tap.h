#pragma once

#include"Note.h"

class Tap : public Note
{
	Q_OBJECT

public:
	Tap(int strtTime,const QString& picturePath,
		const QSize& size, QWidget* parent = nullptr);
	~Tap();

	QString getType()const override { return "Tap"; }
};
