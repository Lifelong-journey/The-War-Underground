#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <Windows.h>
#include <thread>

#include "Define.h"
#include "Entity.h"
#include "Weapon.h"
#include "Card.h"

using namespace std;

//Entity test;
bool bKey[16]; // keyboard press
//int rNum[4] = { 17, 30, 42 };
//Entity test;
bool keyHoldW = false, keyHoldS = false, keyHoldSpace = false, keyHoldA = false, keyHoldD = false;
int nSpeedCounter = 0;
wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
struct eNode* hFree = NULL, * hUp = NULL, * hMid = NULL, * hDown = NULL;
struct wNode* wFree = NULL, * wUp = NULL, * wMid = NULL, * wDown = NULL;
struct eNode* crope = NULL;
struct wNode* cropw = NULL;
Card card[12];

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
    card[1].setCard(0, 10, 10);
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
    return;
}

void SetSceneB(wchar_t* screen)
{


    return;
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

    if (crope != NULL) {
        //wsprintf(&screen[10 * nScreenWidth + 10], L"!!!!!");
        crope->e.eDraw(screen);
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
    if (cropw != NULL) {
        cropw->w.wDraw(screen);
    }
}

void SetCard(wchar_t* screen)
{
    card[1].cDraw(screen);
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
    if (x->e.getRx() + dist > 170 || x->e.getLx() + dist < 10)
        return false;
    if (!x->e.getPlayer()) {
        if (x->ne == NULL || (x->ne->e.getPlayer() == 0 && (x->ne->e.getType() == 0 || x->ne->e.getType() == 4 || x->ne->e.getType() == 5)) || x->ne->e.getLx() > x->e.getRx() + dist)
            return true;
    }
    else {
        if (x->pre == NULL || (x->pre->e.getPlayer() == 1 && (x->pre->e.getType() == 0 || x->pre->e.getType() == 4 || x->pre->e.getType() == 5)) || x->pre->e.getRx() < x->e.getLx() - dist)
            return true;
    }
    return false;
}

void Move(eNode* x)
{
    if (x->e.getMspeed() == 0)
        return;
    x->e.addMcount();

    if (x->e.getMcount() % x->e.getMspeed() == 0 && CheckMove(x, 1)) { // not correct

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
            wn->w.setWeapon(6, i->e.getPlayer());
            wn->w.setX(i->e.getLx() + 3 - 20);
            AddwList(wn, listw);

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

void setcrope(int num, bool player, int x)
{
    if (cropw != NULL) {
        wNodeFree(cropw);
        cropw = NULL;
    }

    if (crope == NULL)
        crope = getFreeeNode();
    crope->e.setEntity(num, player, x);
}

void setcropw(int num, bool player)
{
    if (crope != NULL) {
        eNodeFree(crope);
        crope = NULL;
    }
    if (cropw == NULL)
        cropw = getFreewNode();
    cropw->w.setWeapon(num, player);
}

int main()
{
    inititp();
    inititw();
    initcard();
    // initgraph();
    nalloc();
    // initfreelist

    //test.setEntity(2, 0);
    initBase();

    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;
    //while (1)
    //{
    //    for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
    //        screen[i] = L' ';
    //    SetSceneB(screen);
    //    this_thread::sleep_for(50ms);
    //    nSpeedCounter += 10;
    //    
    //}
    while (1)
    {
        for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
            screen[i] = L' ';
        SetScene(screen);
        // Timing
        this_thread::sleep_for(50ms);
        nSpeedCounter += 10;
        // Input
        if (bKey[4]) {
            //test = true;
            if (!keyHoldSpace) {
                if (cropw != NULL) {
                    int road = cropw->w.getRoad();
                    if (road == UPROAD)
                        AddwList(cropw, wUp);
                    else if (road == MIDROAD)
                        AddwList(cropw, wMid);
                    else
                        AddwList(cropw, wDown);
                    cropw = NULL;
                }
                if (crope != NULL) {
                    if (crope->e.getRoad() == UPROAD && !CheckCollision(crope, hUp)) {
                       AddList(crope, hUp);
                       crope = NULL;
                    }
                    else if (crope->e.getRoad() == MIDROAD && !CheckCollision(crope, hMid)) {
                        //wsprintf(&screen[10 * nScreenWidth + 10], L"!!!!!");
                        //test = true;
                        AddList(crope, hMid);
                        crope = NULL;
                    }
                    else if (crope->e.getRoad() == DOWNROAD && !CheckCollision(crope, hDown)) {
                        AddList(crope, hDown);
                        crope = NULL;
                    }
                }
            }
            keyHoldSpace = true;   
        }
        else
            keyHoldSpace = false;

        if (crope != NULL && bKey[14]) {
            eNodeFree(crope);
            crope = NULL;
        }
        if (cropw != NULL && bKey[14]) {
            wNodeFree(cropw);
            cropw = NULL;
        }

        if (bKey[5]) {
            //setcropw(6, 1);
            setcrope(8, 1, 150);
        }
        if (bKey[6]) {
            setcrope(1, 1, 150);
        }
        if (bKey[7]) {
            setcrope(3, 0, -1);
        }
        if (bKey[8]) {
            setcrope(4, 1, -1);
        }
        if (bKey[9]) {
            setcrope(5, 1, -1);
        }
        if (wMid != NULL)
            wsprintf(&screen[16 * nScreenWidth + 30], L"%d", wMid->w.getTime());
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

        

        if (crope != NULL) {
            if (bKey[0])
                crope->e.eMoveX(-1, 0);

            if (bKey[1])
                crope->e.eMoveX(1, 0);

            if (bKey[2]) {
                if (!keyHoldW)
                    crope->e.eModifyY(1);
                keyHoldW = true;
            }
            else
                keyHoldW = false;

            if (bKey[3]) {
                if (!keyHoldS)
                    crope->e.eModifyY(0);
                keyHoldS = true;
            }
            else
                keyHoldS = false;
        }
        if (cropw != NULL) {
            if (bKey[0] && cropw->w.getType() != 1) {
                if (!keyHoldA)
                    cropw->w.wMoveX(-1, 0);
                else
                    cropw->w.wMoveX(-2, 0);
                keyHoldA = true;
            }
            else
                keyHoldA = false;

            if (bKey[1] && cropw->w.getType() != 1) {
                if (!keyHoldD)
                    cropw->w.wMoveX(1, 0);
                else
                    cropw->w.wMoveX(2, 0);
                keyHoldD = true;
            }
            else
                keyHoldD = false;

            if (bKey[2]) {
                if (!keyHoldW)
                    cropw->w.wMoveY(1);
                keyHoldW = true;
            }
            else
                keyHoldW = false;

            if (bKey[3]) {
                if (!keyHoldS)
                    cropw->w.wMoveY(0);
                keyHoldS = true;
            }
            else
                keyHoldS = false;
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
        for (int k = 0; k < 16; k++)
            bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("ADWS\x20\x31\x32\x33\x34\x35\x36\x37\x38\x39\xc0"[k]))) != 0; // left, right, up, down, space, 1, 2, ~

        SetCard(screen);
        SetEntity(screen);
        SetWeapon(screen);

        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }
    return 0;
}