#ifndef DOTR_EE_SUMMON_CELLS_H
#define DOTR_EE_SUMMON_CELLS_H

/* Preference order of the eight board cells around a side's leader unit, used to place summoned
 * pieces.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* SzDuel_GetUnitLoc (0x1e1090, state_grid): location of unit b of side a, as other groups
 * declare it. Assumed: x in the low byte, y in the next byte (board 7 x 7). */
extern int SzDuel_GetUnitLoc(int side, int pos);

/* 0x24e410. Assumed: side 0 or 1, i the preference index 0-7; the result is a cell
 * (y << 8) | x or -1. Only x + 1 and y + 1 are bounds-checked (against 7); the subtracting
 * cases are not checked. */
int Ta_GetSommonPiecePreference(int side, int direction);

#endif
