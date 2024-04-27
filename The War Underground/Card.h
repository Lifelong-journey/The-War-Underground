#pragma once
#ifndef CARD_H
#define CARD_H

class Card {
private:
	int player;
	int type;
	int lx, rx;
	int mineral;

public:
	void cDraw(wchar_t*);
	void setCard(int, int, int);
	int getPlayer();
	int getType();
	int getLx();
	int getMineral();
};
const int CardMineral[] = { 0, 2, 5, 5, 4, 6, 3, 6, 3, 5, 4, 3, 4, 4, 3, 2 };

void PrintCard(wchar_t*, int, int, int, int);

#endif