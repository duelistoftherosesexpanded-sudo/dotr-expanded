#include "dotr/ee/obj_vectors.h"

void POS_POS_4(struct DotrObjVector *vectors, struct DotrObjVector *delta)
{
    vectors[0].x = vectors[0].x + delta->x;
    vectors[0].y += delta->y;
    vectors[0].z += delta->z;
    vectors[1].x = vectors[1].x + delta->x;
    vectors[1].y += delta->y;
    vectors[1].z += delta->z;
    vectors[2].x = vectors[2].x + delta->x;
    vectors[2].y += delta->y;
    vectors[2].z += delta->z;
    vectors[3].x = vectors[3].x + delta->x;
    vectors[3].y += delta->y;
    vectors[3].z += delta->z;
}
