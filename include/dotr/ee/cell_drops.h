#ifndef DOTR_EE_CELL_DROPS_H
#define DOTR_EE_CELL_DROPS_H

/* Functions that take a packed cell word from the eight-word list at 0x31ab88 and run the
 * object_table / state_grid sequence for the object on that cell (first member 0x1e53c0).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/engine.md. */

/* 128-bit word for the one-instruction copy of the 16-byte template. On the
 * host it is given the alignment of int and may alias the int view. */
#ifdef __MWERKS__
typedef __int128 DotrDropWord128;
#else
__extension__ typedef __int128 DotrDropWord128 __attribute__((aligned(4), may_alias));
#endif

/* Four words handed to SzModel_SetRot (object_table calls its view DotrObjectVector4). */
struct DotrDropWords {
    int words[4];
};

/* Four floats exchanged with 0x1f1fd0 / 0x1e10f0 / 0x1f22b0. */
struct DotrDropVector {
    float x, y, z, w;
};

/* Block answered by SzDuel_GetUnit (the item other groups call DotrStateItem):
 * only the cell bytes at +0x40 / +0x41 and the object index at +0x46 are
 * read here. */
struct DotrDropBlock {
    unsigned char unknown_00[0x40];
    unsigned char locCol;
    unsigned char locRow;
    unsigned char unknown_42[4];
    unsigned char modelId;
};

/* Eight packed cell words (x << 8 | y), -1 once handled; state_grid.h
 * declares the same storage without a bound. */
extern int gSzBattle_bl[8];
/* Sixteen zero bytes at 0x2921d0 that 0x1e53c0 copies with one lq / sq: read
 * here as a named object, while the original source may have had a local
 * initialiser whose template the compiler placed there. Only element 0 is
 * read; the bound of 2 does not affect the generated code (the build places
 * nothing in the small-data area). */
extern struct DotrDropWords data_002921d0[2];

/* Callees, with this group's own argument views. */
extern struct DotrDropBlock *func_001df540_drop(int a, int b);
extern int SzEffect_ClearRefID2(int packed, int value);
extern int SzDuel_SetBusyFlag(int side, int pos, int flag);
extern int func_001f25d0_drop(int index, struct DotrDropVector *in);
extern int func_001f2430_drop(int index, struct DotrDropWords *in);
extern int func_001f1fd0_drop(int index, struct DotrDropVector *out);
extern void func_001e10f0_drop(int x, int y, int z, struct DotrDropVector *v);
extern int func_001f22b0_drop(int index, struct DotrDropVector *in);
extern int SzModel_SetDeform(int modelId, int value);
extern int SzModel_SetDeformRatio(int index, int deformToSet);
extern int SzDuel_SetUnitCtlRateM(int side, int pos, int pair, int value);
extern int SzDuel_SetUnitCtlGoalM(int side, int pos, int value);
extern int SzModel_SetAnimDirect(int index, int slot);
extern int SzModel_SetAnimDir(int modelId, int animDir);
extern int func_001f26e0_drop(int index, int value, void *in);
extern int func_001f2860_drop(int index, int value, void *in);
extern void SzModel_CancelMotionBlur(int value);
extern int SzSound_StopSE(int index, int value);

/* 0x1e53c0. Reads word = gSzBattle_bl[index], x = word >> 8, y = word & 0xff
 * and asks SzDuel_GetUnit(x, y) for the block. Builds four words from the template
 * with word 1 = 0x8000 when x is 0, else 0. For a null block nothing else
 * happens. Otherwise, for object = block +0x46: SzEffect_ClearRefID2(word, 1),
 * SzDuel_SetBusyFlag(x, y, 0), SzModel_SetScale(object, 0), SzModel_SetRot(object, words),
 * SzModel_GetPos(object, &v), SzDuel_CalcUnitPos(block +0x40, block +0x41, 0, &at), v.x and
 * v.z taken from at, SzModel_SetPos(object, &v), SzModel_SetDeform(object, 1),
 * SzModel_SetDeformRatio(object, 32); then with `mode` SzDuel_SetUnitCtlRateM(x, y, 0, 0) and
 * SzDuel_SetUnitCtlGoalM(x, y, 0), without it SzModel_SetAnimDirect(object, 0) and SzModel_SetAnimDir(object,
 * 1); then SzModel_SetRgba(object, 0, 0), SzModel_SetFilter(object, 1, 0), SzModel_CancelMotionBlur(object),
 * SzSound_StopSE(index + 2, -1) and gSzBattle_bl[index] = -1.
 * Assumed: int index / mode, no result. */
void SzBattle_Return(int index, int mode);

#endif
