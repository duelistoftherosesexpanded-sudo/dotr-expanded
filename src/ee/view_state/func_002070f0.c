#include "dotr/ee/view_state.h"

int Ex3d_NodeL2V(struct DotrViewMatrix *m, int count, struct DotrViewVector *v, int a, int b, int c, float *average)
{
    struct DotrViewMatrix local;
    float sum = 0.0f;
    int result;
    int i;
    struct DotrViewVector *p;
    if (!m) {
        RhView_GetWorldViewMatrix(&local);
        m = &local;
    }
    RhGeom_PushMatrix((int)(DotrViewWord)m);
    for (i = 0; i < count; i++) {
        p = (struct DotrViewVector *)((char *)v + i * 48);
        p->unknown_w = 1.0f;
        RhGeom_MulMatrixVector(p, 0, p);
        sum += p->z;
    }
    RhGeom_PopMatrix();
    if (c >= 0) {
        result = Ex3d_Clipping(6, gEx3d_aPlane, count, (int)(DotrViewWord)v, a, b, c);
    }
    if (average) {
        *average = sum / (float)result;
    }
    return result;
}
