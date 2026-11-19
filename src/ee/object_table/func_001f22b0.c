#include "dotr/ee/object_table.h"

int SzModel_SetPos(int modelId, struct DotrObjectVector4 *pos)
{
    struct DotrObject *pModel;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return 0;
    }
    if (pos) {
        pModel->pos = *pos;
    }
    return 1;
}
