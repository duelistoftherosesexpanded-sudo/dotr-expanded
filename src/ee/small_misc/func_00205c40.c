#include "dotr/ee/small_misc.h"
float Ex3d_GetLookVector(int arg)
{
    float *p = func_001faf60_floats(0);

    SzView_GetLookVector(0, arg);
    return *p;
}
