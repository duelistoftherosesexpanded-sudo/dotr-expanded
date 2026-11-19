#include "dotr/ee/block_table.h"

/* The bodies of lSzView_CalcRel (0x1f90e0) and lSzView_CalcLook (0x1f8f80),
 * which the original has inlined here (each with its own index and block
 * test); the helper names are not original. */
static inline int delta_001f9d80(int index)
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
static inline int polar_001f9d80(int index)
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
int SzView_SetCP(int index, struct DotrBlockVector4 *in)
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
        in = &gSzView_pntCPDef;
    }
    p->cameraPoint = *in;
    delta_001f9d80(index);
    polar_001f9d80(index);
    return 1;
}
