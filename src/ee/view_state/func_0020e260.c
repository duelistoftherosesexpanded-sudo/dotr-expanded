#include "dotr/ee/view_state.h"

int RhView_Set(struct DotrViewBlock *block)
{
    struct DotrViewEntry *e = &gRhView_avl[gRhView_nStackIndex[0]];
    RhMem_CopyMemory16(&e->unknown_0c0, block, 9);
    lRhView_ConstructMatrix();
    RhGeom_ResetMatrixStack();
    RhGeom_SetMatrix(&e->unknown_040);
    *(float *)&e->unknown_188[0] = 4186116.0f + e->unknown_160 * e->unknown_160;
    *(float *)&e->unknown_188[4] = e->unknown_164 * e->unknown_164 / 4.0f + e->unknown_160 * e->unknown_160;
    *(float *)&e->unknown_188[8] = e->unknown_168 * e->unknown_168 / 4.0f + e->unknown_160 * e->unknown_160;
    RhGeom_GetMatrix(block);
    RhView_GetWorldViewMatrix(&InvertView);
    RhGeom_InvertMatrix(&InvertView, &InvertView);
    return 1;
}
