#include "dotr/ee/frame_setup.h"

DotrSetupWord SzDuel_StartCardSessionWithPalette_Pipe(DotrSetupWord cursor, int a1)
{
    struct DotrSetupControl *control = &data_00317490_setup;
    struct DotrSetupEntry *entry;
    int out;
    int base;
    int i;

    RhBase_AssignRenderFrame(3, 2, 50);
    cursor = RhBase_EnableRenderFrame(cursor);
    cursor = RhBase_DisableRenderFrame(RhBase_ClearRenderFrame(cursor, 0));
    base = RhBase_GetUserGsAddress();
    base += SzText_InitClutManage(base, 2);
    cursor = SzMsg_Init_Pipe(cursor, a1, base, &out);
    base += out;
    for (i = 0; i < 9; i++) {
        control->unknown_6a0[i] = 0;
    }
    control->unknown_6a0[4] = SzText_LoadRequest(3, 4);
    control->unknown_6a0[5] = SzText_LoadRequest(3, 5);
    control->unknown_6a0[6] = SzText_LoadRequest(3, 6);
    cursor = SzText_DoRequest_Pipe(cursor, 3, base, &out);
    base += out;
    SzText_FreeRequest(3, -1);
    control->unknown_c80 = base;
    base += SzText_InitClutManage(base, 1);
    control->unknown_6a0[0] = SzText_LoadRequest(3, 0);
    cursor = SzText_DoRequest_Pipe(cursor, 3, base, &out);
    base += out;
    SzText_FreeRequest(3, 0);
    cursor = SzMsg_LoadFont_Pipe(cursor, 0, 21, 0, 0);
    entry = SzMsg_GetInfo(0);
    entry->unknown_0c = 22;
    cursor = RhText_SetTextureFlush(SzMsg_LoadFont_Pipe(SzMsg_LoadFont_Pipe(cursor, 1, 161, 0, 22), 2, 162, 0, 44));
    control->unknown_c7c = base;
    control->unknown_c54 = 0;
    control->unknown_d64_5 = 0;
    return cursor;
}
