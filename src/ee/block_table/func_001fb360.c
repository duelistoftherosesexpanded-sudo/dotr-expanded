#include "dotr/ee/block_table.h"
/* The body of lSzView_CalcRel (0x1f90e0), which the original has inlined here; the helper name
 * is not original. */
static inline int delta_001fb360(int index)
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
int SzView_SetLookSP(int index, struct DotrBlockVector4 *in)
{
    struct DotrBlockMatrix64 m;
    int angles[4];
    struct DotrBlockDelta *p;
    struct DotrBlockVector4 *polar;
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
    polar = (struct DotrBlockVector4 *)p->unknown_00;
    *polar = *in;
    polar->y = func_001a6c30(6.2831855f + polar->y, 6.2831855f);
    if (func_001fafb0_angles(index, angles)) {
        RhGeom_GetRotXYZMatrix(m.unknown_00, angles);
    }
    RhGeom_MulMatrixVector((struct DotrViewVector *)&p->relativeDirection, (struct DotrViewMatrix *)&m, (struct DotrViewVector *)gSzView_vecUnit);
    p->sourcePoint.x = p->cameraPoint.x + p->relativeDirection.x * polar->x;
    p->sourcePoint.y = p->cameraPoint.y + p->relativeDirection.y * polar->x;
    p->sourcePoint.z = p->cameraPoint.z + p->relativeDirection.z * polar->x;
    p->sourcePoint.unknown_w = 0;
    delta_001fb360(index);
    return 1;
}
