#include "dotr/ee/scaled_sprite.h"

DotrSpriteWord lSzDuel_DrawBuildDeck_Detail_Special_Pipe(DotrSpriteWord cursor, int id, int x, int y, int flag)
{
    struct DotrSpriteEntry *entry;
    int shift;
    int px;
    int py;
    float sx;
    float sy;

    shift = flag ? 640 : -320;
    sx = 1.0f;
    sy = sx;
    cursor = SzMsg_LoadFont_Pipe(cursor, 1, id + 1174, 0, 22);
    cursor = RhText_SetTextureFlush(cursor);
    entry = SzMsg_GetInfo(1);
    px = x + shift + 28;
    py = y + 48;
    if (entry->unknown_0c > 262) {
        sx = 262.0f / entry->unknown_0c;
    }
    if (entry->unknown_10 > 256) {
        sy = 256.0f / entry->unknown_10;
    }
    cursor = SzMsg_DrawRatioX_Pipe(cursor, 1, px, py, gSzDuel_aicolMsg_bytes, SzMsg_DrawRect_Pipe, sx, sy);
    SzMsg_Free(1);
    return cursor;
}
