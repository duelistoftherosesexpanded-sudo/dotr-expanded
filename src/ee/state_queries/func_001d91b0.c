#include "dotr/ee/state_queries.h"

/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

/* lSzDuel_LockCamera2 (0x1d91b0): points the current side's camera at the middle of two
 * board cells, turned to the line between them. Cell coordinates become (n - 3) * 50
 * (through float), the angle is func_001a6c18(dz, dx) (assumed atan2f) scaled to 65536 per
 * turn, rounded to a multiple of 256 plus a quarter turn. The side's four words at +0x1d0
 * hold 150, the angle and 0x4000 only while SzView_IV2V converts them, then are restored.
 * Compiled warm (see its build entry). */

struct DotrQueryLockVec4 {
    float x;
    float y;
    float z;
    float w;
};

void lSzDuel_LockCamera2(int arg0, int arg1, int arg2, int arg3, int arg4)
{
    struct DotrQueryLockVec4 saved;
    float pos[4];
    float camera[4];
    unsigned char *p;
    int x0;
    int z0;
    int x1;
    int z1;
    int angle;
    unsigned char *cur;

    p = data_00317490_query_bytes + turnOwner[0] * 0x350;
    saved = *(struct DotrQueryLockVec4 *)(p + 0x1D0);
    x0 = (int)(float)((arg1 - 3) * 50);
    z0 = (int)(float)((arg2 - 3) * 50);
    x1 = (int)(float)((arg3 - 3) * 50);
    z1 = (int)(float)((arg4 - 3) * 50);
    angle = (int)(65536.0f * (func_001a6c18((float)(z1 - z0), (float)(x1 - x0)) / 6.2831855f));
    *(int *)(p + 0x1D0) = 0x96;
    *(int *)(p + 0x1D4) = (((angle + 0x80) >> 8) << 8) + 0x4000;
    *(int *)(p + 0x1D8) = 0x4000;
    cur = data_00317490_query_bytes + turnOwner[0] * 0x350;
    func_001fc950_lockcam(cur + 0x1D0, camera);
    pos[0] = (float)((x0 + x1) >> 1);
    pos[2] = (float)((z0 + z1) >> 1);
    pos[1] = (float)*(signed char *)(cur + 0x345);
    func_001fa8a0_lockcam(0, 0, 3, (unsigned short)arg0, 0, pos);
    func_001fa8a0_lockcam(0, 0, 4, arg0 & 0xFFFF, 0, camera);
    *(struct DotrQueryLockVec4 *)(p + 0x1D0) = saved;
}
