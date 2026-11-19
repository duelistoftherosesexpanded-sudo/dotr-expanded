#include "dotr/ee/view_state.h"
int Ex3d_GetSurfaceNormal(int n, struct DotrViewVector *pts, struct DotrViewVector *out)
{
    int i;
    struct DotrViewVector e1, e2, c;
    struct DotrViewVector *a, *b, *d;
    out->x = 0.0f; out->y = 0.0f; out->z = 0.0f; out->unknown_w = 0.0f;
    if (n < 3) return 0;
    for (i = 0; i < n; i++) {
        a = &pts[i % n];
        b = &pts[(i + 1) % n];
        d = &pts[(i + 2) % n];
        e1 = data_00293d60;
        e1.x = b->x - a->x; e1.y = b->y - a->y; e1.z = b->z - a->z;
        e2 = data_00293d70;
        e2.x = d->x - b->x; e2.y = d->y - b->y; e2.z = d->z - b->z;
        RhGeom_OuterProduct(&c, &e1, &e2);
        out->x = out->x + c.x; out->y = out->y + c.y; out->z = out->z + c.z;
    }
    out->x = out->x / n; out->y = out->y / n; out->z = out->z / n;
    RhGeom_NormalizeVector(out);
    return 1;
}
