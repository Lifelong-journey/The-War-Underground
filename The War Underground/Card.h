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


#endif