#include "dotr/ee/object_table.h"

int SzModel_GetCalcFlag(int modelId)
{
    struct DotrObject *p;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[modelId];
    if (p == 0) {
        return 0;
    }
    return p->drawFlag.unknown_bit_6 ? 1 : 0;
}
