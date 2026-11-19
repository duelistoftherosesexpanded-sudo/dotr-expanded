#include "dotr/ee/obj_vectors.h"

int POINT_IN_VECTOR_(struct DotrObjVector *point, struct DotrObjVector *vector, int reverse)
{
    if (reverse == 0) {
        point->x = vector->x;
        point->y = vector->y;
        point->z = vector->z;
    } else {
        vector->x = point->x;
        vector->y = point->y;
        vector->z = point->z;
    }
    return 0;
}
