#include "dotr/ee/model_draw.h"

DotrModelAddress RhModel_Draw(DotrModelAddress cursor, struct DotrModelDraw *draw)
{
    struct DotrModelNode *node;
    int lit;
    unsigned char fog;
    int on;

    fog = on = draw->bit_1;
    if (!on || !draw->bit_10) {
        cursor = RhVu1Mac_SetFogOnOff(cursor, fog, draw->bit_10);
    }
    DOTR_MODEL_PAD->unknown_c0 = (int)cursor;
    DOTR_MODEL_PAD->head = (struct DotrModelHead *)((unsigned char *)draw->block + 16);
    DOTR_MODEL_PAD->skip = ((struct DotrModelStretch *)((unsigned char *)draw->block + DOTR_MODEL_PAD->head->first_offset))->skip;
    RhGeom_PushMatrix(0);
    node = (struct DotrModelNode *)((unsigned char *)draw->block + DOTR_MODEL_PAD->head->unknown_00[0]);
    *(DotrModelQuad *)&DOTR_MODEL_PAD->centre = *(DotrModelQuad *)&node->sphere;
    DOTR_MODEL_PAD->centre.w = 1.0f;
    if (draw->own_matrix) {
        RhMem_CopyMemory16(&DOTR_MODEL_PAD->work, &draw->matrix, 1);
    } else {
        RhGeom_MulMatrixVector(&DOTR_MODEL_PAD->work, 0, &DOTR_MODEL_PAD->centre);
    }
    draw->sample_z = RhView_GetSampleZ(&DOTR_MODEL_PAD->work, node->sphere.w);
    lit = 0;
    if (draw->bit_0 && RhVu1Mac_GetPointLight(0, &DOTR_MODEL_PAD->light_at, &DOTR_MODEL_PAD->light[0], &DOTR_MODEL_PAD->light[1], &DOTR_MODEL_PAD->light[2])
        && RhGeom_GetPointsDistance(&DOTR_MODEL_PAD->light_at, &DOTR_MODEL_PAD->work) <= DOTR_MODEL_PAD->light[1] + node->sphere.w) {
        lit = 1;
    }
    require_coordinate_index = 0;
    if (draw->motion) {
        cursor = lRhModel_DrawNormalMethod(draw, lit);
    } else {
        DOTR_MODEL_PAD->unknown_b0 = RhView_CheckViewVolume(&DOTR_MODEL_PAD->centre, node->sphere.w);
        if (DOTR_MODEL_PAD->unknown_b0 & 1) {
            if ((DOTR_MODEL_PAD->unknown_b0 & 4) || DOTR_MODEL_PAD->head->second_size > 0 || lit) {
                cursor = lRhModel_DrawNormalMethod(draw, lit);
            } else {
                cursor = lRhModel_DrawFastMethod(draw);
            }
        }
    }
    RhGeom_PopMatrix();
    if (!draw->bit_1 || !draw->bit_10) {
        cursor = RhVu1Mac_SetFogOnOff(cursor, 1, 1);
    }
    return cursor;
}
