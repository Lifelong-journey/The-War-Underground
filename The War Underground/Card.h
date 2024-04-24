#pragma once
#ifndef CARD_H
#define CARD_H

class Card {
private:
	int player;
	int type;
	int lx, rx;

public:
	void cDraw(wchar_t*);
	void setCard(int, int, int);
};


#endif