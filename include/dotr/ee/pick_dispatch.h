#ifndef DOTR_EE_PICK_DISPATCH_H
#define DOTR_EE_PICK_DISPATCH_H

/* 0x150f10 and its look-alikes (0x14ce30, 0x13f170, 0x15a5f0, 0x164680, 0x155a00): read two
 * per-side bytes through TaLogic_GetUnitNum, fill an id list with TaHand_GetNextHand, then try
 * a fixed sequence of pick routines and return a small code.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

struct DotrStateBlock;
struct DotrPickBlock;

/* Callees: prototypes as declared in their own groups (assumed, not recovered). */
extern int TaLogic_GetUnitNum(int side, int countSpells);
extern void TaHand_GetNextHand(int index, int *out);
extern int TaHand_GetNextHandNum(int side);
extern int TaHand_IsCardSummon2(int side, int cardNo);
extern int TaHand_GetCardKind(int id);
extern void SzDuel_InitCardInfo(struct DotrStateBlock *unit, int cardId);
extern int TaLogic015_GetSpellPlay(int side, unsigned char *minTask, struct DotrStateBlock *unit, int cardID);
extern int TaLogic015_GetSpecialComboPlay(int side, unsigned char *item, struct DotrPickBlock *block);
extern int TaLogic015_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int cardId);
/* 0x153f30: one argument and an int result are what the call site shows. */
extern int TaLogic015_IsUseRevive(int side);
extern int TaLogic014_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
extern int TaLogic014_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
extern int TaLogic014_IsDoCerem(int value, int index);
extern int TaLogic014_IsActiveMode(void);
extern int SzDuel_GetUnitSpellCount(int side);
extern int SzDuel_GetUnitMonsterCount(int side);
/* 0x142280: one argument and an int result are what the call site shows. */
extern int TaLogic011_IsUseReversalGoddess(int side);
extern int TaLogic011_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
extern int TaLogic011_GetSpecCardPlay(int side, int *ids, struct DotrStateBlock *block, unsigned char *item, int id);
extern int TaLogic011_GetSpecComboPlay(int side, unsigned char *minTask, struct DotrPickBlock *targetUnit);
extern int TaLogic018_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
extern int TaLogic018_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
extern int TaLogic018_IsDoCerem(int side, int cardNo);
extern int TaLogic020_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
extern int TaLogic020_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
extern int TaLogic020_IsDoCerem(int side, int cardNo);
extern int TaLogic020_GetTrapCardPlay(int index, unsigned char *minTask, void *unit);
extern int TaLogic017_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *unit, int cardNo);
extern int SzDuel_Kind2Type(int kind);
extern int TaLogic017_IsExzSafe(int side);
/* 0x155c90: three arguments and an int result are what the call site shows. */
extern int TaLogic017_PlayExzCombo(int side, unsigned char *item, struct DotrStateBlock *unit);
extern int TaLogic008_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
extern int TaLogic008_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
extern int TaLogic008_IsDoCerem(int side, int cardNo);
struct DotrStateItem;
/* Only tested against zero here. */
extern struct DotrStateItem *SzDuel_GetUnit(int side, int pos);
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern int func_00250360_pair(int a, int b);
extern int TaLogic021_GetSpellPlay(int side, unsigned char *task, struct DotrStateBlock *unit, int card);
extern int TaLogic021_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *unit, int id);
extern int TaLogic021_IsUseBlackHole(int side);
extern int SzDuel_GetSummonPower(int side);
extern int TaLogic010_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
extern int TaLogic010_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
extern int TaLogic010_IsDoCerem(int side, int cardNo);
extern int TaLogic010_GetUseThunderBolt(int side);
extern int SzDuel_GetUnitCurseRest(int side, int pos);
extern int TaLogic013_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
extern int TaLogic013_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int cardId);
extern int TaLogic013_IsDoCerem(int side, int cardNo);
extern int TaLogic013_GetSpecialComboPlay(int side, unsigned char *item, struct DotrPickBlock *PUnit);
extern int SearchUnitByRange(int, int, int, int, int, int);
extern int Ta_GetUnitNumFrom22bitFlagList(unsigned flagList);
/* Callees of 0x119a40, defined in pick_copies / position_rules with these signatures. */
extern int TaLogic003_GetSpellPlay(int side, unsigned char *minTask, struct DotrStateBlock *unit, int cardId);
extern int TaLogic003_IsSpellPlay(int side, unsigned char *minTask, struct DotrPickBlock *unit);
extern int TaLogic003_IsEmptyAtkLine(int side, unsigned char lineIndex);
/* Callees of 0x114310, defined elsewhere with these signatures. */
extern int TaLogic001_GetSpellPlay(int side, unsigned char *task, struct DotrStateBlock *unit, int cardNo);
extern int TaLogic001_GetSpecCardPlay(int side, unsigned char *task, struct DotrStateBlock *unit, int cardNo);
extern int TaLogic001_IsDoCerem(int side, int cardNo);
extern int TaLogic001_IsUseLordOfDragon(int side);
extern int TaLogic001_IsUseLifeHourGlass(int side);
extern int TaLogic001_IsUseBraveHourGlass(int side);
extern int TaLogic001_IsUseRevive(int side);
/* 0x1147b0: three arguments and a sign-tested int result are what the call site shows. */
extern int TaLogic001_GetProtectCardPlay(int side, unsigned char *task, struct DotrStateBlock *unit);
/* 0x14b490: one argument and an int result are what the call site shows. */
extern int TaLogic013_IsUseRevive(int side);

/* 0x10f450 and 0x10f290 are defined in pick_copies (same declarations as pick_copies.h). */
extern int TaLogic002_GetSpellPlay(int side, unsigned char *minTask, struct DotrStateBlock *unit);
extern int TaLogic002_GetSpecialComboPlay(int side, unsigned char *minTask, struct DotrPickBlock *unit, unsigned char mode);
/* 0x10f6f0: one constant argument and a zero-tested result are what the call sites show. */
extern int TaLogic002_IsMakeDefense(int defenseNo);

/* Callees of 0x1280c0, defined in pick_copies, score_scans and state_queries (same declarations),
 * except TaLogic_IsAttackedNow (0x27bbc0, driver_scans): three arguments and a result compared
 * with 1 are what the call site shows. */
extern int TaLogic006_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
extern int TaLogic006_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *unit, int id);
extern int TaLogic_IsAttackedNow(int a, int b, int c);
extern int TaLogic006_IsBlackHoleCondition(int side);
extern int TaLogic_GetMaxAPGraveCard(int side, int mask);
extern int TaLogic006_IsDoCerem(int value, int index);

/* Callees of 0x143de0, defined in pick_copies, lower_rules, grid_scans and state_grid (same
 * declarations; the block is passed to 0x1e02f0 through a cast). */
extern int TaLogic012_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
extern int TaLogic012_GetSpecCardPlay(int side, int *ids, struct DotrStateBlock *block, unsigned char *item, int id);
extern int TaLogic012_IsDoCerem(int value, int index);
extern int TaLogic012_IsPinch(int side);
extern int TaLogic012_IsLineEmpty(int id, int y);
struct DotrStateItem;
extern int SzDuel_GetUnitAP2(struct DotrStateItem *unit, int ignoreBoosts);

/* Callees of 0x136490, defined in pick_copies, score_scans, lower_rules and grid_scans (same
 * declarations). */
extern int TaLogic009_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *unit, int targetCardID);
extern int TaLogic009_GetSpellPlay(int side, unsigned char *minTask, struct DotrStateBlock *spellUnit, int cardID);
extern int TaLogic009_IsDoCerem(int side, int cardNo);
extern int TaLogic009_GetRevoltObj(int side);
extern int TaLogic009_IsUseCurseDragonPot(int side);
extern int TaLogic009_IsUseCopyCat(int side);

/* Callees of 0x15f5e0, defined in pick_copies and lower_rules (same declarations), except
 * TaLogic019_IsUseGFK (0x1626f0, kind_gates), declared from the call site (one argument, result
 * tested >= 0). */
extern int TaLogic019_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *unit, int id);
extern int TaLogic019_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
extern int TaLogic019_IsDoCerem(int value, int index);
extern int TaLogic019_GetUseThunderBolt(int side);
extern int TaLogic019_IsUseGFK(int side);
extern int TaLogic019_IsUseRevive(int side);
extern int TaLogic019_IsUseSaint(int side);
extern int TaLogic019_GetSpecialComboPlay(int side, unsigned char *item, struct DotrPickBlock *block);

/* The second argument is only tested against zero and forwarded: block type assumed from the callees. */
int TaLogic015_IsHandPlay(int side, struct DotrStateBlock *unit);
int TaLogic014_IsHandPlay(int side, struct DotrStateBlock *block);
int TaLogic011_IsHandPlay(int side, struct DotrStateBlock *block);
int TaLogic018_IsHandPlay(int side, struct DotrStateBlock *block);
int TaLogic020_IsHandPlay(int side, struct DotrStateBlock *block);
int TaLogic017_IsHandPlay(int side, struct DotrStateBlock *unit);
int TaLogic008_IsHandPlay(int side, struct DotrStateBlock *block);
int TaLogic021_IsHandPlay(int side, struct DotrStateBlock *unit);
int TaLogic010_IsHandPlay(int side, struct DotrStateBlock *block);
int TaLogic013_IsHandPlay(int side, struct DotrStateBlock *block);
int TaLogic_IsHandPlay__00119a40(int side, struct DotrStateBlock *unit);
int TaLogic001_IsHandPlay(int side, struct DotrStateBlock *unit);
int TaLogic_IsHandPlay__0010dfc0(int side, struct DotrStateBlock *unit);
int TaLogic006_IsHandPlay(int side, struct DotrStateBlock *block);
int TaLogic012_IsHandPlay(int side, struct DotrStateBlock *block);
int TaLogic009_IsHandPlay(int side, struct DotrStateBlock *unit);
int TaLogic019_IsHandPlay(int side, struct DotrStateBlock *block);

/* 0x12d520: the third argument is tested as a byte against zero (an active-mode flag). The count of
 * units the side does not control is computed and never read; the `>= 4` test reads the last
 * pick answer instead, which is not assigned on one path when the flag is set. */
extern int TaLogic007_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *unit, int targetCardId);
extern int TaLogic007_GetCurseCardPlay(int side, int *ids, struct DotrPickBlock *unit, unsigned char *minTask);
extern int TaLogic007_IsDoCerem(int value, int index);
extern int TaLogic007_GetSpecCardPlay(int side, int *ids, struct DotrStateBlock *block, unsigned char *item, int id);
extern int TaLogic007_IsRevivePlay(int side);
extern int Ta_IsUnitControl(int a, int b);
int TaLogic007_IsHandPlay(int side, struct DotrStateBlock *unit, unsigned char isActive);

/* 0x16f760: the record's second word is the id handed to Ta_GetCeremSacrifice and
 * TaHand_GetCardAPDP (layout assumed from these uses; pick_copies.h has the same view). */
struct DotrDispatchPair {
    int unknown_0;
    int unknown_4;
};
extern struct DotrDispatchPair *TaHand_GetCardEffect(int cardId, int effectIndex);
extern void Ta_GetCeremSacrifice(int *triple, int id);
extern int Ta_GetSacrificeUnit(int *sacrifices, int side);
extern int Ta_GetUnitOwnerIDFromLstCount(int flagList, int n);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int TaUtil_GetUnitAP(int side, int pos, int col, int row);
extern int TaHand_GetCardAPDP(int id);
extern int TaLogic022_GetSpecCardPlay(int side, unsigned char *task, struct DotrStateBlock *unit, int cardNo);
extern int TaLogic022_GetSpellPlay(int side, unsigned char *task, struct DotrStateBlock *unit, int cardNo);
extern int TaLogicCom_GetUseXYRangeSpell__0016cbb0(int side, unsigned *effect);
int TaLogic022_IsHandPlay(int side, struct DotrStateBlock *unit);

/* 0x123740: the hand-play decision of the TaLogic005 family (the Japanese build names it
 * TaLogic_IsHandPlay like its siblings; the address suffix keeps the name unique). Callees are
 * defined in pick_copies, state_queries, kind_gates and state_grid; the block is forwarded
 * to TaLogic_SetSpecialComboQue__00124c90 through a cast (same storage, that group's view). */
extern int TaLogic005_GetSpellPlay(int side, unsigned char *minTask, struct DotrStateBlock *unit, int cardId);
extern int TaLogic005_Is2Head_ThunderDragon(int side, int *list);
extern int TaLogic_SetSpecialComboQue__00124c90(int side, unsigned char *minTask, struct DotrPickBlock *block, unsigned char mode);
extern int TaLogic005_GetSpecialSpellPlay(int side, unsigned char *minTask, struct DotrStateBlock *unit);
extern int TaLogic005_IsKeepHandCombo(int side, int *hand, unsigned short cardId);
extern int TaLogic005_GetSpecialCardPlay(int side, unsigned char *minTask, struct DotrStateBlock *unit, int targetCard);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int SzDuel_GetUnitKind(int side, int pos);
int TaLogic_IsHandPlay__00123740(int side, struct DotrStateBlock *unit);

/* 0x11e6e0: the hand-play decision of the TaLogic004 family (Japanese build: TaLogic_IsHandPlay;
 * the address suffix keeps the name unique). Both picks are defined in pick_copies; the block
 * is forwarded to TaLogic004_GetSpecialComboPlay through a cast (same storage, that group's view). */
extern int TaLogic004_GetSpellPlay(int side, unsigned char *minTask, struct DotrStateBlock *unit, int cardId);
extern int TaLogic004_GetSpecialComboPlay(int side, unsigned char *task, struct DotrPickBlock *unit, unsigned char mode);
int TaLogic_IsHandPlay__0011e6e0(int side, struct DotrStateBlock *unit);

#endif
