#include "dotr/ee/obj_vectors.h"

int GT4_IN_RGBA(struct DotrObjFloatPair *p, unsigned char *rgba, int mode)
{
    if (mode == 0) {
        OBJ_GsRGB_RGB_IN2_inline(&p[2], rgba);
        OBJ_GsRGB_RGB_IN2_inline(&p[5], rgba);
        OBJ_GsRGB_RGB_IN2_inline(&p[8], rgba);
        OBJ_GsRGB_RGB_IN2_inline(&p[11], rgba);
    } else if (mode == 1) {
        OBJ_GsRGB_RGB_IN2_inline(&p[2], rgba);
        OBJ_GsRGB_RGB_IN2_inline(&p[5], rgba);
    } else if (mode == 2) {
        OBJ_GsRGB_RGB_IN2_inline(&p[8], rgba);
        OBJ_GsRGB_RGB_IN2_inline(&p[11], rgba);
    }
    return 0;
}
