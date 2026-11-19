#include "dotr/ee/object_table.h"

float SzModel_GetRadius(int modelId)
{
    struct DotrObject *p;
    if (modelId < 0 || modelId >= 84) {
        return -1.0f;
    }
    p = gSzModel_ahModel[modelId];
    if (p == 0) {
        return -1.0f;
    }
    return p->radius;
}
