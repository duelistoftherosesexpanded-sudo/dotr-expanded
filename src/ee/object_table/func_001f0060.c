#include "dotr/ee/object_table.h"
#include "dotr/ee/resource_submit.h"
extern void lSzModel_LoadModelPackCB(struct DotrResourceSubmitRecord *record, int completed);
int lSzModel_LoadModelPack(int modelId, char *filePath, int unknown_014, int byte_size, int byte_offset, int unknown_010)
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
    pModel->drawFlag.unknown_bit_0 = unknown_014 ? 0 : 1;
    if (SzFile_ReadRequestParam(modelId, 0, filePath, unknown_014, byte_size, byte_offset, unknown_010, lSzModel_LoadModelPackCB, modelId)) {
        pModel->drawFlag.unknown_bit_3 = -1;
    }
    return 1;
}
