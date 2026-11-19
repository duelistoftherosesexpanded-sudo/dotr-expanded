#ifndef DOTR_EE_STATE_QUERIES_H
#define DOTR_EE_STATE_QUERIES_H

#include "dotr/ee/state_grid.h"

/* Queries over the duel record and entry routines, mostly for the AI logic.
 * Meanings are largely unassigned. Evidence: docs/modules/talogic.md. */
/* 24-byte tree node written by 0x278930 and searched by 0x278e40: an id,
 * five bytes ending at the first 255, and four child links. small_misc.h
 * passes the same records as opaque 24-byte objects. On hosts with wider
 * pointers this view is larger than the target's 24 bytes. */
struct DotrStateNode {
    unsigned short result;
    unsigned char combo[5];
    struct DotrStateNode *children[4];
};

int WallFusion(int side, int pos);
int Ta_GetCurrentSide(int side, int pos);
int CheckLandAgreement(int field, struct DotrStateItem *unit);
int CheckLandAgreement2(int field, int side, int pos);
int Ta_GetAPPlusDP(int side, int pos);
int Ta_DelUnit(int side, int pos);
int Ta_DelUnit2(int a, int b);
int Ta_IsGraveEntryHit(int side, int position, int mode);
int TaLogic_IsUnitWin3(struct DotrStateItem *unit, int base, int x, int y);
void TaLogic_InitTree(struct DotrStateNode *node, unsigned short id, unsigned char *source, int handIndex);
struct DotrStateNode *TaLogic_GetFusionRes(struct DotrStateNode *node);
int Ta_IsVirus_Chk2(struct DotrStateItem *unit);
int Ta_IsVirus_Chk3(struct DotrStateItem *unit);
/* 0x250a60 as called by 0x24fe40, which sets only the item argument. */
extern int func_00250a60_item(struct DotrStateItem *item);
void Ta_SetUnitCurseRest(int a, int b, unsigned char n);
struct DotrStateEntry8 *Ta_GetUnitEffect(int side, int pos, int effInd);
int TaUtil_IsExMonster(int side, int pos);
int TaUtil_GetGraveCardNum(int side, int kind);
int TaUtil_GetUnitAP(int side, int pos, int col, int row);
int TaUtil_GetUnitAP2(struct DotrStateItem *unit, int col, int row);
int TaUtil_GetUnitAP3(int side, int pos, int col, int row);
int TaUtil_GetUnitDP(int side, int pos, int col, int row);
int TaUtil_GetUnitDP2(struct DotrStateItem *unit, int col, int row);
int TaUtil_GetUnitDP3(int side, int pos, int col, int row);
void TaView_SetModelFrontView(int *out, int packed, int add);
void TaView_SetExzModelFrontView(int *out, int packed, int add);
void TaView_SetModelBackView(int *out, int packed, int add);
int Ta_SetDestinyEffect(int side, int *out);
int TaHand_IsCardSummon2(int side, int cardNo);
int TaHand_IsCardSummon(int index, int value);
int TaHand_GetCardAPDP(int id);
int TaHand_IsCardEffect(int id);
int TaHand_GetCardKind(int id);
int TaHand_GetNextHandNum(int side);

/* small_misc.h declares it with one parameter for the one-argument wrapper
 * TaHand_GetCardSummonCost just before it. */
extern int TaHand_GetCardSummonCost2(int cardNo, int teamID);
extern int Ta_GetParamOfs(struct DotrStateItem *item, int c, int d, int flag);
/* The 0-9999 clamp, defined in the small_misc group. */
extern int GetParCap(int stat);
/* SzEffect_GetModelRot2 (defined in packed_item over a four-word vector view) as called
 * with a plain word pointer. */
extern int func_001f6160_words(int packed, int *words);
/* 0x24deb0 passes its own two arguments through and 0x24df00
 * passes the looked-up item. */
extern int Do_CheckLandAgreement(int a, struct DotrStateItem *item);
/* 0x250900 as called by 0x278420 with the item pointer; small_misc.h holds
 * its definition over a prefix view of the same object. */
extern int func_00250900_item(struct DotrStateItem *item);
/* Defined elsewhere (small_misc, side_table); declarations repeated because
 * those headers are not included with this one. */
extern int GetCap(int limit, int value);
extern void TaLogic_CopyArray(unsigned char *source, unsigned char *destination);
extern int TaLogic_GetArrayLength(unsigned char *array);
extern int SzData_CalcExp(int side, int cardNo, int operation, int expChange);

/* Grid bit query and presence word (also declared in small_misc.h/state_grid.h;
 * do not include those with this header). */
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int func_001df540_word(int a, int b);
int Ta_SearchUncTypeNaturalEffectUser(int kind, int n);
int Ta_SearchUncTypeNaturalEffectUserNum(int kind);

/* Slots 0..10, side 0 then side 1 at each index: a present slot with an entry,
 * grid bits 4, 3 and 5 and entry kind 23 is turned into a word by
 * SearchUnitByRange(0, 0, Ta_GetCurrentSide(side, i), (w >> 6) & 3, w & 0x3f, 14); the first one
 * Ta_GetAgreeOwnerIDon22bitFlagLst((a << 8) | b, word) accepts returns 1, otherwise 0. Assumed: two int
 * arguments; 0x24ea10 as declared for small_misc; 0x24ca40 as these callers pass it. */
extern int Ta_GetAgreeOwnerIDon22bitFlagLst(int ownerId, unsigned flagList);
int TaPerm_IsPermCurseActive(int a, int b);
int Ta_SearchUncTypeNaturalEffectUserNumWithSide(int kind, int mode);

/* Graveyard accessors (state_records): entry count and the id at a position of graveyard
 * list `index`. */
extern int SzDuel_GetGraveCount(int side);
extern int SzDuel_InqGrave(int side, int position);

/* Counts how many of the first `target` entries of list `index` are class-0 (mode 29) or
 * non-class-0 (mode 30) by TaHand_GetCardKind(id) >> 5; any other mode, or a target past
 * the list, returns target & 0xff. */
int TaLogic_GetGraveEntryAfterScreen(int side, int target, int mode);
/* Counts entries equal to `id` in list `index` (flags bit 0) plus in the other list
 * 0 <-> 1 (flags >> 1), mod 256. */
int TaLogic_GetGraveCardCount(int side, int flags, int id);
/* Sums the values of kind-52 entries: type 0 adds, type 1 subtracts the low half of
 * word 1; other types are ignored. */
int Ta_GetEnchantOfs(void);
/* Votes over kind-86 entries: mode 3 counts +1, mode 2 counts -1 unless Ta_GetCurrentSide
 * of the entry's slot equals who. Returns the sign of the total. */
int Ta_GetSummonCalcRevision(int who);
/* For an item whose side (unknown_44, inverted when unknown_57 bit 1 is set) differs from
 * SzDuel_GetTurnOwner() and whose unknown_45 is non-zero: the first kind-68 entry with low
 * six bits 30 or 29 decides, by unknown_3e >> 5 being non-zero (30) or zero (29).
 * Otherwise 0. struct DotrStateItem parameter assumed from the offsets. */
int Ta_IsUnitHide2(struct DotrStateItem *item);
/* Best-scoring eligible entry (state byte >> 5 == 0, score = high halfword of
 * TaHand_GetCardAPDP) over the own side (mask bit 0) and the other side (mask >> 1
 * non-zero); ties keep the earlier find. Returns (owner << 24) | (index << 16) | score,
 * or -1 when nothing is eligible. */
int TaLogic_GetMaxAPGraveCard(int side, int mask);
/* Only the halfword at +0x1a2 is observed. */
struct DotrQueryObject1a2 {
    unsigned char unknown_000[0x1a2];
    short unknown_1a2;
};
/* One int argument and an int result compared with 1, from this caller alone. */
extern int TaTuto_IsTuto(int side);
/* Result returned unchanged by TaLogic_AdjustLogicData; int assumed. */
extern int TaLogic_DoAdjustLogicData(struct DotrQueryObject1a2 *p);
int TaLogic_AdjustLogicData(struct DotrQueryObject1a2 *p);
/* Two int arguments and an int result, from Ta_GetHandEnchant2 alone. */
extern int Ta_GetHandEnchant3(int a, int b);
int Ta_GetHandEnchant2(int side, int pos, int equipNo);
/* Two int arguments and an int result, from Ta_GetForceEffection alone. */
extern int Ta_GetLPRevByNaturalEffect(int lpChangeType, int lifeAmount);
int Ta_GetForceEffection(int side, int b);
/* small_misc.h declares this callee with the same type. */
int Ta_GetHandEnchant(int cardNo, int equipNo);
/* One pointer argument passed through from the caller; int result. A view of
 * 0x27fda0, which side_table.h declares with an int id argument. */
extern int func_0027fda0_ptr(void *p);
/* a is only forwarded; p is a 16-bit position stepped by 80 up to 480. */
int TaTuto_OpenMsgPopup(void *a, unsigned short *p);
int TaTuto_CloseMsgPopup(void *a, unsigned short *p);
extern int Ta_GetLDLvRev(int a);
int Ta_GetSummonRev(int side, int id);
/* Defined in small_misc; declared here for Ta_IsLDRangeAvilityFromLoc. */
extern int Ta_IsLDAbl(int ability, int side);
int Ta_IsLDRangeAvilityFromLoc(int packed, int side, int kind, int ability);
/* Ten arguments: eight in registers, a byte and a halfword on the stack. */
extern int TaLogic_GetInvadeRoot2(int a, int b, int c, int d, int e, int f, int g, int h, unsigned char i,
                         unsigned short j);
int TaLogic_GetInvadeRoot(int side, int pos, int c, int d, int e, int f, int g, int h, int i, unsigned char j,
                  unsigned short k);
/* Five int arguments; result tested for zero. */
extern int Ta_IsSquareRange(int a, int b, int c, int d, int e);
/* Defined in the state_records group. */
int Ta_IsLDRange(int side, int col, int row);
/* Both defined in the side_table group. */
int Ta_IsLDRangeAvility(int side, int pos, int ability);
int Ta_IsLDRangeAvilityWithLocation(int side, int unitArrayPos, int col, int row, int ability);
/* First three bytes of an entry as TaHand_GetCardSummonCost2 reads them. */
struct DotrQueryEntryBits {
    unsigned char unknown_0;
    unsigned char unknown_1;
    unsigned char unknown_2_low : 4;
    unsigned char unknown_2_high : 4;
};
/* Defined in the side_table group. small_misc.h's one-argument
 * TaHand_GetCardSummonCost is the thunk before it. */
int TaHand_GetCardSummonCost2(int cardNo, int teamID);

/* Callees of Ta_IsDestinyDraw: defined in state_records (0x1de4e0, 0x1df470)
 * and side_table (0x2158b0, 0x2159d0, 0x215d30); the one-int prototype of
 * TaHand_IsDestinyCondition (0x26efd0, hand_destiny) is assumed. */
extern int SzDuel_GetPlayer(int side);
extern int SzData_GetCurrentDeck(int side);
extern int SzData_GetLeader(int side, int deckNo);
extern int SzData_GetRank(int side, int cardNo);
extern int SzDuel_GetLP(int side);
extern int TaHand_IsDestinyCondition(int side);
int Ta_IsDestinyDraw(int side);

/* Ta_IsHandHeap (defined in the side_table group) pairs the ids at two
 * (side, position) cells and fills a four-word result. The first
 * byte of each 0x1dd0e0 entry is read as a kind in its top three bits (byte
 * view of the entry assumed). Both packed words use the FIRST side; the
 * second side only reaches the callees. Fusion (0x2495a0) is defined in small_misc;
 * parameter meanings and the four-word output are assumed. */
extern int Fusion(int cardA, int cardB);
int Ta_IsHandHeap(int side, int pos, int other_side, int other_pos, int *out);

/* Ta_GetLPRevByNaturalEffect (defined in the side_table group) applies the kind-59 modifiers
 * found at every usable cell to a starting amount. DotrStateModifier is this
 * function's view of the 8-byte entry returned by 0x250740: the low halfword
 * of the second word is read both alone and through the whole word. Names and
 * meanings are assumed. */
struct DotrStateModifier {
    unsigned head;
    union {
        unsigned word;
        struct {
            unsigned short amount;
            unsigned short op;
        } half;
    } u;
};

/* Callee views for TaLogic_IsUnitWin2 (assumed from that function alone): the
 * item-typed 0x250900, the record-returning 0x1df540 and the item-typed
 * 0x2787f0, plus same-signature aliases of the others so that its host test
 * can stub them separately from this group's definitions. */
extern int func_00250900_score(struct DotrStateItem *item);
extern int func_00250bc0_score(struct DotrStateItem *item, int c, int d);
extern int func_00250a60_score(struct DotrStateItem *item, int c, int d);
extern struct DotrStateItem *func_001df540_score(int a, int b);
extern int func_002787f0_score(struct DotrStateItem *a, struct DotrStateItem *b);
extern int func_001e0700_score(int a, int b);
extern int func_001e2100_score(int a, int b, int bit);
extern int func_00250b40_score(int a, int b, int c, int d);
extern int func_002509e0_score(int a, int b, int c, int d);
/* 0x2785d0: compares the item's two scores with a base. Null item 0;
 * position -1 forwards to TaLogic_IsUnitWin3(unit, base, col, row). When TaLogic_IsJudgeUnitDanger(high
 * byte, low byte, skip) answers 0 the scores are 0x250a60 and 0x250bc0 and
 * the base is the argument; otherwise they are 0x278250's own-side terms
 * (both 0 when 0x250900 answers 1 or the +0x3e byte has a high bit, else
 * each plus 0x2787f0) and the base its other-side 0x2509e0 term. Answers 3
 * when the 0x250bc0 score is below the 0x250a60 score and that reaches the
 * base, 2 when it is not below and exceeds the base, else 0. The byte-wide
 * skip is assumed from its masking; 0x278420 is called through an alias so
 * that the host test can answer it; 0x2781b0 is declared as in small_misc.h. */
extern int func_00278420_score(struct DotrStateItem *item, int base, int x, int y);
extern int TaLogic_IsJudgeUnitDanger(int side, int pos, int canSeeFaceDown);
int TaLogic_IsUnitWin5(struct DotrStateItem *unit, int position, int base, int col, int row, unsigned char skip);
/* small_misc.h declares the first parameter as int for its caller; here it
 * is the item it dereferences. position packs two bytes. */
int TaLogic_IsUnitWin2(struct DotrStateItem *sourceUnit, int position, int col, int row);

/* 0x25fb00 writes record word `index` through SzDuel_CalcLP (state_records.h
 * prototype repeated here). Modes 0/1 store Ta_GetLPRevByNaturalEffect(43 or 44, value) with
 * operation 0/1; modes 2/3 do the same from the current word (0x1df470);
 * mode 4 stores value with operation 4 only when it exceeds the current
 * word; other modes do nothing. Parameter names are assumed. */
extern int SzDuel_CalcLP(int side, int operation, int amount);
void Ta_CalcLPinEffectSeq(int side, int mode, int value);

/* 0x123d70 builds a five-bit mask (bit 4 - i for list[i], i below the
 * 0x26fe20 count) in an unsigned char: the first usable entry (0x26fb10 == 1)
 * of kind 0 (0x26fce0 >> 5), then the first of kind 18; only when the mask
 * is exactly 2 the first of kind 15 is added and the mask returned,
 * otherwise 0. The list as an int array and the name side are assumed. */
int TaLogic005_Is2Head_ThunderDragon(int side, int *list);

/* 0x1702b0 answers 1 for an id whose 0x26fce0 kind (>> 5) is nonzero or
 * whose 0x26fc10 state is 1. For ids 54-57 it also answers 1 when the
 * 0x1e1030 word of (side, 0) is 58 and 0x2151d0 finds each of 54, 55, 56
 * and 57 in the side's 0x2158b0 list; otherwise 0. side is SzDuel_GetTurnOwner(). */
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern int SzData_CheckDeck(int side, int deckNo, int cardNo);
extern int SzDuel_GetTurnOwner(void);
int TaLogic022_IsRefuseEmptyCombo(int id);

/* 0x136d60 and 0x11a2f0 (same code, limits 1000 and 3500): for an item, list
 * the slots below the 0x26fe20 count that 0x278e00 reports clear in the bytes
 * at item + 1, then for the first TaLogic_GetArrayLength(list) of them whose table id is
 * not 999, costs no more than the item (0x26fa80, low byte), has
 * low + 2 * high of its 0x26fbb0 word within the limit, is in state 2
 * (0x26fc10) and for which TaLogic_GetHandFusion(item id, id) is 999, call
 * TaLogic_AddEmptyCombo(item + 1, slot). The item as halfwords and the table as ints are
 * assumed; small_misc.h prototypes for 0x278e00/0x2789f0/0x278d20 repeated. */
extern int TaLogic_IsUseHandInCombo(unsigned char handSlot, unsigned char *combo);
extern int TaLogic_GetHandFusion(unsigned short cardNo, unsigned short equipNo);
extern void TaLogic_AddEmptyCombo(unsigned char *combo, int newCard);
void TaLogic_ConvEmptyCombo__00136d60(int side, unsigned short *fusionTree, int *table);
void TaLogic_ConvEmptyCombo__0011a2f0(int side, unsigned short *fusionTree, int *table);

/* 0x10e6b0 and 0x1700d0: the 0x11a2f0 code with one more test after the 999
 * test, skipping an id for which the veto answers nonzero: 0x10f220 for 0x10e6b0 (limit 3500),
 * 0x1702b0 for 0x1700d0 (limit 4000). position_rules.h prototype for
 * 0x10f220 repeated; same item/table assumptions as 0x136d60. */
extern int TaLogic002_IsRefuseEmptyCombo(int id);
void TaLogic_ConvEmptyCombo__0010e6b0(int side, unsigned short *fusionTree, int *table);
void TaLogic_ConvEmptyCombo__001700d0(int side, unsigned short *fusionTree, int *table);

/* Six more copies of the 0x10e6b0 code, each with its own veto callee and sum
 * limit: 0x114f20 (0x115100, 5000), 0x13b600 (0x13b7e0, 4000), 0x148d40 (0x148f20, 4200),
 * 0x14d5c0 (0x14d7a0, 4200), 0x151880 (0x151a60, 3500) and 0x160030 (0x160210, 4000).
 * The six veto functions are byte-identical: 1 for an id whose 0x26fce0 kind (>> 5) is
 * nonzero or whose 0x26fc10 state is 1, otherwise 0 (the start of 0x1702b0). int id and int return
 * assumed. */
int TaLogic001_IsRefuseEmptyCombo(int id);
void TaLogic_ConvEmptyCombo__00114f20(int side, unsigned short *fusionTree, int *table);
int TaLogic010_IsRefuseEmptyCombo(int id);
void TaLogic_ConvEmptyCombo__0013b600(int side, unsigned short *fusionTree, int *table);
int TaLogic013_IsRefuseEmptyCombo(int id);
void TaLogic_ConvEmptyCombo__00148d40(int side, unsigned short *fusionTree, int *table);
int TaLogic014_IsRefuseEmptyCombo(int id);
void TaLogic_ConvEmptyCombo__0014d5c0(int side, unsigned short *fusionTree, int *table);
int TaLogic015_IsRefuseEmptyCombo(int id);
void TaLogic_ConvEmptyCombo__00151880(int side, unsigned short *fusionTree, int *table);
int TaLogic019_IsRefuseEmptyCombo(int id);
int TaLogic020_IsRefuseEmptyCombo(int id);
int TaLogic021_IsRefuseEmptyCombo(int id);
void TaLogic_ConvEmptyCombo__00160030(int side, unsigned short *fusionTree, int *table);
/* 0x164ec0 (veto 0x1650a0) and 0x1691e0 (veto 0x1693c0): the same code with the sum test `== 0` instead of a limit. Same assumed signature. */
void TaLogic_ConvEmptyCombo__00164ec0(int side, unsigned short *fusionTree, int *table);
void TaLogic_ConvEmptyCombo__001691e0(int side, unsigned short *fusionTree, int *table);

/* The remaining seven copies of the 0x10e6b0 slot-pick code with their veto
 * callees and sum limits: 0x11f150 (0x11f330, 3500), 0x124a30 (0x124c10, 4000),
 * 0x128900 (0x128ae0, 3300), 0x12dfc0 (0x12e1a0, 4200), 0x132ee0 (0x1330c0, 4300),
 * 0x13fa90 (0x13fc70, 3100) and 0x144630 (0x144810, 2800). Every veto answers 1 for a nonzero 0x26fce0
 * kind (>> 5); then 0x11f330, 0x128ae0 and 0x1330c0 answer 1 for three to six fixed ids, 0x124c10
 * for a low kind (& 0x1f) of 0, 15 or 18, and the byte-identical 0x12e1a0, 0x13fc70 and 0x144810
 * for any nonzero 0x26fc10 state. int id and int return assumed. */
int TaLogic004_IsRefuseEmptyCombo(int id);
void TaLogic_ConvEmptyCombo__0011f150(int side, unsigned short *fusionTree, int *table);
int TaLogic005_IsRefuseEmptyCombo(int id);
void TaLogic_ConvEmptyCombo__00124a30(int side, unsigned short *fusionTree, int *table);
int TaLogic006_IsRefuseEmptyCombo(int id);
void TaLogic_ConvEmptyCombo__00128900(int side, unsigned short *fusionTree, int *table);
int TaLogic007_IsRefuseEmptyCombo(int id);
void TaLogic_ConvEmptyCombo__0012dfc0(int side, unsigned short *fusionTree, int *table);
int TaLogic008_IsRefuseEmptyCombo(int id);
void TaLogic_ConvEmptyCombo__00132ee0(int side, unsigned short *fusionTree, int *table);
int TaLogic011_IsRefuseEmptyCombo(int id);
void TaLogic_ConvEmptyCombo__0013fa90(int side, unsigned short *fusionTree, int *table);
int TaLogic012_IsRefuseEmptyCombo(int id);
void TaLogic_ConvEmptyCombo__00144630(int side, unsigned short *fusionTree, int *table);

/* Seventeen byte-identical 244-byte copies: 1 when the two cells (x1, y1) and
 * (x2, y2) are exactly two steps apart (|dx| + |dy| == 2), the 0x1df540 word
 * for (side, pos) is nonzero, bit 0 of SzDuel_GetUnitStatus(side, pos) (has moved) is clear
 * and CheckLandAgreement2(SzDuel_GetField(x1, y1), side, pos) answers exactly 1; otherwise 0. Six int
 * parameters and int return assumed from this code alone; the 0x1e22b0
 * prototype is repeated from state_grid.h. */
extern int SzDuel_GetField(int x, int y);
int TaLogic003_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic005_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic006_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic007_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic008_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic009_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic010_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic011_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic012_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic013_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic014_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic015_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic018_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic019_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic020_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic021_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);
int TaLogic022_IsOpenMoveUnit(int side, int pos, int x1, int y1, int x2, int y2);

/* 0x112960 and its byte-identical copies: 1 when id is 54..57, else 0. An int
 * parameter and result are assumed. */
int TaLogicCom_IsExzParts__00112960(int id);
int TaLogic017_IsExzParts(int id);
int TaLogicCom_IsExzParts__0016cf10(int id);

/* 0x112980 and its byte-identical copy: 1 when the SzDuel_GetUnitCardNo(side, 0) word is
 * 58 and SzData_CheckDeck(side, SzData_GetCurrentDeck(side), id) is non-negative for each id
 * 54..57 in order, else 0. An int parameter and result are assumed. */
int TaLogic_IsTactixExz__00112980(int side);
int TaLogic_IsTactixExz__0016cf30(int side);

/* The first i in 1..5 for which CheckLandAgreement(SzDuel_GetField(0, i), item) is 1, else -1.
 * The parameter type follows 0x24deb0's second parameter. */
int TaLogic012_GetAgreeLineZ(struct DotrStateItem *item);

/* 0x24ca40: six int arguments as these two callers pass them. */
extern int SearchUnitByRange(int, int, int, int, int, int);
/* 1 when TaLogic_GetGraveCardCount(side, 3, 2), TaLogic_GetGraveCardCount(side, 3, 0x345) or
 * SearchUnitByRange(0, 0, side, 1, 0x2001c, 14) is non-zero, side read from SzDuel_GetTurnOwner()
 * before each; else 0. */
int TaLogic001_IsActiveMode(void);
/* 1 when SearchUnitByRange(0, 0, side, 1, 0x2001c, 14) or the same call with 0x1c is non-zero, else 0. */
int TaLogic001_IsUseLordOfDragon(int side);

/* 0x1566d0: the 0x10e6b0 slot pick without the sum and state tests: a slot's
 * id is marked when it is not 999, TaLogic017_IsRefuseEmptyCombo(id) is 0, its 0x26fa80 cost does
 * not exceed the item's and the 0x2789f0 pair word is 999. 0x156870 is
 * defined in lower_rules with this declaration. */
extern int TaLogic017_IsRefuseEmptyCombo(int id);
void TaLogic_ConvEmptyCombo__001566d0(int side, unsigned short *fusionTree, int *table);

/* 0x250360 and 0x250370 are defined in small_misc with other declarations;
 * these are the views 0x267380 calls (a position pair, and the item from which
 * 0x250370 reads the +0x44 byte and +0x57 bit 1). */
extern int func_00250360_pair(int a, int b);
extern int func_00250370_item(struct DotrStateItem *item);
/* 0 for a null item, for side equal to the item's +0x44 byte (inverted to
 * 0/1 when +0x57 bit 1 is set) or a zero +0x45 byte; 1 when +0x57 bit 2 is
 * clear. Otherwise walks the Ta_SearchUncTypeNaturalEffectUserNum(68) slots
 * Ta_SearchUncTypeNaturalEffectUser(68, i): the first with grid bits 4, 3, 5, 0x250360 equal to
 * Ta_IsUnitControl2(item) and an entry whose low six bits are 29 answers (+0x3e >> 5) == 0, one
 * with 30 answers (+0x3e >> 5) != 0; none answers 0. The item type and int side are assumed. */
int Ta_IsUnitHide(struct DotrStateItem *unit, int side);

/* 0x1d9820's views of the control block at 0x317490: 848-byte side elements with four
 * words at +0x1d0 (converted by 0x1fc950), two bytes at +0x33d/+0x33e (as
 * item_release.h reads them), a signed byte at +0x345 and a flag bit at +0x346; and
 * two flag bits at +0xd66/+0xd67 of the block. Assumed: the first parameter is unused,
 * nothing is returned (v0 is left from the last call), the bits are signed one-bit
 * fields (the copy sign-extends), and the element is handed to TaLogic_AdjustLogicData under
 * that routine's own type. Callees as declared in state_records.h. */
struct DotrQuerySide {
    unsigned char unknown_000[0x1d0];
    int unknown_1d0[4];
    unsigned char unknown_1e0[0x15d];
    unsigned char unknown_33d;
    unsigned char unknown_33e;
    unsigned char unknown_33f[6];
    signed char unknown_345;
    signed char unknown_346_bit_0 : 1;
    signed char unknown_346_bits_1_7 : 7;
    unsigned char unknown_347[9];
};
struct DotrQueryControlEnd {
    unsigned char unknown_000[0xd66];
    signed char unknown_d66_bits_0_6 : 7;
    signed char unknown_d66_bit_7 : 1;
    signed char unknown_d67_bit_0 : 1;
    signed char unknown_d67_bits_1_7 : 7;
};
extern struct DotrQuerySide data_00317490_query_sides[];
extern struct DotrQueryControlEnd data_00317490_query_end;

void SzDuel_InitTurn(int unused, int side);
/* 0x1ba420: the first id below 854 whose 0x1dd0e0 entry has bit 6 of byte +9 set and
 * whose eight bytes at +0xc, each plus the signed byte passwordKeyHannibal[(id + i) % 8]
 * modulo 256, equal the eight key bytes; -1 when none. Assumed: the entry view below
 * (only byte +9 bit 6 and +0xc..+0x13 are seen), the key as unsigned bytes, and the
 * eight signed bytes at 0x2bef79 as a named object (it may be a literal in the
 * original; the code only shows its address kept in a saved register). */
struct DotrCodeEntry {
    unsigned char unknown_00[9];
    unsigned char unknown_09_low : 6;
    unsigned char coded : 1;
    unsigned char unknown_0a[2];
    unsigned char code[8];
};
extern signed char passwordKeyHannibal[];
int lSzDuel_HitPassword(unsigned char *key);
/* 0x267700 / 0x267860: walk the kind-11 list of 0x266df0 / 0x266c70 like 0x267560
 * (three grid bits, entry 2 of the slot) and act only on an entry whose low six
 * bits are 0. There both call SzDuel_GetUnitKind(side, pos) twice: when its low five bits and
 * the rest are both zero, 0x267700 returns 1 and 0x267860 returns the list slot;
 * otherwise 0x267700 returns 0 at once and 0x267860 goes on to the next slot.
 * Both return 0 for pos == 0 or an exhausted list. Assumed: two int arguments, the
 * one-case switch of the source (it reproduces an unreachable move in the original). */
int Ta_IsResistAll(int side, int pos);
int Ta_IsResistAllMaster(int side, int pos);

/* 0x1d91b0 lSzDuel_LockCamera2 (name from the Japanese build's symbol table).
 * Points the current side's camera at the middle of two board cells, turned to the line
 * between them: cell coordinates become (n - 3) * 50, the angle is func_001a6c18(dz, dx)
 * (library block; assumed atan2f) scaled to 65536 per turn and rounded to a multiple of
 * 256, plus a quarter turn. The side's four words at +0x1d0 are set to 150, the angle and
 * 0x4000 only while SzView_IV2V converts them, then restored. Both camera requests use the
 * frame count's low 16 bits. Parameter meanings are assumed from use.
 * Views of this caller (they link by address): the
 * control block at 0x317490 as bytes (848-byte side elements), 0x1fc950 SzView_IV2V with
 * untyped pointers and 0x1fa8a0 with an int count, an int result and an untyped data
 * pointer. Assumed: these types. */
#ifdef __MWERKS__
extern unsigned char data_00317490_query_bytes[];
#else
/* Host: the byte view names the same fixture as data_00317490_query_sides. */
#define data_00317490_query_bytes ((unsigned char *)data_00317490_query_sides)
#endif
extern void func_001fc950_lockcam(void *in, void *out);
extern int func_001fa8a0_lockcam(int owner, int target, int type, int count, int value, void *data);
extern float func_001a6c18(float a, float b);
void lSzDuel_LockCamera2(int arg0, int arg1, int arg2, int arg3, int arg4);

#endif
