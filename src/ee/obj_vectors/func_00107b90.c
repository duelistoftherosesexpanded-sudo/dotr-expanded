#include "dotr/ee/obj_vectors.h"

void CARD_X_Z_IN_(float *dx, float *dz, struct DotrObjVector *pair, struct DotrObjVector *point)
{
    *dx = point->x - (pair[0].x + pair[1].x);
    *dz = point->z - (pair[0].z + pair[1].z);
    *dx = func_001a6c60(*dx * *dx);
    *dz = func_001a6c60(*dz * *dz);
}
