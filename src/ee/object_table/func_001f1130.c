#include "dotr/ee/object_table.h"

int SzModel_GetOverlayB(int modelId)
{
    struct DotrObject *pModel;
    struct DotrObjectLink *q;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return 0;
    }
    q = pModel->modelBaseData;
    if (q == 0) {
        return 0;
    }
    if ((q->unknown_04 & 0x400) == 0) {
        return 0;
    }
    return pModel->OverlayB;
}
