#ifndef DOTR_EE_KIND_GATES_H
#define DOTR_EE_KIND_GATES_H

/* Functions that dispatch on a small callee answer through a compiler-made jump table and
 * gate a cell on callee answers.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* Callees, with the declarations other groups already use
 * (SzDuel_GetUnitAP through the `_try` view name). */
extern int SzDuel_GetUnitKindOfs(int side, int pos);
extern int SzDuel_GetUnitKind(int side, int pos);
extern int Ta_IsLDRangeAvilityWithLocation(int side, int unitArrayPos, int col, int row, int ability);
extern int TaUtil_GetUnitAP(int side, int pos, int col, int row);
extern int func_001e0290_try(int a, int b, int flag);

/* 0x24c480. Switches on SzDuel_GetUnitKindOfs(x, y) through the 13-entry table at
 * 0x30f9a0. The verdict is 1 when Ta_IsLDRangeAvilityWithLocation(a, b, c, d, 11) answers 0. Cases
 * 0-5 give the verdict when SzDuel_GetUnitKind(a, b) >> 5 is 0; 6, 7, 9 and 10 also
 * need TaUtil_GetUnitAP(a, b, c, d) below 501, 1001, 2001 and 3001; 8 needs
 * SzDuel_GetUnitAP(a, b, 0) below 1501; 12 gives the verdict when the shifted value
 * is NOT 0; 11 answers 1; everything else answers 0.
 * Assumed: six int parameters (the last two in t0 and t1). */
int TaTrapA_IsTrapAct(int x, int y, int a, int b, int c, int d);

/* More callees, declared as in state_records.h. */
extern int SzDuel_InqDestinyFlag(int side);
extern void SzDuel_SetDestinyFlag(int side, int value);
extern int SzDuel_GetTurnOwner(void);
/* Packed position for index 0..6, by whether the second argument is zero;
 * the SzDuel_GetTurnOwner result is discarded. Argument types follow the callers. */
int TaLogic017_GetLine(int index, int far);
/* Constant packed position by lineType (1 or not) and lineIndex; both arguments are
 * masked to a byte, so they are assumed to be unsigned char. */
int TaLogic002_GetLinePiece(unsigned char lineType, unsigned char lineIndex);
/* Constant packed position for index 0..7, 2 otherwise; int as the callers declare it. */
int TaLogic_GetCyclePoint(int index);
extern int TaHand_GetCardKind(int id);
extern int TaLogic005_Is2Head_ThunderDragon(int side, int *list);
extern int Ta_GetUnitNumFrom22bitFlagList(unsigned flagList);
/* 0 when Ta_GetUnitNumFrom22bitFlagList of the low byte of TaLogic005_Is2Head_ThunderDragon(side, hand) is 3;
 * otherwise 1 for a TaHand_GetCardKind(cardId) >> 5 of 0, 15 or 18 and 0 for the rest. The cardId is masked to
 * 16 bits, so it is assumed unsigned short; the hand type follows TaLogic005_Is2Head_ThunderDragon. */
int TaLogic005_IsKeepHandCombo(int side, int *hand, unsigned short cardId);
extern int TaLogic_IsAttackedNow(int a, int b, int c);
extern int SearchUnitByRange(int, int, int, int, int, int);
extern int Ta_GetUnitOwnerIDFromLstCount(int flagList, int n);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
/* 1 when TaLogic_IsAttackedNow(side, 0, 0) is 1, or when at least five entries of the
 * SearchUnitByRange(0, 0, side, 2, 52, 14) list have bit 4 clear in SzDuel_GetUnitStatus; else 0.
 * Callee signatures as the other groups declare them. */
int TaLogic019_IsUseGFK(int side);
#include "dotr/ee/position_order.h"
extern int func_001df540_word(int a, int b);
extern int func_0024de70_pair(int a, int b);
extern int SzDuel_GetUnitLoc(int, int);
/* Sets the order's kind and looks for a TaLogic002_GetLinePiece position that is empty or
 * holds the order's own id. Both flags are compared after a byte mask, so they
 * are assumed unsigned char; position_rules.h declares the int view its callers use. */
void TaLogic_CommandDefenseUnit(struct DotrPositionOrder *order, unsigned char lineType, unsigned char hasTarget);
extern int SzDuel_GetMapUnitID(int x, int y);
extern int func_00250360_pair(int a, int b);
/* Counts the TaLogic002_GetLinePiece positions whose SzDuel_GetMapUnitID id has a nonzero low byte and a
 * Ta_IsUnitControl result equal to SzDuel_GetTurnOwner(); needs two for defenseNo 1 (six positions) and
 * one otherwise (four). The argument is assumed unsigned char (compared after a
 * byte mask, passed on unmasked); pick_dispatch.h declares it as int. */
int TaLogic002_IsMakeDefense(unsigned char defenseNo);

/* 0x2b8700: rows of three halfwords read with index 0..15. Element type,
 * row count and object bounds are assumed from this one reader. */
extern unsigned short Destiny_Tab[][3];

/* 0x26f690. Stores 999 in cardPool[0..2], then switches on
 * SzDuel_GetUnitKind(side, 0) & 0x1f through the 21-entry table at 0x30fcb0 to preset
 * poolWeights[0..2]; other values answer -1. Also -1 when SzDuel_InqDestinyFlag(side) has bit
 * 0x80. Otherwise copies row (flags & 0xf) of 0x2b8700 to cardPool, adds
 * (flags & 0xf0) * 5 and half of it to poolWeights[0] and poolWeights[1], sets poolWeights[2] to
 * minus their sum, adds 10 / 20 / 70, stores flags | 0x80 (low byte) with
 * SzDuel_SetDestinyFlag, fills a non-null note with 114, SzDuel_GetTurnOwner(), 0, 0 and answers 1.
 * Assumed: int side, unsigned halfword `cardPool`, signed halfword `poolWeights`, a
 * note of four ints. */
int Ta_GetDestinyDraw(int side, unsigned short *cardPool, short *poolWeights, int *note);

/* 0x271c90. Switches on SzDuel_GetUnitConstKind(side, 0) & 0x1f through the 18-entry table
 * at 0x30fd10 and answers 1 when `objKind` is one the case accepts, else 0:
 * 1: 7;  2: 3;  3: 0 or 1;  5: 12;  6: 9 or 12;  7: 1;  9: 13;  11: 5 or 6;
 * 12: 15;  15: 2 or 14;  16: 14 or 17;  17: 4. The cases 0, 4, 8, 10, 13,
 * 14 and the values 18..31 answer 0.
 * Assumed: int side, objKind and result; the declaration of SzDuel_GetUnitConstKind is the
 * one state_grid.h and small_misc.h use. */
extern int SzDuel_GetUnitConstKind(int side, int pos);
int Ta_IsLD_PDownObj(int side, int objKind);

/* Item view for Ta_GetLDParamOfs2: only the byte at +0x3e is read. Size and the other
 * fields are unknown. */
struct DotrGateItem {
    char unknown_00[0x3e];
    unsigned char kind;
};

/* Callees of Ta_GetLDParamOfs2, declared as state_queries.h / state_grid.h
 * do (Ta_IsUnitControl2 through a `_gate` view name taking this group's item view). */
extern int func_00250370_gate(struct DotrGateItem *item);
extern int Ta_IsLDRangeAvilityFromLoc(int packed, int side, int kind, int ability);
extern int SzDuel_GetUnitLevel(int side, int pos, int ignoreBoosts);
extern int Ta_IsLDRange(int side, int col, int row);
extern int Ta_IsLDAbl(int ability, int side);

/* 0x271af0. Answers 0 for a null unit. Otherwise side = Ta_IsUnitControl2(unit),
 * kind = unit byte +0x3e & 0x1f, enemySide = 1 when side is 0, else 0. Adds
 * min(500, SzDuel_GetUnitLevel(side, 0, 0) * 100) when Ta_IsLDRangeAvilityFromLoc(row << 8 | col, side, kind,
 * 10) is not 0, and subtracts min(500, SzDuel_GetUnitLevel(enemySide, 0, 0) * 100) when
 * Ta_IsLDRange(enemySide, col, row), Ta_IsLDAbl(14, enemySide) and Ta_IsLD_PDownObj(enemySide, kind) are
 * all not 0. SzDuel_GetUnitLevel is called twice when its scaled answer is below 500.
 * Assumed: item pointer view, int col, row and result. */
int Ta_GetLDParamOfs2(struct DotrGateItem *unit, int col, int row);

/* SzDuel_GetMapUnit (0x1e36e0) with its usual signature; the item type is left incomplete
 * here and the result is cast to the gate view at the use. */
struct DotrStateItem;
extern struct DotrStateItem *SzDuel_GetMapUnit(int x, int y);

/* Answers 1 for loc 0x303 without a call. Otherwise 1 when the item at
 * (3, 3) exists, Ta_IsUnitControl2(item) differs from SzDuel_GetTurnOwner() and the low byte and
 * loc >> 8 are one step from (3, 3) (|x - 3| + |y - 3| == 1); else 0.
 * Assumed: int loc and result. */
int TaLogic001_IsLandNearCntr(int loc);

#endif
