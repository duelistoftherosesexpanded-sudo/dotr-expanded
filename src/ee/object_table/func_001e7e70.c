#include "dotr/ee/object_table.h"
#include "dotr/ee/resource_helpers.h"

int SzModel_Preset(int modelId, int modelType, int modelNumber)
{
    struct DotrObject *model;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    model = gSzModel_ahModel[modelId];
    if (model == 0) {
        return 0;
    }
    if (model->drawFlag.unknown_bit_3) {
        while (model->drawFlag.unknown_bit_3) {
            SzFile_DoRequestS();
        }
    }
    lSzModel_FreeModel(modelId);
    model->modelId = -1;
    if (modelType >= 0) {
        model->type = modelType;
    }
    model->modelNo = modelNumber;
    model->drawFlag.unknown_bit_2 = -1;
    return 1;
}
