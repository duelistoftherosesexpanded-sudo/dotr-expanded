#include "dotr/ee/obj_vectors.h"

int OBJECT_RGB_INOFF(unsigned char *rgb, int amount, int unused, int off)
{
    (void)unused;
    if (off == 0) {
        rgb[0] += amount;
        rgb[1] += amount;
        rgb[2] += amount;
    } else {
        rgb[0] -= amount;
        rgb[1] -= amount;
        rgb[2] -= amount;
    }
    return 0;
}
