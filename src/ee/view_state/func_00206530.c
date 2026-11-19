#include "dotr/ee/view_state.h"
void Ex3d_DivideN(struct DotrViewDivideVertex *p0, struct DotrViewDivideVertex *p1, int flags,
                  struct DotrViewDivideVertex *out, float t)
{
    out->pos[0] = p0->pos[0] + t * (p1->pos[0] - p0->pos[0]);
    out->pos[1] = p0->pos[1] + t * (p1->pos[1] - p0->pos[1]);
    out->pos[2] = p0->pos[2] + t * (p1->pos[2] - p0->pos[2]);
    out->pos[3] = p0->pos[3] + t * (p1->pos[3] - p0->pos[3]);
    if (flags & 1) {
        out->rgba[0] = func_001a23b0((unsigned)p0->rgba[0] + t * (p1->rgba[0] - p0->rgba[0]));
        out->rgba[1] = func_001a23b0((unsigned)p0->rgba[1] + t * (p1->rgba[1] - p0->rgba[1]));
        out->rgba[2] = func_001a23b0((unsigned)p0->rgba[2] + t * (p1->rgba[2] - p0->rgba[2]));
        out->rgba[3] = func_001a23b0((unsigned)p0->rgba[3] + t * (p1->rgba[3] - p0->rgba[3]));
        out->q = p0->q + t * (p1->q - p0->q);
    }
    if (flags & 2) {
        out->t.st[0] = p0->t.st[0] + t * (p1->t.st[0] - p0->t.st[0]);
        out->t.st[1] = p0->t.st[1] + t * (p1->t.st[1] - p0->t.st[1]);
    }
    if (flags & 0x10) {
        struct DotrViewDivideUv *a = &p0->t.uv;
        struct DotrViewDivideUv *b = &p1->t.uv;
        struct DotrViewDivideUv *o = &out->t.uv;
        int n;
        n = a->u;
        o->u = n + func_001a23b0(t * (b->u - n));
        n = a->v;
        o->v = n + func_001a23b0(t * (b->v - n));
    }
}
