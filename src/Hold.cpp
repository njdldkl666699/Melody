#include "Hold.h"

Hold::Hold(int strtTime, int edTime, const QKeySequence& ky, const QString& soundPath,
		const QString& picturePath, const QSize& size, QLabel* parent)
	:Note(strtTime, ky, soundPath, picturePath, size, parent), endTime(edTime), state(Miss)
{}

Hold::~Hold()
{}