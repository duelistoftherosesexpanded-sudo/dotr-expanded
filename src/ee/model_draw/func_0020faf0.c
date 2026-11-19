#include "dotr/ee/model_draw.h"

void lRhModel_DrawCommonPart(struct DotrModelDraw *draw)
{
    if (DOTR_MODEL_PAD->skip + DOTR_MODEL_PAD->head->unknown_58 * 256 > 16144) {
        DOTR_MODEL_PAD->unknown_b4 = DOTR_MODEL_PAD->first;
    } else {
        DOTR_MODEL_PAD->unknown_b4 = DOTR_MODEL_PAD->buffer;
    }
    *(DotrModelQuad *)DOTR_MODEL_PAD->first = *(DotrModelQuad *)((unsigned char *)draw->block + DOTR_MODEL_PAD->head->first_offset);
    RhMem_CopyMemory16(DOTR_MODEL_PAD->first + DOTR_MODEL_PAD->skip, (unsigned char *)draw->block + DOTR_MODEL_PAD->head->first_offset + DOTR_MODEL_PAD->skip,
                       (DOTR_MODEL_PAD->head->first_size - DOTR_MODEL_PAD->skip) >> 4);
    lRhModel_DynamicAddressBind(draw->block, DOTR_MODEL_PAD->first, DOTR_MODEL_PAD->first, 0);
    DOTR_MODEL_PAD->second = (DotrModelAddress *)(DOTR_MODEL_PAD->first + DOTR_MODEL_PAD->head->first_size);
    if (DOTR_MODEL_PAD->head->second_size > 0) {
        if (draw->zbuffer) {
            DOTR_MODEL_PAD->third = (unsigned char *)RhBase_SetZBufferMode((DotrModelAddress)DOTR_MODEL_PAD->second, 0);
        } else {
            DOTR_MODEL_PAD->second[0] = 0x20000000;
            DOTR_MODEL_PAD->second[1] = (DotrModelAddress)DOTR_MODEL_PAD->second + DOTR_MODEL_PAD->head->second_size + 16;
            DOTR_MODEL_PAD->second[2] = 0;
            DOTR_MODEL_PAD->second[3] = 0;
            DOTR_MODEL_PAD->third = (unsigned char *)(DOTR_MODEL_PAD->second + 4);
        }
        RhMem_CopyMemory16(DOTR_MODEL_PAD->third, (unsigned char *)draw->block + DOTR_MODEL_PAD->head->second_offset, DOTR_MODEL_PAD->head->second_size >> 4);
        lRhModel_DynamicAddressBind(draw->block, DOTR_MODEL_PAD->first, DOTR_MODEL_PAD->third, 1);
        DOTR_MODEL_PAD->fourth = DOTR_MODEL_PAD->third + DOTR_MODEL_PAD->head->second_size;
        if (draw->zbuffer) {
            DOTR_MODEL_PAD->fourth = (unsigned char *)RhBase_SetZBufferMode((DotrModelAddress)DOTR_MODEL_PAD->fourth, 1);
        }
    } else {
        DOTR_MODEL_PAD->third = 0;
        DOTR_MODEL_PAD->fourth = 0;
    }
    lRhModel_SetupTexture(draw->block, DOTR_MODEL_PAD->first, DOTR_MODEL_PAD->third, draw->texture ? draw->unknown_24 : -1);
    lRhModel_SetupOptionObject(draw->block, DOTR_MODEL_PAD->first, DOTR_MODEL_PAD->third, !draw->option, draw->unknown_20);
    if (draw->intensity || draw->light_0 || draw->light_1) {
        lRhModel_SetMaterialIntensity(DOTR_MODEL_PAD->first, DOTR_MODEL_PAD->first + DOTR_MODEL_PAD->head->first_size, DOTR_MODEL_PAD->third,
                                      DOTR_MODEL_PAD->third + DOTR_MODEL_PAD->head->second_size, draw);
    }
    DOTR_MODEL_PAD->unknown_b8 = draw->unknown_28;
    if (!draw->motion) {
        lRhModel_SetupNoMotion(draw->block);
    } else {
        DOTR_MODEL_PAD->unknown_b0 = 0;
        DOTR_MODEL_PAD->unknown_bc = DOTR_MODEL_PAD->head->unknown_54;
        if (draw->multi) {
            lRhModel_SetupMultiMotion(draw->block, draw->unknown_08 + 12, draw->unknown_10 + 12, draw->unknown_0c,
                                      draw->unknown_14, draw->unknown_18, draw->unknown_1c);
        } else if (draw->floating) {
            lRhModel_SetupFloatMotion(draw->block, draw->unknown_08 + 12, draw->unknown_0c);
        } else {
            lRhModel_SetupPulseMotion(draw->block, draw->unknown_08 + 12, draw->unknown_0c);
        }
    }
}
