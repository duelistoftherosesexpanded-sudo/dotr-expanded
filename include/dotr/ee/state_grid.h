#ifndef DOTR_EE_STATE_GRID_H
#define DOTR_EE_STATE_GRID_H

#include "dotr/ee/state_records.h"

/* Offset/stride views, not recovered original declarations or object bounds.
 * Evidence: docs/modules/duel.md. */
struct DotrStateEntry20 {
    int unknown_words[5];
};

struct DotrStateCellQuad {
    unsigned char unknown_0;
    unsigned char unknown_1;
    unsigned char unknown_2;
    unsigned char unknown_3;
};

/* The first cell word is compared with an item address by 0x1e09e0. It is a
 * 32-bit word on the target; the host uses a pointer-sized integer. */
#ifdef __MWERKS__
typedef int DotrStateWord;
#else
#include <stdint.h>
typedef intptr_t DotrStateWord;
#endif

struct DotrStateCell {
    DotrStateWord unit;
    struct DotrStateCellQuad unknown_04[4];
    short fieldRate;
    unsigned char field;
    unsigned char mapMovable;
    unsigned char targetSelectionHighlight;
    unsigned char selectionHighlightType;
    unsigned char mapAlpha;
};

#ifdef __MWERKS__
typedef char DotrStateCellMustBe1c[(sizeof(struct DotrStateCell) == 0x1c) ? 1 : -1];
#endif
typedef char DotrStateEntry20MustBe14[(sizeof(struct DotrStateEntry20) == 0x14) ? 1 : -1];

/* Prefix view of the object addressed by item word +4. */
struct DotrStateItemObject {
    struct DotrStateVector unknown_00;
    struct DotrStateVector unknown_10;
    unsigned char unknown_20[0x10];
    struct DotrStateVector unknown_30;
    struct DotrStateCellQuad unknown_40;
};

/* Three words at EE 0x31ab88. */
struct DotrStateTriple {
    int unknown_words[2];
    int unknown_08;
};

extern int gSzBattle_bl[];

/* Sixteen zero bytes at EE 0x2921c0, copied by 0x1e5260 with one 128-bit
 * load/store. A zero-initialized local array produces the same instructions
 * from the compiler's own initializer image. */
#ifdef __MWERKS__
typedef __int128 DotrStateWord128;
#else
__extension__ typedef __int128 DotrStateWord128;
#endif

union DotrStateBlock16 {
    DotrStateWord128 unknown_quad;
    int unknown_words[4];
};

extern union DotrStateBlock16 data_002921c0[];
extern struct DotrStateEntry20 gSzDuel_CardConstInfo[];
extern struct DotrStateItem gSzDuel_aaVarInfo[3][11];
extern struct DotrStateCell gSzDuel_aaMapInfo[7][7];
extern struct DotrStateEntry8 EffIdx_L[][5];
extern struct DotrStateEntry8 EffIdx_M[][5];
extern struct DotrStateEntry8 EffIdx_S[];

void SzDuel_InitMapInfo(struct DotrStateCell *cell);
struct DotrStateCell *SzDuel_GetMapInfo(int x, int y);
struct DotrStateEntry20 *SzDuel_GetCardConstInfo(int cardNo);
void SzDuel_DoUnitRequest(void);
void SzDuel_SetupDuel(void);
int SzDuel_PutUnit(int a, int b, int c);
int SzDuel_TrimUnitDir(int a, int b, int c, int d);
int SzDuel_TrimUnitDir1(int a, int b);

int SzDuel_GetUnitCount(int side);
int SzDuel_GetUnitMonsterCount(int side);
int SzDuel_GetUnitSpellCount(int side);
int SzDuel_GetUnitAP(int side, int pos, int ignoreBoosts);
int SzDuel_GetUnitAP2(struct DotrStateItem *unit, int ignoreBoosts);
int SzDuel_GetUnitDP(int side, int pos, int ignoreBoosts);
int SzDuel_GetUnitDP2(struct DotrStateItem *unit, int ignoreBoosts);

int SzDuel_CalcUnitAP(int side, int pos, int operation, int value);
int SzDuel_CalcUnitAP2(struct DotrStateItem *unit, int operation, int value);
int SzDuel_CalcUnitDP(int side, int pos, int operation, int value);
int SzDuel_CalcUnitDP2(struct DotrStateItem *item, int operation, int value);
int SzDuel_GetUnitConstKind(int side, int pos);
int SzDuel_GetUnitKind(int side, int pos);
int SzDuel_SetUnitKind(int side, int pos, int value);
int SzDuel_GetUnitKindOfs(int side, int pos);
int SzDuel_GetUnitAttr(int side, int pos);
int SzDuel_SetUnitAttr(int side, int pos, int value);
int SzDuel_GetUnitLevel(int side, int pos, int ignoreBoosts);
int SzDuel_GetUnitLevel2(struct DotrStateItem *unit, int ignoreBoosts);
struct DotrStateEntry8 *SzDuel_GetUnitConstEffect2(struct DotrStateItem *unit, int eff);
struct DotrStateEntry8 *SzDuel_GetUnitEffect(int side, int pos, int effInd);
int SzDuel_GetUnitLeaderAbility(int side, int pos, int abil, int extraDataOut);
int SzDuel_GetUnitLeaderAbility2(struct DotrStateItem *unit, int abil, int extraDataOut);

int SzDuel_GetUnitCardNo(int side, int pos);
int SzDuel_GetUnitLoc(int side, int pos);
void SzDuel_CalcUnitPos(int x, int y, int z, struct DotrStateVector *v);
int SzDuel_GetUnitModelID(int side, int pos);
int SzDuel_GetUnitSummonOrder(int side, int pos);
int SzDuel_SetUnitCurseFlag(int a, int b, int flag);
int SzDuel_GetUnitCurseFlag(int a, int b);
int SzDuel_SetUnitCurseRest(int side, int pos, int value);
int SzDuel_GetUnitCurseRest(int side, int pos);
int SzDuel_SetUnitSpellRest(int side, int pos, int value);
int SzDuel_GetUnitSpellRest(int side, int pos);
int SzDuel_SetUnitDelayCount(int side, int pos, int value);
int SzDuel_GetUnitDelayCount(int side, int pos);
int SzDuel_CalcUnitMovable(int side, int pos);
int SzDuel_IsUnitMovableOnLabyrinth(int side, int pos);
int SzDuel_SetUnitCtlModeC(int a, int b, int value);
int SzDuel_SetUnitCtlModeM(int side, int pos, int value);
int SzDuel_SetUnitCtlGoal(int a, int b, int value);
int SzDuel_SetUnitCtlGoalC(int side, int pos, int value);
int SzDuel_SetUnitCtlGoalM(int side, int pos, int value);
int SzDuel_GetUnitCtlGoalC(int a, int b);
int SzDuel_GetUnitCtlGoalM(int a, int b);
int SzDuel_SetUnitCtlRate(int a, int b, int pair, int value);
int SzDuel_SetUnitCtlRateC(int a, int b, int pair, int value);
int SzDuel_SetUnitCtlRateM(int side, int pos, int pair, int value);
int SzDuel_GetUnitCtlRateC(int side, int pos);
int SzDuel_GetUnitCtlRateM(int side, int pos);
int SzDuel_SetUnitOpenRate(int side, int pos, int pair, int openRate);
int SzDuel_GetUnitOpenRate(int side, int pos);
/* 0x1e1e70, the setter beside SzDuel_GetUnitStatus (jump table 0x2bede0, six entries).
 * Answers 0 for no checked item or a selector outside 0..5, else 1 after
 * writing the low bit of `value` to bit `bit` of +0x57; selectors 2 / 3 also
 * set the pair at bits 4-5 / 6-7 of +0x56 to 1 for a non-zero value and to
 * 3 for zero; selectors 4 / 5 write bit 4 / 5, then do what 2 / 3 do. */
int SzDuel_SetUnitStatus(int side, int pos, int bit, int value);
int SzDuel_GetUnitStatus(int side, int pos, int bit);

int SzDuel_SetField(int x, int y, int value);
int SzDuel_GetField(int x, int y);
void SzDuel_InitFieldRate(void);
int SzDuel_SetFieldRate(int x, int y, int value);
int SzDuel_GetFieldRate(int x, int y);
int SzDuel_GetMapMovable(int x, int y);
void SzDuel_InitMapColor(int a, int b, int c);
void SzDuel_InitMapColor2(int a, int b, int c, int d);
void SzDuel_SetMapColor(int col, int row, int c, int d, int e);
int SzDuel_SetMapAlpha(int x, int y, int alpha);
struct DotrStateItem *SzDuel_GetMapUnit(int x, int y);
int SzDuel_GetMapUnitID(int x, int y);

int SzDuel_SetBusyFlag(int side, int pos, int flag);
int SzDuel_SetCardBusyFlag(int side, int pos, int flag);
int SzDuel_SetCardColBusyFlag(int a, int b, int flag);
int SzDuel_SetModelBusyFlag(int side, int pos, int flag);
int SzDuel_SetCardPos(int a, int b, struct DotrStateVector *cardPos);
int SzDuel_GetCardPos(int a, int b, struct DotrStateVector *v);
int SzDuel_SetCardRot(int a, int b, struct DotrStateVector *cardRotation);
int SzDuel_GetCardRot(int a, int b, struct DotrStateVector *v);
int SzDuel_SetCardScale(int a, int b, struct DotrStateVector *v);
int SzDuel_GetCardScale(int a, int b, struct DotrStateVector *v);
int SzDuel_SetCardRgba(int side, int pos, struct DotrStateCellQuad *v);
int SzDuel_GetCardRgba(int side, int pos, struct DotrStateCellQuad *v);
void SzDuel_OpenHandPopup(int a, int b, int c);
void SzDuel_CloseHandPopup(void);
void SzDuel_OpenOptionPopup(int index, int a, int b);
void SzDuel_CloseOptionPopup(void);
void SzDuel_OpenMsgPopup(int operation, int a, int b, int value);
int SzDuel_ActMsgPopup(int unknown);
void SzDuel_CloseMsgPopup(void);
void SzDuel_OpenHandCursor(int a, int b, int c);
void SzDuel_CloseHandCursor(void);
void SzDuel_SetIgnoreJudge(int flag);
void SzBattle_Initialize(void);
int SzBattle_InqCamera(void);
int SzBattle_InqEntry(int index);
void SzBattle_Entry(int side, int packed);
int SzBattle_DrawStage_Pipe(int a);
int SzDuel_DrawStage_Pipe(int a);
int lSzBattle_DrawWasteland_Pipe(int cursor, int index, int count);
int lSzBattle_DrawGrass_Pipe(int cursor, int index, int skip);

/* Same body as SzDuel_GetUnit; several callers contain it inlined. */
static inline struct DotrStateItem *dotr_state_item_checked(int side, int pos)
{
    struct DotrStateItem *unit = SzDuel_GetCardInfo(side, pos);
    if (unit == 0) {
        return 0;
    }
    if (unit->slot >= 11) {
        return 0;
    }
    return unit;
}

#define DOTR_STATE_MAX(a, b) ((a) > (b) ? (a) : (b))
#define DOTR_STATE_MIN(a, b) ((a) < (b) ? (a) : (b))

/* Dependency signatures inferred from the call sites. */
/* Defined in item_release; returns 0 when the checked item is missing, else 1. */
extern int SzDuel_DelUnit(int side, int unitArrayPos);
extern int SzModel_GetModelType(int modelId);
extern void lSzDuel_DoUnitRequest(struct DotrStateCell *cell, DotrStateWord unknown);
extern int SzDuel_GetUnitLeaderAbility3(struct DotrStateItem *unit, int ability, int extraDataOut, int ignoreMenu);
extern unsigned GetMoveRev(struct DotrStateItem *item, int x, int y);
extern void SzDuel_SetMapColor2(int a, int b, int c, int d, int e, int f);
extern void RhMem_FillMemory(void *destination, int value, int size);
extern int SzModel_GetDrawFlag(int unknown);
/* lSzDuel_DrawStage_Pipe (duel_stage_draw): `int (int)` as seen from its one caller, 0x1ae5a0. */
extern int lSzDuel_DrawStage_Pipe(int unknown);
extern int lSzBattle_DrawStage_Pipe(int unknown);
extern int SzDuel_TrimUnitDir2(int a, int b, int c, int d);
extern void SzEffect_LoadModule(int a, int b, int c);
extern int SzModel_LoadMerge(int modelType, int b, int c);
extern int SzData_GetCurrentDeck(int side);
extern int SzData_GetLeader(int side, int deckNo);
extern int SzModel_Load(int modelId, int modelType, int modelNo, int d);
extern int SzPad_Check(int unknown, int zero, int mask);
extern void SzSound_PlaySE(int zero, int soundEffectNumber);
extern int SzGame_GetMenuNum(int unknown);
extern int SzData_AddBag(int side, int key, int cardNo);
extern int SzData_RollSpecial(int side, int cardNo);
extern void SzSound_TransferSEGrp(int a, int b, int c);
extern int SzData_GetOption(int side, int unknown_1);
extern int SzModel_DrawWithOutCalc_Pipe(int cursor, int modelId, int c, void *d, int e, int f, int g);
/* The two float registers are loaded with constants at both observed calls;
 * their position among the integer parameters is not established. */
extern int lSzBattle_DrawFakeShadow(int cursor, int kind, int unknown, float x, float y);

/* Assumed ticker view: an unsigned halfword counter at +0x1a4 of the object. */
struct DotrGridTicker {
    unsigned char unknown_000[0x1a4];
    unsigned short tick;
};
void TaTuto_DrawFinger(struct DotrGridTicker *p, unsigned char period, int a, int b, int c);

/* Pointer-sized integer so the colour array address can be passed as an int word. */
#ifdef __MWERKS__
typedef int DotrGridWord;
#else
#include <stdint.h>
typedef intptr_t DotrGridWord;
#endif

/* Assumed second ticker view: the same halfword counter at +0x162 of another object. */
struct DotrGridTicker2 {
    unsigned char unknown_000[0x162];
    unsigned short tick;
};
void TaTuto_DrawTrapArea(struct DotrGridTicker2 *p, unsigned char period, int x, int y);

/* Bit table at 0x29d580: 7-byte rows selected by the item's halfword at +0x3c,
 * one bit per c - 752. */
extern unsigned char EnchMatch[];
/* Item flag query: c == 772 tests the item's kind byte, other c read bit (c - 752)
 * of its table row. */
int Do_ChkEncMatch(unsigned side, int pos, int cardNumber);
/* Counter update through the 0x1e0290 reads and the 0x1e0450 writes (operations 0 add,
 * 1 subtract, 2 multiply, 3 divide, 4 set). An add, subtract or multiply that would leave
 * 0..9999 instead sets the stored adjustment so the counter lands on the limit. Returns 1,
 * or -1 when op 3 is given a zero divisor. */
int TaUtil_CalcUnitAP(int side, int pos, int operation, int val);
/* Same update as 0x250ca0 for the second counter: reads through 0x1e0370 and writes
 * through 0x1e0570, which use the source halfword at +0x0a and the item adjustment at +0x3a. */
int TaUtil_CalcUnitDP(int a, int b, int op, int val);

/* Assumed view of the sliding object used by 0x25a940: a halfword position at +2 and a
 * start counter at +0xcc. */
struct DotrSlideObject {
    unsigned short unknown_00;
    unsigned short pos;
    unsigned char unknown_04[200];
    int counter;
};
/* Slides o->pos by 30 per call toward 480 (mode 1) or toward 0 / 960 chosen by dir == 1
 * (other modes), reporting the position through SzDuel_OpenHandPopup(x, 0, pos - 480).
 * While the position is 0 and the counter is still 0, the position is first set to 960
 * unless dir == 1. Returns 1 once the target is reached. The first argument is unused. */
int TaEff_PopHand(int unused, struct DotrSlideObject *o, unsigned char dir, int x, unsigned char mode);

/* 0x1e11b0: 0 when the checked item lookup (0x1dd130, +0x45 below 11) fails.
 * Otherwise writes four floats: [0] and [2] are (x - 3) * 50 and (y - 3) * 50
 * for the item's +0x40 / +0x41 bytes when below 7 (else 0) plus the signed
 * halfwords at +0x34 / +0x36; [1] is the grid cell's +0x14 halfword times 35
 * over 128; [3] is 1.0. Returns 1. For a byte of 7 or more the cell lookup
 * yields null and the halfword is read from address 0x14, as in the original.
 * The float-array output and the halfword view of +0x34 are assumed. */
int SzDuel_GetUnitPos(int a, int b, float *out);

/* 0x250360 is defined in small_misc with another declaration; the `_pair`
 * view is the two-int form other groups use. */
extern int func_00250360_pair(int a, int b);

/* 0x1e2570. Stores 0xff in unknown_17 of all 49 cells, then 0 in each of the
 * up to eight in-range cells around (cx, cy) unless its unknown_16 is 8 or
 * its 0x1e36e0 item has Ta_IsUnitControl(unknown_44, unknown_45) different from side.
 * The cell lookup, the unknown_16 read (-1 out of range) and the unknown_17
 * store are the range-checked bodies other functions here have, inlined.
 * Assumed: three int parameters. */
void lSzDuel_CalcMapSummonEnable(int side, int cx, int cy);

/* 0x1e2a60: one step of the reach search over the 7x7 cells. Leaves at once when (x, y) is out
 * of range; below the first level also when (x, y) is one of the n (xs, ys) pairs, or the cell's
 * 0x1e36e0 item has Ta_IsUnitControl(unknown_44, unknown_45) different from side while check is set, or
 * the item has unknown_45 0 and unknown_44 equal to side. Records (x, y) at px/py[depth], lowers
 * the cell's unknown_17 to n + depth (not for an item-less cell whose unknown_16 is 8 while pass
 * is 0), spends one of budget on an item cell or such an 8 cell when n + depth > 0 and leaves
 * when none is left, then visits the four neighbours not already in px/py while depth + 1 does
 * not exceed limit. Assumed: all thirteen parameters as int or byte pointers, void return. */
void lSzDuel_CalcMapMovableRecursive(int pass, int check, int side, int x, int y, int limit, unsigned char *xs,
                                     unsigned char *ys, int n, unsigned char *px, unsigned char *py, int depth,
                                     int budget);
/* 0x1e27d0: with reset set stores 0xff in unknown_17 of all 49 cells, else 0xfe in every cell
 * below 0xff; runs the search above from (x0, y0) with two 64-byte work arrays (their size is
 * the frame's; the element count is assumed), depth 0 and budget 1; then stores i in unknown_17
 * of each in-range cell (xs[i], ys[i]), i < n. Assumed: ten parameters as int or byte pointers. */
void lSzDuel_CalcMapMovable(int reset, int pass, int check, int side, int x0, int y0, int limit, unsigned char *xs,
                            unsigned char *ys, int n);

/* 0x1e5210: load callback; stores the record's word at 0x14 in gSzBattle_bl[2] when the
 * load completed, else 0. Only the word at 0x14 is used; the record is assumed to be the
 * resource submit record (resource_submit.h), declared here by its offsets. */
struct DotrStateLoadRecord {
    int unknown_00[5];
    int unknown_14;
};
void lSzBattle_CameraCB(struct DotrStateLoadRecord *record, int completed);

/* TaEnc_GetEnchantLst (0x249410): name, parameters and the locals temp, PushNum, i from the
 * Japanese build's debug information (which gives an unsigned char result; this body returns
 * the count without narrowing, so int is used). Row nCardNo of the bit table at 0x29d580. */
int TaEnc_GetEnchantLst(unsigned short nCardNo, unsigned short *pEncLst);

/* SzDuel_DrawLine_Pipe (0x1dc560). Assumed: int cursor; the position local is four floats (the
 * frame leaves 16 bytes for it; only three are written); SzDuel_DrawCursor_Pipe returns the
 * next cursor (scratch_quads.h defines it as void: its last call's result stays in v0, and
 * this caller uses it). MATCHING DEVICE: the two extents are locals assigned in a chain;
 * passed as constants, the compiler would load $f12 first, while the original loads $f13 first. */
extern int SzDuel_DrawCursor_Pipe(int a, float *centre, float dx, float dz, int alpha, unsigned char *colour,
                                  int single);
int SzDuel_DrawLine_Pipe(int cursor);

#endif
