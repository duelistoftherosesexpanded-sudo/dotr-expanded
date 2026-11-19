#include "dotr/ee/small_misc.h"
/* Builds a 4x4 rotation matrix (16 floats) from three phase angles; 0x20b480 is cosine and
 * 0x20b550 sine. */
void RhGeom_GetRotXYZMatrix(float *m, int *ang)
{
    float cos_x = RhMath_CosI(ang[0]);
    float sin_x = RhMath_SinI(ang[0]);
    float cos_y = RhMath_CosI(ang[1]);
    float sin_y = RhMath_SinI(ang[1]);
    float cos_z = RhMath_CosI(ang[2]);
    float sin_z = RhMath_SinI(ang[2]);
    m[0] = cos_y * cos_z;
    m[4] = cos_z * (sin_x * sin_y) - cos_x * sin_z;
    m[8] = cos_z * (cos_x * sin_y) + sin_x * sin_z;
    m[1] = cos_y * sin_z;
    m[5] = sin_z * (sin_x * sin_y) + cos_x * cos_z;
    m[9] = sin_z * (cos_x * sin_y) - sin_x * cos_z;
    m[2] = -sin_y;
    m[6] = sin_x * cos_y;
    m[10] = cos_x * cos_y;
    m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f;
    m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f;
    m[15] = 1.0f;
}
