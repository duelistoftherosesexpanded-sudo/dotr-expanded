#include "dotr/ee/packed_item.h"

struct DotrStateItemObject *SzEffect_GetCardRequest2(int packed)
{
    struct DotrStateItem *p = SzDuel_GetUnit(packed >> 8, packed & 0xff);
    if (p == 0) {
        return 0;
    }
    return p->cardModel;
}
