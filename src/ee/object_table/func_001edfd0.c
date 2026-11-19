#include "dotr/ee/object_table.h"

int SzModel_DrawMotionBlur_Pipe(int cursor, int index)
{
    /* Declaration order is the saved-register order of the original (s0..s4). */
    struct DotrObjectCopy112 *save;
    int isCached;
    int i;
    struct DotrObject *pModel;
    struct DotrObjectCopy112 *b;
    int j;

    save = &data_0031ad00_copy[0];
    isCached = 0;
    gSzModel_nBlurMode[0] = (index < 0) ? 2 : 1;
    for (i = 31, b = &data_0031ad00_copy[31]; i > 0; i--, b--) {
        int cur = b->index;

        if (cur < 0) {
            continue;
        }
        if (index >= 0 && cur != index) {
            continue;
        }
        if (!SzModel_IsModelDisp(cur)) {
            b->index = -1;
            continue;
        }
        pModel = gSzModel_ahModel[b->index];
        if (pModel == 0) {
            continue;
        }
        if (index < 0) {
            if (!b->flag) {
                continue;
            }
        } else {
            if (b->flag) {
                continue;
            }
        }
        if (!isCached) {
            SzModel_SetDrawCache(0);
            isCached = 1;
        }
        for (j = 0; j < 2; j++) {
            save->unknown_40[j] = pModel->unknown_198[j];
            save->unknown_58[j] = pModel->unknown_1e0[j];
        }
        RhMem_CopyMemory16(save, pModel, 4);
        save->unknown_5a = pModel->rgba;
        save->mode = pModel->alphaMode.unknown_bits_0_1;
        for (j = 0; j < 2; j++) {
            pModel->unknown_198[j] = b->unknown_40[j];
            pModel->unknown_1e0[j] = b->unknown_58[j];
        }
        RhMem_CopyMemory16(pModel, b, 4);
        pModel->rgba.unknown_2 = (b->unknown_5a.unknown_2 * (b->unknown_62 - b->unknown_64) + b->unknown_5e.unknown_2 * b->unknown_64) / b->unknown_62;
        pModel->rgba.unknown_1 = (b->unknown_5a.unknown_1 * (b->unknown_62 - b->unknown_64) + b->unknown_5e.unknown_1 * b->unknown_64) / b->unknown_62;
        pModel->rgba.unknown_0 = (b->unknown_5a.unknown_0 * (b->unknown_62 - b->unknown_64) + b->unknown_5e.unknown_0 * b->unknown_64) / b->unknown_62;
        pModel->rgba.unknown_3 = (b->unknown_5a.unknown_3 * (b->unknown_62 - b->unknown_64) + b->unknown_5e.unknown_3 * b->unknown_64) / b->unknown_62;
        pModel->alphaMode.unknown_bits_0_1 = b->mode;
        if (b->flag) {
            cursor = SzModel_ExSort_Pipe(cursor, b->index, 0, 0, 0, 0, 0x80fffff5);
        } else {
            cursor = SzModel_Draw_Pipe(cursor, b->index, 0, 0, 0, 0, 0x80fffff5);
        }
        for (j = 0; j < 2; j++) {
            pModel->unknown_198[j] = save->unknown_40[j];
            pModel->unknown_1e0[j] = save->unknown_58[j];
        }
        RhMem_CopyMemory16(pModel, save, 4);
        pModel->rgba = save->unknown_5a;
        pModel->alphaMode.unknown_bits_0_1 = save->mode;
        if (++b->unknown_64 >= b->unknown_62) {
            b->index = -1;
        }
    }
    if (isCached) {
        SzModel_SetDrawCache(0);
    }
    gSzModel_nBlurMode[0] = 0;
    return cursor;
}
