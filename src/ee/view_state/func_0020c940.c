#include "dotr/ee/view_state.h"
/* Builds a transform from optional scale (3 floats), rotation angles (3 ints) and position (3 floats). */
void RhGeom_GetScaleRotXYZTransMatrix(float *outMatrix, float *scale, int *rot, float *pos)
{
    float cosRotX, sinRotX, cosRotY, sinRotY, cosRotZ, sinRotZ;
    RhGeom_UnitMatrix((struct DotrViewMatrix *)outMatrix);
    if (pos) {
        outMatrix[12] = pos[0];
        outMatrix[13] = pos[1];
        outMatrix[14] = pos[2];
    }
    if (rot) {
        cosRotX = RhMath_CosI(rot[0]);
        sinRotX = RhMath_SinI(rot[0]);
        cosRotY = RhMath_CosI(rot[1]);
        sinRotY = RhMath_SinI(rot[1]);
        cosRotZ = RhMath_CosI(rot[2]);
        sinRotZ = RhMath_SinI(rot[2]);
        outMatrix[0] = cosRotY * cosRotZ;
        outMatrix[4] = cosRotZ * (sinRotX * sinRotY) - cosRotX * sinRotZ;
        outMatrix[8] = cosRotZ * (cosRotX * sinRotY) + sinRotX * sinRotZ;
        outMatrix[1] = cosRotY * sinRotZ;
        outMatrix[5] = sinRotZ * (sinRotX * sinRotY) + cosRotX * cosRotZ;
        outMatrix[9] = sinRotZ * (cosRotX * sinRotY) - sinRotX * cosRotZ;
        outMatrix[2] = -sinRotY;
        outMatrix[6] = sinRotX * cosRotY;
        outMatrix[10] = cosRotX * cosRotY;
    }
    if (scale) {
        outMatrix[0] = outMatrix[0] * scale[0];
        outMatrix[1] = outMatrix[1] * scale[0];
        outMatrix[2] = outMatrix[2] * scale[0];
        outMatrix[4] = outMatrix[4] * scale[1];
        outMatrix[5] = outMatrix[5] * scale[1];
        outMatrix[6] = outMatrix[6] * scale[1];
        outMatrix[8] = outMatrix[8] * scale[2];
        outMatrix[9] = outMatrix[9] * scale[2];
        outMatrix[10] = outMatrix[10] * scale[2];
    }
}
