#ifndef DOTR_EE_CAMP_NUMBER_H
#define DOTR_EE_CAMP_NUMBER_H

/* Campaign-screen drawing helpers (Taku_Camp_*) that draw through the message slot table
 * (SzMsg_LoadFont_Pipe, SzMsg_GetInfo, SzMsg_DrawRect_Pipe) with a cursor passed along.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

#ifdef __MWERKS__
typedef int DotrCampWord;
#else
#include <stdint.h>
typedef intptr_t DotrCampWord;
#endif

/* Views assumed from the access widths: SzMsg_GetInfo returns an entry whose four words
 * from +4 are a source rectangle (x, y, width, height); the 16 bytes at 0x29a7d0 are
 * copied whole and passed as SzMsg_DrawRect_Pipe's fifth argument; the sixth is four
 * bytes on the stack (255, 255, 255, 128). */
struct DotrCampRect {
    int w[4];
};
struct DotrCampInfo {
    int unknown_00;
    struct DotrCampRect rect;
};
struct DotrCampColor {
    unsigned char r, g, b, a;
};
extern struct DotrCampRect data_0029a7d0;

/* Callees. SzMsg_LoadFont_Pipe (slot_table) and SzMsg_DrawRect_Pipe (cursor_packets): assumed
 * prototypes from this call site (other groups declare SzMsg_DrawRect_Pipe without arguments
 * because they only pass its address). The others are declared in packet_builds.h and slot_table.h with word-typed
 * arguments. The tests stub all of them. */
extern DotrCampWord SzMsg_LoadFont_Pipe(DotrCampWord cursor, int msgInfoIndex, int stringIndex, int a2, int a3);
extern DotrCampWord RhText_SetTextureFlush(DotrCampWord cursor);
extern struct DotrCampInfo *SzMsg_GetInfo(int index);
extern void SzMsg_Free(int index);
extern DotrCampWord SzMsg_DrawRect_Pipe(DotrCampWord cursor, int index, int x, int y,
                                        struct DotrCampRect *rect, struct DotrCampColor *color);

/* 0x2232f0. Draws four 16-pixel-spaced cells at y 272 from x 160 + x: the last digit of a,
 * the fixed cell 10, then the tens and units digits of b. Each cell loads font 7 into
 * message slot 1 (row offset 22), flushes the texture, moves the slot's source rectangle
 * by 22 * column and 22 * row (digit 0: column 4, row 1; 1..5: column digit - 1, row 0;
 * 6..9: column digit - 6, row 1; 10: column 2, row 4), sets its size to 22 x 22, draws it
 * with the rectangle from 0x29a7d0 and the colour 255/255/255/128 and frees the slot.
 * Returns the cursor from the last draw.
 * Assumed: int parameters; short locals (from the sign extensions); member names. */
DotrCampWord Taku_Camp_Kazu_Disp(DotrCampWord cursor, int a, int b, int x);

#endif
