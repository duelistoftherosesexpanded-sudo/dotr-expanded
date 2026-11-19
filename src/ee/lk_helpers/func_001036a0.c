#include "dotr/ee/lk_helpers.h"

void LkMulVector(struct DotrLkVector *target, struct DotrLkVector *other)
{
    target->x = target->x * other->x;
    target->y = target->y * other->y;
    target->z = target->z * other->z;
}
