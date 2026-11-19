#include "dotr/ee/fx_calc.h"

void GRot_Matrix(float *out, int angle, int axis)
{
    struct DotrFxAngles rot = data_0028c210;
    int *words = (int *)&rot;

    angle &= 0xffff;
    words[axis] = angle;
    RhGeom_RotXYZMatrix(out, words);
}
