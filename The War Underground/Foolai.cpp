#include "Foolai.h"
#include "Entity.h"
#include "Card.h"
#include "Define.h"
#include "Weapon.h"
#include "Gamestart.h"
#include "Timecount.h"

#include <queue>

double fabsoluteW[15] = { 0 };

void fInitAbsoluteW(Inittype* itp)
{
	for (int i = 0; i <= 10; i++)
	{
		fabsoluteW[i] = (double)itp[i].power * itp[i].range / (itp[i].aspeed ? itp[i].aspeed : 10) * 10;
	}
	fabsoluteW[6] = 450;
	fabsoluteW[5] = 600;
}
// everything vs 6 = 450
double frelativeC[13][13] =
{ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 1, 0.33, 0.13, 0.66, -1, -2, -1, 75, 0.66, 0.33},
	{1, 3, 1, 2.25, -1, -1, -2, 0.41, -1, 0.25, 2.9},
	{0, 7.69, 0.44, 1, -1, -2, 0.22, -1, 0.11, 4.3},
	{1, 1.5, 0, 0, 0, 0, -2, 0, -1, 0.33, 0},
	{1}, // if no misile && no enemy before, 600, else 200
	{1}, // if life < 300, w, else, 300
	{1}, // if friend before, 1x, else 0.1x
	{1}, // if friend before, 1.5x friendlife; else 0
	{1, 1.5, 4, 9.5, 3, -1, -2, -1, -1, 1, 6},
	{1, 3, 0.33, 0.22, -1, -2, -1, 0.05, 0.16, 1}
};

double fgetEW(Entity* e)
{
	if (e->getType() == 5)
		return fabsoluteW[5] * e->getLife() / 300;
	if (e->getType() == 6)
		return fabsoluteW[6];
	else
		return fabsoluteW[e->getType()] + e->getLife();
}


// If no entity on the road
// the bunker and missile is a waste
// the heavy artillery can be easily defensed
// other cards are ok
double fgetwNull(int type)
{
	if (type == 11 || type == 4)
		return 0;
	if (type == 7)
		return fabsoluteW[type] * 0.1;
	
	return fabsoluteW[type];
}

// If road isn't empty
// return the Weighted relative value
double fgetwFull(eNode* tt, int type)
{
	// Bomber and missile only need to check the first enemy and return its absolute value
	if (type == BOMBER || type == MISSILE) {
		for (auto i = tt; i != NULL; i = i->pre)
		{
			if (!i->e.getPlayer()) {
				return fgetEW(&i->e);
			}
		}
		return fgetwNull(type);
	}
	// bunker only need to check the first enemy
	// if out of range, return 0
	// if no enemy, return 0
	
	if (type == BUNKER) {
		for (auto i = tt; i != NULL; i = i->pre)
		{
			auto e = i->e;
			if (!e.getPlayer()) {
				if (e.getType() == BOMBER)
					return fabsoluteW[BOMBER];
				if (e.getType() == GUARD)
					return fabsoluteW[GUARD];

				return fabsoluteW[e.getType()] * frelativeC[GUARD][e.getType()];
			}
		}
		return 0;
	}

	// if no enemy before plant, return 600
	// else return 200
	if (type == PLANT) {
		if (!tt->e.getPlayer())
			return fabsoluteW[PLANT];
		else
			return fabsoluteW[PLANT] / 3;
	}

	// every friend directly before guard can increase guard's value
	if (type == GUARD) {
		double preFriendLife = 0;
		for (auto i = tt; i != NULL && i->e.getPlayer(); i = i->pre)
		{
			preFriendLife += i->e.getLife();
		}
		return fabsoluteW[GUARD] * preFriendLife / 40;
	}

	// other entities need to be calculate
	double preFriendW = 0;
	for (auto i = tt; i != NULL && i->e.getPlayer(); i = i->pre)
	{
		if (i->e.getType() != 4)
			preFriendW += tt->e.getLife() + (double)tt->e.getPower() * tt->e.getRange() / (tt->e.getAspeed() ? tt->e.getAspeed() : 10) * 10;
	}
	double sumW = 0, sumCW = 0;
	int cnt = 0;
	for (auto i = tt; i != NULL; i = i->pre)
	{
		auto e = i->e;
		if (!e.getPlayer()) {
			double w = e.getLife() + (double)e.getPower() * e.getRange() / (e.getAspeed() ? e.getAspeed() : 10) * 10;
			cnt++;
			int x = 0;
			switch (cnt) {
			case 1:
				x = 10;
				break;
			case 2:
				x = 8;
				break;
			case 3:
				x = 4;
				break;
			}
			sumCW += frelativeC[type][e.getType()] * x * w;
			sumW += x * w;
		}	
	}
	if (sumCW == 0)
		return fgetwNull(type);

	double warvc = sumCW / sumW; // Weighted average relative value coefficient
	return warvc *fabsoluteW[type];
}

void fFindMx(eNode* tt, int type, int crroad, int& choose, double& mxAverage, int& road, int num)
{
	double wacrv = 0; // Weighted average cost relative value
	if (tt == NULL) {
		wacrv = fgetwNull(type) / CardMineral[type]; 
		if (wacrv > mxAverage) {
			mxAverage = wacrv;
			choose = num;
			road = crroad;
		}
	}
	else {
		wacrv = fgetwFull(tt, type);
		if (wacrv / (double)CardMineral[type] > mxAverage) {
			mxAverage = wacrv / (double)CardMineral[type];
			choose = num;
			road = crroad;
		}
	}

}


bool fSearch(int lMineral, int& rMineral, eNode* hMid, eNode* hUp, eNode* hDown, Card* card, queue<int>* lsq, queue<int>* rsq, int& choose, int& road, TimeCount* t)
{
	//int choose = 0;
	double mxAverage = 0;
	//int road = -1;
	auto tUp = hUp, tMid = hMid, tDown = hDown;
	if (tUp != NULL) {
		while (tUp->ne != NULL)
			tUp = tUp->ne;
	}
	if (tDown != NULL) {
		while (tDown->ne != NULL)
			tDown = tDown->ne;
	}
	while (tMid->ne != NULL)
		tMid = tMid->ne;
	if (tMid->e.getType() == 0)
		tMid = tMid->pre;
	if (tMid->e.getType() == 0)
		tMid = NULL;
	double mW = 0;
	for (auto i = hMid; i != NULL; i = i->ne)
	{
		auto e = i->e;
		if (e.getType() == 0)
			continue;
		if (e.getPlayer() == 0)
			mW -= e.getLife() + (double)e.getPower() * e.getRange() / (e.getAspeed() ? e.getAspeed() : 10) * 10;
		else
			mW += e.getLife() + (double)e.getPower() * e.getRange() / (e.getAspeed() ? e.getAspeed() : 10) * 10;
	}

	// the base is dangerous
	if (mW < -800 || t->getM() * 60 + t->getS() >= 150) {
		for (int i = 6; i <= 10; i++)
		{
			if (rMineral < card[i].getMineral())
				continue;
			int type = card[i].getType();
			fFindMx(tMid, type, MIDROAD, choose, mxAverage, road, i);
		}
		if (mxAverage == 0)
			return false;
		return true;
	}


	for (int i = 6; i <= 10; i++)
	{
		if (rMineral < card[i].getMineral())
			continue;
		int type = card[i].getType();
		fFindMx(tMid, type, MIDROAD, choose, mxAverage, road, i);
		fFindMx(tUp, type, UPROAD, choose, mxAverage, road, i);
		fFindMx(tDown, type, DOWNROAD, choose, mxAverage, road, i);
	}
	// if mxaverage is too small, then do nothing
	if (mxAverage < MOTIONLESS && rMineral < 10 || mxAverage == 0)
		return false;
	return true;
}