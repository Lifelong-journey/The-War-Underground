#include "Weapon.h"
#include "Define.h"
#include <iostream>
#include <Windows.h>

using namespace std;

static int rNum[4] = { 17, 30, 42 }; // the same as U, M, D overhead

struct InitWeapon {
	int type;
	int power, time, aspeed, mspeed;
	int road, lx, len, rx;
	int tcount;
}itw[10];

void inititw()
{
	itw[1] = { 1, 200, INF, 1, 2, MIDROAD, 15, 10, 24, 0 }; // missile
	itw[2] = { 2, 15, 150, 20, 0, MIDROAD, 15, 20, 34, 0 }; // water
	itw[3] = { 3, 0, 50, 1, 0, MIDROAD, 15, 15, 29, 0 }; // emp
	itw[4] = { 4, 20, 100, 20, 0, MIDROAD, 15, 20, 34, 0 }; // nanobots
	itw[5] = { 5, 200, 300, 1, 0, MIDROAD, 15, 5, 19, 0 }; // mine
	itw[6] = { 6, 5, 1, 1, 0, MIDROAD, 15, 40, 54, 0 }; // Field Shield
}

Weapon::Weapon(int num, bool ply)
{
	player = ply;
	type = itw[num].type;
	power = itw[num].power;
	time = itw[num].time;
	aspeed = itw[num].aspeed;
	mspeed = itw[num].mspeed;
	road = itw[num].road;
	lx = itw[num].lx;
	len = itw[num].len;
	rx = itw[num].rx;
	tcount = itw[num].tcount;
}

void Weapon::setWeapon(int num, bool ply, int x)
{
	player = ply;
	type = itw[num].type;
	power = itw[num].power;
	time = itw[num].time;
	aspeed = itw[num].aspeed;
	mspeed = itw[num].mspeed;
	road = itw[num].road;
	lx = x;
	len = itw[num].len;
	rx = lx + len - 1;
	tcount = itw[num].tcount;
}

void Weapon::wDraw(wchar_t* screen)
{
	switch (type) {
		case 1:
			if (!player) {
				wsprintf(&screen[(rNum[road] - 3) * nScreenWidth + lx + 3], L">");
				wsprintf(&screen[(rNum[road] - 2) * nScreenWidth + lx], L"-<-=====>>");
				wsprintf(&screen[(rNum[road] - 1) * nScreenWidth + lx + 3], L">");
			}
			else {
				wsprintf(&screen[(rNum[road] - 3) * nScreenWidth + lx + 6], L"<");
				wsprintf(&screen[(rNum[road] - 2) * nScreenWidth + lx], L"<<=====->-");
				wsprintf(&screen[(rNum[road] - 1) * nScreenWidth + lx + 6], L"<");
			}
			break;
		case 2:
			for (int i = lx; i + 3 <= rx; i += 3)
			{
				screen[(rNum[road] - 7) * nScreenWidth + i] = L'~';
				screen[(rNum[road] - 4) * nScreenWidth + i] = L'~';
				screen[(rNum[road] - 1) * nScreenWidth + i] = L'~';
			}
			break;
		case 3:
			for (int i = lx; i + 3 <= rx; i += 3)
			{
				screen[(rNum[road] - 7) * nScreenWidth + i] = L'*';
				screen[(rNum[road] - 4) * nScreenWidth + i] = L'*';
				screen[(rNum[road] - 1) * nScreenWidth + i] = L'*';
			}
			break;
		case 4:
			for (int i = lx; i + 3 <= rx; i += 3)
			{
				screen[(rNum[road] - 7) * nScreenWidth + i] = L'+';
				screen[(rNum[road] - 4) * nScreenWidth + i] = L'+';
				screen[(rNum[road] - 1) * nScreenWidth + i] = L'+';
			}
			break;
		case 5:
			if (time >= 240) {
				wsprintf(&screen[(rNum[road] - 2) * nScreenWidth + lx + 1], L"_?_");
				wsprintf(&screen[(rNum[road] - 1) * nScreenWidth + lx], L"[   ]");
			}
			break;
		case 6:
			for (int i = 1; i <= 3; i++)
			{
				screen[(rNum[road] - i) * nScreenWidth + lx] = L'{';
				screen[(rNum[road] - i) * nScreenWidth + rx] = L'}';
			}
			break;
	}
}

int Weapon::getType()
{
	return type;
}

bool Weapon::getPlayer()
{
	return player;
}

int Weapon::getPower()
{
	return power;
}

int Weapon::getTime()
{
	return time;
}

int Weapon::getAspeed()
{
	return aspeed;
}

int Weapon::getMspeed()
{
	return mspeed;
}

int Weapon::getRoad()
{
	return road;
}

int Weapon::getLx()
{
	return lx;
}

int Weapon::getRx()
{
	return rx;
}

int Weapon::getTcount()
{
	return tcount;
}

void Weapon::addTcount()
{
	++tcount;
}

void Weapon::wMoveY(bool mdf)
{
	if (mdf) { // move up
		road = max(road - 1, UPROAD);
	}
	else {
		road = min(road + 1, DOWNROAD);
	}
}

void Weapon::wMoveX(int x, bool isready)
{
	lx += x;
	rx += x;
}

void Weapon::cutTime()
{
	--time;
}

void Weapon::resetTcount()
{
	tcount = 0;
}

void Weapon::setX(int x)
{
	lx = x;
	rx = x + len;
}

/*

[___]
  ?

  {   }
  {   }
  {   }
*/