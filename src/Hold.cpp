#include "Hold.h"

Hold::Hold(int strtTime, int edTime, QKeySequence ky, QObject* parent)
	:Note(strtTime, ky, parent), endTime(edTime)
{

}

Hold::~Hold()
{}

bool Hold::judge()
{
	//to be implemented
	return true;
}
