#include "Tap.h"

Tap::Tap(int strtTime, QKeySequence ky, QLabel* parent)
	: Note(strtTime, ky, parent)
{

}

Tap::~Tap()
{}

bool Tap::judge()
{
	//to be implemented
	return true;
}
