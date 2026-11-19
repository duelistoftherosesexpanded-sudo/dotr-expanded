#ifndef DOTR_EE_CELL_PICKS_H
#define DOTR_EE_CELL_PICKS_H

/* An AI helper of the order_tries fallbacks that scans board cells and returns the best one
 * as a packed row/column value.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* Callees, declared with the arguments this caller passes; results and
 * parameter types are assumed int. Ta_IsUnitControl2 (0x250370) and
 * TaLogic_IsUnitWin4 (0x278510) are defined in small_misc with an object
 * pointer / int item and an unsigned char last parameter: these are views
 * taking the first argument as passed here. */
extern int SzDuel_GetField(int x, int y);
extern int SzDuel_GetMapUnitID(int x, int y);
extern int func_00250360_pair(int a, int b);
extern int func_00250370_word(int word);
extern int TaLogic_GetWarnHideLvl__00141b30(int side);
extern int func_00278510_word(int item, int packed, int c, int d, int e, int skip);

/* Called by TaLogic011_CommandDefenseUnit (order_tries) with an SzDuel_GetUnit (0x1df540) answer,
 * a column and a row.
 * For x 0..6 and y 0..6 with SzDuel_GetField(x, y) == 2 and cell = SzDuel_GetMapUnitID(x, y):
 * a negative cell qualifies; another qualifies when
 * Ta_IsUnitControl(cell >> 8, cell & 0xff) differs from Ta_IsUnitControl2(word) and
 * TaLogic_IsUnitWin4(word, cell, TaLogic_GetWarnHideLvl__00141b30(Ta_IsUnitControl2(word)), x, y,
 * 0) > 0. A qualifying cell with d = |column - x| + |row - y| in 0..best-1 (best starts at 14)
 * becomes the answer (y << 8) | x and lowers best to d. The answer is a local
 * with no initial value, so the result is indeterminate when no cell
 * qualifies. The first parameter is kept int as order_tries.h declares it. */
int TaLogic011_GetNearMountain(int word, int column, int row);

#endif
