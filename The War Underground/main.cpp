#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <Windows.h>
#include <thread>
#include <random>
#include <ctime>
#include <unordered_map>

#include "Define.h"
#include "Entity.h"
#include "Weapon.h"
#include "Card.h"
#include "Timecount.h"

using namespace std;

//Entity test;
bool bKey[24]; // keyboard press

default_random_engine e(unsigned int(time(0)));

bool keyHoldW = false, keyHoldS = false, keyHoldSpace = false, keyHoldA = false, keyHoldD = false;
bool keyHoldJ = false, keyHoldL = false, keyHoldEnter = false, keyHoldI = false, keyHoldK = false;
int nSpeedCounter = 0;
wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
struct eNode* hFree = NULL, * hUp = NULL, * hMid = NULL, * hDown = NULL;
struct wNode* wFree = NULL, * wUp = NULL, * wMid = NULL, * wDown = NULL;
struct eNode* lcrope = NULL, * rcrope = NULL;
struct wNode* lcropw = NULL, * rcropw = NULL;
int lcropc = 0, rcropc = 0;
int lMineral = 6, rMineral = 6;
int lnem = 0, rnem = 0;
Card card[12];
int lq[12], rq[12];
int lhh = 0, rhh = 0;
Entity* lbase = NULL, * rbase = NULL;
bool Goliath = false, Needle = false;

bool test = false;

void nalloc()
{
    for (int i = 0; i < 30; i++) 
    {
        eNode* ep = new eNode();
        ep->pre = NULL;
        ep->ne = hFree;
        if (hFree != NULL)
            hFree->pre = ep;
        hFree = ep;
    }

    for (int i = 0; i < 20; i++)
    {
        wNode* wp = new wNode();
        wp->ne = wFree;
        wp->pre = NULL;
        if (wFree != NULL)
            wFree->pre = wp;
        wFree = wp;
    }
}

void initcard()
{
    int num = 0;
    while (num < 5)
    {
        int rd = e() % 9 + 1;
        for (int j = 1; j <= num; j++)
        {
            if (card[j].getType() == lq[rd])
                goto nextlturn;
        }

        card[++num].setCard(0, lq[rd], 40 + (num - 1) * 20);
    nextlturn:;
    }
    num = 0;
    while (num < 5)
    {
        int rd = e() % 9 + 1;
        for (int j = 1; j <= num; j++)
        {
            if (card[j + 5].getType() == rq[rd])
                goto nextrturn;
        }
        card[++num + 5].setCard(1, rq[rd], 40 + (num - 1) * 20);
    nextrturn:;
    }
}


void print()
{
    wsprintf(&screen[10 * nScreenWidth + 10], L"!!!!!");
}

void PrintPipe(int y, int x)
{
    for (int i = 0; i < 3; i++)
    {
        screen[(y + i) * nScreenWidth + x + 1] = L' ';
        screen[(y + i) * nScreenWidth + x + 2] = L' ';
        screen[(y + i) * nScreenWidth + x + 3] = L' ';
        screen[(y + i) * nScreenWidth + x] = L'#';
        screen[(y + i) * nScreenWidth + x + 4] = L'#';
    }
    screen[y * nScreenWidth + x + 2] = L'-';
    screen[(y + 2) * nScreenWidth + x + 2] = L'-';
}

void PrintFlag(wchar_t* screen)
{
    wsprintf(&screen[0 * nScreenWidth + 10], L"-----------");
    wsprintf(&screen[1 * nScreenWidth + 10], L"| *  ^  * |");
    wsprintf(&screen[2 * nScreenWidth + 10], L"|  < 0 >  |");
    wsprintf(&screen[3 * nScreenWidth + 10], L"|   +++   |");
    wsprintf(&screen[4 * nScreenWidth + 10], L"|    *    |");
    wsprintf(&screen[5 * nScreenWidth + 10], L"-----------");

    wsprintf(&screen[46 * nScreenWidth + 10], L"-----------");
    wsprintf(&screen[47 * nScreenWidth + 10], L"| __-$-__ |");
    wsprintf(&screen[48 * nScreenWidth + 10], L"|  / O \\  |");
    wsprintf(&screen[49 * nScreenWidth + 10], L"|  / O \\  |");
    wsprintf(&screen[50 * nScreenWidth + 10], L"| _-===-_ |");
    wsprintf(&screen[51 * nScreenWidth + 10], L"-----------");
}

void SetScene(wchar_t* screen)
{
    for (int i = 0; i < nScreenWidth; i++)
    {
        //screen[i] = L'=';
        //screen[(nScreenHeight - 1) * nScreenWidth + i] = L'=';
        screen[6 * nScreenWidth + i] = L'-';
        screen[45 * nScreenWidth + i] = L'-';

        screen[7 * nScreenWidth + i] = L'=';

        screen[17 * nScreenWidth + i] = L'=';
        screen[18 * nScreenWidth + i] = L'*';
        screen[19 * nScreenWidth + i] = L'=';

        screen[30 * nScreenWidth + i] = L'=';
        screen[31 * nScreenWidth + i] = L'*';
        screen[32 * nScreenWidth + i] = L'=';

        screen[42 * nScreenWidth + i] = L'=';
        screen[43 * nScreenWidth + i] = L'*';
        screen[44 * nScreenWidth + i] = L'=';
    }
    for (int i = 0; i < nScreenHeight; i++)
    {
        screen[i * nScreenWidth] = L'=';
        screen[(i + 1) * nScreenWidth - 1] = L'=';
    }
    PrintPipe(17, PIPEA - 2); // 30
    PrintPipe(17, PIPEB - 2);
    //PrintPipe(17, 68);
    //PrintPipe(17, 106);
    PrintPipe(17, PIPEC - 2);
    PrintPipe(30, PIPED - 2);
    PrintPipe(30, PIPEE - 2);
    PrintFlag(screen);
    wsprintf(&screen[nScreenWidth + 140], L"%d", lMineral);
    wsprintf(&screen[47 * nScreenWidth + 140], L"%d", rMineral);
    return;
}

void SetSceneB(wchar_t* screen, int crnum, int x, int y)
{
    for (int i = 0; i < nScreenWidth; i++)
    {
        screen[6 * nScreenWidth + i] = L'-';
        screen[45 * nScreenWidth + i] = L'-';
    }
    int cnt = 0;
    for (int i = 8; i <= 38; i += 10)
    {
        for (int j = 40; j <= 120; j += 20)
        {
            cnt++;
            switch (cnt) {
            case 1:
                wsprintf(&screen[(i + 2) * nScreenWidth + j + 4], L"  o   ");
                wsprintf(&screen[(i + 3) * nScreenWidth + j + 4], L"< H --");
                wsprintf(&screen[(i + 4) * nScreenWidth + j + 4], L" / \\  ");
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 1], L"%d", CardMineral[cnt]);
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 13], L"%d", CardMineral[cnt]);
                break;
            case 2:
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 2], L"    ___ __ ");
                wsprintf(&screen[(i + 2) * nScreenWidth + j + 2], L" __/___\\__ ");
                wsprintf(&screen[(i + 3) * nScreenWidth + j + 2], L"/_________\\");
                wsprintf(&screen[(i + 4) * nScreenWidth + j + 2], L"o o o o o o");
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 1], L"%d", CardMineral[cnt]);
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 13], L"%d", CardMineral[cnt]);
                break;
            case 3:
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 2], L"  // // _  ");
                wsprintf(&screen[(i + 2) * nScreenWidth + j + 2], L" //_//_|_\\ ");
                wsprintf(&screen[(i + 3) * nScreenWidth + j + 2], L"|_________\\");
                wsprintf(&screen[(i + 4) * nScreenWidth + j + 2], L" O      O  ");
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 1], L"%d", CardMineral[cnt]);
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 13], L"%d", CardMineral[cnt]);
                break;
            case 4:
                wsprintf(&screen[(i + 2) * nScreenWidth + j + 2], L"  _     _  ");
                wsprintf(&screen[(i + 3) * nScreenWidth + j + 2], L" / \\___/ \\ ");
                wsprintf(&screen[(i + 4) * nScreenWidth + j + 2], L"/ #|   |# \\");
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 1], L"%d", CardMineral[cnt]);
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 13], L"%d", CardMineral[cnt]);
                break;
            case 5:
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 1], L"        $$$  ");
                wsprintf(&screen[(i + 2) * nScreenWidth + j + 1], L"        ___  ");
                wsprintf(&screen[(i + 3) * nScreenWidth + j + 1], L" ______/. .\\ ");
                wsprintf(&screen[(i + 4) * nScreenWidth + j + 1], L"|[]00 /  .  \\");
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 1], L"%d", CardMineral[cnt]);
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 13], L"%d", CardMineral[cnt]);
                break;
            case 6:
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 4], L"   (!) ");
                wsprintf(&screen[(i + 2) * nScreenWidth + j + 4], L" ___|_ ");
                wsprintf(&screen[(i + 3) * nScreenWidth + j + 4], L"|_____|");
                wsprintf(&screen[(i + 4) * nScreenWidth + j + 4], L" o   o ");
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 1], L"%d", CardMineral[cnt]);
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 13], L"%d", CardMineral[cnt]);
                break;
            case 7:
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 2], L"  [_]======>");
                wsprintf(&screen[(i + 2) * nScreenWidth + j + 2], L" /___\\___   ");
                wsprintf(&screen[(i + 3) * nScreenWidth + j + 2], L"[--------)  ");
                wsprintf(&screen[(i + 4) * nScreenWidth + j + 2], L" O  OOOOO   ");
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 1], L"%d", CardMineral[cnt]);
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 13], L"%d", CardMineral[cnt]);
                break;
            case 8:
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 4], L"((*A*))");
                wsprintf(&screen[(i + 2) * nScreenWidth + j + 4], L" __|__ ");
                wsprintf(&screen[(i + 3) * nScreenWidth + j + 4], L"|__+__|");
                wsprintf(&screen[(i + 4) * nScreenWidth + j + 4], L"O     O");
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 1], L"%d", CardMineral[cnt]);
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 13], L"%d", CardMineral[cnt]);
                break;
            case 9:
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 3], L"    ==={-");
                wsprintf(&screen[(i + 2) * nScreenWidth + j + 3], L"_[]_||___");
                wsprintf(&screen[(i + 3) * nScreenWidth + j + 3], L"\\_______/");
                wsprintf(&screen[(i + 4) * nScreenWidth + j + 3], L" oo   oo ");
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 1], L"%d", CardMineral[cnt]);
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 13], L"%d", CardMineral[cnt]);
                break;
            case 10:
                wsprintf(&screen[(i + 2) * nScreenWidth + j + 4], L"  o   ");
                wsprintf(&screen[(i + 3) * nScreenWidth + j + 4], L"[]H--=");
                wsprintf(&screen[(i + 4) * nScreenWidth + j + 4], L" / \\  ");
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 1], L"%d", CardMineral[cnt]);
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 13], L"%d", CardMineral[cnt]);
                break;
            case 11:
                wsprintf(&screen[(i + 2) * nScreenWidth + j + 5], L">");
                wsprintf(&screen[(i + 3) * nScreenWidth + j + 2], L"-<-=====>>");
                wsprintf(&screen[(i + 4) * nScreenWidth + j + 5], L">");
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 1], L"%d", CardMineral[cnt]);
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 13], L"%d", CardMineral[cnt]);
                break;
            case 12:
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 4], L" _   _ ");
                wsprintf(&screen[(i + 2) * nScreenWidth + j + 4], L"\\ / \\ /");
                wsprintf(&screen[(i + 3) * nScreenWidth + j + 4], L"   O   ");
                wsprintf(&screen[(i + 4) * nScreenWidth + j + 4], L"  /_\\  ");
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 1], L"%d", CardMineral[cnt]);
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 13], L"%d", CardMineral[cnt]);
                break;
            case 13:
                wsprintf(&screen[(i + 2) * nScreenWidth + j + 3], L"   * *   ");
                wsprintf(&screen[(i + 3) * nScreenWidth + j + 3], L"( (EMP) )");
                wsprintf(&screen[(i + 4) * nScreenWidth + j + 3], L"   * *   ");
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 1], L"%d", CardMineral[cnt]);
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 13], L"%d", CardMineral[cnt]);
                break;
            case 14:
                wsprintf(&screen[(i + 2) * nScreenWidth + j + 2], L"=\\       /=");
                wsprintf(&screen[(i + 3) * nScreenWidth + j + 2], L" ||=====|| ");
                wsprintf(&screen[(i + 4) * nScreenWidth + j + 2], L"=/       \\=");
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 1], L"%d", CardMineral[cnt]);
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 13], L"%d", CardMineral[cnt]);
                break;
            case 15:
                wsprintf(&screen[(i + 2) * nScreenWidth + j + 6], L"_?_");
                wsprintf(&screen[(i + 3) * nScreenWidth + j + 5], L"[   ]");
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 1], L"%d", CardMineral[cnt]);
                wsprintf(&screen[(i + 1) * nScreenWidth + j + 13], L"%d", CardMineral[cnt]);
                break;
            }

            for (int k = j; k <= j + 15 - 1; k++)
            {
                screen[i * nScreenWidth + k] = L'~';
                screen[(i + 5) * nScreenWidth + k] = L'~';
            }
            for (int k = i + 1; k <= i + 4; k++)
            {
                screen[k * nScreenWidth + j] = L'|';
                screen[k * nScreenWidth + j + 14] = L'|';
            }
        }
    }

    if (crnum != 0) {
        for (int i = x - 1; i <= x + 15; i++)
        {
            screen[(y - 1) * nScreenWidth + i] = L'*';
            screen[(y + 6) * nScreenWidth + i] = L'*';
        }
        for (int i = y; i <= y + 5; i++)
        {
            screen[i * nScreenWidth + x - 1] = L'*';
            screen[i * nScreenWidth + x + 15] = L'*';
        }
    }
    return;
}

void SetQueue(wchar_t* screen)
{
    int cnt = 1;
    while (lq[cnt])
    {
        PrintCard(screen, lq[cnt], 0, 2 + (cnt - 1) * 20, 0);
        cnt++;
    }
    cnt = 1;
    while (rq[cnt])
    {
        PrintCard(screen, rq[cnt], 1, 2 + (cnt - 1) * 20, 46);
        cnt++;
    }
}


void SetEntity(wchar_t* screen)
{
    for (auto i = hUp; i != NULL; i = i->ne) {
        i->e.eDraw(screen);
        i->e.resetData();
    }

    for (auto i = hMid; i != NULL; i = i->ne) {
        i->e.eDraw(screen);
        i->e.resetData();
    }

    for (auto i = hDown; i != NULL; i = i->ne) {
        i->e.eDraw(screen);
        i->e.resetData();
    }

    if (lcrope != NULL) {
        //wsprintf(&screen[10 * nScreenWidth + 10], L"!!!!!");
        lcrope->e.eDraw(screen);
    }
    if (rcrope != NULL) {
        rcrope->e.eDraw(screen);
    }

    return;
}
void SetWeapon(wchar_t* screen)
{
    for (auto i = wUp; i != NULL; i = i->ne)
        i->w.wDraw(screen);
    for (auto i = wMid; i != NULL; i = i->ne)
        i->w.wDraw(screen);
    for (auto i = wDown; i != NULL; i = i->ne)
        i->w.wDraw(screen);
    if (lcropw != NULL) {
        lcropw->w.wDraw(screen);
    }
    if (rcropw != NULL) {
        rcropw->w.wDraw(screen);
    }
}

void SetCard(wchar_t* screen)
{
    for (int i = 1; i <= 10; i++)
        card[i].cDraw(screen);
}

void SetStartButton(wchar_t* screen, int cnt)
{
    for (int i = 140; i <= 159; i++)
    {
        screen[23 * nScreenWidth + i] = L'-';
        screen[28 * nScreenWidth + i] = L'-';
    }
    for (int i = 24; i <= 28; i++)
    {
        screen[i * nScreenWidth + 140] = L'|';
        screen[i * nScreenWidth + 159] = L'|';
    }
    if (cnt % 20 <= 10)
        wsprintf(&screen[25 * nScreenWidth + 146], L"⇒  ⇒  ⇒");
    wsprintf(&screen[27 * nScreenWidth + 143], L"Touch To Start");
}

void SetTimeCount(wchar_t* screen, int m, int s)
{
    wsprintf(&screen[10 * nScreenWidth + 30], L"%d : %d", m, s);
}


void Attack(eNode* source, eNode* target)
{
    source->e.setState(ATTACK);
    if (source->e.getType() == 6) {
        for (auto i = source->ne; i != NULL; i = i->ne) {
            if (source->e.getRx() + 15 < i->e.getLx())
                break;
            if (i->e.getPlayer() != source->e.getPlayer())
                i->e.mdfLife(-source->e.getPower());
        }
        for (auto i = source->pre; i != NULL; i = i->pre) {
            if (source->e.getLx() - 15 > i->e.getRx())
                break;
            if (i->e.getPlayer() != source->e.getPlayer())
                i->e.mdfLife(-source->e.getPower());
        }
        source->e.mdfLife(-200);
        return;
    }

    source->e.addAcount(1);
    if (source->e.getType() != 11 && source->e.getType() != 12)
        source->e.resetMcount();
    if (source->e.getAcount() > source->e.getAspeed()) {
        source->e.addAcount(-source->e.getAspeed());
        if (source->e.getType() == 10) {
            if (target->e.getLx() > source->e.getLx()) {
                for (auto i = target; i != NULL; i = i->ne) {
                    if (source->e.getRx() + source->e.getRange() < i->e.getLx())
                        break;
                    if (i->e.getPlayer() != source->e.getPlayer())
                        i->e.mdfLife(-source->e.getPower());
                }
            }
            else {
                for (auto i = target; i != NULL; i = i->pre) {
                    if (source->e.getLx() - source->e.getRange() > i->e.getRx())
                        break;
                    if (i->e.getPlayer() != source->e.getPlayer())
                        i->e.mdfLife(-source->e.getPower());
                }
            }
            return;
        }
        target->e.mdfLife(-source->e.getPower());
    }
    return;
}

bool CheckAttack(eNode* a) // Can be better
{
    if (a->e.getType() == 7) {
        if (a->pre == NULL) // Nothing left
        {
            if (a->ne == NULL)
                return false;
            else {
                auto next = a->ne;
                while (next != NULL && (next->e.getPlayer() == a->e.getPlayer() || next->e.getRx() < a->e.getRx() + 25)) { // find the right-first enemy
                    if (next->e.getLx() > a->e.getRange() + a->e.getRx()) // out of attack range
                        return false;
                    next = next->ne;
                }

                if (next == NULL || next->e.getLx() > a->e.getRange() + a->e.getRx() || next->e.getRx() < a->e.getRx() + 25) // out of attack range Or no enemy
                    return false;
                else
                    Attack(a, next);
            }
        }
        else if (a->ne == NULL) { // Nothing right
            auto prev = a->pre;
            while (prev != NULL && (prev->e.getPlayer() == a->e.getPlayer() || prev->e.getLx() > a->e.getLx() - 25)) {
                if (prev->e.getRx() < a->e.getLx() - a->e.getRange()) // out of attack range
                    return false;
                prev = prev->pre;
            }
            if (prev == NULL || prev->e.getRx() < a->e.getLx() - a->e.getRange() || prev->e.getLx() > a->e.getLx() - 25)  // out of attack range Or no enemy
                return false;
            else {
                a->e.setDir(1);
                Attack(a, prev);
            }

        }
        else {
            auto prev = a->pre, next = a->ne;
            while (prev != NULL && (prev->e.getPlayer() == a->e.getPlayer() || prev->e.getLx() > a->e.getLx() - 25)) {
                if (prev->e.getRx() < a->e.getLx() - a->e.getRange()) // out of attack range
                    break;
                prev = prev->pre;
            }
            while (next != NULL && (next->e.getPlayer() == a->e.getPlayer() || next->e.getRx() < a->e.getRx() + 25)) { // find the right-first enemy
                if (next->e.getLx() > a->e.getRange() + a->e.getRx()) // out of attack range
                    break;
                next = next->ne;
            }
            if (prev == NULL) {
                if (next == NULL || next->e.getLx() > a->e.getRange() + a->e.getRx() || next->e.getRx() < a->e.getRx() + 25) // out of attack range Or no enemy
                    return false;
                else
                    Attack(a, next);
            }
            else if (next == NULL) {
                if (prev->e.getRx() < a->e.getLx() - a->e.getRange() || prev->e.getLx() > a->e.getLx() - 25)
                    return false;
                else {
                    a->e.setDir(1);
                    Attack(a, prev);
                }
            }
            else {
                if (prev->e.getRx() < a->e.getLx() - a->e.getRange() || prev->e.getLx() > a->e.getLx() - 25) {
                    if (next->e.getLx() > a->e.getRange() + a->e.getRx() || next->e.getRx() < a->e.getRx() + 25)
                        return false;
                    else
                        Attack(a, next);
                }
                else if (next->e.getLx() > a->e.getRange() + a->e.getRx() || next->e.getRx() < a->e.getRx() + 25) {
                    a->e.setDir(1);
                    Attack(a, prev);
                }
                else if (next->e.getLx() - a->e.getRx() <= a->e.getLx() - prev->e.getRx())
                    Attack(a, next);
                else {
                    a->e.setDir(1);
                    Attack(a, prev);
                }
            }
        }
        return true;
    }

    if (a->pre == NULL) // Nothing left
    {
        if (a->ne == NULL)
            return false;
        else {
            auto next = a->ne;
            while (next != NULL && next->e.getPlayer() == a->e.getPlayer()) { // find the right-first enemy
                if (next->e.getLx() > a->e.getRange() + a->e.getRx()) // out of attack range
                    return false;
                next = next->ne;
            }

            if (next == NULL || next->e.getLx() > a->e.getRange() + a->e.getRx()) // out of attack range Or no enemy
                return false;
            else
                Attack(a, next);
        }
    }
    else if (a->ne == NULL) { // Nothing right
        auto prev = a->pre;
        while (prev != NULL && prev->e.getPlayer() == a->e.getPlayer()) {
            if (prev->e.getRx() < a->e.getLx() - a->e.getRange()) // out of attack range
                return false;
            prev = prev->pre;
        }
        if (prev == NULL || prev->e.getRx() < a->e.getLx() - a->e.getRange())  // out of attack range Or no enemy
            return false; 
        else {
            a->e.setDir(1);
            Attack(a, prev);
        }

    }
    else {
        auto prev = a->pre, next = a->ne;
        while (prev != NULL && prev->e.getPlayer() == a->e.getPlayer()) {
            if (prev->e.getRx() < a->e.getLx() - a->e.getRange()) // out of attack range
                break;
            prev = prev->pre;
        }
        while (next != NULL && next->e.getPlayer() == a->e.getPlayer()) { // find the right-first enemy
            if (next->e.getLx() > a->e.getRange() + a->e.getRx()) // out of attack range
                break;
            next = next->ne;
        }
        if (prev == NULL) {
            if (next == NULL || next->e.getLx() > a->e.getRange() + a->e.getRx()) // out of attack range Or no enemy
                return false;
            else
                Attack(a, next);
        }
        else if (next == NULL) {
            if (prev->e.getRx() < a->e.getLx() - a->e.getRange())
                return false;
            else {
                a->e.setDir(1);
                Attack(a, prev);
            }

        }
        else {
            if (prev->e.getRx() < a->e.getLx() - a->e.getRange()) {
                if (next->e.getLx() > a->e.getRange() + a->e.getRx())
                    return false;
                else
                    Attack(a, next);
            }
            else if (next->e.getLx() > a->e.getRange() + a->e.getRx()) {
                a->e.setDir(1);
                Attack(a, prev);
            }
            else if (next->e.getLx() - a->e.getRx() <= a->e.getLx() - prev->e.getRx())
                Attack(a, next);
            else {
                a->e.setDir(1);
                Attack(a, prev);
            }
        }
    }
    return true;
}

void AddList(eNode* a, eNode* &list)
{
    
    if (list == NULL) {
        //test = true;
        a->pre = NULL;
        a->ne = list;
        list = a;
        return;
    }

    eNode* now = list;
    for (auto i = list; i != NULL; i = i->ne)
    {
        if (i->e.getLx() > a->e.getLx()) {
            a->pre = i->pre;
            a->ne = i;
            if (i->pre != NULL) {
                i->pre->ne = a;
                i->pre = a;
            }
            else {

                i->pre = a;
                list = a;
            }
            return;
        }
        now = i;
    }
    now->ne = a;
    a->pre = now;
    a->ne = NULL;
    return;
}

void AddwList(wNode* a, wNode* &list) // 头插法
{
    if (list == NULL) {
        a->ne = NULL;
        a->pre = NULL;
        list = a;
    }
    else {
        a->pre = NULL;
        a->ne = list;
        list->pre = a;
        list = a;
    }
}

eNode* getFreeeNode()
{
    eNode* x = hFree;
    hFree = hFree->ne;
    x->pre = NULL;
    x->ne = NULL;
    return x;
}

wNode* getFreewNode()
{
    wNode* x = wFree;
    wFree = wFree->ne;
    return x;
}

bool CheckMove(eNode* x, int dist)
{
    /*if (x->e.getRx() + dist > 170 || x->e.getLx() + dist < 10)
        return false;*/
    if (!x->e.getPlayer()) {
        if (x->ne == NULL || /*(x->ne->e.getPlayer() == 0 && (x->ne->e.getType() == 0 || x->ne->e.getType() == 4 || x->ne->e.getType() == 5)) ||*/ x->ne->e.getLx() > x->e.getRx() + dist)
            return true;
    }
    else {
        if (x->pre == NULL || /*(x->pre->e.getPlayer() == 1 && (x->pre->e.getType() == 0 || x->pre->e.getType() == 4 || x->pre->e.getType() == 5)) ||*/ x->pre->e.getRx() < x->e.getLx() - dist)
            return true;
    }
    x->e.resetMcount();
    return false;
}

void Move(eNode* x)
{
    x->e.addMcount();
    if (x->e.getMspeed() == 0)
        return;
    if (x->e.getType() == 11) {
        if (x->e.getMcount() % x->e.getMspeed() == 0 && CheckMove(x, 2)) {
            x->e.eMoveX(-2, true);
        }
    }
    if (x->e.getMcount() % x->e.getMspeed() == 0 && CheckMove(x, 1)) {

        if (!x->e.getPlayer())
            x->e.eMoveX(1, true);
        else
            x->e.eMoveX(-1, true);
    }
}

void wMove(wNode* x)
{
    if (x->w.getMspeed() == 0)
        return;
    if (!x->w.getPlayer())
        x->w.wMoveX(1, true);
    else
        x->w.wMoveX(-1, true);
}

void initBase()
{
    eNode* p1 = getFreeeNode();
    eNode* p2 = getFreeeNode();
    lbase = &(p1->e);
    rbase = &(p2->e);
    p1->e.setEntity(0, 0, 2);
    p2->e.setEntity(0, 1, 165);
    hMid = p1;
    p1->pre = NULL;
    p2->ne = NULL;
    p1->ne = p2;
    p2->pre = p1;
}

bool CheckCollision(eNode* a, eNode* list)
{
    for (auto i = list; i != NULL; i = i->ne)
    {
        //if (i->e.getPlayer() == a->e.getPlayer() && (i->e.getType() == 4 || i->e.getType() == 5))
        //    continue;
        if (!(i->e.getRx() < a->e.getLx() || i->e.getLx() > a->e.getRx())) {// not at left or not at right, then invalid 
            return true;
        }
    }
    return false;
}

bool CheckAttackB(eNode* a, eNode* list)
{
    for (auto i = list; i != NULL; i = i->ne)
    {
        int alx = a->e.getLx(), arx = a->e.getRx(), ar = a->e.getRange(), ilx = i->e.getLx(), irx = i->e.getRx();
        if (i->e.getPlayer() != a->e.getPlayer() && (ilx <= arx + ar && ilx > arx || irx >= alx - ar && irx < alx))
            return true;
    }
    return  false;
}

void eNodeDelete(eNode* x, eNode*& list)
{
    if (x->pre == NULL) {
        if (x->ne == NULL) {
            //test = true;
            list = NULL;
        }

        else {
            list = x->ne;
            x->ne->pre = NULL;
        }
    }
    else if (x->ne == NULL) {
        x->pre->ne = NULL;
    }
    else {
        x->pre->ne = x->ne;
        x->ne->pre = x->pre;
    }
}

void eNodeFree(eNode* x)
{
    hFree->pre = x;
    x->ne = hFree;
    hFree = x;
}

void wNodeDelete(wNode* x, wNode*& list)
{
    if (x->pre == NULL) {
        if (x->ne == NULL) {
            //test = true;
            list = NULL;
        }
        else {
            list = x->ne;
            x->ne->pre = NULL;
        }
    }
    else if (x->ne == NULL) {
        x->pre->ne = NULL;
    }
    else {
        x->pre->ne = x->ne;
        x->ne->pre = x->pre;
    }
    x->ne = NULL;
    x->pre = NULL;
}

void wNodeFree(wNode* x)
{

    wFree->pre = x;
    x->ne = wFree;
    wFree = x;
}

bool CheckAlive(eNode* a)
{
    return (a->e.getLife() > 0);
}

void wait()
{
    return;
}

bool CheckUpDown(eNode* x)
{
    int road = x->e.getRoad(), lx = x->e.getLx(), rx = x->e.getRx();
    bool player = x->e.getPlayer();
    if (road == MIDROAD) {
        if (lx <= PIPEA && rx >= PIPEA || lx <= PIPEB && rx >= PIPEB || lx <= PIPEC && rx >= PIPEC)
            if (CheckAttackB(x, hUp) && !CheckCollision(x, hUp)) {
                x->e.eMoveUp();
                eNodeDelete(x, hMid);
                AddList(x, hUp);
                return true;
            }
        if (lx <= PIPED && rx >= PIPED || lx <= PIPEE && rx >= PIPEE)
            if (CheckAttackB(x, hDown) && !CheckCollision(x, hDown)) {
                x->e.eMoveDown();
                eNodeDelete(x, hMid);
                AddList(x, hDown);
                return true;
            }
    }
    else if (road == UPROAD) {
        if ((!player && (lx <= PIPEA && rx >= PIPEA)) || (player && (lx <= PIPEC && rx >= PIPEC)) || (lx <= PIPEB && rx >= PIPEB)) {
            if (CheckAttackB(x, hMid) && !CheckCollision(x, hMid)) {
                x->e.eMoveDown();
                eNodeDelete(x, hUp);
                AddList(x, hMid);
                return true;
            }
        }
        else if ((!player && lx <= PIPEC && rx >= PIPEC) || (player && lx <= PIPEA && rx >= PIPEA)) {
            if (!CheckCollision(x, hMid)) {
                x->e.eMoveDown();
                eNodeDelete(x, hUp);
                AddList(x, hMid);
            }
            else
                wait();
            return true;
        }
    }
    else if (road == DOWNROAD) {
        if ((!player && lx <= PIPED && rx >= PIPED) || (player && lx <= PIPEE && rx >= PIPEE)) {
            if (CheckAttackB(x, hMid) && !CheckCollision(x, hMid)) {
                x->e.eMoveUp();
                eNodeDelete(x, hDown);
                AddList(x, hMid);
                return true;
            }
        }
        else if ((!player && lx <= PIPEE && rx >= PIPEE) || (player && lx <= PIPED && rx >= PIPED)) {
            if (!CheckCollision(x, hMid)) {
                x->e.eMoveUp();
                eNodeDelete(x, hDown);
                AddList(x, hMid);
            }
            else
                wait();
            return true;
        }
    }
    return false;
}

void StatusAccountE(eNode* &list)
{
    eNode* now = NULL;
    for (auto i = list; i != NULL; i = i->ne)
    {
        if (i->e.getType() == 11 && !Goliath && i->e.getLife() <= 1000) {
            Goliath = true;
            int cnt = 10;
            for (auto j = i->pre; j != NULL; j = j->pre) {
                if (!j->e.getPlayer()) {
                    while (cnt-- && j->e.getLife() > 0)
                        j->e.mdfLife(-40);
                }
            }
        }
        if (i->e.getType() == 12 && !Goliath && i->e.getLife() <= 1000) {
            Needle = true;
            for (auto j = i->ne; j != NULL; j = j->ne) {
                if (j->e.getPlayer()) {
                    j->e.mdfLife(-250);
                }
            }
        }
        if (i->e.getType() == 4 && i->e.getMcount() % 20 == 0 && i->e.getMcount() != 0) {
            i->e.mdfLife(-5);
        }
        else if (i->e.getType() == 5 && i->e.getMcount() % 20 == 0 && i->e.getMcount() != 0) {
            if (i->e.getMcount() % 60 == 0) {
                if (i->e.getPlayer())
                    rMineral = min(10, rMineral + 1);
                else
                    lMineral = min(10, lMineral + 1);
            }
            i->e.mdfLife(-15);
        }
        if (now != NULL) {
            eNodeDelete(now, list);
            eNodeFree(now);
            now = NULL;
        }
        if (!CheckAlive(i))
            now = i;
    }
    if (now != NULL) {
        eNodeDelete(now, list);
        eNodeFree(now);
    }
}

void StatusAccountW(wNode* &list)
{
    wNode* now = NULL;
    for (auto i = list; i != NULL; i = i->ne)
    {

        i->w.cutTime();
        if (now != NULL) {
            wNodeDelete(now, list);
            wNodeFree(now);
            now = NULL;
        }
        if (i->w.getTime() < 0)
            now = i;
    }
    if (now != NULL) {
        wNodeDelete(now, list);
        wNodeFree(now);
    }
}

void AttackMove(eNode*& list, wNode*& listw)
{
    for (auto i = list; i != NULL; i = i->ne)
    {
        if (i->e.getType() == 8) {
            Move(i);
            wNode* wn = getFreewNode();
            wn->w.setWeapon(6, i->e.getPlayer(), i->e.getLx() + 3 - 20);
            AddwList(wn, listw);
            continue;
        }
        if (i->e.getType() == 11 || i->e.getType() == 12) {
            if (!CheckAttack(i))
                i->e.resetAcount();
            Move(i);
            continue;
        }
        if (!CheckAttack(i)) {
            //i->e.setDir(0); // this will be done at the draw part
            i->e.resetAcount();
            if (!CheckUpDown(i))
                Move(i); // not attack
        }
    }
}

bool TakeEffect(wNode* wn, eNode* elist, wNode*& wlist)
{
    auto w = wn->w;
    for (auto j = elist; j != NULL; j = j->ne)
    {
        auto& e = j->e;
        if (e.getRx() < w.getLx() || e.getLx() > w.getRx())
            continue;
        int type = w.getType();
        switch (type) {
        case 1:
            if (e.getPlayer() == w.getPlayer())
                continue;
            e.mdfLife(-w.getPower());
            wNodeDelete(wn, wlist);
            wNodeFree(wn);
            return true; // the missile can only attack one entity
        case 2:
            e.mdfLife(-w.getPower());
            return false; // the water can damage all the entity, no matter enemy or friend
        case 3:
            if (e.getPlayer() == w.getPlayer())
                continue;
            e.mdfRange(5);
            return false;
        case 4:
            if (e.getPlayer() != w.getPlayer())
                continue;
            e.mdfLife(w.getPower());
            return false;
        case 5:
            if (w.getTime() >= 240)
                continue;
            e.mdfLife(-w.getPower());
            wNodeDelete(wn, wlist);
            wNodeFree(wn);
            return true;
        case 6:
            if (e.getPlayer() != w.getPlayer())
                continue;
            e.mdfArmor(w.getPower());
            return false;
        }
    }
    return false;
}

void EffectMove(wNode*& list, eNode* elist)
{
    for (auto i = list; i != NULL; i = i->ne)
    {
        i->w.addTcount();
        if (i->w.getTcount() == i->w.getAspeed()) {
            i->w.resetTcount();
            if (TakeEffect(i, elist, list))
                return;
        }
        if (i->w.getMspeed() != 0)
            if (i->w.getPlayer() == 0)
                i->w.wMoveX(i->w.getMspeed(), 1);
            else
                i->w.wMoveX(-i->w.getMspeed(), 1);
    }
}

void setlcrope(int num, bool player, int x)
{
    if (lcropw != NULL) {
        wNodeFree(lcropw);
        lcropw = NULL;
    }

    if (lcrope == NULL)
        lcrope = getFreeeNode();
    lcrope->e.setEntity(num, player, x);
}

void setrcrope(int num, bool player, int x)
{
    if (rcropw != NULL) {
        wNodeFree(rcropw);
        rcropw = NULL;
    }

    if (rcrope == NULL)
        rcrope = getFreeeNode();
    rcrope->e.setEntity(num, player, x);
}


void setrcropw(int num, bool player, int x)
{
    if (rcrope != NULL) {
        eNodeFree(rcrope);
        rcrope = NULL;
    }
    if (rcropw == NULL)
        rcropw = getFreewNode();
    rcropw->w.setWeapon(num, player, x);
}

void setlcropw(int num, bool player, int x)
{
    if (lcrope != NULL) {
        eNodeFree(lcrope);
        lcrope = NULL;
    }
    if (lcropw == NULL)
        lcropw = getFreewNode();
    lcropw->w.setWeapon(num, player, x);
}

void eClear(eNode*& list)
{
    eNode* now = NULL;
    for (auto i = list; i != NULL; i = i->ne)
    {
        if (now != NULL) {
            eNodeDelete(now, list);
            eNodeFree(now);
        }
        now = i;
    }
    if (now != NULL) {
        eNodeDelete(now, list);
        eNodeFree(now);
    }
    list = NULL;
}

void wClear(wNode*& list)
{
    wNode* now = NULL;
    for (auto i = list; i != NULL; i = i->ne)
    {
        if (now != NULL) {
            wNodeDelete(now, list);
            wNodeFree(now);
        }
        now = i;
    }
    if (now != NULL) {
        wNodeDelete(now, list);
        wNodeFree(now);
    }
    list = NULL;
}

int main()
{
    inititp();
    inititw();
    // initgraph();
    nalloc();
    // initfreelist

    //test.setEntity(2, 0);
    initBase();

    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    int crnum = 0;
    int x0 = 0, y0 = 0;
    int nSpeedCount = 0;
    bool isCountDown = false;
    int CountDown = 0;
    while (1)
    {
        ++nSpeedCount;
        bool readyToStart = (lhh == 9 && rhh == 9);

        this_thread::sleep_for(50ms);
        if (isCountDown) {
            CountDown++;
            wsprintf(&screen[26 * nScreenWidth + 150], L"%d", 5 - (CountDown / 20));
            if (CountDown == 100)
                break;
            WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
            continue;
        }
        for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
            screen[i] = L' ';
        nSpeedCounter += 10;
        POINT pt;
        GetCursorPos(&pt);
        int x = pt.x, y = pt.y;
        if (bKey[22]) {
            if (y >= 200 && y <= 280) {
                if (x >= 385 && x <= 510) {
                    crnum = 1;
                    x0 = 40, y0 = 8;
                }
                else if (x >= 575 && x <= 700) {
                    crnum = 2;
                    x0 = 60, y0 = 8;
                }
                else if (x >= 760 && x <= 890) {
                    crnum = 3;
                    x0 = 80, y0 = 8;
                }
                else if (x >= 945 && x <= 1075) {
                    crnum = 4;
                    x0 = 100, y0 = 8;
                }
                else if (x >= 1135 && x <= 1260) {
                    crnum = 5;
                    x0 = 120, y0 = 8;
                }
            }
            else if (y >= 385 && y <= 465) {
                if (x >= 385 && x <= 510) {
                    crnum = 6;
                    x0 = 40, y0 = 18;
                }
                else if (x >= 575 && x <= 700) {
                    crnum = 7;
                    x0 = 60, y0 = 18;
                }
                else if (x >= 760 && x <= 890) {
                    crnum = 8;
                    x0 = 80, y0 = 18;
                }
                else if (x >= 945 && x <= 1075) {
                    crnum = 9;
                    x0 = 100, y0 = 18;
                }
                else if (x >= 1135 && x <= 1260) {
                    crnum = 10;
                    x0 = 120, y0 = 18;
                }
            }
            else if (y >= 575 && y <= 650) {
                if (x >= 385 && x <= 510) {
                    crnum = 11;
                    x0 = 40, y0 = 28;
                }
                else if (x >= 575 && x <= 700) {
                    crnum = 12;
                    x0 = 60, y0 = 28;
                }
                else if (x >= 760 && x <= 890) {
                    crnum = 13;
                    x0 = 80, y0 = 28;
                }
                else if (x >= 945 && x <= 1075) {
                    crnum = 14;
                    x0 = 100, y0 = 28;
                }
                else if (x >= 1135 && x <= 1260) {
                    crnum = 15;
                    x0 = 120, y0 = 28;
                }
            }
        }
        if (bKey[23]) {
            crnum = 0;
        }
        if (bKey[4] && crnum != 0 && lhh < 9) {
            bool isSame = false;
            for (int i = 1; i <= lhh; i++)
                if (lq[i] == crnum) {
                    isSame = true;
                    break;
                }
            if (!isSame) {
                lq[++lhh] = crnum;
                crnum = 0;
            }
        }
        if (bKey[15] && crnum != 0 && rhh < 9) {
            bool isSame = false;
            for (int i = 1; i <= rhh; i ++)
                if (rq[i] == crnum) {
                    isSame = true;
                    break;
                }
            if (!isSame) {
                rq[++rhh] = crnum;
                crnum = 0;
            }
        }
        if (bKey[10] && lhh != 0) {
            lq[lhh--] = 0;
        }

        if (bKey[21] && rhh != 0) {
            rq[rhh--] = 0;
        }

        

        SetSceneB(screen, crnum, x0, y0);
        SetQueue(screen);
        if (readyToStart) {
            SetStartButton(screen, nSpeedCount);
            if (bKey[22] && x >= 1320 && x <= 1495 && y >= 480 && y <= 560) {
                for (int i = 23; i <= 29; i++)
                {
                    for (int j = 140; j <= 160; j++)
                        screen[i * nScreenWidth + j] = L' ';
                }
                isCountDown = true;
            }
        }
        for (int k = 0; k < 24; k++)
            bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("ADWS\x20\x31\x32\x33\x34\x35\xc0JLIK\x0D\x37\x38\x39\x30\xBD\x08\x01\x02"[k]))) != 0;
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }

    initcard();
    //hMid->ne->e.setEntity(11, 1, 150);
    //hMid->e.setEntity(12, 0, 20);
    TimeCount t;
    bool Endgame = false;
    while (1)
    {
   // -----------
   // | *  ^  * |
   // |  / O \  |
   // |   +++   |
   // |    *    |
   // -----------  
   // | __-$-__ | 
   // |  < 0 >  |
   // | __-|-__ |
   // |   -$-   |
   // ----------- 
        this_thread::sleep_for(50ms);
        nSpeedCounter += 10;
        t.addMs();
        if (t.isGameEnd() && !Endgame) {
            Endgame = true;
            lbase->setEntity(12, 0, 10);
            rbase->setEntity(11, 1, 170);
            eClear(hUp);
            eClear(hDown);
            wClear(wUp);
            wClear(wDown);
        }
        if (lbase->getLife() <= 0 || rbase->getLife() <= 0) { // Game over
            for (int i = 20; i <= 31; i++)
                for (int j = 40; j <= 140; j++)
                    screen[i * nScreenWidth + j] = L' ';
            for (int i = 20; i <= 31; i++)
            {
                screen[i * nScreenWidth + 40] = L'|';
                screen[i * nScreenWidth + 140] = L'|';
            }
            for (int i = 40; i <= 140; i++)
            {
                screen[20 * nScreenWidth + i] = L'-';
                screen[31 * nScreenWidth + i] = L'-';
            }
            wsprintf(&screen[24 * nScreenWidth + 90], L"_    _    _   _____     _      ");
            wsprintf(&screen[25 * nScreenWidth + 90], L"\\    \\    /     |      | \\\\   |");
            wsprintf(&screen[26 * nScreenWidth + 90], L" \\  / \\  /      |      |  \\\\  |");
            wsprintf(&screen[27 * nScreenWidth + 90], L"  \\/   \\/     __|__    |   \\\\_|");
            if (nSpeedCounter % 200 <= 100)
                wsprintf(&screen[29 * nScreenWidth + 73], L"Press the space bar to return");
            else
                wsprintf(&screen[29 * nScreenWidth + 73], L"                             ");
            if (rbase->getLife() <= 0) {
                wsprintf(&screen[23 * nScreenWidth + 60], L"-----------");
                wsprintf(&screen[24 * nScreenWidth + 60], L"| *  ^  * |");
                wsprintf(&screen[25 * nScreenWidth + 60], L"|  < 0 >  |");
                wsprintf(&screen[26 * nScreenWidth + 60], L"|   +++   |");
                wsprintf(&screen[27 * nScreenWidth + 60], L"|    *    |");
                wsprintf(&screen[28 * nScreenWidth + 60], L"-----------");
            }
            else {
                wsprintf(&screen[23 * nScreenWidth + 60], L"-----------");
                wsprintf(&screen[24 * nScreenWidth + 60], L"| __-$-__ |");
                wsprintf(&screen[25 * nScreenWidth + 60], L"|  / O \\  |");
                wsprintf(&screen[26 * nScreenWidth + 60], L"|  / O \\  |");
                wsprintf(&screen[27 * nScreenWidth + 60], L"| _-===-_ |");
                wsprintf(&screen[28 * nScreenWidth + 60], L"-----------");
            }
            if (bKey[4]) {
                break;
            }
            for (int k = 0; k < 24; k++)
                bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("ADWS\x20\x31\x32\x33\x34\x35\xc0JLIK\x0D\x37\x38\x39\x30\xBD\x08\x01\x02"[k]))) != 0;
            WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth* nScreenHeight, { 0,0 }, & dwBytesWritten);
            continue;
        }
        for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
            screen[i] = L' ';
        SetScene(screen);
        // Timing


        lnem = max(0, lnem - 1);
        rnem = max(0, rnem - 1);
        if (nSpeedCounter % 450 == 0) {
            lMineral = min(10, lMineral + 1);
            rMineral = min(10, rMineral + 1);
        }
        // Input
        if (bKey[4]) {
            if (card[lcropc].getMineral() > lMineral) {
                lnem = 50;
            }
            else {
                int isUse = false;
                if (!keyHoldSpace) {
                    if (lcropw != NULL) {
                        int road = lcropw->w.getRoad();
                        if (road == UPROAD)
                           AddwList(lcropw, wUp);
                        else if (road == MIDROAD)
                            AddwList(lcropw, wMid);
                        else
                            AddwList(lcropw, wDown);
                        isUse = true;
                    }
                    if (lcrope != NULL) {
                        if (lcrope->e.getRoad() == UPROAD && !CheckCollision(lcrope, hUp)) {
                           AddList(lcrope, hUp);
                           isUse = true;
                        }
                        else if (lcrope->e.getRoad() == MIDROAD && !CheckCollision(lcrope, hMid)) {
                            //wsprintf(&screen[10 * nScreenWidth + 10], L"!!!!!");
                            //test = true;
                            AddList(lcrope, hMid);
                            isUse = true;
                        }
                        else if (lcrope->e.getRoad() == DOWNROAD && !CheckCollision(lcrope, hDown)) {
                            AddList(lcrope, hDown);
                            isUse = true;
                        }
                    }
                }
                if (isUse) {
                    lMineral -= card[lcropc].getMineral();
                    int x = card[lcropc].getLx();
                    while (1)
                    {
                        int rd = e() % 9 + 1;
                        bool isOk = true;
                        for (int i = 1; i <= 5; i++)
                        {
                            //if (i == lcropc)
                            //    continue;
                            if (lq[rd] == card[i].getType()) {
                                isOk = false;
                                break;
                            }
                        }
                        if (isOk) {
                            card[lcropc].setCard(0, rd, x);
                            break;
                        }
                    }
                    lcrope = NULL;
                    lcropw = NULL;
                    lcropc = 0;
                }
            }
            
            keyHoldSpace = true;   
        }
        else
            keyHoldSpace = false;

        if (bKey[15]) {
            if (card[rcropc].getMineral() > rMineral) {
                rnem = 50;
            }
            else {
                bool isUse = false;
                if (!keyHoldEnter) {
                    if (rcropw != NULL) {
                        int road = rcropw->w.getRoad();
                        if (road == UPROAD)
                            AddwList(rcropw, wUp);
                        else if (road == MIDROAD)
                            AddwList(rcropw, wMid);
                        else
                            AddwList(rcropw, wDown);
                        isUse = true;
                    }
                    if (rcrope != NULL) {
                        if (rcrope->e.getRoad() == UPROAD && !CheckCollision(rcrope, hUp)) {
                            AddList(rcrope, hUp);
                            isUse = true;
                        }
                        else if (rcrope->e.getRoad() == MIDROAD && !CheckCollision(rcrope, hMid)) {
                            //wsprintf(&screen[10 * nScreenWidth + 10], L"!!!!!");
                            //test = true;
                            AddList(rcrope, hMid);
                            isUse = true;
                        }
                        else if (rcrope->e.getRoad() == DOWNROAD && !CheckCollision(rcrope, hDown)) {
                            AddList(rcrope, hDown);
                            isUse = true;
                        }
                    }
                }
                if (isUse) {
                    rMineral -= card[rcropc].getMineral();
                    int x = card[rcropc].getLx();
                    while (1)
                    {
                        int rd = e() % 9 + 1;
                        bool isOk = true;
                        for (int i = 6; i <= 10; i++)
                        {
                            /*if (i == rcropc)
                                continue;*/
                            if (rq[rd] == card[i].getType()) {
                                isOk = false;
                                break;
                            }
                        }
                        if (isOk) {
                            card[rcropc].setCard(1, rd, x);
                            break;
                        }
                    }
                    rcropc = 0;
                    rcrope = NULL;
                    rcropw = NULL;
                }
            }

            keyHoldEnter = true;
        }
        else
            keyHoldEnter = false;

        if (lcrope != NULL && bKey[10]) {
            lcropc = 0;
            eNodeFree(lcrope);
            lcrope = NULL;
        }
        if (lcropw != NULL && bKey[10]) {
            lcropc = 0;
            wNodeFree(lcropw);
            lcropw = NULL;
        }
        if (rcrope != NULL && bKey[21]) {
            rcropc = 0;
            eNodeFree(rcrope);
            rcrope = NULL;
        }
        if (rcropw != NULL && bKey[21]) {
            rcropc = 0;
            wNodeFree(rcropw);
            rcropw = NULL;
        }
        for (int i = 5; i <= 9; i++)
        {
            if (bKey[i]) {
                int type = card[i - 4].getType();
                lcropc = i - 4;
                if (type <= 10)
                    setlcrope(type, 0, 20);
                else
                    setlcropw(type - 10, 0, 20);
                break;
            }
        }
        for (int i = 16; i <= 20; i++)
        {
            if (bKey[i]) {
                rcropc = i - 10;
                int type = card[i - 10].getType();
                if (type <= 10)
                    setrcrope(type, 1, 150);
                else
                    setrcropw(type - 10, 1, 150);
                break;
            }
        }
        //if (wMid != NULL)
        //    wsprintf(&screen[16 * nScreenWidth + 30], L"%d", wMid->w.getTime());
        //if (hDown != NULL && hDown->e.getState() == ATTACK) {
        //    wsprintf(&screen[10 * nScreenWidth + 10], L"!!!!!");
        //}
        /*if (test)
            wsprintf(&screen[10 * nScreenWidth + 10], L"%d", wMid->w.getRx());*/
        /*int cnt = 0;
        for (auto i = hMid; i != NULL; i = i->ne) {
            cnt++;
            wsprintf(&screen[10 * nScreenWidth + 10], L"%d", cnt);
        }*/

        

        if (lcrope != NULL) {
            if (bKey[0])
                lcrope->e.eMoveX(-1, 0);

            if (bKey[1])
                lcrope->e.eMoveX(1, 0);

            if (bKey[2] && !t.isGameEnd()) {
                if (!keyHoldW)
                    lcrope->e.eModifyY(1);
                keyHoldW = true;
            }
            else
                keyHoldW = false;

            if (bKey[3] && !t.isGameEnd()) {
                if (!keyHoldS)
                    lcrope->e.eModifyY(0);
                keyHoldS = true;
            }
            else
                keyHoldS = false;
        }
        if (lcropw != NULL) {
            if (bKey[0] && lcropw->w.getType() != 1) {
                if (!keyHoldA)
                    lcropw->w.wMoveX(-1, 0);
                else
                    lcropw->w.wMoveX(-2, 0);
                keyHoldA = true;
            }
            else
                keyHoldA = false;

            if (bKey[1] && lcropw->w.getType() != 1) {
                if (!keyHoldD)
                    lcropw->w.wMoveX(1, 0);
                else
                    lcropw->w.wMoveX(2, 0);
                keyHoldD = true;
            }
            else
                keyHoldD = false;

            if (bKey[2] && !t.isGameEnd()) {
                if (!keyHoldW)
                    lcropw->w.wMoveY(1);
                keyHoldW = true;
            }
            else
                keyHoldW = false;

            if (bKey[3] && !t.isGameEnd()) {
                if (!keyHoldS)
                    lcropw->w.wMoveY(0);
                keyHoldS = true;
            }
            else
                keyHoldS = false;
        }

        if (rcrope != NULL) {
            if (bKey[11])
                rcrope->e.eMoveX(-1, 0);

            if (bKey[12])
                rcrope->e.eMoveX(1, 0);

            if (bKey[13] && !t.isGameEnd()) {
                if (!keyHoldI)
                    rcrope->e.eModifyY(1);
                keyHoldI = true;
            }
            else
                keyHoldI = false;

            if (bKey[14] && !t.isGameEnd()) {
                if (!keyHoldK)
                    rcrope->e.eModifyY(0);
                keyHoldK = true;
            }
            else
                keyHoldK = false;
        }
        if (rcropw != NULL) {
            if (bKey[11] && rcropw->w.getType() != 1) {
                if (!keyHoldJ)
                    rcropw->w.wMoveX(-1, 0);
                else
                    rcropw->w.wMoveX(-2, 0);
                keyHoldJ = true;
            }
            else
                keyHoldJ = false;

            if (bKey[12] && rcropw->w.getType() != 1) {
                if (!keyHoldL)
                    rcropw->w.wMoveX(1, 0);
                else
                    rcropw->w.wMoveX(2, 0);
                keyHoldL = true;
            }
            else
                keyHoldL = false;

            if (bKey[13] && !t.isGameEnd()) {
                if (!keyHoldI)
                    rcropw->w.wMoveY(1);
                keyHoldI = true;
            }
            else
                keyHoldI = false;

            if (bKey[14] && !t.isGameEnd()) {
                if (!keyHoldK)
                    rcropw->w.wMoveY(0);
                keyHoldK = true;
            }
            else
                keyHoldK = false;
        }


        // Game Logic
        // Check attack
        EffectMove(wUp, hUp);
        EffectMove(wMid, hMid);
        EffectMove(wDown, hDown); // Keep Order!
        AttackMove(hUp, wUp);
        AttackMove(hMid, wMid);
        AttackMove(hDown, wDown);


        // Check invalid entities and delete
        StatusAccountE(hUp);
        StatusAccountE(hMid);
        StatusAccountE(hDown);
        StatusAccountW(wUp);
        StatusAccountW(wMid);
        StatusAccountW(wDown);

        // Display to player
        for (int k = 0; k < 24; k++)
            bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("ADWS\x20\x31\x32\x33\x34\x35\xc0JLIK\x0D\x37\x38\x39\x30\xBD\x08\x01\x02"[k]))) != 0;

        SetCard(screen);
        SetEntity(screen);
        SetWeapon(screen);
        SetTimeCount(screen, t.getM(), t.getS());

        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }
    return 0;
}
/*
  
   *  ^  *
    / O \
     +++
      *
      
    __-$-__ 
     < 0 >
    __-|-__
      -$-

                    _____            
     \    /\    /     |     |  \    |
      \  /  \  /      |     |   \   |
       \/    \/     __|__   |    \  | 



        _-+-____ 
       {[]___  _)
    <=====|[   ]  
          // /   
          \\ \   
      ||---|o|      




        _-+-____
       {[]___  _)
    <=====|[   ]
          / /\ \ 
          | | \ \
      |---|---|o|


        _-+-____
       {[]___  _)
    <=====|[   ]
          / /\ \
          | | \ \
       |--|o|-|o|


        _[]======     
    ___/____\________ 
    \----\        ___\
   ==| [ ])======>____
   ==|_______________/

           /---->_._._._ 
          [_____-=-=-=-=
        ___/___\_____
       |__[]_____[]__\
      / …… …… …… …… ……\
*/