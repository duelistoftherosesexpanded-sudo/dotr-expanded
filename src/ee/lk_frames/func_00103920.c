#include "dotr/ee/lk_frames.h"

int LkChange2D(struct DotrLkFrame *frame, float x, float y)
{
    float point[4];
    float local[16];

    Ex3d_GetPointOnScreen(point, x, y);
    LkRotSF(frame);
    frame->pos[0] = point[0];
    frame->pos[1] = point[1];
    frame->pos[2] = point[2];
    RhView_GetWorldScreenMatrix(frame->matrix);
    RhGeom_GetScaleRotXYZTransMatrix(local, frame->scale, frame->rot, frame->pos);
    RhGeom_MulMatrix(frame->matrix, local);
    return 0;
}
