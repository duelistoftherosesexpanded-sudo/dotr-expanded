#include "dotr/ee/view_state.h"

/* MATCHING DEVICE: a copy of RhView_PushStack (0x20e460). The original expands that
 * function here, presumably because both are in one source file; this build compiles one
 * function per file, so the body is repeated as an inline function. */
static inline int DotrViewPushStackCopy(void)
{
    int depth = gRhView_nStackIndex[0];
    if (!(depth + 1 < 2)) {
        return 0;
    }
    gRhView_avl[depth + 1] = gRhView_avl[depth];
    gRhView_nStackIndex[0]++;
    return 1;
}

/* v = 2 (n . v) n - v: the vector mirrored about the unit normal and reversed. Assumed to be
 * a macro or inline function in the original (three uses, each with its own factor). */
#define DOTR_VIEW_REFLECT(n, v) \
    { \
        float k = 2.0f * ((n)->x * (v)->x + (n)->y * (v)->y + (n)->z * (v)->z); \
        (v)->x = k * (n)->x - (v)->x; \
        (v)->y = k * (n)->y - (v)->y; \
        (v)->z = k * (n)->z - (v)->z; \
    }

int RhView_PushFakeMirror(struct DotrViewVector *pos, struct DotrViewVector *normal, struct DotrViewBlock *out)
{
    /* Room for two vectors and two matrices plus alignment. The pointers below step by a
     * whole matrix after the second vector, so the inverse matrix starts 144 bytes into the
     * 160 and RhGeom_InvertMatrix writes 48 bytes past this buffer, as in the original. */
    unsigned char raw[0xb0];
    struct DotrViewMirror *mirror = (struct DotrViewMirror *)gRhView_FakeMirror;
    struct DotrViewFrame *frame = (struct DotrViewFrame *)&gRhView_avl[0].unknown_0c0;
    struct DotrViewVector *up = (struct DotrViewVector *)(((DotrViewWord)raw + 15) & -16);
    struct DotrViewVector *side;
    struct DotrViewMatrix *m;
    struct DotrViewMatrix *inv;
    struct DotrViewFrame *view;

    up->x = 0.0f;
    up->y = 1.0f;
    up->z = 0.0f;
    side = up + 1;
    m = (struct DotrViewMatrix *)side + 1;
    up->unknown_w = 0.0f;
    inv = m + 1;
    view = (struct DotrViewFrame *)out;
    if (gRhView_FakeMirror[0].unknown_44) {
        return 0;
    }
    *(struct DotrViewBlock *)view = *(struct DotrViewBlock *)frame;
    mirror->pos = *pos;
    mirror->normal = *normal;
    mirror->dir.x = -frame->unknown_40[4];
    mirror->dir.y = -frame->unknown_40[5];
    mirror->dir.z = -frame->unknown_40[6];
    mirror->eye.x = frame->unknown_40[0] - mirror->pos.x;
    mirror->eye.y = frame->unknown_40[1] - mirror->pos.y;
    mirror->eye.z = frame->unknown_40[2] - mirror->pos.z;
    mirror->dist = RhGeom_GetVectorScalor(&mirror->eye);
    RhGeom_NormalizeVector(&mirror->normal);
    RhGeom_NormalizeVector(&mirror->dir);
    RhGeom_NormalizeVector(&mirror->eye);
    DOTR_VIEW_REFLECT(&mirror->normal, &mirror->dir)
    DOTR_VIEW_REFLECT(&mirror->normal, &mirror->eye)
    *(struct DotrViewVector *)&view->unknown_40[4] = mirror->dir;
    view->unknown_40[0] = pos->x - mirror->eye.x * mirror->dist;
    view->unknown_40[1] = pos->y - mirror->eye.y * mirror->dist;
    view->unknown_40[2] = pos->z - mirror->eye.z * mirror->dist;
    view->unknown_40[3] = 0.0f;
    RhView_GetWorldViewMatrix(m);
    RhGeom_InvertMatrix(inv, m);
    RhGeom_MulMatrixVector(up, inv, up);
    DOTR_VIEW_REFLECT(&mirror->normal, up)
    RhGeom_OuterProduct(side, up, (struct DotrViewVector *)&view->unknown_40[4]);
    RhGeom_CreateCameraMatrix2(m, &view->unknown_40[0], &view->unknown_40[4], 0);
    RhGeom_MulMatrixVector(side, m, side);
    *(int *)&view->unknown_40[16] = func_001a22c0(65536.0f * (func_001a6c18(side->y, side->x) / 6.2831855f)) + 0x7fff + 1;
    DotrViewPushStackCopy();
    mirror->active = 1;
    return 1;
}
