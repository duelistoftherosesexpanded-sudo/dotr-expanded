#include "dotr/ee/obj_vectors.h"

int V0IN_V1_V2_XZ(struct DotrObjVector *pair, struct DotrObjVector *to, struct DotrObjVector *from)
{
    pair[0].x += to->x - from->x;
    pair[0].z += to->z - from->z;
    pair[1].x += to->x - from->x;
    pair[1].z += to->z - from->z;
    return 0;
}
