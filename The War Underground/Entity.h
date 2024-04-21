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

#define PIPEA 30
#define PIPEB 89
#define PIPEC 148
#define PIPED 50
#define PIPEE 128

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
	int mcount, acount;

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
	int getAspeed();
	bool getPlayer();
	int getRoad();
	int getLx();
	int getRange();
	int getRx();
	int getPower();
	int getLife();
	int getMcount();
	int getAcount();

	void setDir(bool);
	void setState(int);
	void mdfLife(int);
	void resetMcount();
	void resetAcount();
	void addMcount();
	void addAcount();

	void eMoveUp();
	void eMoveDown();
};

struct eNode {
	Entity e;
	eNode* pre;
	eNode* ne;
};

//void initgraph();
void inititp();



#endif
