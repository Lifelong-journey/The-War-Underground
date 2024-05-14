#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <Windows.h>
#include "Bullet.h"
#include "Define.h"

using namespace std;

struct Inittype {
	int type;
	int power, mspeed, range, life, armor, aspeed;
	int state, abnormal;
	int road;
	int lx, len, rx, height;
	int mcount, acount;
};
extern Inittype itp[];
extern int rNum[];


class Entity {
private:
	int type; // 
	bool player;
	int power, mspeed, range, life, armor, aspeed;
	int state, abnormal;
	int road;
	int lx, len, rx, height;
	int dir; // 0 right  1 left
	int mcount, acount;
	//Bullet b[BULLETNUMBER];


public:
	Entity() = default;
	Entity(int, bool);
	void setEntity(int, bool, int);
	//void setBullet(int);
	//void moveBullet();
	void eDraw(wchar_t*);
	//void drawBullet(wchar_t*);
	bool eAttack();
	bool eCollision();
	void eMoveX(int, bool);
	void eModifyY(bool);

	int getDir();
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
	int getState();
	int getType();

	void setDir(bool);
	void setState(int);
	void mdfLife(int);
	void mdfArmor(int);
	void mdfRange(int);
	void resetMcount();
	void resetAcount();
	void resetData();
	void addMcount();
	void addAcount(int);

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
