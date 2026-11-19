#include "dotr/ee/object_table.h"

int SzModel_GetRgba(int modelId, struct DotrObjectQuad *out)
{
    struct DotrObject *pModel;
    if (modelId < 0 || modelId >= 84) {
        return -1;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return -1;
    }
    if (out) {
        *out = pModel->rgba;
    }
    return pModel->alphaMode.unknown_bits_0_1;
}
