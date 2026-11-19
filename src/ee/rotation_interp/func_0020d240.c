#include "dotr/ee/rotation_interp.h"
/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

/* RhGeom_RoationComplexInterpolate: the rotations are taken as 20-bit angles (low halfword
 * << 4). Besides the target itself it forms the equivalent rotation (pi - y, with x and z from
 * 0x1a6c18 of the sines and cosines divided by cos(pi - y)); each target component is moved by
 * a full turn when it is more than half a turn from the start, and the form with the smaller
 * summed distance is used (the first on a tie). Each result component is
 * start * weight0 + end * weight1, wrapped to 20 bits and rounded back to 16.
 * Compiled warm (float-to-int conversions as casts). */
void RhGeom_RoationComplexInterpolate(int *out, int *from, int *to, float weight0, float weight1)
{
    int end[2][3];
    int diff[2][3];
    int start[3];
    float sinX;
    float cosX;
    float sinZ;
    float cosZ;
    float cosY;
    float cosY2;
    int d;
    int pick;

    start[0] = (from[0] & 0xFFFF) << 4;
    start[1] = (from[1] & 0xFFFF) << 4;
    start[2] = (from[2] & 0xFFFF) << 4;
    end[0][0] = (to[0] & 0xFFFF) << 4;
    end[0][1] = (to[1] & 0xFFFF) << 4;
    end[0][2] = (to[2] & 0xFFFF) << 4;
    sinX = RhMath_SinI(to[0]);
    cosX = RhMath_CosI(to[0]);
    sinZ = RhMath_SinI(to[2]);
    cosZ = RhMath_CosI(to[2]);
    cosY = RhMath_CosI(to[1]);
    end[1][1] = (0x80000 - end[0][1]) & 0xFFFFF;
    cosY2 = RhMath_CosI(end[1][1] >> 4);
    end[1][0] = ((int)(65536.0f * (func_001a6c18(sinX * cosY / cosY2, cosX * cosY / cosY2) / 6.2831855f)) & 0xFFFF) << 4;
    end[1][2] = ((int)(65536.0f * (func_001a6c18(cosY * sinZ / cosY2, cosY * cosZ / cosY2) / 6.2831855f)) & 0xFFFF) << 4;
    {
        int i;
        int j;

        for (i = 0; i < 2; i++) {
            for (j = 0; j < 3; j++) {
                d = end[i][j] - start[j];
                if (d > 0x80000) {
                    end[i][j] -= 0x100000;
                    diff[i][j] = 0x100000 - d;
                } else if (d < -0x80000) {
                    end[i][j] += 0x100000;
                    diff[i][j] = d + 0x100000;
                } else if (d < 0) {
                    diff[i][j] = -d;
                } else {
                    diff[i][j] = d;
                }
            }
        }
    }
    pick = (diff[0][0] + diff[0][1] + diff[0][2] > diff[1][0] + diff[1][1] + diff[1][2]) ? 1 : 0;
    out[0] = (((int)(start[0] * weight0 + end[pick][0] * weight1) & 0xFFFFF) + 8) >> 4;
    out[1] = (((int)(start[1] * weight0 + end[pick][1] * weight1) & 0xFFFFF) + 8) >> 4;
    out[2] = (((int)(start[2] * weight0 + end[pick][2] * weight1) & 0xFFFFF) + 8) >> 4;
}
