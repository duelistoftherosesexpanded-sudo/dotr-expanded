#ifndef DOTR_EE_CAMP_IVENT_WINDOW_H
#define DOTR_EE_CAMP_IVENT_WINDOW_H

/* The campaign map's event window Taku_Camp_Ivent_Window, which draws the sliding event dialog,
 * its choices and code digits from the camp work area at 0x1ed0000 and the camp state at 0x299e10.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* Taku_Camp_Ivent_Window reads the camp work area at the literal address 0x1ed0000 (bytes 0x1ed53a6
 * mode, 0x1ed53a7 slide, 0x1ed53a8 selection, 0x1ed5494/0x1ed5495 and a state view) and the camp
 * state at 0x299e10 as bytes (the event number at +0xc2, also read as the separate symbol
 * 0x299ed2).
 * MATCHING DEVICE: under the target compiler the two camp-state symbols are separate externs and
 * AT(a) is the literal address itself; on the host both symbols name one fixture and AT(a) indexes
 * a fixture of the work area. Assumed: these meanings, from use. */
#ifdef __MWERKS__
extern signed char data_00299e10_iventwin;
extern short CutSceneNumber_iventwin;
#define DOTR_IVENT_WINDOW_AT(a) (a)
#else
#include <stdint.h>
union DotrIventWinBoard {
    signed char bytes[0x100];
    short align;
};
extern union DotrIventWinBoard dotr_camp_ivent_window_board;
#define data_00299e10_iventwin (dotr_camp_ivent_window_board.bytes[0])
#define CutSceneNumber_iventwin (*(short *)(dotr_camp_ivent_window_board.bytes + 0xc2))
union DotrIventWinWork {
    unsigned char bytes[0x5500];
    double align;
};
extern union DotrIventWinWork dotr_camp_ivent_window_work;
#define DOTR_IVENT_WINDOW_AT(a) ((intptr_t)dotr_camp_ivent_window_work.bytes + ((a) - 0x01ED0000))
#endif

#ifndef NULL
#define NULL 0
#endif

/* Views: the window frame (corners and kind), sprite templates
 * (eight corner coordinates, twenty data bytes, a texture index) and pairs of them at
 * 0x29aa08..0x29ab50, tex_bara_data (0x2980d0), the texture table tex_uv_data (0x2950a0), the
 * code records at 0x299d00 (value, number, three-bit digits, types), the choice texts at
 * 0x30f0bb..0x30f0e8 and the work-area state (selection, mode, three codes and values from
 * 0x1ed53f8). Assumed: names from use. */
typedef struct {
    short x0, y0, x1, y1;
    short kind;
} DotrIventWinFrame;

typedef struct {
    short x0, y0, x1, y1, x2, y2, x3, y3;
    unsigned char data[0x14];
    unsigned short index;
} DotrIventWinSprite;

typedef struct {
    DotrIventWinSprite s[2];
} DotrIventWinPair;

typedef struct {
    unsigned char r, g, b, a;
} DotrIventWinColor;

typedef struct {
    short u, v, width, height;
    unsigned char pad08[8];
    unsigned short texture;
    unsigned char pad12[2];
} DotrIventWinTex;

typedef struct {
    unsigned short value;
    unsigned short number;
    unsigned short digits;
    unsigned short pad06;
    unsigned int types;
} DotrIventWinCode;

typedef struct {
    unsigned char pad[0x53A8];
    signed char sel;
    signed char pad53A9;
    signed char mode;
    unsigned char pad53AB[0x4D];
    unsigned short codes[3];
    unsigned short values[3];
} DotrIventWinState;

extern DotrIventWinFrame data_0029aa08_iventwin;
extern DotrIventWinSprite data_0029aa20_iventwin;
extern DotrIventWinPair data_0029aa50_iventwin;
extern DotrIventWinSprite data_0029aaa0_iventwin;
extern DotrIventWinPair data_0029aad0_iventwin;
extern DotrIventWinSprite data_0029ab20_iventwin;
extern DotrIventWinSprite data_0029ab50_iventwin;
extern DotrIventWinSprite tex_bara_data[];
extern DotrIventWinTex tex_uv_data[];
extern DotrIventWinCode data_00299d00_iventwin[];
extern char data_0030f0bb_iventwin[];
extern char data_0030f0c7_iventwin[];
extern char data_0030f0d4_iventwin[];
extern char data_0030f0e8_iventwin[];

/* Callees not declared inside the function. The others (Taku_Camp_Ivent_Name, Taku_Window_Set,
 * Taku_Obj_Set, Taku_Bunsyou_Set, SzMsg_LoadFont_Pipe, Taku_FT4_Set, SzMsg_DrawRatioX_Pipe) are
 * declared inside the body with their own types, so this header declares none of them. Cursors are
 * 32-bit words (int). */
extern int SzDuel_DrawWindow_Font_Pipe(int packet, int x, int y, int font, int flags, int count, signed char *text,
                                       unsigned char *color);
extern int SzMsg_DrawRect_Pipe(int arg0, int arg1, int arg2, int arg3, int *arg4, unsigned char *arg5);
extern void *SzMsg_GetInfo(int arg0);
extern int SzDuel_GetMsgName(int cardNo);
extern void SzMsg_Free(int arg0);
extern int RhText_SetTextureFlush(int arg0);
extern int Taku_Camp_Kazu_Disp(int arg0, int arg1, int arg2, int arg3);

/* 0x224e40: draws the event window for the current event number; returns the cursor. */
int Taku_Camp_Ivent_Window(int arg0);

#endif
