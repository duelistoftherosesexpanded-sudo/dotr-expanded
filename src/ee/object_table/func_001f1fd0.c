#include "dotr/ee/object_table.h"

int SzModel_GetPos(int modelId, struct DotrObjectVector4 *xyzCoords)
{
    struct DotrObject *pModel;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return 0;
    }
    if (xyzCoords) {
        *xyzCoords = pModel->pos;
    }
    return 1;
}
