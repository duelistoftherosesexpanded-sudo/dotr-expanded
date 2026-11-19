#include "dotr/ee/camp_teki_window.h"

/* The work area by offset, as in the target (literal addresses). The base is the literal
 * 0x1ed0000 under __MWERKS__ and the host fixture otherwise (camp_teki_window.h). */
#define WB(o) (*(signed char *)(DOTR_TEKI_WINDOW_BASE + (o)))
#define WH(o) (*(short *)(DOTR_TEKI_WINDOW_BASE + (o)))
#define WU(o) (*(unsigned short *)(DOTR_TEKI_WINDOW_BASE + (o)))
#define FACE DOTR_TEKI_WINDOW_FACE

/* Macro of this source only, for the leader's name line: drawn at full width below `lim`
 * dots, else squeezed to `limf`. */
#define RATIO(cur, xx, yy, lim, limf) \
    info = SzMsg_GetInfo(1); \
    if (info->w < lim) { \
        cursor = SzMsg_DrawRatioX_Pipe(cur, 1, xx, yy, 1.0f, 1.0f, col, SzMsg_DrawRect_Pipe); \
    } else { \
        cursor = SzMsg_DrawRatioX_Pipe(cur, 1, xx, yy, limf / (float)info->w, 1.0f, col, SzMsg_DrawRect_Pipe); \
    } \
    SzMsg_Free(1);

/* MATCHING DEVICE (each needed for the exact bytes; none is a claim about the original source):
 * - the entry order `names = ...; its = ...; disp = 0; sel = 0; name = 196; k = b198;` (the
 *   two structure copies are statements, not initialisers) and the short `k` that holds the
 *   location for the switch; the local declaration orders, also inside the blocks;
 * - `if (disp)` and `if (sel)` bare, `if (sel == 0)` explicit; `disp = sel` after `sel = 1`;
 * - location 11 with the byte at 0x1ed018c set: `if (c == 2) {...; side = 0;} else { side = 1;
 *   if (c == 1) ... }`;
 * - the window calls pass `side`, `x`, `y` and `y + 26` directly; the leader block alone goes
 *   through `int xs = x;` for the text positions;
 * - the name box's width as an int `wd` set in an if/else whose else arm reads the low half
 *   of the message width through `*(short *)&info->w`, then `(short)wd >> 1`; the length
 *   as `int len10 = (short)(len * 10)` compared as `(short)len10`;
 * - the tooltip block: the temporaries `cx`, `half`, `d`; `px = d; if (d <= 0) px = 0;`;
 *   `int tx = px`; the block-local `c` for the cursor after the flush; the clamp written as
 *   `if (tx < 128 && ty < 209) ty = 208; else if (ty < 121) ty = 120;`;
 * - the default of the leader-kind switch reads its code through an unsigned short view of
 *   the same template (DotrTekiWindowItemU). */
int Taku_Camp_TEKI_WINDOW(int cursor)
{
    struct DotrTekiWindowBoard *b = &data_00299e10_tekiwindow;
    struct DotrTekiWindowNames names;
    struct DotrTekiWindowPair its;
    short x;
    short y;
    short sel;
    short name;
    short msg;
    signed char side;
    short disp;
    short yoff;
    short k;
    unsigned char col[4];
    struct DotrTekiWindowInfo *info;

    names = data_0029aef0_tekiwindow;
    its = data_0029af50_tekiwindow;
    disp = 0;
    sel = 0;
    name = 196;
    k = b->b198;
    col[0] = col[1] = col[2] = 128;
    col[3] = 128;
    x = WB(0x5440) * -640 / 100;
    y = WH(0x6c) - 256;
    yoff = (WH(0x68) << 8) >> 4;
    if (WB(0x18c) == 1 && b->b198 == 11) {
        side = b->b0 ^ 1;
    } else if (b->b198 == 11) {
        side = b->b0 ^ 1;
    } else if (data_00298ba0_tekiwindow[b->b198].h10 == 8) {
        side = 0;
    } else {
        side = 1;
    }
    switch (k) {
    case 0: name = 215; msg = 237; break;
    case 1: msg = 240; break;
    case 2: name = 198; msg = 220; break;
    case 3: name = 205; msg = 227; break;
    case 4: name = 204; msg = 226; break;
    case 5: name = 200; msg = 222; break;
    case 6: name = 203; msg = 225; break;
    case 7: name = 206; msg = 228; break;
    case 8: name = 199; msg = 221; break;
    case 9: name = 202; msg = 224; break;
    case 10: name = 201; msg = 223; break;
    case 11:
        if (WB(0x18c) != 0) {
            if (WB(0x53c2) == 2) {
                name = 217; msg = 239;
                side = 0;
            } else {
                side = 1;
                if (WB(0x53c2) == 1) {
                    name = 216; msg = 238;
                } else {
                    name = 197; msg = 219;
                }
            }
        } else if (b->b0 == 0 && b->lv[11] >= 3) {
            if (WB(0x53c2) == 1) {
                name = 216; msg = 238;
            } else {
                name = 197; msg = 219;
            }
        } else if (b->b0 == 1) {
            name = 217; msg = 239;
        } else {
            name = 197; msg = 219;
        }
        break;
    case 12: name = 207; msg = 229; break;
    case 13: name = 208; msg = 230; break;
    case 14: name = 209; msg = 231; break;
    case 15: msg = 241; break;
    case 16: name = 210; msg = 232; break;
    case 17: msg = 242; break;
    case 18: name = 211; msg = 233; break;
    case 19: name = 212; msg = 234; break;
    case 20: name = 213; msg = 235; break;
    case 21: name = 214; msg = 236; break;
    default: name = 196; msg = 218; break;
    }
    b->h196 = name - 196;
    if (WH(0x6c) != 0) {
        if (WB(0x18c) != 0) {
            if (b->b198 == 1 || b->b198 == 15 || b->b198 == 17) {
                sel = 0;
                disp = 0;
            } else {
                sel = 1;
                disp = sel;
            }
        } else if (b->b0 == 0) {
            if (b->b198 == 0 || b->b198 == 1) {
                sel = 0;
                disp = 0;
            } else if (b->b198 == 11 && b->lv[b->b198] <= 0) {
                sel = 0;
                disp = 0;
            } else {
                switch (b->lv[b->b198]) {
                case 1:
                    disp = 1;
                    sel = 0;
                    break;
                case 2:
                case 3:
                case 4:
                case 5:
                    if (WB(0x53ab) != 0) {
                        sel = 0;
                    } else {
                        sel = 1;
                        if (b->b198 == 11 && WB(0x53c2) == 1 && b->lv[11] < 4) {
                            sel = 0;
                        }
                    }
                    disp = 1;
                    break;
                }
            }
        } else {
            if (b->b198 == 15 || b->b198 == 17) {
                sel = 0;
                disp = 0;
            } else if (b->b198 == 11 && b->lv[b->b198] <= 0) {
                sel = 0;
                disp = 0;
            } else {
                switch (b->lv[b->b198]) {
                case 1:
                    disp = 1;
                    sel = 0;
                    break;
                case 2:
                case 3:
                    if (WB(0x53ab) != 0) {
                        sel = 0;
                    } else {
                        sel = 1;
                    }
                    disp = 1;
                    break;
                }
            }
        }
        if (WB(0x537b) != 0 && WB(0x5440) == 100) {
            sel = 1;
            disp = sel;
        }
        if (WB(0x537a) != 0 && WB(0x537b) != 0 && WB(0x5440) < 100) {
            sel = 1;
            disp = sel;
        }
        if (disp) {
            int yy;
            short no;
            short cost;

            cursor = Taku_Obj_Set(cursor, &tex_bara_data2_tekiwindow[side], 0, y);
            cursor = Taku_Window_Set(cursor, &camp_window_Teki_data_tekiwindow[0], side, 0, 0, y, 0, 128, WH(0x224));
            cursor = Taku_Window_Set(cursor, &camp_window_Teki_data_tekiwindow[2], side, 0, 0, y, 0, 128, WH(0x224));
            yy = y - yoff;
            cursor = Taku_Window_Set(cursor, &camp_window_Teki_data_tekiwindow[3], side, 0, 0, yy, 1, 128, 0);
            no = b->h196 + 1;
            if (name == 217) {
                no = b->h196;
            }
            switch (WB(0x540a)) {
            case 1:
                FACE->id = 255;
                /* fall through */
            case 2:
                FACE->no = no;
                FACE->kind = data_002999cc_tekiwindow[no][WB(0x5468)];
                FACE->mode = 1;
                FACE->x = camp_window_Teki_data_tekiwindow[3].x;
                FACE->y = camp_window_Teki_data_tekiwindow[3].y + y - yoff;
                ShFace_Request(FACE);
                WB(0x540a) = 2;
                break;
            case 10:
                FACE->no = no;
                FACE->kind = data_002999cc_tekiwindow[no][WB(0x5468)];
                FACE->mode = 3;
                FACE->x = camp_window_Teki_data_tekiwindow[3].x;
                FACE->y = camp_window_Teki_data_tekiwindow[3].y + y - yoff;
                ShFace_Request(FACE);
                WB(0x540a) = 2;
                break;
            case 100:
                ShFace_Close(FACE);
                WB(0x540a) = 0;
                break;
            }
            cursor = Taku_FT4_Set(cursor, &data_002983a0_tekiwindow[2], 0, yy, 0);
            cursor = Taku_FT4_Set(cursor, &data_002983a0_tekiwindow[3], 0, yy, 0);
            cursor = Taku_FT4_Set(cursor, &data_002983a0_tekiwindow[side], 0, yy, 0);
            if (WB(0x5406) == 0) {
                SzData_SetupFromConstDeck(1, b->h196 + 26);
            }
            cost = WH(0x5444) = SzData_GetDeckCost(1, 0);
            cursor = Taku_Obj_Set(cursor, &data_00298320_tekiwindow[0], 0, y);
            cursor = SzDuel_DrawWindow_FontDigit_Pipe(cursor, 410, y + 26, 0, 0, 4, cost);
            cursor = SzMsg_LoadFont_Pipe(cursor, 1, name, 0, 22);
            cursor = RhText_SetTextureFlush(cursor);
            info = SzMsg_GetInfo(1);
            if (info->w < 324) {
                /* MATCHING DEVICE: the two ratios go through locals declared `sy, sx` and set
                 * by one chained assignment; the other calls pass the constants directly. */
                float sy;
                float sx;

                sy = sx = 1.0f;
                cursor = SzMsg_DrawRatioX_Pipe(cursor, 1, 53, y + 21, sx, sy, col, SzMsg_DrawRect_Pipe);
            } else {
                cursor = SzMsg_DrawRatioX_Pipe(cursor, 1, 53, y + 21, 324.0f / (float)info->w, 1.0f, col, SzMsg_DrawRect_Pipe);
            }
            SzMsg_Free(1);
        }
        if (sel == 0) {
            WB(0x5406) = -1;
        } else if (WB(0x5406) == 0) {
            WB(0x5406) = 1;
        }
        if (WB(0x537a) != 0 && WB(0x5440) == 100 && WB(0x5406) < 2) {
            WB(0x5406) = 1;
        }
        if (sel) {
            int id;
            int xs;
            short rank;
            int r;
            short tx;
            short ty;

            WB(0x5488) = 1;
            id = SzData_GetLeader(1, 0);
            cursor = Taku_Window_Set(cursor, &camp_window_Teki_data_tekiwindow[1], side, 0, x, y, 0, 128, WH(0x224));
            cursor = Taku_Window_Set(cursor, &camp_window_Teki_data_tekiwindow[4], side, 0, x, y, 0, 128, WH(0x224));
            cursor = Taku_Window_Set(cursor, &data_00297bca_tekiwindow, side, 0, x, y, 0, 128, WH(0x224));
            xs = x;
            cursor = SzDuel_DrawWindow_Font_Pipe(cursor, xs + 152, y + 63, 1, 1, 4, data_0030f102_tekiwindow, 0);
            cursor = SzDuel_DrawWindow_Font_Pipe(cursor, xs + 192, y + 63, 1, 1, 6, data_0030f1e8_tekiwindow, 0);
            switch ((SzDuel_GetCardConstInfo(id)->b0 & 0x1f) + 1) {
            case 1: teki_syurui_data_tekiwindow.code = 78; break;
            case 2: teki_syurui_data_tekiwindow.code = 88; break;
            case 3: teki_syurui_data_tekiwindow.code = 93; break;
            case 4: teki_syurui_data_tekiwindow.code = 79; break;
            case 5: teki_syurui_data_tekiwindow.code = 83; break;
            case 6: teki_syurui_data_tekiwindow.code = 85; break;
            case 7: teki_syurui_data_tekiwindow.code = 86; break;
            case 8: teki_syurui_data_tekiwindow.code = 87; break;
            case 9: teki_syurui_data_tekiwindow.code = 81; break;
            case 10: teki_syurui_data_tekiwindow.code = 82; break;
            case 11: teki_syurui_data_tekiwindow.code = 89; break;
            case 12: teki_syurui_data_tekiwindow.code = 84; break;
            case 13: teki_syurui_data_tekiwindow.code = 90; break;
            case 14: teki_syurui_data_tekiwindow.code = 91; break;
            case 15: teki_syurui_data_tekiwindow.code = 92; break;
            case 16: teki_syurui_data_tekiwindow.code = 80; break;
            case 17: teki_syurui_data_tekiwindow.code = 94; break;
            case 18: teki_syurui_data_tekiwindow.code = 95; break;
            case 19: teki_syurui_data_tekiwindow.code = 96; break;
            case 20: teki_syurui_data_tekiwindow.code = 97; break;
            case 21: teki_syurui_data_tekiwindow.code = 101; break;
            default: teki_syurui_data_tekiwindow.code = teki_syurui_tex_no_tekiwindow; break;
            }
            cursor = Taku_Obj_Set(cursor, (struct DotrTekiWindowItem *)&teki_syurui_data_tekiwindow, x, y);
            rank = SzData_GetRank(1, id);
            if (rank != 0) {
                r = rank - 1;
                cursor = Taku_Obj_Set(cursor, &tex_batti_data2_tekiwindow[(short)(r / 3)], x, y);
                switch ((short)(r % 3 + 1)) {
                case 1:
                    cursor = Taku_Obj_Set(cursor, &tex_hosi_data2_tekiwindow[2], x, y);
                    break;
                case 2:
                    cursor = Taku_Obj_Set(cursor, &tex_hosi_data2_tekiwindow[0], x, y);
                    cursor = Taku_Obj_Set(cursor, &tex_hosi_data2_tekiwindow[1], x, y);
                    break;
                case 3:
                    cursor = Taku_Obj_Set(cursor, &tex_hosi_data2_tekiwindow[0], x, y);
                    cursor = Taku_Obj_Set(cursor, &tex_hosi_data2_tekiwindow[1], x, y);
                    cursor = Taku_Obj_Set(cursor, &tex_hosi_data2_tekiwindow[2], x, y);
                    break;
                }
            }
            cursor = Taku_Obj_Set(cursor, &data_00298320_tekiwindow[1], x, y);
            cursor = Taku_Obj_Set(cursor, &data_00298320_tekiwindow[2], x, y);
            cursor = SzDuel_DrawWindow_FontDigit_Pipe(cursor, xs + 324, y + 73, 1, 0, 4, b->h66[b->h196]);
            cursor = SzDuel_DrawWindow_FontDigit_Pipe(cursor, xs + 421, y + 73, 1, 0, 4, b->h130[b->h196]);
            cursor = SzMsg_LoadFont_Pipe(cursor, 1, SzDuel_GetMsgName(id), 0, 22);
            cursor = RhText_SetTextureFlush(cursor);
            tx = (short)camp_window_Teki_data_tekiwindow[4].x + 4 + x;
            ty = (short)camp_window_Teki_data_tekiwindow[4].y + 2 + y;
            RATIO(cursor, tx, ty, 246, 246.0f)
        }
    }
    if (WB(0x5472) != 0) {
        short len;
        int len10;
        short tx;
        int wd;

        len = func_0019bef0(names.s[b->b198]);
        len10 = (short)(len * 10);
        cursor = SzMsg_LoadFont_Pipe(cursor, 1, 258, 0, 22);
        cursor = RhText_SetTextureFlush(cursor);
        info = SzMsg_GetInfo(1);
        if (info->w < (short)len10) {
            wd = len10 + 16;
        } else {
            wd = *(short *)&info->w + 16;
        }
        camp_window_Teki_data_tekiwindow[7].w = wd;
        tx = 320 - ((short)wd >> 1);
        cursor = Taku_Window_Set(cursor, &camp_window_Teki_data_tekiwindow[7], 2, 0, tx, 224, 0, 128, 64);
        cursor = SzMsg_DrawRatioX_Pipe(cursor, 1, tx + 8, 228, 1.0f, 1.0f, col, SzMsg_DrawRect_Pipe);
        SzMsg_Free(1);
    } else if (WB(0x5374) == 0 && WU(4) == 2 && WB(0x72) == 0 && WB(0x547a) == 0 && WB(0x6e) != 0
               && WB(0x5481) != 0 && !SzEffect_IsUse2(-1, -1) && WB(0x5376) != 0 && WB(0x5482) == 0) {
        int cx;
        short wd;
        int tx;
        int c;
        int d;
        short ty;
        int m;
        int half;
        short px;

        func_0019bef0(names.s[b->b198]);
        if (WB(0x5446) == 0) {
            m = msg;
        } else {
            m = 249;
            WB(0x5446) = 0;
        }
        cursor = SzMsg_LoadFont_Pipe(cursor, 1, m, 0, 22);
        c = RhText_SetTextureFlush(cursor);
        info = SzMsg_GetInfo(1);
        wd = *(short *)&info->w + 16;
        camp_window_Teki_data_tekiwindow[5].w = wd;
        cx = WH(0);
        half = wd >> 1;
        d = cx - half;
        px = d;
        if (d <= 0) {
            px = 0;
        }
        if (cx + half >= 480) {
            px = 480 - wd;
        }
        tx = px;
        ty = WH(2) + 32;
        if (tx < 128 && ty < 209) {
            ty = 208;
        } else if (ty < 121) {
            ty = 120;
        }
        if (ty >= 290) {
            ty = 290;
        }
        cursor = Taku_Window_Set(c, &camp_window_Teki_data_tekiwindow[5], 2, 0, px, ty, 0, 128, 64);
        cursor = SzMsg_DrawRatioX_Pipe(cursor, 1, tx + 8, ty + 4, 1.0f, 1.0f, col, SzMsg_DrawRect_Pipe);
        SzMsg_Free(1);
    }
    if ((b->b0 == 0 && b->lv[11] >= 3) || WB(0x18c) != 0) {
        if (b->b198 == 11) {
            short yo;
            short ys;

            yo = yoff;
            ys = y;
            cursor = Taku_G4_Set(cursor, &data_00298b1c_tekiwindow, 0, ys - yo);
            cursor = SzDuel_DrawWindow_Font_Pipe(cursor, data_00298b1c_tekiwindow.w[0] + 40,
                                                 data_00298b1c_tekiwindow.w[1] + y + 24 - yo, 1, 1, 6,
                                                 data_0030f1ef_tekiwindow, 0);
            cursor = Taku_Obj_Set(cursor, &its.it[0], data_00298b1c_tekiwindow.w[0] + 4,
                                  (short)(data_00298b1c_tekiwindow.w[1] + 24) + ys - yo);
            cursor = Taku_Obj_Set(cursor, &its.it[1], data_00298b1c_tekiwindow.w[2] - 26,
                                  (short)(data_00298b1c_tekiwindow.w[1] + 24) + ys - yo);
        }
    }
    return cursor;
}
