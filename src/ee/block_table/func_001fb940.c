#include "dotr/ee/block_table.h"

/* The bodies of lSzView_CalcRel (0x1f90e0) and lSzView_CalcLook (0x1f8f80), which the original
 * has inlined here, as in SzView_SetViewCamera (0x1fc210);
 * the helper names are not original. */
static inline int delta_001fb940(int index)
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
static inline int polar_001fb940(int index)
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
/* 0x1fb940 SzView_SetInterpCameraMotion (name from the Japanese build's symbols). */
int SzView_SetInterpCameraMotion(int index, struct DotrCameraMotion *motion, int frames, int frame, int model, int mirror)
{
    struct DotrBlockVector4 pos;
    int ang[4];
    float mat[16];
    struct DotrBlockCamera *p;
    struct DotrKeyTriple *look;
    struct DotrKeySingle *roll;
    struct DotrKeySingle *fov;
    float t;
    float r;
    float f;
    float c;
    struct DotrViewVector *v;
    if (index < 0 || index >= 1) {
        return 0;
    }
    if (frames == 0) {
        return 0;
    }
    if (motion == 0) {
        return 0;
    }
    if (motion->magic != 0x544f4d43) {
        return 0;
    }
    p = (struct DotrBlockCamera *)gSzView_hView_delta[index];
    if (p == 0) {
        return 0;
    }
    look = (struct DotrKeyTriple *)&motion->data[motion->track[1].offset];
    roll = (struct DotrKeySingle *)&motion->data[motion->track[2].offset];
    fov = (struct DotrKeySingle *)&motion->data[motion->track[3].offset];
    t = (float)(motion->frames - 1) * (float)frame / (float)frames;
    lSzView_CalcInterpCameraMotion((struct DotrKeyOut *)&p->delta.sourcePoint, (struct DotrKeyTriple *)&motion->data[motion->track[0].offset], motion->track[0].count, t);
    lSzView_CalcInterpCameraMotion((struct DotrKeyOut *)&p->delta.cameraPoint, look, motion->track[1].count, t);
    r = lSzView_CalcInterpCameraMotion1(roll, motion->track[2].count, t);
    f = lSzView_CalcInterpCameraMotion1(fov, motion->track[3].count, t);
    p->roll = func_001a22c0(32768.0f * r / 180.0f);
    f = 3.1415927f * f / 180.0f / 2.0f;
    if (f < 1e-6f) {
        f = 1e-6f;
    }
    c = RhMath_Cos(f);
    p->screen = 224.0f * (c / RhMath_Sin(f));
    if (p->screen < p->screen_min) {
        p->screen = p->screen_min;
    }
    if (mirror) {
        p->delta.sourcePoint.x = -p->delta.sourcePoint.x;
        p->delta.cameraPoint.x = -p->delta.cameraPoint.x;
        p->roll = -p->roll;
    }
    if (model >= 0) {
        if (SzModel_GetRot(model, (struct DotrObjectVector4 *)ang)) {
            RhGeom_GetRotXYZMatrix(mat, ang);
            RhGeom_PushMatrix(mat);
            v = (struct DotrViewVector *)&p->delta.sourcePoint;
            RhGeom_MulMatrixVector(v, 0, v);
            v = (struct DotrViewVector *)&p->delta.cameraPoint;
            RhGeom_MulMatrixVector(v, 0, v);
            RhGeom_PopMatrix();
        }
        if (SzModel_GetPos(model, (struct DotrObjectVector4 *)&pos)) {
            p->delta.sourcePoint.x += pos.x;
            p->delta.sourcePoint.y += pos.y;
            p->delta.sourcePoint.z += pos.z;
            p->delta.cameraPoint.x += pos.x;
            p->delta.cameraPoint.y += pos.y;
            p->delta.cameraPoint.z += pos.z;
        }
    }
    delta_001fb940(index);
    polar_001fb940(index);
    return 1;
}
