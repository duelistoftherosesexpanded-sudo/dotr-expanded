#include "dotr/ee/object_table.h"

int SzModel_GetRot(int modelId, struct DotrObjectVector4 *rotationArray)
{
    struct DotrObject *p;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[modelId];
    if (p == 0) {
        return 0;
    }
    if (rotationArray) {
        *rotationArray = p->rot;
    }
    return 1;
}
