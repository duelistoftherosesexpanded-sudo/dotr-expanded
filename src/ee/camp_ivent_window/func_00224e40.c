#include "dotr/ee/camp_ivent_window.h"

/* 0x224e40. Its two switch tables (0x30eca0 and 0x30ed00) are compared as one range. Compiled with
 * #pragma opt_dead_assignments off, set directly before the function and reset after it. Callees
 * and data are declared inside the function body; the work area is addressed by its literal
 * through AT. */
#ifndef __MWERKS__
/* Host build only: the code turns the int cursor into a pointer and back, leaves some locals
 * unset on some paths, passes a callback of another type and falls through a case, which the
 * target compiler accepts. */
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#pragma GCC diagnostic ignored "-Wpointer-sign"
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#endif

/* MATCHING DEVICE: the work area by its literal addresses (camp_ivent_window.h). */
#define AT(a) DOTR_IVENT_WINDOW_AT(a)

#ifdef __MWERKS__
#pragma opt_dead_assignments off
#endif

int Taku_Camp_Ivent_Window(int arg0) {
    short ny;
    extern int Taku_Camp_Ivent_Name();
    extern int Taku_Window_Set(int, void *, int, int, unsigned short, unsigned short, int, int, int);
    extern int Taku_Obj_Set(int, void *, short, short);
    extern int Taku_Bunsyou_Set(int, int, int, short, short, float, float, int, void *);
    extern int SzMsg_LoadFont_Pipe(int, int, int, int, int);
    extern int Taku_FT4_Set(int, void *, short, short, unsigned short);
    extern int SzMsg_DrawRatioX_Pipe(int, int, int, int, float, float, void *, void (*)(void));
    short flag2;
    signed char *ctx;
    short i;
    DotrIventWinState *state;
    short x;
    short y;
    short w;
    short h;
    short nx;
    int right;
    int left;
    int t;
    int xx;
    short j;
    short n;
    short m;
    short k2;
    DotrIventWinSprite *tmpl;
    short k3;
    short i2;
    int xx3;
    int pos;
    signed char v;
    short cur;
    short flag;
    short flip;
    short k1;
    short k;
    int id;
    int q;
    int width;
    signed char sel;
    signed char mode;
    DotrIventWinCode *code;
    float sx;
    float sy;

    unsigned short digits[3][4];
    unsigned short types[3][4];
    DotrIventWinSprite s100;
    DotrIventWinPair pair130;
    DotrIventWinSprite s180;
    DotrIventWinPair pair1B0;
    DotrIventWinSprite s200;
    DotrIventWinSprite s230;
    unsigned short hundreds[3];
    unsigned short ones[3];
    DotrIventWinFrame frame;
    DotrIventWinColor color;

    ctx = (signed char *)&data_00299e10_iventwin;
    state = (DotrIventWinState *)AT(0x01ED0000);
    frame = data_0029aa08_iventwin;
    s100 = data_0029aa20_iventwin;
    pair130 = data_0029aa50_iventwin;
    s180 = data_0029aaa0_iventwin;
    pair1B0 = data_0029aad0_iventwin;
    s200 = data_0029ab20_iventwin;
    color.r = color.g = color.b = 0x80;
    color.a = 0x80;
    if (CutSceneNumber_iventwin == 0x2738 && *(signed char *)AT(0x01ED53A6) != 0) {
        arg0 = Taku_Camp_Ivent_Name(arg0);
    } else if (*(signed char *)AT(0x01ED53A6) != 0) {
        switch (*(short *)(0xC2 + ctx)) {
        case 0x2738:
            x = 0x1B;
            y = 0x28;
            w = 0x24A;
            h = 0x180;
            break;
        case 0x276A:
        case 0x2742:
            x = 0x5B;
            y = 0x93;
            w = 0x1CA;
            h = 0x9A;
            break;
        case 0x2756:
            x = 0x1B;
            y = 0xAB;
            w = 0x24A;
            h = 0x8A;
            break;
        }
        frame.kind = 2;
        frame.x1 = ((w - 0x10) * *(signed char *)AT(0x01ED53A7)) / 100 + 0x10;
        frame.y1 = ((h - 0x10) * *(signed char *)AT(0x01ED53A7)) / 100 + 0x10;
        arg0 = Taku_Window_Set(arg0, &frame, 2, 0, x, y, 0, 0x80, 0x40);
        if (*(signed char *)AT(0x01ED53A6) != 0) {
            t = (*(signed char *)AT(0x01ED53A7) * 0x280) / 100;
            nx = x + w - 0x8B;
            right = 0x280 - t;
            ny = y + h - 0x2A;
            frame.kind = 9;
            frame.x1 = 0x82;
            frame.y1 = 0x22;
            left = t - 0x280;
            arg0 = Taku_Window_Set(arg0, &frame, 3, 0, nx + right, ny, 0, 0x80, 0x40);
            arg0 = Taku_Obj_Set(arg0, &pair1B0, nx + 0x1B + right, ny + 9);
            arg0 = Taku_Bunsyou_Set(arg0, 1, 0xA7, nx + 0x3C + right, 6 + ny, 1.0f, 1.0f, 0, &color);
            switch (*(short *)(0xC2 + ctx)) {
            case 0x2738:
            case 0x276A:
                if (*(signed char *)AT(0x01ED53A6) == 2) {
                    tmpl = tex_bara_data;
                    for (i = 0; i < 2; i++) {
                        s100 = *tmpl;
                        if (i == 0) {
                            s100.x0 = 0x90;
                            s100.y0 = 0xE0;
                        } else {
                            s100.x0 = 0x90;
                            s100.y0 = 0xB0;
                        }
                        s100.x1 = s100.x0 + 0x20;
                        s100.y1 = s100.y0 + 0x20;
                        arg0 = Taku_Obj_Set(arg0, &s100, 0, 0);
                        tmpl++;
                    }
                }
                frame.x1 = 0x140;
                frame.y1 = 0x21;
            case 0x2742:
                if (*(short *)(ctx + 0xC2) == 0x2742) {
                    x = 0x97;
                } else {
                    x = 0xB0;
                }
                sel = *(signed char *)AT(0x01ED53A8);
                t = (short)((sel == 0) ? 0xB0 : 0xE0);
                frame.kind = 8;
                y = t;
                if (*(short *)(0xC2 + ctx) == 0x2742) {
                    frame.x1 = 0x148;
                    frame.y1 = 0x21;
                }
                if (*(signed char *)AT(0x01ED53A6) == 2) {
                    arg0 = Taku_Window_Set(arg0, &frame, 3, 0, x + left, y, 0, 0x80, 0x40);
                }
                if (*(signed char *)AT(0x01ED53A6) == 2) {
                    for (i2 = 0; i2 < 2; i2++) {
                        if (i2 == state->sel) {
                            color.r = color.g = color.b = 0x80;
                            color.a = 0x80;
                        } else {
                            color.r = color.g = color.b = 0x40;
                            color.a = 0x80;
                        }
                        if (*(short *)(0xC2 + ctx) == 0x2742) {
                            x = 0x97;
                        } else {
                            x = 0xB0;
                        }
                        if (i2 == 0) {
                            sy = sx = 1.0f;
                            if (*(short *)(0xC2 + ctx) == 0x276A) {
                                id = 0x100;
                            } else {
                                id = 0xFD;
                            }
                            arg0 = Taku_Bunsyou_Set(arg0, 1, id, x + 8 + left, 0xB4, sx, sy, 0, &color);
                        } else {
                            if (*(short *)(ctx + 0xC2) == 0x276A) {
                                id = 0x101;
                            } else {
                                id = 0xFE;
                            }
                            sy = sx = 1.0f;
                            arg0 = Taku_Bunsyou_Set(arg0, 1, id, x + 8 + right, 0xE4, sx, sy, 0, &color);
                        }
                    }
                }
                color.r = color.g = color.b = 0x80;
                color.a = 0x80;
                break;
            case 0x2756:
            case 0x2757:
            case 0x2760:
                for (n = 0; n < 3; n++) {
                    hundreds[n] = data_00299d00_iventwin[state->codes[n]].number / 100;
                    ones[n] = data_00299d00_iventwin[state->codes[n]].number % 100;
                    for (m = 0; m < 4; m++) {
                        digits[n][m] = ((int)data_00299d00_iventwin[state->codes[n]].digits >> (m * 3)) & 7;
                        types[n][m] = (data_00299d00_iventwin[state->codes[n]].types >> (5 * m)) & 0x1F;
                    }
                }
                if (*(signed char *)AT(0x01ED5494) == 0) {
                    switch (v = *(signed char *)AT(0x01ED5495)) {
                    case 0:
                        cur = 2;
                        break;
                    default:
                        cur = v;
                        break;
                    }
                } else {
                    cur = *(signed char *)AT(0x01ED53A8);
                }
                frame.kind = 1;
                frame.x1 = 0xF0;
                frame.y1 = 0x20;
                arg0 = Taku_Window_Set(arg0, &frame, 2, 0, left + 0xA, 0x8B, 0, 0x80, 0x40);
                sx = 1.0f;
                arg0 = Taku_Bunsyou_Set(arg0, 1, 0xFF, left + 0x10, 0x91, sx, sx, 0, &color);
                arg0 = SzDuel_DrawWindow_Font_Pipe(arg0, left + 0x2E, 0xE6, 1, 0, 0xB, data_0030f0bb_iventwin, NULL);
                arg0 = SzDuel_DrawWindow_Font_Pipe(arg0, left + 0x52, 0xF5, 1, 0, 0xC, data_0030f0c7_iventwin, NULL);
                arg0 = SzDuel_DrawWindow_Font_Pipe(arg0, right + 0x1A2, 0xB5, 1, 0, 0x13, data_0030f0d4_iventwin, NULL);
                arg0 = SzDuel_DrawWindow_Font_Pipe(arg0, right + 0x1A2, 0xDD, 1, 0, 0xE, data_0030f0e8_iventwin, NULL);

                for (n = 0, xx3 = left; n <= hundreds[cur]; n++, xx3 += 0x10) {
                    if (n < hundreds[cur]) {
                        arg0 = Taku_Obj_Set(arg0, &pair130.s[0], xx3, 0);
                    } else {
                        tex_uv_data[pair130.s[1].index].width = (ones[cur] * 16) / 100;
                        pair130.s[1].x1 = pair130.s[1].x0 + tex_uv_data[pair130.s[1].index].width;
                        arg0 = Taku_Obj_Set(arg0, &pair130.s[1], xx3, 0);
                    }
                }
                arg0 = Taku_Camp_Kazu_Disp(arg0, hundreds[cur], ones[cur], left);
                for (n = 0; n < 4; n++) {
                    flag = 1;
                    switch (digits[cur][n]) {
                    case 1:
                        s180.index = 0x8A;
                        break;
                    case 2:
                        s180.index = 0x8F;
                        break;
                    case 3:
                        s180.index = 0x8C;
                        break;
                    case 4:
                        s180.index = 0x8D;
                        break;
                    case 5:
                        s180.index = 0x8B;
                        break;
                    case 6:
                        s180.index = 0x8E;
                        break;
                    default:
                        flag = 0;
                        break;
                    }
                    if (flag) {
                        arg0 = Taku_Obj_Set(arg0, &s180, right + (0x208 - n * 0x20), 0xC4);
                    }
                }
                for (k3 = 0; k3 < 4; k3++) {
                    flag2 = 1;
                    switch (types[cur][k3]) {
                    case 1:
                        s180.index = 0x4E;
                        break;
                    case 2:
                        s180.index = 0x58;
                        break;
                    case 3:
                        s180.index = 0x5D;
                        break;
                    case 4:
                        s180.index = 0x4F;
                        break;
                    case 5:
                        s180.index = 0x53;
                        break;
                    case 6:
                        s180.index = 0x55;
                        break;
                    case 7:
                        s180.index = 0x56;
                        break;
                    case 8:
                        s180.index = 0x57;
                        break;
                    case 9:
                        s180.index = 0x51;
                        break;
                    case 10:
                        s180.index = 0x52;
                        break;
                    case 11:
                        s180.index = 0x59;
                        break;
                    case 12:
                        s180.index = 0x54;
                        break;
                    case 13:
                        s180.index = 0x5A;
                        break;
                    case 14:
                        s180.index = 0x5B;
                        break;
                    case 15:
                        s180.index = 0x5C;
                        break;
                    case 16:
                        s180.index = 0x50;
                        break;
                    case 17:
                        s180.index = 0x5E;
                        break;
                    case 18:
                        s180.index = 0x5F;
                        break;
                    case 19:
                        s180.index = 0x60;
                        break;
                    case 20:
                        s180.index = 0x61;
                        break;
                    case 21:
                        s180.index = 0x65;
                        break;
                    default:
                        flag2 = 0;
                        break;
                    }
                    if (k3 == 3) {
                        s200.index = s180.index;
                    }
                    if (flag2) {
                        arg0 = Taku_Obj_Set(arg0, &s180, right + (0x208 - k3 * 0x20), 0xEB);
                    }
                }
                for (k = 0, xx = 0xE0; k < 2; k++, xx += 0x94) {
                    s230 = data_0029ab50_iventwin;
                    flip = 0;
                    if (1 == k) {
                        s230.x0 = 0x18;
                        s230.y0 = 0;
                        s230.x1 = 0;
                        s230.y1 = 0;
                        s230.x2 = 0x18;
                        s230.y2 = 0x18;
                        s230.x3 = 0;
                        s230.y3 = 0x18;
                        flip = 1;
                    }
                    mode = state->mode;
                    if (mode == 0) {
                        if (k == 0) {
                            arg0 = Taku_FT4_Set(arg0, &s230, left + xx, 0xF1, flip);
                        } else {
                            arg0 = Taku_FT4_Set(arg0, &s230, right + xx, 0xF1, flip);
                        }
                    } else if (mode == 1 && k == 0) {
                        arg0 = Taku_FT4_Set(arg0, &s230, xx, 0xF1, flip);
                    } else if (mode == 2 && k == 1) {
                        arg0 = Taku_FT4_Set(arg0, &s230, xx, 0xF1, flip);
                    }
                }
                frame.kind = 9;
                frame.x1 = 0xD5;
                frame.y1 = 0x22;
                arg0 = Taku_Window_Set(arg0, &frame, 1, 0, 0x22 + left, 0xB2, 0, 0x80, 0x40);
                arg0 = Taku_Obj_Set(arg0, &s200, 0x27 + left, 0xB7);
                q = RhText_SetTextureFlush(SzMsg_LoadFont_Pipe(arg0, 1, SzDuel_GetMsgName(state->values[cur]), 0, 0x16));
                width = *(int *)((unsigned char *)SzMsg_GetInfo(1) + 0xC);
                if (width <= 0xA9) {
                    sy = sx = 1.0f;
                    arg0 = SzMsg_DrawRatioX_Pipe(q, 1, left + 0x45, 0xB7, sx, sy, &color, SzMsg_DrawRect_Pipe);
                } else {
                    arg0 = SzMsg_DrawRatioX_Pipe(q, 1, left + 0x45, 0xB7, 170.0f / width, 1.0f, &color, SzMsg_DrawRect_Pipe);
                }
                SzMsg_Free(1);
                break;
            }
        }
        if (*(signed char *)AT(0x01ED53A6) == 1) {
            *(signed char *)AT(0x01ED53A7) += 6;
            if (*(signed char *)AT(0x01ED53A7) >= 100) {
                *(signed char *)AT(0x01ED53A7) = 100;
                *(signed char *)AT(0x01ED53A6) = 2;
            }
        }
        switch (*(short *)(ctx + 0xC2)) {
        case 0x2738:
        case 0x276A:
        case 0x2742:
            if (*(signed char *)AT(0x01ED53A6) == 3) {
                *(signed char *)AT(0x01ED53A7) -= 6;
                if (*(signed char *)AT(0x01ED53A7) < 0) {
                    *(signed char *)AT(0x01ED53A7) = 0;
                    *(signed char *)AT(0x01ED53A6) = 4;
                }
            }
            break;
        case 0x2756:
        case 0x2757:
        case 0x2760:
            if (*(signed char *)AT(0x01ED53A6) == 3 && *(signed char *)AT(0x01ED5494) >= 3) {
                *(signed char *)AT(0x01ED53A7) -= 6;
                if (*(signed char *)AT(0x01ED53A7) < 0) {
                    *(signed char *)AT(0x01ED53A7) = 0;
                    *(signed char *)AT(0x01ED53A6) = 4;
                }
            }
            break;
        }
    }
    return arg0;
}
#ifdef __MWERKS__
#pragma opt_dead_assignments reset
#endif
