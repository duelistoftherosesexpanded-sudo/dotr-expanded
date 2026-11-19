#include "dotr/ee/small_misc.h"

int SzDuel_FreeCardRequest(struct DotrMiscNode *p)
{
    if (p == 0) {
        return 0;
    }
    SzDuel_ClearDrawCard(p);
    p->unknown_90 = 0;
    return 1;
}
