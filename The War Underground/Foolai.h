#pragma once
#ifndef FOOLAI_H
#define FOOLAI_H
#include "Entity.h"
#include "Card.h"
#include "Timecount.h"

#include <queue>

void fInitAbsoluteW(Inittype*);
bool fSearch(int lMineral, int& rMineral, eNode* hMid, eNode* hUp, eNode* hDown, Card* card, queue<int>* lsq, queue<int>* rsq, int& choose, int& road, TimeCount* t);
void fFindMx(eNode* tt, int crroad, int type, int& choose, double& mxAverage, int& road, int num);
double fgetwFull(eNode* tt, int type);
double fgetwNull(int type);
double fgetEW(Entity* e);
#endif