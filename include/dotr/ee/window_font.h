#ifndef DOTR_EE_WINDOW_FONT_H
#define DOTR_EE_WINDOW_FONT_H

/* Window text drawing: SzDuel_DrawWindow_Font_Pipe (0x1b6f70) builds glyph quads from the font
 * tables at 0x2be950 / 0x290b70, and SzGame_ModeDrawLast (0x1a98b0) calls it after tiling a window
 * texture; both build cursor_packets-style primitive packets on the stack.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

#include "dotr/ee/cursor_packets.h"
#include "dotr/ee/window_control.h"

/* 0x1b6f70 SzDuel_DrawWindow_Font_Pipe (original name). Assumed: the per-font cell size as
 * two bytes (width, height) at 0x2be950, read as two byte arrays at 0x2be950 / 0x2be951 indexed
 * by font * 2 (one object on the host); the glyph table at 0x290b70
 * as 10-byte entries, 37 per font, holding a texture index into the duel control block's
 * texture words and the texel rectangle (u0, v0, u1, v1); the default colour as four bytes at
 * 0x2bea00; the packet as a cursor_packets-style header, colour, 1.0 and room for six uv/xyz
 * vertices (four are written; the frame shows 120 bytes); the uv word as 64-bit bit-fields
 * (u at bit 0, v at bit 16). The last argument is a colour pointer (other headers declare it
 * as int for their own callers). flags: 1 proportional, 2 centred, 4 slanted top edge, 8 and 16
 * select ExGetAlphaVal(2) / (3). */
struct DotrFontGlyph {
    unsigned char texture;
    unsigned short u0;
    unsigned short v0;
    unsigned short u1;
    unsigned short v1;
};
struct DotrFontUv {
    unsigned long u : 14;
    unsigned long unknown_14 : 2;
    unsigned long v : 14;
    unsigned long unknown_30 : 34;
};
typedef struct {
    long header[2];
    unsigned char colour[4];
    float unknown_14;
    struct DotrCursorSlotRectVertex vertex[6];
} DotrFontPacket;
extern unsigned char gSzDuel_abFontSize_bytes[];
#ifdef __MWERKS__
extern unsigned char data_002be951[];
#else
#define data_002be951 (gSzDuel_abFontSize_bytes + 1)
#endif
extern struct DotrFontGlyph data_00290b70[];
extern unsigned char icolRgba_2580[];
int lSzDuel_GetFontIndex(char character);
DotrDrawWord SzDuel_DrawWindow_Font_Pipe(DotrDrawWord packet, int x, int y, int font, int flags, int count,
                                         const char *text, unsigned char *color);

/* 0x1a98b0 SzGame_ModeDrawLast (original name). Assumed: a view of the game block at
 * 0x317250 with the frame counter at +0xc, a fade byte at +0x228, the text alpha at +0x229,
 * a signed 2-bit alpha step and the 2-bit mode in the byte at +0x22c and an enable bit in
 * the byte at +0x22e. The function reads the mode through the symbol and the rest through a
 * register, hence the second view symbol (same address; the same object on the host). The
 * packet is the cursor_packets rectangle without its leading 8 bytes; the four bytes at
 * 0x28ff20 are the text colour; the text at 0x2be815 is passed to func_0019bef0 for its
 * length. ExSortSoftFocus_Pipe (0x208280) is declared in cursor_packets.h; this caller passes
 * (cursor, 0, 0, 0, 0, 12, 0), the last argument a null colour pointer. */
struct DotrModeLastView {
    unsigned char unknown_000[12];
    unsigned int counter;
    unsigned char unknown_010[0x218];
    unsigned char busy;
    unsigned char alpha;
    unsigned char unknown_22a[2];
    signed char unknown_bits_0_1 : 2;
    signed char step : 2;
    unsigned char mode : 2;
    unsigned char unknown_22d;
    signed char unknown_22e_bits : 4;
    signed char enabled : 1;
};
struct DotrModeLastPacket {
    long header[2];
    unsigned char colour[4];
    float unknown_14;
    struct DotrCursorSlotRectVertex vertex[2];
    long unknown_38;
    struct DotrCursorTex tex0;
    struct DotrCursorTex tex1;
    long unknown_50;
};
struct DotrModeLastColour {
    unsigned char b[4];
};
extern struct DotrModeLastView gSzGame_gl_last;
#ifdef __MWERKS__
extern struct DotrModeLastView gSzGame_gl_lastview;
#else
#define gSzGame_gl_lastview gSzGame_gl_last
#endif
extern struct DotrModeLastColour data_0028ff20;
extern char data_002be815[];
extern int SzGame_GetMode(int a);
extern DotrDrawWord RhBase_GetVifPacketArea(void);
extern void RhBase_SetVifPacketArea(DotrDrawWord cursor);
extern DotrDrawWord SzDuel_StartWindowSessionWithPalette_Pipe(DotrDrawWord cursor, int a1);
extern void SzDuel_SetWindowDefPrioZ(int value);
extern int *SzDuel_GetWindowTexture(int index);
extern DotrDrawWord SzDuel_FinishWindowSession_Pipe(DotrDrawWord a);
extern int func_0019bef0(const char *string);
void SzGame_ModeDrawLast(void);

/* 0x1b11c0 SzDuel_DrawWindow_Pipe (original name). Offset views and assumed names, not
 * recovered declarations. Assumed: the control block at 0x317490 as far as this function
 * reads it (two 848-byte side elements with the cursor cell at +0x33d/+0x33e, two unit
 * pointers at +0x6c8/+0x6cc, the bytes from +0xd35 and signed one-bit flags in
 * +0xd65..+0xd67); the unit record only at +0x40..+0x45 and bit 6 of +0x57; SzDuel_GetUnit
 * and SzDuel_GetMapUnit returning that record; the callee signatures below, taken from the
 * argument registers of this caller. lSzDuel_DrawWindow_CardInfo_Pipe's fifth argument is
 * narrowed to a byte and its last two are stored as 64-bit stack words.
 * lSzDuel_DrawWindow_FieldPiece_Pipe's float is declared before x and y because this caller
 * loads it before evaluating them (title_draws.h and map_edit.h agree). */
extern int SzDuel_GetField(int x, int y);
extern DotrDrawWord lSzDuel_DrawWindow_FieldPiece_Pipe(DotrDrawWord cursor, int colour, int field, float f, int x, int y);
extern DotrDrawWord lSzDuel_DrawWindow_PlayerInfo_Pipe(DotrDrawWord cursor, int side, int x, int y, int active);
extern DotrDrawWord lSzDuel_DrawWindow_CardInfo_Pipe(DotrDrawWord cursor, struct DotrWindowUnit *unit, int x, int y,
                                                     unsigned char side, int a5, int a6, int a7, long a8, long a9);
extern struct DotrWindowUnit *SzDuel_GetUnit(int side, int pos);
extern struct DotrWindowUnit *SzDuel_GetMapUnit(int x, int y);
extern int func_00250360_pair(int a, int b);
extern int Ta_IsVisualAttrAdv(int a, int b, int c, int d);
extern DotrDrawWord lSzDuel_DrawWindow_CommandGuide_Pipe(DotrDrawWord cursor, int a1, int a2, int a3);
extern DotrDrawWord lSzDuel_DrawWindow_Field_Pipe(DotrDrawWord cursor, int id, int x, int y);
extern DotrDrawWord lSzDuel_DrawWindow_Frame_Pipe(DotrDrawWord cursor, int a1, int a2, int a3, int x, int y, int a6, int a7, long a8);
extern DotrDrawWord SzDuel_DrawWindow_FontDigit_Pipe(DotrDrawWord cursor, int x, int y, int a, int flags, int width, int value);
extern DotrDrawWord lSzDuel_DrawWindow_Level_Pipe(DotrDrawWord cursor, int x, int y);
extern DotrDrawWord lSzDuel_DrawWindow_SummonCost_Pipe(DotrDrawWord cursor, int x, int y);
extern DotrDrawWord SzDuel_DrawHandCursor_Pipe(DotrDrawWord cursor);
extern void SzMsg_Free(int index);
DotrDrawWord SzDuel_DrawWindow_Pipe(DotrDrawWord cursor);

#endif
