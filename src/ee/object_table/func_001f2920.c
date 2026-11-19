#include "dotr/ee/object_table.h"

int SzModel_GetModelNo(int modelId)
{
    struct DotrObject *p;
    if (modelId < 0 || modelId >= 84) {
        return -1;
    }
    p = gSzModel_ahModel[modelId];
    if (p == 0) {
        return -1;
    }
    return p->modelNo;
}
