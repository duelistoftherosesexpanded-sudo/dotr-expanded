#include "dotr/ee/lk_helpers.h"

int LkIncrVec(struct DotrLkVector *value, float limit, float step)
{
    int flags;

    flags = 0;
    if (value->x <= limit - step) {
        value->x += step;
        flags |= 1;
    } else {
        value->x = limit;
        flags &= ~1;
    }
    if (value->y <= limit - step) {
        value->y += step;
        flags |= 2;
    } else {
        value->y = limit;
        flags &= ~2;
    }
    if (value->z <= limit - step) {
        value->z += step;
        flags |= 4;
    } else {
        value->z = limit;
        flags &= ~4;
    }
    return flags;
}
