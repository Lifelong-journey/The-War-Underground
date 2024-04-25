#pragma once
#ifndef WEAPON_H
#define WEAPON_H

#include <iostream>

using namespace std;

class Weapon {
private:
	int type;
	bool player;
	int power, time, aspeed, mspeed;
	int road, lx, len, rx;
	int tcount;

public:
	Weapon() = default;
	Weapon(int, bool);
	int getType();
	bool getPlayer();
	int getPower();
	int getTime();
	int getAspeed();
	int getMspeed();
	int getRoad();
	int getLx();
	int getRx();
	int getTcount();

	void addTcount();
	void wDraw(wchar_t*);
	void wMoveX(int, bool);
	void wMoveY(bool);
	void setWeapon(int, bool, int);
	void cutTime();
	void resetTcount();
	void setX(int);
};

struct wNode {
	Weapon w;
	wNode* ne;
	wNode* pre;
};

void inititw();
#endif
