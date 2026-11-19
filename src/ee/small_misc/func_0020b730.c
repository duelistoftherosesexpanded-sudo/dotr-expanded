#include "dotr/ee/small_misc.h"

static inline float blend(float left, float right, int fraction)
{
    float upper = fraction;
    float lower = 16 - fraction;
    return left * lower + right * upper;
}
float RhMath_Sin(float rad)
{
    float sign;
    int fraction;
    float *table;
    int phase;
    float result;
    phase = (short)func_001a22c0(rad * 65536.0f / 6.2831855f + 0.5f);
    sign = 1.0f;
    if (phase < 0) {
        sign = -1.0f;
        phase = -phase;
    }
    if (phase > 16384) phase = 0x8000 - phase;
    if ((phase & 0x3fff) == 0) {
        if (phase == 16384) {
            result = sign;
        } else {
            result = 0.0f;
        }
    } else {
        fraction = phase & 0xf;
        table = &data_00294050[phase >> 4];
        result = sign * blend(table[0], table[1], fraction) / 16.0f;
    }
    return result;
}
