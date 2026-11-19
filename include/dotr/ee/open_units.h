#ifndef DOTR_EE_OPEN_UNITS_H
#define DOTR_EE_OPEN_UNITS_H

/* TaLogic_IsOpenUnit copies of several logic units (first: logic 008, 0x134b40).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* Callees, with the assumed prototypes other groups already use. The unit
 * record SzDuel_GetUnit (0x1df540) returns is only tested against zero here, so it is
 * viewed as a word; 0x250360 is called with a side and a slot. */
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern int func_00250360_pair(int a, int b);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern unsigned *SzDuel_GetUnitEffect(int side, int pos, int effInd);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int SzDuel_GetUnitKind(int side, int pos);
extern int TaLogic_GetActionReaction(int, int, int, int, int, int);
extern int TaLogic008_IsDoCerem(int side, int cardNo);
extern int SzDuel_GetField(int x, int y);
extern int TaLogic008_GetWallEmpty(int side);
extern int func_001df540_word(int a, int b);

/* 0x134b40: 0 when status bit 4 is set, 1 to open the unit, 2 otherwise. */
int TaLogic_IsOpenUnit__00134b40(int x, int y);

/* Further callees of the logic 017 copy (0x158360), with the assumed
 * prototypes other groups use. The object passed third to 0x155c90 is a
 * 96-byte local of the caller (stack 144..240); only its size is observed
 * here, and it is passed without a cast because a cast changes the order in
 * which the two buffer addresses are formed. */
struct DotrRuleOrder;
struct DotrOpenCardInfo {
    int unknown_00[24];
};
extern void TaHand_GetNextHand(int index, int *out);
extern int TaLogic017_IsExzParts(int id);
extern int TaLogic017_IsExzodia(int side);
extern int SzDuel_GetLP(int side);
extern int SearchUnitByRange(int, int, int, int, int, int);
extern int Ta_GetUnitNumFrom22bitFlagList(unsigned flagList);
extern int Ta_GetUnitOwnerIDFromLstCount(int flagList, int n);
extern int TaLogic_IsJudgeUnitDanger(int side, int pos, int canSeeFaceDown);
extern int TaHand_GetNextHandNum(int side);
extern int TaHand_GetCardKind(int id);
extern int SzDuel_Kind2Type(int kind);
extern int TaLogic017_IsExzSafe(int side);
extern int TaLogic017_PlayExzCombo(int side, unsigned char *item, struct DotrOpenCardInfo *unit);
extern int Ta_GetUnitNum(int side, unsigned char kind);

/* TaLogic_IsOpenUnit__00158360 (logic 017): same results as 0x134b40. The third parameter is
 * never read; it is declared because both callers (0x157480,
 * 0x157a50) pass their order pointer in a2. */
int TaLogic_IsOpenUnit__00158360(int x, int y, struct DotrRuleOrder *order);

/* TaLogic_IsOpenUnit__0013d3d0 (logic 010): same results as 0x134b40; the third parameter is
 * never read (both callers pass their order pointer in a2). The
 * 0x13d2c0 answer is used as (side << 8) | slot for SzDuel_GetUnitLoc. */
extern int TaLogic010_IsDoCerem(int side, int cardNo);
extern int TaLogic010_GetUseThunderBolt(int side);
int TaLogic_IsOpenUnit__0013d3d0(int x, int y, struct DotrRuleOrder *order);

/* TaLogic_IsOpenUnit__00126380 (logic 005): same results as 0x134b40; the third parameter is
 * never read (the callers pass their order pointer in a2). In the
 * card-565 scan the status query is made with the unit's own slot, not the
 * scanned one. */
extern int TaLogic005_IsDoCerem(int side, int cardNo);
int TaLogic_IsOpenUnit__00126380(int x, int y, struct DotrRuleOrder *order);

/* TaLogic_IsOpenUnit__0012b210 (logic 006): same results as 0x134b40; the third parameter is
 * never read (the callers pass their order pointer in a2). The
 * unit-control answer for the unit itself is fetched and discarded. */
extern int TaLogic006_IsDoCerem(int value, int index);
extern int TaLogic_IsAttackedNow(int a, int b, int c);
extern int TaLogic006_IsBlackHoleCondition(int side);
extern int TaLogic_GetMaxAPGraveCard(int side, int mask);
extern int SzDuel_GetUnitMonsterCount(int side);
int TaLogic_IsOpenUnit__0012b210(int x, int y, struct DotrRuleOrder *order);

/* TaLogic_IsOpenUnit__001670a0 (logic 020): 0 when status bit 4 is set, otherwise 2 or 1 as for
 * 0x134b40, except that one path (effect 4 with a second word other than
 * 18 << 16) returns the number of list units it scanned. The byte counter
 * is tested for a negative value on both effect-4 paths, which cannot
 * happen. The third parameter is never read (the callers pass
 * their order pointer in a2). */
extern unsigned ConvUnitIDtoFlagList_OFF(unsigned flagList, int side, int pos);
extern int Ta_RemoveObjFromFlagLst(int flagList, int side);
extern int func_001df470_side(int side);
extern int SzDuel_GetUnitCount(int);
extern int SzDuel_InqHandPhase(int side);
extern int TaLogic020_IsDoCerem(int side, int cardNo);
int TaLogic_IsOpenUnit__001670a0(int x, int y, struct DotrRuleOrder *order);

/* TaLogic_IsOpenUnit__00139180 (logic 009). Card 613 and effect 32 count slots 1..11 of both sides
 * with a curse rest (above zero / not zero) that x controls, bound 2. The sacrifice test and the effect-23
 * condition are expanded in place. TaHand_GetCardEffect's result is assumed to be two words
 * (only +4 is read); SzDuel_GetUnit's pointer is passed on as a word. Two parameters are read. */
struct DotrOpenPair {
    int unknown_0;
    int unknown_4;
};
extern struct DotrOpenPair *TaHand_GetCardEffect(int cardId, int effectIndex);
extern void Ta_GetCeremSacrifice(int *Buf, int Obj);
extern int Ta_GetSacrificeUnit(int *sacrifices, int side);
extern int TaUtil_GetUnitAP(int side, int pos, int col, int row);
extern int SzDuel_GetUnitCurseRest(int side, int pos);
extern int TaLogic_GetLandChange(int, int, int, int, int, int);
extern int TaLogic009_GetRevoltObj(int side);
int TaLogic_IsOpenUnit__00139180(int x, int y);

#endif
