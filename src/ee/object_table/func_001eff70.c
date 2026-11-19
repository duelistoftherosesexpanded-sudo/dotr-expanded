#include "dotr/ee/object_table.h"
#include "dotr/ee/resource_submit.h"

extern void lSzModel_LoadModelCB(struct DotrResourceSubmitRecord *record, int completed);

int lSzModel_LoadModel(int modelId, char *filePath)
{
    struct DotrObject *pModel;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return 0;
    }
    if (pModel->drawFlag.unknown_bit_3) {
        return 0;
    }
    lSzModel_FreeModel(modelId);
    pModel->drawFlag.unknown_bit_0 = -1;
    if (SzFile_ReadRequest(modelId, filePath, 0, -1, lSzModel_LoadModelCB, modelId)) {
        pModel->drawFlag.unknown_bit_3 = -1;
    }
    return 1;
}
