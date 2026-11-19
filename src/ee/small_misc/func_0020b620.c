#include "dotr/ee/small_misc.h"
/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

/* RhMath_Cos (0x20b620): RhMath_Sin with the phase advanced by a quarter turn. Compiled warm,
 * with the float-to-int conversion written as a cast. */
float RhMath_Cos(float rad)
{
    int phase;
    float sign;
    int fraction;
    float weight;
    float *entry;
    float lower;
    float upper;

    phase = (short)(int)(16384.0f + (0.5f + ((65536.0f * rad) / 6.2831855f)));
    sign = 1.0f;
    if (phase < 0) {
        sign = -1.0f;
        phase = -phase;
    }
    if (phase > 0x4000) {
        phase = 0x8000 - phase;
    }
    if ((phase & 0x3FFF) == 0) {
        if (phase == 0x4000) {
            return sign;
        }
        sign = 0.0f;
    } else {
        fraction = phase & 0xF;
        weight = fraction;
        entry = &data_00294050[phase >> 4];
        lower = entry[0];
        upper = entry[1];
        sign = (sign * (lower * (16 - fraction) + upper * weight)) / 16.0f;
    }
    return sign;
}
