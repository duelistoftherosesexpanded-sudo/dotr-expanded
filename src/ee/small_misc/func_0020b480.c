#include "dotr/ee/small_misc.h"

static inline float special(int phase, float sign)
{
    if (phase == 16384) {
        return sign;
    } else {
        return 0.0f;
    }
}

static inline float blend(float left, float right, int fraction)
{
    float upper = fraction;
    float lower = 16 - fraction;
    return left * lower + right * upper;
}

float RhMath_CosI(int phase)
{
    float sign;
    int fraction;
    float *table;
    phase = (short)(phase + 16384);
    sign = 1.0f;
    if (phase < 0) {
        sign = -1.0f;
        phase = -phase;
    }
    if (phase > 16384) phase = 0x8000 - phase;
    if ((phase & 0x3fff) == 0) {
        return special(phase, sign);
    }
    fraction = phase & 0xf;
    table = &data_00294050[phase >> 4];
    return sign * blend(table[0], table[1], fraction) / 16.0f;
}
