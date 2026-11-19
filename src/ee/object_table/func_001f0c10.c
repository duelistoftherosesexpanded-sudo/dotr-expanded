#include "dotr/ee/object_table.h"

int lSzModel_FreeMotion(int modelId, int slot)
{
    struct DotrObject *p;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    if (slot < 0 || slot >= 8) {
        return 0;
    }
    p = gSzModel_ahModel[modelId];
    if (p == 0) {
        return 0;
    }
    if (p->animationDataPointers[slot] == 0) {
        return 1;
    }
    SzMem_Free((void *)(DotrObjectWord)p->animationDataPointers[slot]);
    p->animationDataPointers[slot] = 0;
    p->animationFrameCountArray[slot] = 1;
    return 1;
}
