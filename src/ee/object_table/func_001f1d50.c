#include "dotr/ee/object_table.h"

int SzModel_GetAnimFrameMax(int modelId, int animIndex)
{
    struct DotrObject *p;
    if (modelId < 0 || modelId >= 84) {
        return -1;
    }
    if (animIndex < 0) {
        animIndex = SzModel_GetAnimParam(modelId, 0);
    }
    if (animIndex < 0 || animIndex >= 8) {
        return -1;
    }
    p = gSzModel_ahModel[modelId];
    if (p == 0) {
        return -1;
    }
    if (p->animationDataPointers[animIndex] == 0) {
        return -1;
    }
    return p->animationFrameCountArray[animIndex] - 1;
}
