#include "Tap.h"

Tap::Tap(int strtTime, const QString& picturePath,
	const QSize& size, QWidget* parent)
	: Note(strtTime, picturePath, size, parent)
{}

Tap::~Tap()
{}