#include "dotr/ee/deck_detail.h"

/* 0x1bf710. The callees are declared inside the function body. Compiled with #pragma
 * optimization_level 3, set directly before the function and reset after it. */
#ifndef __MWERKS__
/* Host build only: the code passes addresses as 32-bit words and mixes char signedness and
 * pointer types, which the target compiler accepts and GCC would reject as errors. */
#pragma GCC diagnostic ignored "-Wpointer-sign"
#pragma GCC diagnostic ignored "-Wint-conversion"
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
#ifdef __MWERKS__
#pragma optimization_level 3
#endif
int lSzDuel_DrawBuildDeck_Library_Pipe(int cursor, int x, int y)
{
    extern int SzDuel_DrawWindow_FontDigit_Pipe();
    extern int lSzDuel_DrawWindow_ExIcon_Pipe(int, unsigned char *, int, int, int, int, int, int, int, int, int, int, int, unsigned char);
    extern int SzDuel_DrawWindow_Font_Pipe();
    DotrDeckPaneDuel317490 *c;
    int side;
    int show;
    char *text;
    char *back;
    int i;
    int j;
    int row;
    int col;
    int id;
    unsigned char *info;
    unsigned char alpha;
    int n;
    int type;
    int bx;
    int ox;
    int gx;
    int gy;
    int px;
    int qx;
    int qy;
    int k;
    int py;
    int cur;
    int icon;
    int last;

    c = &data_00317490_deckpane;
    show = 1;
    side = c->s.player;
    show = (c->s.mode == 0) ? show : 0;
    text = data_002bef93_deckpane + 0x30;
    back = data_002bef93_deckpane + 0;
    bx = x - 640;
    px = bx + 80;
    py = y + 16;
    cursor = lSzDuel_DrawWindow_Frame_Pipe(cursor, 12, -1, 11, px, py, -192, -48, 1);
    px = bx + 176;
    py = y + 31;
    cur = SzDuel_DrawWindow_Font_Pipe(cursor, px, py, 2, 3, func_0019bef0(text), text, 0);
    px = bx + 16;
    py = y + 16;
    cur = lSzDuel_DrawWindow_Frame_Pipe(cur, 12, -1, 5, px, py, -64, -20, 1);
    px = bx + 1746;
    py = y + 1842;
    icon = lSzDuel_DrawWindow_ExIcon_Pipe(cur, gSzDuel_Icon_deckpane + 2030, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, 128);
    px = bx + 39;
    py = y + 20;
    cur = SzDuel_DrawWindow_FontDigit_Pipe(icon, px, py, 1, 0, 4, SzData_GetBagCountAll(side));
    px = bx + 16;
    py = y + 36;
    cur = lSzDuel_DrawWindow_ExBoard_Pipe(cur, 2, px, py, -64, -28, -1, 0);
    if (show) {
        px = bx + 1749;
        py = y + 1869;
        cur = lSzDuel_DrawWindow_ExIcon_Pipe(cur, gSzDuel_Icon_deckpane + 2600, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, 128);
    }
    px = bx + 1772;
    py = y + 1868;
    cur = lSzDuel_DrawWindow_ExIcon_Pipe(cur, gSzDuel_Icon_deckpane + 1960, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, 128);
    px = bx + 8;
    py = y + 64;
    cur = lSzDuel_DrawWindow_Frame_Pipe(cur, 12, -1, 7, px, py, -288, -318, 1);
    for (i = 0; i < 10; i++) {
        cur = SzDuel_DrawWindow_FontDigit_Pipe(cur, bx + 55 + i * 24, y + 69, 1, 0, 1, i);
    }
    qx = bx + 15;
    qy = y + 91;
    for (k = 0; k < 10; k++) {
        cur = SzDuel_DrawWindow_FontDigit_Pipe(cur, qx, qy, 1, 32, 3, (c->s.digit[0] + k) * 10);
        qy += 28;
    }
    px = bx + 1778;
    gy = y + 1907;
    for (row = 0; row < 10; row++) {
        gx = px;
        for (col = 0; col < 10; col++, gx += 24) {
            id = col + (c->s.digit[0] + row) * 10;
            alpha = 0;
            if (id == 671) {
                if (SzData_GetLookFlag(side, id)) {
                    cur = lSzDuel_DrawWindow_ExIcon_Pipe(cur, gSzDuel_Icon_deckpane + 2200, gx, gy,
                                             0, 1, 0, 0, 0, 0, 1, 0, -1, 48);
                }
                continue;
            }
            if ((info = SzDuel_GetCardConstInfo(id)) == 0) {
                continue;
            }
            n = SzData_GetBagCount(side, id, -1);
            if (n < 0) {
                continue;
            }
            if (n > 0) {
                alpha = 128;
            } else if (SzData_GetLookFlag(side, id)) {
                alpha = 48;
            }
            if (alpha) {
                type = SzDuel_Kind2Type(info[0] >> 5);
                if (type == 0 && *(unsigned short *)(info + 4) != 0xffff) {
                    type = 4;
                }
                cur = lSzDuel_DrawWindow_ExIcon_Pipe(cur, gSzDuel_Icon_deckpane + (type + 220) * 10, gx,
                                         gy, 0, 1, 0, 0, 0, 0, 1, 0, -1, alpha);
            }
        }
        gy += 28;
    }
    px = bx + 45;
    py = y + 78;
    cur = lSzDuel_DrawWindow_Frame_Pipe(cur, -1, -1, 6, px + c->s.digit[1] * 24, py + c->s.digit[2] * 28, -24, -30, 1);
    for (i = 0; i < 10; i++) {
        cur = SzDuel_DrawWindow_FontDigit_Pipe(cur, bx + 55 + i * 24, y + 365, 1, 0, 1, i);
    }
    px = bx + 296;
    py = y + 64;
    cur = lSzDuel_DrawWindow_Frame_Pipe(cur, 12, -1, 12, px, py, 1, -318, 1);
    px = bx + 296;
    py = y + 80 + c->s.digit[0] * 238 / 76;
    cur = lSzDuel_DrawWindow_Frame_Pipe(cur, -1, -1, 13, px, py, 1, 1, 1);
    px = bx + 16;
    py = y + 382;
    cur = lSzDuel_DrawWindow_ExBoard_Pipe(cur, 2, px, py, -236, -50, -1, 0);
    px = bx + 1772;
    py = y + 2210;
    cur = lSzDuel_DrawWindow_ExIcon_Pipe(cur, gSzDuel_Icon_deckpane + 1970, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, 128);
    if (show) {
        px = bx + 1749;
        py = y + 2215;
        cur = lSzDuel_DrawWindow_ExIcon_Pipe(cur, gSzDuel_Icon_deckpane + 2610, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, 128);
        px = bx + 1751;
        py = y + 2234;
        last = lSzDuel_DrawWindow_ExIcon_Pipe(cur, gSzDuel_Icon_deckpane + 2540, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, 128);
        px = bx + 44;
        py = y + 413;
        cur = SzDuel_DrawWindow_Font_Pipe(last, px, py, 1, 0, func_0019bef0(back), back, 0);
    }
    return cur;
}
