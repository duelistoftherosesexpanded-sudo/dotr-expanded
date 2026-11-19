#include "dotr/ee/state_records.h"

static inline struct DotrStateRecord *lookup(int i)
{
    struct DotrStateRecord *p = data_00317490_records;
    return p + i;
}

int SzDuel_SetPlayerCur(int side, int x, int z)
{
    struct DotrStateRecord *p = lookup(side);
    struct DotrStateVector old, current;
    if (x < 0 || x >= 7 || z < 0 || z >= 7) {
        return 0;
    }
    SzView_IV2V((char *)p + 0x1d0, &old);
    ((unsigned char *)p)[0x33d] = x;
    ((unsigned char *)p)[0x33e] = z;
    current.x = (x - 3) * 50;
    current.z = (z - 3) * 50;
    current.y = (signed char)p->unknown_345;
    SzView_SetCP(0, &current);
    SzView_SetLookSP(0, &old);
    return 1;
}
