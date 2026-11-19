#include "dotr/ee/camp_st_window.h"

/* The work area by offset, as in the target (literal addresses). The base is the literal
 * 0x1ed0000 under __MWERKS__ and the host fixture otherwise (camp_st_window.h). */
#define ST_MODE (*(signed char *)(DOTR_ST_WINDOW_BASE + 0x18c))
#define ST_SLIDE (*(short *)(DOTR_ST_WINDOW_BASE + 0x70))
#define ST_OPEN (*(short *)(DOTR_ST_WINDOW_BASE + 0x6c))
#define ST_LOC11 (*(signed char *)(DOTR_ST_WINDOW_BASE + 0x53c2))
#define ST_DEPTH (*(short *)(DOTR_ST_WINDOW_BASE + 0x226))
#define ST_COST (*(short *)(DOTR_ST_WINDOW_BASE + 0x5442))

/* MATCHING DEVICE (each needed for the exact bytes; none is a claim about the original source):
 * - the local declaration order, with the function-level floats `sx` and `sy` after `info`;
 * - the first branch of the final test written `sy = sx = 1.0f;` and passed as `(sx, sy)`
 *   (`mtc1 v0,$f13` then `mov.s $f12,$f13`; the same form as state_grid's 0x1dc560), the
 *   second branch with the literal ratio expressions;
 * - the halfword at 0x1ed0226 read once through the base pointer `g` (the three side
 *   windows) and once by its literal address (window 16);
 * - the side test compares `w->mode == (signed char)t`; the rank switch on
 *   `(short)(r % 3 + 1)` and the template index `(short)(r / 3)`. */
int Taku_Camp_ST_WINDOW(int cursor)
{
    struct DotrStWindowBoard *w;
    struct DotrStWindowBank *g;
    unsigned char col[4];
    short y;
    int t;
    int i;
    short deck;
    short cost;
    int leader;
    short rank;
    int r;
    short j;
    short k;
    short cnt;
    int n;
    unsigned short x;
    short xx, yy;
    struct DotrStWindowInfo *info;
    float sx;
    float sy;

    w = &data_00299e10_stwindow;
    y = 128 - (ST_SLIDE * 128 >> 8);
    col[0] = col[1] = col[2] = 128;
    col[3] = 128;
    g = (struct DotrStWindowBank *)DOTR_ST_WINDOW_BASE;
    if (ST_MODE) {
        if (w->kind == 11) {
            if (ST_LOC11 < 2) { t = 1; } else { t = 0; }
        } else if (data_00298ba0_stwindow[w->kind].kind == 8) {
            t = 0;
        } else {
            t = 1;
        }
        if (w->mode == (signed char)t) {
            cursor = Taku_Obj_Set(cursor, &tex_bara_data_stwindow[w->mode], 0, y);
            for (i = 0; i < 3; i++) {
                cursor = Taku_Window_Set(cursor, &camp_window_My_data_stwindow[i], w->mode + 10, 0, 0, y, 0, 128, g->depth);
            }
            if (ST_OPEN == 256) {
                w->mode ^= 1;
            }
        } else {
            cursor = Taku_Obj_Set(cursor, &tex_bara_data_stwindow[w->mode], 0, y);
            for (i = 0; i < 3; i++) {
                cursor = Taku_Window_Set(cursor, &camp_window_My_data_stwindow[i], w->mode, 0, 0, y, 0, 128, g->depth);
            }
        }
    } else {
        cursor = Taku_Obj_Set(cursor, &tex_bara_data_stwindow[w->mode], 0, y);
        for (i = 0; i < 3; i++) {
            cursor = Taku_Window_Set(cursor, &camp_window_My_data_stwindow[i], w->mode, 0, 0, y, 0, 128, g->depth);
        }
    }
    cursor = Taku_Window_Set(cursor, &camp_window_My_data_stwindow[16], w->mode, 0, 0, y, 0, 128, ST_DEPTH);
    deck = SzData_GetCurrentDeck(0);
    cursor = Taku_Obj_Set(cursor, &tex_deck_data_stwindow[deck], 0, y);
    cost = SzData_GetDeckCost(0, deck);
    ST_COST = cost;
    leader = SzData_GetLeader(0, deck);
    rank = SzData_GetRank(0, leader);
    if (rank != 0) {
        r = rank - 1;
        cursor = Taku_Obj_Set(cursor, &tex_batti_data_stwindow[(short)(r / 3)], 0, y);
        switch ((short)(r % 3 + 1)) {
        case 1:
            cursor = Taku_Obj_Set(cursor, &tex_hosi_data_stwindow[2], 0, y);
            break;
        case 2:
            cursor = Taku_Obj_Set(cursor, &tex_hosi_data_stwindow[0], 0, y);
            cursor = Taku_Obj_Set(cursor, &tex_hosi_data_stwindow[1], 0, y);
            break;
        case 3:
            cursor = Taku_Obj_Set(cursor, &tex_hosi_data_stwindow[0], 0, y);
            cursor = Taku_Obj_Set(cursor, &tex_hosi_data_stwindow[1], 0, y);
            cursor = Taku_Obj_Set(cursor, &tex_hosi_data_stwindow[2], 0, y);
            break;
        }
    }
    for (j = 0; j < 2; j++) {
        if (w->mode == 0) {
            if (j == 1) {
                for (n = 0; n < 8; n++) {
                    x = n * 16;
                    cursor = Taku_Obj_Set(cursor, &camp_card_stwindow[j], x, y);
                }
            } else {
                for (k = 0, cnt = 0; k <= 21; k++) {
                    switch (k) {
                    case 2:
                    case 4:
                    case 5:
                    case 6:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                        if (w->seen[k] >= 3) {
                            cnt++;
                        }
                        break;
                    }
                }
                for (n = 0; n < cnt; n++) {
                    x = n * 16;
                    cursor = Taku_Obj_Set(cursor, &camp_card_stwindow[j], x, y);
                }
            }
        } else {
            if (j == 0) {
                for (n = 0; n < 8; n++) {
                    x = n * 16;
                    cursor = Taku_Obj_Set(cursor, &camp_card_stwindow[j], x, y);
                }
            } else {
                for (k = 0, cnt = 0; k <= 21; k++) {
                    switch (k) {
                    case 12:
                    case 13:
                    case 14:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 0:
                        if (w->seen[k] == 3) {
                            cnt++;
                        }
                        break;
                    }
                }
                for (n = 0; n < cnt; n++) {
                    x = n * 16;
                    cursor = Taku_Obj_Set(cursor, &camp_card_stwindow[j], x, y);
                }
            }
        }
    }
    for (i = 0; i < 4; i++) {
        cursor = Taku_Obj_Set(cursor, &camp_window_data_stwindow[i], 0, y);
    }
    cursor = SzDuel_DrawWindow_FontDigit_Pipe(cursor, 157, y + 384, 1, 0, 4, cost);
    cursor = SzDuel_DrawWindow_FontDigit_Pipe(cursor, 253, y + 384, 1, 0, 4, w->wins[0]);
    cursor = SzDuel_DrawWindow_FontDigit_Pipe(cursor, 349, y + 384, 1, 0, 4, w->wins[1]);
    cursor = SzDuel_DrawWindow_Font_Pipe(cursor, 411, y + 375, 1, 1, 4, data_0030f0f7_stwindow, 0);
    cursor = SzDuel_DrawWindow_Font_Pipe(cursor, 450, y + 375, 1, 1, 5, data_0030f0fc_stwindow, 0);
    cursor = SzMsg_LoadFont_Pipe(cursor, 1, 3, 0, 22);
    cursor = RhText_SetTextureFlush(cursor);
    cursor = SzMsg_DrawRatioX_Pipe(cursor, 1, 54, y + 348, 1.0f, 1.0f, col, SzMsg_DrawRect_Pipe);
    SzMsg_Free(1);
    cursor = SzMsg_LoadFont_Pipe(cursor, 1, SzDuel_GetMsgName(leader), 0, 22);
    cursor = RhText_SetTextureFlush(cursor);
    xx = camp_window_My_data_stwindow[2].x + 4;
    yy = camp_window_My_data_stwindow[2].y + 2 + y;
    info = SzMsg_GetInfo(1);
    if (info->w < 248) {
        sy = sx = 1.0f;
        cursor = SzMsg_DrawRatioX_Pipe(cursor, 1, xx, yy, sx, sy, col, SzMsg_DrawRect_Pipe);
    } else {
        cursor = SzMsg_DrawRatioX_Pipe(cursor, 1, xx, yy, 248.0f / (float)info->w, 1.0f, col, SzMsg_DrawRect_Pipe);
    }
    SzMsg_Free(1);
    return cursor;
}
