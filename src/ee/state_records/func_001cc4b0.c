#include "dotr/ee/state_records.h"

void SzDuel_InitMapEdit(int flag)
{
    unsigned char *p = (unsigned char *)data_00317490_records;
    struct DotrStateVector a = data_002914b0;
    struct DotrStateVector b = data_002914c0;
    int i;
    for (i = 0; i < 84; i++) {
        SzModel_SetDrawFlag(i, 0);
    }
    for (i = 0; i < 5; i++) {
        SzModel_SetDrawFlag(i, 1);
    }
    for (i = 0; i < 10; i++) {
        SzModel_SetDrawFlag(i + 74, 1);
    }
    SzView_SetCP(0, &a);
    SzView_SetLookSP(0, &b);
    data_00a9c08c.unknown_3 = 3;
    data_00a9c08c.unknown_4 = 3;
    data_00a9c08c.unknown_5 = 0;
    data_00a9c08c.unknown_6_bit0 = 0;
    SzDuel_InitMapColor2(0, 0, 1, 0);
    p[3415] = 99;
    if (flag) {
        data_00a9c08c.unknown_1 = 5;
        data_00a9c08c.unknown_0 = data_00a9c08c.unknown_1 - 2;
    }
    p[3427] = 0;
}
