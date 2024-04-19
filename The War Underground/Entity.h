#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <Windows.h>

using namespace std;



#define UNUSED -1
#define DIE 0
#define MOVE 1
#define ATTACK 2

#define NORMAL 1 << 0


#define UPROAD 0
#define MIDROAD 1
#define DOWNROAD 2

#define LBOUNDARY 50
#define RBOUNDARY 129


const int nScreenWidth = 179, nScreenHeight = 52;

class Entity {
private:
	int type;
	//bool isReady;
	bool player;
	int power, mspeed, range, life, armor, aspeed;
	int state, abnormal;
	int road;
	int lx, len, rx, height;
	int dir; // 0 right  1 left

public:
	Entity() = default;
	Entity(int, bool);
	void setEntity(int, bool, int);
	void eDraw(wchar_t*);
	bool eAttack();
	bool eCollision();
	void eMoveX(int, bool);
	void eModifyY(bool);

	int getMspeed();
	bool getPlayer();
	int getRoad();
	int getLx();
	int getRange();
	int getRx();
	int getPower();
	int getAspeed();
	int getLife();

	void setDir(bool);
	void setState(int);
	void mdfLife(int);
};

struct eNode {
	Entity e;
	eNode* pre;
	eNode* ne;
};

//void initgraph();
void inititp();



#endif
