#include "Tap.h"

Tap::Tap(int strtTime, const QKeySequence& ky, const QString& picturePath,
	const QSize& size, QLabel* parent)
	: Note(strtTime, ky, picturePath, size, parent)
{}

Tap::~Tap()
{}