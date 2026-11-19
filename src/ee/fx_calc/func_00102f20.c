#include "dotr/ee/fx_calc.h"
#include "dotr/ee/view_state.h"

/* Fills count + 1 points from start towards end along a spiral whose radius follows half a
 * sine wave: point 0 is start; point i (1..count) lies at radius * sin(180 * i / count degrees)
 * on the circle at angle + i * turn degrees, at height i * length / count, turned by the
 * matrix built from the direction's two angles and moved to start. Unlike ShFx_Spin2P the
 * end point is not copied; point count is computed like the others. */
void ShFx_Hsin2P(struct DotrFxSpin *p)
{
    struct DotrViewMatrix m;
    struct DotrViewVector dir;
    struct DotrViewVector v;
    int ang[4];
    struct DotrFxPoint *out;
    float step;
    float wave;
    float r;
    float rad;
    int i;
    float len;

    /* MATCHING DEVICE: the output pointer assigned inside the first store. */
    *(out = p->out) = p->start;
    out++;
    dir.x = p->end.x - p->start.x;
    dir.y = p->end.y - p->start.y;
    dir.z = p->end.z - p->start.z;
    step = RhGeom_GetVectorScalor(&dir) / p->count;
    wave = 180.0f / p->count;
    len = func_001a6c60(dir.x * dir.x + dir.z * dir.z);
    ang[0] = func_001a22c0(65536.0f * (func_001a6c18(len, dir.y) / 6.2831855f)) & 0xffff;
    ang[1] = func_001a22c0(65536.0f * (func_001a6c18(dir.x, dir.z) / 6.2831855f)) & 0xffff;
    ang[2] = 0;
    RhGeom_GetRotXYZMatrix(m.unknown_values, ang);
    for (i = 1; i < p->count + 1; i++) {
        r = p->radius * func_001a6a88(3.1415927f * (wave * i) / 180.0f);
        rad = 3.1415927f * (p->angle + i * p->turn) / 180.0f;
        v.x = r * func_001a6898(rad);
        v.z = r * func_001a6a88(rad);
        v.y = step * i;
        RhGeom_MulMatrixVector((struct DotrViewVector *)out, &m, &v);
        out->x += p->start.x;
        out->y += p->start.y;
        out->z += p->start.z;
        out++;
    }
}
