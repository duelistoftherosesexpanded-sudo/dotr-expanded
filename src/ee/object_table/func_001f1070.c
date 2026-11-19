#include "dotr/ee/object_table.h"

int SzModel_LoadOverlayB(int modelId)
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
    /* MATCHING DEVICE: the early return keeps this exit ahead of the call; a switch moves it. */
    if (pModel->type != 4 && pModel->type != 5 && pModel->type != 6) {
        return 1;
    }
    q->unknown_04 &= ~0x400;
    return func_001f3910_result(1, pModel->modelNo, 0) != 0 ? 1 : 0;
}
