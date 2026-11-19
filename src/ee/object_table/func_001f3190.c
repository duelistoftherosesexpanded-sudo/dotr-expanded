#include "dotr/ee/object_table.h"

int SzModel_SetCalcFlag(int modelId, int flagValue)
{
    struct DotrObject *pModel;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return 0;
    }
    pModel->drawFlag.unknown_bit_6 = flagValue;
    return 1;
}
