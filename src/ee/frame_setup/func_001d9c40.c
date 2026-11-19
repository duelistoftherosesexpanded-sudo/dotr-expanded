#include "dotr/ee/frame_setup.h"

DotrSetupWord lSzDuel_SetupCardTexture(DotrSetupWord cursor, int textureId)
{
    struct DotrSetupControl *control = &data_00317490_setup;
    int base = data_00317490_setup.unknown_c80;
    int i;

    if (data_00317490_setup.unknown_6a0[textureId] != 0) {
        return cursor;
    }
    if (textureId >= 0 && textureId < 4) {
        for (i = 0; i <= 3; i++) {
            control->unknown_6a0[i] = 0;
        }
        control->unknown_6a0[8] = 0;
        base += SzText_InitClutManage(base, 1);
        control->unknown_6a0[textureId] = SzText_LoadRequest(3, textureId);
        cursor = SzText_DoRequest_Pipe(cursor, 3, base, 0);
        SzText_FreeRequest(3, textureId);
        cursor = RhText_SetTextureFlush(cursor);
    } else if (textureId == 8) {
        for (i = 0; i <= 3; i++) {
            control->unknown_6a0[i] = 0;
        }
        base += SzText_InitClutManage(base, 1);
        control->unknown_6a0[textureId] = SzText_LoadRequest(9, 0);
        cursor = SzText_DoRequest_Pipe(cursor, 9, base, 0);
        SzText_FreeRequest(9, 0);
        cursor = RhText_SetTextureFlush(cursor);
    }
    return cursor;
}
