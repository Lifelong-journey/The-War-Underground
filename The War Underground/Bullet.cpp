#include "Bullet.h"
#include "Define.h"

void Bullet::setBullet(bool t, int tp, int xx, int yy, int destination)
{
	isValid = t;
	type = tp;
	x = xx;
	dst = destination;
	//mspeed = 0;
	y = yy;
	mspeed = 2;
	dir = destination < xx;
}

void Bullet::bMove()
{
	if (!dir) {
		x += mspeed;
		if (x >= dst)
			this->setInvalid();
	}
	else {
		x -= mspeed;
		if (x <= dst)
			this->setInvalid();
	}
}

void Bullet::setInvalid()
{
	isValid = false;
}

void Bullet::bDraw(wchar_t* screen)
{
	switch (type) {
	case 1:
		screen[y * nScreenWidth + x] = L'.';
		break;
	case 2:
		screen[y * nScreenWidth + x] = L'*';
		break;
	case 3:
		if (!dir)
			screen[y * nScreenWidth + x] = L'>';
		else
			screen[y * nScreenWidth + x] = L'<';
		break;
	case 4:
		if (!dir)
			screen[y * nScreenWidth + x] = L')';
		else
			screen[y * nScreenWidth + x] = L'(';
		break;
	case 5:
		screen[y * nScreenWidth + x] = L'!';
		break;
	}
}

bool Bullet::getValid()
{
	return isValid;
}