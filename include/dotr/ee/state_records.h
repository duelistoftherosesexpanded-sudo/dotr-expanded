#ifndef DOTR_EE_STATE_RECORDS_H
#define DOTR_EE_STATE_RECORDS_H

/* Offset/stride views, not recovered original declarations or object bounds.
 * Record and control symbols alias EE 0x317490; standalone byte symbols below
 * also overlap the control view. Evidence: docs/modules/duel.md.
 * Bitfield representation is compiler-dependent and checked on target and host.
 */
struct DotrStateRecordBits {
    signed char unknown_bit_0 : 1;
    unsigned char unknown_bits_1_6 : 6;
    signed char unknown_bit_7 : 1;
};

struct DotrStateRecordList {
    unsigned short unknown_values[40];
    unsigned char unknown_count;
};

struct DotrStateRecordList120 {
    unsigned short unknown_values[120];
    unsigned char unknown_count;
};

/* Each halfword holds a 12-bit value (999 when vacant) and a 4-bit level. */
struct DotrStateRecordList5 {
    unsigned short unknown_values[5];
    unsigned char unknown_count;
};

/* 96-byte local block whose size gives the 0x1df580 frame; contents unknown. */
struct DotrStateBlock {
    int unknown_words[24];
};

/* One 92-byte entry of the 3 x 11 table at EE 0x318280 (see
 * docs/modules/duel.md). Only the named offsets are observed. */
struct DotrStateItemBits {
    signed char unknown_bit_0 : 1;
    signed char unknown_bit_1 : 1;
    signed char unknown_bit_2 : 1;
    signed char unknown_bit_3 : 1;
    signed char unknown_bit_4 : 1;
    signed char unknown_bit_5 : 1;
    signed char unknown_bit_6 : 1;
    signed char unknown_bit_7 : 1;
};

/* Three two-bit fields at item +0x56; observed bits 0, 1 and 3 at +0x58. */
struct DotrStateItemPairs {
    signed char unknown_bits_0_1 : 2;
    signed char unknown_bits_2_3 : 2;
    signed char unknown_bits_4_5 : 2;
    signed char unknown_bits_6_7 : 2;
};

struct DotrStateItemFlags {
    signed char unknown_bit_0 : 1;
    signed char unknown_bit_1 : 1;
    signed char unknown_bit_2 : 1;
    signed char unknown_bit_3 : 1;
    signed char unknown_bits_4_7 : 4;
};

struct DotrStateItemObject;

/* Prefix view of the object addressed by the first word of an item. */
struct DotrStateItemSource {
    unsigned char kind;
    unsigned char kindOfs;
    unsigned char unknown_02_low : 4;
    unsigned char unknown_02_high : 4;
    unsigned char deckCost;
    unsigned short effectId;
    unsigned char unknown_06[2];
    unsigned short apWithFlags;
    unsigned short dpWithFlags;
};

struct DotrStateEntry8 {
    int unknown_words[2];
};

struct DotrStateItem {
    struct DotrStateItemSource *constInfo;
    struct DotrStateItemObject *cardModel; /* defined in state_grid.h */
    struct DotrStateEntry8 cardEffects[5];
    int unknown_30;
    unsigned char unknown_34[4];
    short apBoost;
    short dpBoost;
    unsigned short cardNo;
    unsigned char kind;
    unsigned char attr;
    unsigned char locCol;
    unsigned char locRow;
    unsigned char unknown_42[2];
    unsigned char team;
    unsigned char slot;
    unsigned char modelId;
    unsigned char summonOrder;
    unsigned char lvRev;
    unsigned char curseRest;
    unsigned char spellRest;
    unsigned char delayCount;
    unsigned char unknown_4c[2];
    unsigned char ctlModeC;
    unsigned char unknown_4f;
    unsigned char ctlGoalC;
    unsigned char ctlGoalM;
    unsigned char ctlRateC;
    unsigned char ctlRateM;
    unsigned char openRate;
    unsigned char unknown_55;
    struct DotrStateItemPairs unknown_56;
    struct DotrStateItemBits status;
    struct DotrStateItemFlags modelStatusFlags;
    unsigned char unknown_59[3];
};

/* The leading pointer makes host offsets differ; the target size is checked
 * only by the target compiler. */
#ifdef __MWERKS__
typedef char DotrStateItemMustBe5c[(sizeof(struct DotrStateItem) == 0x5c) ? 1 : -1];
#endif

struct DotrStateVector {
    float x, y, z;
    float unknown_w;
};

struct DotrStateRecord {
    unsigned char unknown_000[0x1e0];
    struct DotrStateRecordList unknown_1e0;
    struct DotrStateRecordList120 unknown_232;
    struct DotrStateRecordList5 unknown_324;
    unsigned char unknown_330[10];
    unsigned short unknown_33a;
    unsigned char unknown_33c;
    unsigned char unknown_33d[4];
    unsigned char unknown_341;
    unsigned char unknown_342;
    unsigned char unknown_343;
    unsigned char unknown_344;
    unsigned char unknown_345;
    struct DotrStateRecordBits unknown_346;
    unsigned char unknown_347[9];
};

struct DotrStateControlBits {
    signed char unknown_bit_0 : 1;
    signed char unknown_bit_1 : 1;
    unsigned char unknown_bits_2_4 : 3;
    signed char unknown_bit_5 : 1;
    unsigned char unknown_bits_6_7 : 2;
};

struct DotrStateControl {
    unsigned char unknown_000[0xc9a];
    short unknown_c9a;
    short unknown_c9c;
    unsigned char unknown_c9e[4];
    short unknown_ca2;
    short unknown_ca4;
    short unknown_ca6;
    short unknown_ca8;
    short unknown_caa;
    unsigned char unknown_cac[2];
    short unknown_cae;
    short unknown_cb0;
    unsigned char unknown_cb2;
    unsigned char unknown_cb3;
    unsigned char unknown_cb4;
    unsigned char unknown_cb5;
    unsigned char unknown_cb6[0x7f];
    unsigned char unknown_d35;
    unsigned char unknown_d36;
    unsigned char unknown_d37[5];
    unsigned char unknown_d3c; /* read as a side index by lSzDuel_IsUnitHide */
    unsigned char unknown_d3d[0x13];
    unsigned char unknown_d50;
    unsigned char unknown_d51[2];
    unsigned char unknown_d53;
    unsigned char unknown_d54;
    unsigned char unknown_d55[0x11];
    struct DotrStateRecordBits unknown_d66;
    struct DotrStateControlBits unknown_d67;
};

typedef char DotrStateRecordBitsMustBeOneByte[
    (sizeof(struct DotrStateRecordBits) == 1) ? 1 : -1];
typedef char DotrStateControlBitsMustBeOneByte[
    (sizeof(struct DotrStateControlBits) == 1) ? 1 : -1];
typedef char DotrStateRecordStrideMustBe350[
    (sizeof(struct DotrStateRecord) == 0x350) ? 1 : -1];
typedef char DotrStateControlPrefixMustBeD68[
    (sizeof(struct DotrStateControl) == 0xd68) ? 1 : -1];

extern struct DotrStateRecord data_00317490_records[];
extern struct DotrStateControl data_00317490_control[];
extern unsigned char turnFirst[];
extern unsigned char turnCount[];
extern unsigned char turnOwner[];
extern struct DotrStateRecordBits data_003181f6[];

int SzDuel_GetPlayer(int side);
void SzDuel_SetPlayer(int side, int playerId);
int SzDuel_GetTurnOwner(void);
int SzDuel_GetTurnFirst(void);
int SzDuel_GetTurnCount(void);
void SzDuel_SetTurnCount(int value);
int SzDuel_GetSummonPower(int side);
int SzDuel_CalcSummonPower(int side, int operation, int amount);
int SzDuel_GetSummonCharge(int side);
void SzDuel_SetSummonCharge(int side, int value);
int SzDuel_InqHandPhase(int side);
void SzDuel_SetHandPhase(int side, int value);
int SzDuel_InqDestinyFlag(int side);
void SzDuel_SetDestinyFlag(int side, int value);
int SzDuel_GetWindowDisp(void);
void SzDuel_SetWindowDisp(int value);
void SzDuel_SetWindowDraw(int value);
void SzDuel_SetWindowBusy(int value);
int SzDuel_SetPlayerCur(int side, int x, int z);
void SzDuel_InitDeck(int side);
int SzDuel_GetDeckCount(int side);
int SzDuel_InqDeck(int side, int position);
int SzDuel_AddDeck(int side, int cardNo);
void SzDuel_PackDeck(int side);
int SzDuel_SetDeck(int side, int position, int cardNo);
int SzDuel_DelDeck(int side, int position);
int SzDuel_DrawDeck(int side);
void SzDuel_InitGrave(int side);
int SzDuel_InqGrave(int side, int position);
int SzDuel_GetGraveCount(int side);
int SzDuel_AddGrave(int side, int cardNo);
void SzDuel_PackGrave(int side);
int SzDuel_DelGrave(int side, int position);
void SzDuel_InitHand(int side);
int SzDuel_InqHand(int side, int position);
int SzDuel_GetHandCount(int side);
int SzDuel_AddHand(int side, int cardId);
void SzDuel_PackHand(int side);
int SzDuel_DelHand(int index, int position);
/* 0x175e60: no parameters observed; the result of the last SzDuel_DelHand call is
 * left in v0 (int return assumed). */
int TaTuto_DbgInit034(void);
/* 0x178320: one 16-bit selector assumed from the `andi 0xffff`; no result observed. */
void TaTuto_SetDeck035(unsigned short n);
int SzDuel_ClearHandTag(int side);
int SzDuel_SetHandTag(int side, int position, int level);
int SzDuel_GetLP(int side);
int SzDuel_CalcLP(int side, int operation, int amount);
struct DotrStateItem *SzDuel_GetUnit(int side, int pos);
void SzDuel_AddUnit(int side, int col, int row, int cardNo, int e);

/* Dependency signatures inferred from the call sites. The update initializes only x/y/z of its
 * outgoing vector; unknown_w is left unset by that call. */
extern int func_0019a5c8(void);
extern int func_001a22c0(float value);
extern void SzView_IV2V(void *source, struct DotrStateVector *saved);
extern void SzView_SetCP(int unknown, struct DotrStateVector *position);
extern void SzView_SetLookSP(int unknown, struct DotrStateVector *saved);
struct DotrStateItem *SzDuel_GetCardInfo(int side, int pos);
extern void SzDuel_InitCardInfo(struct DotrStateBlock *unit, int cardId);
extern int SzDuel_AddUnit2(int a, int b, int c, struct DotrStateBlock *block, int e);

/* Two arguments (record word, index) as read by the callee at 0x26fa80; small_misc.h
 * declares one, so do not include both. */
extern int TaHand_GetCardSummonCost(int cardNo, int index);
int Ta_GetSummonCost(void);
/* View of the 96-byte block as two words and eleven 8-byte elements; only the
 * element address is formed, the element contents are not read. */
struct DotrStatePair {
    int unknown_0;
    int unknown_4;
};
struct DotrStatePairBlock {
    int unknown_words[2];
    struct DotrStatePair pairs[11];
};
struct DotrStatePair *TaHand_GetCardEffect(int cardId, int effectIndex);
/* Views for lSzDuel_LockCamera: the fourth argument is narrowed to 16 bits at
 * each call, the record's words at +0x1d0 and its signed byte at +0x345. */
extern void func_001fa8a0_vec(int a, int b, int kind, unsigned short d, int e, float *data);
extern void func_001fc950_words(int *in, float *out);
struct DotrStateRecordView {
    unsigned char unknown_000[0x1d0];
    int unknown_1d0[4];
    unsigned char unknown_1e0[0x165];
    signed char unknown_345;
    unsigned char unknown_346[10];
};
extern struct DotrStateRecordView data_00317490_recview[];
/* Defined in the side_table group. */
void lSzDuel_LockCamera(int b, int x, int y);

/* SzDuel_InitMapEdit: bytes at 0xa9c08c-0xa9c092 as stored (bit 0 of the last is
 * cleared through a read-modify-write, so a bitfield is assumed), and the two
 * 16-byte objects at 0x2914b0/0x2914c0 it copies to the stack with lq/sq
 * (named objects here; they may be compiler-made initialiser templates).
 * The two callees are defined in object_table and state_grid. */
struct DotrStateSetupBytes {
    unsigned char unknown_0, unknown_1, unknown_2, unknown_3, unknown_4, unknown_5;
    unsigned char unknown_6_bit0 : 1;
};
extern struct DotrStateSetupBytes data_00a9c08c;
extern struct DotrStateVector data_002914b0;
extern struct DotrStateVector data_002914c0;
extern int SzModel_SetDrawFlag(int index, int value);
extern void SzDuel_InitMapColor2(int a, int b, int c, int d);
void SzDuel_InitMapEdit(int flag);

#endif
