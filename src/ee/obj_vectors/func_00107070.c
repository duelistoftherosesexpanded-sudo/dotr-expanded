#include "dotr/ee/obj_vectors.h"

int XYZ_POS_4(struct DotrObjVector *vectors, long axis, float delta)
{
    if (axis == 0) {
        vectors[0].x += delta;
        vectors[1].x += delta;
        vectors[2].x += delta;
        vectors[3].x += delta;
    } else if ((short)axis == 1) {
        vectors[0].y += delta;
        vectors[1].y += delta;
        vectors[2].y += delta;
        vectors[3].y += delta;
    } else {
        vectors[0].z += delta;
        vectors[1].z += delta;
        vectors[2].z += delta;
        vectors[3].z += delta;
    }
    return 0;
}
