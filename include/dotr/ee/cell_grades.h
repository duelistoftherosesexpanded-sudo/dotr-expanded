#ifndef DOTR_EE_CELL_GRADES_H
#define DOTR_EE_CELL_GRADES_H

/* AI helpers above the library block, called from the AI rule functions below it, that grade
 * or act on one cell through state_grid queries.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* Callees, declared with the arguments this group passes. For 0x27d830
 * six int arguments and an int result are assumed. The others
 * are defined in state_grid (0x1e0290 there under another view). */
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int SzDuel_GetUnitKind(int side, int pos);
extern int func_001e0290_try(int a, int b, int flag);
extern int SzDuel_GetUnitDP(int side, int pos, int ignoreBoosts);
extern int TaLogic_GetActionReaction(int, int, int, int, int, int);

/* 2 when pos is 0 or word differs from SzDuel_GetUnitLoc(side, pos). Otherwise
 * reaction = TaLogic_GetActionReaction(side, pos, word & 0xff, word >> 8, 4, last) (a second call
 * with 5 is dropped). With SzDuel_GetUnitStatus(side, pos, 5) set the answer is 0 for
 * reaction 2 or 5 and 2 otherwise on every path. With it clear: 1 when
 * SzDuel_GetUnitKind(side, pos) >> 5 is not 0 and reaction is neither 2 nor 5; else 2
 * unless SzDuel_GetUnitAP(side, pos, 0) < SzDuel_GetUnitDP(side, pos, 0) or isMove3 is 1, and
 * then 1 for a reaction other than 2 and 5, else 2.
 * Assumed: the parameter types (word compared as an unsigned halfword, isMove3 as
 * an unsigned byte) and the int result; lower_rules.h declares an all-int view. */
int TaLogic_IsDefStyleUnit(int side, int pos, unsigned short word, unsigned char isMove3, int last);

/* Prefix view of the order record the rule functions pass (lower_rules.h and
 * position_rules.h hold fuller views): only the position halfword is read here. */
struct DotrGradeOrder {
    unsigned short position;
};
extern int Ta_GetCurrentSide(int side, int pos);
extern void TaLogic_GetUnitCtlLst2(int, unsigned short *words, int, int, int);
extern int TaLogic_GetMaxApUnit3(unsigned short *words, int key);
/* 0x27e780: (order, word, mode) with no result is assumed. */
extern void TaLogic_SetTaskAtkOrMove(struct DotrGradeOrder *order, int word, int mode);

/* side = 0x24de70 of the order's position; a 22-halfword list is filled with
 * 0xffff. When SzDuel_GetUnitKind(position) >> 5 is 0 and SzDuel_GetUnitStatus(position, 4) is set
 * the answer is 0. Otherwise TaLogic_GetUnitCtlLst2(side == 0, list, 52, 0, 0) fills the list
 * and TaLogic_GetMaxApUnit3(list, side == 0) picks: negative answers 0, else
 * TaLogic_SetTaskAtkOrMove(order, pick, mode) runs and the answer is 1. MATCHING DEVICE: the
 * first test is written as a two-arm switch, which reproduces the branch pair.
 * Assumed: int mode, int result, the callee signatures. */
int TaLogic_CommandGunnerUnit(struct DotrGradeOrder *order, int mode);

/* More callees, with the declarations other groups use (0x250360
 * through its `_pair` view name). */
extern int func_00250360_pair(int a, int b);
extern int Ta_IsActiveTrapB(int side, int trapEffect);
extern int GetCap(int limit, int value);
extern int SzDuel_GetMapUnitID(int x, int y);
extern int SzDuel_GetUnitCardNo(int side, int pos);

/* 0x27e5b0. 1 when Ta_IsActiveTrapB(Ta_IsUnitControl(ownerId >> 8, ownerId & 0xff), 4) is not
 * negative. Otherwise walks x from GetCap(0, cx - 1) to GetCap(0, cx + 1)
 * and y likewise around cy (both upper bounds re-read every step); for a cell
 * at distance |x - cx| + |y - cy| == 1 whose SzDuel_GetMapUnitID(x, y) word is not
 * negative, with a = word >> 8 and b = word & 0xff: answers 1 when
 * SzDuel_GetUnitKind(a, b) >> 5 is 3, SzDuel_GetUnitCardNo(a, b) is not 813 and Ta_IsUnitControl(a, b)
 * equals the first answer. 0 otherwise.
 * Assumed: three int parameters. */
int TaLogic_IsSafeTrap(int ownerId, int cx, int cy);

#endif
