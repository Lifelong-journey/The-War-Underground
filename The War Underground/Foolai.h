#pragma once
#ifndef FOOLAI_H
#define FOOLAI_H
#include "Entity.h"
#include "Card.h"
#include "Timecount.h"

#include <queue>

void fInitAbsoluteW();
bool fSearch(int rMineral, eNode* hMid, eNode* hUp, eNode* hDown, Card* card, Card* ln, int& choose, int& road, TimeCount* t);
double fFindMx(eNode* tt, int type, Card* card, Card* ln, int mineralNeeded);
#endif