#ifndef DOTR_EE_PICK_COPIES_H
#define DOTR_EE_PICK_COPIES_H

/* AI rule functions that occur as several byte-identical copies (same callees, same code)
 * and fill an item's five slot bytes from a side's id list; first members are the seven
 * 552-byte copies of 0x1703e0.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */
/* Callees, with the prototypes of their defining groups repeated. */
extern void TaHand_GetNextHand(int index, int *out);
extern int TaHand_GetNextHandNum(int side);
extern int TaHand_IsCardSummon2(int side, int cardNo);
extern int TaHand_GetCardKind(int id);
extern int Ta_GetHandEnchant(int cardNo, int equipNo);
extern int TaEnc_GetEncScore(int cardID);

/* The item as bytes with five slot bytes at +6, and the totals object with
 * signed halfwords at +0x38/+0x3a and an id halfword at +0x3c, are assumed
 * from these functions alone; nothing else of either layout is known. */
struct DotrPickTotals {
    char unknown_00[0x38];
    short total_38;
    short total_3a;
    unsigned short id_3c;
};

/* 0x1703e0 and six byte-identical copies: set item[6..10] to 0xff, read the
 * side's five ids (0x26fd30), return -1 for a null totals object; otherwise
 * item[6] becomes the last slot below the 0x26fe20 count that is neither
 * item[6] nor item[7], whose id equals totals->id_3c and for which
 * TaHand_IsCardSummon2(side, id) is 1. When the 0x26fce0 kind (>> 5) of the id in slot
 * item[6] is 0, every slot that is neither item[6] nor item[7], with
 * Ta_GetHandEnchant(that id, slot id) >= 0, kind 2 and an id other than 795 and 796
 * is appended from item[7] on and TaEnc_GetEncScore(slot id) is added to both totals.
 * Returns 1. */
int TaLogic022_SetSpecialComboQue(int side, unsigned char *task, struct DotrPickTotals *unit);
int TaLogic019_SetSpecialComboQue(int side, unsigned char *item, struct DotrPickTotals *totals);
int TaLogic017_SetSpecialComboQue(int side, unsigned char *item, struct DotrPickTotals *totals);
int TaLogic014_SetSpecialComboQue(int side, unsigned char *item, struct DotrPickTotals *totals);
int TaLogic015_SetSpecialComboQue(int side, unsigned char *item, struct DotrPickTotals *totals);
int TaLogic018_SetSpecialComboQue(int side, unsigned char *item, struct DotrPickTotals *totals);
int TaLogic013_SetSpecialComboQue(int side, unsigned char *item, struct DotrPickTotals *totals);

/* 0x136f30 and five byte-identical copies: the 0x1703e0 code without the two
 * 0x2493a0 additions; the action unit is only tested for null and read
 * for id_3c. */
int TaLogic009_SetSpecialComboQue(int side, unsigned char *minTask, struct DotrPickTotals *actionUnit);
int TaLogic010_SetSpecialComboQue(int side, unsigned char *item, struct DotrPickTotals *totals);
int TaLogic021_SetSpecialComboQue(int side, unsigned char *task, struct DotrPickTotals *unit);
int TaLogic008_SetSpecialComboQue(int side, unsigned char *item, struct DotrPickTotals *totals);
int TaLogic020_SetSpecialComboQue(int side, unsigned char *item, struct DotrPickTotals *totals);
int TaLogic006_SetSpecialComboQue(int side, unsigned char *item, struct DotrPickTotals *totals);

/* 0x1146c0 and thirteen byte-identical copies: read the side's five ids
 * (0x26fd30) and, for the first slot below the 0x26fe20 count whose id equals
 * the low 16 bits of id and for which TaHand_IsCardSummon2(side, id) is 1, store the slot
 * in item[6], pass the block and that id to 0x1dcd70 and return the id's
 * 0x26fce0 kind >> 5; -1 when no slot qualifies. The block is only forwarded;
 * its type is taken from the 0x1dcd70 declaration in state_records.h. */
struct DotrStateBlock;
extern void SzDuel_InitCardInfo(struct DotrStateBlock *unit, int cardId);
int TaLogic001_GetSpecCardPlay(int side, unsigned char *task, struct DotrStateBlock *unit, int cardNo);
/* 0x1147b0: the 0x1146c0 search for ids 823, 821, 824, 828, 825 (Magic Jammer, Anti Raigeki,
 * White Hole, Negate Attack, Royal Decree) in that order; first result >= 0 wins. */
int TaLogic001_GetProtectCardPlay(int side, unsigned char *task, struct DotrStateBlock *unit);
/* 0x155c90: orders the side's slots into item[6..]: first the slots 0x156870 reports 0 for
 * (sorted by 0x155fb0), then the first slot 0x1587e0 reports 1 for and the kind-2 slots paired
 * with it. Callee signatures as declared in state_queries, lower_rules and low_leaves; local
 * sizes from the frame. */
extern int TaLogic017_IsExzParts(int id);
extern int TaLogic017_IsRefuseEmptyCombo(int id);
extern void TaLogic017_SortTempArray(unsigned short *values, int lo, int hi, int key, int *table);
int TaLogic017_PlayExzCombo(int side, unsigned char *item, struct DotrPickTotals *unit);
int TaLogic005_GetSpecialCardPlay(int side, unsigned char *minTask, struct DotrStateBlock *unit, int targetCard);
int TaLogic006_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *unit, int id);
int TaLogic008_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
int TaLogic009_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *unit, int targetCardID);
int TaLogic010_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
int TaLogic013_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int cardId);
int TaLogic014_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
int TaLogic015_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int cardId);
int TaLogic018_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
int TaLogic019_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *unit, int id);
int TaLogic020_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
int TaLogic021_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *unit, int id);
int TaLogic022_GetSpecCardPlay(int side, unsigned char *task, struct DotrStateBlock *unit, int cardNo);

/* 0x12dae0 and two byte-identical copies: the 0x1146c0 walk over a caller's id
 * array (not read through 0x26fd30), asking 0x26fb10 once before the id
 * comparison and once after it. Parameter order and types are assumed. */
int TaLogic007_GetSpecCardPlay(int side, int *ids, struct DotrStateBlock *block, unsigned char *item, int id);
int TaLogic011_GetSpecCardPlay(int side, int *ids, struct DotrStateBlock *block, unsigned char *item, int id);
int TaLogic012_GetSpecCardPlay(int side, int *ids, struct DotrStateBlock *block, unsigned char *item, int id);

/* 0x117bd0 and its copy: 0x254c90 fills three stack words from the second word
 * of the TaHand_GetCardEffect(index, 0) record, 0x254890 answers for them and the first
 * argument; 0 when that answer is negative, else 1. This two-word view of the
 * record is this header's own (state_records.h holds struct DotrStatePair);
 * the callees' signatures are assumed from these two callers. */
struct DotrPickPair {
    int unknown_0;
    int unknown_4;
};
extern struct DotrPickPair *TaHand_GetCardEffect(int cardId, int effectIndex);
extern void Ta_GetCeremSacrifice(int *triple, int id);
extern int Ta_GetSacrificeUnit(int *sacrifices, int side);
int TaLogic001_IsDoCerem(int side, int cardNo);
int TaLogic020_IsDoCerem(int side, int cardNo);

/* 0x138f90 and four copies differing only in a limit (2600, 3000, 2400, 2500,
 * 3000): after the 0x117bd0 steps, 0 for a negative 0x254890 answer; otherwise
 * each of the Ta_GetUnitNumFrom22bitFlagList(answer) entries Ta_GetUnitOwnerIDFromLstCount(answer,
 * i) is split into (entry >> 8, entry & 0xff), 0x1e1090 of that pair is split the other way
 * round, and 0 is returned as soon as 0x2509e0 of the four values reaches the
 * limit; else 1. Prototypes repeated from small_misc.h, state_grid.h and
 * state_queries.h; parameter meanings are not known. */
extern int Ta_GetUnitNumFrom22bitFlagList(unsigned flagList);
extern int Ta_GetUnitOwnerIDFromLstCount(int flagList, int n);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int TaUtil_GetUnitAP(int side, int pos, int col, int row);
int TaLogic009_IsDoCerem(int side, int cardNo);
int TaLogic010_IsDoCerem(int side, int cardNo);
int TaLogic013_IsDoCerem(int side, int cardNo);
int TaLogic014_IsDoCerem(int value, int index);
int TaLogic019_IsDoCerem(int value, int index);
/* Same walk with limit 0: any non-negative score rejects. */
int TaLogic018_IsDoCerem(int side, int cardNo);
/* Limit 3000, and a listed cell whose 0x1e1030 word is 402, 403, 427 or 428
 * rejects before it is scored. */
int TaLogic002_IsDoCerem(int value, int index);
/* Three more copies (limits 2600, 2000, 2500), each the first function of a
 * 696-byte span whose second function is a lower_rules step copy. */
int TaLogic005_IsDoCerem(int side, int cardNo);
int TaLogic006_IsDoCerem(int value, int index);
int TaLogic012_IsDoCerem(int value, int index);

/* 0x112850 and its byte-identical copy: the 0x138f90 walk with the limit taken
 * from the record instead of a constant: the unsigned high halfword of
 * TaHand_GetCardAPDP(record word), read before the 0x254890 answer is tested; the score
 * is compared unsigned. Prototype repeated from state_queries.h. */
extern int TaHand_GetCardAPDP(int id);
int TaLogicCom_IsDoCerem__00112850(int side, int cardNo);
int TaLogicCom_IsDoCerem__0016ce00(int side, int cardNo);

/* 0x15e440 and its byte-identical copy: looks at SzDuel_GetMapUnitID(i, row) for i 1..5,
 * row 3 when the low byte of c is 1, else 4. A zero cell is skipped; a cell
 * equal to (side << 8) | pos answers 1; a cell whose Ta_IsUnitControl(cell >> 8,
 * cell & 0xff) equals Ta_IsUnitControl(side, pos) answers 0; 1 when none does. The
 * two-argument view of 0x250360 is small_misc.h's (its definition
 * takes no parameters); the 0x1e3790 prototype is repeated from state_grid.h. */
extern int SzDuel_GetMapUnitID(int x, int y);
extern int func_00250360_pair(int a, int b);
int TaLogic019_IsEmptyCntrGuard(int side, int pos, unsigned char c);
int TaLogic022_IsEmptyCntrGuard(int side, int pos, unsigned char c);

/* 0x126940 and its byte-identical copy: for n (low byte) 1 the high byte of
 * the halfword at +4 and the high halfword of the word at +8 become all ones;
 * for n 0 the low byte and low halfword do; any other n changes nothing.
 * Only these two fields of the object are known. */
struct DotrPickMask {
    int unknown_0;
    unsigned short half_4;
    unsigned short unknown_6;
    unsigned int goalLoc1;
};
void TaLogic005_InitRogicControl(struct DotrPickMask *task, unsigned char type);
/* 0x11c3e0 and 0x1215e0: the goalLoc1 half of 0x126940 only (same object view
 * assumed). */
void TaLogic003_InitRogicControl(struct DotrPickMask *p, unsigned char n);
void TaLogic004_InitRogicControl(struct DotrPickMask *unitTask, unsigned char n);
void TaLogic006_InitRogicControl(struct DotrPickMask *p, unsigned char n);

/* 0x112a40 and its byte-identical copy: 0 unless the SzDuel_GetUnitCardNo(side, 0) word
 * is 58; otherwise, for k 0..3, the eight Ta_GetSommonPiecePreference(side, i) entries are split
 * into (entry & 0xff, entry >> 8); where 0x1e36e0 of that pair is non-null
 * and 0x1e3790 of it is non-negative, that cell (hi = cell >> 8, lo = cell &
 * 0xff) sets flag k when SzDuel_GetUnitCardNo(hi, lo) == Ta_GetExzodiaPartsCardNo(k),
 * SzDuel_GetUnitCurseRest(hi, lo) and SzDuel_GetUnitSpellRest(hi, lo) are 0 and hi == side. The sum
 * of the four flags is returned. The flags are a four-word stack array that the code never
 * clears: a flag that is not set is read as whatever the stack held.
 * Prototypes repeated from state_grid.h and small_misc.h; 0x24e410's
 * signature is inferred from these calls. */
struct DotrStateItem;
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern int Ta_GetSommonPiecePreference(int a, int direction);
extern struct DotrStateItem *SzDuel_GetMapUnit(int x, int y);
extern int Ta_GetExzodiaPartsCardNo(int limbId);
extern int SzDuel_GetUnitCurseRest(int side, int pos);
extern int SzDuel_GetUnitSpellRest(int side, int pos);
int TaLogic_IsExzodia__00112a40(int side);
int TaLogic_IsExzodia__0016cff0(int side);
int TaLogic017_IsExzodia(int side);
/* The same count at 0x268460; a cell also needs SzDuel_GetUnitStatus bit 4 set and
 * bits 0 and 1 clear. */
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
int Ta_IsExzodia(int side);

/* 0x114d40 and its copies (476 bytes each; they differ only in the slot-pick
 * callee): -1 for a null block; otherwise, for each slot below the 0x26fe20
 * count whose id has a zero 0x26fce0 kind >> 5, a node is built by 0x278a90,
 * replaced by 0x278e40 and scored by 0x278f10. A slot with 0x26fb10 == 1 whose
 * score exceeds the best such score so far has its node passed to the
 * slot-pick callee, its five slot bytes copied to item[6..10], the block and
 * node id passed to 0x1dcd70 and the score halves, less the low 13 bits of two
 * halfwords of the block's first-word target, stored at block +0x38/+0x3a.
 * Returns the highest score of any scored slot, or -1.
 * Assumed from this code alone: the node and block views below, the
 * signatures of 0x278a90 and 0x278f10, and five-entry local
 * arrays (the frame leaves room for more). The slot-pick callees, 0x278e40
 * and 0x278dd0, are defined with other pointer types
 * (state_queries.h, small_misc.h); their prototypes are repeated here in this
 * header's views. */
struct DotrPickNode {
    unsigned short id;
    unsigned char slots[5];
};
struct DotrPickTarget {
    unsigned char unknown_00[8];
    unsigned short apWithFlags; /* AP in the low 13 bits */
    short dpWithFlags;          /* DP in the low 13 bits */
};
struct DotrPickBlock {
    struct DotrPickTarget *target;
    unsigned char unknown_04[0x34];
    short apBoost;
    short dpBoost;
    unsigned short cardNo; /* read by 0x12e1f0 and its copy */
    unsigned char kind;
};
extern struct DotrPickNode *TaLogic_GetFusionTree(int side, struct DotrPickNode *node, unsigned char slot, unsigned short id,
                                          int unknown, short *first, short *second);
extern struct DotrPickNode *TaLogic_GetFusionRes(struct DotrPickNode *node);
extern int TaLogic_GetEnchantQue(int side, struct DotrPickNode *node, int a, int b);
extern void TaLogic_CopyArray(unsigned char *source, unsigned char *destination);
extern void TaLogic_ConvEmptyCombo__00114f20(int side, unsigned short *fusionTree, int *table);
extern void TaLogic_ConvEmptyCombo__00128900(int side, unsigned short *fusionTree, int *table);
extern void TaLogic_ConvEmptyCombo__0012dfc0(int side, unsigned short *fusionTree, int *table);
extern void TaLogic_ConvEmptyCombo__00132ee0(int side, unsigned short *fusionTree, int *table);
extern void TaLogic_ConvEmptyCombo__00136d60(int side, unsigned short *fusionTree, int *table);
extern void TaLogic_ConvEmptyCombo__0013b600(int side, unsigned short *fusionTree, int *table);
extern void TaLogic_ConvEmptyCombo__00144630(int side, unsigned short *fusionTree, int *table);
/* Six more copies (0x148b60, 0x14d3e0, 0x15fe50, 0x164ce0, 0x169000,
 * 0x16fef0). The slot-pick callees 0x164ec0 and 0x1691e0 are given the
 * signature of the other slot-pick callees (assumed). */
extern void TaLogic_ConvEmptyCombo__00148d40(int side, unsigned short *fusionTree, int *table);
extern void TaLogic_ConvEmptyCombo__0014d5c0(int side, unsigned short *fusionTree, int *table);
extern void TaLogic_ConvEmptyCombo__00160030(int side, unsigned short *fusionTree, int *table);
extern void TaLogic_ConvEmptyCombo__00164ec0(int side, unsigned short *fusionTree, int *table);
extern void TaLogic_ConvEmptyCombo__001691e0(int side, unsigned short *fusionTree, int *table);
extern void TaLogic_ConvEmptyCombo__001700d0(int side, unsigned short *fusionTree, int *table);
/* A fourteenth copy that also skips id 342; its slot-pick callee 0x11f150 is
 * defined in state_queries. */
extern void TaLogic_ConvEmptyCombo__0011f150(int side, unsigned short *fusionTree, int *table);
int TaLogic_SetComboQue__0011ef60(int side, unsigned char *minTask, struct DotrPickBlock *unit);
int TaLogic001_SetComboQue(int side, unsigned char *task, struct DotrPickBlock *unit);
int TaLogic006_SetComboQue(int side, unsigned char *item, struct DotrPickBlock *block);
int TaLogic007_SetComboQue(int side, unsigned char *item, struct DotrPickBlock *block);
int TaLogic008_SetComboQue(int side, unsigned char *item, struct DotrPickBlock *block);
int TaLogic009_SetComboQue(int side, unsigned char *minTask, struct DotrPickBlock *targetUnit);
int TaLogic010_SetComboQue(int side, unsigned char *item, struct DotrPickBlock *block);
int TaLogic012_SetComboQue(int side, unsigned char *item, struct DotrPickBlock *block);
int TaLogic013_SetComboQue(int side, unsigned char *item, struct DotrPickBlock *block);
int TaLogic014_SetComboQue(int side, unsigned char *item, struct DotrPickBlock *block);
int TaLogic019_SetComboQue(int side, unsigned char *item, struct DotrPickBlock *block);
int TaLogic020_SetComboQue(int side, unsigned char *item, struct DotrPickBlock *block);
int TaLogic021_SetComboQue(int side, unsigned char *task, struct DotrPickBlock *unit);
int TaLogic022_SetComboQue(int side, unsigned char *task, struct DotrPickBlock *unit);
/* 0x10e320 and 0x11a0d0: the 0x114d40 code with the score taken from 0x279130
 * when Taku_Get_WinCount() is below 3 and from 0x278f10 otherwise. 0x279130's
 * definition takes (int, unsigned short *); the four-argument view here is assumed
 * from these two callers' argument registers. The slot-pick callees 0x10e6b0
 * and 0x11a2f0 are declared as in state_queries.h. */
extern int Taku_Get_WinCount(void);
extern int func_00279130_pick(int side, struct DotrPickNode *node, int a, int b);
extern void TaLogic_ConvEmptyCombo__0010e6b0(int side, unsigned short *fusionTree, int *table);
extern void TaLogic_ConvEmptyCombo__0011a2f0(int side, unsigned short *fusionTree, int *table);
int TaLogic_SetComboQue__0010e320(int side, unsigned char *minTask, struct DotrPickBlock *unit);
int TaLogic_SetComboQue__0011a0d0(int side, unsigned char *minTask, struct DotrPickBlock *unit);
/* 0x114b40 and seventeen byte-identical copies:
 * read the side's five ids (0x26fd30); -1 for a null block. Otherwise set
 * item[6..10] to 0xff and take the first slot below the 0x26fe20 count, in
 * three passes: one whose id equals id (never when id is 999), one whose
 * 0x26fce0 kind >> 5 is neither 0 nor 2, one whose kind >> 5 is not 0. The
 * slot goes to item[6], the block and the slot's id to 0x1dcd70, and 1 is
 * returned; -1 when no pass finds a slot. The block is only tested and
 * forwarded; the three pairs of loop locals are assumed from register use. */
int TaLogic001_GetSpellPlay(int side, unsigned char *task, struct DotrStateBlock *unit, int cardNo);
int TaLogic003_GetSpellPlay(int side, unsigned char *minTask, struct DotrStateBlock *unit, int cardId);
int TaLogic005_GetSpellPlay(int side, unsigned char *minTask, struct DotrStateBlock *unit, int cardId);
int TaLogic006_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
int TaLogic007_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *unit, int targetCardId);
int TaLogic008_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
int TaLogic009_GetSpellPlay(int side, unsigned char *minTask, struct DotrStateBlock *spellUnit, int cardID);
int TaLogic010_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
int TaLogic012_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
int TaLogic013_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
int TaLogic014_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
int TaLogic015_GetSpellPlay(int side, unsigned char *minTask, struct DotrStateBlock *unit, int cardID);
int TaLogic017_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *unit, int cardNo);
int TaLogic018_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
int TaLogic019_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
int TaLogic020_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);
int TaLogic021_GetSpellPlay(int side, unsigned char *task, struct DotrStateBlock *unit, int card);
int TaLogic022_GetSpellPlay(int side, unsigned char *task, struct DotrStateBlock *unit, int cardNo);

/* 0x12e1f0 and its byte-identical copy: item[6..10] = 0xff, then the 0x26fd30
 * ids; a null block answers -1. Each slot below the 0x26fe20 count that is
 * neither item[6] nor item[7], holds the block's halfword at +0x3c and has a
 * 0x26fb10 answer of 1 is written to item[6]. When 0x26fce0 >> 5 of that
 * slot's id is 0, every other slot whose Ta_GetHandEnchant(that id, slot id) is not
 * negative, whose 0x26fce0 >> 5 is 2 and whose id is neither 795 nor 796 is
 * appended from item[7] on (byte counter). Returns the block's byte at +0x3e
 * >> 5. The block view is struct DotrPickBlock's (assumed the same object). */
int TaLogic007_SetSpecialComboQue(int side, unsigned char *item, struct DotrPickBlock *block);
int TaLogic012_SetSpecialComboQue(int side, unsigned char *item, struct DotrPickBlock *block);

/* 0x11ed50 and 0x13f640: the 0x114b40 passes with one more test in the third
 * pass, which also passes over a slot holding the id 799 (0x11ed50) or 800
 * (0x13f640). Same signature assumptions. */
int TaLogic004_GetSpellPlay(int side, unsigned char *minTask, struct DotrStateBlock *unit, int cardId);
int TaLogic011_GetSpellPlay(int side, unsigned char *item, struct DotrStateBlock *block, int id);

/* 0x148630 and its copy TaLogic015_GetSpecialComboPlay (ids 797/272/273 and 799/7/501): read the
 * side's ids (0x26fd30). The last slot holding the first id goes to item[6];
 * the last slot holding the second id with a 0x26fb10 answer of 1 goes to
 * item[7]; -1 when either is missing. Otherwise calls SzDuel_InitCardInfo(block, third id),
 * appends from item[8] every other slot whose id has a non-negative
 * Ta_GetHandEnchant(third id, id), a 0x26fce0 kind >> 5 of 2 and is not 795 or 796, adds
 * the low halfword of TaEnc_GetEncScore(id) to both +0x38 and +0x3a for each, and
 * returns 1. That the third argument is the struct DotrPickBlock object and
 * the five-word id buffer are assumed. */
int TaLogic013_GetSpecialComboPlay(int side, unsigned char *item, struct DotrPickBlock *PUnit);
int TaLogic015_GetSpecialComboPlay(int side, unsigned char *item, struct DotrPickBlock *block);
/* 0x124c90: sets item[6..10] to 0xff and reads the side's ids (0x26fd30);
 * -1 for a null block. With mode 0, item[6] becomes the last slot below the
 * 0x26fe20 count that is neither item[6] nor item[7], holds the block's
 * +0x3c id and has a 0x26fb10 answer of 1. Otherwise the ids are passed to
 * 0x123d70 and item[6], item[7] and item[8] become the last slot whose
 * 0x26fce0 kind (low byte) is 0, 18 and 15 with a 0x26fb10 answer of 1.
 * Returns 0. The count is asked again after every slot. 0x123d70 is declared
 * as in state_queries.h; the byte-wide mode is assumed from its masking. */
extern int TaLogic005_Is2Head_ThunderDragon(int side, int *list);
int TaLogic_SetSpecialComboQue__00124c90(int side, unsigned char *minTask, struct DotrPickBlock *block, unsigned char mode);

/* 0x13fcc0: the mode-0 start of 0x124c90 without the 0x26fb10 test (item[6]
 * becomes the last slot holding the block's +0x3c id); then, when the 0x26fce0
 * kind of that slot's id has >> 5 of zero, every other slot whose id has a
 * non-negative Ta_GetHandEnchant(first id, id) answer and a kind >> 5 of 2 and is not
 * 795 or 796 is appended from item[7]. Returns the block's +0x3e byte >> 5,
 * or -1 for a null block. With no slot holding the id, item[6] stays 0xff and
 * indexes the eight-word list out of range, as the target does. */
int TaLogic011_SetSpecialComboQue(int side, unsigned char *minTask, struct DotrPickBlock *targetUnit);

/* 0x119f30: fills 22 halfwords with 0xffff, passes them to TaUtil_GetUnitLst(side, 2)
 * and reads the side's ids; -1 for a null block (item untouched). Otherwise
 * item[6..10] become 0xff and -1 is returned for a negative TaLogic_GetMaxApUnit(words)
 * answer. The low five bits of SzDuel_GetUnitKind(answer >> 8, answer & 0xff) choose
 * an id: 18 -> 724, 2 -> 721, 14 -> 722, anything else -1. item[6] becomes
 * the last slot below the 0x26fe20 count holding that id, and the id is
 * returned. Prototypes repeated from small_misc.h and state_grid.h; the
 * 24-halfword buffer size is taken from the stack layout. */
extern void TaUtil_GetUnitLst(int sideToCheck, int compMode, unsigned short *units);
extern int TaLogic_GetMaxApUnit(unsigned short *ownerIdArray);
extern int SzDuel_GetUnitKind(int side, int pos);
int TaLogic003_IsSpellPlay(int side, unsigned char *minTask, struct DotrPickBlock *unit);

/* 0x115160: 0x13fcc0 returning 1 instead of the block's +0x3e byte >> 5
 * (the block is read only for its +0x3c id). */
int TaLogic001_SetSpecialComboQue(int side, unsigned char *task, struct DotrPickBlock *unit);

/* 0x15ab10: -1 for a null block, before anything is read. Otherwise item[6]
 * becomes the last slot below the 0x26fe20 count holding the block's +0x3c
 * id when that id's 0x26fce0 kind has >> 5 of zero. A second walk takes each
 * slot whose id has a non-negative Ta_GetHandEnchant(block id, id) answer, a kind >> 5
 * of 2 and is not 795 or 796: while SzDuel_GetUnitAP2(block, 0) plus TaEnc_GetEncScore(id) is
 * below 1400 the low halfword of TaEnc_GetEncScore(id) is added to +0x38 and +0x3a and
 * the slot is appended from item[7]; otherwise the walk ends (the index is
 * set to the count). Returns 1. item[6..10] are not preset here, and the
 * append index is not bounded. 0x1e02f0's definition takes a
 * DotrStateItem; the block-typed view is assumed from this caller. */
extern int func_001e02f0_pick(struct DotrPickBlock *block, int unknown);
int TaLogic018_SetComboQue(int side, unsigned char *item, struct DotrPickBlock *unit);

/* Reads the side's id list (0x26fd30) and walks the TaHand_GetNextHandNum(side) slots
 * twice: the last slot holding id 790 goes to item[6] (-1 when there is
 * none); then the last slot holding id 297 for which TaHand_IsCardSummon2(side, 297) is 1
 * goes to item[7] (-1 when there is none). On success marks the block with
 * SzDuel_InitCardInfo(block, 297) and answers 1. The block is not null-tested.
 * Assumed: the 0x10e320 parameter types, int result. */
int TaLogic019_GetSpecialComboPlay(int side, unsigned char *item, struct DotrPickBlock *block);

/* 0x1de020 is defined in resource_support with this declaration. */
extern int SzDuel_Kind2Type(int kind);
/* Two walks over the TaHand_GetNextHandNum(side) slots of a caller-supplied id list. First
 * walk: the first id with SzDuel_Kind2Type(TaHand_GetCardKind(id) >> 5) == 2. Second walk: the
 * first id with TaHand_IsCardSummon2(side, id) == 1 that is 795, 684 or 311. A hit stores
 * its slot at item[6], marks the block with SzDuel_InitCardInfo(block, id) and answers
 * TaHand_GetCardKind(id) >> 5; no hit answers -1.
 * Assumed: int id list, the 0x10e320 block and item types, int result. */
int TaLogic007_GetCurseCardPlay(int side, int *ids, struct DotrPickBlock *unit, unsigned char *minTask);

/* 0x26fb60 is defined in state_queries with this declaration. */
extern int TaHand_IsCardSummon(int index, int value);
/* Reads the side's id list (0x26fd30) and walks the TaHand_GetNextHandNum(side) slots
 * twice. First walk: every slot holding id 800 is stored at item[7] and marked
 * with SzDuel_InitCardInfo(block, 800); without one the answer is -1. Second walk: the
 * first id whose 0x26fce0 kind has >> 5 of zero, a non-negative
 * Ta_GetHandEnchant(id, 800) answer, a TaHand_IsCardSummon(side, id) answer of 1 and is not 795 or
 * 796 is stored at item[7], marked and answered as its kind >> 5; -1 without
 * one. The block is not null-tested.
 * Assumed: the 0x15fa00 parameter types, int result, an eight-word id list
 * (from the stack layout). */
int TaLogic011_GetSpecComboPlay(int side, unsigned char *minTask, struct DotrPickBlock *targetUnit);

/* 0x11eb10. Reads the side's id list, then by the mode byte. 0: the last
 * slot holding id 799 goes to item[6]; with one, the first id whose 0x26fce0
 * kind has >> 5 of zero, a non-negative Ta_GetHandEnchant(id, 799) answer, a
 * TaHand_IsCardSummon2(side, id) answer of 1 and is not 795 or 796 goes to item[7], is
 * marked with SzDuel_InitCardInfo(block, id) and answers its kind >> 5. 1: the first
 * slot holding id 514 with a TaHand_IsCardSummon2(side, 514) answer of 1 goes to item[6],
 * is marked and answers its kind >> 5. -1 otherwise and for any other mode.
 * Assumed: the 0x124c90 parameter types, int result, an eight-word id list.
 * MATCHING DEVICE: the mode is tested with a `switch`. */
int TaLogic004_GetSpecialComboPlay(int side, unsigned char *task, struct DotrPickBlock *unit, unsigned char mode);

/* 0x10f290. Reads the side's id list; -1 for a non-zero mode byte, when
 * SzDuel_GetMapUnit(0, 0) is non-null or when SzDuel_GetUnitLoc(side, 0) is not 1. Otherwise
 * the last slot holding id 798 goes to item[6]; with one, the first id whose
 * 0x26fce0 kind has >> 5 of zero, a non-negative Ta_GetHandEnchant(id, 798) answer and
 * a TaHand_IsCardSummon(side, id) answer of 1 goes to item[7], is marked with
 * SzDuel_InitCardInfo(block, id) and answers its kind >> 5; -1 otherwise.
 * Assumed: as 0x11eb10. MATCHING DEVICE: a one-case `switch` on the mode (a plain early
 * return fills one more delay slot). */
int TaLogic002_GetSpecialComboPlay(int side, unsigned char *minTask, struct DotrPickBlock *unit, unsigned char mode);

/* 0x11f3c0. -1 for a null block. Otherwise presets item[6..10] to 0xff,
 * reads the id list and, for mode 0 or 1, calls TaLogic004_GetSpecialComboPlay(side, item, block, 0)
 * (its answer is not used). Mode 0 then stores in item[8] the last slot that
 * is neither item[6] nor item[7], has Ta_GetHandEnchant(488, id) >= 0 and
 * TaHand_GetCardKind(id) >> 5 == 2 and is not 795 or 796. Answers 0.
 * Assumed: the parameter types of 0x11eb10 and an eight-word id list. */
int TaLogic_SetSpecialComboQue__0011f3c0(int side, unsigned char *item, struct DotrPickBlock *unit, unsigned char mode);

/* 0x10e540. Same opening; only mode 0 goes on: it calls
 * TaLogic002_GetSpecialComboPlay(side, item, block, 0) and, when Taku_Get_WinCount() is above 3, stores each
 * slot passing the same tests (with 427 for 488) in item[8], item[9], ... in
 * turn. Nothing but the list count bounds that index. Answers 0. */
int TaLogic_SetSpecialComboQue__0010e540(int side, unsigned char *item, struct DotrPickBlock *block, unsigned char mode);

/* 0x124810: the 0x114d40 walk with another scorer. A built node whose id is
 * 535 is skipped; otherwise the score is the 0x26fbb0 word of the node id,
 * replaced by the 0x278f10 answer when its high half is 1500 or more. The
 * slot callee 0x124a30 is defined in state_queries. */
extern void TaLogic_ConvEmptyCombo__00124a30(int side, unsigned short *fusionTree, int *table);
int TaLogic005_SetComboQue(int side, unsigned char *minTask, struct DotrPickBlock *unit);

/* 0x13f850: the 0x114d40 walk asking 0x23c160 twice: below 3 the node is
 * built by 0x278c80 instead of 0x278a90, and the slot callee 0x13fa90 runs
 * only at 4 or more. 0x278c80 is defined in small_misc with
 * another record type; the `_pick` view gives it this family's node type
 * (assumed from the shared uses of the result). 0x13fa90 is defined in
 * state_queries. */
extern struct DotrPickNode *func_00278c80_pick(int side, struct DotrPickNode *node, unsigned char slot, unsigned short id, int unknown, short *first, short *second);
extern void TaLogic_ConvEmptyCombo__0013fa90(int side, unsigned short *fusionTree, int *table);
int TaLogic011_SetComboQue(int side, unsigned char *item, struct DotrPickBlock *block);

/* 0x1564e0: a 0x114d40 variant that asks 0x26fb10 before building the node
 * (slots not answering 1 are skipped unscored), compares and returns the low
 * 16 bits of the 0x278f10 score, yet keeps the whole score as the best. The
 * slot callee 0x1566d0 is defined in state_queries. */
extern void TaLogic_ConvEmptyCombo__001566d0(int side, unsigned short *fusionTree, int *table);
int TaLogic017_SetComboQue(int side, unsigned char *item, struct DotrPickBlock *block);

/* 0x135000: same source shape as TaLogic001_IsDoCerem (three-word record on the stack; types
 * assumed as there). */
int TaLogic008_IsDoCerem(int side, int cardNo);

/* TaLogic015_SetComboQue is the 0x16fef0 shape with the 0x26fb10 test moved in front of the node
 * building: a slot that is not affordable is neither built nor scored, so the result is the
 * best affordable score. The pick callee is TaLogic_ConvEmptyCombo__00151880 (defined in state_queries; the
 * signature here is the family's, assumed) and runs after the block stores. */
extern void TaLogic_ConvEmptyCombo__00151880(int side, unsigned short *fusionTree, int *table);
int TaLogic015_SetComboQue(int side, unsigned char *item, struct DotrPickBlock *block);

/* 0x10f450: a four-pass variant of 0x16fcf0 without the id argument. Pass 1 takes the first id whose
 * 0x26fce0 kind (>> 5) is 5 and for which TaLogic002_IsDoCerem(side, id) is 1; pass 2 the first of
 * kind 3 or 4; pass 3 the first kind that is neither 0 nor 2; pass 4 the first kind other than 0.
 * Assumed: three arguments (a3 is not read); the block type is the group's view. */
int TaLogic002_GetSpellPlay(int side, unsigned char *minTask, struct DotrStateBlock *unit);

/* Over the eight Ta_GetSommonPiecePreference(side, i) positions: the cell 0x1e3790 gives for a
 * position (when not negative) is kept when SzDuel_GetUnitCardNo(high, low) equals
 * Ta_GetExzodiaPartsCardNo(limbId) and its SzDuel_GetUnitSummonOrder(high, low) score is strictly below the best
 * so far (starting at 255). Returns the kept cell or -1. Assumed: two int
 * arguments; 0x24e410's signature is inferred from these calls. */
extern int SzDuel_GetUnitSummonOrder(int side, int pos);
int Ta_GetExzodiaPartsUnit(int side, int limbId);

/* 0x1307e0: the 0x162490 walk (limit 3000) that also fails on a cell whose
 * SzDuel_GetUnitCardNo word is 298 or 326, or 305 with SzDuel_GetUnitStatus bit 4 clear.
 * Argument meanings are assumed from 0x162490. */
int TaLogic007_IsDoCerem(int value, int index);

/* TaLogic005_GetSpecialSpellPlay (name, as for the others here, from the Japanese build's
 * symbols): picks a spell from the TaHand_GetNextHand id list in a fixed order of card numbers
 * (829; 844 if TaLogic005_IsDoCerem; 818; 742 with three attribute-4 units in slots 0-5; 796 with a
 * unit whose curse rest is 255; 820 with card 829 in slots 6-10), stores its hand index in item[6],
 * fills block and returns its kind >> 5, or -1. Parameter types are assumed from
 * TaLogic001_GetProtectCardPlay; slot and card meanings are not known. */
extern int SzDuel_GetUnitAttr(int side, int pos);
int TaLogic005_GetSpecialSpellPlay(int side, unsigned char *minTask, struct DotrStateBlock *unit);

#endif
