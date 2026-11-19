#include "dotr/ee/object_table.h"

int lSzModel_BindModelByAddr(int modelId, struct DotrBindHeader *addr)
{
    struct DotrBindObject *pModel;
    char *currentSectionPtr;
    int i;
    pModel = (struct DotrBindObject *)gSzModel_ahModel[modelId];
    if (pModel == 0) return 0;
    pModel->base = addr;
    pModel->desc = (char *)pModel->base + 16;
    RhModel_BindAddress(pModel->base);
    SzModel_BindTextureFile(modelId);
    currentSectionPtr = (char *)pModel->base + pModel->base->motion_offset;
    for (i = 0; i < 8; i++) {
        if (pModel->base->flags & (1 << i)) {
            pModel->heads[i] = (struct DotrBindMotion *)currentSectionPtr;
            NjaReloc_Adjust_M3DMOT((struct DotrSlotHead *)pModel->heads[i]);
            pModel->values[i] = pModel->heads[i]->first[1];
            currentSectionPtr += ((int *)currentSectionPtr)[1];
        } else {
            pModel->values[i] = 1;
            pModel->heads[i] = 0;
        }
    }
    if (pModel->base->flags & 0x100) {
        pModel->block_164 = currentSectionPtr;
        currentSectionPtr = (char *)((struct DotrBindPair *)currentSectionPtr + 0x1000);
    } else {
        pModel->block_164 = 0;
    }
    if (pModel->base->flags & 0x200) {
        RhText_BindAddress(currentSectionPtr);
        pModel->text = (struct DotrBindSized *)currentSectionPtr;
        currentSectionPtr += ((struct DotrBindSized *)currentSectionPtr)->size;
    } else {
        pModel->text = 0;
    }
    if (pModel->base->flags & 0x400) {
        pModel->block_168 = currentSectionPtr;
        currentSectionPtr = (char *)((struct DotrBindPair *)currentSectionPtr + 0x1000);
    } else {
        pModel->block_168 = 0;
    }
    for (i = 0; i < 8; i++) {
        if (pModel->base->flags & (1 << (i + 11))) {
            RhText_BindAddress(currentSectionPtr);
            pModel->texts[i] = (struct DotrBindSized *)currentSectionPtr;
            currentSectionPtr += ((struct DotrBindSized *)currentSectionPtr)->size;
        } else {
            pModel->texts[i] = 0;
        }
    }
    if (pModel->base->flags & 0x80000) {
        int size = (((struct DotrBindSized *)currentSectionPtr)->size + 15) & ~15;
        pModel->sized_16c = (struct DotrBindSized *)currentSectionPtr;
        currentSectionPtr += size;
    }
    if (pModel->base->flags & 0x100000) {
        int size = (((struct DotrBindSized *)currentSectionPtr)->size + 15) & ~15;
        pModel->sized_170 = (struct DotrBindSized *)currentSectionPtr;
        currentSectionPtr += size;
    }
    if (pModel->base->flags & 0x200000) {
        pModel->block_174 = currentSectionPtr;
    } else {
        pModel->block_174 = 0;
    }
    lSzModel_LoadCB(modelId);
    return 1;
}
