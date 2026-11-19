#include "dotr/ee/obj_vectors.h"

void OBJ_GsRGB_RGB_IN2(struct DotrObjFloatPair *out, unsigned char *rgba)
{
    struct DotrObjRgbaOne tmp;

    tmp.rgba[0] = rgba[0];
    tmp.rgba[1] = rgba[1];
    tmp.rgba[2] = rgba[2];
    tmp.rgba[3] = rgba[3];
    tmp.one = 1.0f;
    *out = *(struct DotrObjFloatPair *)&tmp;
}
