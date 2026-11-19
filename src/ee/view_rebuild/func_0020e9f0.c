#include "dotr/ee/view_rebuild.h"

int lRhView_ConstructMatrix(void)
{
    struct DotrViewEntry *e = &gRhView_avl[gRhView_nStackIndex[0]];
    struct DotrViewFrame *frame = (struct DotrViewFrame *)&e->unknown_0c0;
    int x, y, w, h;
    float cx, cy;

    RhBase_GetScreenCenter(&x, &y);
    cx = (float)x + e->unknown_150 - e->unknown_164 / 2.0f;
    cy = (float)y + e->unknown_154 - e->unknown_168 / 2.0f;
    RhBase_GetZMinMax(&w, &h);
    RhGeom_CreateCameraMatrix2(&e->unknown_040, &frame->unknown_40[0], &frame->unknown_40[4], *(int *)&frame->unknown_40[16]);
    RhGeom_GetScreenMatrix(&e->unknown_080, e->unknown_160, e->unknown_158, e->unknown_15c * RhBase_GetAspectY(0), cx, cy,
                  (float)w, (float)h, e->unknown_16c, 2.0f * e->unknown_170);
    RhMem_CopyMemory16(&e->unknown_000, &e->unknown_080, 4);
    RhGeom_MulMatrix(&e->unknown_000, &e->unknown_040);
    RhMem_CopyMemory16(frame, &e->unknown_000, 4);
    return 1;
}
