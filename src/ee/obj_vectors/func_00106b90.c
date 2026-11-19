#include "dotr/ee/obj_vectors.h"

int OBJECT_RGB_A_OFF(unsigned char *object, int amount, unsigned char floor)
{
    object[0xaf] -= amount;
    object[0xb3] -= amount;
    if (object[0xaf] < floor) {
        return 1;
    }
    return 0;
}
