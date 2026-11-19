#include "dotr/ee/obj_vectors.h"

int OBJECT_RGB_A_ON(unsigned char *object, int amount, unsigned char ceiling)
{
    object[0xaf] += amount;
    object[0xb3] += amount;
    if (object[0xaf] > ceiling) {
        return 1;
    }
    return 0;
}
