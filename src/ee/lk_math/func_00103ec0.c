#include "dotr/ee/lk_math.h"

int LkMoveHoming(float *pos, float *target, struct DotrLkVec *dir, int limit)
{
    float m[16];
    int ang[4];
    struct DotrLkVec out;
    int ty, tx, tz;
    int dy, dx, dz;

    /* MATCHING DEVICE: the first subtraction written out, the other two as -=. */
    target[0] = target[0] - pos[0];
    target[1] -= pos[1];
    target[2] -= pos[2];
    ty = func_001a22c0(func_001a6c18(target[0], target[2]) / 6.2831855f * 65536.0f) % 0x10000;
    tx = func_001a22c0(func_001a6c18(target[2], target[1]) / 6.2831855f * 65536.0f) % 0x10000;
    tz = func_001a22c0(func_001a6c18(target[1], target[0]) / 6.2831855f * 65536.0f) % 0x10000;
    dy = func_001a22c0(func_001a6c18(dir->x, dir->z) / 6.2831855f * 65536.0f) % 0x10000;
    dx = func_001a22c0(func_001a6c18(dir->z, dir->y) / 6.2831855f * 65536.0f) % 0x10000;
    dz = func_001a22c0(func_001a6c18(dir->y, dir->x) / 6.2831855f * 65536.0f) % 0x10000;
    ang[0] = dotr_lk_regulate_rot_border(tx, dx);
    ang[1] = dotr_lk_regulate_rot_border(ty, dy);
    ang[2] = dotr_lk_regulate_rot_border(tz, dz);
    ang[3] = 0;
    if (ang[0] >= limit) {
        ang[0] = limit;
    } else if (ang[0] <= -limit) {
        ang[0] = -limit;
    }
    if (ang[1] >= limit) {
        ang[1] = limit;
    } else if (ang[1] <= -limit) {
        ang[1] = -limit;
    }
    if (ang[2] >= limit) {
        ang[2] = limit;
    } else if (ang[2] <= -limit) {
        ang[2] = -limit;
    }
    RhGeom_GetRotXYZMatrix(m, ang);
    RhGeom_MulMatrixVector(&out, m, dir);
    pos[0] = pos[0] + out.x;
    pos[1] = pos[1] + out.y;
    pos[2] = pos[2] + out.z;
    dir->x = out.x;
    dir->y = out.y;
    dir->z = out.z;
    /* MATCHING DEVICE: w cleared as an int word (the original stores $zero). */
    *(int *)&dir->w = 0;
    return 0;
}
