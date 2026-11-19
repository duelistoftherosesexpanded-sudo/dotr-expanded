#ifndef DOTR_EE_CELL_VERDICTS_H
#define DOTR_EE_CELL_VERDICTS_H

/* AI rule functions (below the library block) that take a cell (two coordinates) and answer a
 * small verdict code from the cell's SzDuel_GetUnitStatus / SzDuel_GetUnitKind /
 * Ta_GetUnitEffect queries.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

struct DotrPickBlock;

/* Callees, with the declarations other groups already use; the six int arguments of
 * TaLogic_GetActionReaction (0x27d830) are assumed. SzDuel_GetUnit (0x1df540) is defined in
 * state_records with another pointer type; the `_pick` view passes its answer on to
 * TaLogic003_IsSpellPlay (pick_copies). */
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int SzDuel_GetUnitKind(int side, int pos);
extern int TaLogic_GetActionReaction(int, int, int, int, int, int);
extern unsigned *func_00250740_word(int a, int b, int n);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern int func_00250360_pair(int a, int b);
extern struct DotrPickBlock *func_001df540_pick(int a, int b);
extern int TaLogic003_IsSpellPlay(int side, unsigned char *minTask, struct DotrPickBlock *unit);

/* 0x11c140. 0 when SzDuel_GetUnitStatus(x, y, 4) is non-zero. Otherwise 2 unless
 * SzDuel_GetUnitKind(x, y) >> 5 is 1, TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) is not 5 and
 * Ta_GetUnitEffect(x, y, 0) is non-null; then by that word >> 8: 13 answers 1; 2
 * answers 1 when the high halfword of the record's third word equals
 * SzDuel_GetUnitLoc(x, y); anything else answers 1 when SzDuel_GetUnitCardNo(x, y) equals
 * TaLogic003_IsSpellPlay(Ta_IsUnitControl(x, y), scratch, SzDuel_GetUnit(x, y)). 2 otherwise.
 * Assumed: int coordinates, a record of unsigned words, a 32-byte scratch
 * item (17..32 bytes give the same frame). */
int TaLogic_IsOpenUnit__0011c140(int x, int y, unsigned *record);

/* 0x10ee00. SzDuel_GetUnitLoc(x, y) is read first; 0 when SzDuel_GetUnitStatus(x, y, 4) is
 * non-zero. Then by SzDuel_GetUnitKind(x, y) >> 5 (a six-entry jump table): 0 answers 1
 * when SzDuel_GetUnitCardNo(x, y) is 402, 403, 427 or 428; 1 is the 0x11c140 walk without
 * its last comparison; 5 answers 1 when TaLogic_GetActionReaction is not 5,
 * TaLogic002_IsDoCerem(Ta_IsUnitControl(x, y), that SzDuel_GetUnitCardNo word) is 1 and SzDuel_GetField
 * of the first answer's low byte and remaining bits is not 9. 2 otherwise.
 * Assumed: TaLogic002_IsDoCerem takes two ints. */
extern int TaLogic002_IsDoCerem(int pair, int kind);
extern int SzDuel_GetField(int x, int y);
int TaLogic_IsOpenUnit__0010ee00(int x, int y, unsigned *record);

/* 0x130940: 0 when SzDuel_GetUnitStatus(x, y, 4) is set; else by SzDuel_GetUnitKind(x, y) >> 5:
 * 0 goes by the SzDuel_GetUnitCardNo word (298: 1; 322: 1 when flag's low byte is 1;
 * 326: 1 when at least four present SzDuel_GetUnit(side 0/1, 0..10) entries have a
 * Ta_IsUnitControl answer other than x); 1 answers 1 unless TaLogic_GetActionReaction is 5; 5 is the
 * 0x10ee00 case with TaLogic007_IsDoCerem as the gate. 2 otherwise.
 * Assumed: the third argument is never read; flag is an int. */
extern int TaLogic007_IsDoCerem(int value, int index);
struct DotrRuleOrder;
int TaLogic_IsOpenUnit__00130940(int x, int y, struct DotrRuleOrder *unused, int flag);

/* 0x141b90: 0 when SzDuel_GetUnitStatus(x, y, 4) is set; else by SzDuel_GetUnitKind(x, y) >> 5.
 * 0 goes by the SzDuel_GetUnitCardNo word: 368 / 386 want
 * TaLogic011_IsUseReversalGoddess / TaLogic011_IsUseBindChain of the Ta_IsUnitControl answer to
 * be 1; 383 wants Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, x, 1, 20, 14)) to be 3
 * or more; 388 wants SzDuel_GetLP(x) below the other side's SzDuel_GetLP (x is used as a side
 * there). 1 is the 0x11c140 walk with word classes 13 (SzDuel_GetLP of the Ta_IsUnitControl
 * answer below 2000), 18 (the set count with 8, at least 4) and 37
 * (TaLogic_IsAttackedNow(answer, 0, 0) is 1). 2 otherwise.
 * Assumed: SearchUnitByRange and TaLogic_IsAttackedNow take ints. */
extern int func_001df470_side(int side);
extern int TaLogic011_IsUseReversalGoddess(int side);
extern int TaLogic011_IsUseBindChain(int side);
extern int SearchUnitByRange(int, int, int, int, int, int);
extern int Ta_GetUnitNumFrom22bitFlagList(unsigned flagList);
extern int TaLogic_IsAttackedNow(int a, int b, int c);
int TaLogic_IsOpenUnit__00141b90(int x, int y);

/* 0x146220: 0 when SzDuel_GetUnitStatus(x, y, 4) is set; else by the SzDuel_GetUnitCardNo word:
 * 192 wants Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, x, 1, 5, 14)) to be 3 or more;
 * 325 wants three entries of the (x, 2, 52, 14) set whose cell (Ta_GetUnitOwnerIDFromLstCount
 * answer, high bits and low byte) has SzDuel_GetUnitStatus bit 3; 731 and 751 answer 2 when
 * TaLogic_GetActionReaction returns 5, else want TaLogic012_IsPinch of the Ta_IsUnitControl
 * answer to be 1; 846 is the
 * 0x10ee00 case 5 with TaLogic012_IsDoCerem(.., 846) as the gate. 2 otherwise. */
extern int Ta_GetUnitOwnerIDFromLstCount(int flagList, int n);
extern int TaLogic012_IsPinch(int side);
extern int TaLogic012_IsDoCerem(int value, int index);
int TaLogic_IsOpenUnit__00146220(int x, int y);

/* 0x117870: 0 when SzDuel_GetUnitStatus(x, y, 4) is set; else by SzDuel_GetUnitKind(x, y) >> 5.
 * 0: by the SzDuel_GetUnitCardNo word: 83 is 1 when SearchUnitByRange(0, 0, pair, 1, 0x2001c, 14)
 * or the same with 28 is non-zero; 146 is 1 when TaUtil_GetGraveCardNum(pair, 0) is at
 * least 6; 2 otherwise. 5: the 0x10ee00 case 5 with TaLogic001_IsDoCerem as the gate and
 * SzDuel_GetField(x, y). 1: 2 when TaLogic_GetActionReaction returns 5; else by the
 * SzDuel_GetUnitEffect(x, y, 0) word >> 8: 13 is 1, 2 is 1 when
 * TaLogic001_IsLandNearCntr(SzDuel_GetUnitLoc(x, y)) is 1, 20 is 2, any other forwards to
 * TaLogicCom_IsOpenUnit__00111320(x, y, z). 2 otherwise. The first SzDuel_GetUnitLoc answer is
 * discarded. Assumed: TaLogicCom_IsOpenUnit__00111320 takes three ints. */
extern unsigned *SzDuel_GetUnitEffect(int side, int pos, int effInd);
extern int TaUtil_GetGraveCardNum(int side, int kind);
extern int TaLogic001_IsDoCerem(int side, int cardNo);
extern int TaLogic001_IsLandNearCntr(int loc);
extern int TaLogicCom_IsOpenUnit__00111320(int x, int y, int z);
int TaLogic_IsOpenUnit__00117870(int x, int y, int z);

/* 0x1211b0: 0 when SzDuel_GetUnitStatus(x, y, 4) is set. Counts, over rows 1..4 of x and
 * of the other side (x == 0 ? 1 : 0), the cells whose SzDuel_GetUnitKind low five bits
 * are 14 and whose Ta_IsUnitControl answer is x. Then by SzDuel_GetUnitKind(x, y) >> 5 (jump
 * table): 0 goes by the SzDuel_GetUnitCardNo word (514: 1 for a count of 3 or more; 502:
 * fills a word list with 0xffff, has TaUtil_GetUnitLst(other side, 3, list) and
 * TaLogic_GetMaxApUnit(list) pick a cell and answers 1 when its Ta_IsUnitControl answer is not
 * the pair answer; 523: 1 when six or more of rows 1..10 on both sides have a
 * Ta_IsUnitControl answer other than the pair answer and a clear bit 4), else 2;
 * 1 answers 2 when TaLogic_GetActionReaction returns 5 or the Ta_GetUnitEffect word is null, 1 for
 * word >> 8 of 18 with a count of 3 or more, else 2; 2..5 and anything else answer 0.
 * The 502 list is 16 entries on the frame but 22 are written (an overrun in the
 * original code). */
extern void TaUtil_GetUnitLst(int sideToCheck, int compMode, unsigned short *units);
extern int TaLogic_GetMaxApUnit(unsigned short *ownerIdArray);
/* The third argument is not read; caller 0x11fae0 passes its order there. */
int TaLogic_IsOpenUnit__001211b0(int x, int y, struct DotrRuleOrder *unused);

/* 0x14ae60: 0 when SzDuel_GetUnitStatus(x, y, 4) is set. Then by SzDuel_GetUnitKind(x, y) >> 5.
 * 0 goes by the SzDuel_GetUnitCardNo word: 291 answers 1 when Ta_GetUnitNumFrom22bitFlagList of the
 * (0, 0, x, 1, 6, 14) set is 3 or more, 272 and 273 when that of the
 * (0, 0, x, 1, 0x1e001c, 14) set is positive, else 2. 5: 2 when TaLogic_GetActionReaction
 * returns 5; 1 when TaLogic013_IsDoCerem(Ta_IsUnitControl(x, y), word) is 1 and SzDuel_GetField
 * of the SzDuel_GetUnitLoc cell is not 9. 1: 2 when TaLogic_GetActionReaction returns 5 or the
 * SzDuel_GetUnitEffect(x, y, 0) word is null; by word >> 8: 13 is 1, 2 is 1 when
 * TaLogic_GetLandChange(SzDuel_GetUnit(x, y), cell low byte, cell >> 8, 2, 6, 2) is the
 * non-negative cell, 4 is 1 when TaLogic013_IsUseHarpySweaper(x, cell low byte, cell >> 8) is 1.
 * 2 otherwise. The first
 * Ta_IsUnitControl answer is discarded and the set is asked with x, not that answer. Assumed:
 * TaLogic_GetLandChange takes six ints; SzDuel_GetUnit is used here as an int. */
extern int TaLogic013_IsDoCerem(int side, int cardNo);
extern int func_001df540_word(int a, int b);
extern int TaLogic_GetLandChange(int, int, int, int, int, int);
extern int TaLogic013_IsUseHarpySweaper(int side, int col, int row);
int TaLogic_IsOpenUnit__0014ae60(int x, int y);

/* 0x14f490: as 0x14ae60 with other gates. 0: words 551 and 612 answer 1 when
 * TaLogic014_IsUnitNaturalEffectUse(Ta_IsUnitControl(x, y), word) is 1, else 2. 5: gate TaLogic014_IsDoCerem.
 * 1: word >> 8 of 13 is 1; 2 asks TaLogic_GetLandChange(.., 4, 9, 2); 4 is 1 when none of the three
 * TaLogic014_GetLine spots has a null SzDuel_GetMapUnit item and SzDuel_GetUnitSpellCount(x) is 0 (a free
 * spot is asked from TaLogic014_GetLine a second time and that answer's sign is tested);
 * 18 is 1 when SzDuel_GetUnitMonsterCount(x) is 4 or more. 2 otherwise. */
struct DotrStateItem;
extern int TaLogic014_IsUnitNaturalEffectUse(int side, int cardNo);
extern int TaLogic014_IsDoCerem(int value, int index);
extern int TaLogic014_GetLine(int index);
extern struct DotrStateItem *SzDuel_GetMapUnit(int x, int y);
extern int SzDuel_GetUnitSpellCount(int side);
extern int SzDuel_GetUnitMonsterCount(int side);
int TaLogic_IsOpenUnit__0014f490(int x, int y);

/* 0x1620b0: as 0x14ae60 without a group 0. 5: gate TaLogic019_IsDoCerem. 1: by word >> 8:
 * 4 is 1 when the SzDuel_GetUnitLoc cell of (x, y) shares its low byte or its high
 * part with the SzDuel_GetUnitLoc cell of the TaLogic019_GetUseThunderBolt(pair) spot (asked as spot >> 8,
 * spot low byte); 73 is 1 when TaLogic019_IsUseGFK(pair) is 1; 37 is 1 for word 749
 * when TaLogic_IsAttackedNow(x, 0, 0) is 1 and for word 746 when TaLogic_IsAttackedNow(x, y, 0) is 1;
 * 31 is 1 for word 733 when TaLogic019_IsUseRevolt(pair) is 1. 2 otherwise. Assumed:
 * TaLogic019_IsUseRevolt takes one int. */
extern int TaLogic019_IsDoCerem(int value, int index);
extern int TaLogic019_GetUseThunderBolt(int side);
extern int TaLogic019_IsUseGFK(int side);
extern int TaLogic019_IsUseRevolt(int side);
int TaLogic_IsOpenUnit__001620b0(int x, int y);

/* 0x15cbb0: as 0x14ae60 with other gates; side is Ta_IsUnitControl(x, y) and the cell
 * is SzDuel_GetUnitLoc(x, y). 0: word 224 is 1 when SzDuel_GetSummonPower(side) is under 2; 652 is 2
 * when Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(cell low, cell high, side, 1, 52, 20)) is 2 or more,
 * else 1 when the same with 2 is 6 or more; 655 is 1 when
 * Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, x, 2, 52, 14)) is 6 or more. 5: gate
 * TaLogic018_IsDoCerem. 1: word >> 8 of 24 as word 655; 18 is 2 when
 * Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(cell low, cell high, side, 1, 29, 29)) is positive, else
 * 1 when Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, side, 1, 19, 14)) is 3 or more. 2 otherwise.
 * Assumed: argument types follow the callees' declarations. */
extern int SzDuel_GetSummonPower(int side);
extern int TaLogic018_IsDoCerem(int side, int cardNo);
int TaLogic_IsOpenUnit__0015cbb0(int x, int y);

#endif
