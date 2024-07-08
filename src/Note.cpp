#include "Note.h"

Note::Note(int strtTime, QKeySequence ky, QLabel* parent)
	: QLabel(parent), startTime(strtTime), key(ky)
{}

Note::~Note()
{}
