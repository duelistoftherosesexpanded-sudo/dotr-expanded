#include "dotr/ee/object_table.h"

int SzModel_GetAnimParam(int modelId, int paramIndex)
{
    struct DotrObject *p;
    if (modelId < 0 || modelId >= 84) {
        return -1;
    }
    if (paramIndex < 0 || paramIndex >= 4) {
        return -1;
    }
    p = gSzModel_ahModel[modelId];
    if (p == 0) {
        return -1;
    }
    return p->unknown_1e0[paramIndex];
}
