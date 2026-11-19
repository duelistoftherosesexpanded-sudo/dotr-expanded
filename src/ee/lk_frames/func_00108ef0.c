#include "dotr/ee/lk_frames.h"

void S_I_P_IN(struct DotrLkMatrix *out, float *scale, int *rot, float *pos)
{
    float local[16];
    struct DotrLkMatrix world;

    RhView_GetWorldScreenMatrix(world.m);
    RhGeom_GetScaleRotXYZTransMatrix(local, scale, rot, pos);
    RhGeom_MulMatrix(world.m, local);
    *out = world;
}
