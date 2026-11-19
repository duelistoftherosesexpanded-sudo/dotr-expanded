#include "dotr/ee/obj_vectors.h"

void ATARI3(struct DotrObjVector *out, struct DotrObjVector *offset, struct DotrObjVector *point,
            struct DotrObjVector *from, struct DotrObjVector *to)
{
    struct DotrObjVector r;
    float dx, dy, dz, len, ax, ay, az, t, px, py, pz;
    r.x = 0.0f;
    px = 0.0f; py = point->y; pz = 0.0f;
    ax = from->x + offset->x;
    ay = from->y;
    az = from->z + offset->z;
    dx = (to->x + offset->x) - ax;
    dy = to->y - ay;
    dz = (to->z + offset->z) - az;
    len = dx * dx + dy * dy + dz * dz;
    if (len == 0.0f) { *out = r; return; }
    t = (dx * (px - ax) + dy * (py - ay) + dz * (pz - az)) / len;
    r.x = ax + dx * t;
    r.y = 0.0f;
    r.z = az + dz * t;
    *out = r;
}
