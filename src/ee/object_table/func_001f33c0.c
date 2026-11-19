#include "dotr/ee/object_table.h"

int SzModel_SetStoneFlag(int modelId, int value)
{
    struct DotrObject *p;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[modelId];
    if (p == 0) {
        return 0;
    }
    p->stateFlags.unknown_bit_1 = value;
    return 1;
}
