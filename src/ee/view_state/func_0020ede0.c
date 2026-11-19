#include "dotr/ee/view_state.h"

/* One side plane: d is the signed distance numerator of the sphere centre, n the squared
 * length of the plane normal, so d * d / n is the squared distance compared with the squared
 * radius. Behind the plane and further away than the radius the sphere is outside (the
 * function returns); nearer than the radius on either side it crosses the plane. Assumed to
 * be a macro or inline function in the original: each use has its own two temporaries. */
#define DOTR_VIEW_SIDE(x, n, cross, outside) \
    { \
        float d = (x); \
        float q = d * d / (n); \
        if (d < 0.0f) { \
            if (q >= rr) { \
                return outside; \
            } \
            cross; \
        } else if (q <= rr) { \
            cross; \
        } \
    }

int RhView_CheckViewVolume(struct DotrViewVector *centre, float radius)
{
    float sx;
    float rr;
    float z;
    float sy;
    float nn;
    int flags;
    float ny;
    float h;
    struct DotrViewEntry *e;
    float dist;
    float nx;
    float hy;
    float hx;

    flags = 0;
    e = &gRhView_avl[gRhView_nStackIndex[0]];
    centre->unknown_w = 1.0f;
    RhGeom_MulMatrixVector(&data_0032a230, 0, centre);
    z = data_0032a230.z;
    if (e->unknown_16c > z + radius) {
        return 4;
    }
    if (e->unknown_16c > z - radius) {
        flags = 4;
    }
    if (e->unknown_170 < z - radius) {
        return 4;
    }
    if (e->unknown_170 < z + radius) {
        flags = 4;
    }
    sx = e->unknown_164 * (1.0f / e->unknown_158);
    sy = e->unknown_168 * (1.0f / e->unknown_15c);
    dist = e->unknown_160;
    nn = *(float *)&e->unknown_188[0];
    ny = dist * data_0032a230.y;
    h = 2046.0f * z;
    rr = radius * radius;
    DOTR_VIEW_SIDE(h - ny, nn, flags = 6, 4)
    DOTR_VIEW_SIDE(h + ny, nn, flags = 6, 4)
    nx = dist * data_0032a230.x;
    DOTR_VIEW_SIDE(h - nx, nn, flags = 6, 4)
    DOTR_VIEW_SIDE(h + nx, nn, flags = 6, 4)
    nn = *(float *)&e->unknown_188[8];
    hy = z * sy / 2.0f;
    DOTR_VIEW_SIDE(hy - ny, nn, flags |= 2, flags | 2)
    DOTR_VIEW_SIDE(hy + ny, nn, flags |= 2, flags | 2)
    hx = z * sx / 2.0f;
    nn = *(float *)&e->unknown_188[4];
    DOTR_VIEW_SIDE(hx - nx, nn, flags |= 2, flags | 2)
    DOTR_VIEW_SIDE(hx + nx, nn, flags |= 2, flags | 2)
    return flags | 1;
}
