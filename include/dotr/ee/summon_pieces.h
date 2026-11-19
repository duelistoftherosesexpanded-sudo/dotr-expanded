#ifndef DOTR_EE_SUMMON_PIECES_H
#define DOTR_EE_SUMMON_PIECES_H

/* TaLogic006/008/011_GetSummonPiece: per-logic copies of the summon-place search of free_spots
 * with their own key and target callees.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

#include "dotr/ee/free_spots.h"

/* Key callees, as position_rules.h declares them (defined there). */
extern int TaLogic_GetWarnHideLvl__00134ae0(int side);
extern int TaLogic_GetWarnHideLvl__00141b30(int side);

/* Target callees, defined as (int word, int column, int row) in position_rules and
 * cell_picks; these copies pass the block pointer as the first argument (cast to an int, the
 * same register), so the views take the block. Assumed: the meaning
 * of the returned packed (row << 8 | column) place. */
extern int func_00131f80_piece(struct DotrFreeBlock *block, int column, int row);
extern int func_0013eb20_piece(struct DotrFreeBlock *block, int column, int row);

/* TaLogic_GetInvadeRoot2 (0x27a130) as TaLogic008_GetSummonPiece calls it: declared without
 * parameters, every argument is passed as an int; this view takes ints
 * in the same positions as free_spots.h's func_0027a130_free. */
extern int func_0027a130_piece(struct DotrFreeBlock *block, struct DotrFreeNibbles *bytes, int column, int row, int x,
                               int y, int g, int h, int i, int key);

/* TaLogic006_GetSummonPiece's callees: its key callee (defined in position_rules as
 * int (int side)) and 0x2792c0 with narrow last parameters (the
 * free_spots.h view takes an int count). Assumed: these types. */
extern int TaLogic_GetWarnHideLvl__0012b1b0(int side);
extern int func_002792c0_piece6(struct DotrFreeBlock *block, struct DotrFreeNibbles *bytes, int column, int row, int x,
                                int y, unsigned short id, unsigned char n);

/* 0x128d40 TaLogic006_GetSummonPiece (name from the Japanese build's symbols): logic 006's copy
 * with fixed fallback targets (1, 3) and (2, 5). The second parameter is not read. */
int TaLogic006_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *block, int packed);

/* 0x133320 TaLogic008_GetSummonPiece and 0x13feb0 TaLogic011_GetSummonPiece (names from the
 * Japanese build's symbols). Parameters as the other copies (free_spots.h); the second is not
 * read (an unsigned char in 008, an int in 011). */
int TaLogic008_GetSummonPiece(int side, unsigned char kind, struct DotrFreeBlock *block, int packed);
int TaLogic011_GetSummonPiece(int side, int kind, struct DotrFreeBlock *block, int packed);

#endif
