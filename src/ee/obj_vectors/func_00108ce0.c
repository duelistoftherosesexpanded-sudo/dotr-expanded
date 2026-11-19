#include "dotr/ee/obj_vectors.h"

int PAINT_VECTOR_4TG_SET_IN(struct DotrObjVector *corners, float half_width, float half_height)
{
    corners[0].x = -half_width;
    corners[0].y = half_height;
    corners[0].z = 0.0f;
    corners[1].x = half_width;
    corners[1].y = half_height;
    corners[1].z = 0.0f;
    corners[2].x = -half_width;
    corners[2].y = -half_height;
    corners[2].z = 0.0f;
    corners[3].x = half_width;
    corners[3].y = -half_height;
    corners[3].z = 0.0f;
    return 0;
}
