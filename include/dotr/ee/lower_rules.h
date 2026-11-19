#ifndef DOTR_EE_LOWER_RULES_H
#define DOTR_EE_LOWER_RULES_H

/* Mostly TaLogic functions (0x100780-0x172370 in the game code below the library block, and
 * four at 0x276b60-0x27e780 after it) that combine answers of the cell-word, set-query and
 * list callees into a decision.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* The order record these functions receive: a packed position (row in the high
 * byte, column in the low byte), a halfword at +4 and a word at +8 whose low
 * halfword is also read on its own. Same offsets as position_rules.h's order
 * view; only these fields are shown by this code. */
struct DotrRuleOrder {
    unsigned short position;
    unsigned short summonOrder;
    unsigned short missions;   /* high byte the mission: 2 attack, 3 move to a location, 5 run, 6 move,
                                * 9 enchant, 10 summon, 11 trap, 12 revive, 13 revolt, 14 none */
    unsigned short unknown_06;
    union {
        unsigned short half;
        int word;
    } flags;
};

/* Callees, declared with the arguments these callers pass; results and
 * parameter types are assumed int unless a caller shows otherwise. Other group
 * headers hold different views of the same functions. */
extern int func_00250360_pair(int a, int b);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern int SzDuel_GetUnitKind(int side, int pos);
extern int func_001df540_word(int a, int b);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int SearchUnitByRange(int, int, int, int, int, int);
extern int Ta_GetUnitNumFrom22bitFlagList(unsigned flagList);
extern int Ta_GetUnitOwnerIDFromLstCount(int flagList, int n);
/* Its result is masked to a byte by 0x14b3e0, taken as an unsigned char result. */
extern unsigned char TaLogic013_IsActiveMode(int, int, int);
extern void TaHand_GetNextHand(int index, int *out);
extern int TaHand_GetNextHandNum(int side);
extern int TaHand_GetCardKind(int id);
extern int TaHand_GetCardAPDP(int id);
extern int SzDuel_Kind2Type(int kind);
extern void SzDuel_InitCardInfo(void *unit, int cardId);
extern int TaLogic017_IsExzParts(int id);
extern int SzDuel_GetLP(void);
extern int SzDuel_GetUnitMonsterCount(int side);
/* Callees of 0x167d30; results unused. */
extern void TaLogic_CommandStdLDUnit(struct DotrRuleOrder *order, int, int, int);
extern void TaLogic021_CommandBtlUnit(struct DotrRuleOrder *order, int);
extern void TaLogic_CommandEncUnit(struct DotrRuleOrder *order, int, int, int);
/* View of 0x16b0e0, declared elsewhere without arguments: 0x167d30 passes it one value. */
extern int func_0016b0e0_arg(int);
/* Fillers of a 22-halfword list; five and four arguments passed. */
extern void TaLogic_GetUnitCtlLst2(int, unsigned short *words, int, int, int);
extern void TaLogic_GetUnitCtlLst(int, unsigned short *words, int, int);
extern int TaLogic_GetMaxApUnit(unsigned short *ownerIdArray);
extern int TaLogic_IsUnitWin(int unit1OwnerId, int unit2, int col, int row);
extern int SzModel_GetModelType(int modelId);
extern int SzModel_IsModelReady(int index);
extern int SzModel_Load(int modelId, int modelType, int modelNo, int d);
/* View of Ta_GetUnitEffect with its result as a pointer to its first word. */
extern unsigned *func_00250740_word(int a, int b, int n);
/* Six arguments passed by 0x10dc40. */
extern int TaLogic_GetLandChange(int, int, int, int, int, int);

/* Nothing when SzDuel_GetUnit of the order's position is 0 or Ta_GetUnitEffect(row, column, 0)
 * is null. By that entry's first word >> 8: 13 stores 0x5ff at +4; 2 stores
 * 0x6ff at +4 and puts the low halfword of
 * TaLogic_GetLandChange(SzDuel_GetUnit answer, column, row of the SzDuel_GetUnitLoc halfword, 0, 6, 2)
 * above the +8 halfword. The code also tests that masked halfword for < 0,
 * which cannot hold; the arm it guards would use the SzDuel_GetUnitLoc halfword. */
void TaLogic002_CommandMagicUnit(struct DotrRuleOrder *order);
/* For indices 0..83: when SzModel_IsModelReady(i) is non-zero, calls SzModel_Load(i, -1, -1, 1)
 * unless SzModel_GetModelType(i) is 4 or 5 and SzDuel_GetUnit(-1, i) is 0. */
void lMain_LoadModel(void);
/* 1 when SzDuel_GetLP() >= 1500, two SearchUnitByRange(0, 0, side, 1, 0x2001c, 14) answers
 * do not sum to 0 and SzDuel_GetUnitMonsterCount(side) >= 4; otherwise 0. */
int TaLogic001_IsUseBraveHourGlass(int side);
/* Calls Ta_IsUnitControl, SzDuel_GetUnitLoc and SzDuel_GetUnitCardNo with the order's position; by the
 * SzDuel_GetUnitCardNo answer: 745 stores 0x6ff at +4 and the +8 halfword | 0x03010000
 * (SzDuel_GetUnitLoc row <= 2) or | 0x05020000 at +8; 690 stores 0x6ff and | 0x04050000;
 * 685 stores 0xcff and | 0xffff0000; anything else stores 0x5ff at +4 only. */
void TaLogic006_CommandSpellUnit(struct DotrRuleOrder *order);
/* 1 when Ta_GetUnitNumFrom22bitFlagList of the (2, 0, 14) set is >= 2, or when the first entry of the
 * (3, 33, 14) set has a Ta_IsUnitControl answer other than side and a zero SzDuel_GetUnitKind word. */
int TaLogic009_IsUseCurseDragonPot(int side);
/* As 0x1625e0 with TaLogic_IsUnitWin(first, second, ...) > 0. */
int TaLogic010_GetUseThunderBolt(int side);
/* 1 when the (1, 52, 14) set holds at most 2, the (2, 52, 14) set at least 5 and
 * one of its entries has a SzDuel_GetUnitLoc row <= 2; otherwise 0. */
int TaLogic012_IsPinch(int side);
/* row <= 2: 0x306 when col >= 3, else 0x300. Otherwise the SzDuel_GetUnitLoc(currentSide == 0, 0) answer. */
int TaLogic013_GetInvadePoint(int currentSide, int col, int row);
/* 1 when SzDuel_GetUnitCardNo(side, pos) is 273, 30 or 11. Two forwarded int arguments assumed. */
int TaLogic013_IsUnitInvader(int side, int pos);
/* 1 when TaLogic013_IsActiveMode(side, col, row) is non-zero, SearchUnitByRange(col, row, side, 1, 30, 18) is 0
 * and SearchUnitByRange(col, row, side, 2, 30, 18) is not; otherwise 0. */
int TaLogic013_IsUseHarpySweaper(int side, int col, int row);
/* 1 when TaHand_GetCardKind(id) >> 5 is non-zero, the TaHand_GetCardAPDP(id) low halfword plus
 * twice its signed high halfword exceeds 4500, or TaLogic017_IsExzParts(id) is 1. */
int TaLogic017_IsRefuseEmptyCombo(int id);
/* Fills a 22-halfword list with 0xffff, TaLogic_GetUnitCtlLst2(side == 0, list, 29, 0, 0),
 * packs it with TaLogic_GetMaxApUnit; refills, TaLogic_GetUnitCtlLst(side, list, 29, 1), packs again.
 * Returns the first packed value when both are >= 0 and
 * TaLogic_IsUnitWin(second, first, column, row of SzDuel_GetUnitLoc(first)) < 0; otherwise -1. */
int TaLogic019_GetUseThunderBolt(int side);
/* First of the TaHand_GetNextHand ids (TaHand_GetNextHandNum of them) whose
 * SzDuel_Kind2Type(TaHand_GetCardKind >> 5) is 2: stores its index at minTask[6], calls
 * SzDuel_InitCardInfo(unit, id) and returns TaHand_GetCardKind(id) >> 5; -1 when there is none. An
 * eight-word id buffer is assumed. */
int TaLogic020_GetTrapCardPlay(int index, unsigned char *minTask, void *unit);
/* 0 when 0x1df540 of the order's position is 0, SzDuel_GetUnitStatus(.., 0) is not, or the
 * position is 0xffff. Otherwise 1 after: column 0 calls TaLogic_CommandStdLDUnit(order, 3, 1, 1)
 * when the +8 word is -1; else by SzDuel_GetUnitKind >> 5: 0 calls TaLogic021_CommandBtlUnit(order, 1),
 * 2 calls TaLogic_CommandEncUnit(order, 4, 1, low byte of TaLogic_GetWarnHideLvl__0016b0e0(Ta_IsUnitControl
 * answer)), anything else stores 0x5ff at +4. */
int TaLogic_CommandUnit__00167d30(struct DotrRuleOrder *order);

/* 0x161ea0 and its copy 0x172000 (other finder and fallback callees). A
 * local array of twelve bytes has both nibbles of each byte set to 15 and is not
 * used again. The finder's answer is discarded when
 * TaLogic_GetActionReaction(row, column, 0, 0, 6, 0) is 5. A non-negative answer whose high
 * halfword is not 0xffff: counts the non-zero SzDuel_GetUnit(side == 0, i) for i
 * from 1 up to that halfword (at most 10), then stores the position, kind 13,
 * the low byte of TaLogic_IsDefStyleUnit(row, column, result halfword +4, 1, 0) at +16, 1 at
 * +12 and the count - 1 at +4, calls TaLogic_InitRogicControl(order, mode) and answers 0.
 * Otherwise a non-zero SzDuel_GetUnitStatus(row, column, 4) calls
 * SzDuel_SetUnitStatus(row, column, 0, 1), stores the position and kind 14 and answers 0;
 * else the fallback's answer. The result view, the nibble view and the
 * finder and fallback signatures are assumed from these two callers;
 * TaLogic_InitRogicControl is declared here with the order and an int as passed. */
struct DotrRuleNibbles {
    unsigned char low : 4;
    unsigned char high : 4;
};
struct DotrRuleResult {
    unsigned short position;
    unsigned char kind;            /* task: 2 attack, 3 confirm unit, 5 run, 6 move to a location, 9 enchant,
                                    * 10 summon, 11 trap, 12 revive, 13 revolt, 14 nothing */
    unsigned char unknown_03;
    unsigned short loc;
    unsigned short unknown_06;
    int unknown_08;
    int shouldFlipUp;              /* 0 face down, 1 face up, 2 no change */
    unsigned char atkDefPosition;  /* 0 attack, 1 defence, 2 no change */
};
extern int TaLogic019_IsUseRevolt(int side);
extern int TaLogicCom_GetRevoltObj__0016b3c0(int side);
/* Last parameter is 8-bit: 0x27e780 forwards its unsigned char argument unmasked. */
extern int TaLogic_GetActionReaction(int, int, int, int, int, unsigned char);
extern int TaLogic_IsDefStyleUnit(int side, int pos, unsigned short word, unsigned char isMove3, int last);
extern void func_0027b1b0_order(struct DotrRuleOrder *order, int mode);
extern int SzDuel_SetUnitStatus(int side, int pos, int bit, int value);
/* func_001607d0_rest / func_00170940_rest are TaLogic_GetMinimumTask_RunMission__001607d0 /
 * TaLogic_GetMinimumTask_RunMission__00170940 (reach_picks, defined with an unsigned char mode
 * that they store with sb) as the callers here see them: with int mode, like
 * func_0027b1b0_order. With the unsigned char prototype the callers get an andi the original
 * lacks. */
extern int func_001607d0_rest(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode);
extern int func_00170940_rest(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode);
int TaLogic_GetMinimumTask_RevoltMission__00161ea0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode);
int TaLogic_GetMinimumTask_RevoltMission__00172000(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode);
/* A third copy, 0x138950: finder TaLogic009_GetRevoltObj (declared in score_scans.h; called
 * here through a view, its int argument and result as declared there),
 * fallback TaLogic_GetMinimumTask_RunMission__001374d0 (signature assumed as for the other two),
 * and a last argument of 1 instead of 0 in its TaLogic_GetActionReaction and TaLogic_IsDefStyleUnit calls. */
extern int func_00138b60_find(int side);
extern int func_001374d0_rest(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode);
int TaLogic_GetMinimumTask_RevoltMission__00138950(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode);
/* 0x138780 has the same frame with finder TaLogic009_IsUseCopyCat (declared in grid_scans.h)
 * and no counting loop: a non-negative answer's high halfword and low byte go
 * to TaLogic_GetGraveEntryAfterScreen (declared in state_queries.h, third argument 0), and the result
 * gets kind 12 and (high halfword << 8) | that answer. Both callees as declared
 * in their own groups. */
extern int TaLogic009_IsUseCopyCat(int side);
extern int TaLogic_GetGraveEntryAfterScreen(int side, int target, int mode);
int TaLogic_GetMinimumTask_ReviveMission__00138780(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode);
/* 0x166d10 is 0x138780 with finder TaLogic020_IsUseRevive (declared in grid_scans.h), a
 * third TaLogic_GetGraveEntryAfterScreen argument of 30 and fallback
 * TaLogic_GetMinimumTask_MoveMission__001664a0 (declared in move_missions.h with byte mode and flag
 * parameters; this caller hands its mode on unmasked, so it sees the function through func_001664a0_ints, a
 * second C view of the same address: a cast at the call gives an indirect
 * call). */
extern int TaLogic020_IsUseRevive(int side);
extern int func_001664a0_ints(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode, int flag);
int TaLogic_GetMinimumTask_ReviveMission__00166d10(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode);

/* 0x11c430 and L004 (differ in one callee): one step of a slot set. A null
 * set gives 0x800. Otherwise the halfword at +0x1a2 is incremented: 600, 601
 * or 602 gives 0x20 and 603 or more 0x800. With the 5-bit mode at +0x1a0 zero,
 * TaLogic_DoAdjustLogicData(set) is called and the finder (0x117e60 / 0x11c600) fills a local
 * step record (0 -> 0x800), which TaLogic_GetInputQueFromMinimumTask applies (negative -> 0). The result
 * is ids[mode - 1], or 0 when that is 0xffff; when ids[mode] is 0xffff the
 * mode becomes 0 and all 32 ids 0xffff, else the mode is incremented.
 * Assumed from this code alone: the set view (ids and mode as in block_table.h's
 * DotrSlotSet), the step record's fields (the offsets of struct DotrRuleResult
 * with five bytes at +6), and the three callee signatures; TaLogic_DoAdjustLogicData is declared
 * in state_queries.h with another pointer type, hence the view name. */
struct DotrRuleEntry {
    unsigned short position;
    unsigned short summonOrder;
    unsigned short kinds;
    unsigned char unknown_06[10];
};
struct DotrRuleSet {
    struct DotrRuleEntry entries[22];
    unsigned short ids[32];
    unsigned char mode : 5;
    unsigned char aiActiveRelated;
    unsigned short inputCount;
};
struct DotrRuleStep {
    unsigned short position;
    unsigned char kind;
    unsigned short loc;
    unsigned char slots[5];
    int shouldFlipUp;
    unsigned char atkDefPosition;
};
extern int func_00275e80_set(struct DotrRuleSet *set);
extern int TaLogic_GetMinimumTask__00117e60(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);
extern int TaLogic_GetInputQueFromMinimumTask(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);

extern int TaLogic_GetMinimumTask__0011c600(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);
int L003(int side, struct DotrRuleSet *set);
int L004(int side, struct DotrRuleSet *set);
/* Two more copies, with the finders 0x112b90 and 0x158c80 (same assumed
 * signature). */
extern int TaLogic_GetMinimumTask__00112b90(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);
extern int TaLogic_GetMinimumTask__00158c80(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);
int L001(int side, struct DotrRuleSet *set);
int L018(int side, struct DotrRuleSet *set);
/* Three more copies, with the finders 0x121800, 0x126c80 and 0x1424b0
 * (same assumed signature); each directly follows one of
 * pick_copies' TaLogicNNN_IsDoCerem copies in the executable. */
extern int TaLogic_GetMinimumTask__00121800(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);
extern int TaLogic_GetMinimumTask__00126c80(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);
extern int TaLogic_GetMinimumTask__001424b0(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);
int L005(int side, struct DotrRuleSet *set);
int L006(int side, struct DotrRuleSet *set);
int L012(int side, struct DotrRuleSet *set);
/* Three more copies, with the finders 0x139890, 0x163130 and 0x16d140
 * (same assumed signature); each directly follows a position_rules
 * leaf function in the executable. 0x172370 has no null test and does not fill the
 * step's five slot bytes. */
extern int TaLogic_GetMinimumTask__00139890(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);
extern int TaLogic_GetMinimumTask__00163130(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);
extern int TaLogic_GetMinimumTask__0016d140(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);
int L010(int side, struct DotrRuleSet *set);
int L020(int side, struct DotrRuleSet *set);
int L022(int side, struct DotrRuleSet *set);
/* 2500 (0x117c30) or 2000 (0x15d120) while SzDuel_GetLP() is at most 1200; above
 * that 1500 when SzDuel_GetUnitMonsterCount(side) is below 4 (0x117c30) or 5 (0x15d120), else
 * 750. */
int TaLogic_GetWarnHideLvl__00117c30(int side);
int TaLogic_GetWarnHideLvl__0015d120(int side);

/* 0x122b50: nothing when SzDuel_GetUnit of the order's position is 0. Otherwise
 * calls Ta_GetCurrentSide and SzDuel_GetUnitLoc for the position and acts on its SzDuel_GetUnitCardNo word:
 * 829 clears the flag word above its low halfword; 818 also sets bit 24; 820
 * does as 818 when some cell of the 7 by 7 grid has a SzDuel_GetField answer of 1
 * and a null SzDuel_GetMapUnit answer, and otherwise leaves the word. +4 always ends as 0x5ff
 * (each arm first stores 0x3ff or 0x5ff, which is then overwritten). The
 * scan also compares each such cell's distance from the SzDuel_GetUnitLoc cell with a
 * bound that stays -1. Ta_GetCurrentSide and SzDuel_GetField are declared as in
 * state_queries.h and state_grid.h; SzDuel_GetMapUnit's result is only tested for
 * null. */
struct DotrStateItem;
extern int Ta_GetCurrentSide(int side, int pos);
extern int SzDuel_GetField(int x, int y);
extern struct DotrStateItem *SzDuel_GetMapUnit(int x, int y);
void TaLogic005_CommandTrapBUnit(struct DotrRuleOrder *order);

/* 0x1229f0: nothing when SzDuel_GetUnit of the order's position is 0. Otherwise asks
 * Ta_GetCurrentSide (side) and SzDuel_GetUnitLoc (cell) for the position; when its SzDuel_GetUnitCardNo word
 * is 565, counts for columns 6..10 the non-zero SzDuel_GetUnitStatus(s, column, 4) whose
 * Ta_IsUnitControl(s, column) equals side, for s = side and s = (side == 0). A count
 * above 0 stores 0x3ff at +4 and puts the cell above the flag word's low
 * halfword. Ends with TaLogic_CommandBtlUnit__00122270(order, 0), declared here as a view taking this
 * group's order type (order_tries.h holds the main declaration). The
 * count is assumed to be an unsigned char. */
extern void func_00122270_order(struct DotrRuleOrder *order, int flag);
void TaLogic_CommandEfcMonster__001229f0(struct DotrRuleOrder *order);

/* 0x1627d0 and TaLogic019_IsUseSaint (same code; gate SzDuel_GetUnitMonsterCount / SzDuel_GetUnitSpellCount
 * and ids 60, 87, 43 / 700, 715, 827). -1 when the gate answers 5 or more, or when
 * TaLogic_GetGraveCardCount(side, 3, id) is not positive for all three ids. Otherwise looks
 * for the second id (the third in 0x162a70), then the third (first), then the
 * first (second): each first among the side's SzDuel_InqGrave entries below its
 * SzDuel_GetGraveCount count, then among the other side's (side == 0). Returns the
 * owning side << 16 | position of the first hit, or -1. The counts are asked
 * again after every entry. TaLogic_GetGraveCardCount, SzDuel_GetGraveCount and SzDuel_InqGrave are declared
 * as in state_queries.h; SzDuel_GetUnitSpellCount's int argument and result are assumed. */
extern int SzDuel_GetUnitSpellCount(int side);
extern int TaLogic_GetGraveCardCount(int side, int flags, int id);
extern int SzDuel_GetGraveCount(int side);
extern int SzDuel_InqGrave(int side, int position);
int TaLogic019_IsUseRevive(int side);
int TaLogic019_IsUseSaint(int side);
/* 0x14b490: the same search gated by SzDuel_GetUnitMonsterCount (monster count) instead of
 * SzDuel_GetUnitSpellCount; ids 273, 30, 11 asked, searched 11, 30, 273. */
int TaLogic013_IsUseRevive(int side);

/* 0x168530: rewrite the order's +4 halfword and the flag word above its low
 * halfword from the SzDuel_GetUnitLoc cell of its position (high byte hi, low byte lo)
 * and the high byte of +4. 16: hi 6 gives 0xfff and lo << 16, else 0x10ff and
 * (lo | 0x600) << 16. 15: hi 0 gives 0x10ff and (lo | 0x600) << 16, else 0xfff
 * and lo << 16. Otherwise, hi below 4: hi 0 gives 0x10ff and lo << 16, else
 * 0x6ff with 0x30000 for lo 3, 0x60000 for lo above 3 and nothing below; hi 4
 * or more gives 0x6ff with 0x3030000 when SzDuel_GetMapUnit(3, 3) is null, 0x3020000
 * when SzDuel_GetMapUnit(2, 3) is null, 0x3040000 when a second SzDuel_GetMapUnit(2, 3) is null,
 * else SzDuel_GetUnitLoc(pair == 0, 0) << 16, pair being the Ta_IsUnitControl answer for the
 * position. The order layout is the one assumed for 0x122b50. */
void TaLogic021_CommandInvadeUnit(struct DotrRuleOrder *order);

/* 0x118510 and its copy 0x11cd30 (other prepare and try callees): choose a
 * packed position for a side from four 22-halfword lists, each filled with
 * 0xffff and then by TaLogic_GetUnitCtlLst(side, list, n, 1). n = 29: a non-negative
 * TaLogic_GetAtkUnitNow(side, list, set) is the answer. n = 30: the first entry with
 * SzDuel_GetUnitKind >> 5 == 3 whose TaLogic_IsUnitReadyMove and TaLogic_IsPickUp answers are both 1. Then
 * the side's own position (side << 8): its slot TaLogic_GetActionUnitFromOwnerID(set, position) is
 * prepared (0x118330 / 0x11cb20) and, when the high byte of the slot's +4
 * halfword is 10, SzDuel_InqHandPhase(side) answering 1 with a non-negative try callee
 * (0x119300 / 0x11dfb0)(position, side, step) answers position | 0x10000,
 * else TaLogic_IsUnitReadyMove(position) answering 1 answers the position; for other kinds a zero
 * SzDuel_GetUnitStatus(side, 0, 0) asks only TaLogic_IsUnitReadyMove, a non-zero one only SzDuel_InqHandPhase
 * and the try. n = 31: the first entry with both acceptances. n = 31 again: the
 * first entry with a zero SzDuel_GetUnitStatus(row, column, 0) and a TaLogic_IsPickUp answer of 1.
 * Otherwise -1. Each list is walked while the index is below TaLogic_GetUnitCtlLstLen(list).
 * Assumed: the 16-byte entry view of the set (struct DotrRuleEntry, same
 * storage as the bytes below +0x160), the step pointer type (only passed on),
 * and every callee signature; TaLogic_GetActionUnitFromOwnerID, TaLogic_GetUnitCtlLstLen, TaLogic_IsPickUp
 * and SzDuel_InqHandPhase are declared elsewhere with other types. */
extern int TaLogic_GetAtkUnitNow(int side, unsigned short *ownerIds, struct DotrRuleSet *units);
extern int TaLogic_GetUnitCtlLstLen(unsigned short *units);
extern int TaLogic_IsUnitReadyMove(unsigned short wUnit);
extern int TaLogic_IsPickUp(int ownerId);
extern int func_00276c80_set(struct DotrRuleSet *set, int value);
extern int SzDuel_InqHandPhase(int side);
extern void func_00118330_entry(struct DotrRuleEntry *entry);
extern int TaLogic_GetMinimumTask_Summon__00119300(int position, int side, struct DotrRuleStep *step);
extern void func_0011cb20_entry(struct DotrRuleEntry *entry);
extern int TaLogic_GetMinimumTask_Summon__0011dfb0(int position, int side, struct DotrRuleStep *step);
int TaLogic_GetNextActiveUnit__00118510(struct DotrRuleSet *set, int side, struct DotrRuleStep *step);
int TaLogic_GetNextActiveUnit__0011cd30(struct DotrRuleSet *set, int side, struct DotrRuleStep *step);

/* Three more copies of 0x118510, each with its own prepare and try callee: 0x121e90 (0x121ce0,
 * 0x122d30), 0x1273e0 (0x1271f0, 0x127e30) and 0x131750 (0x131540, 0x132460). Assumed as for
 * 0x118510; 0x127e30 and 0x132460 are declared in step_picks.h with another pointer type, hence the
 * views. */
extern void func_00121ce0_entry(struct DotrRuleEntry *entry);
extern int TaLogic_GetMinimumTask_Summon__00122d30(int position, int side, struct DotrRuleStep *step);
extern void func_001271f0_entry(struct DotrRuleEntry *entry);
extern int func_00127e30_step(int position, int side, struct DotrRuleStep *step);
extern void func_00131540_entry(struct DotrRuleEntry *entry);
extern int func_00132460_step(int position, int side, struct DotrRuleStep *step);
int TaLogic_GetNextActiveUnit__00121e90(struct DotrRuleSet *set, int side, struct DotrRuleStep *step);
int TaLogic_GetNextActiveUnit__001273e0(struct DotrRuleSet *set, int side, struct DotrRuleStep *step);
int TaLogic_GetNextActiveUnit__00131750(struct DotrRuleSet *set, int side, struct DotrRuleStep *step);

/* Three copies of 0x118510 with a fourth parameter: 0x113370 (0x113140, 0x114080), 0x13a010
 * (0x139d90, 0x13ab70) and 0x147100 (0x146ec0, 0x148020). The third argument is passed on unread as
 * the prepare callee's second argument and the fourth as the try callee's third. Assumed as for
 * 0x118510, plus the int type of the third argument and the two-argument prepare signature (the
 * prepare callees take an order record elsewhere, hence the `_entry` views); the try callees are
 * declared in step_picks.h with another pointer type, hence the views. */
extern void func_00113140_entry(struct DotrRuleEntry *entry, int key);
extern int func_00114080_step(int position, int side, struct DotrRuleStep *step);
extern void func_00139d90_entry(struct DotrRuleEntry *entry, int key);
extern int func_0013ab70_step(int position, int side, struct DotrRuleStep *step);
extern void func_00146ec0_entry(struct DotrRuleEntry *entry, int key);
extern int func_00148020_step(int position, int side, struct DotrRuleStep *step);
int TaLogic_GetNextActiveUnit__00113370(struct DotrRuleSet *set, int side, int key, struct DotrRuleStep *step);
int TaLogic_GetNextActiveUnit__0013a010(struct DotrRuleSet *set, int side, int key, struct DotrRuleStep *step);
int TaLogic_GetNextActiveUnit__00147100(struct DotrRuleSet *set, int side, int key, struct DotrRuleStep *step);

/* 0x113140, the prepare callee of 0x113370: 0x118330's rejections and column-0
 * stores after an unused SzDuel_GetUnitLoc read, except that a TaLogic_IsAttackedNow answer other
 * than 1 stores 0xA06 with the low halfword of +8 set to 0x101, or 0x6FF with
 * the high halfword set to 0x101. Other columns go by SzDuel_GetUnitKind >> 5: 0 calls
 * TaLogic001_CommandEfcMonster(order, deep) when TaUtil_IsExMonster answers 1, else
 * TaLogic001_CommandBtlUnit(order, 1, deep); 1 and 3 call TaLogic001_CommandSpellUnit(order, deep); 2 sends
 * TaLogic_CommandEncUnit(order, 5, 0, TaLogic_GetWarnHideLvl__00117c30(key) & 0xFF); anything else stores
 * 0x5FF. Returns 1 unless rejected. Assumed: the int type of the second argument (only passed on),
 * the signatures of TaLogic001_CommandEfcMonster and 0x113ed0, and the order
 * view of TaLogic001_CommandBtlUnit (declared in order_tries.h with another pointer type). */
extern void TaLogic001_CommandEfcMonster(struct DotrRuleOrder *order, int deep);
extern void func_00113760_order(struct DotrRuleOrder *order, int flag, int deep);
int TaLogic_CommandUnit__00113140(struct DotrRuleOrder *order, int deep);
/* 0x163660 is the one-argument form of 0x113140: 1 as the third TaLogic_IsAttackedNow
 * argument, 0x103 where that one stores 0x101, kinds 0 and 5 to TaLogic020_CommandEfcMonster
 * (one argument assumed from this call) or TaLogic020_CommandBtlUnit with 1,
 * kinds 1, 3 and 4 to TaLogic020_CommandSpellUnit, kind 2 to TaLogic_CommandEncUnit(order, 5, 1,
 * TaLogic_GetWarnHideLvl__001676d0(key)). TaLogic020_CommandBtlUnit (order_tries)
 * and TaLogic020_CommandSpellUnit (grid_scans) are declared with their own order structures and seen here
 * through `_order` views; 0x163870 calls this function through the `_entry` view above and ignores
 * the result. */
extern void TaLogic020_CommandEfcMonster(struct DotrRuleOrder *order);
extern void func_00163c50_order(struct DotrRuleOrder *order, int flag);
extern void func_00164260_order(struct DotrRuleOrder *order);
extern int TaLogic_GetWarnHideLvl__001676d0(int key);
int TaLogic_CommandUnit__00163660(struct DotrRuleOrder *order);

/* 0x146ec0 and 0x139d90, the prepare callees of 0x147100 and 0x13a010: the
 * 0x113140 source with other callees and column-0 stores. 0x146ec0 stores the
 * bare 0xA05 / 0x5FF when TaLogic_IsAttackedNow is not 1; kind 0 calls TaLogic013_CommandInvadeUnit(order)
 * when 0x14b390 answers 1, else TaLogic013_CommandEfcMonster(order) when TaUtil_IsExMonster answers 1, else
 * TaLogic013_CommandBtlUnit(order); 1 and 3 call TaLogic013_CommandSpellUnit(order, deep); 2 sends
 * TaLogic_CommandEncUnit(order, 5, 0, TaLogic_GetWarnHideLvl__0014b330(key) & 0xFF). 0x139d90 keeps
 * the leading SzDuel_GetUnitLoc word and, when TaLogic_IsAttackedNow is not 1, puts it (its high byte plus
 * one when the low byte of the second argument is 1) into the low halfword of +8
 * with 0xA06 or the high halfword with 0x6FF; kind 0 calls TaLogic010_CommandBtlUnit(order, 1,
 * deep); 1 and 3 call TaLogic010_CommandSpellUnit(order, deep); 2 sends TaLogic_CommandEncUnit(order, 2, 0,
 * TaLogic_GetWarnHideLvl__0013da70(key) & 0xFF). Assumed: as for 0x113140, the signatures of
 * TaLogic013_CommandInvadeUnit, 0x147ac0, TaLogic013_CommandSpellUnit and 0x13aa40, and the order views
 * of TaLogic013_CommandBtlUnit and TaLogic010_CommandBtlUnit (declared in order_tries.h with another pointer
 * type); 0x14b330 and 0x13da70 are declared as in order_tries.h. */
extern int TaLogic_GetWarnHideLvl__0014b330(int side);
extern void TaLogic013_CommandInvadeUnit(struct DotrRuleOrder *order);
extern void func_001474f0_order(struct DotrRuleOrder *order);
extern void TaLogic013_CommandSpellUnit(struct DotrRuleOrder *order, int deep);
extern int TaLogic_GetWarnHideLvl__0013da70(int side);
extern void func_0013a400_order(struct DotrRuleOrder *order, int flag, int deep);
int TaLogic_CommandUnit__00146ec0(struct DotrRuleOrder *order, int deep);
int TaLogic_CommandUnit__00139d90(struct DotrRuleOrder *order, int deep);

/* Three callees of the prepare functions above; each rewrites the order's +4
 * halfword and sometimes the high halfword of +8.
 * 0x147ac0: when the SzDuel_GetUnitCardNo word of the position is 291 and the Ta_GetUnitNumFrom22bitFlagList
 * count of SearchUnitByRange(0, 0, key, 1, 6, 14) is at least 3, stores 0x3FF and the
 * position's SzDuel_GetUnitLoc word; otherwise calls TaLogic013_CommandBtlUnit(order).
 * 0x13aa40, by SzDuel_GetUnitKind >> 5: 3 stores 0x3FF and the low byte of the
 * position's cell under the high byte of the SzDuel_GetUnitLoc(key, 0) word; 1 asks
 * TaLogic010_GetUseThunderBolt(key) and, for an answer of 0 or more, stores 0x6FF and the cell's
 * high byte over the low byte of that answer's SzDuel_GetUnitLoc word; else 0x5FF.
 * 0x113ed0: 0x5FF without a Ta_GetUnitEffect record; kind 1 goes by the record word
 * >> 8: 2 stores 0x6FF and 0x0303, 20 stores 0xCFF, 4 asks TaLogicCom_GetUseXYRangeSpell__00112600(key,
 * record) and for an answer of 0 or more stores 0x6FF and the answer (under
 * the cell's high byte when the record's low six bits are 0); kind 3 stores
 * 0xBFF when TaLogic_CommandGunnerUnit(order, 0) is 1; everything else 0x5FF.
 * Assumed: void results, the unread second parameters (passed by the
 * callers; `(void)deep;` is a host-build statement), int locals; 0x112600 is
 * declared as in score_scans.h. */
extern int TaLogicCom_GetUseXYRangeSpell__00112600(int side, unsigned *record);
void TaLogic013_CommandEfcMonster(struct DotrRuleOrder *order);
void TaLogic010_CommandSpellUnit(struct DotrRuleOrder *order, int deep);
void TaLogic001_CommandSpellUnit(struct DotrRuleOrder *order, int deep);

/* TaLogic001_IsUseLifeHourGlass (a gate of position_rules' TaLogic001_CommandEfcMonster): 0 when
 * SzDuel_GetLP is below 1500. Then twice, for the low byte n of TaLogic_GetGraveCardCount(side, 3, 2) and of
 * TaLogic_GetGraveCardCount(side, 3, 0): when n is above 0, counts (in a byte) the i in 0..7
 * with Ta_GetSummonablePieceWithOrder(side, i, 1) >= 0 and returns 1 when the count is at least n
 * and 5 - SzDuel_GetUnitMonsterCount(side) is at least n. Otherwise 0. Assumed: the int
 * arguments and result of Ta_GetSummonablePieceWithOrder. */
extern int Ta_GetSummonablePieceWithOrder(int side, int i, int avoidLabyrinth);
int TaLogic001_IsUseLifeHourGlass(int side);

/* Three more copies of 0x118510 with their own prepare and try callees: 0x135970 (0x135790,
 * 0x136210), 0x13e3d0 (0x13e1d0, 0x13eee0) and 0x142b70 (0x1429b0, 0x143ba0). Assumed as for
 * 0x118510, including the six callees' signatures. */
extern void func_00135790_entry(struct DotrRuleEntry *entry);
extern int TaLogic_GetMinimumTask_Summon__00136210(int position, int side, struct DotrRuleStep *step);
extern void func_0013e1d0_entry(struct DotrRuleEntry *entry);
extern int TaLogic_GetMinimumTask_Summon__0013eee0(int position, int side, struct DotrRuleStep *step);
extern void func_001429b0_entry(struct DotrRuleEntry *entry);
extern int TaLogic_GetMinimumTask_Summon__00143ba0(int position, int side, struct DotrRuleStep *step);
int TaLogic_GetNextActiveUnit__00135970(struct DotrRuleSet *set, int side, struct DotrRuleStep *step);
int TaLogic_GetNextActiveUnit__0013e3d0(struct DotrRuleSet *set, int side, struct DotrRuleStep *step);
int TaLogic_GetNextActiveUnit__00142b70(struct DotrRuleSet *set, int side, struct DotrRuleStep *step);

/* Three more copies of 0x118510 with their own prepare and try callees: 0x14bfd0 (0x14bde0,
 * 0x14cba0), 0x150470 (0x150270, 0x150c40) and 0x159380 (0x1591b0, 0x15a360). Assumed as for
 * 0x118510; 0x14cba0 and 0x15a360 are step_picks functions seen through `_step` views; the other
 * four callees' signatures are assumed from the calls. */
extern void func_0014bde0_entry(struct DotrRuleEntry *entry);
extern int func_0014cba0_step(int position, int side, struct DotrRuleStep *step);
extern void func_00150270_entry(struct DotrRuleEntry *entry);
extern int TaLogic_GetMinimumTask_Summon__00150c40(int position, int side, struct DotrRuleStep *step);
extern void func_001591b0_entry(struct DotrRuleEntry *entry);
extern int func_0015a360_step(int position, int side, struct DotrRuleStep *step);
int TaLogic_GetNextActiveUnit__0014bfd0(struct DotrRuleSet *set, int side, struct DotrRuleStep *step);
int TaLogic_GetNextActiveUnit__00150470(struct DotrRuleSet *set, int side, struct DotrRuleStep *step);
int TaLogic_GetNextActiveUnit__00159380(struct DotrRuleSet *set, int side, struct DotrRuleStep *step);

/* 0x121ce0, the prepare callee of TaLogic_GetNextActiveUnit__00121e90 (which passes a set entry and
 * ignores the result; this code reads the same 12 bytes as an order). 0 when
 * 0x1df540 of the position is 0, SzDuel_GetUnitStatus(row, column, 0) is not 0 or the
 * position is 0xffff. Otherwise 1 after: column 0 calls TaLogic_CommandStdLDUnit(order, 3, 2, 0)
 * when the flag word is -1; other columns go by SzDuel_GetUnitKind >> 5: 0 calls
 * 0x1229f0 when TaUtil_IsExMonster(row, column) is 1, else TaLogic_CommandBtlUnit__00122270(order, 1); 2
 * calls TaLogic_CommandEncUnit(order, 2, 0, low byte of TaLogic_GetWarnHideLvl__001254a0(Ta_IsUnitControl
 * answer)); 4 calls 0x122b50; anything else stores 0x5ff at +4. Assumed: the order view, int
 * result, and the callee views (0x1254a0 and TaUtil_IsExMonster are declared elsewhere
 * with other types). */
extern int func_001254a0_key(int key);
extern int func_002507d0_pair(int a, int b);
int TaLogic_CommandUnit__00121ce0(struct DotrRuleOrder *order);

/* 0x1429b0, the prepare callee of TaLogic_GetNextActiveUnit__00142b70 (which passes a set entry and
 * ignores the result, hence its `_entry` view). Rejections as 0x121ce0. Column
 * 0 with flag word -1 stores 0x50A or 0xA05 at +4 by TaLogic_IsAttackedNow(side, 0, 0) == 1
 * when SzDuel_InqHandPhase(side) == 1, else 0x5FF; kind 0 calls TaLogic012_CommandBtlUnit when the word is
 * 352, else TaLogic012_CommandLineUnit; kind 2 sends TaLogic_CommandEncUnit(order, 2, 0,
 * TaLogic_GetWarnHideLvl__001461c0(key) & 0xFF); other kinds store 0x5FF. Assumed: the order view, the int
 * result, TaLogic_IsAttackedNow's and TaLogic012_CommandLineUnit's signatures, `_key`/`_order` views of
 * 0x1461c0 and TaLogic012_CommandBtlUnit. */
extern int TaLogic_IsAttackedNow(int a, int b, int c);
extern int func_001461c0_key(int key);
extern void func_00143540_order(struct DotrRuleOrder *order);
extern void TaLogic012_CommandLineUnit(struct DotrRuleOrder *order);
int TaLogic_CommandUnit__001429b0(struct DotrRuleOrder *order);

/* 0x1591b0, the prepare callee of TaLogic_GetNextActiveUnit__00159380 (which passes a set entry and
 * ignores the result, hence its `_entry` view). It first calls SzDuel_GetUnitLoc on the
 * position and drops the result; rejections as 0x121ce0. Column 0 with flag
 * word -1 calls TaLogic018_CommandLDUnit; kind 0 calls TaLogic018_CommandEfcMonster when TaUtil_IsExMonster
 * is 1, else TaLogic018_CommandBtlUnit; kind 2 sends TaLogic_CommandEncUnit(order, 3, 0,
 * TaLogic_GetWarnHideLvl__0015d120(key) & 0xFF); kind 5 calls TaLogic018_CommandBtlUnit and, when the high
 * byte of +4 is then 2, stores 0x6FF at +4 and replaces the high flag halfword (a packed position) with its
 * SzDuel_GetUnitLoc cell; other kinds store 0x5FF. Assumed: the order view, the int result,
 * TaLogic018_CommandLDUnit's and TaLogic018_CommandEfcMonster's signatures, the `_order` view of
 * order_tries' TaLogic018_CommandBtlUnit. */
extern void TaLogic018_CommandLDUnit(struct DotrRuleOrder *order);
extern void TaLogic018_CommandEfcMonster(struct DotrRuleOrder *order);
extern void func_00159d00_order(struct DotrRuleOrder *order);
int TaLogic_CommandUnit__001591b0(struct DotrRuleOrder *order);

/* Three more copies of 0x118510 with their own prepare and try callees: 0x15daf0 (0x15d8e0,
 * 0x15f300), 0x163870 (0x163660, 0x1643f0) and 0x16d8c0 (0x16d6a0, 0x16f4d0). Assumed as for
 * 0x118510; 0x1643f0 and 0x16f4d0 are step_picks functions seen through `_step` views; the
 * signatures of 0x15f300, 0x163660 and 0x16d6a0 are assumed from the calls. */
extern void func_0015d8e0_entry(struct DotrRuleEntry *entry);
extern int TaLogic_GetMinimumTask_Summon__0015f300(int position, int side, struct DotrRuleStep *step);
extern void func_00163660_entry(struct DotrRuleEntry *entry);
extern int func_001643f0_step(int position, int side, struct DotrRuleStep *step);
extern void func_0016d6a0_entry(struct DotrRuleEntry *entry);
extern int func_0016f4d0_step(int position, int side, struct DotrRuleStep *step);
int TaLogic_GetNextActiveUnit__0015daf0(struct DotrRuleSet *set, int side, struct DotrRuleStep *step);
int TaLogic_GetNextActiveUnit__00163870(struct DotrRuleSet *set, int side, struct DotrRuleStep *step);
int TaLogic_GetNextActiveUnit__0016d8c0(struct DotrRuleSet *set, int side, struct DotrRuleStep *step);
/* Two more copies. 0x12c3f0 passes its prepare callee 0x12c1a0 the low four
 * bits of the set's byte +0x1a1 and its try callee 0x12d270 the TaLogic007_IsActiveMode()
 * answer as a fourth argument. 0x10d080 passes the set before the entry to its
 * prepare callee 0x10ce20, tries with 0x10dd50, and accepts a second-list entry
 * whose SzDuel_GetUnitKind answer >> 5 is 3 or 4 (asked twice). The five callees'
 * signatures are assumed from the argument registers. */
extern void func_0012c1a0_entry(struct DotrRuleEntry *entry, int kind);
extern int TaLogic_GetMinimumTask_Summon__0012d270(int position, int side, struct DotrRuleStep *step, int limit);
extern int TaLogic007_IsActiveMode(void);
extern void func_0010ce20_entry(struct DotrRuleSet *set, struct DotrRuleEntry *entry);
extern int TaLogic_GetMinimumTask_Summon__0010dd50(int position, int side, struct DotrRuleStep *step);
int TaLogic_GetNextActiveUnit__0012c3f0(struct DotrRuleSet *set, int side, struct DotrRuleStep *step);
int TaLogic_GetNextActiveUnit__0010d080(struct DotrRuleSet *set, int side, struct DotrRuleStep *step);

/* 0x15d8e0, the prepare callee of TaLogic_GetNextActiveUnit__0015daf0 (which passes a set entry and
 * ignores the result, hence its `_entry` view). It first calls SzDuel_GetUnitLoc on the
 * position and drops the result; rejections and the column 0 stores as
 * 0x1429b0. Kind 0 calls TaLogic019_CommandEfcMonster when TaUtil_IsExMonster is 1, else
 * TaLogic019_CommandBtlUnit; kind 2 sends TaLogic_CommandEncUnit(order, 5, 0,
 * TaLogic_GetWarnHideLvl__00162580(key) & 0xFF); kinds 1, 3 and 4 call TaLogic019_CommandSpellUnit; other
 * kinds store 0x5FF. Assumed: the order view, the int result, the signatures of TaLogic019_CommandEfcMonster,
 * TaLogic019_CommandBtlUnit and TaLogic019_CommandSpellUnit, the `_key` view of position_rules' 0x162580. */
extern void TaLogic019_CommandEfcMonster(struct DotrRuleOrder *order);
extern void TaLogic019_CommandBtlUnit(struct DotrRuleOrder *order);
extern void TaLogic019_CommandSpellUnit(struct DotrRuleOrder *order);
extern int func_00162580_key(int key);
int TaLogic_CommandUnit__0015d8e0(struct DotrRuleOrder *order);

/* Seven more prepare callees, each reached from its caller through an `_entry`
 * view (the caller passes a set entry and ignores the result). All reject (0)
 * as 0x1429b0 does and otherwise return 1; 0x14bde0 and 0x150270 first call
 * SzDuel_GetUnitLoc on the position and drop the result. Column 0 with the flag word -1
 * stores at +4 by TaLogic_IsAttackedNow(hi, lo, c) and SzDuel_InqHandPhase(hi): 0x50A / 0x5FF when the
 * first is 1, else 0xA05 / 0x5FF, with these differences: c is 1 in 0x135790;
 * 0x150270 first sends TaLogic_CommandStdLDUnit(order, 3, 3, 0) when TaLogic015_IsActiveMode() is 1; 0x118330
 * and 0x11cb20 store 0xFF05 for the last outcome; 0x131540 stores 0xA06 and
 * puts SzDuel_GetUnitLoc(key == 0, 0) & 0xFF in the low half of the flag word, or 0x6FF
 * and that byte in bits 16-23 above the low half. By kind (SzDuel_GetUnitKind >> 5),
 * anything not listed storing 0x5FF at +4:
 *   0x14bde0  0: 0x14c720 when 0x2507d0 is 1, else 0x14c3b0; 1: 0x14c870;
 *             2: TaLogic_CommandEncUnit(order, 5, 0, TaLogic_GetWarnHideLvl__0014f9b0(key) & 0xFF).
 *   0x150270  0, 1, 3, 4: 0x150850;
 *             2: TaLogic_CommandEncUnit(order, 5, 0, TaLogic_GetWarnHideLvl__00153ed0(key) & 0xFF).
 *   0x135790  0: 0x136010 when 0x1e1030 is 189, else TaLogic009_CommandBtlUnit(order, 1);
 *             1: 0x1360e0; 2: TaLogic_CommandEncUnit(order, 2, 1, 0); 3: 0x136010.
 *   0x13e1d0  0: 0x13ecf0 when 0x2507d0 is 1, else TaLogic011_CommandBtlUnit(order, 1);
 *             2: TaLogic_CommandEncUnit(order, 2, 0, TaLogic_GetWarnHideLvl__00141b30(key) & 0xFF);
 *             3: 0xBFF when TaLogic_CommandGunnerUnit(order, 0) is 1, else 0x5FF.
 *   0x118330  0: TaLogic_CommandBtlUnit__001188f0(order, 1);
 *             2: TaLogic_CommandEncUnit(order, 3, 0, TaLogic_GetWarnHideLvl__0011aa10(key) & 0xFF);
 *             3: 0x118f50.
 *   0x11cb20  0: 0x11d9b0 when 0x2507d0 is 1, else TaLogic_CommandBtlUnit__0011d110(order, 1);
 *             2: TaLogic_CommandEncUnit(order, 4, 0, TaLogic_GetWarnHideLvl__0011fa80(key) & 0xFF);
 *             3: 0x11dc00.
 *   0x131540  0: TaLogic008_CommandBtlUnit(order, 1); 1, 3: 0x132280;
 *             2: TaLogic_CommandEncUnit(order, 4, 0, TaLogic_GetWarnHideLvl__00134ae0(key) & 0xFF).
 * Assumed: the order view and the int result; the signatures of
 * 0x14c720, 0x14c870, 0x1360e0, 0x13ecf0, 0x27d720, 0x118f50,
 * 0x11d9b0, 0x11dc00 and 0x132280; `_order`, `_key` and `_flag` are views of
 * functions declared in other groups with other types. */
extern void TaLogic014_CommandEfcMonster(struct DotrRuleOrder *order);
extern void func_0014c3b0_order(struct DotrRuleOrder *order);
extern void TaLogic014_CommandSpellUnit(struct DotrRuleOrder *order);
extern int func_0014f9b0_key(int key);
extern int func_00153ab0_flag(void);
extern void func_00150850_order(struct DotrRuleOrder *order);
extern int func_00153ed0_key(int key);
extern void func_00136010_order(struct DotrRuleOrder *order);
extern void func_00135d50_order(struct DotrRuleOrder *order, int flag);
extern void TaLogic009_CommandSpellUnit(struct DotrRuleOrder *order);
extern void TaLogic011_CommandEfcMonster(struct DotrRuleOrder *order);
extern void func_0013e7b0_order(struct DotrRuleOrder *order, int flag);
extern int func_00141b30_key(int key);
extern int TaLogic_CommandGunnerUnit(struct DotrRuleOrder *order, int a);
extern void func_001188f0_order(struct DotrRuleOrder *order, int flag);
extern void TaLogic_CommandTrapUnit__00118f50(struct DotrRuleOrder *order);
extern int func_0011aa10_key(int key);
extern void TaLogic_CommandEfcMonster__0011d9b0(struct DotrRuleOrder *order);
extern void func_0011d110_order(struct DotrRuleOrder *order, int flag);
extern void TaLogic_CommandTrapUnit__0011dc00(struct DotrRuleOrder *order);
extern int func_0011fa80_key(int key);
extern void func_00131b30_order(struct DotrRuleOrder *order, int flag);
extern void TaLogic008_CommandSpellUnit(struct DotrRuleOrder *order);
extern int func_00134ae0_key(int key);
int TaLogic_CommandUnit__0014bde0(struct DotrRuleOrder *order);
int TaLogic_CommandUnit__00150270(struct DotrRuleOrder *order);
int TaLogic_CommandUnit__00135790(struct DotrRuleOrder *order);
int TaLogic_CommandUnit__0013e1d0(struct DotrRuleOrder *order);
int TaLogic_CommandUnit__00118330(struct DotrRuleOrder *order);
int TaLogic_CommandUnit__0011cb20(struct DotrRuleOrder *order);
int TaLogic_CommandUnit__00131540(struct DotrRuleOrder *order);

/* 0x16d6a0, the prepare callee of TaLogic_GetNextActiveUnit__0016d8c0 (called through the `_entry` view,
 * result ignored). As 0x15d8e0, but it first keeps the 0x1e1030 word of the
 * position: kind 0 calls TaLogic022_CommandEfcMonster when TaUtil_IsExMonster is 1 or that word is 54..57,
 * else TaLogic022_CommandBtlUnit; kind 2 sends TaLogic_CommandEncUnit(order, 5, 0,
 * TaLogic_GetWarnHideLvl__00172310(key) & 0xFF); kinds 1, 3 and 4 call TaLogic022_CommandSpellUnit; other
 * kinds store 0x5FF. Assumed: the order view, the int result, the signatures of TaLogic022_CommandEfcMonster,
 * TaLogic022_CommandBtlUnit and TaLogic022_CommandSpellUnit, the `_key` view of position_rules' 0x172310. */
extern void TaLogic022_CommandEfcMonster(struct DotrRuleOrder *order);
extern void TaLogic022_CommandBtlUnit(struct DotrRuleOrder *order);
extern void TaLogic022_CommandSpellUnit(struct DotrRuleOrder *order);
extern int func_00172310_key(int key);
int TaLogic_CommandUnit__0016d6a0(struct DotrRuleOrder *order);

/* 0x154ad0 and 0x167e80: as 0x118510, except that the third list (31, both
 * acceptances) is walked before the side's slot is prepared and tried. The
 * prepare callees are 0x1547a0 and lower_rules' 0x167d30, the tries 0x155770
 * and step_picks' 0x1687a0. Assumed as for 0x118510, including the signatures
 * of 0x1547a0 and 0x155770; 0x167d30 and 0x1687a0 are called through views that
 * differ from their own groups' declarations. */
extern void func_001547a0_entry(struct DotrRuleEntry *entry);
extern int TaLogic_GetMinimumTask_Summon__00155770(int position, int side, struct DotrRuleStep *step);
extern void func_00167d30_entry(struct DotrRuleEntry *entry);
extern int func_001687a0_step(int position, int side, struct DotrRuleStep *step);
int TaLogic_GetNextActiveUnit__00154ad0(struct DotrRuleSet *set, int side, struct DotrRuleStep *step);
int TaLogic_GetNextActiveUnit__00167e80(struct DotrRuleSet *set, int side, struct DotrRuleStep *step);

/* 0x1547a0, the prepare callee of TaLogic_GetNextActiveUnit__00154ad0 (called through the `_entry` view,
 * result ignored). Rejections and the first column 0 stores as 0x15d8e0. When
 * TaLogic_IsAttackedNow is not 1 it asks SzDuel_GetMapUnitID(3, 5): a non-negative answer whose
 * Ta_IsUnitControl key equals the position's, with the position's SzDuel_GetUnitLoc word 0x403
 * and the answer not the position itself, stores 0x20A or 0x2FF at +4 (by
 * SzDuel_InqHandPhase) and the answer above the low half of the flag word; otherwise
 * 0xA06 with 0x403 in the low half, or 0x6FF with 0x0403 in the high half.
 * Kind 0 calls TaLogic017_CommandEfcMonster when TaUtil_IsExMonster is 1 or TaLogic017_IsExzParts(the
 * SzDuel_GetUnitCardNo word) is 1, else TaLogic017_CommandBtlUnit; kinds 1 to 4 call
 * TaLogic017_CommandSpellUnit; other kinds store 0x5FF.
 * Assumed: the order view, the int result, the signatures of TaLogic017_CommandEfcMonster,
 * TaLogic017_CommandBtlUnit and TaLogic017_CommandSpellUnit and of SzDuel_GetMapUnitID as other groups
 * declare it. */
extern int SzDuel_GetMapUnitID(int x, int y);
/* TaLogic_IsPassRoot (declared in small_misc.h): seven arguments, the last a pointer to a 16-bit unit id. */
extern int TaLogic_IsPassRoot(int side, int pos, int col, int row, int destCol, int destRow, unsigned short *atkTargetPtr);
/* 0x276b60: prototype and the local gUnitID from the Japanese build's debug information. */
int TaLogic_GetQueueXY_Move1(int nSide, int nID, int nX, int nZ, int gX, int gZ);
extern void TaLogic017_CommandEfcMonster(struct DotrRuleOrder *order);
extern void TaLogic017_CommandBtlUnit(struct DotrRuleOrder *order);
extern void TaLogic017_CommandSpellUnit(struct DotrRuleOrder *order);
int TaLogic_CommandUnit__001547a0(struct DotrRuleOrder *order);

/* TaLogic_GetMinimumTask_TrapMission__00116b60 (and its look-alikes 0x12aac0, 0x130120, 0x1415f0, 0x15c670):
 * the spot is the upper half of the order's flags word when
 * mode is 1, else its low halfword. Without a SzDuel_GetUnit word at the spot:
 * TaLogic_InitRogicControl(order, 1) and -1. Otherwise result kind 6 and 0 are returned with
 * one of two fills. The fallback (position, the SzDuel_GetUnitLoc cell of the order's
 * position, TaLogic_IsDefStyleUnit(.., cell, 1, 0)) is used when the spot is 0xffff, when
 * GetMoveRev(the position's SzDuel_GetUnit word, cell low, cell high) is 0, when
 * TaLogic_IsAttackedNow(column, row, 0) is 1, when TaLogic_GetMinGoalCount counts no entry and when no
 * entry is chosen. Else the twelve nibble pairs are set to 15 / 15, filled by
 * TaLogic_GetInvadeRoot(column, row, pairs, cell low, cell high, spot-cell low, spot-cell
 * high, spot, 0, 30, key(side)) and each pair without a SzDuel_GetMapUnit item is
 * scored by TaLogic_GetUnitWarning(low, high, side, 3, 1, key(side)); a higher score wins,
 * an equal one only when nearer the spot's cell (sum of the two absolute
 * differences); the winner is stored as high << 8 | low with
 * TaLogic_IsDefStyleUnit(.., that, 0, 0).
 * An equal score while nothing is chosen (a first score of 0) reads pair 255
 * of the twelve, outside the array, as the original does.
 * Assumed: the unsigned char fourth parameter (masked on entry), the
 * signature of TaLogic_GetUnitWarning (unsigned char last argument, masked
 * by this caller) and int results; `_rule` views of TaLogic_GetInvadeRoot (pointer third
 * argument), TaLogic_GetMinGoalCount and GetMoveRev (int first argument, int result). */
extern int func_0027a090_rule(int a, int b, struct DotrRuleNibbles *cells, int d, int e, int f, int g, int h, int i,
                              unsigned char j, unsigned short k);
extern int func_0027afc0_rule(struct DotrRuleNibbles *cells);
extern int TaLogic_GetUnitWarning(int a, int b, int c, int d, int e, unsigned char f);
extern int func_002464f0_rule(int item, int x, int y);
int TaLogic_GetMinimumTask_TrapMission__00116b60(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);

/* Copies of 0x116b60 with another key callee: 0x130780, 0x141b30, 0x15d120.
 * Assumed: as for 0x116b60; `_key` view of position_rules' TaLogic_GetWarnHideLvl__00130780 (int
 * argument and result). */
extern int func_00130780_key(int key);
int TaLogic_GetMinimumTask_TrapMission__00130120(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
int TaLogic_GetMinimumTask_TrapMission__001415f0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
int TaLogic_GetMinimumTask_TrapMission__0015c670(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
/* 0x11b0e0 and 0x120150: 0x15c670 with the key choice expanded in place (thresholds 1500 and
 * 1000). Assumed: SzDuel_GetLP is passed the side here although the function reads no argument. */
extern int func_001df470_side(int side);
int TaLogic_GetMinimumTask_TrapMission__0011b0e0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);
int TaLogic_GetMinimumTask_TrapMission__00120150(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);

/* As 0x15c670 with key callee 0x153ed0, except: the low byte of TaLogic015_IsActiveMode() is
 * taken first and the fourth TaLogic_GetUnitWarning argument is 2 when it is zero, else 5.
 * Assumed: as for 0x116b60. */
int TaLogic_GetMinimumTask_TrapMission__00153550(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);

/* Copy of 0x116b60 with the key callee 0x12b1b0 and 5 as the fourth TaLogic_GetUnitWarning
 * argument. Assumed: as for 0x116b60; `_key` view of position_rules' 0x12b1b0. */
extern int func_0012b1b0_key(int side);
int TaLogic_GetMinimumTask_TrapMission__0012aac0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);

/* 0x135060 / 0x1422e0: copies of 0x1676e0 with the finders 0x131060 / 0x13dca0
 * (signatures assumed from the argument registers, as for 0x163130). */
extern int TaLogic_GetMinimumTask__00131060(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);
int L008(int side, struct DotrRuleSet *set);


extern int TaLogic_GetMinimumTask__0013dca0(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);
int L011(int side, struct DotrRuleSet *set);

/* 0x142280: 1 when Ta_GetUnitNumFrom22bitFlagList counts 3 or more in the SearchUnitByRange word for (0, 0,
 * side, 1, 0x3E80025, 14). */
int TaLogic011_IsUseReversalGoddess(int side);

/* 0x12c1a0: same shape as 0x113140 without the leading SzDuel_GetUnitLoc call. Callee signatures assumed
 * from the argument registers; TaLogic007_CommandBtlUnit is passed `deep` unmasked here, hence the `_order`
 * view of the (order, flag, unsigned char) function. `func_0012c1a0_entry` above is
 * 0x12c3f0's view of this function. */
extern void TaLogic007_CommandEfcMonster(struct DotrRuleOrder *order, int deep);
extern void func_0012c7f0_order(struct DotrRuleOrder *order, int flag, int deep);
extern void TaLogic_CommandTrapUnit__0012ce50(struct DotrRuleOrder *order);
int TaLogic_CommandUnit__0012c1a0(struct DotrRuleOrder *order, int deep);

/* TaLogic_CommandUnit__001271f0 is 0x163660's shape without the leading SzDuel_GetUnitLoc call: 0 as
 * the third TaLogic_IsAttackedNow argument, bare 0xA06 / 0x6FF stores (flags keep the low or the
 * zero-extended half), kind 0 alone asks TaUtil_IsExMonster and kinds 1, 3, 4, 5 run 0x127d20. Assumed:
 * the order views of TaLogic006_CommandEfcMonster and TaLogic006_CommandBtlUnit (declared elsewhere
 * with other order types); func_001271f0_entry is 0x1273e0's view of it. */
extern void func_00127b00_order(struct DotrRuleOrder *order);
extern void func_001277c0_rule(struct DotrRuleOrder *order, int flag);
int TaLogic_CommandUnit__001271f0(struct DotrRuleOrder *order);

/* TaLogic_CommandUnit__0010ce20 is the same shape with the set as first argument: it first
 * stores the low four bits of TaLogic002_IsActiveMode() into the low nibble of the set's byte 0x1a1 and
 * passes that nibble to TaLogic_CommandEfcMonster__0010d880 / TaLogic_CommandBtlUnit__0010d490 (kind 0); kind
 * 1 runs 0x10dc40, kind 2 sends TaLogic_CommandEncUnit(order, 2, 0, TaLogic_GetWarnHideLvl__0010eda0(key)
 * & 0xFF), kinds 3 and 4 call TaLogic_CommandDefenseUnit(order, 2, 0), kind 5
 * TaLogic_CommandDefenseUnit(order, 1, 0).
 * Assumed: the order views of 0x10d880, 0x10d490 and 0x10db00; DotrRuleSetLow is a view of
 * DotrRuleSet used for the nibble store only; func_0010ce20_entry is 0x10d080's view. */
struct DotrRuleSetLow {
    unsigned char pad[0x1a1];
    unsigned char low : 4;
};
extern int TaLogic002_IsActiveMode(void);
extern int TaLogic_GetWarnHideLvl__0010eda0(int side);
extern void func_0010d880_rule(struct DotrRuleOrder *order, int flag, int deep);
extern void func_0010d490_rule(struct DotrRuleOrder *order, int flag, int deep);
extern void func_0010db00_rule(struct DotrRuleOrder *order, int one, int hit);
int TaLogic_CommandUnit__0010ce20(struct DotrRuleSet *set, struct DotrRuleOrder *order);

/* 0x171d60: the 0x138780 rule with the finder TaLogicCom_IsUseRevive (three arguments
 * seen) fed from the SzDuel_GetUnitEffect entry of the order's cell: entry 3
 * when SzDuel_GetUnitKind >> 5 is 0, else entry 0. With a non-zero kind the find is
 * dropped when SzDuel_GetField answers 9 or TaLogic_GetActionReaction answers 5, and the bit-4 exit
 * applies. loc takes the side instead of the found index unless the
 * entry's bits 6-7 are 3. Assumed: the entry is a word pointer. */
extern unsigned *SzDuel_GetUnitEffect(int side, int pos, int effInd);
extern int TaLogicCom_IsUseRevive(int side, int sidesToCount, int mode);
int TaLogic_GetMinimumTask_ReviveMission__00171d60(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode);

/* 0x161bf0: the 0x171d60 rule with two finders in place of TaLogicCom_IsUseRevive: with an
 * entry, TaLogic019_IsUseRevive(side) for a non-zero kind (then the same two vetoes) and
 * TaLogic019_IsUseSaint(side) for kind 0; falls back to 0x1607d0. */
int TaLogic_GetMinimumTask_ReviveMission__00161bf0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode);

/* 0x1170a0: the 0x171d60 rule with the finder TaLogic001_IsUseRevive(side). SzDuel_GetUnitKind is
 * asked a second time after SzDuel_GetUnitLoc and that answer >> 5 gates the two
 * vetoes; the bit-4 exit applies whatever the kind; falls back to 0x115850
 * (the same four arguments passed on). */
extern int TaLogic001_IsUseRevive(int side);
/* func_00115850_rest is TaLogic_GetMinimumTask_RunMission__00115850 (reach_picks, unsigned char
 * mode) as the caller here sees it: int mode. */
extern int func_00115850_rest(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode);
int TaLogic_GetMinimumTask_ReviveMission__001170a0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode);

/* 0x16b0f0: 1 unless the cell named by entry 0 of the SearchUnitByRange(0, 0, side, 3, 33, 14)
 * list belongs to another side (Ta_IsUnitControl), scores 3000 or more through
 * TaUtil_GetUnitAP and three or more of the side's cells 1..10 answer non-zero to
 * SzDuel_GetUnit. Assumed: plain int arguments throughout. */
extern int TaUtil_GetUnitAP(int side, int pos, int col, int row);
int TaLogic021_IsUseBlackHole(int side);

/* 0x16b1f0: the 0x1676e0 step with the finder TaLogic_GetMinimumTask__001678b0 (same three
 * arguments). */
extern int TaLogic_GetMinimumTask__001678b0(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);
int L021(int side, struct DotrRuleSet *set);

/* Seven more copies of the 0x1676e0 step, with the finders 0x10c940, 0x135230,
 * 0x14b900, 0x12bb70, 0x146960, 0x154220 and 0x15d350 (same assumed three
 * arguments). */
extern int TaLogic_GetMinimumTask__0010c940(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);
extern int TaLogic_GetMinimumTask__00135230(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);
extern int TaLogic_GetMinimumTask__0014b900(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);
extern int TaLogic_GetMinimumTask__0012bb70(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);
extern int TaLogic_GetMinimumTask__00146960(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);
extern int TaLogic_GetMinimumTask__00154220(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);
extern int TaLogic_GetMinimumTask__0015d350(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);
int L002(int side, struct DotrRuleSet *set);
int L009(int side, struct DotrRuleSet *set);
int L014(int side, struct DotrRuleSet *set);
int L007(int side, struct DotrRuleSet *set);
int L013(int side, struct DotrRuleSet *set);
int L017(int side, struct DotrRuleSet *set);
int L019(int side, struct DotrRuleSet *set);

/* 0x154040: the 0x1676e0 step with the finder TaLogic_GetMinimumTask__0014fd40 (same three
 * arguments). */
extern int TaLogic_GetMinimumTask__0014fd40(struct DotrRuleSet *set, struct DotrRuleStep *step, int side);
int L015(int side, struct DotrRuleSet *set);
/* 0x154210: a jump to 0x172370 with both arguments untouched. Assumed: the
 * signature of 0x172370. */
int L016(int side, struct DotrRuleSet *set);

/* 0x1de5c0 with its usual signature. */
extern int SzDuel_GetSummonPower(int side);

/* Calls SzDuel_GetUnitCardNo, Ta_IsUnitControl, SzDuel_GetUnitEffect(.., 0) and SzDuel_GetUnitLoc for (side,
 * index) and uses only the first and third answers. 0 when SzDuel_GetUnitStatus(side, index, 4)
 * is not 0. Otherwise by SzDuel_GetUnitKind(side, index) >> 5: for 0, 1 when the
 * SzDuel_GetUnitCardNo answer is 224 or 364 and SzDuel_GetSummonPower(side) < 4; for 1, 2 when
 * TaLogic_GetActionReaction(side, index, 0, 0, 6, 1) is 5 or the entry is null, else by the
 * entry word >> 8: 39 gives 1 when SzDuel_GetSummonPower(side) < 4 and 4 gives 1 when
 * TaLogic021_IsUseBlackHole(side) is 1. Every other path answers 2.
 * Assumed: int arguments and result. */
struct DotrRuleOrder;
int TaLogic_IsOpenUnit__0016ae30(int side, int index, struct DotrRuleOrder *order);

/* 0x27e780. Prototype, local names and the task layout are from the Japanese build's debug
 * information (driver.c); the structure tag is unnamed there. Sets the task purpose to
 * 0x6ff with the enemy's location in the high half of dwGoal when TaLogic_GetActionReaction
 * answers 1 or 2, otherwise 0x2ff with the enemy id. */
struct jp_9add8 {
    unsigned short wUnit;
    unsigned short wOrder;
    unsigned short wPurpose;
    unsigned int dwGoal;
    unsigned char bCond : 2;
};
void TaLogic_SetTaskAtkOrMove(struct jp_9add8 *pTask, int nEnemyID, unsigned char fView);

/* TaLogic_IsUnitCategory (0x27c5a0): name and the (nID, sKind) parameters from the Japanese
 * build's debug information; this build passes one more leading argument that the body never
 * reads (its name nSide is an assumption). */
int TaLogic_IsUnitCategory(int nSide, int nID, int sKind);

/* TaLogic_GetEnchantQue (0x278f10): prototype, locals and both structures from the Japanese
 * build's debug information. The callee prototypes below are assumptions (small_misc.h types
 * TaLogic_IsKeepEnchant's first parameter as struct DotrMiscRule *; pick_copies.h declares
 * this function itself with untyped parameters); the views are not reconciled. */
struct TreeTag {
    unsigned short nRoot;
    unsigned char nComboLink[5];
    struct TreeTag *pNextLink[4];
};
struct jp_9c054 {
    unsigned short wCardNo;
    unsigned short wObj;
    unsigned int dwPar;
};
extern int Ta_IsActiveTrapB(int side, int trapEffect);
int TaLogic_IsUseHandInCombo(unsigned char handSlot, unsigned char *combo);
extern int Ta_GetHandEnchant(int cardNo, int equipNo);
int TaEnc_IsEncDo(int eNO);
int TaLogic_IsKeepEnchant(struct jp_9c054 *rule, int cardId, int fusionResult);
int TaLogic_GetArrayLength(unsigned char *array);
extern int TaEnc_GetEncScore(int cardID);
int TaLogic_GetEnchantQue(int nSide, struct TreeTag *p_fTree, struct jp_9c054 **pKeep, unsigned char nKeepDepth);

/* 0x14aab0 TaLogic_GetMinimumTask_ReviveMission (original name; third copy): the frame of
 * 0x138780 with finder TaLogic013_IsUseRevive (this group), an extra test that drops the
 * answer when SzDuel_GetField at the unit's location is 9, last arguments 0 for TaLogic_GetActionReaction and
 * 0x27e360 and 1 for 0x27dbc0, and the fallback 0x149690. Assumed: the fallback's int mode
 * view (reach_picks.h declares it as TaLogic_GetMinimumTask_RunMission__00149690 with an
 * unsigned char mode; with that prototype this caller gets an andi the original lacks). */
extern int func_00149690_rest(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode);
int TaLogic_GetMinimumTask_ReviveMission__0014aab0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode);

#endif
