#include "dotr/ee/object_table.h"

int SzModel_SetDeform(int modelId, int value)
{
    struct DotrObject *p;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[modelId];
    if (p == 0) {
        return 0;
    }
    if (p->drawFlag.unknown_bit_4 != value) {
        p->drawFlag.unknown_bit_4 = value;
    }
    return 1;
}
