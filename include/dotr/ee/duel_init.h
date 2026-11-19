#ifndef DOTR_EE_DUEL_INIT_H
#define DOTR_EE_DUEL_INIT_H

/* lSzDuel_InitCom (duel scene start: model draw flags, deform and the effect environment) and
 * SzDuel_Init (duel reset: decks, graves, hands, leader units, options and the map).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* Addresses are handed to the SzEffect setters as words; on the target this is the `int` that
 * index_fanout.h declares for them. */
#ifdef __MWERKS__
typedef int DotrInitWord;
#else
#include <stdint.h>
typedef intptr_t DotrInitWord;
#endif

/* Five objects whose addresses lSzDuel_InitCom passes on. Only data_002910b0 is read here (two
 * floats compared for equality); the element types and sizes of the others are assumptions. */
extern float data_002910b0[2];
extern char gSzDuel_StageParam[];
extern char data_0029100c[];
extern char data_00291020[];
extern char data_002910b8[];

/* Callee prototypes assumed from the arguments this caller passes. */
extern int SzGame_GetMode(int);
extern void SzView_SetStraightLight(int unknown, int index, int value);
extern int SzModel_SetDrawFlag(int index, int value);
extern int SzModel_SetDeform(int modelId, int value);
extern int SzModel_SetDeformRatio(int index, int deformToSet);
extern void SzEffect_SetFogColorForMonster(DotrInitWord fogColour);
extern void SzEffect_SetFogColorForDuel(DotrInitWord value);
extern void SzEffect_SetFogParamZForMonster(DotrInitWord value);
extern void SzEffect_SetFogParamZForDuel(DotrInitWord value);
extern void SzEffect_SetFogParamYForMonster(DotrInitWord value);
extern void SzEffect_SetFogParamYForDuel(DotrInitWord value);
extern void SzEffect_SetStraightLightForMonster(DotrInitWord pointLightParameters);
extern void SzEffect_SetStraightLightForDuel(DotrInitWord value);
extern void SzEffect_SetPointLightForMonster(DotrInitWord pointLightParameters);
extern void SzEffect_SetPointLightForDuel(DotrInitWord pointLightParameters);
extern void SzEffect_SetIntensityForMonster(DotrInitWord intensityValue);
extern void SzEffect_SetIntensityForDuel(DotrInitWord value);
extern void SzEffect_SetBlackOutForMonster(DotrInitWord value);
extern void SzEffect_SetBlackOutForDuel(DotrInitWord blackoutAmount);

/* lSzDuel_InitCom (0x1ada80): name from the Japanese build's symbol table; one argument, tested
 * against zero (int assumed), no result used. */
void lSzDuel_InitCom(int flag);

/* 0x1add00 SzDuel_Init (original name). Assumed: the control block at 0x317490 through this
 * function's own offset view (cast from the window_control.h object, whose grouping of the flag
 * bits at +0xd65..+0xd67 follows other readers); every flag here as a signed one-bit field
 * except the 2-bit fields at +0xd64 and +0xd67 and bit 1 of +0xd66, which is loaded unsigned;
 * the word at +0x334 of a side as an address handed to SzMem_Free (kept as `int` so the view has
 * the same layout on the host); the two 16-byte objects at 0x2910c0 / 0x2910d0 as four floats
 * copied whole; callee signatures from the argument registers of this caller. No result is set,
 * so the function is declared void here; board_reset.h declares it `int` for
 * SzDuel_Initialize, which leaves v0 untouched after the call; the true return type is
 * unknown. */
#include "dotr/ee/window_control.h"
struct DotrInitVector {
    float x, y, z, w;
};
struct DotrInitPair {
    short a;
    short b;
};
struct DotrInitSide {
    char unknown_000[464];
    int unknown_1d0;
    int unknown_1d4;
    int unknown_1d8;
    char unknown_1dc[340];
    int unknown_330;
    int memory_334;
    short unknown_338;
    unsigned short points;
    unsigned char player;
    unsigned char x;
    unsigned char y;
    char unknown_33f[2];
    unsigned char unknown_341;
    unsigned char unknown_342;
    unsigned char unknown_343;
    unsigned char unknown_344;
    unsigned char unknown_345;
    signed char unknown_346_bit_0 : 1;
    signed char unknown_346_high : 7;
    char unknown_347[9];
};
struct DotrInitControl {
    struct DotrInitSide sides[2];
    char unknown_6a0[40];
    int unit_6c8;
    int unit_6cc;
    char unknown_6d0[1420];
    int unknown_c5c[7];
    int unknown_c78;
    char unknown_c7c[10];
    short unknown_c86;
    short unknown_c88;
    char unknown_c8a[12];
    short unknown_c96;
    short unknown_c98;
    struct DotrInitPair unknown_c9a[4];
    short unknown_caa[4];
    unsigned char unknown_cb2[4];
    char unknown_cb6[126];
    unsigned char unknown_d34;
    unsigned char unknown_d35;
    unsigned char unknown_d36;
    unsigned char map;
    unsigned char unknown_d38[4];
    unsigned char side;
    unsigned char unknown_d3d;
    unsigned char unknown_d3e;
    unsigned char unknown_d3f[17];
    unsigned char unknown_d50;
    unsigned char unknown_d51;
    unsigned char unknown_d52;
    unsigned char unknown_d53;
    unsigned char unknown_d54;
    unsigned char unknown_d55;
    unsigned char unknown_d56;
    unsigned char unknown_d57;
    unsigned char unknown_d58;
    unsigned char unknown_d59;
    unsigned char unknown_d5a;
    unsigned char unknown_d5b;
    char unknown_d5c[7];
    unsigned char unknown_d63;
    signed char unknown_d64_01 : 2;
    signed char unknown_d64_23 : 2;
    signed char unknown_d64_45 : 2;
    signed char unknown_d64_6 : 1;
    signed char unknown_d64_7 : 1;
    signed char unknown_d65_0 : 1;
    signed char unknown_d65_1 : 1;
    signed char unknown_d65_2 : 1;
    signed char unknown_d65_3 : 1;
    signed char unknown_d65_4 : 1;
    signed char unknown_d65_5 : 1;
    signed char unknown_d65_6 : 1;
    signed char unknown_d65_7 : 1;
    signed char unknown_d66_0 : 1;
    unsigned char unknown_d66_1 : 1;
    signed char unknown_d66_2 : 1;
    signed char unknown_d66_3 : 1;
    signed char unknown_d66_4 : 1;
    signed char unknown_d66_5 : 1;
    signed char unknown_d66_6 : 1;
    signed char unknown_d66_7 : 1;
    signed char unknown_d67_0 : 1;
    signed char unknown_d67_1 : 1;
    signed char unknown_d67_2 : 1;
    signed char unknown_d67_3 : 1;
    signed char unknown_d67_4 : 1;
    signed char unknown_d67_5 : 1;
    signed char unknown_d67_67 : 2;
};
extern struct DotrInitVector data_002910c0;
extern struct DotrInitVector data_002910d0;
extern void SzDuel_InitDeck(int side);
extern void SzDuel_InitGrave(int side);
extern void SzDuel_InitHand(int side);
extern int SzData_GetCurrentDeck(int side);
extern int SzData_GetLeader(int side, int deckNo);
extern void SzDuel_InitCardRequest(void);
extern void SzDuel_InitCardControl(void);
extern int SzDuel_DelUnit(int side, int unitArrayPos);
extern void SzDuel_InitMapColor(int a, int b, int c);
extern void SzDuel_AddUnit(int side, int col, int row, int cardNo, int e);
extern int SzDuel_SetUnitCtlRateM(int side, int pos, int pair, int value);
extern int SzDuel_SetUnitCtlModeM(int side, int pos, int value);
extern int SzData_GetCardNo(int side, int deckNo, int cardSlot);
extern int SzGame_GetMenuNum(int unknown);
extern int SzDuel_AddDeck(int side, int cardNo);
extern int SzDuel_SetDeck(int side, int position, int cardNo);
extern int SzData_GetOption(int side, int which);
extern int SzData_SetOption(int side, int which, int flag);
extern int SzView_SetCP(int index, struct DotrInitVector *in);
extern int SzView_SetLookSP(int index, struct DotrInitVector *in);
extern int SzDuel_SetFieldRate(int x, int y, int value);
extern int SzSound_TransferSEGrp(int index, int id, int force);
extern void SzDuel_SetupMap(int index);
extern int TaLogic_InitLogicData(int side, struct DotrInitSide *set);
extern void SzMem_Free(void *pointer);
void SzDuel_Init(int map, int side, int fresh);

#endif
