#include "dotr/ee/obj_vectors.h"

int XC_ZS_IN(struct DotrObjVector *target, int unit, int radius, int angle)
{
    target->x = (float)radius * SIN_COS_INRAD(angle, unit, 1);
    target->z = (float)radius * SIN_COS_INRAD(angle, unit, 0);
    return 0;
}
