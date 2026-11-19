#include "dotr/ee/lk_frames.h"

int LkRotSF(struct DotrLkFrame *frame)
{
    int rot[4];
    float local[16];
    int *yaw;

    Ex3d_GetLookVector(rot);
    yaw = &rot[1];
    frame->rot[1] = *yaw % 65536;
    frame->rot[0] = rot[0];
    frame->rot[2] = rot[2];
    RhView_GetWorldScreenMatrix(frame->matrix);
    RhGeom_GetScaleRotXYZTransMatrix(local, frame->scale, frame->rot, frame->pos);
    RhGeom_MulMatrix(frame->matrix, local);
    return *yaw;
}
