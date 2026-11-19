#ifndef DOTR_EE_LOGIC017_SUMMON_H
#define DOTR_EE_LOGIC017_SUMMON_H

/* Summon-cell choice of tactics logic 017: a target line cell and the nearest free neighbour
 * of the leader.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* View of the card block passed as the third argument (position_rules.h struct
 * DotrPositionBlock has the same layout). Assumed: +0x3c a card identifier, the top three bits
 * of the byte at +0x3e a "far" line selector handed to TaLogic017_GetLine. */
struct DotrLogic017SummonBlock {
    unsigned char unknown_00[0x3c];
    unsigned short unknown_3c;
    unsigned char unknown_3e;
    unsigned char unknown_3f[0x21];
};

/* Callees as their groups declare them (state_grid, kind_gates, state_queries,
 * summon_cells); SzDuel_GetMapUnit's result is only tested against null here. */
struct DotrStateItem;
extern int SzDuel_GetUnitLoc(int side, int pos);
extern struct DotrStateItem *SzDuel_GetMapUnit(int x, int y);
extern int SzDuel_GetMapUnitID(int x, int y);
extern int SzDuel_GetField(int x, int y);
extern int TaLogic017_GetLine(int index, int far);
extern int TaLogic017_IsExzParts(int id);
extern int Ta_GetSommonPiecePreference(int side, int direction);

/* TaLogic017_GetEmptyExzPiece (0x156ea0, grid_scans) and TaLogic017_GetEmptyTrapPiece
 * (0x156f70, position_rules) are declared there as taking no argument, but this caller passes
 * the side in a0, so these are distinct views with one int argument. */
extern int func_00156ea0_summon(int side);
extern int func_00156f70_summon(int side);

/* 0x156b40. Assumed: the second argument is unused (never read). */
int TaLogic017_GetSummonPiece(int side, int unused, struct DotrLogic017SummonBlock *block);

#endif
