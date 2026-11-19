#include "dotr/ee/object_table.h"

int SzModel_Destroy(int modelId)
{
    struct DotrObject *model;
    struct DotrObject **slot;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    slot = &gSzModel_ahModel[modelId];
    model = *slot;
    if (model == 0) {
        return 0;
    }
    if (lSzModel_FreeModel(modelId) == 0) {
        return 0;
    }
    SzMem_Free(model);
    *slot = 0;
    return 1;
}
