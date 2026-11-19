#ifndef DOTR_EE_LOW_LEAVES_H
#define DOTR_EE_LOW_LEAVES_H

/* Unrelated leaf functions with no shared storage, kept together in one group: eight below the
 * library block (0x101ad0-0x17b3f0) and four in the game code after it (0x255a30-0x271950).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* Library-block callee of 0x17b3f0; an int result is assumed. */
extern int func_001a2e60(void);

/* Stores first at +2 and +0, second at +1, third at +3. */
void ShFx_initTimer(char *out, char first, char second, char third);
/* Swaps two words of an array; the element type is assumed. */
void TaLogic017_SortSwap(int *values, int a, int b);
/* Assumed prototype from the call sites (defined in side_table). */
extern int TaHand_GetCardSummonCost2(int cardNo, int teamID);
/* Recursive partition sort of values[lo..hi] by TaHand_GetCardSummonCost2(table[values[i]], key),
 * larger first. It reads halfwords but exchanges with TaLogic017_SortSwap, which moves words:
 * the bytes show both, the intended element type is not known. */
void TaLogic017_SortTempArray(unsigned short *values, int lo, int hi, int key, int *table);
/* 1 when value equals one of the first 49 halfwords of list. */
int TaLogic018_IsZonePast(unsigned int value, unsigned short *list);
/* Returns 0 as a 64-bit result (the code clears the full register). */
long TaLogic_GetWarnHideLvl__0016b0e0(void);
/* 1 when 0x1a2e60() is positive. */
int _dpfgt(void);
int _dpflt(void);

/* Callees of 0x15a180 with their usual signatures. */
extern int SzDuel_GetField(int x, int y);
extern int TaLogic018_GetZoneSize(unsigned short *p);
extern int GetCap(int limit, int value);
/* 0x2717a0: prototype from the Japanese build's debug information (Leader.c); the result
 * local is assumed. */
int Ta_GetLDChgLand(int Race);
/* 0x271830: prototype from the Japanese build's debug information. Callee signatures as in
 * kind_gates.h. */
int Ta_GetLDLvRev(int nSide);
extern int Ta_IsLDAbl(int ability, int side);
extern int SzDuel_GetUnitConstKind(int side, int pos);
extern int SzDuel_GetUnitLevel(int side, int pos, int ignoreBoosts);
/* 0x271950: prototype and the locals MinJ, MinI, j, i from the Japanese build's debug
 * information. MATCHING DEVICE: the two further locals (half range, upper z bound) give the
 * original register assignment. */
int Ta_IsSquareRange(int nX, int nZ, int mX, int mZ, int R);

/* Does nothing when 0x15a2e0 finds y << 8 | x in the list or SzDuel_GetField(x, y)
 * is not 9. Otherwise stores y << 8 | x at index TaLogic018_GetZoneSize(list) and calls
 * itself for every cell of the block [GetCap(0, x - 1), min(x + 1, 6)] x
 * [GetCap(0, y - 1), min(y + 1, 6)] that is exactly one step away.
 * Assumed: int x and y, 49-halfword list, no result. */
void TaLogic018_GetZoneSide(int x, int y, unsigned short *list);

/* Ta_Do_Effect (0x255a30): prototype, the local eID and struct jp_11dc9 from the Japanese
 * build's debug information. struct jp_11e6e is only passed on and stays incomplete here;
 * slot_orders.h describes the same two objects as jp_4e65f / jp_4e704 and small_misc.h types
 * the callees differently (job pointer, plain ints); the views are not reconciled. */
struct jp_11dc9 {
    int eNO;
    unsigned int dwFrom;
    unsigned int dwTo;
    unsigned int dwPar;
};
struct jp_11e6e;
int TaEffect_IsLoadModule2(struct jp_11e6e *Temp, int eNO);
int SzEffect_Entry(int a, int b, int c, int d, int e);
int Ta_Do_Effect(struct jp_11dc9 *eInfo, struct jp_11e6e *Temp);

#endif
