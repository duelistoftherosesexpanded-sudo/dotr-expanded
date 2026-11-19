#include "dotr/ee/lk_helpers.h"
void LkGetCenter(struct DotrLkVector *from, struct DotrLkVector *to, struct DotrLkVector *out)
{
    struct DotrLkVector half;

    half.x = to->x;
    half.y = to->y;
    half.z = to->z;
    half.x -= from->x;
    half.y -= from->y;
    half.z -= from->z;
    half.x /= 2.0f;
    half.y /= 2.0f;
    half.z /= 2.0f;
    out->x = from->x;
    out->y = from->y;
    out->z = from->z;
    out->x += half.x;
    out->y += half.y;
    out->z += half.z;
}
