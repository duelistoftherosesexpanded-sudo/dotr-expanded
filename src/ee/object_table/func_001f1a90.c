#include "dotr/ee/object_table.h"

int SzModel_SetAnimSpeedParam(int modelId, int paramIndex, float speed)
{
    struct DotrObject *pModel;
    int v;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    if (paramIndex < 0 || paramIndex >= 4) {
        return 0;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return 0;
    }
    v = pModel->unknown_1e0[paramIndex];
    if (v < 0) {
        return 0;
    }
    if (pModel->animationDataPointers[v] == 0) {
        return 0;
    }
    pModel->unknown_198[paramIndex].x = speed;
    return 1;
}
