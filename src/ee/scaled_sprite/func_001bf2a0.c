#include "dotr/ee/scaled_sprite.h"

/* lSzDuel_DrawBuildDeck_Detail_Enchant_Pipe (0x1bf2a0): lists the enchant cards of monster
 * cardId in the deck detail panel. Nothing for an id of 683 or more; otherwise one name per
 * entry TaEnc_GetEnchantLst returns (count as a byte), a texture flush, then per entry a
 * 262 x 26 board and the name scaled to at most 254 wide.
 * The callees are reached through views (scaled_sprite.h) declared inside the function. */
DotrSpriteWord lSzDuel_DrawBuildDeck_Detail_Enchant_Pipe(DotrSpriteWord arg0, int cardId, int arg2, int arg3, int arg4, int arg5)
{
    int count;
    unsigned short list[24];
    int offset;
    int i;
    int y;
    int x;
    int *size;
    int id;
    float scale;

    offset = arg4 ? 0x280 : -0x140;
    if (cardId >= 0x2AB) {
        return arg0;
    } else {
        count = func_00249410_enchant(cardId & 0xFFFF, list);
        i = 0;
        if (i < count) {
            do {
                arg0 = SzMsg_LoadFont_Pipe(arg0, i + 1, SzDuel_GetMsgName(list[i]), 0, (i + 1) * 22);
                i++;
            } while (i < count);
        }
        arg0 = RhText_SetTextureFlush(arg0);
        i = 0;
        if (i < count) {
            y = arg3 + 0x30;
            x = arg2 + offset + 0x1C;
            do {
                size = func_001feab0_enchant(i + 1);
                scale = 1.0f;
                id = lSzDuel_DrawWindow_ExBoard_Pipe(arg0, arg5, x, y, -0x106, -0x1A, -1, 0);
                if (size[3] > 0xFE) {
                    scale = 254.0f / size[3];
                }
                arg0 = func_001fe660_enchant(id, i + 1, x + 5, y + 2, scale, 1.0f, (DotrSpriteAddr)gSzDuel_aicolMsg_bytes, SzMsg_DrawRect_Pipe);
                SzMsg_Free(i + 1);
                i++;
                y += 0x1A;
            } while (i < count);
        }
        return arg0;
    }
}
