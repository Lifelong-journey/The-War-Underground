#include "Cleverai.h"
#include "Entity.h"
#include "Card.h"
#include "Define.h"
#include "Weapon.h"
#include "Gamestart.h"
#include "Timecount.h"

#include <queue>

double absoluteW[15] = { 0 };

void initAbsoluteW(Inittype* itp)
{
	for (int i = 0; i <= 10; i++)
	{
		absoluteW[i] = itp[i].life + (double)itp[i].power * itp[i].range / (itp[i].aspeed ? itp[i].aspeed : 10) * 10;
	}
	absoluteW[6] = 450;
	absoluteW[5] = 600;
}
// everything vs 6 = 450
double relativeC[13][13] =
{ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 1, 0.33, 0.13, 0.66, -1, -2, -1, 75, 0.66, 0.33},
	{1, 3, 1, 2.25, -1, -1, -2, 0.6, -1, 0.25, 2.9},
	{0, 7.69, 0.44, 1, -1, -2, 0.4, -1, 0.11, 4.3},
	{1, 1.5, 0, 0, 0, 0, -2, 0, -1, 0.33, 0},
	{1}, 
	{1}, 
	{1}, 
	{1}, 
	{1, 1.5, 4, 9.5, 3, -1, -2, -1, -1, 1, 6},
	{1, 3, 0.33, 0.22, -1, -2, -1, 0.05, 0.16, 1}
};

double getEW(Entity* e)
{
	if (e->getType() == 5)
		return absoluteW[5] * e->getLife() / 300;
	if (e->getType() == 6)
		return absoluteW[6];
	else
		return absoluteW[e->getType()] + e->getLife();
}


// If no entity on the road
// the bunker and missile is a waste
// the heavy artillery can be easily defensed
// other cards are ok
double getwNull(int type)
{
	if (type == 11 || type == 4)
		return 0;
	if (type == 7)
		return absoluteW[type] * 0.1;

	return absoluteW[type];
}

// If road isn't empty
// return the Weighted relative value
double getwFull(eNode* tt, int type)
{
	// Bomber and missile only need to check the first enemy and return its absolute value
	if (type == BOMBER || type == MISSILE) {
		for (auto i = tt; i != NULL; i = i->pre)
		{
			if (!i->e.getPlayer()) {
				return getEW(&i->e);
			}
		}
		return getwNull(type);
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
					return absoluteW[BOMBER];
				if (e.getType() == GUARD)
					return absoluteW[GUARD];

				return absoluteW[e.getType()] * relativeC[BUNKER][e.getType()];
			}
		}
		return 0;
	}

	// if no enemy before plant, return 600
	// else return 200
	if (type == PLANT) {
		if (!tt->e.getPlayer())
			return absoluteW[PLANT];
		else
			return absoluteW[PLANT] / 3;
	}

	// every friend directly before guard can increase guard's value
	if (type == GUARD) {
		double preFriendLife = 0;
		for (auto i = tt; i != NULL && i->e.getPlayer(); i = i->pre)
		{
			preFriendLife += i->e.getLife();
		}
		return absoluteW[GUARD] * preFriendLife / 50;
	}

	// other entities need to be calculate
	// the right first enemy has the weight of 10, the second has 8, the third has 4, others 0

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
			sumCW += relativeC[type][e.getType()] * x * w;
			sumW += x * w;
		}
	}
	if (sumCW == 0)
		return getwNull(type);

	double warvc = sumCW / sumW; // Weighted average relative value coefficient
	if (type == ARTILLERY) {
		if (tt->e.getPlayer())
			warvc *= 1.5;
		else
			warvc *= 0.5;
	}
	return warvc * absoluteW[type];
}

void FindMx(eNode* tt, int type, int crroad, int& choose, double& mxAverage, int& road, int num)
{
	double wacrv = 0; // Weighted average cost relative value
	if (tt == NULL) {
		wacrv = getwNull(type) / CardMineral[type];
		if (wacrv > mxAverage) {
			mxAverage = wacrv;
			choose = num;
			road = crroad;
		}
	}
	else {
		wacrv = getwFull(tt, type);
		if (wacrv / (double)CardMineral[type] > mxAverage) {
			mxAverage = wacrv / (double)CardMineral[type];
			choose = num;
			road = crroad;
		}
	}
}

double MinMax(int lMineral, int rMineral, eNode* hMid, eNode* hUp, eNode* hDown, Card* card, queue<int>* lsq, queue<int>* rsq, int& choose, int& road, TimeCount* t, int depth, int lmissile, int rmissile)
{
	if (depth % 2) {
		for (int i = 6; i <= 10; i++) {
			eNode* en = hUp;
			while (en != NULL && en->ne != NULL)
				en = en->ne;
			eNode* ep = new eNode();
			ep->e.setEntity(card[i].getType(), 1, 150);
			if (en == NULL) {// road is empty
				hUp = ep;
				ep->pre = ep->ne = NULL;

			}
		}
	}
}


bool Search(int lMineral, int& rMineral, eNode* hMid, eNode* hUp, eNode* hDown, Card* card, queue<int>* lsq, queue<int>* rsq, int& choose, int& road, TimeCount* t)
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
			FindMx(tMid, type, MIDROAD, choose, mxAverage, road, i);
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
		FindMx(tMid, type, MIDROAD, choose, mxAverage, road, i);
		FindMx(tUp, type, UPROAD, choose, mxAverage, road, i);
		FindMx(tDown, type, DOWNROAD, choose, mxAverage, road, i);
	}
	// if mxaverage is too small, then do nothing
	if (mxAverage < MOTIONLESS && rMineral < 10 || mxAverage == 0)
		return false;
	return true;
}