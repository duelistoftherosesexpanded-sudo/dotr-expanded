#ifndef DOTR_EE_LOGIC014_SUMMON_H
#define DOTR_EE_LOGIC014_SUMMON_H

/* Summon-cell choice of tactics logic 014: the cheapest reachable preference cell, else the
 * cell nearest an open line target, else the last low free cell.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* The block view (struct DotrFreeBlock: byte +0x3e, its top three bits the level), the
 * twelve-nibble list and the `_free` views of 0x24fe40, 0x278510, 0x27a130, 0x2792c0 and
 * 0x27afc0 are shared with the sibling copies in free_spots.h, as is SzDuel_Kind2Type. */
#include "dotr/ee/free_spots.h"

/* Callees as their groups declare them: TaLogic014_GetLine (0x14c9d0) and
 * TaLogic_GetWarnHideLvl__0014f9b0 (position_rules), SzDuel_GetTurnOwner (0x1de540,
 * state_records). Assumed: GetLine returns a packed cell (row << 8 | column) for index 0..2. */
extern int TaLogic014_GetLine(int index);
extern int SzDuel_GetTurnOwner(void);
extern int TaLogic_GetWarnHideLvl__0014f9b0(int side);

/* 0x14da40 TaLogic014_GetSummonPiece (name from the Japanese build's symbols). Assumed: the
 * parameter types of its siblings; the second parameter is overwritten by the level bits
 * (+0x3e >> 5) before it is read, and `int` is the type that reproduces the original. */
int TaLogic014_GetSummonPiece(int side, int kind, struct DotrFreeBlock *block, int packed);

#endif
