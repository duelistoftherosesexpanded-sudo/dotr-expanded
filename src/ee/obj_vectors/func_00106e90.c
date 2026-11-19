#include "dotr/ee/obj_vectors.h"

void OBJ_VE_SCL_KAKE(struct DotrObjVector *target, struct DotrObjVector *scale)
{
    target->x *= scale->x;
    target->y *= scale->y;
    target->z *= scale->z;
}
