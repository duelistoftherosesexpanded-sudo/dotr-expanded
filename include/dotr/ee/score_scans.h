#ifndef DOTR_EE_SCORE_SCANS_H
#define DOTR_EE_SCORE_SCANS_H

/* AI rule functions (below the library block) that walk a cell set (0x24ca40) or a
 * 22-halfword list (0x27c070) and compare 0x2509e0 scores of its entries.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* Callees, declared with the arguments these callers pass; results and
 * parameter types are assumed int unless a caller shows otherwise. Other group
 * headers hold different views of the same functions. */
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int Ta_SearchUncTypeNaturalEffectUserNum(int kind);
extern int SearchUnitByRange(int, int, int, int, int, int);
extern int Ta_RemoveObjFromFlagLst(int flagList, int side);
extern void TaLogic_GetUnitCtlLst(int, unsigned short *words, int, int);
extern int TaLogic_GetUnitCtlLstLen(unsigned short *units);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int SzDuel_GetUnitCurseRest(int side, int pos);
extern int SzDuel_GetUnitSpellRest(int side, int pos);
extern int TaUtil_GetUnitAP(int side, int pos, int col, int row);
extern int Ta_GetUnitOwnerIDFromLstCount(int flagList, int n);
extern int Ta_GetUnitNumFrom22bitFlagList(unsigned flagList);
extern int func_00250360_pair(int a, int b);
extern void TaLogic_GetUnitCtlLst2(int, unsigned short *words, int, int, int);
extern int TaLogic_GetMaxApUnit(unsigned short *ownerIdArray);
extern int TaLogic_IsUnitWin(int unit1OwnerId, int unit2, int col, int row);
extern int SzDuel_GetUnitKind(int side, int pos);

/* 0x1110d0, its byte-identical copy 0x16b3c0 and TaLogic009_GetRevoltObj (last 0x27c070
 * argument 1 instead of 0). -1 when Ta_SearchUncTypeNaturalEffectUserNum(57) is positive. Otherwise takes
 * the SzDuel_GetUnitLoc(side == 0, 0) halfword as a home cell, fills a 22-halfword list
 * with 0xffff and passes it to TaLogic_GetUnitCtlLst(side == 0, list, 52, 0 or 1). Among
 * the TaLogic_GetUnitCtlLstLen(list) entries whose 0x1e1090 cell is one step (row plus column
 * distance 1) from the home cell and whose SzDuel_GetUnitStatus(.., 1), 0x1e1580 and
 * 0x1e1640 answers are all 0, keeps the entry with the greatest
 * TaUtil_GetUnitAP(row, column of the entry, cell column, cell row) score of at least
 * 1 (the later one on ties). -1 when none qualifies; otherwise that entry in
 * the high halfword and, in the low halfword, the last kept entry equal to
 * Ta_GetUnitOwnerIDFromLstCount(Ta_RemoveObjFromFlagLst(SearchUnitByRange(0, 0, side, 2, 33, 14),
 * side), 0), or 0xffff. */
int TaLogicCom_GetRevoltObj__001110d0(int side);
int TaLogicCom_GetRevoltObj__0016b3c0(int side);
int TaLogic019_IsUseRevolt(int side);
int TaLogic009_GetRevoltObj(int side);
/* 0x1123c0 and its copy TaLogicCom_IsUseObjEffect__0016c6b0 (limit 2500 instead of 3000). Walks the
 * SearchUnitByRange(0, 0, side, sidesToMatch, searchMode, 14) set and keeps the greatest 0x2509e0
 * score of the entries whose 0x250360 answer is side and of the others
 * (both start at -1). 1 when the others' best exceeds both the side's best
 * and the limit. Otherwise 0 when sidesToMatch bit 0 is set and the (1, searchMode, 14)
 * set is not empty; 1 when sidesToMatch >> 1 is non-zero and the (2, searchMode, 14) set
 * holds more than 3; else 0. 0x16c6b0 calls the second parameter sideComp. The code first counts to 22 with no effect. */
int TaLogicCom_IsUseObjEffect__001123c0(int side, int sidesToMatch, int searchMode);
int TaLogicCom_IsUseObjEffect__0016c6b0(int side, int sideComp, int searchMode);
/* 0x112600 and its byte-identical copy 0x16cbb0. Fills a 22-halfword list
 * with 0xffff, then acts on the low six bits of the record's first word
 * (the record is assumed to start with an unsigned word). 29: takes the
 * 0x27b650 answer of the list after TaLogic_GetUnitCtlLst2(side == 0, list, 29, 0, 0) and,
 * after refilling, after TaLogic_GetUnitCtlLst(side, list, 29, 1); when neither is
 * negative returns the SzDuel_GetUnitLoc(first >> 8, first & 0xff) cell if
 * TaLogic_IsUnitWin(first, second, cell & 0xff, cell >> 8) is not negative. 30: for
 * each x and y in 0..6 counts the SearchUnitByRange(x, y, side, 1, 52, 18) entries
 * whose SzDuel_GetUnitStatus(.., 4) answer is 0 or whose 0x1e0700 answer >> 5 is
 * non-zero (count kept in a byte) and returns y << 8 | x for the first
 * position with more than 2. -1 otherwise. */
int TaLogicCom_GetUseXYRangeSpell__00112600(int side, unsigned *record);
int TaLogicCom_GetUseXYRangeSpell__0016cbb0(int side, unsigned *effect);

/* SzDuel_GetUnit (state_records) returns a pointer; only its null test is used
 * here, through this int view. */
extern int func_001df540_word(int a, int b);
/* 0x12b000. Fills a 22-halfword list with 0xffff and takes the 0x27b650
 * answer of the list after TaLogic_GetUnitCtlLst2(side == 0, list, 29, 0, 0) and, after
 * refilling, after TaLogic_GetUnitCtlLst2(side, list, 29, 1, 1). Each answer is scored as
 * TaUtil_GetUnitAP(hi, lo, cell & 0xff, cell >> 8) with cell = SzDuel_GetUnitLoc(hi, lo), read
 * twice. Only when the second score is below the first and the first is at
 * least 3000 are the non-zero SzDuel_GetUnit(side, 1..10) answers counted. Returns
 * 1 when fewer than 3 were counted, otherwise 0. The answers are not tested
 * for a negative value. MATCHING DEVICE: the order in which the seven
 * locals are declared. */
int TaLogic006_IsBlackHoleCondition(int side);

#endif
