#pragma once
#ifndef BULLET_H
#define BULLET_H

class Bullet {
private:
	bool isValid;
	int type;
	int dst;
	int x;
	int y;
	int mspeed;
	int dir;
	
public:
	Bullet() = default;
	void setBullet(bool, int, int, int, int);
	void setInvalid();
	void bMove();
	void bDraw(wchar_t*);
	bool getValid();
};


#endif