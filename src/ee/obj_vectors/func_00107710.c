#include "dotr/ee/obj_vectors.h"

int OBJ_GsRGB_RGB_IN(unsigned char *target, unsigned char *source)
{
    target[0] = source[0];
    target[1] = source[1];
    target[2] = source[2];
    target[3] = source[3];
    return 0;
}
