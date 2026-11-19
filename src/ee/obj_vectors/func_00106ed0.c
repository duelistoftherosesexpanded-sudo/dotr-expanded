#include "dotr/ee/obj_vectors.h"

void OBJ_VE_SCL_KAKE_4(struct DotrObjVector *vectors, struct DotrObjVector *scale)
{
    vectors[0].x = vectors[0].x * scale->x;
    vectors[0].y = vectors[0].y * scale->y;
    vectors[0].z = vectors[0].z * scale->z;
    vectors[1].x = vectors[1].x * scale->x;
    vectors[1].y = vectors[1].y * scale->y;
    vectors[1].z = vectors[1].z * scale->z;
    vectors[2].x = vectors[2].x * scale->x;
    vectors[2].y = vectors[2].y * scale->y;
    vectors[2].z = vectors[2].z * scale->z;
    vectors[3].x = vectors[3].x * scale->x;
    vectors[3].y = vectors[3].y * scale->y;
    vectors[3].z = vectors[3].z * scale->z;
}
