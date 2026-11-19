#ifndef DOTR_EE_WINDOW_PANELS_H
#define DOTR_EE_WINDOW_PANELS_H

/* Panels drawn inside the duel information window by SzDuel_DrawWindow_Pipe (adjacent functions
 * from 0x1b19f0; they read the window control block at 0x317490 and call the window sprite and
 * digit drawers).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

#include "dotr/ee/window_control.h"

#ifdef __MWERKS__
typedef int DotrPanelWord;
#else
#include <stdint.h>
typedef intptr_t DotrPanelWord;
#endif

/* 0x1b19f0 lSzDuel_DrawWindow_PlayerInfo_Pipe (original name). Assumed: in the side
 * element of the control block the unsigned 16-bit points at +0x33a, the player id at +0x33c
 * and the hand count at +0x341; the word at +0xc58 of the control block as the portrait's
 * depth argument; the icon table at 0x290080 in 10-byte rows and the text at 0x2bef76 as
 * character data; the callee signatures from the argument registers of this caller (other
 * headers declare the same callees for their own sources); the last parameter is an int, as
 * window_font.h declares it. */
extern char gSzDuel_Icon_bytes[];
extern char data_002bef76[];
extern int SzDuel_GetPlayer(int side);
extern int SzDuel_GetDeckCount(int side);
extern int SzDuel_GetUnitMonsterCount(int side);
extern int SzDuel_GetUnitSpellCount(int side);
extern DotrPanelWord lSzDuel_DrawWindow_Frame_Pipe(DotrPanelWord cursor, int a1, int a2, int a3, int x, int y, int a6, int a7,
                                                   long a8);
extern DotrPanelWord lSzDuel_DrawWindow_PicMini_Pipe(DotrPanelWord cursor, int id, int x, int y, int width, int height,
                                                     unsigned int z, long bright);
extern DotrPanelWord lSzDuel_DrawWindow_Board_Pipe(DotrPanelWord cursor, int side, int x, int y, int e, int f, int g);
extern DotrPanelWord SzDuel_DrawWindow_Digit_Pipe(DotrPanelWord cursor, int x, int y, int width, int value, int f);
extern DotrPanelWord lSzDuel_DrawWindow_Icon_Pipe(DotrPanelWord cursor, char *row, int x, int y, int e, int f, int g, int h,
                                                  long bright, long j, long k);
extern DotrPanelWord SzDuel_DrawWindow_Font_Pipe(DotrPanelWord cursor, int x, int y, int font, int flags, int n, const char *text,
                                                 unsigned char *colour);
extern DotrPanelWord SzDuel_DrawWindow_FontDigit_Pipe(DotrPanelWord cursor, int x, int y, int a, int flags, int width, int value);
/* Data as lSzDuel_DrawWindow_PlayerInfo_Pipe names it (each symbol links by address): the
 * control block at 0x317490 as bytes (in a union), the icon table at 0x290080 and the single
 * rows 0x290882, 0x2908e6, 0x29094a, 0x290954 and 0x290a1c in it, and the text at 0x2bef76.
 * MATCHING DEVICE: separate symbols on the target; on the host they point into the fixtures
 * above. */
union DotrPanelInfoControl {
    unsigned char b[0xd70];
    int align;
};
#ifdef __MWERKS__
extern union DotrPanelInfoControl data_00317490_panels;
extern unsigned char gSzDuel_Icon_panels[];
extern unsigned char data_00290882_panels[];
extern unsigned char data_002908e6_panels[];
extern unsigned char data_0029094a_panels[];
extern unsigned char data_00290954_panels[];
extern unsigned char data_00290a1c_panels[];
extern signed char data_002bef76_panels[];
#else
#define data_00317490_panels (*(union DotrPanelInfoControl *)&data_00317490_window)
#define gSzDuel_Icon_panels ((unsigned char *)gSzDuel_Icon_bytes)
#define data_00290882_panels (gSzDuel_Icon_panels + 0x802)
#define data_002908e6_panels (gSzDuel_Icon_panels + 0x866)
#define data_0029094a_panels (gSzDuel_Icon_panels + 0x8ca)
#define data_00290954_panels (gSzDuel_Icon_panels + 0x8d4)
#define data_00290a1c_panels (gSzDuel_Icon_panels + 0x99c)
#define data_002bef76_panels ((signed char *)data_002bef76)
#endif
/* Views of four callees for lSzDuel_DrawWindow_PlayerInfo_Pipe (they link by address): it calls
 * them through block-scope declarations without parameters, so every argument is passed with the
 * default promotions; these views keep that (no prototype), while the
 * prototypes above keep the 64-bit forms lSzDuel_DrawWindow_CommandGuide_Pipe needs. Assumed
 * arguments: lSzDuel_DrawWindow_Frame_Pipe (cursor, colour, icons, style, x, y, columns,
 * rows, bright), lSzDuel_DrawWindow_PicMini_Pipe (cursor, id, x, y, width, height, depth,
 * bright), lSzDuel_DrawWindow_Icon_Pipe (cursor, row pointer, x, y, then seven ints),
 * SzDuel_DrawWindow_Font_Pipe (cursor, x, y, font, flags, count, text, colour pointer). */
extern DotrPanelWord func_001b4780_info();
extern DotrPanelWord func_001b7f90_info();
extern DotrPanelWord func_001b3c80_info();
extern DotrPanelWord func_001b6f70_info();
DotrPanelWord lSzDuel_DrawWindow_PlayerInfo_Pipe(DotrPanelWord cursor, int side, int x, int y, int active);

/* 0x1b3100 lSzDuel_DrawWindow_CommandGuide_Pipe (original name). Assumed: the guide table at
 * 0x291180 as rows of four 4-byte entries (10-bit icon row, two 3-bit offsets, then a 16-bit
 * message id; 1023 and 0xffff mean none); bit 0 of +0x346 in the side element, bits 0 and 4
 * of +0x57 in the unit record and bit 3 of +0xd66 in the control block as signed one-bit
 * flags; the message slot entry only as far as its width word at +0xc; SzMsg_DrawRect_Pipe
 * passed by address only, and SzMsg_DrawRatioX_Pipe with the two floats last, as the
 * scaled_sprite and map_edit callers declare it. */
struct DotrGuideEntry {
    unsigned short icon : 10;
    unsigned short dx : 3;
    unsigned short dy : 3;
    unsigned short message;
};
struct DotrPanelInfo {
    int unknown_00[3];
    int width;
};
extern struct DotrGuideEntry data_00291180[][4];
extern int func_00250360_pair(int a, int b);
extern int SzDuel_GetHandCount(int side);
extern int Ta_IsUnitHide(struct DotrWindowUnit *unit, int side);
extern int SzDuel_GetMapMovable(int x, int y);
extern int SzView_InqRequest(int a, int b);
extern DotrPanelWord SzMsg_LoadFont_Pipe(DotrPanelWord cursor, int index, int message, int a3, int size);
extern DotrPanelWord RhText_SetTextureFlush(DotrPanelWord cursor);
extern struct DotrPanelInfo *SzMsg_GetInfo(int index);
extern void SzMsg_DrawRect_Pipe(void);
extern DotrPanelWord SzMsg_DrawRatioX_Pipe(DotrPanelWord cursor, int index, int x, int y, void *e, void (*function)(void),
                                           float sx, float sy);
extern void SzMsg_Free(int index);
DotrPanelWord lSzDuel_DrawWindow_CommandGuide_Pipe(DotrPanelWord cursor, int a1, int x, int y);

#endif
