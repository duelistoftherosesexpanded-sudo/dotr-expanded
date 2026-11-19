#include "dotr/ee/object_table.h"

/* Same body as SzModel_IsModelStandby. */
static inline int active(int index)
{
    struct DotrObject *p;
    if (index < 0 || index >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[index];
    if (p == 0) {
        return 0;
    }
    return p->modelId >= 0 ? 1 : 0;
}

int SzModel_GetRef(int modelId, int slot, struct DotrObjectVector4 *out)
{
    struct DotrObject *pModel;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return 0;
    }
    if (!active(modelId)) {
        return 0;
    }
    if (!pModel->drawFlag.unknown_bit_5) {
        return 0;
    }
    if (slot < 0 || slot >= 8) {
        return 0;
    }
    if (!(pModel->boneRefIds[slot] & 0x8000)) {
        return 0;
    }
    *out = pModel->boneRefWorldPos[slot];
    return 1;
}
