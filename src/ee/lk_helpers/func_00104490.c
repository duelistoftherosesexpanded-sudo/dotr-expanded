#include "dotr/ee/lk_helpers.h"
void LkOBJ_TransitVector(struct DotrLkVector *value, struct DotrLkVector *target, int steps)
{
    struct DotrLkVector part;

    if (steps != 0) {
        part.x = target->x;
        part.y = target->y;
        part.z = target->z;
        part.x -= value->x;
        part.y -= value->y;
        part.z -= value->z;
        part.x /= steps;
        part.y /= steps;
        part.z /= steps;
        value->x += part.x;
        value->y += part.y;
        value->z += part.z;
    } else {
        value->x = target->x;
        value->y = target->y;
        value->z = target->z;
    }
}
