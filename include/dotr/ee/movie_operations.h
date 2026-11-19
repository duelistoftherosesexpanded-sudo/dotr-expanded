#ifndef DOTR_EE_MOVIE_OPERATIONS_H
#define DOTR_EE_MOVIE_OPERATIONS_H

/* An address held in a word: 32 bits on the target, pointer-sized on the host. Not an
 * original type.
 */
#ifdef __MWERKS__
typedef unsigned int DotrMovieAddress;
#else
#include <stdint.h>
typedef uintptr_t DotrMovieAddress;
#endif

/* The offsets used by the functions at 0x1a87c0..0x1a8ba8: a prefix of the object as they
 * access it, not its complete layout. The indexed byte accesses at 0x1f0/0x1f1/0x1f9 stay
 * address-based. The eight-byte spans keep the neighbouring offsets in place; the original
 * array declarations and the index ranges of the callers are unknown.
 */
struct DotrMovieOperationsStatePrefix {
    unsigned char unknown_000[8];
    unsigned int field_008; /* counted up to 900 by lSzGame_ActTitle */
    int field_00c;
    unsigned char unknown_010[0x14c];
    int field_15c; /* set from the high byte of field_21c by lSzGame_ActTitle */
    int field_160; /* cleared by 0x1ac560; object_table.h views it as data_003173b0 */
    int field_164; /* set to 1 or 0 by lSzGame_ActTitle before mode 11 */
    unsigned char unknown_168[0x28];
    int field_190; /* tested for non-zero by SzGame_ModeAct */
    unsigned char unknown_194[0x5d];
    signed char bytes_1f1[8];
    unsigned char bytes_1f9[8];
    unsigned char unknown_201[0xb];
    unsigned char bytes_20c[4]; /* stepped between 0 and 255 by SzGame_ModeAct (mode 17) */
    unsigned char unknown_210[4];
    int field_214; /* stepped by SzGame_ModeAct (mode 17) */
    int field_218;
    unsigned short field_21c;
    signed char field_21e;
    signed char field_21f;
    signed char field_220;
    signed char field_221;
    unsigned char field_222;
    unsigned char unknown_223;
    unsigned char field_224; /* title screen: steps of 4 up to 128 */
    unsigned char field_225; /* title screen: selected entry 0..3 */
    unsigned char field_226; /* title screen: steps of 4 up to 128 */
    unsigned char field_227;
    signed char field_228;
    signed char field_229;
    unsigned char unknown_22a[2];
    struct {
        signed char unknown_0 : 2;
        signed char field_2 : 2;
        unsigned char field_4 : 2;
        signed char unknown_6 : 2;
    } field_22c;
    unsigned char unknown_22d;
    struct {
        signed char unknown_0 : 4;
        signed char field_4 : 1;
        signed char field_5 : 1;
        signed char unknown_6 : 2;
    } field_22e;
    struct {
        signed char field_0 : 1;
        signed char field_1 : 1;
    } field_22f;
};

extern struct DotrMovieOperationsStatePrefix dotr_ee_movie_operations_state_00317250;
extern void SzFade_Entry(int, int, int, int, int, int);
extern void SzGame_ReqBGM(int, int);

/* Only element 0 of each is read here; the array lengths are not significant (0x299e10 starts
 * a larger block that the camp groups view as structures). */
extern short SelectedBattleArena[8];
extern signed char data_00299e10[16];
void Taku_Camp_Set_Go_Duel(void);

void SzGame_Goto(int arg0, int arg1);
void SzGame_Push(int arg0, int arg1);
void SzGame_Pop(void);
void SzGame_SetMode(int arg0, int arg1);
void SzGame_ComInit(void);
/* 0x1ac560 accesses the state directly through a symbol, here a second name
 * for the same storage (the same object on the host). The result of
 * SzGame_ActMemCardProc (0x1ab1d0) is only tested for a negative value; no arguments are
 * loaded for it. */
#ifdef __MWERKS__
extern struct DotrMovieOperationsStatePrefix gSzGame_gl_operations;
#else
#define gSzGame_gl_operations dotr_ee_movie_operations_state_00317250
#endif
extern int SzGame_ActMemCardProc(void);
void lSzGame_ActMemCheck(void);

/* Callees of SzGame_ModeAct (0x1aa260). Most are declared as their sources have them;
 * for the mode functions (SzDebug_Act, SzModel_Act, SzView_Act, SzDuel_Act,
 * SzDuel_ActBuildDeck, lSzGame_ActTitle, TkCamp_Act, ShFace_Act, SzView_ActCamera, ShSlot_Act,
 * SzText_LoadMerge and the unnamed 0x1a7380) the argument lists are the registers loaded
 * here and the results are assumed unused, except SzDuel_Act's, which is tested. */
extern int SzGame_GetMode(int);
extern int SzFile_InqRequestEx(void);
extern int SzSound_IsLoadingBGM(void);
extern int SzText_LoadMerge(int a, int b);
extern int SzFade_SetPauseFlag(int index, int flag);
extern int SzSound_IsStandby(void);
extern int SzBattle_InqEntry(int index);
extern int SzDuel_GetUnitModelID(int side, int pos);
extern int SzModel_ReqSE(int channel, int modelId);
extern void SzEffect_ReqSE(void);
extern unsigned char SzModel_Motion(int modelId);
extern void SzEffect_TrimModule(void);
extern int SzFade_IsDoing(int flag);
extern void SzGame_ActMenu(void);
extern void SzDebug_Act(void);
extern int SzDebug_IsEffect(void);
extern void SzModel_Act(int index);
extern void SzView_Act(int a);
extern int SzDuel_Act(void);
extern void SzDuel_ActBuildDeck(void);
extern void SzDuel_Act3DView(void);
extern void SzDuel_ActTrade(void);
extern void SzDuel_ActCustomDuel(void);
/* 0x1ac700: no arguments are loaded and no result is set. */
void lSzGame_ActTitle(void);
extern void TkCamp_Act(void);
extern void ShFace_Act(void);
extern void SzDuel_ActMapEdit(void);
extern void SzDuel_ActCardTest(void);
extern void SzView_ActCamera(void);
extern void ShSlot_Act(void);
extern int SzPad_Check(int unknown, int zero, int mask);
/* Callees of lSzGame_ActTitle (0x1ac700), declared as their groups have them. func_00197548
 * is library code (out of scope); its argument here is the address 0x2be80f, taken
 * to be a string, and its result is only tested for zero. */
extern char *func_00197548(const char *label);
extern char data_002be80f[];
extern int SzData_GetOption(int side, int which);
extern int SzData_SetOption(int side, int which, int flag);
extern int SzSound_SetOutput(int flag);
extern int SzSound_PlaySE(int index, int soundEffectNumber);
extern int SzSound_IsPlaySE(void);
extern void SzGame_InitMemCardProc(int kind);
/* 0x1aa260: no arguments are loaded and no result is set. */
void SzGame_ModeAct(void);

#endif
