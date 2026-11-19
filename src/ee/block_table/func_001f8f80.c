#include "dotr/ee/block_table.h"

int lSzView_CalcLook(int index)
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
