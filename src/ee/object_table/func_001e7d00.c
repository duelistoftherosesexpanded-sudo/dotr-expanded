#include "dotr/ee/object_table.h"

int lSzModel_Clean(int modelId)
{
    struct DotrObject *pModel;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return 0;
    }
    pModel->unknown_198[0].x = 1.0f;
    pModel->unknown_198[0].y = 1.0f;
    pModel->unknown_198[0].z = 1.0f;
    pModel->unknown_198[1].x = 1.0f;
    pModel->unknown_198[1].y = 1.0f;
    pModel->unknown_198[1].z = 0.0f;
    pModel->unknown_1e0[1] = -1;
    pModel->unknown_1e0[0] = 0;
    pModel->stateFlags.unknown_bit_1 = 0;
    pModel->stoneRatio = 0;
    pModel->maxOT = 0;
    pModel->blendTickTotal = 6;
    pModel->blendTickCurrent = 0;
    pModel->motionDelay = 0;
    pModel->drawFlag.unknown_bit_5 = 0;
    SzModel_SetRefID(modelId, -1, 1);
    pModel->coordIndex = 32767;
    return 1;
}
