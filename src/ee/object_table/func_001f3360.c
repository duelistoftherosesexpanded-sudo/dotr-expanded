#include "dotr/ee/object_table.h"

int SzModel_GetPauseFlag(int modelId)
{
    struct DotrObject *p;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[modelId];
    if (p == 0) {
        return 0;
    }
    return p->stateFlags.unknown_bit_0 ? 1 : 0;
}
