#include "dotr/ee/ex3d_clip.h"

int Ex3d_RotTransPers(struct DotrEx3dVector *in, float *matrix, union DotrEx3dPacket *packet, float *w)
{
    float world_screen[16];
    struct DotrEx3dVector v;
    float q;

    v = *in;
    if (matrix == 0) {
        RhView_GetWorldScreenMatrix(world_screen);
        matrix = world_screen;
    }
    RhGeom_PushMatrix(matrix);
    v.w = 1.0f;
    RhGeom_MulMatrixVector(&v, 0, &v);
    RhGeom_PopMatrix();
    q = 1.0f / v.w;
    v.x *= q;
    v.y *= q;
    v.z *= q;
    if (v.z < 1.0f || v.z > 16777215.0f || v.x < 0.0f || v.x > 4095.0f || v.y < 0.0f || v.y > 4095.0f) {
        return 0;
    }
    packet->xyz.x = func_001a23b0(16.0f * v.x);
    packet->xyz.y = func_001a23b0(16.0f * v.y);
    packet->xyz.z = (unsigned int)func_001a23b0(v.z);
    if (w != 0) {
        *w = v.w;
    }
    return 1;
}
