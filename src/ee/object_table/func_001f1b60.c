#include "dotr/ee/object_table.h"

float SzModel_GetAnimFrameParam(int modelId, int animIndex)
{
    struct DotrObject *pModel;
    int v;
    if (modelId < 0 || modelId >= 84) {
        return -1.0f;
    }
    if (animIndex < 0 || animIndex >= 4) {
        return -1.0f;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return -1.0f;
    }
    v = pModel->unknown_1e0[animIndex];
    if (v < 0) {
        return -1.0f;
    }
    if (pModel->animationDataPointers[v] == 0) {
        return -1.0f;
    }
    return pModel->unknown_198[animIndex].y;
}
