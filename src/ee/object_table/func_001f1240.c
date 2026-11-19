#include "dotr/ee/object_table.h"

int SzModel_IsModelReady(int index)
{
    struct DotrObject *p;
    if (index < 0 || index >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[index];
    if (p == 0) {
        return 0;
    }
    return p->drawFlag.unknown_bit_2 ? 1 : 0;
}
