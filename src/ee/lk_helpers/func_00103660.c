#include "dotr/ee/lk_helpers.h"

void LkSubVector(struct DotrLkVector *target, struct DotrLkVector *other)
{
    target->x -= other->x;
    target->y -= other->y;
    target->z -= other->z;
}
