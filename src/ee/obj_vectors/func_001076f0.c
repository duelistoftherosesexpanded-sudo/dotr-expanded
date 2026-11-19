#include "dotr/ee/obj_vectors.h"

int OBJ_RGB_IN(unsigned char *target, unsigned char r, unsigned char g, unsigned char b)
{
    target[0] = r;
    target[1] = g;
    target[2] = b;
    return 0;
}
