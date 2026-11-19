#include "dotr/ee/obj_vectors.h"

void POS_POS(struct DotrObjVector *target, struct DotrObjVector *delta)
{
    target->x += delta->x;
    target->y += delta->y;
    target->z += delta->z;
}
