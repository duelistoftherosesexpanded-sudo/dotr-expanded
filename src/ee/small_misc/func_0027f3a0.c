#include "dotr/ee/small_misc.h"

void Ta_LkSubVector(struct DotrMiscVector3 *a, struct DotrMiscVector3 *b)
{
    a->x -= b->x;
    a->y -= b->y;
    a->z -= b->z;
}
