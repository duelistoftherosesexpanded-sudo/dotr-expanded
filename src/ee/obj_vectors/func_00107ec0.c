#include "dotr/ee/obj_vectors.h"

int OBJ_GsRGB_RGB_IN_DATA(unsigned char *rgb, int unused, short kind)
{
    (void)unused;
    if (kind == 0) {
        rgb[0] = 255;
        rgb[1] = 255;
        rgb[2] = 255;
    } else if (kind == 1) {
        rgb[0] = 255;
        rgb[1] = 0;
        rgb[2] = 0;
    } else if (kind == 2) {
        rgb[0] = 0;
        rgb[1] = 255;
        rgb[2] = 0;
    } else if (kind == 3) {
        rgb[0] = 0;
        rgb[1] = 0;
        rgb[2] = 255;
    } else if (kind == 4) {
        rgb[0] = 255;
        rgb[1] = 255;
        rgb[2] = 0;
    } else if (kind == 5) {
        rgb[0] = 255;
        rgb[1] = 0;
        rgb[2] = 255;
    } else if (kind == 6) {
        rgb[0] = 0;
        rgb[1] = 255;
        rgb[2] = 255;
    } else if (kind == 7) {
        rgb[0] = 255;
        rgb[1] = 128;
        rgb[2] = 255;
    } else if (kind == 8) {
        rgb[0] = 128;
        rgb[1] = 0;
        rgb[2] = 255;
    } else if (kind == 9) {
        rgb[0] = 255;
        rgb[1] = 128;
        rgb[2] = 0;
    } else {
        rgb[0] = 255;
        rgb[1] = 255;
        rgb[2] = 255;
    }
    return 0;
}
