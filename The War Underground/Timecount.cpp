#include "Timecount.h"

void TimeCount::addMs()
{
	ms += 50;
	if (ms == 1000) {
		ms = 0;
		s++;
	}
	if (s == 60) {
		s = 0;
		m++;
	}
}

int TimeCount::getS()
{
	return s;
}

int TimeCount::getM()
{
	return m;
}

bool TimeCount::isGameEnd()
{
	return m > 2 || m >= 2 && s >= 30;
}