#ifndef DOTR_EE_BATTLE_DRAWS_H
#define DOTR_EE_BATTLE_DRAWS_H

/* Battle-stage draw functions (lSzBattle_Draw*_Pipe) that pass position, rotation and scale
 * vectors to SzModel_DrawWithOutCalc_Pipe.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/engine.md. */

/* A word that holds an address: `int` on the target, as object_table.h declares
 * SzModel_DrawWithOutCalc_Pipe's third and fifth arguments (the fourth, the rotation, is
 * passed the same way). */
#ifdef __MWERKS__
typedef int DotrBattleDrawWord;
#else
#include <stdint.h>
typedef intptr_t DotrBattleDrawWord;
#endif

/* Only the byte read at 0x46 of SzDuel_GetUnit's result (unsigned load). */
struct DotrBattleDrawUnit {
    char unknown_00[0x46];
    unsigned char unknown_46;
};

/* Quadword vectors as lSzBattle_DrawNormal_Pipe declares them: a 128-bit word
 * (each initial value is copied whole with one lq/sq pair), four floats or four ints. Member
 * meaning (position/scale in floats, rotation in integer angle units) is assumed from use: only
 * x and z of the float vectors and y of the integer vector are touched. */
#ifdef __MWERKS__
typedef __int128 DotrBattleDrawQ128;
#else
__extension__ typedef __int128 DotrBattleDrawQ128;
#endif
union DotrBattleDrawQuad {
    DotrBattleDrawQ128 q;
    float f[4];
    int i[4];
};

/* Initial values at 0x2922b0 (scale) and 0x2922c0..0x292300 (one rotation per kind). */
extern union DotrBattleDrawQuad data_002922b0;
extern union DotrBattleDrawQuad data_002922c0;
extern union DotrBattleDrawQuad data_002922d0;
extern union DotrBattleDrawQuad data_002922e0;
extern union DotrBattleDrawQuad data_002922f0;
extern union DotrBattleDrawQuad data_00292300;
/* Words at 0x31ab88 (state_grid.h has the same declaration). */
extern int gSzBattle_bl[];

extern int SzModel_DrawWithOutCalc_Pipe(int cursor, int modelId, DotrBattleDrawWord c, DotrBattleDrawWord d,
                                        DotrBattleDrawWord e, int f, int g);
extern int SzModel_GetPos(int modelId, union DotrBattleDrawQuad *xyzCoords);
/* As in state_grid.h: the position of the two float arguments is not established. */
extern int lSzBattle_DrawFakeShadow(int cursor, int kind, int unknown, float x, float y);
extern struct DotrBattleDrawUnit *SzDuel_GetUnit(int side, int pos);

/* 0x1e60e0: draws model `index` once, or 4 / 2 / 12 / 28 times for kind 1 / 3 / 5 / 6 with
 * stepped rotations or offset positions; kind 0 also draws the shadows. The
 * parameter names are assumed (the Japanese build gives only the function name). */
int lSzBattle_DrawNormal_Pipe(int cursor, int index, int kind);

#endif
