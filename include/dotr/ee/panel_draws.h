#ifndef DOTR_EE_PANEL_DRAWS_H
#define DOTR_EE_PANEL_DRAWS_H

/* Draw-list builders at 0x1b1ef0 and 0x1b20e0 that put a frame, a text piece and either a
 * number or a slot image at a position, chosen by SzDuel_GetPlayer of the control block's side
 * byte.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* Cursor word, slot entry view, data_00317490_bytes, SzMsg_DrawRect_Pipe and the callees
 * lSzDuel_DrawWindow_Frame_Pipe, lSzDuel_DrawWindow_Icon_Pipe (cursor view), SzMsg_LoadFont_Pipe,
 * RhText_SetTextureFlush, SzMsg_GetInfo and SzMsg_Free. */
#include "dotr/ee/scaled_sprite.h"

/* Callees as declared in state_records, small_misc and state_grid (those headers
 * are not included: small_misc.h and state_grid.h cannot be combined). */
extern int SzDuel_GetPlayer(int side);
extern int lSzDuel_DrawWindow_Board_Pipe(int a, int b, int c, int d, int e, int f, int g);
extern int SzDuel_DrawWindow_Digit_Pipe(int a, int b, int c, int d, int e, int f);
extern int SzDuel_GetUnitLevel(int side, int pos, int ignoreBoosts);
/* View of SzMsg_DrawX_Pipe (slot_table) whose last argument is the function address the
 * caller passes; slot_table declares that word as int. */
extern DotrSpriteWord func_001fe7d0_panel(DotrSpriteWord cursor, int a0, int a1, int a2, int a3, void (*function)(void));
/* Passed to lSzDuel_DrawWindow_Icon_Pipe as its second argument; contents not examined. */
extern char data_002901ca[];
extern char data_002909b8[];
/* As declared in state_records. */
extern int Ta_GetSummonCost(void);

/* 0x1b1ef0: frame (lSzDuel_DrawWindow_Frame_Pipe with control byte +0xd54, -1, 6, x, y, 8, 3,
 * 1), text piece (lSzDuel_DrawWindow_Icon_Pipe with 0x2901ca at x + 1735, y + 1830), board
 * (lSzDuel_DrawWindow_Board_Pipe at x + 33, y + 4), then at x + 36, y + 6 either the slot-1
 * image (SzMsg_LoadFont_Pipe, RhText_SetTextureFlush, SzMsg_GetInfo entry word +0xc = 44,
 * SzMsg_DrawX_Pipe with SzMsg_DrawRect_Pipe, SzMsg_Free) or SzDuel_DrawWindow_Digit_Pipe with
 * the number SzDuel_GetUnitLevel(2, control byte +0xd4f + 1, 0). The image is drawn when
 * SzDuel_GetPlayer(control byte +0xd3c) < 32 and SzDuel_GetPlayer(that byte ^ 1) >= 100.
 * Assumed: three int arguments, the last call's result returned, callee signatures as above. */
DotrSpriteWord lSzDuel_DrawWindow_Level_Pipe(DotrSpriteWord cursor, int x, int y);

/* 0x1b20e0: as 0x1b1ef0 with the text piece 0x2909b8, and the number replaced by
 * Ta_GetSummonCost() when control byte +0xd4e is not zero (SzDuel_GetUnitLevel is still called
 * first).
 * MATCHING DEVICE: the conditional expression reproduces the original's unfilled delay
 * slots; an if statement does not. Same assumptions. */
DotrSpriteWord lSzDuel_DrawWindow_SummonCost_Pipe(DotrSpriteWord cursor, int x, int y);

/* 0x1b6ca0: draws `value` as up to `digits` decimal digits, least significant first
 * and rightmost, through slot entry 0: entry word +4 = digit * 22, then SzMsg_DrawX_Pipe with
 * SzMsg_DrawRect_Pipe at x + (digits - i - 1) * step, step 16 when `narrow` is not zero, else 20.
 * Stops once the next power of ten exceeds the value unless `all` is not zero; a
 * negative value then gets one more piece with word +4 = 242 if a place is left.
 * The value 0x7fffffff draws `digits` pieces with word +4 = 264 instead. Assumed:
 * eight int arguments (the eighth forwarded as SzMsg_DrawX_Pipe's fifth), the cursor
 * returned; the entry word is written through an int cast because the entry view
 * of scaled_sprite.h does not name +4. */
DotrSpriteWord SzDuel_DrawWindow_ExDigit_Pipe(DotrSpriteWord cursor, int x, int y, int digits, int value, int narrow, int all, int a7);

/* 0x27f830: 1 when SzDuel_GetPlayer(index) is in 32..99, else 0; the callee is called a
 * second time for the upper bound. Assumed: one int argument, int result. */
int TaTuto_IsTuto(int side);

#endif
