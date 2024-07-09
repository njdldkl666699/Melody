#include "Tap.h"

Tap::Tap(int strtTime, const QKeySequence& ky, const QString& soundPath,
		const QString& picturePath, const QSize& size, QLabel* parent)
	: Note(strtTime, ky, soundPath, picturePath, size, parent)
{

}

Tap::~Tap()
{}