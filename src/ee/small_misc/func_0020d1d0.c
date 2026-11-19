#include "dotr/ee/small_misc.h"
/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

/* RhGeom_RotationInterpolate (0x20d1d0): weighted mix of two 16-bit angles along the shorter
 * arc, rounded by adding 0.5. Compiled warm (see its build entry): the float-to-int cast becomes
 * a call to the compiler's conversion helper fptosi at 0x1a22c0. */
int RhGeom_RotationInterpolate(float first_rate, float second_rate, int first, int second)
{
    int end = second & 0xFFFF;
    int start = first & 0xFFFF;
    int delta = end - start;

    if (delta > 0x8000) {
        end -= 0x10000;
    } else if (delta < -0x8000) {
        end += 0x10000;
    }

    return (int)((float)start * first_rate + (float)end * second_rate + 0.5f);
}
