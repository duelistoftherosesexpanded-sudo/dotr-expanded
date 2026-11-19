#include "dotr/ee/model_methods.h"

/* 0x20f780. Scratchpad literals go through DOTR_METHODS_SPR. */

void *lRhModel_DrawNormalMethod(void *modelBaseAddr, int arg1) {
    struct DotrMethodsNormalFlags *obj = modelBaseAddr;
    int flags;
    int result;
    unsigned int flag5;
    unsigned int flag4;

    flags = 1;
    flag5 = obj->flag5_2 ? 0x40 : 0;
    flag4 = (obj->flag4_1 != 0) ? 0x10 : 0;
    flags |= flag4 | flag5;
    flags |= arg1 ? 2 : 0;
    RhGeomEx_SetMatrixForPointLight();
    if (!obj->flag4_2) {
        if (*(int *)DOTR_METHODS_SPR(0xB0) & 4) {
            flags |= 0x20;
        }
        *(void **)DOTR_METHODS_SPR(0xC4) = RhVu1Mac_ChangeDrawMethod(*(void **)DOTR_METHODS_SPR(0xC0), flags);
    } else {
        *(void **)DOTR_METHODS_SPR(0xC4) = RhVu1Mac_ReserveChangeDrawMethod(*(void **)DOTR_METHODS_SPR(0xC0));
    }
    lRhModel_DrawCommonPart(obj);
    if (!(*(int *)DOTR_METHODS_SPR(0xB0) & 1)) {
        return *(void **)DOTR_METHODS_SPR(0xC0);
    }
    if (*(int *)(*(unsigned char **)DOTR_METHODS_SPR(0xDC) + 0x1C) > 0) {
        if (obj->flag5_3) {
            result = *(int *)DOTR_METHODS_SPR(0xD0);
        } else {
            RhMem_CopyMemory16(gRhModel_mlsmSwapArea, (void *)DOTR_METHODS_SPR(0), 0x11);
            result = RhBase_SetZBufferMode((DotrMethodsWord)RhModelCpu_DrawAlpha(
                (unsigned char *)RhBase_SetZBufferMode(*(int *)DOTR_METHODS_SPR(0xD0), 0),
                *(int *)DOTR_METHODS_SPR(0xCC),
                *(int *)(*(unsigned char **)DOTR_METHODS_SPR(0xDC) + 0x1C),
                obj->flag4_1), 1);
            RhMem_CopyMemory16((void *)DOTR_METHODS_SPR(0), gRhModel_mlsmSwapArea, 0x11);
        }
        *(int *)DOTR_METHODS_SPR(0xD4) = result;
    } else {
        *(int *)DOTR_METHODS_SPR(0xD4) = *(int *)DOTR_METHODS_SPR(0xC8);
    }
    if (*(int *)DOTR_METHODS_SPR(0xB0) & 4) {
        flags |= 0x20;
    } else if (arg1 == 0) {
        RhView_GetViewScreenMatrix((void *)DOTR_METHODS_SPR(0));
        lRhModel_ChangeL2VtoL2S(*(int *)DOTR_METHODS_SPR(0xB4) + 0x10, DOTR_METHODS_SPR(0),
                     ((*(int *)DOTR_METHODS_SPR(0xD8) - 0x10) >> 4) / 7);
    }
    if (*(void **)DOTR_METHODS_SPR(0xB4) == (void *)DOTR_METHODS_SPR(0xF0)) {
        RhMem_CopyMemory16(*(unsigned char **)DOTR_METHODS_SPR(0xC4) + 0x10, (void *)DOTR_METHODS_SPR(0x100),
                     (*(int *)DOTR_METHODS_SPR(0xD8) - 0x10) >> 4);
    }
    if (obj->flag4_2) {
        RhVu1Mac_ChangeDrawMethod(*(void **)DOTR_METHODS_SPR(0xC0), flags);
    }
    return *(void **)DOTR_METHODS_SPR(0xD4);
}
