#include "dotr/ee/obj_vectors.h"

int PAINT_VECTOR_4TG_SET_IN2(struct DotrObjVector *corners, float half_width, float half_depth)
{
    corners[0].x = -half_width;
    corners[0].y = 0.0f;
    corners[0].z = half_depth;
    corners[1].x = half_width;
    corners[1].y = 0.0f;
    corners[1].z = half_depth;
    corners[2].x = -half_width;
    corners[2].y = 0.0f;
    corners[2].z = -half_depth;
    corners[3].x = half_width;
    corners[3].y = 0.0f;
    corners[3].z = -half_depth;
    return 0;
}
