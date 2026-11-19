#ifndef DOTR_EE_GRID_SCANS_H
#define DOTR_EE_GRID_SCANS_H

/* TaLogic helpers (game code below the library block) that walk grid cells, graveyard slots or a
 * short list of fixed spots or ids through state_grid / state_records queries and answer a
 * position, a count or a yes/no.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* Callees, declared with the arguments this group passes; all are also declared
 * in the state_grid, state_records, state_queries, small_misc or position_rules
 * headers, some there under other views. Results are assumed int unless a caller here
 * dereferences or null-tests them. */
struct DotrStateItem;
/* View of the record 0x1df540 answers: only the halfword at +0x3c is read. */
struct DotrScanRecord {
    unsigned char unknown_00[0x3c];
    unsigned short unknown_3c;
};
/* Prefix view of the order record (lower_rules.h and position_rules.h hold the
 * same offsets): position (row high byte, column low byte), a halfword at +4
 * and a word at +8 whose low halfword is also read on its own. */
struct DotrScanOrder {
    unsigned short position;
    unsigned short unknown_02;
    unsigned short unknown_04;
    unsigned short unknown_06;
    union {
        unsigned short half;
        int word;
    } flags;
};
extern struct DotrStateItem *SzDuel_GetMapUnit(int x, int y);
extern int SzDuel_GetMapUnitID(int x, int y);
extern int SzDuel_GetField(int x, int y);
extern int SzDuel_GetUnitSpellCount(int side);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int SzDuel_GetGraveCount(int side);
extern int SzDuel_InqGrave(int side, int position);
extern struct DotrScanRecord *func_001df540_scan(int a, int b);
extern int Ta_GetCurrentSide(int side, int pos);
extern int CheckLandAgreement2(int field, int side, int pos);
extern int func_00250360_pair(int a, int b);
extern int TaLogic020_GetLinePiece(int index);

/* The first of the spots 0x0303, 0x0404, 0x0402, 0x0503 (x in the low byte, y
 * in the high byte) for which SzDuel_GetMapUnit(x, y) answers null, or -1. The spot is
 * chosen by a switch on the loop counter narrowed to a byte, with a default
 * equal to the last case. Assumed: no parameters, int result. */
int TaLogic017_GetEmptyExzPiece(void);

/* -1 when SzDuel_GetUnitSpellCount(side) is 5 or more. Otherwise the first graveyard slot i of
 * side with SzDuel_InqGrave(side, i) == 713 answers side << 16 | i; failing that the first
 * such slot of the other side (1 for side 0, else 0) answers other << 16 | i;
 * else -1. Assumed: int side, int result. */
int TaLogic009_IsUseCopyCat(int side);

/* 1 only when (x, y) and (to_x, to_y) are exactly two steps apart (sum of the
 * two absolute differences), kind is 2, SzDuel_GetUnit(row, column) is not null,
 * SzDuel_GetUnitStatus(row, column, 0) is clear and
 * CheckLandAgreement2(SzDuel_GetField(x, y), row, column) is 1; else 0.
 * Assumed: seven int parameters, int result. */
int TaLogic004_IsOpenMoveUnit(int row, int column, int x, int y, int to_x, int to_y, int kind);

/* Walks x = 0..6 of row y. For each cell where SzDuel_GetMapUnitID(x, y) is not negative,
 * the answer is appended to out when mask bit 0 is set and
 * Ta_IsUnitControl(answer >> 8, answer & 0xff) equals side, and appended (again) when
 * mask bit 1 is set and it differs from side. Returns the count, kept in a
 * byte. Assumed: halfword output, int parameters, int result. */
int TaLogic012_GetLineUnitLst(unsigned short *out, int y, int side, int mask);

/* Nothing when 0x1df540 of the order position is null. Otherwise calls
 * 0x24de70 and 0x1e1090 on the position (results unused), stores 0xcff at +4
 * when the record's +0x3c halfword is 688 and 0x3ff otherwise, then for
 * i = 0..4 takes spot = TaLogic020_GetLinePiece(i) as a halfword and on the first spot whose
 * SzDuel_GetMapUnitID(spot & 0xff, spot >> 8) is the order position or negative puts
 * the spot into the high half of the +8 word. Assumed: no result. */
void TaLogic020_CommandSpellUnit(struct DotrScanOrder *order);

/* More callees. SearchUnitByRange (0x24ca40): six int arguments and an int result are
 * assumed. 0x1dcd70 is declared here with an int block of 24 words, which is the room this
 * group's caller leaves on its stack; its own group's declaration takes a state block. */
extern int TaHand_GetCardKind(int id);
extern void func_001dcd70_item(int *block, int id);
extern int SzDuel_GetTurnOwner(void);
extern int CheckLandAgreement(int field, struct DotrStateItem *unit);
extern int SearchUnitByRange(int, int, int, int, int, int);
extern int TaLogic_GetGraveCardCount(int side, int flags, int id);

/* Fills a local block with SzDuel_InitCardInfo(block, id). 0 when the low byte of
 * TaHand_GetCardKind(id) has any of its top three bits set. Otherwise side =
 * SzDuel_GetTurnOwner(); counts (in a byte) the cells x = 0..6 of row y whose
 * SzDuel_GetMapUnitID(x, y) is not negative and whose 0x250360 side equals it; 0 when the
 * count is not 0; else 1 only when CheckLandAgreement(SzDuel_GetField(0, y), block) is 1. An
 * empty seven-step loop precedes the count (the original has it).
 * Assumed: int parameters and result, the 96-byte local. */
int TaLogic012_IsLineEmpty(int id, int y);

/* For each of the ids 811, 805, 821, 824 in turn: when
 * SearchUnitByRange(0, 0, side, 1, id << 16 | 0x1c, 14) is 0 and TaLogic_GetGraveCardCount(side, 1, id)
 * is 0 too the answer is 0. Otherwise 1. Assumed: int side and result. */
int TaLogic017_IsSetTrap(int side);

/* 0x158800: `j 0x158810` (TaLogic017_IsSetTrap) and a nop, written as a tail call. */
int TaLogic017_IsExzSafe(int side);

/* Views of what 0x1dd0e0 and 0x26fc90 answer (both declared elsewhere with
 * other types): one byte at +3 and one word are all this group reads. */
struct DotrScanEntry {
    unsigned char unknown_00[3];
    unsigned char deckCost;
};
struct DotrScanPair {
    unsigned word;
};
extern struct DotrScanEntry *func_001dd0e0_scan(int id);
extern struct DotrScanPair *func_0026fc90_scan(int id, int b);

/* -1 when SzDuel_GetUnitSpellCount(side) is 5 or more. Otherwise, for each graveyard slot i
 * of side with id = SzDuel_InqGrave(side, i) and TaHand_GetCardKind(id) >> 5 not 0 (not a
 * monster): constInfo = SzDuel_GetCardConstInfo(id), effect = TaHand_GetCardEffect(id, 0);
 * answers side << 16 | i when effect is not null, constInfo IS null, its deckCost is 15 or
 * more, SzDuel_GetUnitSpellCount(side) < 5 and effect's word >> 8 is not 20. The original
 * reads deckCost through constInfo only
 * when it is null (`bnez` skips otherwise), and so does this C. Else -1.
 * Assumed: int side and result, the two views. */
int TaLogic007_IsRevivePlay(int side);

/* 0x24e910 and 0x24ec50 are declared the same way in small_misc.h;
 * 0x1e0290's own declaration takes a DotrStateItem view, the `_try` int view is
 * the one other groups use. */
extern int Ta_GetUnitNumFrom22bitFlagList(unsigned flagList);
extern int Ta_GetUnitOwnerIDFromLstCount(int flagList, int n);
extern int func_001e0290_try(int a, int b, int flag);
/* 0x14aca0. With side = SzDuel_GetTurnOwner(): walks the 0x24e910 entries of the
 * SearchUnitByRange(0, 0, side, 1, 0x0111001c, 14) set, then of the same call with
 * 0x001e001c, and answers 1 at the first entry whose SzDuel_GetUnitAP(hi, lo, 1)
 * answer is below its SzDuel_GetUnitAP(hi, lo, 0) answer. Otherwise 1 when the same
 * call with 0x000b001c is non-zero, else 0. MATCHING DEVICE: the comparison
 * flag local and the separate, ordered locals of the two walks. */
int TaLogic013_IsActiveMode(void);

/* View of the 96-byte block 0x1dcd70 fills, as 0x166ee0 reads it: a pointer
 * to an entry in the first word. 0x1e09e0 is declared in state_grid.h with a
 * DotrStateItem parameter and another result type; only the first word of
 * its answer is read here. */
struct DotrScanHeld {
    struct DotrScanEntry *entry;
    int unknown_04[23];
};
extern struct DotrScanPair *func_001e09e0_scan(struct DotrScanHeld *held, int n);
/* 0x166ee0. -1 when SzDuel_GetUnitSpellCount(side) is 5 or more. Otherwise walks the
 * graveyard slots of side and then of the other side (side == 0 ? 1 : 0): for an id
 * whose 0x26fce0 kind >> 5 is not 0 the block is filled with 0x1dcd70 and the
 * slot is taken when its entry's deckCost is at least the highest so far
 * (starting at 40) and the word of SzDuel_GetUnitConstEffect2(block, 0) >> 8 is not 20. Answers
 * owner << 16 | slot of the last slot taken, or -1.
 * Assumed: int side and result, the views above. */
int TaLogic020_IsUseRevive(int side);
/* 0x153f30: the 0x166ee0 walk over the other side's slots only
 * (side == 0 ? 1 : 0); answers other << 16 | slot of the last slot taken, or
 * -1. Assumed: as 0x166ee0. */
int TaLogic015_IsUseRevive(int side);

/* 0x16c8f0. Callee signatures are assumed views from the call sites; 0x1e02f0 is
 * declared in state_grid.h with a DotrStateItem parameter and is handed the held
 * block through a cast. mode other than 29 or 30 is replaced by 29 or 30 on the
 * parity of 0x19a5c8(). Mode 29: -1 when SzDuel_GetUnitMonsterCount(side) is 5 or more; otherwise
 * TaLogic_GetMaxAPGraveCard(side, sidesToCount) gives a packed word (>> 24 side, >> 16 graveyard slot), and the
 * answer is (>> 24) | ((>> 16) & 0xff) when the id's kind >> 5 is 0 and 0x1e02f0
 * of its block is at least 2500, else -1. Mode 30: the two scans of 0x166ee0,
 * the own side only with sidesToCount bit 0 and the other side only with bit 1. */
struct DotrStateItem;
extern int func_0019a5c8(void);
extern int SzDuel_GetUnitMonsterCount(int side);
extern int TaLogic_GetMaxAPGraveCard(int side, int mask);
extern int SzDuel_GetUnitAP2(struct DotrStateItem *unit, int ignoreBoosts);
int TaLogicCom_IsUseRevive(int side, int sidesToCount, int mode);

#endif
