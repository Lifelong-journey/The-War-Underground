#pragma once
#ifndef CLEVERAI_H
#define CLEVERAI_H

#include "Entity.h"
#include "Card.h"
#include "Timecount.h"

#include <queue>

typedef pair<int, int> PII;

void InitAbsoluteW();
double MinMax(int lMineral, int rMineral, eNode* hMid, eNode* hUp, eNode* hDown, Card* card, Card lncard, Card rncard, int& choose, int& road, TimeCount* t, int depth, PII lfirst, PII rfirst, PII rsecond);

#endif
