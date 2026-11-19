#include "dotr/ee/lk_frames.h"

int LkSetAngleXY_m90(float *vector)
{
    return func_001a22c0(360.0f / (6.2831855f / func_001a6c18(vector[1], vector[0])) + 90.0f);
}
