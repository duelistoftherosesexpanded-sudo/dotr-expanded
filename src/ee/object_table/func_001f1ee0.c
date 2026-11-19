#include "dotr/ee/object_table.h"

int SzModel_SetIntensity(int modelId, struct DotrObjectVector3 *intensity)
{
    struct DotrObject *pModel;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return 0;
    }
    if (intensity) {
        pModel->intensity = *intensity;
    } else {
        pModel->intensity.x = 0.8f;
        pModel->intensity.y = 0.8f;
        pModel->intensity.z = 1.0f;
    }
    return 1;
}
