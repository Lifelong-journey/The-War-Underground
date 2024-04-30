#include "Gamestart.h"
#include "Card.h"
#include "Entity.h"
#include "Weapon.h"
#include "Define.h"

void wClear(wNode*& list, wNode*& wFree)
{
    wNode* now = NULL;
    for (auto i = list; i != NULL; i = i->ne)
    {
        if (now != NULL) {
            wNodeDelete(now, list);
            wNodeFree(now, wFree);
        }
        now = i;
    }
    if (now != NULL) {
        wNodeDelete(now, list);
        wNodeFree(now, wFree);
    }
    list = NULL;
}

void eClear(eNode*& list, eNode*& hFree)
{
    eNode* now = NULL;
    for (auto i = list; i != NULL; i = i->ne)
    {
        if (now != NULL) {
            eNodeDelete(now, list);
            eNodeFree(now, hFree);
        }
        now = i;
    }
    if (now != NULL) {
        eNodeDelete(now, list);
        eNodeFree(now, hFree);
    }
    list = NULL;
}

void setlcropw(int num, bool player, int x, eNode*& lcrope, eNode*& hFree, wNode*& lcropw, wNode*& wFree)
{
    if (lcrope != NULL) {
        eNodeFree(lcrope, hFree);
        lcrope = NULL;
    }
    if (lcropw == NULL)
        lcropw = getFreewNode(wFree);
    lcropw->w.setWeapon(num, player, x);
}

void setrcropw(int num, bool player, int x, eNode*& rcrope, eNode*& hFree, wNode*& rcropw, wNode*& wFree)
{
    if (rcrope != NULL) {
        eNodeFree(rcrope, hFree);
        rcrope = NULL;
    }
    if (rcropw == NULL)
        rcropw = getFreewNode(wFree);
    rcropw->w.setWeapon(num, player, x);
}

void setrcrope(int num, bool player, int x, wNode*& rcropw, eNode*& rcrope, wNode*& wFree, eNode*& hFree)
{
    if (rcropw != NULL) {
        wNodeFree(rcropw, wFree);
        rcropw = NULL;
    }

    if (rcrope == NULL)
        rcrope = getFreeeNode(hFree);
    rcrope->e.setEntity(num, player, x);
}

void setlcrope(int num, bool player, int x, eNode*& lcrope, wNode*& lcropw, wNode*& wFree, eNode*& hFree)
{
    if (lcropw != NULL) {
        wNodeFree(lcropw, wFree);
        lcropw = NULL;
    }

    if (lcrope == NULL)
        lcrope = getFreeeNode(hFree);
    lcrope->e.setEntity(num, player, x);
}

void EffectMove(wNode*& list, eNode* elist, wNode*& wFree)
{
    for (auto i = list; i != NULL; i = i->ne)
    {
        i->w.addTcount();
        if (i->w.getTcount() == i->w.getAspeed()) {
            i->w.resetTcount();
            if (TakeEffect(i, elist, list, wFree))
                return;
        }
        if (i->w.getMspeed() != 0)
            if (i->w.getPlayer() == 0)
                i->w.wMoveX(i->w.getMspeed(), 1);
            else
                i->w.wMoveX(-i->w.getMspeed(), 1);
    }
}

bool TakeEffect(wNode* wn, eNode* elist, wNode*& wlist, wNode*& wFree)
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
            wNodeFree(wn, wFree);
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
            wNodeFree(wn, wFree);
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

void AttackMove(eNode*& list, wNode*& listw, wNode*& wFree, eNode*& hUp, eNode*& hMid, eNode* hDown)
{
    for (auto i = list; i != NULL; i = i->ne)
    {
        if (i->e.getType() == 8) {
            Move(i);
            wNode* wn = getFreewNode(wFree);
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
            if (!CheckUpDown(i, hUp, hMid, hDown))
                Move(i); // not attack
        }
    }
}

void StatusAccountW(wNode* &list, wNode* &wFree)
{
    wNode* now = NULL;
    for (auto i = list; i != NULL; i = i->ne)
    {

        i->w.cutTime();
        if (now != NULL) {
            wNodeDelete(now, list);
            wNodeFree(now, wFree);
            now = NULL;
        }
        if (i->w.getTime() < 0)
            now = i;
    }
    if (now != NULL) {
        wNodeDelete(now, list);
        wNodeFree(now, wFree);
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

bool CheckMove(eNode* x, int dist)
{
    if (!x->e.getPlayer()) {
        if (x->ne == NULL || x->ne->e.getLx() > x->e.getRx() + dist)
            return true;
    }
    else {
        if (x->pre == NULL || x->pre->e.getRx() < x->e.getLx() - dist)
            return true;
    }
    x->e.resetMcount();
    return false;
}

void SetTimeCount(wchar_t* screen, int m, int s)
{
    wsprintf(&screen[10 * nScreenWidth + 30], L"%d : %d", m, s);
}

void StatusAccountE(eNode*& list, bool &Goliath, bool &Needle, eNode* &hFree, int &lMineral, int &rMineral)
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
            eNodeFree(now, hFree);
            now = NULL;
        }
        if (!CheckAlive(i))
            now = i;
    }
    if (now != NULL) {
        eNodeDelete(now, list);
        eNodeFree(now, hFree);
    }
}

bool CheckAlive(eNode* a)
{
    return (a->e.getLife() > 0);
}

void wNodeFree(wNode* x, wNode*& wFree)
{

    wFree->pre = x;
    x->ne = wFree;
    wFree = x;
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

void eNodeFree(eNode* x, eNode*& hFree)
{
    hFree->pre = x;
    x->ne = hFree;
    hFree = x;
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

bool CheckUpDown(eNode* x, eNode*& hUp, eNode*& hMid, eNode*& hDown)
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
            return true;
        }
    }
    return false;
}

eNode* getFreeeNode(eNode*& hFree)
{
    eNode* x = hFree;
    hFree = hFree->ne;
    x->pre = NULL;
    x->ne = NULL;
    return x;
}

wNode* getFreewNode(wNode*& wFree)
{
    wNode* x = wFree;
    wFree = wFree->ne;
    return x;
}

void AddwList(wNode* a, wNode*& list) // ͷ�巨
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

void AddList(eNode* a, eNode*& list)
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

void SetWeapon(wchar_t* screen, wNode* wUp, wNode* wMid, wNode* wDown, wNode* lcropw, wNode* rcropw)
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

void SetEntity(wchar_t* screen, eNode* hUp, eNode* hMid, eNode* hDown, eNode* lcrope, eNode* rcrope)
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

void PrintPipe(wchar_t* screen, int y, int x)
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

void SetScene(wchar_t* screen, int& lMineral, int& rMineral)
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
    PrintPipe(screen, 17, PIPEA - 2); // 30
    PrintPipe(screen, 17, PIPEB - 2);
    //PrintPipe(17, 68);
    //PrintPipe(17, 106);
    PrintPipe(screen, 17, PIPEC - 2);
    PrintPipe(screen, 30, PIPED - 2);
    PrintPipe(screen, 30, PIPEE - 2);
    PrintFlag(screen);
    wsprintf(&screen[nScreenWidth + 140], L"%d", lMineral);
    wsprintf(&screen[47 * nScreenWidth + 140], L"%d", rMineral);
    return;
}
