#include "dotr/ee/obj_vectors.h"

int PSYS_RR2H_IN(void *system, float x, float y, float z)
{
    struct DotrObjVector *target = (struct DotrObjVector *)((char *)system + 0x449c);

    target->x = x;
    target->y = y;
    target->z = z;
    return 0;
}
