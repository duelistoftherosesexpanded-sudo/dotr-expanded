#include "dotr/ee/obj_vectors.h"

int OBJ_RR2H_IN(float x, float y, float z, void *object)
{
    *(short *)((char *)object + 0xa8) = func_001a22c0(x);
    *(short *)((char *)object + 0xaa) = func_001a22c0(y);
    *(float *)((char *)object + 0xa0) = z;
    return 0;
}
