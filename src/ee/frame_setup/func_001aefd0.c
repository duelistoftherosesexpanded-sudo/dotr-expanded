#include "dotr/ee/frame_setup.h"

DotrSetupWord SzDuel_StartWindowSessionWithPalette_Pipe(DotrSetupWord cursor, int a1)
{
    struct DotrSetupControl *control = &data_00317490_setup;
    struct DotrSetupEntry *entry;
    int out;
    int base;
    int i;

    base = RhBase_GetUserGsAddress();
    base += SzText_InitClutManage(base, 2);
    cursor = SzMsg_Init_Pipe(cursor, a1, base, &out);
    base += out;
    for (i = 0; i < 9; i++) {
        control->unknown_6a0[i] = 0;
    }
    control->unknown_6a0[6] = SzText_LoadRequest(3, 6);
    control->unknown_6a0[7] = SzText_LoadRequest(3, 7);
    cursor = SzText_DoRequest_Pipe(cursor, 3, base, &out);
    base += out;
    SzText_FreeRequest(3, -1);
    cursor = SzMsg_LoadFont_Pipe(cursor, 0, 21, 0, 0);
    entry = SzMsg_GetInfo(0);
    entry->unknown_0c = 22;
    cursor = RhText_SetTextureFlush(cursor);
    control->unknown_c7c = base;
    control->unknown_c58 = 0xfffff8;
    SzMsg_SetDefPrioZ(0xfffff8);
    return cursor;
}
