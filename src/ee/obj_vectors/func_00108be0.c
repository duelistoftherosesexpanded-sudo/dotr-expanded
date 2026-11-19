#include "dotr/ee/obj_vectors.h"

void RAD_XYZ_IN(struct DotrObjVector *out, struct DotrObjVector *in, int axis, float s, float c)
{
    struct DotrObjVector a;
    struct DotrObjVector b;

    a = *in;
    if (axis == 0) {
        b.x = s * a.x + -c * a.z;
        b.z = c * a.x + s * a.z;
        b.y = a.y;
    }
    if ((short)axis == 1) {
        b.x = a.x;
        b.y = c * a.y + s * a.z;
        b.z = -s * a.y + c * a.z;
    }
    if ((short)axis == 2) {
        b.x = s * a.x + c * a.y;
        b.y = -c * a.x + s * a.y;
        b.z = a.z;
    }
    *out = b;
}
