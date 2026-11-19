#include "dotr/ee/block_table.h"
/* The body of lSzView_CalcRel (0x1f90e0), which the original has inlined here; the helper name
 * is not original. */
static inline int delta_001fb6a0(int index)
{
    struct DotrBlockDelta *b;
    if (index < 0 || index > 0) return 0;
    b = gSzView_hView_delta[index];
    if (!b) return 0;
    b->relativeDirection.x = b->cameraPoint.x - b->sourcePoint.x;
    b->relativeDirection.y = b->cameraPoint.y - b->sourcePoint.y;
    b->relativeDirection.z = b->cameraPoint.z - b->sourcePoint.z;
    b->relativeDirection.unknown_w = 0;
    RhGeom_NormalizeVector(&b->relativeDirection);
    return 1;
}
int SzView_RollCamera(int index, int nested, int dir, float yaw, float pitch, float dist)
{
    struct DotrBlockDelta *p;
    float *polar;
    struct DotrBlockVector4 *src;
    struct DotrBlockVector4 *dst;
    int mode;
    float limit;
    float xz;
    float y;
    float z;
    if (index < 0 || index >= 1) {
        return 0;
    }
    p = gSzView_hView_delta[index];
    if (p == 0) {
        return 0;
    }
    polar = (float *)p->unknown_00;
    if (dir > 0) {
        src = &p->cameraPoint;
        dst = &p->sourcePoint;
    } else {
        src = &p->sourcePoint;
        dst = &p->cameraPoint;
    }
    mode = *(int *)((char *)p + 0x8b0);
    switch (mode) {
    case 0:
    case 2:
        limit = 50.0f;
        if (mode == 2) {
            limit = 10.0f;
        }
        if (nested == 0 && polar[0] + dist < limit) {
            SzView_RollCamera(index, 1, -dir, 0.0f, 0.0f, limit - (polar[0] + dist));
        }
        if (polar[2] + pitch > 1.5706217f) {
            pitch = 1.5706217f - polar[2];
        }
        if (polar[2] + pitch < -1.5706217f) {
            pitch = -1.5706217f - polar[2];
        }
        break;
    default:
        break;
    }
    polar[0] = polar[0] + dist;
    polar[1] = func_001a6c30(6.2831855f + (polar[1] + yaw), 6.2831855f);
    polar[2] = func_001a6c30(polar[2] + pitch, 6.2831855f);
    xz = polar[0] * RhMath_Cos(polar[2]);
    y = polar[0] * RhMath_Sin(polar[2]);
    z = xz * RhMath_Sin(polar[1]);
    xz = xz * RhMath_Cos(polar[1]);
    dst->x = src->x + xz * dir;
    dst->y = src->y + y * dir;
    dst->z = src->z + z * dir;
    delta_001fb6a0(index);
    return 1;
}
