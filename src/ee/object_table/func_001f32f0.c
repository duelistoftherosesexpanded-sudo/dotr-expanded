#include "dotr/ee/object_table.h"

int SzModel_SetPauseFlag(int modelId, int value)
{
    struct DotrObject *pModel;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return 0;
    }
    pModel->stateFlags.unknown_bit_0 = value;
    return 1;
}
