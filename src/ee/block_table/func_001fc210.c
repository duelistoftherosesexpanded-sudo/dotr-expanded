#include "dotr/ee/block_table.h"

/* The bodies of 0x1f90e0 and 0x1f8f80, which the original has inlined here
 * (each with its own index and block test, twice: once per case); the helper names are not
 * original. */
static inline int delta_001fc210(int index)
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
static inline int polar_001fc210(int index)
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
/* 0x1fc210 SzView_SetViewCamera (name from the Japanese build's symbols). */
int SzView_SetViewCamera(int index, int model, struct DotrBlockVector4 *base, int mode, float ratio)
{
    struct DotrBlockVector4 center;
    struct DotrBlockVector4 old;
    struct DotrBlockDelta *p;
    float inv = 1.0f - ratio;
    if (index < 0 || index >= 1) {
        return 0;
    }
    p = gSzView_hView_delta[index];
    if (p == 0) {
        return 0;
    }
    if (base == 0) {
        base = &p->cameraPoint;
    }
    center = p->cameraPoint;
    switch (mode) {
    case 1:
        SzModel_GetCenter(model, &center);
        p->cameraPoint.x = center.x * ratio + base->x * inv;
        p->cameraPoint.y = base->y * inv + center.y * ratio;
        p->cameraPoint.z = base->z * inv + center.z * ratio;
        p->cameraPoint.unknown_w = 1.0f;
        delta_001fc210(index);
        polar_001fc210(index);
        break;
    case 2:
        old = *(struct DotrBlockVector4 *)p->unknown_00;
        SzModel_GetCenter(model, &center);
        p->cameraPoint.x = center.x * ratio + base->x * inv;
        p->cameraPoint.y = base->y * inv + center.y * ratio;
        p->cameraPoint.z = base->z * inv + center.z * ratio;
        p->cameraPoint.unknown_w = 1.0f;
        delta_001fc210(index);
        polar_001fc210(index);
        old.y -= ((struct DotrBlockVector4 *)p->unknown_00)->y;
        old.z -= ((struct DotrBlockVector4 *)p->unknown_00)->z;
        old.x -= ((struct DotrBlockVector4 *)p->unknown_00)->x;
        SzView_RollCamera(index, 0, 1, old.y, old.z, old.x);
        break;
    }
    return 1;
}
