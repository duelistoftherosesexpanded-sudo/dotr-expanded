#include "dotr/ee/object_table.h"

int SzModel_GetDeform(int modelId)
{
    struct DotrObject *p;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[modelId];
    if (p == 0) {
        return 0;
    }
    return p->drawFlag.unknown_bit_4 ? 1 : 0;
}
