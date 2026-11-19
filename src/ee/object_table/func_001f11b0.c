#include "dotr/ee/object_table.h"

/* MATCHING DEVICE: the case order. */
int SzModel_GetCamera(int modelId, int kind)
{
    struct DotrObject *pModel;
    int r = 0;
    if (modelId < 0 || modelId >= 84) {
        return r;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return r;
    }
    if (pModel->modelBaseData == 0) {
        return r;
    }
    switch (kind) {
    case 19:
        r = pModel->pCameraMotion;
        break;
    case 20:
        r = pModel->pCameraMotion2;
        break;
    }
    return r;
}
