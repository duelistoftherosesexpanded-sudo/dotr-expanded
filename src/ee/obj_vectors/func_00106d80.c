#include "dotr/ee/obj_vectors.h"

int RAD_IN_360_(int amount, short *angle)
{
    *angle += amount;
    if (*angle >= 360 || *angle <= -360) {
        *angle %= 360;
    }
    return 0;
}
