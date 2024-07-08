#include "Hold.h"

Hold::Hold(int strtTime, int edTime, QKeySequence ky, QLabel* parent)
	:Note(strtTime, ky, parent), endTime(edTime), state(Miss)
{

}

Hold::~Hold()
{}