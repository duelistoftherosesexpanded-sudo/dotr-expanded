#ifndef DOTR_EE_SLOT_SEEKS_H
#define DOTR_EE_SLOT_SEEKS_H

/* 0x26ac90 and its caller 0x26ab80: a search of slots 6..10 on one or both sides for the entry
 * with a given id and the lowest 0x1e13c0 answer, and the id-to-side-mask selection in front of
 * it; and the TrapB.c decision checks 0x26b1f0 and 0x26ae70.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* Callees, declared as in lower_rules.h (0x250740's word view, 0x1e0700),
 * state_queries.h (0x266df0) and state_grid.h (0x1e13c0). */
extern int Ta_SearchUncTypeNaturalEffectUserNum(int kind);
extern unsigned *func_00250740_word(int a, int b, int n);
extern int SzDuel_GetUnitKind(int side, int pos);
extern int SzDuel_GetUnitSummonOrder(int side, int pos);

/* 0x26ac90: -1 when Ta_SearchUncTypeNaturalEffectUserNum(15) is positive. Otherwise looks at slots 6..10
 * of `side` when bit 0 of sidesToCount is set and then of the other side (1 for side
 * 0, else 0) when sidesToCount >> 1 is nonzero: a slot counts when Ta_GetUnitEffect(side,
 * slot, 0) is non-null, SzDuel_GetUnitKind(side, slot) >> 5 is 4, the entry's first
 * word >> 8 equals trapEffect and SzDuel_GetUnitSummonOrder(side, slot) is below the lowest answer so
 * far (starting at 255; the callee is asked a second time for the value
 * kept). Returns (side << 8) | slot of the last slot kept, or -1 when none
 * was (the initial 255 is the "none" value). Parameter meanings are assumed. */
int Ta_GetActiveTrapBUnit(int side, int sidesToCount, unsigned trapEffect);

/* 0x26ab80: Ta_GetActiveTrapBUnit(side, 3, trapEffect) for effects 43, 46, 17, 45, 76 and 80,
 * Ta_GetActiveTrapBUnit(side, 2, trapEffect) for 49, 48, 4, 14, 75, 74, 35, 44, 42 and 41,
 * -1 for any other effect. */
int Ta_IsActiveTrapB(int side, int trapEffect);

/* Callees of 0x26b1f0, declared as in state_queries.h, state_grid.h and
 * small_misc.h; the unit record is only passed on here. */
struct DotrStateItem;
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int Ta_GetCurrentSide(int side, int pos);
extern int Ta_IsLDRangeAvilityWithLocation(int side, int unitArrayPos, int col, int row, int ability);
extern int Ta_IsUnitControl(int a, int b);
extern struct DotrStateItem *SzDuel_GetUnit(int side, int pos);
extern int Ta_IsUnitHide(struct DotrStateItem *unit, int side);
extern int Ta_IsResistAll(int side, int pos);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);

/* Ta_IsTrapBInDecideOperation2 (TrapB.c). Prototype and local names from the Japanese build's
 * debug information. 0 when nID is 0. 1 when the unit is in range
 * (0x271f90 with its location and 11) and not hidden from the side that
 * does not control it. 0 when 0x267700 answers. Then by stDefense: 0 looks
 * for trap 44 seen from the unit's current side, anything else for 74 and,
 * only when 74 is absent, 35. A trap found counts when IsView is set or its
 * status bit 4 is; it is cancelled (0) by an active trap 46 seen from the
 * other side of nSide. Results: 0x10001 for 44 and 74, 1 for 35, else 0.
 * What the results mean to the caller is not established. */
int Ta_IsTrapBInDecideOperation2(int nSide, int nID, int stDefense, unsigned char IsView);

/* Further callees of 0x26ae70, declared as in small_misc.h and cell_tints.h. */
extern int Ta_IsLDAbl(int ability, int side);
extern void Ta_SetRangeVisual(int x, int y, int c, int reach, int e, int mode, int visualType);

/* Effect-call record written by 0x26ae70. Member names and offsets from the
 * Japanese build's debug information (TrapB.c); the tag is unnamed there. The
 * same layout is struct jp_6989c in effect_seqs.h. */
struct jp_749a8 {
    int eNO;
    unsigned int dwFrom;
    unsigned int dwTo;
    unsigned int dwPar;
};

/* Ta_IsTrapBInDecideOperation (TrapB.c). Prototype and local names from the Japanese build's
 * debug information. Same tests as 0x26b1f0 up to the trap search, except
 * that being in range only sets a flag. A trap found (44 for stDefense 0,
 * else 74, then 35) fills *eInfo: with the range flag, the range of the
 * current side's unit 0 is shown (0x260490) and the record is 254, 0,
 * Self << 8, 16, result 1; with an active trap 46 on the other side of nSide
 * it is 102, 1, that trap, 0, result 1; otherwise 70 (trap 44) or 71, the
 * trap, nSide << 8 | nID and 0 (1 for trap 74), result 0x10001 (1 for trap
 * 35). 0 with *eInfo untouched in every other case. What the record numbers
 * mean is not established. */
int Ta_IsTrapBInDecideOperation(int nSide, int nID, int stDefense, struct jp_749a8 *eInfo);

#endif
