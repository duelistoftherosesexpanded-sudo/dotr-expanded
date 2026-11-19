#include "dotr/ee/small_misc.h"

void Ta_LkOBJ_TransitVector(struct DotrMiscVector3 *dst, struct DotrMiscVector3 *src, int n)
{
    struct DotrMiscVector3 tmp;
    if (n) {
        tmp.x = src->x;
        tmp.y = src->y;
        tmp.z = src->z;
        Ta_LkSubVector(&tmp, dst);
        tmp.x = tmp.x / (float)n;
        tmp.y = tmp.y / (float)n;
        tmp.z = tmp.z / (float)n;
        Ta_LkAddVector(dst, &tmp);
    } else {
        dst->x = src->x;
        dst->y = src->y;
        dst->z = src->z;
    }
}
