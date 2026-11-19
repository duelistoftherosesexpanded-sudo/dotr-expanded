#include "dotr/ee/block_table.h"
/* The body of lSzView_CalcLook (0x1f8f80), which the original has inlined here; the helper name
 * is not original. */
static inline int polar_001fad60(int index)
{
    struct DotrBlockVector4 out;
    struct DotrBlockDelta *p;
    struct DotrBlockVector4 *a;
    struct DotrBlockVector4 *b;
    if (index < 0 || index >= 1) {
        return 0;
    }
    p = gSzView_hView_delta[index];
    if (p == 0) {
        return 0;
    }
    a = &p->sourcePoint;
    b = &p->cameraPoint;
    out.x = func_001a6c60((a->z - b->z) * (a->z - b->z) + (a->x - b->x) * (a->x - b->x));
    out.y = func_001a6c18(a->z - b->z, a->x - b->x);
    out.y = func_001a6c30(6.2831855f + out.y, 6.2831855f);
    out.z = func_001a6c18(a->y - b->y, out.x);
    out.x = func_001a6c60((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y) + (a->z - b->z) * (a->z - b->z));
    *(struct DotrBlockVector4 *)p->unknown_00 = out;
    return 1;
}
int SzView_SetRelSP(int index, struct DotrBlockVector4 *in)
{
    struct DotrBlockDelta *p;
    if (index < 0 || index >= 1) {
        return 0;
    }
    p = gSzView_hView_delta[index];
    if (p == 0) {
        return 0;
    }
    if (in == 0) {
        return 0;
    }
    p->relativeDirection = *in;
    RhGeom_NormalizeVector(&p->relativeDirection);
    p->sourcePoint.x = p->cameraPoint.x - in->x * *(float *)p->unknown_00;
    p->sourcePoint.y = p->cameraPoint.y - in->y * *(float *)p->unknown_00;
    p->sourcePoint.z = p->cameraPoint.z - in->z * *(float *)p->unknown_00;
    p->sourcePoint.unknown_w = 0;
    polar_001fad60(index);
    return 1;
}
