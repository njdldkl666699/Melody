#include "Note.h"

Note::Note(int strtTime, QKeySequence ky, QObject* parent)
	: QObject(parent), startTime(strtTime), key(ky)
{

}

Note::~Note()
{}
