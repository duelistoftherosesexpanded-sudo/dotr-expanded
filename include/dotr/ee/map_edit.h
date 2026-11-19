#ifndef DOTR_EE_MAP_EDIT_H
#define DOTR_EE_MAP_EDIT_H

/* Functions that draw the map-edit screen of the duel (SzDuel_DrawMapEdit_Pipe at 0x1ccbb0 and
 * its helpers), reading the cursor bytes at 0xa9c08f/0xa9c090.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

#include "dotr/ee/cell_tints.h"
#include "dotr/ee/scaled_sprite.h"
#include "dotr/ee/pad_toggles.h"

/* Block at 0xa9c000 as read by 0x1ccbb0: two bytes at 0x8f/0x90 used as the cursor's column and
 * row (SzDuel_InitMapEdit stores bytes at 0xa9c08c-0xa9c092; state_records.h has that view). The
 * original holds the literal address 0xa9c000 (lui/ori) in a saved register inside the first loop
 * and addresses the bytes absolutely elsewhere. MATCHING DEVICE: a pointer constant gives that
 * form, a symbol does not. The host build uses an object instead. The start and size of the
 * real object are not known. */
struct DotrMapEditState {
    unsigned char unknown_00[0x8c];
    unsigned char unknown_8c; /* 0x1cc620: (this + 2) % unknown_8d picks the saved grid */
    unsigned char unknown_8d;
    unsigned char unknown_8e;
    unsigned char unknown_8f; /* cursor x, 0-6 */
    unsigned char unknown_90; /* cursor y, 0-6 */
    unsigned char unknown_91; /* frames to wait before the next cursor step */
    unsigned char unknown_92_bit_0 : 1; /* set after a sideways step taken with up/down held */
};
#ifdef __MWERKS__
#define DOTR_MAP_EDIT ((struct DotrMapEditState *)0x00a9c000)
#else
extern struct DotrMapEditState dotr_map_edit_00a9c000;
#define DOTR_MAP_EDIT (&dotr_map_edit_00a9c000)
#endif

/* Four bytes at 0x2914d0 copied to a local colour as one word (assumed: the compiler-placed
 * initialiser of that local, or a constant of the same type). */
extern struct DotrTintQuad data_002914d0;
/* Halfword at 0x318116, read unsigned; its high byte plus 64 becomes the colour's last byte. */
extern unsigned short data_00318116;

extern void SzDuel_InitMapColor2(int a, int b, int c, int d);
extern void SzDuel_SetWindowDefPrioZ(int value);
extern int SzDuel_GetField(int x, int y);
extern DotrSpriteWord SzDuel_StartWindowSessionWithPalette_Pipe(DotrSpriteWord cursor, int a1);
/* Assumed prototypes: argument lists from the call site in 0x1ccbb0, meanings unknown. */
extern DotrSpriteWord lSzDuel_DrawWindow_FieldPiece_Pipe(DotrSpriteWord cursor, int a1, int field, float f, int a3, int a4);
extern DotrSpriteWord lSzDuel_DrawWindow_CommandGuide_Pipe(DotrSpriteWord cursor, int a1, int a2, int a3);
extern DotrSpriteWord lSzDuel_DrawMapEdit_Graph_Pipe(DotrSpriteWord cursor, int a1, int a2);

DotrSpriteWord SzDuel_DrawMapEdit_Pipe(DotrSpriteWord cursor);

/* 0x1cc620 SzDuel_ActMapEdit (name from the Japanese build's symbols; the
 * Japanese body differs). Assumed: the pad-mask meanings; that SzData_GetData(0)
 * answers the entry whose 7 by 7 byte grids start at +0xfea3 (as small_misc.h
 * and side_table.h view it); the drift view of 0x317490 from pad_toggles.h. */
struct DotrMapEditGrids {
    unsigned char unknown_0000[0xfea3];
    unsigned char grids[5][7][7];
};
extern void *SzData_GetData(int side);
extern int SzDuel_SetField(int x, int y, int value);
extern void SzGame_Pop(void);
void SzDuel_ActMapEdit(void);

#endif
