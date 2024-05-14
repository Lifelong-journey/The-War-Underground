#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <Windows.h>
#include <thread>
#include <random>
#include <ctime>
#include <unordered_map>
#include <queue>

#include "Define.h"
#include "Entity.h"
#include "Weapon.h"
#include "Card.h"
#include "Timecount.h"
#include "Choosecard.h"
#include "Gamestart.h"
#include "Foolai.h"
#include "Cleverai.h"

using namespace std;

typedef pair<int, int> PII;

//Entity test;
bool bKey[24]; // keyboard press

default_random_engine e(unsigned int(time(0)));


int nSpeedCounter = 0;
wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
struct eNode* hFree = NULL, * hUp = NULL, * hMid = NULL, * hDown = NULL;
struct wNode* wFree = NULL, * wUp = NULL, * wMid = NULL, * wDown = NULL;

int lcropc = 0, rcropc = 0;

int lnem = 0, rnem = 0;
Card card[12];
Entity* lbase = NULL, * rbase = NULL;

bool test = false;

void eFreelist(eNode* x)
{
    if (x == NULL)
        return;
    if (x->ne != NULL)
        eFreelist(x->ne);
    eNodeFree(x, hFree);
}

void wFreelist(wNode* x)
{
    if (x == NULL)
        return;
    if (x->ne != NULL)
        wFreelist(x->ne);
    wNodeFree(x, wFree);
}

void nreuse()
{
    eFreelist(hUp);
    eFreelist(hMid);
    eFreelist(hDown);
    wFreelist(wUp);
    wFreelist(wMid);
    wFreelist(wDown);
    hUp = hMid = hDown = NULL;
    wUp = wMid = wDown = NULL;
}

void nalloc()
{
    for (int i = 0; i < 30; i++) 
    {
        eNode* ep = new eNode();
        ep->e.setState(UNUSED);
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

void initcard(int* lq, int* rq, queue<int>* lsq, queue<int>* rsq)
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
    for (int i = 1; i <= 9; i++)
    {
        bool isUse = false;
        for (int j = 1; j <= 5; j++)
            if (card[j].getType() == lq[i]) {
                isUse = true;
                break;
            }
        if (!isUse)
            lsq->push(lq[i]);
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
    for (int i = 1; i <= 9; i++)
    {
        bool isUse = false;
        for (int j = 6; j <= 10; j++)
            if (card[j].getType() == rq[i]) {
                isUse = true;
                break;
            }
        if (!isUse)
            rsq->push(rq[i]);
    }
}

void print()
{
    wsprintf(&screen[10 * nScreenWidth + 10], L"!!!!!");
}

void SetCard(wchar_t* screen)
{
    for (int i = 1; i <= 10; i++)
        card[i].cDraw(screen);
}

void initBase()
{
    eNode* p1 = getFreeeNode(hFree);
    eNode* p2 = getFreeeNode(hFree);
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

void setInitial(wchar_t* screen, int pnumber)
{
    const int theH = 2, theL = 45;
    wsprintf(&screen[theH       * nScreenWidth + theL], L"888888888888888  oooo                        oooo         ooooo         oooo");
    wsprintf(&screen[(theH + 1) * nScreenWidth + theL], L"8'    888    '8  '888                         '8.         .888.         .8'");
    wsprintf(&screen[(theH + 2) * nScreenWidth + theL], L"'     888     '   888  .oo.                    '8.       .8' '8.       .8'              ");
    wsprintf(&screen[(theH + 3) * nScreenWidth + theL], L"      888         888P'Y88b   .ob888do.         '8.     .8'   '8.     .8'    .oooo.     8o. .o88o.");
    wsprintf(&screen[(theH + 4) * nScreenWidth + theL], L"      888         888   888  .b       8.         '8.   .8'     '8.   .8'    'P  )88b    888d8    8b");
    wsprintf(&screen[(theH + 5) * nScreenWidth + theL], L"      888         888   888  d888888888b          '8. .8'       '8. .8'      .oP'88b    888");
    wsprintf(&screen[(theH + 6) * nScreenWidth + theL], L"      888         888   888  'Y                    '888'         '888       d8(  888    888");
    wsprintf(&screen[(theH + 7) * nScreenWidth + theL], L"      888        o888o o999o  'Y888888P'            '8'           '8'       'Y888''8o  o888o");

    const int uH = 12, uL = 15;
    wsprintf(&screen[uH       * nScreenWidth + uL], L"ooooo    ooooo                        oooo                                 .ooooooo.                                                                   oooo");
    wsprintf(&screen[(uH + 1) * nScreenWidth + uL], L"'888'    '888'                        '88'                                d8P'   'Y8b                                                                  '88'");
    wsprintf(&screen[(uH + 2) * nScreenWidth + uL], L" 888      888                          88                                d88                                                                            88");
    wsprintf(&screen[(uH + 3) * nScreenWidth + uL], L" 888      888    8.oooooo.     .oooooo.88    .ob888do.    8o. .o88o.    888              8o. .o88o.     .ooooo.     ooo     88    8.oooooo.     .oooooo.88");
    wsprintf(&screen[(uH + 4) * nScreenWidth + uL], L" 888      888    888   888    .P'    'Y.8   .b       8.   888d8    8b   888      .oooo.  888d8    8b   d8     8b    '88     88    888   888    .P'    'Y.8");
    wsprintf(&screen[(uH + 5) * nScreenWidth + uL], L" 888      888    888   888    b(      )d8   d888888888b   888           888b     oo88    888          88       88    88     88    888   888    b(      )d8");
    wsprintf(&screen[(uH + 6) * nScreenWidth + uL], L" '88b    d88'    888   888    'b      d'8   'Y            888           '88b    o8 88    888           d8     8b     88     88    888   888    'b      d'8");
    wsprintf(&screen[(uH + 7) * nScreenWidth + uL], L"  'Y8bood8Y'    o888o o888o    'booood'88.   'Y888888P'  o888o           'Y8booc8P 88   o888o           'ooooo'      'Y8ooo8Y8o  o888o o888o    'booood'88.");

    const int tH = 40, tL = 80;
    if (pnumber != TWOPLAYER) {
        wsprintf(&screen[(tH + 0) * nScreenWidth + tL], L"─▄▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▄");
        wsprintf(&screen[(tH + 1) * nScreenWidth + tL], L"█░░░█░░░░░░░░░░▄▄░██░█");
        wsprintf(&screen[(tH + 2) * nScreenWidth + tL], L"█░▀▀█▀▀░▄▀░▄▀░░▀▀░▄▄░█");
        wsprintf(&screen[(tH + 3) * nScreenWidth + tL], L"█░░░▀░░░▄▄▄▄▄░░██░▀▀░█");
        wsprintf(&screen[(tH + 4) * nScreenWidth + tL], L"─▀▄▄▄▄▄▀─────▀▄▄▄▄▄▄▀");
    }


    const int hH = 30, hL = 120;
    if (pnumber != HISTORY) {
        wsprintf(&screen[(hH + 0) * nScreenWidth + hL], L"╦╦╦╦╦╦▄▀▀▀▀▀▀▀▀▄╦╦╦╦╦╦");
        wsprintf(&screen[(hH + 1) * nScreenWidth + hL], L"▒▓▒▓▒█╗░░░▐░░░░╔█▒▓▒▓▒");
        wsprintf(&screen[(hH + 2) * nScreenWidth + hL], L"▒▓▒▓▒█║░░░▐▄▄░░║█▒▓▒▓▒");
        wsprintf(&screen[(hH + 3) * nScreenWidth + hL], L"▒▓▒▓▒█╝░░░░░░░░╚█▒▓▒▓▒");
        wsprintf(&screen[(hH + 4) * nScreenWidth + hL], L"╩╩╩╩╩╩▀▄▄▄▄▄▄▄▄▀╩╩╩╩╩╩");
    }


    const int pH = 30, pL = 42;
    if (pnumber != PRACTICE) {
        wsprintf(&screen[(pH + 0) * nScreenWidth + pL], L"──▄▀▀▀▄───────────────");
        wsprintf(&screen[(pH + 1) * nScreenWidth + pL], L"──█───█───────────────");
        wsprintf(&screen[(pH + 2) * nScreenWidth + pL], L"─███████─────────▄▀▀▄─");
        wsprintf(&screen[(pH + 3) * nScreenWidth + pL], L"░██─▀─██░░█▀█▀▀▀▀█░░█░");
        wsprintf(&screen[(pH + 4) * nScreenWidth + pL], L"░███▄███░░▀░▀░░░░░▀▀░░");
    }

    switch (pnumber) {
    case PRACTICE:
        wsprintf(&screen[(pH + 1) * nScreenWidth + pL - 10], L" ▄▄   █ ▄▀  ▀▀     ▄▀▀  ▄█▄  ▀   ▄▀▀   ▀▀ ");
        wsprintf(&screen[(pH + 2) * nScreenWidth + pL - 10], L"█▄▄█  █▀   █  █   █      █   █  █     ▀▄▄▀");
        wsprintf(&screen[(pH + 3) * nScreenWidth + pL - 10], L"█     █     ▄▄ ▀▄  ▀▄▄   ▀▄  █   ▀▄▄  ▀▄▄▄");
        break;
    case HISTORY:
        wsprintf(&screen[(hH + 0) * nScreenWidth + hL - 10], L"█");
        wsprintf(&screen[(hH + 1) * nScreenWidth + hL - 10], L"█▄▄   ▀  ▄▀▀▀  ▄█▄   ▀▀   █ ▄▀ ▀▄ ▄▀");
        wsprintf(&screen[(hH + 2) * nScreenWidth + hL - 10], L"█  ▀  █  ▀▀▀▄   █   █  █  █▀     █");
        wsprintf(&screen[(hH + 3) * nScreenWidth + hL - 10], L"█  █  █  ▄▄▄▀   ▀▄   ▄▄   █      █");
        break;
    case TWOPLAYER:
        wsprintf(&screen[(tH + 1) * nScreenWidth + tL - 15], L"▄█▄  ▀     ▀   ▀▀     ▄▄   █   ▀▀   ▀▄ ▄▀  ▀▀   █ ▄▀");
        wsprintf(&screen[(tH + 2) * nScreenWidth + tL - 15], L" █    █ █ █   █  █   █▄▄█  █  █  █    █   ▀▄▄▀  █▀");
        wsprintf(&screen[(tH + 3) * nScreenWidth + tL - 15], L" ▀▄    ▄ ▄     ▄▄    █     ▀▄  ▄▄ ▀▄  █   ▀▄▄▄  █");
        break;
    }
    /*
     ▄▄   █ ▄▀  ▀▀     ▄▀▀  ▄█▄  ▀   ▄▀▀   ▀▀ 
    █▄▄█  █▀   █  █   █      █   █  █     ▀▄▄▀
    █     █     ▄▄ ▀▄  ▀▄▄   ▀▄  █   ▀▄▄  ▀▄▄▄

    ▄█▄  ▀     ▀   ▀▀     ▄▄   █   ▀▀   ▀▄ ▄▀  ▀▀   █ ▄▀
     █    █ █ █   █  █   █▄▄█  █  █  █    █   ▀▄▄▀  █▀
     ▀▄    ▄ ▄     ▄▄    █     ▀▄  ▄▄ ▀▄  █   ▀▄▄▄  █
  █
  █▄▄   ▀  ▄▀▀▀  ▄█▄   ▀▀   █ ▄▀ ▀▄ ▄▀
  █  ▀  █  ▀▀▀▄   █   █  █  █▀     █
  █  █  █  ▄▄▄▀   ▀▄   ▄▄   █      █

    */
}

int main()
{
    inititp();
    inititw();
    nalloc();
    while (1)
    {
        int lMineral = 6, rMineral = 6;
        bool keyHoldW = false, keyHoldS = false, keyHoldSpace = false, keyHoldA = false, keyHoldD = false; // need to be modified
        bool keyHoldJ = false, keyHoldL = false, keyHoldEnter = false, keyHoldI = false, keyHoldK = false;
        struct eNode* lcrope = NULL, * rcrope = NULL;
        struct wNode* lcropw = NULL, * rcropw = NULL;
        bool Goliath = false, Needle = false;
        int lq[11] = { 0 }, rq[11] = { 0 };
        queue<int> lsq, rsq;
        int lhh = 0, rhh = 0;
        bool isUseAI = false;
        // initgraph();

        // initfreelist
        initBase();

        HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        SetConsoleActiveScreenBuffer(hConsole);
        DWORD dwBytesWritten = 0;

        int crnum = 0;
        int x0 = 0, y0 = 0;
        int nSpeedCount = 0;
        bool isCountDown = false;
        int CountDown = 0;

        int PlayMode = -1;

        while (1)
        {
            this_thread::sleep_for(50ms);
            int pnumber = 0;
            for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
                screen[i] = L' ';


            POINT pt;
            GetCursorPos(&pt);
            int x = pt.x, y = pt.y;
            //wsprintf(&screen[30 * nScreenWidth + 10], L"%d %d", x, y);
            if (x >= 400 && x <= 600 && y >= 600 && y <= 690) {
                pnumber = PRACTICE;
                if (bKey[22])
                    PlayMode = PRACTICE;
            }

            else if (x >= 1130 && x <= 1330 && y >= 600 && y <= 690) {
                pnumber = HISTORY;
                if (bKey[22])
                    PlayMode = HISTORY;
            }

            else if (x >= 750 && x <= 950 && y >= 780 && y <= 870) {
                pnumber = TWOPLAYER;
                if (bKey[22])
                    PlayMode = TWOPLAYER;
            }

            if (PlayMode != -1)
                break;

            setInitial(screen, pnumber);

            for (int k = 0; k < 24; k++)
                bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("ADWS\x20\x31\x32\x33\x34\x35\xc0JLIK\x0D\x37\x38\x39\x30\xBD\x08\x01\x02"[k]))) != 0;
            WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
        }

        // AI mode start
        if (PlayMode == PRACTICE || PlayMode == HISTORY) {
            rhh = 9;
            rq[1] = 2, rq[2] = 3, rq[3] = 6, rq[4] = 7, rq[5] = 8, rq[6] = 9, rq[7] = 11, rq[8] = 1, rq[9] = 5;
            while (1)
            {
                ++nSpeedCount;
                bool readyToStart = (lhh == 9 && rhh == 9);
                this_thread::sleep_for(50ms);
                if (isCountDown) {
                    CountDown++;
                    setCountDown(screen, CountDown);
                    //wsprintf(&screen[26 * nScreenWidth + 150], L"%d", 5 - (CountDown / 20));
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
                ccaInput(bKey, lq, rq, x, y, x0, y0, crnum, lhh, rhh);
                ccSetScene(screen, crnum, x0, y0);
                ccSetQueue(screen, lq, rq);

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

            initcard(lq, rq, &lsq, &rsq);

            TimeCount t;
            bool Endgame = false;
            while (1)
            {
                this_thread::sleep_for(50ms);
                nSpeedCounter += 10;
                t.addMs();
                if (t.isGameEnd() && !Endgame) {
                    Endgame = true;
                    int llife = lbase->getLife();
                    int rlife = rbase->getLife();
                    lbase->setEntity(12, 0, 10);
                    rbase->setEntity(11, 1, 160);
                    lbase->mdfLife(-(1000 - llife) * 2);
                    rbase->mdfLife(-(1000 - rlife) * 2);
                    eClear(hUp, hFree);
                    eClear(hDown, hFree);
                    wClear(wUp, wFree);
                    wClear(wDown, wFree);
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
                    WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
                    continue;
                }
                for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
                    screen[i] = L' ';
                SetScene(screen, lMineral, rMineral);
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
                            lsq.push(card[lcropc].getType());
                            card[lcropc].setCard(0, lsq.front(), x);
                            lsq.pop();
                            lcrope = NULL;
                            lcropw = NULL;
                            lcropc = 0;
                        }
                    }
                    keyHoldSpace = true;
                }
                else
                    keyHoldSpace = false;

                if (lcrope != NULL && bKey[10]) {
                    lcropc = 0;
                    eNodeFree(lcrope, hFree);
                    lcrope = NULL;
                }
                if (lcropw != NULL && bKey[10]) {
                    lcropc = 0;
                    wNodeFree(lcropw, wFree);
                    lcropw = NULL;
                }

                for (int i = 5; i <= 9; i++)
                {
                    if (bKey[i]) {
                        int type = card[i - 4].getType();
                        lcropc = i - 4;
                        if (type <= 10)
                            setlcrope(type, 0, 20, lcrope, lcropw, wFree, hFree);
                        else
                            setlcropw(type - 10, 0, 20, lcrope, hFree, lcropw, wFree);
                        break;
                    }
                }



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


                // ai
                int choose = 0, road = -1;
                Card card1, card2;
                card1.setCard(0, lsq.front(), 0);
                card2.setCard(1, rsq.front(), 0);

                if (PlayMode == HISTORY ? fSearch(rMineral, hMid, hUp, hDown, card, &card1, choose, road, &t) : (MinMax(lMineral, rMineral, hMid, hUp, hDown, card, card1, card2, choose, road, &t, 1, { 0, 0 }, { 0, 0 }, { 0, 0 }), choose)) {
                    if (card[choose].getType() == 11) {
                        isUseAI = true;
                        setrcropw(1, 1, 150, rcrope, hFree, rcropw, wFree);
                        if (road == UPROAD)
                            rcropw->w.wMoveY(true);
                        if (road == DOWNROAD)
                            rcropw->w.wMoveY(false);
                        switch (road) {
                        case UPROAD:
                            AddwList(rcropw, wUp);
                            break;
                        case MIDROAD:
                            AddwList(rcropw, wMid);
                            break;
                        case DOWNROAD:
                            AddwList(rcropw, wDown);
                            break;
                        }
                    }
                    else {
                        if (rcrope == NULL || rcrope->e.getType() != card[choose].getType() || rcrope->e.getRoad() != road) {
                            setrcrope(card[choose].getType(), 1, 150, rcropw, rcrope, wFree, hFree);
                            if (road == UPROAD)
                                rcrope->e.eModifyY(true);
                            if (road == DOWNROAD)
                                rcrope->e.eModifyY(false);
                        }


                        switch (road) {
                        case UPROAD:
                            if (!CheckCollision(rcrope, hUp)) {
                                AddList(rcrope, hUp);
                                isUseAI = true;
                            }
                            else
                                rcrope->e.eMoveX(-1, 0);
                            break;

                        case MIDROAD:
                            if (!CheckCollision(rcrope, hMid)) {
                                AddList(rcrope, hMid);
                                isUseAI = true;
                            }
                            else
                                rcrope->e.eMoveX(-1, 0);
                            break;

                        case DOWNROAD:
                            if (!CheckCollision(rcrope, hDown)) {
                                AddList(rcrope, hDown);
                                isUseAI = true;
                            }
                            else
                                rcrope->e.eMoveX(-1, 0);
                            break;
                        }
                    }
                    if (isUseAI) {
                        rMineral -= card[choose].getMineral();
                        int x = card[choose].getLx();
                        rsq.push(card[choose].getType());
                        card[choose].setCard(1, rsq.front(), x);
                        rsq.pop();
                        rcropc = 0;
                        rcrope = NULL;
                        rcropw = NULL;
                        isUseAI = false;
                    }
                }


                // Game Logic
                // Check attack
                EffectMove(wUp, hUp, wFree);
                EffectMove(wMid, hMid, wFree);
                EffectMove(wDown, hDown, wFree); // Keep Order!
                AttackMove(hUp, wUp, wFree, hUp, hMid, hDown);
                AttackMove(hMid, wMid, wFree, hUp, hMid, hDown);
                AttackMove(hDown, wDown, wFree, hUp, hMid, hDown);


                // Check invalid entities and delete
                StatusAccountE(hUp, Goliath, Needle, hFree, lMineral, rMineral, t.getM() * 60 + t.getS(), t.getMs());
                StatusAccountE(hMid, Goliath, Needle, hFree, lMineral, rMineral, t.getM() * 60 + t.getS(), t.getMs());
                StatusAccountE(hDown, Goliath, Needle, hFree, lMineral, rMineral, t.getM() * 60 + t.getS(), t.getMs());

                StatusAccountW(wUp, wFree);
                StatusAccountW(wMid, wFree);
                StatusAccountW(wDown, wFree);

                // Display to player
                for (int k = 0; k < 24; k++)
                    bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("ADWS\x20\x31\x32\x33\x34\x35\xc0JLIK\x0D\x37\x38\x39\x30\xBD\x08\x01\x02"[k]))) != 0;

                SetCard(screen);
                SetEntity(screen, hUp, hMid, hDown, lcrope, rcrope);
                setBullet(screen);
                SetWeapon(screen, wUp, wMid, wDown, lcropw, rcropw);
                SetTimeCount(screen, t.getM(), t.getS());

                //int cntt = 0;
                //for (auto i = wMid; i != NULL; i = i->ne)
                //    cntt++;
                //wsprintf(&screen[10 * nScreenWidth + 60], L"%d", cntt);

                WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
            }
        }




        // Two player mode start
        else if (PlayMode == TWOPLAYER) {
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

                cctInput(bKey, lq, rq, x, y, x0, y0, crnum, lhh, rhh);
                ccSetScene(screen, crnum, x0, y0);
                ccSetQueue(screen, lq, rq);

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

            initcard(lq, rq, &lsq, &rsq);
            TimeCount t;
            bool Endgame = false;

            while (1)
            {
                this_thread::sleep_for(50ms);
                nSpeedCounter += 10;
                t.addMs();
                if (t.isGameEnd() && !Endgame) {
                    Endgame = true;
                    lbase->setEntity(12, 0, 10);
                    rbase->setEntity(11, 1, 170);
                    eClear(hUp, hFree);
                    eClear(hDown, hFree);
                    wClear(wUp, wFree);
                    wClear(wDown, wFree);
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
                    WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
                    continue;
                }
                for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
                    screen[i] = L' ';
                SetScene(screen, lMineral, rMineral);
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
                            lsq.push(card[lcropc].getType());
                            card[lcropc].setCard(0, lsq.front(), x);
                            lsq.pop();
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
                            rsq.push(card[rcropc].getType());
                            card[rcropc].setCard(1, rsq.front(), x);
                            rsq.pop();
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
                    eNodeFree(lcrope, hFree);
                    lcrope = NULL;
                }
                if (lcropw != NULL && bKey[10]) {
                    lcropc = 0;
                    wNodeFree(lcropw, wFree);
                    lcropw = NULL;
                }
                if (rcrope != NULL && bKey[21]) {
                    rcropc = 0;
                    eNodeFree(rcrope, hFree);
                    rcrope = NULL;
                }
                if (rcropw != NULL && bKey[21]) {
                    rcropc = 0;
                    wNodeFree(rcropw, wFree);
                    rcropw = NULL;
                }
                for (int i = 5; i <= 9; i++)
                {
                    if (bKey[i]) {
                        int type = card[i - 4].getType();
                        lcropc = i - 4;
                        if (type <= 10)
                            setlcrope(type, 0, 20, lcrope, lcropw, wFree, hFree);
                        else
                            setlcropw(type - 10, 0, 20, lcrope, hFree, lcropw, wFree);
                        break;
                    }
                }
                for (int i = 16; i <= 20; i++)
                {
                    if (bKey[i]) {
                        rcropc = i - 10;
                        int type = card[i - 10].getType();
                        if (type <= 10)
                            setrcrope(type, 1, 150, rcropw, rcrope, wFree, hFree);
                        else
                            setrcropw(type - 10, 1, 150, rcrope, hFree, rcropw, wFree);
                        break;
                    }
                }

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
                EffectMove(wUp, hUp, wFree);
                EffectMove(wMid, hMid, wFree);
                EffectMove(wDown, hDown, wFree); // Keep Order!
                AttackMove(hUp, wUp, wFree, hUp, hMid, hDown);
                AttackMove(hMid, wMid, wFree, hUp, hMid, hDown);
                AttackMove(hDown, wDown, wFree, hUp, hMid, hDown);


                // Check invalid entities and delete
                StatusAccountE(hUp, Goliath, Needle, hFree, lMineral, rMineral, t.getM() * 60 + t.getS(), t.getMs());
                StatusAccountE(hMid, Goliath, Needle, hFree, lMineral, rMineral, t.getM() * 60 + t.getS(), t.getMs());
                StatusAccountE(hDown, Goliath, Needle, hFree, lMineral, rMineral, t.getM() * 60 + t.getS(), t.getMs());
                StatusAccountW(wUp, wFree);
                StatusAccountW(wMid, wFree);
                StatusAccountW(wDown, wFree);

                // Display to player
                for (int k = 0; k < 24; k++)
                    bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("ADWS\x20\x31\x32\x33\x34\x35\xc0JLIK\x0D\x37\x38\x39\x30\xBD\x08\x01\x02"[k]))) != 0;

                SetCard(screen);
                SetEntity(screen, hUp, hMid, hDown, lcrope, rcrope);
                setBullet(screen);
                SetWeapon(screen, wUp, wMid, wDown, lcropw, rcropw);
                SetTimeCount(screen, t.getM(), t.getS());


                WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
            }       
        }
        nreuse();
        clearBullet();
    }

    return 0;
}

/*
  888888888888888  oooo                        oooo         ooooo         oooo
  8'    888    '8  '888                         '8.         .888.         .8'
  '     888     '   888  .oo.                    '8.       .8' '8.       .8'
        888         888P"Y88b   .ob888do.         '8.     .8'   '8.     .8'    .oooo.     8o. .o88o.
        888         888   888  .b       8.         '8.   .8'     '8.   .8'    'P  )88b    888d8    8b
        888         888   888  d888888888b          '8. .8'       '8. .8'      .oP"88b    888
        888         888   888  'Y                    '888'         '888       d8(  888    888
        888        o888o o999o  'Y888888P'            '8'           '8'       'Y888""8o  o888o


  ooooo    ooooo                        oooo                                 .ooooooo.                                                                   oooo
  '888'    '888'                        '88'                                d8P'   'Y8b                                                                  '88'
   888      888                          88                                d88                                                                            88
   888      888    8.oooooo.     .oooooo.88    .ob888do.    8o. .o88o.    888              8o. .o88o.     .ooooo.     ooo     88    8.oooooo.     .oooooo.88
   888      888    888   888    .P"    "Y.8   .b       8.   888d8    8b   888      .oooo.  888d8    8b   d8     8b    '88     88    888   888    .P"    "Y.8
   888      888    888   888    b(      )d8   d888888888b   888           888b     oo88    888          88       88    88     88    888   888    b(      )d8
   '88b    d88'    888   888    'b      d'8   'Y            888           '88b    o8 88    888           d8     8b     88     88    888   888    'b      d'8
    'Y8bood8Y'    o888o o888o    'booood'88.   'Y888888P'  o888o           'Y8booc8P 88   o888o           'ooooo'      'Y8ooo8Y8o  o888o o888o    'booood'88.

*/

/*

─▄▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▄
█░░░█░░░░░░░░░░▄▄░██░█
█░▀▀█▀▀░▄▀░▄▀░░▀▀░▄▄░█
█░░░▀░░░▄▄▄▄▄░░██░▀▀░█
─▀▄▄▄▄▄▀─────▀▄▄▄▄▄▄▀

    ▄█▄  ▀     ▀   ▀▀     ▄▄   █   ▀▀   ▀▄ ▄▀  ▀▀   █ ▄▀
     █    █ █ █   █  █   █▄▄█  █  █  █    █   ▀▄▄▀  █▀
     ▀▄    ▄ ▄     ▄▄    █     ▀▄  ▄▄ ▀▄  █   ▀▄▄▄  █


╦╦╦╦╦╦▄▀▀▀▀▀▀▀▀▄╦╦╦╦╦╦
▒▓▒▓▒█╗░░░▐░░░░╔█▒▓▒▓▒
▒▓▒▓▒█║░░░▐▄▄░░║█▒▓▒▓▒
▒▓▒▓▒█╝░░░░░░░░╚█▒▓▒▓▒
╩╩╩╩╩╩▀▄▄▄▄▄▄▄▄▀╩╩╩╩╩╩

  █
  █▄▄   ▀  ▄▀▀▀  ▄█▄   ▀▀   █ ▄▀ ▀▄ ▄▀
  █  ▀  █  ▀▀▀▄   █   █  █  █▀     █
  █  █  █  ▄▄▄▀   ▀▄   ▄▄   █      █


─────▄▄────▄▀▀█▀▀▀▀▄
──▄▀▀──▀▀▄▄█▄▄█────█
▄▀─────────█──█────█
────────────▀▀▀▀▀▀▀
──▄▀▀▀▄───────────────
──█───█───────────────
─███████─────────▄▀▀▄─
░██─▀─██░░█▀█▀▀▀▀█░░█░
░███▄███░░▀░▀░░░░░▀▀░░

     ▄▄   █ ▄▀  ▀▀     ▄▀▀  ▄█▄  ▀   ▄▀▀   ▀▀ 
    █▄▄█  █▀   █  █   █      █   █  █     ▀▄▄▀
    █     █     ▄▄ ▀▄  ▀▄▄   ▀▄  █   ▀▄▄  ▀▄▄▄

   ▄︻┻═┳一  一═┳┻︻▄

*/
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