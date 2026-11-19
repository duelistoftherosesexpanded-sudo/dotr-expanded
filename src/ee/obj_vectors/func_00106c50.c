#include "dotr/ee/obj_vectors.h"

int OBJECT_RGBA_INOFF(unsigned char *rgba, int amount, unsigned char limit, int off)
{
    if (off == 0) {
        if (rgba[3] < limit) {
            rgba[3] += amount;
        } else {
            return 1;
        }
    } else {
        if (rgba[3] > limit) {
            rgba[3] -= amount;
        } else {
            return 1;
        }
    }
    return 0;
}
