#include "Foolai.h"
#include "Entity.h"
#include "Card.h"
#include "Define.h"
#include "Weapon.h"
#include "Gamestart.h"
#include "Timecount.h"

#include <queue>

#define BASE 0
#define SOLDIER 1
#define TANK 2
#define BAZOOKA 3
#define BUNKER 4
#define PLANT 5
#define BOMBER 6
#define ARTILLERY 7
#define GUARD 8
#define SONIC 9
#define FIRE 10
#define MISSILE 11

double fabsoluteW[15] = { 0 };
double life[15] = { 0 };

void fInitAbsoluteW()
{
	for (int i = 1; i <= 10; i++)
	{
		fabsoluteW[i] = (double)itp[i].power * itp[i].range / (itp[i].aspeed ? itp[i].aspeed : 10) * 10;
		life[i] = itp[i].life;
	}
	fabsoluteW[6] = 300;
	fabsoluteW[5] = 300;
	fabsoluteW[0] = 1000;
}
// everything vs 6 = 450
double frelativeC[13][13] =
{ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 1, 0.33, 0.13, 0.66, 8, 4, 10, 1, 0.66, 0.33},
	{1, 3, 1, 2.25, 1, 1.5, 1, 0.41, 0.2, 0.25, 2.9},
	{0, 7.69, 0.44, 1, -1, -2, 0.22, -1, 0.11, 4.3},
	{1, 1.5, 0, 0, 0, 0, -2, 0, -1, 0.33, 0},
};



double fFindMx(eNode* tt, int type, Card* card, Card* ln, int mineralNeeded)
{
	double wacrv1 = 0; // Weighted average cost relative value

	bool lhasb = false; // if left player has bomber or missile
	double preFriendLife = 0, preEnemyW = 0;

	for (int i = 1; i <= 5; i++)
		if (card[i].getType() == MISSILE || card[i].getType() == BOMBER)
			lhasb = true;
	if (ln->getType() == MISSILE || ln->getType() == BOMBER)
		lhasb = true;

	// to find the friend life before
	for (auto i = tt; i != NULL; i = i->pre) 
	{
		int ttype = i->e.getType();
		if (!i->e.getPlayer()) { // to find the right first enemy value and find if it is worth to set missile or bomber	
			if (ttype == BOMBER)
				preEnemyW = 300;
			else if (i->e.getLife() > 200)
				preEnemyW = 200;
			else
				preEnemyW = i->e.getLife() + fabsoluteW[ttype];
			break;
		}

		if (ttype == BOMBER || ttype == PLANT || ttype == SOLDIER) // can't protect our entity
			continue;
		else
			preFriendLife += i->e.getLife();
	}

	switch (type) {
	case PLANT:
		if (preFriendLife > 1e-5) // have friend protected
			wacrv1 = 600;
		else if (preEnemyW < 1e-5 && !lhasb) // no enemy && left player doesn't hold bomb
			wacrv1 = 600;
		else
			wacrv1 = 200;
		break;

	case BOMBER:
		wacrv1 = preEnemyW;
		break;

	case MISSILE:
		wacrv1 = preEnemyW;
		for (auto i = tt; i != NULL; i = i->pre) 
		{
			if (!i->e.getPlayer() && i->e.getType() == SOLDIER && i->ne != NULL && i->ne->e.getPlayer() && i->ne->e.getType() == ARTILLERY) {
				wacrv1 = i->ne->e.getLife() + fabsoluteW[ARTILLERY];
				break;
			}
		}

		break;

	case GUARD:
		wacrv1 = preFriendLife; // guard value depend on the first friend life before it
		break;

	case ARTILLERY:
		if (preFriendLife > 100)
			wacrv1 = (200 + fabsoluteW[ARTILLERY]) * 2;
		else
			wacrv1 = (double)(200 + fabsoluteW[ARTILLERY]) / 4;
		break;

	case BAZOOKA:
		if (preFriendLife > 100)
			wacrv1 = (life[BAZOOKA] + fabsoluteW[BAZOOKA]) * 1.5;
		else
			wacrv1 = (double)(200 + fabsoluteW[BAZOOKA]) / 2;
		break;

	case SONIC:
		if (tt == NULL && !lhasb)
			wacrv1 = (life[SONIC] + fabsoluteW[SONIC]) * 0.8;
		else if ((!lhasb || preFriendLife > 50) && tt->e.getType() != BOMBER && preEnemyW > 1e-5)
			wacrv1 = (life[SONIC] + fabsoluteW[SONIC]) * 3;
		else
			wacrv1 = (double)(life[SONIC] + fabsoluteW[SONIC]) / 2;
		break;

	default:
		for (auto i = tt; i != NULL; i = i->pre)
		{
			if (!i->e.getPlayer()) {
				wacrv1 = (double)(life[type] + fabsoluteW[type]) * frelativeC[type][i->e.getType()];
				break;
			}
		}
	}

	switch (mineralNeeded) {
	case 1:
		wacrv1 *= 0.95;
		break;
	case 2:
		wacrv1 *= 0.9;
		break;
	case 3:
		wacrv1 *= 0.8;
		break;
	case 4:
		wacrv1 *= 0.6;
		break;
	default:
		wacrv1 *= 0.3;
	}
	return wacrv1;
}

double fEndGame(eNode* tt, int type, Card* card, int mineralNeeded)
{
	return 0;
}


bool fSearch(int rMineral, eNode* hMid, eNode* hUp, eNode* hDown, Card* card, Card* ln, int& choose, int& road, TimeCount* t)
{
	//int choose = 0;
	double mxAverage = 0;
	int mn = 0;
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


	for (int i = 6; i <= 10; i++)
	{
		double craw = 0;
		int crmn = rMineral >= card[i].getMineral() ? 0 : card[i].getMineral() - rMineral;
		int type = card[i].getType();
		//if (t->getM() * 60 + t->getS() > 150) { // if endgame, then change strategy
		//	craw = fEndGame(tMid, type, card, crmn);
		//}
		craw = fFindMx(tMid, type, card, ln, crmn) / card[i].getMineral();
		if (craw > mxAverage) {
			mxAverage = craw;
			mn = crmn;
			choose = i;
			road = MIDROAD;
		}
		if (t->getM() * 60 + t->getS() <= 140) { // if there are only 10s from the end game, then only consider the midroad
			craw = fFindMx(tUp, type, card, ln, crmn) / card[i].getMineral();
			if (craw > mxAverage) {
				mxAverage = craw;
				mn = crmn;
				choose = i;
				road = UPROAD;
			}
			craw = fFindMx(tDown, type, card, ln, crmn) / card[i].getMineral();
			if (craw > mxAverage) {
				mxAverage = craw;
				mn = crmn;
				choose = i;
				road = DOWNROAD;
			}
		}

	}
	// if mxaverage is too small, then do nothing
	if (mn)
		return false;
	return true;
}