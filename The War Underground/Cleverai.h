#pragma once
#ifndef CLEVERAI_H
#define CLEVERAI_H

#include "Entity.h"
#include "Card.h"
#include "Timecount.h"

#include <queue>

void initAbsoluteW(Inittype*);
bool Search(int lMineral, int& rMineral, eNode* hMid, eNode* hUp, eNode* hDown, Card* card, queue<int>* lsq, queue<int>* rsq, int& choose, int& road, TimeCount* t);
void FindMx(eNode* tt, int crroad, int type, int& choose, double& mxAverage, int& road, int num);
double getwFull(eNode* tt, int type);
double getwNull(int type);
double getEW(Entity* e);

#endif
