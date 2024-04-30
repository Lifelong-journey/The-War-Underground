#include <Windows.h>

#include "Choosecard.h"
#include "Define.h"
#include "Card.h"

void ccSetQueue(wchar_t* screen, int* lq, int* rq)
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

void ccInput(bool* bKey, int* lq, int* rq, int &x, int &y, int &x0, int &y0, int &crnum, int &lhh, int &rhh)
{
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
        for (int i = 1; i <= rhh; i++)
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
}

void ccSetScene(wchar_t* screen, int crnum, int x, int y)
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