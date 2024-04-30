#pragma once
#ifndef TWOPLAYERGAME_H
#define TWOPLAYERGAME_H

#include "Entity.h"
#include "Weapon.h"

void PrintFlag(wchar_t* screen);
void SetScene(wchar_t* screen, int& lMineral, int& rMineral);
void PrintPipe(wchar_t* screen, int y, int x);
void SetEntity(wchar_t* screen, eNode* hUp, eNode* hMid, eNode* hDown, eNode* lcrope, eNode* rcrope);
bool CheckCollision(eNode* a, eNode* list);
void SetWeapon(wchar_t* screen, wNode* wUp, wNode* wMid, wNode* wDown, wNode* lcropw, wNode* rcropw);
bool CheckAttack(eNode* a);
void Attack(eNode* source, eNode* target);
void AddList(eNode* a, eNode*& list);
void AddwList(wNode* a, wNode*& list);
wNode* getFreewNode(wNode*& wFree);
eNode* getFreeeNode(eNode*& hFree);
bool CheckUpDown(eNode* x, eNode*& hUp, eNode*& hMid, eNode*& hDown);
bool CheckAttackB(eNode* a, eNode* list);
void eNodeDelete(eNode* x, eNode*& list);
void eNodeFree(eNode* x, eNode*& hFree);
void wNodeDelete(wNode* x, wNode*& list);
void wNodeFree(wNode* x, wNode*& wFree);
bool CheckAlive(eNode* a);
void StatusAccountE(eNode*& list, bool& Goliath, bool& Needle, eNode*& hFree, int& lMineral, int& rMineral);
void SetTimeCount(wchar_t* screen, int m, int s);
bool CheckMove(eNode* x, int dist);
void Move(eNode* x);
void wMove(wNode* x);
void StatusAccountW(wNode*& list, wNode*& wFree);
void AttackMove(eNode*& list, wNode*& listw, wNode*& wFree, eNode*& hUp, eNode*& hMid, eNode* hDown);
bool TakeEffect(wNode* wn, eNode* elist, wNode*& wlist, wNode*& wFree);
void EffectMove(wNode*& list, eNode* elist, wNode*& wFree);
void setlcrope(int num, bool player, int x, eNode*& lcrope, wNode*& lcropw, wNode*& wFree, eNode*& hFree);
void setrcrope(int num, bool player, int x, wNode*& rcropw, eNode*& rcrope, wNode*& wFree, eNode*& hFree);
void setrcropw(int num, bool player, int x, eNode*& rcrope, eNode*& hFree, wNode*& rcropw, wNode*& wFree);
void setlcropw(int num, bool player, int x, eNode*& lcrope, eNode*& hFree, wNode*& lcropw, wNode*& wFree);
void eClear(eNode*& list, eNode*& hFree);
void wClear(wNode*& list, wNode*& wFree);

#endif