#include "dotr/ee/object_table.h"

int SzModel_SetAnimDir(int modelId, int animDir)
{
    struct DotrObject *pModel;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return 0;
    }
    pModel->animDir.unknown_bits_6_7 = animDir;
    return 1;
}
