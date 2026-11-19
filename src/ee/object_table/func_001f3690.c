#include "dotr/ee/object_table.h"

int SzModel_GetStoneRatio(int index)
{
    struct DotrObject *p;
    if (index < 0 || index >= 84) {
        return -1;
    }
    p = gSzModel_ahModel[index];
    if (p == 0) {
        return -1;
    }
    return p->stateFlags.unknown_bit_1 ? p->stoneRatio : -1;
}
