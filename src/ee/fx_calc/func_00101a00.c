#include "dotr/ee/fx_calc.h"
#include "dotr/ee/view_state.h"

/* Builds the rotation matrix for the direction's pitch and yaw. */
void ShFx_Vec2Mat(struct DotrFxPoint *v, float *m)
{
    int rot[4];
    float len;

    len = func_001a6c60(v->x * v->x + v->z * v->z);
    rot[0] = func_001a22c0(65536.0f * (func_001a6c18(len, v->y) / 6.2831855f)) & 0xffff;
    rot[1] = func_001a22c0(65536.0f * (func_001a6c18(v->x, v->z) / 6.2831855f)) & 0xffff;
    rot[2] = 0;
    RhGeom_GetRotXYZMatrix(m, rot);
}
