#include "Cleverai.h"
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
#define GOLIATH 10
#define MISSILE 13

typedef pair<int, int> PII;

double absoluteW[15] = { 0 };
Entity eRoad[10];



void InitAbsoluteW()
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

double calNull(Entity e, double fl)
{
	switch (e.getType()) {
	case MISSILE:
		return 0;
	case BOMBER:
		return 300;
	case BUNKER:
		return 0;
	case PLANT:
		return (double)e.getLife() / 300 * 600;
	case GUARD:
		return fl * absoluteW[GUARD] / 50;
	case ARTILLERY:
		return absoluteW[ARTILLERY] * 0.5;
	default:
		return absoluteW[e.getType()];
	}
}

double Calculate(Entity* e, int n, double& cost)
{
	double w = 0;
	for (int i = 1; i <= n; i++)
	{
		if (e[i].getType() == BASE)
			continue;
		if (e[i].getPlayer()) {
			if (e[i].getType() == MISSILE)
				cost += 3;
			else
				cost += CardMineral[e[i].getType()];
			double preFriendLife = 0;
			int preFirstEnemy = 0;
			if (i > 1 && e[i - 1].getPlayer() && e[i - 1].getType() != PLANT)
				preFriendLife = e[i - 1].getLife();

			for (int j = i - 1; j >= 1; j--)
			{
				if (!e[j].getPlayer()) {
					preFirstEnemy = j;
					break;
				}
			}

			if (preFirstEnemy == 0) // no enemy
				w += calNull(e[i], preFriendLife);
			else if (e[i].getType() == MISSILE || e[i].getType() == BOMBER) {
				if (e[preFirstEnemy].getLife() > 200)
					w += 200;
				else {
					auto ee = e[preFirstEnemy];
					w += ee.getLife() + (double)ee.getPower() * ee.getRange() / (ee.getAspeed() ? ee.getAspeed() : 10) * 10;
				}
			}
			else if (e[i].getType() == PLANT) {
				if (!e[i - 1].getPlayer())
					w += (double)e[i].getLife() / 300 * 600 / 3;
				else
					w += (double)e[i].getLife() / 300 * 600;
			}
			else if (e[i].getType() == GUARD) {
				w += preFriendLife * absoluteW[GUARD] / 50;
			}
			else {
				double sumW = 0, sumCW = 0;
				int cnt = 0;
				for (int j = i - 1; j >= 1; j--)
				{
					auto ee = e[j];
					if (!ee.getPlayer()) {
						double w = ee.getLife() + (double)ee.getPower() * ee.getRange() / (ee.getAspeed() ? ee.getAspeed() : 10) * 10;
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
						sumCW += relativeC[e[i].getType()][ee.getType()] * x * w;
						sumW += x * w;
					}
				}
				if (e[i].getType() == ARTILLERY) {
					if (!e[i - 1].getPlayer() || e[i - 1].getType() == MISSILE) {
						w += sumCW / sumW * 0.5;
					}
					else
						w += sumCW / sumW * 2;
				}
			}
		}
		
	}
	return w;
}

double MinMax(int lMineral, int rMineral, eNode* hMid, eNode* hUp, eNode* hDown, Card* card, Card lncard, Card rncard, int& choose, int& road, TimeCount* t, int depth, PII lfirst, PII rfirst, PII rsecond)
{
	double w = 0;
	double mxAverage = 0;
	double sumw = 0;

	double cost = 0;
	int cnt = 0;
	switch (depth) {
	case 1:
		for (int i = 6; i <= 10; i++) {
			if (rMineral >= card[i].getMineral()) {

				if (!t->isGameEnd()) {
					w = MinMax(lMineral + 2, rMineral - card[i].getMineral() + 2, hMid, hUp, hDown, card, lncard, rncard, choose, road, t, depth + 1, lfirst, { UPROAD, card[i].getType() == 11 ? 13 : card[i].getType() }, rsecond);
					if (mxAverage < w) {
						mxAverage = w;
						choose = i;
						road = UPROAD;
					}

					w = MinMax(lMineral + 2, rMineral - card[i].getMineral() + 2, hMid, hUp, hDown, card, lncard, rncard, choose, road, t, depth + 1, lfirst, { DOWNROAD, card[i].getType() == 11 ? 13 : card[i].getType() }, rsecond);
					if (mxAverage < w) {
						mxAverage = w;
						choose = i;
						road = DOWNROAD;
					}
				}
				w = MinMax(lMineral + 2, rMineral - card[i].getMineral() + 2, hMid, hUp, hDown, card, lncard, rncard, choose, road, t, depth + 1, lfirst, { MIDROAD, card[i].getType() == 11 ? 13 : card[i].getType() }, rsecond);
				if (mxAverage < w) {
					mxAverage = w;
					choose = i;
					road = MIDROAD;
				}

			}
		}
		if (rMineral < 10)
			w = MinMax(lMineral + 2, rMineral + 2, hMid, hUp, hDown, card, lncard, rncard, choose, road, t, depth + 1, lfirst, rfirst, rsecond);
		if (mxAverage < w) {
			mxAverage = w;
			choose = 0;
			road = -1;
		}
		break;

	case 2:
		w = INF;
		for (int i = 1; i <= 5; i++) {
			if (lMineral >= card[i].getMineral() && card[i].getType() <= 10) {
				if (!t->isGameEnd()) {
					w = min(w, MinMax(lMineral - card[i].getMineral() + 2, rMineral + 2, hMid, hUp, hDown, card, lncard, rncard, choose, road, t, depth + 1, { UPROAD, card[i].getType() == 11 ? 13 : card[i].getType() }, rfirst, rsecond));
					w = min(w, MinMax(lMineral - card[i].getMineral() + 2, rMineral + 2, hMid, hUp, hDown, card, lncard, rncard, choose, road, t, depth + 1, { DOWNROAD, card[i].getType() == 11 ? 13 : card[i].getType() }, rfirst, rsecond));
				}

				w = min(w, MinMax(lMineral - card[i].getMineral() + 2, rMineral + 2, hMid, hUp, hDown, card, lncard, rncard, choose, road, t, depth + 1, { MIDROAD, card[i].getType() == 11 ? 13 : card[i].getType() }, rfirst, rsecond));

			}
		}
		w = min(w, MinMax(lMineral + 2, rMineral + 2, hMid, hUp, hDown, card, lncard, rncard, choose, road, t, depth + 1, lfirst, rfirst, rsecond));
		return w;
		break;

	case 3:
		if (rfirst.second && rncard.getMineral() <= rMineral) {
			if (!t->isGameEnd()) {
				w = max(w, MinMax(lMineral + 2, rMineral - lncard.getMineral() + 2, hMid, hUp, hDown, card, lncard, rncard, choose, road, t, depth + 1, lfirst, rfirst, { DOWNROAD, rncard.getType() == 11 ? 13 : rncard.getType() }));
				w = max(w, MinMax(lMineral + 2, rMineral - lncard.getMineral() + 2, hMid, hUp, hDown, card, lncard, rncard, choose, road, t, depth + 1, lfirst, rfirst, { UPROAD, rncard.getType() == 11 ? 13 : rncard.getType() }));
			}
			w = max(w, MinMax(lMineral + 2, rMineral - lncard.getMineral() + 2, hMid, hUp, hDown, card, lncard, rncard, choose, road, t, depth + 1, lfirst, rfirst, { MIDROAD, rncard.getType() == 11 ? 13 : rncard.getType() }));
		}

		for (int i = 6; i <= 10; i++) {
			if (rfirst.second && rfirst.second == i)
				continue;
			if (rMineral >= card[i].getMineral()) {
				w = max(w, MinMax(lMineral + 2, rMineral - card[i].getMineral() + 2, hMid, hUp, hDown, card, lncard, rncard, choose, road, t, depth + 1, lfirst, rfirst, { UPROAD, card[i].getType() == 11 ? 13 : card[i].getType() }));
				w = max(w, MinMax(lMineral + 2, rMineral - card[i].getMineral() + 2, hMid, hUp, hDown, card, lncard, rncard, choose, road, t, depth + 1, lfirst, rfirst, { MIDROAD, card[i].getType() == 11 ? 13 : card[i].getType() }));
				w = max(w, MinMax(lMineral + 2, rMineral - card[i].getMineral() + 2, hMid, hUp, hDown, card, lncard, rncard, choose, road, t, depth + 1, lfirst, rfirst, { DOWNROAD, card[i].getType() == 11 ? 13 : card[i].getType() }));
			}
		}
		w = max(w, MinMax(lMineral + 2, rMineral + 2, hMid, hUp, hDown, card, lncard, rncard, choose, road, t, depth + 1, lfirst, rfirst, rsecond));
		return w;
		break;

	case 4: // to get the sum w, a array should be built
		if (lfirst.second && lfirst.first == UPROAD && lfirst.second <= 10)
			eRoad[++cnt].setEntity(lfirst.second, 0, 10);
		for (auto i = hUp; i != NULL; i = i->ne) {
			eRoad[++cnt] = i->e;
		}
		if (rfirst.second && rfirst.first == UPROAD)
			eRoad[++cnt].setEntity(rfirst.second, 1, 150);
		if (rsecond.second && rsecond.first == UPROAD)
			eRoad[++cnt].setEntity(rsecond.second, 1, 150);
		sumw += Calculate(eRoad, cnt, cost);

		cnt = 0;
		if (lfirst.second && lfirst.first == MIDROAD && lfirst.second <= 10)
			eRoad[++cnt].setEntity(lfirst.second, 0, 10);
		for (auto i = hMid; i != NULL; i = i->ne) {
			if (i->e.getType() == 0)
				continue;
			eRoad[++cnt] = i->e;
		}
		if (rfirst.second && rfirst.first == MIDROAD)
			eRoad[++cnt].setEntity(rfirst.second, 1, 150);
		if (rsecond.second && rsecond.first == MIDROAD)
			eRoad[++cnt].setEntity(rsecond.second, 1, 150);
		sumw += Calculate(eRoad, cnt, cost);

		cnt = 0;
		if (lfirst.second && lfirst.first == DOWNROAD && lfirst.second <= 10)
			eRoad[++cnt].setEntity(lfirst.second, 0, 10);
		for (auto i = hDown; i != NULL; i = i->ne) {
			eRoad[++cnt] = i->e;
		}
		if (rfirst.second && rfirst.first == DOWNROAD)
			eRoad[++cnt].setEntity(rfirst.second, 1, 150);
		if (rsecond.second && rsecond.first == DOWNROAD)
			eRoad[++cnt].setEntity(rsecond.second, 1, 150);
		sumw += Calculate(eRoad, cnt, cost);
		if (cost)
			return sumw / cost;
		else return 0;
	}
	return 0;
}
