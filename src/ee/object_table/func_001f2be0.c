#include "dotr/ee/object_table.h"

int SzModel_InqRefID(int modelId, int refIdIndex)
{
    struct DotrObject *p;
    if (modelId < 0 || modelId >= 84) {
        return -1;
    }
    p = gSzModel_ahModel[modelId];
    if (p == 0) {
        return -1;
    }
    if (refIdIndex < 0 || refIdIndex >= 8) {
        return -1;
    }
    return p->boneRefIds[refIdIndex];
}
