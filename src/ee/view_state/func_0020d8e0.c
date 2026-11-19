#include "dotr/ee/view_state.h"

/* Builds a scale/offset matrix with depth terms in m and a temporary with scale a,
 * then combines them through RhGeom_MulMatrix. */
void RhGeom_GetScreenMatrix(struct DotrViewMatrix *m, float a, float b, float c, float d, float e, float f, float g, float h, float i)
{
    struct DotrViewMatrix t;
    float *v = m->unknown_values;
    RhGeom_UnitMatrix(&t);
    t.unknown_values[0] = a;
    t.unknown_values[5] = a;
    t.unknown_values[10] = 0.0f;
    t.unknown_values[15] = 0.0f;
    t.unknown_values[14] = 1.0f;
    t.unknown_values[11] = 1.0f;
    RhGeom_UnitMatrix(m);
    v[0] = b;
    v[5] = c;
    v[10] = i * h * (-f + g) / (-h + i);
    v[12] = d;
    v[13] = e;
    v[14] = (-g * h + f * i) / (-h + i);
    RhGeom_MulMatrix(m, &t);
}
