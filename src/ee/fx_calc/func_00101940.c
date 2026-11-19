#include "dotr/ee/fx_calc.h"
#include "dotr/ee/view_state.h"

/* Writes the direction's pitch and yaw as 16-bit angles and a zero third word. */
void ShFx_Vec2Rot(struct DotrFxPoint *v, int *rot)
{
    float len;

    len = func_001a6c60(v->x * v->x + v->z * v->z);
    rot[0] = func_001a22c0(65536.0f * (func_001a6c18(len, v->y) / 6.2831855f)) & 0xffff;
    rot[1] = func_001a22c0(65536.0f * (func_001a6c18(v->x, v->z) / 6.2831855f)) & 0xffff;
    rot[2] = 0;
}
