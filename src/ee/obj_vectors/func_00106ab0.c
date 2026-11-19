#include "dotr/ee/obj_vectors.h"

void PV_IN_(short *target, float *source, int second)
{
    if (second == 0) {
        target[0] = func_001a22c0(source[0]);
        target[1] = func_001a22c0(source[1]);
        target[2] = func_001a22c0(source[2]);
        target[3] = func_001a22c0(source[3]);
        return;
    }
    target[4] = func_001a22c0(source[0]);
    target[5] = func_001a22c0(source[1]);
    target[6] = func_001a22c0(source[2]);
    target[7] = func_001a22c0(source[3]);
}
