#include "dotr/ee/scaled_sprite.h"

/* The record is the header's DotrSpriteAbilityItem (8 header bytes as two words, then the
 * point pairs and the owner byte). SzMsg_DrawRatioX_Pipe is declared in the body under the view
 * name func_001fe660_ability, because the header's assumed prototype orders the parameters
 * differently. */

#ifndef __MWERKS__
/* Host build only: this C passes the icon row as an unsigned short pointer and the
 * colour as a 32-bit word, as the target compiler accepts. */
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
DotrSpriteWord lSzDuel_DrawBuildDeck_Detail_Ability_Pipe(DotrSpriteWord arg0, int cardNo, int arg2, int arg3, int arg4,
                                                         int arg5)
{
    extern int func_001fe660_ability(int, int, int, int, float, float, int, void (*)(void));
    unsigned char flag;
    int count = 0;
    int j;
    int offset;
    int i;
    struct DotrSpriteAbilityItem state;
    int width;
    float scale;
    int item;
    int x;
    int n;
    int pipe;
    int y;
    int ty;
    int tx;

    flag = DeckBuildingSide_ability;
    offset = arg4 != 0 ? 0x280 : -0x140;
    if (cardNo >= 0x2AB) {
        return arg0;
    } else {
        func_001dcd70_ability(&state, cardNo);
        state.owner = flag;
        j = 0;
        do {
            state.pair[j] = EffIdx_L_ability[cardNo][j];
            j++;
        } while (j < 5);
        i = 4;
        do {
            item = i - 4;
            arg0 = SzMsg_LoadFont_Pipe(arg0, item + 1, item + 0x65, 0, (item + 1) * 22);
            i++;
        } while (i < 0x11);
        pipe = RhText_SetTextureFlush(arg0);
        i = 4;
        y = arg3 + 0x751;
        ty = arg3 + 0x30;
        x = arg2 + 0x6DC + offset;
        tx = arg2 + offset + 0x36;
        do {
            n = i - 4;
            if (func_001e0c80_ability(&state, i, 0, 1)) {
                arg0 = lSzDuel_DrawWindow_ExBoard_Pipe(lSzDuel_DrawWindow_ExIcon_Pipe(pipe, (unsigned short *)&gSzDuel_Icon_bytes[(i + 0x1E) * 10], x, y,
                                                   0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80),
                                     arg5, tx, ty, -0xEC, -0x1A, -1, 0);
                width = *(int *)((unsigned char *)SzMsg_GetInfo(n + 1) + 0xC);
                if ((width < 0xE3) ^ 1) {
                    scale = 226.0f / (float)width;
                } else {
                    scale = 1.0f;
                }
                pipe = func_001fe660_ability(arg0, n + 1, tx + 5, ty + 2,
                                    scale, 1.0f, (int)gSzDuel_aicolMsg_bytes, SzMsg_DrawRect_Pipe);
                y += 0x1A;
                ty += 0x1A;
                count++;
            }
            SzMsg_Free(n + 1);
            i++;
        } while (i < 0x11);
    }
    return pipe;
}
