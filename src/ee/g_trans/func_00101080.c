#include "dotr/ee/g_trans.h"

/* MATCHING DEVICE: GRot_Matrix (0x100e00) inlined, the same copy as in func_00100e40.c. */
static inline void g_rot(float *out, int angle, int axis)
{
    struct DotrGAngles rot = data_0028c210_g;
    int *words = (int *)&rot;

    angle &= 0xffff;
    words[axis] = angle;
    func_0020cb90_g(out, words);
}

/* MATCHING DEVICE: the element as an inline helper with the sign in a local. The three index
 * draws share one expression; the target compiler evaluates them left to right. */
static inline float g_rand_elem(void)
{
    float *p = &data_0028bc10[(func_0019a5c8() >> 23) % 4][(func_0019a5c8() >> 23) % 32][(func_0019a5c8() >> 23) % 3];
    float s = 2.0f * (float)((func_0019a5c8() >> 23) % 2) - 1.0f;

    return s * *p;
}

void GSet_RandVector(float *out, float scale)
{
    float m[16];

    out[0] = g_rand_elem();
    out[1] = g_rand_elem();
    out[2] = g_rand_elem();
    out[3] = 1.0f;
    func_00187b48(out, out, scale);
    func_0020bbb0_g(m);
    g_rot(m, ((func_0019a5c8() >> 23) % 256) << 16 >> 8, 0);
    g_rot(m, ((func_0019a5c8() >> 23) % 256) << 16 >> 8, 1);
    g_rot(m, ((func_0019a5c8() >> 23) % 256) << 16 >> 8, 2);
    func_001878e0(out, m, out);
}
