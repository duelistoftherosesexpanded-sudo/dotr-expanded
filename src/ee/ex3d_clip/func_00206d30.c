#include "dotr/ee/ex3d_clip.h"

#define DOTR_EX3D_ABS(x) ((x) >= 0.0f ? (x) : -(x))

int Ex3d_Clipping(int plane_count, struct DotrEx3dPlane *planes, int count, union DotrEx3dVertex *vertices, int unused,
                  union DotrEx3dVertex *work, int flags)
{
    struct DotrEx3dPlane *plane = planes;
    struct DotrEx3dPair saved;
    union DotrEx3dVertex cut;
    int keep;
    int p;
    int i;
    int out;
    union DotrEx3dVertex *cur;
    union DotrEx3dVertex *next;
    float d0;
    float d1;

    (void)unused;
    if (count < 2) {
        return count;
    }
    keep = flags & 1;
    if (!keep) {
        saved = vertices->v.pair;
    }
    if ((unsigned int)count > 341) {
        count = 341;
    }
    for (p = 0; p < plane_count; p++, plane++) {
        out = 0;
        for (i = 0; i < count; i++) {
            cur = &vertices[i];
            next = &vertices[(i + 1) % count];
            d0 = (plane->x * cur->v.pos[0] + plane->y * cur->v.pos[1] + plane->z * cur->v.pos[2] + plane->d) / plane->e;
            d1 = (plane->x * next->v.pos[0] + plane->y * next->v.pos[1] + plane->z * next->v.pos[2] + plane->d) /
                 plane->e;
            if (d0 < 0.0f && d1 < 0.0f) {
                continue;
            }
            if (!(d0 < 0.0f) && !(d1 < 0.0f)) {
                work[out++] = *cur;
            } else {
                Ex3d_DivideN(cur, next, flags, &cut, DOTR_EX3D_ABS(d0) / (DOTR_EX3D_ABS(d0) + DOTR_EX3D_ABS(d1)));
                if (!(d0 < 0.0f)) {
                    work[out++] = *cur;
                }
                work[out++] = cut;
            }
            if ((unsigned int)out >= 341) {
                out = 341;
                break;
            }
        }
        if (count < 3) {
            if (out > count) {
                out = count;
            }
        }
        for (i = 0; i < out; i++) {
            vertices[i] = work[i];
        }
        count = out;
    }
    if (!keep) {
        vertices->v.pair = saved;
    }
    return count;
}
