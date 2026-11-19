#include "dotr/ee/scaled_sprite.h"

DotrSpriteWord lSzDuel_DrawWindow_Field_Pipe(DotrSpriteWord cursor, int id, int x, int y)
{
    struct DotrSpriteEntry *entry;
    int width;
    float sx;

    sx = 1.0f;
    cursor = SzMsg_LoadFont_Pipe(cursor, 1, id + 71, 0, 22);
    cursor = RhText_SetTextureFlush(cursor);
    cursor = lSzDuel_DrawWindow_Frame_Pipe(cursor, -1, 2, 3, x, y, 9, 18, 1);
    cursor = lSzDuel_DrawWindow_Frame_Pipe(cursor, 5, -1, 3, x, y + 64, 9, 2, 1);
    entry = SzMsg_GetInfo(1);
    width = entry->unknown_0c;
    if (width > 84) {
        sx = 84.0f / width;
    }
    cursor = SzMsg_DrawRatioX_Pipe(cursor, 1, x + 48 - func_001a22c0((width >> 1) * sx), y + 67, 0, SzMsg_DrawRect_Pipe, sx, 1.0f);
    SzMsg_Free(1);
    return cursor;
}
