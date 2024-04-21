#include "Entity.h"
#include <iostream>
#include <cstring>

using namespace std;


int rNum[4] = { 17, 30, 42 }; // the same as U, M, D overhead



struct Inittype {
	int type;
	int power, mspeed, range, life, armor, aspeed;
	int state, abnormal;
	int road;
	int lx, len, rx, height;
	int mcount, acount;
}itp[20];

//wstring graph[30];

void inititp()
{
	itp[0] = { 0, 20, 0, 10, 1000, 0, 15, MOVE, NORMAL, MIDROAD, 3, 12, 14, 4, 0, 0 }; // base
	itp[1] = { 1, 5, 1, 5, 50, 0, 10, MOVE, NORMAL, MIDROAD, 15, 6, 20, 3, 0, 0 }; // soldier
	itp[2] = { 2, 25, 2, 10, 300, 0, 30, MOVE, NORMAL, MIDROAD, 15, 11, 25, 4, 0, 0 }; // tank
	itp[3] = { 3, 50, 2, 30, 150, 0, 50, MOVE, NORMAL, MIDROAD, 15, 11, 25, 4, 0, 0 }; // bazooka
}

//void initgraph()
//{
//	graph[1].append(L"  o   < H -- / \\  ");
//}

Entity::Entity(int num, bool ply)
{
	dir = ply;
	player = ply;
	type = itp[num].type;
	power = itp[num].power;
	mspeed = itp[num].mspeed;
	aspeed = itp[num].aspeed;
	range = itp[num].range;
	life = itp[num].life;
	armor = itp[num].armor;
	state = itp[num].state;
	abnormal = itp[num].abnormal;
	road = itp[num].road;
	lx = itp[num].lx;
	len = itp[num].len;
	rx = itp[num].rx;
	height = itp[num].height;
	mcount = itp[num].mcount;
	acount = itp[num].acount;
}
void Entity::setEntity(int num, bool ply, int x)
{
	dir = ply;
	player = ply;
	type = itp[num].type;
	power = itp[num].power;
	mspeed = itp[num].mspeed;
	aspeed = itp[num].aspeed;
	range = itp[num].range;
	life = itp[num].life;
	armor = itp[num].armor;
	state = itp[num].state;
	abnormal = itp[num].abnormal;
	road = itp[num].road;
	len = itp[num].len;
	height = itp[num].height;
	mcount = itp[num].mcount;
	acount = itp[num].acount;
	if (x == -1) {
		lx = itp[num].lx;
		rx = itp[num].rx;
	}
	else {
		lx = x;
		rx = x + len;
	}
}

void Entity::eDraw(wchar_t* screen)
{
	int cnt = 0;
	//screen[y * nScreenWidth + 10] = L'@';
	switch (type)
	{
	case 0:
		if (!dir) {
			wsprintf(&screen[(rNum[road] - 4) * nScreenWidth + lx], L"       P    ");
			wsprintf(&screen[(rNum[road] - 3) * nScreenWidth + lx], L" $    _|_   ");
			wsprintf(&screen[(rNum[road] - 2) * nScreenWidth + lx], L" |  _|_0_|_ ");
			wsprintf(&screen[(rNum[road] - 1) * nScreenWidth + lx], L"/*\\| [] [] |");
		}
		else {
			wsprintf(&screen[(rNum[road] - 4) * nScreenWidth + lx], L"    P       ");
			wsprintf(&screen[(rNum[road] - 3) * nScreenWidth + lx], L"   _|_    $ ");
			wsprintf(&screen[(rNum[road] - 2) * nScreenWidth + lx], L" _|_0_|_  | ");
			wsprintf(&screen[(rNum[road] - 1) * nScreenWidth + lx], L"| [] [] |/*\\");
		}
		wsprintf(&screen[(rNum[road] - 5) * nScreenWidth + lx], L"[%d|%d]", life, itp[0].life);
		break;

	case 1:
		if (!dir) {
			wsprintf(&screen[(rNum[road] - 3) * nScreenWidth + lx], L"  o   ");
			wsprintf(&screen[(rNum[road] - 2) * nScreenWidth + lx], L"< H --");
			wsprintf(&screen[(rNum[road] - 1) * nScreenWidth + lx], L" / \\  ");
		}
		else {
			wsprintf(&screen[(rNum[road] - 3) * nScreenWidth + lx], L"   o  ");
			wsprintf(&screen[(rNum[road] - 2) * nScreenWidth + lx], L"-- H >");
			wsprintf(&screen[(rNum[road] - 1) * nScreenWidth + lx], L"  / \\ ");
		}
		wsprintf(&screen[(rNum[road] - 5) * nScreenWidth + lx - 1], L"[%d|%d]", life, itp[1].life);
		break;
	case 2:
		if (!dir) {
			wsprintf(&screen[(rNum[road] - 4) * nScreenWidth + lx], L"    ___ __ ");
			wsprintf(&screen[(rNum[road] - 3) * nScreenWidth + lx], L" __/___\\__ ");
			wsprintf(&screen[(rNum[road] - 2) * nScreenWidth + lx], L"/_________\\");
			wsprintf(&screen[(rNum[road] - 1) * nScreenWidth + lx], L"o o o o o o");
		}
		else {
			wsprintf(&screen[(rNum[road] - 4) * nScreenWidth + lx], L" __ ___    ");
			wsprintf(&screen[(rNum[road] - 3) * nScreenWidth + lx], L" __/___\\__ ");
			wsprintf(&screen[(rNum[road] - 2) * nScreenWidth + lx], L"/_________\\");
			wsprintf(&screen[(rNum[road] - 1) * nScreenWidth + lx], L"o o o o o o");
		}
		wsprintf(&screen[(rNum[road] - 5) * nScreenWidth + lx], L"[%d|%d]", life, itp[2].life);
		break;
	case 3:
		if (!dir) {
			wsprintf(&screen[(rNum[road] - 4) * nScreenWidth + lx], L"  // // _  ");
			wsprintf(&screen[(rNum[road] - 3) * nScreenWidth + lx], L" //_//_|_\\ ");
			wsprintf(&screen[(rNum[road] - 2) * nScreenWidth + lx], L"|_________\\");
			wsprintf(&screen[(rNum[road] - 1) * nScreenWidth + lx], L" O      O  ");
		}
		else {
			wsprintf(&screen[(rNum[road] - 4) * nScreenWidth + lx], L"  _ \\\\ \\\\  ");
			wsprintf(&screen[(rNum[road] - 3) * nScreenWidth + lx], L" /_|_\\\\_\\\\ ");
			wsprintf(&screen[(rNum[road] - 2) * nScreenWidth + lx], L"/_________|");
			wsprintf(&screen[(rNum[road] - 1) * nScreenWidth + lx], L" O      O  ");
		}
		wsprintf(&screen[(rNum[road] - 5) * nScreenWidth + lx], L"[%d|%d]", life, itp[3].life);
		break;
	}

	return;
}


int Entity::getMspeed()
{
	return mspeed;
}

bool Entity::getPlayer()
{
	return player;
}

int Entity::getRoad()
{
	return road;
}

int Entity::getLx()
{
	return lx;
}


int Entity::getRange()
{
	return range;
}

int Entity::getRx()
{
	return rx;
}

int Entity::getPower()
{
	return power;
}

int Entity::getAspeed()
{
	return aspeed;
}

int Entity::getLife()
{
	return life;
}

int Entity::getMcount()
{
	return mcount;
}

int Entity::getAcount()
{
	return acount;
}

void Entity::setDir(bool t)
{
	dir = t;
}

void Entity::setState(int x)
{
	state = x;
}

void Entity::resetMcount()
{
	mcount = 0;
}

void Entity::resetAcount()
{
	acount = 0;
}

void Entity::addMcount()
{
	++mcount;
}

void Entity::addAcount()
{
	++acount;
}

void Entity::mdfLife(int x)
{
	life += x;
}

void Entity::eMoveUp()
{
	--road;
}

void Entity::eMoveDown()
{
	++road;
}

bool Entity::eAttack()
{
	return false;
}

bool Entity::eCollision()
{
	return false;
}

void Entity::eMoveX(int mv, bool isReady)
{
	lx += mv;
	rx += mv;
	return;
}

void Entity::eModifyY(bool mdf)
{
	if (mdf) { // move up
		road = max(road - 1, UPROAD);
	}
	else {
		road = min(road + 1, DOWNROAD);
	}
}
/*
  o
< H --
 / \
	 ___ __
  __/___\__
 /_________\
 o o o o o o

    % % _  
  _/_/_|_\ 
 |________\
  O      O 



	       P    
     $    _|_   
     |  _|_0_|_ 
    /*\| [] [] |
	
	
	    P    
	   _|_    $ 
	 _|_0_|_  | 
	| [] [] |/*\
*/