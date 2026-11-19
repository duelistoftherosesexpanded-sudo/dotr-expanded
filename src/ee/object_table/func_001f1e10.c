#include "dotr/ee/object_table.h"

int SzModel_GetIntensity(int modelId, struct DotrObjectVector3 *intensity)
{
    struct DotrObject *pModel;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return 0;
    }
    *intensity = pModel->intensity;
    return 1;
}
