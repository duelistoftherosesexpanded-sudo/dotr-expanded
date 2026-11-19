#include "dotr/ee/object_table.h"

int SzModel_GetScale(int modelId, struct DotrObjectVector4 *out)
{
    struct DotrObject *p;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[modelId];
    if (p == 0) {
        return 0;
    }
    if (out) {
        *out = p->scale;
    }
    return 1;
}
