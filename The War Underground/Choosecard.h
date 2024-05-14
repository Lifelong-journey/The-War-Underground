#pragma once
#ifndef CHOOSECARD_H
#define CHOOSECARD_H

void cctInput(bool* bKey, int* lq, int* rq, int& x, int& y, int& x0, int& y0, int& crnum, int& lhh, int& rhh);
void ccaInput(bool* bKey, int* lq, int* rq, int& x, int& y, int& x0, int& y0, int& crnum, int& lhh, int& rhh);
void ccSetScene(wchar_t* screen, int crnum, int x, int y);
void ccSetQueue(wchar_t* screen, int* lq, int* rq);
void SetStartButton(wchar_t* screen, int cnt);
void setCountDown(wchar_t* screen, int c);

#endif