#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <Windows.h>
#include <thread>

#include "Entity.h"

using namespace std;

//Entity test;
bool bKey[16]; // keyboard press
//int rNum[4] = { 17, 30, 42 };
//Entity test;
bool keyHoldW = false, keyHoldS = false, keyHoldSpace = false;
int nSpeedCounter = 0;
wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
struct eNode* hFree = NULL, * hUp = NULL, * hMid = NULL, * hDown = NULL;
struct eNode* crop = NULL;

bool test = false;

void nalloc()
{
    for (int i = 0; i < 30; i++) 
    {
        eNode* ep = new eNode();
        ep->pre = NULL;
        ep->ne = hFree;
        hFree = ep;
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



void SetEntity(wchar_t* screen)
{
    for (auto i = hUp; i != NULL; i = i->ne)
        i->e.eDraw(screen);
    for (auto i = hMid; i != NULL; i = i->ne)
        i->e.eDraw(screen);
    for (auto i = hDown; i != NULL; i = i->ne)
        i->e.eDraw(screen);
    if (crop != NULL) {
        //wsprintf(&screen[10 * nScreenWidth + 10], L"!!!!!");
        crop->e.eDraw(screen);
    }

    return;
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

eNode* getFreeNode()
{
    eNode* x = hFree;
    hFree = hFree->ne;
    return x;
}

bool CheckMove(eNode* x, int dist)
{

    if (x->e.getRx() + dist > 170 || x->e.getLx() + dist < 10)
        return false;
    if (!x->e.getPlayer()) {
        if (x->ne == NULL || x->ne->e.getLx() > x->e.getRx() + dist)
            return true;
    }
    else {
        if (x->pre == NULL || x->pre->e.getRx() < x->e.getLx() - dist)
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

void initBase()
{
    eNode* p1 = getFreeNode();
    eNode* p2 = getFreeNode();
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

void NodeFree(eNode* x, eNode* &list)
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
                NodeFree(x, hMid);
                AddList(x, hUp);
                return true;
            }
        if (lx <= PIPED && rx >= PIPED || lx <= PIPEE && rx >= PIPEE)
            if (CheckAttackB(x, hDown) && !CheckCollision(x, hDown)) {
                x->e.eMoveDown();
                NodeFree(x, hMid);
                AddList(x, hDown);
                return true;
            }
    }
    else if (road == UPROAD) {
        if ((!player && (lx <= PIPEA && rx >= PIPEA)) || (player && (lx <= PIPEC && rx >= PIPEC)) || (lx <= PIPEB && rx >= PIPEB)) {
            if (CheckAttackB(x, hMid) && !CheckCollision(x, hMid)) {
                x->e.eMoveDown();
                NodeFree(x, hUp);
                AddList(x, hMid);
                return true;
            }
        }
        else if ((!player && lx <= PIPEC && rx >= PIPEC) || (player && lx <= PIPEA && rx >= PIPEA)) {
            if (!CheckCollision(x, hMid)) {
                x->e.eMoveDown();
                NodeFree(x, hUp);
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
                NodeFree(x, hDown);
                AddList(x, hMid);
                return true;
            }
        }
        else if ((!player && lx <= PIPEE && rx >= PIPEE) || (player && lx <= PIPED && rx >= PIPED)) {
            if (!CheckCollision(x, hMid)) {
                x->e.eMoveUp();
                NodeFree(x, hDown);
                AddList(x, hMid);
            }
            else
                wait();
            return true;
        }
    }
    return false;
}

void StatusAccount(eNode* &list)
{
    eNode* now = NULL;
    for (auto i = list; i != NULL; i = i->ne)
    {
        if (now != NULL) {
            NodeFree(now, list);
            now = NULL;
        }
        if (!CheckAlive(i))
            now = i;
    }
    if (now != NULL)
        NodeFree(now, list);
}

void AttackMove(eNode*& list)
{
    for (auto i = list; i != NULL; i = i->ne)
    {
        if (!CheckAttack(i)) {
            //i->e.setDir(0); // this will be done at the draw part
            i->e.resetAcount();
            if (!CheckUpDown(i))
                Move(i); // not attack
        }
    }
}

int main()
{
    inititp();
    // initgraph();
    nalloc();
    // initfreelist

    //test.setEntity(2, 0);
    initBase();

    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;
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
                if (crop != NULL) {
                    if (crop->e.getRoad() == UPROAD && !CheckCollision(crop, hUp)) {
                       AddList(crop, hUp);
                       crop = NULL;
                    }
                    else if (crop->e.getRoad() == MIDROAD && !CheckCollision(crop, hMid)) {
                        //wsprintf(&screen[10 * nScreenWidth + 10], L"!!!!!");
                        //test = true;
                        AddList(crop, hMid);
                        crop = NULL;
                    }
                    else if (crop->e.getRoad() == DOWNROAD && !CheckCollision(crop, hDown)) {
                        AddList(crop, hDown);
                        crop = NULL;
                    }
                }
            }
            keyHoldSpace = true;   
        }
        else
            keyHoldSpace = false;

        if (crop != NULL && bKey[14]) {
            crop = NULL;
        }

        if (crop == NULL && bKey[5]) {
            crop = getFreeNode();
            crop->e.setEntity(10, 0, -1);
        }
        if (crop == NULL && bKey[6]) {
            crop = getFreeNode();
            crop->e.setEntity(1, 1, 150);
        }
        if (crop == NULL && bKey[7]) {
            crop = getFreeNode();
            crop->e.setEntity(3, 0, -1);
        }
        if (crop == NULL && bKey[8]) {
            crop = getFreeNode();
            crop->e.setEntity(4, 1, -1);
        }
        if (crop == NULL && bKey[9]) {
            crop = getFreeNode();
            crop->e.setEntity(5, 1, -1);
        }
        if (hDown != NULL)
            wsprintf(&screen[16 * nScreenWidth + 30], L"%d", hDown->e.getAcount());
        //if (hDown != NULL && hDown->e.getState() == ATTACK) {
        //    wsprintf(&screen[10 * nScreenWidth + 10], L"!!!!!");
        //}
        /*if (test)
            wsprintf(&screen[10 * nScreenWidth + 10], L"!!!!!");*/
        /*int cnt = 0;
        for (auto i = hMid; i != NULL; i = i->ne) {
            cnt++;
            wsprintf(&screen[10 * nScreenWidth + 10], L"%d", cnt);
        }*/

        

        if (crop != NULL) {
            if (bKey[0])
                crop->e.eMoveX(-1, 0);

            if (bKey[1])
                crop->e.eMoveX(1, 0);

            if (bKey[2]) {
                if (!keyHoldW)
                    crop->e.eModifyY(1);
                keyHoldW = true;
            }
            else
                keyHoldW = false;

            if (bKey[3]) {
                if (!keyHoldS)
                    crop->e.eModifyY(0);
                keyHoldS = true;
            }
            else
                keyHoldS = false;
        }

        // Game Logic
        // Check attack
        AttackMove(hUp);
        AttackMove(hMid);
        AttackMove(hDown);

        // Check invalid entities and delete
        StatusAccount(hUp);
        StatusAccount(hMid);
        StatusAccount(hDown);

        // Display to player
        for (int k = 0; k < 16; k++)
            bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("ADWS\x20\x31\x32\x33\x34\x35\x36\x37\x38\x39\xc0"[k]))) != 0; // left, right, up, down, space, 1, 2, ~


        SetEntity(screen);

        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }
    return 0;
}
