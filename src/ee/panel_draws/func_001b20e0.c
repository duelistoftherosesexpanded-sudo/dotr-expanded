#include "dotr/ee/panel_draws.h"

DotrSpriteWord lSzDuel_DrawWindow_SummonCost_Pipe(DotrSpriteWord cursor, int x, int y)
{
    unsigned char *control = data_00317490_bytes;
    int px = x + 3;
    int py = y + 3;
    int text_x = px + 1728;
    int text_y = py + 1824;
    int image = 0;
    DotrSpriteWord result;
    struct DotrSpriteEntry *entry;
    int value_x;
    int value_y;
    int number;

    if (SzDuel_GetPlayer(data_00317490_bytes[0xd3c]) < 32) {
        if (SzDuel_GetPlayer(control[0xd3c] ^ 1) >= 100) {
            image = 1;
        }
    }
    cursor = lSzDuel_DrawWindow_Frame_Pipe(cursor, control[0xd54], -1, 6, x, y, 8, 3, 1);
    cursor = func_001b3c80_cursor(cursor, data_002909b8, text_x + 4, text_y + 3, 0, 1, 0, 0, 1, 0, -1);
    value_x = px + 30;
    value_y = py + 1;
    result = lSzDuel_DrawWindow_Board_Pipe(cursor, control[0xd54], value_x, value_y, 3, 1, -1);
    if (image) {
        result = SzMsg_LoadFont_Pipe(result, 1, 28, 0, 22);
        result = RhText_SetTextureFlush(result);
        entry = SzMsg_GetInfo(1);
        entry->unknown_0c = 44;
        result = func_001fe7d0_panel(result, 1, value_x + 3, value_y + 2, 0, SzMsg_DrawRect_Pipe);
        SzMsg_Free(1);
    } else {
        number = SzDuel_GetUnitLevel(2, control[0xd4f] + 1, 0);
        number = control[0xd4e] ? Ta_GetSummonCost() : number;
        result = SzDuel_DrawWindow_Digit_Pipe(result, value_x + 3, value_y + 2, 2, number, 0);
    }
    return result;
}
