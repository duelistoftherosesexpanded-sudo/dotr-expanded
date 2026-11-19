#include "dotr/ee/small_misc.h"

void Taku_Vector_Set(struct DotrMiscVector3 *a, struct DotrMiscVector3 *b)
{
    a->x=b->x;
    a->y=b->y;
    a->z=b->z;
}
