#include "dotr/ee/trade_panes.h"

/* 0x1c4b00. The callees are declared inside the function body. Compiled with
 * #pragma opt_dead_assignments off, set directly before the function and reset after it. */
#ifndef __MWERKS__
/* Host build only: this C passes addresses as 32-bit words and mixes char signedness
 * and pointer types as the target compiler accepts; GCC would reject these as errors. */
#pragma GCC diagnostic ignored "-Wpointer-sign"
#pragma GCC diagnostic ignored "-Wint-conversion"
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
#ifdef __MWERKS__
#pragma opt_dead_assignments off
#endif
int lSzDuel_DrawTrade_Card_Pipe(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5) {
    extern int lSzDuel_DrawWindow_ExIcon_Pipe(int, void *, int, int, int, int, int, int, long long, long long, long long, long long, long long, long long);
    extern int lSzDuel_DrawWindow_Emblem_Pipe(int, int, int, long long, int, int);
    extern int SzDuel_DrawWindow_FontDigit_Pipe();
    extern int SzDuel_DrawWindow_Font_Pipe();
    extern int lSzDuel_DrawWindow_ExBoard_Pipe();
    extern int SzMsg_GetInfo();
    extern int SzMsg_DrawRatioX_Pipe(int, int, int, int, float, float, int, void (*)(void));
    extern int SzDuel_Type2Attr();
    extern long long SzData_GetRank();
    int ret;
    int id3;
    unsigned char *state = data_00317490_tradepane.b;
    unsigned char *info;
    int kind;
    int style;
    int attribute;
    unsigned char *name;
    unsigned char *own_counts;
    unsigned char *counts;
    unsigned char *other_counts;
    signed char *text;
    unsigned char mode;
    unsigned char category;
    int low;
    float scale;
    int px;
    int x_position;
    int py;
    int ty;
    unsigned char *count;
    int width;
    int id;
    DotrTradePaneBox1c4b00 box __attribute__((aligned(8)));
    int value;
    int hint = 0;

    info = SzDuel_GetCardConstInfo(arg2);
    if (arg1 != 0) {
        own_counts = *(unsigned char **)(state + 0x6E4);
    } else {
        own_counts = *(unsigned char **)(state + 0x6E0);
    }
    if (arg1 != 0) {
        other_counts = *(unsigned char **)(state + 0x6E0);
    } else {
        other_counts = *(unsigned char **)(state + 0x6E4);
    }
    mode = state[arg1 + 0xCC0];
    if (mode == 2) {
        counts = other_counts;
    } else {
        counts = own_counts;
    }
    if (mode == 2) {
        text = &data_002bf02d_tradepane;
    } else {
        text = (signed char *)data_002bf030_tradepane;
    }
    scale = 1.0f;
    if (info == NULL) {
        return lSzDuel_DrawWindow_Frame_Pipe(arg0, -1, -1, 9, arg3, arg4, -0x130, -0x36, 0);
    }
    category = info[0];
    kind = (int)category >> 5;
    low = category & 0x1F;
    if (kind == 0) {
        name = gSzDuel_Icon_tradepane + low * 10;
    } else {
        name = gSzDuel_Icon_tradepane + (kind + 20) * 10;
    }
    style = SzDuel_Kind2Type(kind);
    attribute = SzDuel_Type2Attr(style, info[2] & 0xF);
    if (style == 0) {
        if (*(unsigned short *)(info + 4) != 0xFFFF) {
            style = 4;
        }
    }
    id = lSzDuel_DrawWindow_Frame_Pipe(RhText_SetTextureFlush(SzMsg_LoadFont_Pipe(arg0, 1, SzDuel_GetMsgName(arg2), 0, 22)), style + 7, -1, 9, arg3, arg4, -0x130, -0x36, 0);
    if (SzData_IsBagNew(arg1, arg2) != 0) {
        x_position = arg3 + 0x6C0 + (arg1 != 0 ? 0x108 : 8);
        id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_00290a12_tradepane, x_position, arg4 + 0x727, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    } else {
        id = SzDuel_DrawWindow_FontDigit_Pipe(id, arg3 + (arg1 != 0 ? 0x108 : 8), arg4 + 7, 1, 0, 3, arg2);
    }
    if (kind == 0) {
        long long leader = SzData_GetRank(arg1, arg2);
        x_position = arg3 + dotr_tradepane_offset_1c4b00(arg1, 0x28, 0xE8);
        id = lSzDuel_DrawWindow_Emblem_Pipe(id, x_position, arg4 + 3, leader, 0, dotr_tradepane_offset_1c4b00(arg1, 0, 1));
    }
    x_position = arg3 + 0x6C0 + (arg1 != 0 ? 0x114 : 4);
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, name, x_position, arg4 + 0x73A, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    x_position = arg3 + 0x6C0 + (arg1 != 0 ? 0xFA : 0x1E);
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_00290020_tradepane + attribute * 10, x_position, arg4 + 0x73A, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    if (kind == 0) {
        x_position = arg3 + 0x6C0 + (arg1 != 0 ? 0xCF : 0x38);
        id = lSzDuel_DrawWindow_ExIcon_Pipe(id, &data_002908e6_tradepane, x_position, arg4 + 0x743, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
        id = SzDuel_DrawWindow_FontDigit_Pipe(id, arg3 + (arg1 != 0 ? 0xCF : 0x38) + 0x16, arg4 + 0x25, 1, 0, 2, ((DotrTradePaneNibbles1c4b00 *)(info + 2))->high);
        x_position = arg3 + 0x6C0 + (arg1 != 0 ? 3 : 0xF8);
        id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_002908be_tradepane, x_position, arg4 + 0x723, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
        id = SzDuel_DrawWindow_FontDigit_Pipe(id, arg3 + (arg1 != 0 ? 3 : 0xF8) + 0x10, arg4 + 5, 1, 0, 4, *(unsigned short *)(info + 8) & 0x1FFF);
        x_position = arg3 + 0x6C0 + (arg1 != 0 ? 3 : 0xF8);
        id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_002908c8_tradepane, x_position, arg4 + 0x733, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
        id = SzDuel_DrawWindow_FontDigit_Pipe(id, arg3 + (arg1 != 0 ? 3 : 0xF8) + 0x10, arg4 + 0x15, 1, 0, 4, *(unsigned short *)(info + 10) & 0x1FFF);
    }
    x_position = arg3 + 0x6C0 + (arg1 != 0 ? 0x96 : 0x68);
    id = lSzDuel_DrawWindow_ExIcon_Pipe(id, data_002908f0_tradepane, x_position, arg4 + 0x743, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    ret = SzDuel_DrawWindow_FontDigit_Pipe(id, arg3 + (arg1 != 0 ? 0x96 : 0x68) + 0x16, arg4 + 0x25, 1, 0, 3, info[3]);
    px = arg3 + 0x6C0 + (arg1 != 0 ? 0x66 : 0xAD);
    py = arg4 + 0x743;
    if (SzData_GetBagCount(arg1, arg2, -1) < 9) {
        ret = lSzDuel_DrawWindow_ExIcon_Pipe(ret, data_0029086e_tradepane, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    } else {
        ret = lSzDuel_DrawWindow_ExIcon_Pipe(ret, data_00290878_tradepane, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    }
    px = arg3 + (arg1 != 0 ? 0x66 : 0xAD) + 0x13;
    py = arg4 + 0x25;
    ret = SzDuel_DrawWindow_FontDigit_Pipe(ret, px, py, 1, 0, 1, SzData_GetBagCount(arg1, arg2, hint != 0 ? -1 : 3));
    count = counts + arg2;
    if (*count != 0) {
        ty = arg4 + 0x25;
        ret = lSzDuel_DrawWindow_Frame_Pipe(ret, 0xD, -1, 7, arg3 + dotr_tradepane_offset_1c4b00(arg1, 0xE8, 0), arg4 + 0x23, -0x48, -0x10, 1);
        px = arg3 + (arg1 != 0 ? 0 : 0xE8) + 0xC;
        value = func_0019bef0((char *)text);
        ret = SzDuel_DrawWindow_Font_Pipe(ret, px, ty, 1, 0, value, text, 0);
        px = arg3 + (arg1 != 0 ? 0 : 0xE8) + 0x30;
        ret = SzDuel_DrawWindow_FontDigit_Pipe(ret, px, ty, 1, 0, 1, *count);
    }
    id3 = lSzDuel_DrawWindow_ExBoard_Pipe(ret, style + 7, arg3 + (arg1 != 0 ? 0x38 : 0x48), arg4 + 3, -0xB0, -0x20, -1, 0);
    width = *(int *)((unsigned char *)SzMsg_GetInfo(1) + 0xC);
    if (width > 168) {
        scale = 168.0f / (float)width;
    }
    id3 = SzMsg_DrawRatioX_Pipe(id3, 1, arg3 + dotr_tradepane_offset_1c4b00(arg1, 0x48, 0x38) + 6, arg4 + 9, scale, 1.0f, (int)gSzDuel_aicolMsg_tradepane, SzMsg_DrawRect_Pipe);
    id3 = SzMsg_DrawRatioX_Pipe(id3, 1, arg3 + dotr_tradepane_offset_1c4b00(arg1, 0x48, 0x38) + 5, arg4 + 8, scale, 1.0f, (int)data_002be944_tradepane, SzMsg_DrawRect_Pipe);
    id3 = SzMsg_DrawRatioX_Pipe(id3, 1, arg3 + dotr_tradepane_offset_1c4b00(arg1, 0x48, 0x38) + 4, arg4 + 7, scale, 1.0f, (int)data_002be948_tradepane, SzMsg_DrawRect_Pipe);
    SzMsg_Free(1);
    if (arg5 != 0) {
        if (SzData_GetBagCount(arg1, arg2, 3) <= 0) {
            ((DotrTradePaneTag1c4b00 *)box.tag)->flags = 6;
            ((DotrTradePaneTag1c4b00 *)box.tag)->count = 3;
            ((DotrTradePaneTag1c4b00 *)box.tag)->registers = 0x441;
            ((DotrTradePaneTag1c4b00 *)box.tag)->padding = 0;
            ExAssignAlpha(box.tag, ExGetAlphaVal(2));
            box.color.r = 0x80;
            box.color.g = 0x80;
            box.color.b = 0x80;
            box.color.a = 0x40;
            box.color.q = 1.0f;
            box.vertex0.x = (arg3 + 0x6C0) << 4;
            box.vertex0.y = (arg4 + 0x720) << 4;
            box.vertex0.z = *(int *)(state + 0xC58);
            box.vertex0.fog = 0;
            box.vertex1.x = (arg3 + 0x7F0) << 4;
            box.vertex1.y = (arg4 + 0x756) << 4;
            box.vertex1.z = *(int *)(state + 0xC58);
            box.vertex1.fog = 0;
            id3 = ExSortPrim_Pipe(id3, (int)box.tag, 0, 0);
        }
    }
    return id3;
}
