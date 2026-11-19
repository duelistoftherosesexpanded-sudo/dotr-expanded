#ifndef DOTR_EE_SEARCH_RANGE_H
#define DOTR_EE_SEARCH_RANGE_H

/* The AI's unit search SearchUnitByRange, which collects into a flag word the units within a board
 * distance or on a field kind that pass one of many filters.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* Callee prototypes; the callees are defined in state_grid, small_misc and state_queries.
 * SzDuel_GetUnitCardNo is declared inside the function without a prototype. Their meanings
 * (assumed, from the callees' own code) are in docs/modules/other.md. */
extern int SzDuel_GetUnitKind(int side, int pos);
extern int SzDuel_GetUnitAttr(int side, int pos);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int SzDuel_GetUnitSummonOrder(int unused0, int unused1);
extern int SzDuel_GetUnitStatus(int side, int pos, int arg2);
extern int SzDuel_GetField(int arg0, int arg1);
extern int ConvUnitIDtoFlagList(unsigned int flagList, int teamId, int unitArrayPos);
extern void TaUtil_GetUnitLst(int sideToCheck, int compMode, short *units);
extern int Ta_SideMatch(int sideA, int sideB, int compMode);
extern int Ta_GetCurrentSide(int side, int pos);
extern int TaUtil_GetUnitAP(int side, int pos, int col, int row);
extern int TaUtil_GetUnitDP(int side, int pos, int col, int row);
extern int TaLogic_GetUnitCtlLstLen(unsigned short *units);

/* 0x24ca40 SearchUnitByRange(col, row, side, compMode, searchMode, range): lists the units (side
 * and compMode as TaUtil_GetUnitLst) and returns the flag word (ConvUnitIDtoFlagList) of those
 * that pass. range 18 (same column or row), 20 (forest), 22 (mountain), 23 (meadow), 24 (sea) and
 * 29 (crush) select by row/column or by field kind; any other range is a board (Manhattan)
 * distance limit, with the low half of searchMode choosing one of many tests (kind, attribute,
 * status, attack/defence against searchMode's high half, the strongest or weakest single unit,
 * ...). The third parameter keeps its placeholder name: the body has a local `side`. */
int SearchUnitByRange(int col, int row, int arg2, int compMode, int searchMode, int range);

#endif
