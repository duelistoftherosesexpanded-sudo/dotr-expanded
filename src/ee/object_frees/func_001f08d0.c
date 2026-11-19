#include "dotr/ee/object_frees.h"

/* MATCHING DEVICE: the closing loop is written as an inlined helper with its
 * own locals; whether the original had one is not known. */
static inline void func_001f08d0_unlink(int index)
{
    int i;
    int v;
    for (i = 1; i < 32; i++) {
        v = data_0031ad00_free[i].unknown_68_bits_0_26;
        if (v >= 0) {
            if (index < 0 || v == index) {
                data_0031ad00_free[i].unknown_68_bits_0_26 = -1;
            }
        }
    }
}

int lSzModel_FreeModel(int modelId)
{
    struct DotrFreeObject *p;
    struct DotrFreeEntry *e;
    struct DotrFreeList *list;
    int i;
    int n;
    int j;

    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    p = gSzModel_ahModel_free[modelId];
    if (p == 0) {
        return 0;
    }
    if (p->modelBaseData == 0) {
        return 1;
    }
    if (p->drawFlag.unknown_bit_3) {
        return 0;
    }
    if (p->modelBaseData->unknown_04 != 0) {
        for (i = 0; i < 8; i++) {
            p->AnimationDataPointers[i] = 0;
        }
        p->mainEffectTexture = 0;
        for (i = 0; i < 8; i++) {
            p->effectTextureB[i] = 0;
        }
    } else {
        if (p->unknown_13c->unknown_2c != 0) {
            list = (struct DotrFreeList *)((char *)p->modelBaseData + p->unknown_13c->unknown_28);
            n = list->count;
            for (j = 0, e = list->entries; j < n; j++, e++) {
                if (e->unknown_0 != 0) {
                    SzMem_Free(e->unknown_0);
                }
                if (e->unknown_8 != 0) {
                    SzMem_Free(e->unknown_8);
                }
            }
        }
        for (i = 0; i < 8; i++) {
            lSzModel_FreeMotion(modelId, i);
        }
        if (p->mainEffectTexture != 0) {
            SzMem_Free(p->mainEffectTexture);
            p->mainEffectTexture = 0;
        }
        for (i = 0; i < 8; i++) {
            if (p->effectTextureB[i] != 0) {
                SzMem_Free(p->effectTextureB[i]);
                p->effectTextureB[i] = 0;
            }
        }
    }
    p->OverlayM = 0;
    p->OverlayB = 0;
    SzView_FreeRequest(0, 1);
    if (p->modelBaseData->unknown_04 & 0x80000) {
        p->pCameraMotion = 0;
    } else if (p->pCameraMotion != 0) {
        SzMem_Free(p->pCameraMotion);
        p->pCameraMotion = 0;
    }
    if (p->modelBaseData->unknown_04 & 0x100000) {
        p->pCameraMotion2 = 0;
    } else if (p->pCameraMotion2 != 0) {
        SzMem_Free(p->pCameraMotion2);
        p->pCameraMotion2 = 0;
    }
    if (p->modelBaseData->unknown_04 & 0x200000) {
        p->ptrBlock = 0;
    } else if (p->ptrBlock != 0) {
        SzMem_Free(p->ptrBlock);
        p->ptrBlock = 0;
    }
    if (p->effectCallback >= 0) {
        SzEffect_Destroy(p->effectCallback);
    }
    if (p->drawFlag.unknown_bit_0) {
        SzMem_Free(p->modelBaseData);
    }
    p->modelBaseData = 0;
    func_001f08d0_unlink(modelId);
    return 1;
}
