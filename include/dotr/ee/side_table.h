#ifndef DOTR_EE_SIDE_TABLE_H
#define DOTR_EE_SIDE_TABLE_H

/* Offset views of the two 0xffa0-byte objects at EE 0x32f790, selected by
 * comparing an argument with the byte at 0x34f6d0. Not recovered declarations
 * or object bounds; most field meanings are unassigned. small_misc.h declares an
 * incompatible view of the same storage under another symbol. The group also holds
 * functions unrelated to these objects, declared further down.
 * Evidence: docs/modules/save-data.md and, for the other functions, their module entries. */
struct DotrSideEntry {
    unsigned unknown_00_bits_0_30 : 31;
    unsigned unknown_00_bit_31 : 1;
    unsigned unknown_04_bits_0_29 : 30;
    unsigned unknown_04_bits_30_31 : 2;
};

struct DotrSideRecord {
    struct DotrSideEntry copies[9];
    unsigned short rank : 14;
    unsigned short unknown_48_bit_14 : 1;
    unsigned short unknown_48_bit_15 : 1;
    unsigned short exp;
};

/* Forty halfwords (999 when vacant) and two trailing halfwords. */
struct DotrSideList {
    unsigned short cards[40];
    unsigned short leader;
    unsigned short deckCost;
};

struct DotrSideBits {
    unsigned char unknown_bits_0_1 : 2;
    unsigned char unknown_bit_2 : 1;
    unsigned char unknown_bit_3 : 1;
    unsigned char unknown_bit_4 : 1;
    unsigned char unknown_bit_5 : 1;
    unsigned char unknown_bits_6_7 : 2;
};

struct DotrSide {
    struct DotrSideRecord cards[854];
    unsigned short newcomers[5];
    unsigned char unknown_fd92[2];
    struct DotrSideList decks[3];
    unsigned char ownedAltArts[16];
    unsigned char treasureFlags[3];
    unsigned char unknown_fea3[5][7][7];
    int unknown_ff98;
    struct DotrSideBits flags;
    unsigned char unknown_ff9d[3];
};

typedef char DotrSideEntryMustBe8[(sizeof(struct DotrSideEntry) == 8) ? 1 : -1];
typedef char DotrSideRecordMustBe4c[(sizeof(struct DotrSideRecord) == 0x4c) ? 1 : -1];
typedef char DotrSideListMustBe54[(sizeof(struct DotrSideList) == 0x54) ? 1 : -1];
typedef char DotrSideMustBeFfa0[(sizeof(struct DotrSide) == 0xffa0) ? 1 : -1];

/* Six-byte rows at EE 0x2d0870: a 14-bit identifier, a two-bit count and two
 * alternative halfwords. */
struct DotrSideAlternative {
    unsigned short unknown_00_bits_0_13 : 14;
    unsigned short unknown_00_bits_14_15 : 2;
    unsigned short altArts[2];
};

/* Four-byte rows at EE 0x2d08d0. The lookup reads 22 rows: an
 * identifier byte, two coordinate nibbles, and a result halfword. */
struct DotrSidePlacement {
    unsigned char duelist;
    unsigned char unknown_01_bits_0_3 : 4;
    unsigned char unknown_01_bits_4_7 : 4;
    unsigned short cardNo;
};

typedef char DotrSidePlacementMustBe4[(sizeof(struct DotrSidePlacement) == 4) ? 1 : -1];

extern struct DotrSide gSzData_All_sides[2];
extern unsigned char gSzData_bRoseFlag[];
extern unsigned short gSzData_awExpTable[];
extern struct DotrSideAlternative aSpecialCard[];
extern struct DotrSidePlacement aTreasureData[];

/* MATCHING DEVICE: the selection and the two address forms are macros; as
 * inline helpers they change register allocation in several callers. */
#define DOTR_SIDE(a) (&gSzData_All_sides[gSzData_bRoseFlag[0] == (a) ? 0 : 1])
#define DOTR_SIDE_LIST(p, k) ((struct DotrSideList *)((char *)(p) + (k) * 84 + 0xfd94))
#define DOTR_SIDE_ENTRIES(p, id) ((struct DotrSideEntry *)((char *)(p) + (id) * 76))

/* The inline form of the selection; three routines need it for one access. */
static inline struct DotrSide *dotr_side_select(int a)
{
    return &gSzData_All_sides[gSzData_bRoseFlag[0] == a ? 0 : 1];
}

#define DOTR_SIDE_MAX(a, b) ((a) > (b) ? (a) : (b))
#define DOTR_SIDE_MIN(a, b) ((a) < (b) ? (a) : (b))

/* 0x214620. Resets the selected side's object: SzData_ClearCardInfo on each of the nine
 * entries of all 854 records, the record's 14-bit value, halfword +0x4a and
 * two top bits cleared; the five halfwords at +0xfd88 and each list's forty
 * values and +0x50 set to 999, each list's +0x52 cleared; the sixteen bytes at
 * +0xfe90 cleared; 22 bytes from +0xfea0 cleared (only three hold the 22 flag
 * bits; the rest are then rewritten); the 5 x 7 x 7 bytes at +0xfea3 set to 7;
 * the word +0xff98 cleared and the bits of +0xff9c set to 0, 1, 1, 1, 0 (bits
 * 0-1, 2, 3, 4, 5; bits 6-7 kept). The side is not bounded: any value other
 * than the byte at 0x34f6d0 selects the second object.
 * Assumed: void result (v0 is not set). */
void SzData_ClearData(int side);
int SzData_SetOption(int side, int which, int flag);
int SzData_GetOption(int side, int which);
struct DotrSideEntry *SzData_CheckBag(int side, int cardNo, int kind, int flag);
struct DotrSideEntry *SzData_GetCardInfo(int side, int cardNo, int position);
int SzData_ClearCardInfo(struct DotrSideEntry *entry);
int SzData_CheckDup(int side, int key, int uniqueId, int cardNo);
int SzData_CheckDeck(int side, int deckNo, int cardNo);
int SzData_PushDeckLeader(int side, int deckNo, int cardNo);
int SzData_PullDeckLeader(int side, int deckNo);
/* 0x2154e0. Answers 0 for a side outside 0..1, an id (671 read as 999)
 * outside 0..853, three or more copies of the id among the list's forty
 * halfwords, no vacant (999) position, no record entry of kind 3 with its
 * flag clear (SzData_CheckBag(side, id, 3, 0)) or no SzDuel_GetCardConstInfo(id) entry. Otherwise
 * adds that entry's byte +3 to the list's +0x52 total, stores the id in the
 * first vacant position, sets the record entry's kind to `deckNo` and answers 1. */
int SzData_PushDeck(int side, int deckNo, int cardNo);
int SzData_PullDeck(int side, int deckNo, int deckSlot);
int SzData_GetCurrentDeck(int side);
int SzData_SetCurrentDeck(int side, int deckNo);
int SzData_GetLeader(int side, int deckNo);
int SzData_GetCardNo(int side, int deckNo, int cardSlot);
int SzData_CalcExp(int side, int cardNo, int operation, int expChange);
int SzData_CalcDeckCost(int side, int deckNo);
int SzData_GetRank(int side, int cardNo);
int SzData_SetRank(int side, int id);
int SzData_GetDeckCost(int side, int list);
int SzData_GetDeckCount(int side, int deckNo);
int SzData_AddBag(int side, int key, int cardNo);
int SzData_AddBag2(int side, int key, int value, int cardNo);
int SzData_DelBag(int side, int cardNo);
int SzData_RollSpecial(int side, int cardNo);
int SzData_ExchSpecial(int side, int cardNo);
int SzData_PushBagNew(int side, int cardNo);
int SzData_IsBagNew(int side, int cardNo);
int SzData_GetBagCountAll(int side);
int SzData_GetBagCount(int side, int cardNo, int kind);
int SzData_GetLookFlag(int side, int id);
int SzData_SetLookFlag(int side, int id, int flag);
int SzData_GetPassFlag(int side, int cardNo);
int SzData_SetPassFlag(int side, int id, int flag);
int SzData_GetTreasureFlag(int side, int bit);
int SzData_SetTreasureFlag(int side, int currentMap, int flag);
int SzData_SearchTreasure(int duelist, int col, int row, int range);
int SzData_GetMapEditFlag(int side);
int SzData_SetMapEditFlag(int side, int flag);
int SzData_SetRankDirect(int side, int cardNo, int rank);

/* Dependency prototypes assumed from the call sites (SzDuel_Xax2New is defined in resource_support).
 * SzDuel_GetCardConstInfo (0x1dd0e0) is defined in state_grid. */
extern int SzDuel_Xax2New(int index);
extern int func_0019a5c8(void);
/* Defined in the sound_gate group. */
extern int SzSound_SetOutput(int flag);

/* The callees are defined in state_grid (state_grid.h declares them with three- and four-int
 * views). Two int arguments assumed. */
void TaEff_RestoreStdRate(int side, int pos);

/* Besides TaEff_RestoreStdRate (this group), the callees are defined in state_grid.
 * Returns whether every non-negative 7x7 cell was counted as handled. */
int Ta_IsFinishFade(void);

/* The callees are defined in this group (small_misc.h keeps the extern used by
 * lSzDuel_TradeDo). Mirror of small_misc.h's DotrMiscControlCounts:
 * two byte pointers at 0x317490 + 0x6e0/+0x6e4, each indexed by 854 ids; side 0
 * reads the first. Meanings assumed. */
struct DotrSideCounts {
    unsigned char unknown_000[0x6e0];
    unsigned char *first;
    unsigned char *second;
};
extern struct DotrSideCounts data_00317490_sidecounts;
void lSzDuel_TradeDoSide(int side);

/* small_misc.h keeps a struct-pointer view of TaHand_GetNextHand for its caller. The callees
 * are defined in state_records and state_queries; `out` is assumed to be an int array. */
extern int SzDuel_InqHand(int side, int position);
extern int SzDuel_GetHandCount(int side);
extern int SzDuel_InqDeck(int side, int position);
extern int TaHand_GetNextHandNum(int side);
void TaHand_GetNextHand(int index, int *out);

/* Views for 0x27f180: an owner with a pointer at +0x78 to 48-byte rows, a
 * signed 12-bit count in the halfword at +0x82 and a word at +0x90; each row
 * has a 24-bit field at bits 32-55 of the 64-bit word at +0x10 and a byte at
 * +0x1b. The `_owner` names are typed views of SzDuel_GetCardNodeOnScreen (0x1da2b0),
 * SzDuel_GetCardNode (0x1d9f00), SzDuel_SetupCard (0x1da7a0) and SzDuel_DrawCard (0x1dbcf0)
 * (all defined in draw_state, the last two with word parameters);
 * prototypes and the two-float point are assumed. Host layouts
 * differ where a pointer is held. */
struct DotrSideFadeRow {
    unsigned char unknown_00[16];
    unsigned long unknown_10 : 32;
    unsigned long color : 24;
    unsigned long unknown_17 : 8;
    unsigned char unknown_18[3];
    unsigned char alpha;
    unsigned char unknown_1c[20];
};
struct DotrSideFadeOwner {
    unsigned char unknown_00[0x78];
    struct DotrSideFadeRow *rows;
    unsigned char unknown_7c[6];
    short count : 12;
    short unknown_82_bits_12_15 : 4;
    unsigned char unknown_84[12];
    int unknown_90;
};
struct DotrSidePoint {
    float x;
    float y;
};
extern int func_001da2b0_owner(struct DotrSideFadeOwner *owner, int a, float x, float y);
extern int func_001d9f00_owner(struct DotrSideFadeOwner *owner, int a);
extern void func_001da7a0_owner(int a, struct DotrSideFadeOwner *owner);
extern void func_001dbcf0_owner(struct DotrSideFadeOwner *owner);
int Ta_LkPrim_DrawCard(struct DotrSideFadeOwner *owner, unsigned char alpha, struct DotrSidePoint *point);

/* The callees are defined in state_grid; the four-int prototype of 0x1e1e70 is assumed from
 * this call site. */
extern int SzDuel_SetUnitStatus(int side, int pos, int bit, int value);
void TaUtil_DoOpen(int side1, int pos2, int mode);

/* SzDuel_DrawStageAfter_Pipe (draw_state.h keeps a word-typed extern for its caller). The 16-byte vector at
 * 0x291120 is copied whole; only its x and z floats are otherwise evidenced. Callee prototypes
 * (0x1f3130 / 0x1f1370 in object_table, 0x1ef8a0 in model_exsort, 0x1dc560 in
 * state_grid) and the int cursor are assumed. */
struct DotrSideVector {
    float x, y, z;
    float unknown_w;
};
extern struct DotrSideVector data_00291120;
extern int SzModel_GetDrawFlag(int index);
extern int SzModel_IsModelDisp(int modelId);
extern int SzModel_ExSort_Pipe(int cursor, int index, struct DotrSideVector *v, int a, int b, int c, int d);
extern int SzDuel_DrawLine_Pipe(int cursor);
int SzDuel_DrawStageAfter_Pipe(int cursor);
/* Assumed from this code alone: 1 when the list holds 40 values and its
   0x50 word is not 999. */
int lSzDuel_IsDeckReady(int side, int deckNo);

/* 0x1de540 is defined in state_records; no argument is passed here. */
extern int SzDuel_GetTurnOwner(void);
/* id unchanged when SzData_GetOption(SzDuel_GetTurnOwner(), 0) is 0. Otherwise nineteen ids map
 * to 3054..3072 in this order: 2551, 3003, 3004, 3005, 2835, 2843, 2849, 2852,
 * 2862, 2863, 2865, 2877, 2882, 2884, 2905, 2910, 2912, 2951, 2954; any other
 * id is returned unchanged. The target switches on id - 2524. state_queries.h
 * declares the same address with a pointer argument for its two callers. */
int TaTuto_GetCardNameMsgID(int id);

/* Ta_GetRecycleRequestLvl: takes the larger SzData_GetRank value of the ids SzData_GetLeader
 * gives for lists 0 and 1 (999 skipped), then adjusts base by func_0019a5c8
 * remainders. Both parameters and the result are assumed to be int. */
int Ta_GetRecycleRequestLvl(int side, int base);

/* 0x214290. Clears both side objects through RhMem_FillMemory (0x1ff40 bytes, declared
 * as in pool_table.h) and then gives each the reset 0x214620 gives the selected one,
 * with the entry reset inline and without its null test, without the store to +0xff98,
 * and with bit 5 of +0xff9c set instead of cleared. Assumed: no arguments, no result. */
extern void RhMem_FillMemory(void *destination, int value, int size);
void SzData_Initialize(void);

/* 0x2514f0. Card constant record; member names and layout from the Japanese build's
 * debug information (tag unnamed there). Another view of the 20-byte records that
 * state_grid.h declares as struct DotrStateEntry20. */
struct jp_8d5b {
    unsigned char bKind;
    unsigned char bKindOfs;
    unsigned char bAttr : 4;
    unsigned char bLevel : 4;
    unsigned char bDeckCost;
    unsigned short wEffIdx;
    unsigned short wEffNo;
    unsigned short wAP : 13;
    unsigned short fPermanent : 1;
    unsigned short fGetPasswd : 1;
    unsigned short fGetRecycle : 1;
    unsigned short wDP : 13;
    unsigned short fGetSlot : 1;
    unsigned short fGetFever : 1;
    unsigned short fAlternate : 1;
    unsigned char abPasswd[8];
};
struct DotrStateEntry20;
struct DotrStateEntry20 *SzDuel_GetCardConstInfo(int cardNo);
/* Eight fixed card numbers map to fixed answers; otherwise a random pick among the
 * records with fGetFever set (the eight answers excluded), 999 when there is none.
 * Prototype and locals from the Japanese build's debug information. */
int TaSlot_GetFeverGain(unsigned short nCardNo);

/* 0x250430. Fills a list with the card numbers other than cNo in 0..682 or 683..853
 * (chosen by a random draw whose odds depend on bKind >> 5 of cNo's record) whose
 * bDeckCost equals the level from Ta_GetRecycleRequestLvl and whose fGetRecycle is set,
 * lowering the level until something is found; returns a random entry, 820 when none.
 * Prototype and locals from the Japanese build's debug information. */
int Ta_GetRecycle(int nSide, int cNo);

/* 0x214990. Fills a side from one of the 51 constant decks at 0x2d0970 (41 halfwords
 * each: the leader, whose bits 12-15 are its rank, then forty cards; bits 0-11 are the
 * card number). A deck outside 0..50 does nothing. Otherwise: SzData_ClearData; the word
 * +0xff98 becomes deck + 0x40000000 for decks 17 and up, and for decks 0..16 the sum of
 * the 13-bit fields of the halfword list starting two entries after index playerNameOffset
 * of 0x2d49d4 (up to and including the first with bit 15), plus func_0019a5c8(), masked
 * to 31 bits and drawn again while zero; all 41 cards go to SzData_AddBag; the five
 * halfwords at +0xfd88 are set to 999; the leader goes to SzData_SetRankDirect and
 * SzData_PushDeckLeader, the forty cards to SzData_PushDeck (list 0); then
 * SzText_Free(0, 1), (0, 0), (1, -1).
 * Assumed: void result, int parameters, element types from the access widths; the
 * table layouts are inferred from this function only. */
extern int playerNameOffset;
extern unsigned short StringBlob[];
extern unsigned short gSzData_ConstDeck[51][41];
extern int SzText_Free(int group, int id);
void SzData_SetupFromConstDeck(int side, int deck);

#endif
