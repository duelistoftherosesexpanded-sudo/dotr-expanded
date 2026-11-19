#include "dotr/ee/obj_vectors.h"

int G4_IN_RGBA(struct DotrObjFloatPair *p, unsigned char *rgba, int mode)
{
    if (mode == 0) {
        OBJ_GsRGB_RGB_IN2_inline(&p[2], rgba);
        OBJ_GsRGB_RGB_IN2_inline(&p[4], rgba);
        OBJ_GsRGB_RGB_IN2_inline(&p[6], rgba);
        OBJ_GsRGB_RGB_IN2_inline(&p[8], rgba);
    } else if (mode == 1) {
        OBJ_GsRGB_RGB_IN2_inline(&p[2], rgba);
        OBJ_GsRGB_RGB_IN2_inline(&p[4], rgba);
    } else if (mode == 2) {
        OBJ_GsRGB_RGB_IN2_inline(&p[6], rgba);
        OBJ_GsRGB_RGB_IN2_inline(&p[8], rgba);
    }
    return 0;
}
