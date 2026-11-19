#include "dotr/ee/model_methods.h"

/* 0x20fa10. Scratchpad literals go through DOTR_METHODS_SPR. */

void *lRhModel_DrawFastMethod(void *baseFileHeader) {
    unsigned int mode = 1;
    unsigned int flag5 = ((struct DotrMethodsFastFlags *)baseFileHeader)->flag5 ? 0x40 : 0;
    unsigned int flag4 = (((struct DotrMethodsFastFlags *)baseFileHeader)->flag4 != 0) ? 0x10 : 0;
    mode |= flag4 | flag5;
    *(void **)DOTR_METHODS_SPR(0xC4) = RhVu1Mac_ChangeDrawMethod(*(void **)DOTR_METHODS_SPR(0xC0), mode);
    RhView_GetViewScreenMatrix((void *)DOTR_METHODS_SPR(0));
    RhGeomEx_SetMatrixForStraightLight((void *)DOTR_METHODS_SPR(0));
    lRhModel_DrawCommonPart(baseFileHeader);
    if (*(unsigned char **)DOTR_METHODS_SPR(0xB4) == (unsigned char *)DOTR_METHODS_SPR(0xF0)) {
        RhMem_CopyMemory16(*(unsigned char **)DOTR_METHODS_SPR(0xC4) + 0x10, (void *)DOTR_METHODS_SPR(0x100), (*(int *)DOTR_METHODS_SPR(0xD8) - 0x10) >> 4);
    }
    *(void **)DOTR_METHODS_SPR(0xD4) = *(void **)DOTR_METHODS_SPR(0xC8);
    return *(void **)DOTR_METHODS_SPR(0xD4);
}
