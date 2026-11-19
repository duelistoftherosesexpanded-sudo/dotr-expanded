#include "dotr/ee/g_trans.h"

/* MATCHING DEVICE: GRot_Matrix (0x100e00) inlined; see g_trans.h. */
static inline void g_rot(float *out, int angle, int axis)
{
    struct DotrGAngles rot = data_0028c210_g;
    int *words = (int *)&rot;

    angle &= 0xffff;
    words[axis] = angle;
    func_0020cb90_g(out, words);
}

void GVec2RotMat_Z(float *out, float *v)
{
    func_0020bbb0_g(out);
    g_rot(out, func_001a22c0_g(65536.0f * (func_001a6c18_g(v[1], v[0]) / 6.2831855f)), 2);
    g_rot(out, func_001a22c0_g(65536.0f * (func_001a6c18_g(func_001a6c60_g(func_001a6c48_g(v[0], 2.0f) + func_001a6c48_g(v[1], 2.0f)), v[2]) / 6.2831855f)), 1);
}
