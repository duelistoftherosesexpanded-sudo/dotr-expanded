#include "dotr/ee/object_table.h"

int SzModel_IsModelDisp(int modelId)
{
    struct DotrObject *model;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    model = gSzModel_ahModel[modelId];
    if (model == 0) {
        return 0;
    }
    if (model->modelId < 0) {
        return 0;
    }
    return model->drawFlag.unknown_bit_1 ? 1 : 0;
}
