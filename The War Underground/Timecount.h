#pragma once
#ifndef TIMECOUNT_H
#define TIMECOUNT_H

class TimeCount {
private:
	int m, s;
	int ms;

public:
	TimeCount() : m{ 0 }, s{ 0 }, ms{ 0 } {};
	void addMs();
	int getM();
	int getS();
	bool isGameEnd();
};

#endif