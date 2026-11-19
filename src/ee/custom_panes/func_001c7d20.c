#include "dotr/ee/custom_panes.h"

/* 0x1c7d20. The callees are declared inside the function body. */
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
/* Helpers for this function (with its own SzMsg_GetInfo view). */
typedef struct DotrCustPaneTextInfo1c7d20 {
    unsigned char pad[12];
    int width;
} DotrCustPaneTextInfo1c7d20;

static inline float dotr_custpane_scale_1c7d20(int index, int limit) {
    extern void *SzMsg_GetInfo();
    int width = ((DotrCustPaneTextInfo1c7d20 *)SzMsg_GetInfo(index))->width;
    if (width > limit) {
        return (float)limit / (float)width;
    }
    return 1.0f;
}

static inline int dotr_custpane_wide_1c7d20(DotrCustPaneTextInfo1c7d20 *text) {
    return text->width > 200;
}

int lSzDuel_DrawCustomDuel_PlayerInfo_Pipe(int arg0, int side, int arg2, int arg3) {
    extern int lSzDuel_DrawWindow_ExIcon_Pipe(int, void *, int, int, int, int, int, int, long long, long long, long long, long long, long long, long long);
    extern int lSzDuel_DrawWindow_Emblem_Pipe(int, int, int, int, int, int);
    extern int lSzDuel_DrawWindow_ExBoard_Pipe(int, int, int, int, int, int, int, int);
    extern int SzDuel_DrawWindow_FontDigit_Pipe(int, int, int, int, int, int, int);
    extern int SzDuel_DrawWindow_Font_Pipe(int, int, int, int, int, int, signed char *, int);
    extern int SzDuel_GetSummonCharge();
    extern int SzData_GetRank();
    extern void *SzMsg_GetInfo();
    extern int SzMsg_DrawRatioX_Pipe(int, int, int, int, float, float, int, void (*)(void));
    int ret;
    int nx;
    int ny;
    int tx;
    int px;
    int py;
    int ty;
    unsigned char *spD0;
    int spEC;
    int frame_x;
    int temp_v0;
    int temp_s0;
    int temp_s0_2;
    int temp_s0_3;
    int temp_s0_4;
    int temp_s0_5;
    int temp_s0_6;
    int temp_s0_7;
    int image_x;
    int temp_v0_2;
    int temp_s1;
    int image_y;
    int temp_s2;
    int temp_s2_2;
    int first_x;
    int first_y;
    int var_s3;
    signed char *var_s7;
    unsigned char *temp_fp;
    int temp_fp_2;
    int var_a2;
    DotrCustPaneTextInfo1c7d20 *first_text;
    float first_scale;
    int hint = 0;
    int number_x;
    int number_y;

    spD0 = data_00317490_custpane.b;
    temp_v0 = SzData_GetCurrentDeck(side);
    temp_v0_2 = SzData_GetLeader(side, temp_v0);
    temp_fp = SzDuel_GetCardConstInfo(temp_v0_2);
    if (*((unsigned char *)side + DOTR_CUSTPANE_SAVE(0xA9C00E)) != 0) {
        var_s7 = (signed char *)data_002bf051_custpane;
    } else {
        var_s7 = (signed char *)data_002bf056_custpane;
    }
    var_a2 = 3;
    if (side != 0) {
        unsigned char mode = *(unsigned char *)DOTR_CUSTPANE_SAVE(0xA9C088);
        switch (mode) {
        case 0:
            var_a2 = 4;
            break;
        default:
            var_a2 = mode + 0x8C;
            break;
        }
    }
    arg0 = SzMsg_LoadFont_Pipe(arg0, 1, var_a2, 0, 0x16);
    temp_s1 = RhText_SetTextureFlush(arg0);
    tx = arg2 + 0x6C0 + ((side != 0) ? 0x110 : 0);
    temp_s1 = lSzDuel_DrawWindow_ExIcon_Pipe(temp_s1, gSzDuel_Icon_custpane + ((side + 0xF3) * 10), tx, arg3 + 0x720, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
    frame_x = arg2 + 0x20;
    (void)frame_x; /* keeps the x add at its own statement: the original computes it before side + 5 */
    spEC = lSzDuel_DrawWindow_Frame_Pipe(temp_s1, side, side + 5, 0xB, frame_x, arg3, -0xF0, -0x20, 1);
    first_x = arg2 + 0x40;
    first_y = arg3 + 5;
    first_text = (DotrCustPaneTextInfo1c7d20 *)SzMsg_GetInfo(1);
    if (dotr_custpane_wide_1c7d20(first_text)) {
        first_scale = 200.0f / (float)first_text->width;
    } else {
        first_scale = 1.0f;
    }
    temp_s2 = SzMsg_DrawRatioX_Pipe(spEC, 1, first_x, first_y, first_scale, 1.0f, 0, SzMsg_DrawRect_Pipe);
    SzMsg_Free(1);
    var_s3 = lSzDuel_DrawWindow_ExBoard_Pipe(lSzDuel_DrawWindow_Frame_Pipe(temp_s2, side, -1, 4, arg2, arg3 + 0x20, -0x130, -0x80, 1), side, arg2 + 0xD, arg3 + 0x25, -0x116, -0x5E, -1, 0);
    if (temp_fp != NULL) {
        temp_s2_2 = *temp_fp & 0x1F;
        temp_s1 = SzDuel_GetMsgName(temp_v0_2);
        temp_fp_2 = SzData_GetRank(side, temp_v0_2);
        var_s3 = lSzDuel_DrawWindow_ExIcon_Pipe(lSzDuel_DrawWindow_ExIcon_Pipe(lSzDuel_DrawWindow_ExIcon_Pipe(lSzDuel_DrawWindow_Frame_Pipe(RhText_SetTextureFlush(SzMsg_LoadFont_Pipe(SzMsg_LoadFont_Pipe(SzMsg_LoadFont_Pipe(var_s3, 1, temp_s1, 0, 0x16), 2, temp_s2_2 + 0x25, 0, 0x2C), 3, temp_fp_2 + 0x52, 0, 0x42)), 0xC, -1, 0xB, arg2 + 0x11, arg3 + 0x29, -0x6E, -0x56, 1), data_0029099a_custpane, arg2 + 0x6D9, arg3 + 0x751, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80), gSzDuel_Icon_custpane + ((temp_v0 + 0xE3) * 10), arg2 + 0x6E4, arg3 + 0x764, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80), data_002908f0_custpane, arg2 + 0x6E0, arg3 + 0x784, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
        number_x = arg2 + 0x35;
        number_y = arg3 + 0x65;
        var_s3 = SzDuel_DrawWindow_FontDigit_Pipe(var_s3, number_x, number_y, 0, hint != 0 ? 1 : 0, 4, SzData_GetDeckCost(side, temp_v0));
        image_x = arg2 + 0x4A;
        image_y = arg3 + 0x2E;
        temp_s0 = lSzDuel_DrawWindow_Frame_Pipe(lSzDuel_DrawWindow_PicMini_Pipe(var_s3, side, image_x, image_y, 0x30, 0x30, *(int *)(spD0 + 0xC58), 1), -1, -1, 4, image_x, image_y, 3, 6, 1);
        temp_s0_2 = SzMsg_DrawRatioX_Pipe(temp_s0, 1, arg2 + 0x81, arg3 + 0x2B, dotr_custpane_scale_1c7d20(1, 156), 1.0f, 0, SzMsg_DrawRect_Pipe);
        SzMsg_Free(1);
        px = arg2 + 0x741;
        py = arg3 + 0x767;
        temp_s0_3 = lSzDuel_DrawWindow_ExIcon_Pipe(temp_s0_2, gSzDuel_Icon_custpane + (temp_s2_2 * 10), px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, hint != 0 ? 0x40 : 0x80);
        var_s3 = SzMsg_DrawRatioX_Pipe(temp_s0_3, 2, arg2 + 0x9B, arg3 + 0x47, dotr_custpane_scale_1c7d20(2, 138), 1.0f, 0, SzMsg_DrawRect_Pipe);
        SzMsg_Free(2);
        if (*(unsigned char *)DOTR_CUSTPANE_SAVE(0xA9C088) != 0 || *((unsigned char *)side + DOTR_CUSTPANE_SAVE(0xA9C00A)) != 0) {
            temp_s0_4 = lSzDuel_DrawWindow_Emblem_Pipe(var_s3, arg2 + 0x81, arg3 + 0x65, temp_fp_2, 1, 0);
            var_s3 = SzMsg_DrawRatioX_Pipe(temp_s0_4, 3, arg2 + 0x9B, arg3 + 0x65, dotr_custpane_scale_1c7d20(3, 138), 1.0f, 0, SzMsg_DrawRect_Pipe);
        }
        SzMsg_Free(3);
    }
    ret = lSzDuel_DrawWindow_ExIcon_Pipe(lSzDuel_DrawWindow_Frame_Pipe(var_s3, 0xC, -1, 5, arg2 + 0x14, arg3 + 0x86, -0x70, -0x14, 1), &data_00290a1c_custpane, arg2 + 0x6D6, arg3 + 0x7A8, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    nx = arg2 + 0x3E;
    ny = arg3 + 0x8A;
    ret = lSzDuel_DrawWindow_ExIcon_Pipe(lSzDuel_DrawWindow_Frame_Pipe(SzDuel_DrawWindow_FontDigit_Pipe(ret, nx, ny, 0, hint != 0 ? 1 : 0, 4, SzDuel_GetLP(side)), 0xC, -1, 5, arg2 + 0x94, arg3 + 0x86, -0x3C, -0x14, 1), &data_002908e6_custpane, arg2 + 0x756, arg3 + 0x7A8, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0x80);
    nx = arg2 + 0xAC;
    ny = arg3 + 0x8A;
    ret = SzDuel_DrawWindow_FontDigit_Pipe(ret, nx, ny, 0, hint != 0 ? 1 : 0, 2, SzDuel_GetSummonCharge(side));
    if (*(unsigned char *)DOTR_CUSTPANE_SAVE(0xA9C088) == 0) {
        nx = arg2 + 0x100;
        ny = arg3 + 0x8A;
        ret = SzDuel_DrawWindow_Font_Pipe(ret, nx, ny, 0, 2, func_0019bef0((const char *)var_s7), var_s7, hint != 0 ? (int)data_002be944_custpane : 0);
    }
    return ret;
}
