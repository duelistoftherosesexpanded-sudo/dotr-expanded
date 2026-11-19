#include "dotr/ee/state_grid.h"

int SzDuel_TrimUnitDir(int a, int b, int c, int d)
{
    struct DotrStateItem *p = SzDuel_GetUnit(a, b);
    struct DotrStateItem *q = SzDuel_GetUnit(c, d);
    int r = 0;
    if (p == q) {
        q = 0;
    }
    if (p && q) {
        r |= SzDuel_TrimUnitDir2(a, b, q->locCol, q->locRow);
        r |= SzDuel_TrimUnitDir2(c, d, p->locCol, p->locRow);
    } else {
        if (p) {
            r |= SzDuel_TrimUnitDir1(a, b);
        }
        if (q) {
            r |= SzDuel_TrimUnitDir1(c, d);
        }
    }
    return r;
}
