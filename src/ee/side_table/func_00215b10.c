#include "dotr/ee/side_table.h"

int SzData_CalcExp(int side, int cardNo, int op, int expChange)
{
    struct DotrSide *p = DOTR_SIDE(side);
    struct DotrSideRecord *r;
    int currentExp, v;
    if (side < 0 || side >= 2) {
        return 0;
    }
    if (cardNo == 671) {
        cardNo = 999;
    }
    if (cardNo < 0 || cardNo >= 854) {
        return 0;
    }
    r = &p->cards[cardNo];
    currentExp = r->exp;
    switch (op) {
    case 0: v = currentExp + expChange; break;
    case 1: v = currentExp - expChange; break;
    case 2: v = currentExp * expChange; break;
    case 3: v = currentExp / expChange; break;
    case 4: v = expChange; break;
    default: return 0;
    }
    r->exp = DOTR_SIDE_MIN(DOTR_SIDE_MAX(v, 0), 0xffff);
    return 1;
}
