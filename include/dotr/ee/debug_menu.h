#ifndef DOTR_EE_DEBUG_MENU_H
#define DOTR_EE_DEBUG_MENU_H

/* SzDebug_Act and lSzDebug_ActEffect: adjacent functions (0x203e20, 0x2047f0) that drive the debug
 * menu state at 0x325c80 from pad input; the first calls the second.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/engine.md. */

#include "dotr/ee/small_state.h"

/* SzDebug_Act reaches the debug state at 0x325c80 through data_00325c80_owner (small_state.h).
 * These index its leading words; the flag byte at +0x2c holds bit 0 hide, bit 1 effect menu
 * available, bit 2 effect menu active. The names describe the observed use and are not original;
 * a structure with named members in place of the word array changes the generated code. */
enum {
    DOTR_DEBUG_MENU = 0,  /* 0x00: page 0..17; stepped by 17 or 19 modulo 18 */
    DOTR_DEBUG_MODEL = 1, /* 0x04: model slot, 0..43 */
    DOTR_DEBUG_DIGIT = 2, /* 0x08: which of the three decimal digits is edited, 0..2 */
    /* lSzDebug_ActEffect: the five values handed to SzEffect_Entry, then the edit cursor. */
    DOTR_DEBUG_EFF_NO = 3,     /* 0x0c: effect number, three decimal digits */
    DOTR_DEBUG_EFF_ARG = 4,    /* 0x10: third argument, three decimal digits */
    DOTR_DEBUG_EFF_A_HI = 5,   /* 0x14: 0..1, high byte of the fourth argument */
    DOTR_DEBUG_EFF_A_LO = 6,   /* 0x18: 0..10, low byte of the fourth argument */
    DOTR_DEBUG_EFF_B_HI = 7,   /* 0x1c: 0..1, high byte of the fifth argument */
    DOTR_DEBUG_EFF_B_LO = 8,   /* 0x20: 0..10, low byte of the fifth argument */
    DOTR_DEBUG_EFF_ROW = 9,    /* 0x24: which of the six values is edited, 0..5 */
    DOTR_DEBUG_EFF_COLUMN = 10 /* 0x28: digit within the edited value */
};
/* The bytes of a card's constant record and of a card record that SzDebug_Act touches. */
struct DotrDebugCardConst {
    unsigned char unknown_00;
    unsigned char unknown_01;
    unsigned char unknown_02;
};
struct DotrDebugCard {
    struct DotrDebugCardConst *info; /* 0x00 */
    char unknown_04[0x38];
    unsigned short unknown_3c;       /* card id from SzDuel_Xax2New */
    unsigned char unknown_3e;
    unsigned char unknown_3f;
    char unknown_40[4];
    unsigned char unknown_44;        /* passed as the first argument of SzBattle_Entry */
    unsigned char unknown_45;
};
/* Twelve bytes at 0x293d28 copied into the local digit array before it is overwritten. */
struct DotrDebugDigits {
    int d[3];
};
extern struct DotrDebugDigits data_00293d28;
/* The same for lSzDebug_ActEffect's two digit arrays (0x293d38, 0x293d48). */
extern struct DotrDebugDigits data_00293d38;
extern struct DotrDebugDigits data_00293d48;
/* Ten string pointers at 0x293d00 indexed by model type; only handed to func_0019bef0. */
extern char *gBaseStr_ptrs[];

/* Callees of SzDebug_Act with assumed prototypes (argument counts from the call sites). */
extern int SzModel_IsModelStandby(int index);
extern int SzModel_GetModelType(int modelId);
extern int SzGame_GetMode(int);
extern int SzView_GetMode(int index);
extern int SzView_SetMode(int index, int value);
extern void lSzDebug_ActEffect(void);
/* Callees of lSzDebug_ActEffect, as declared in pool_table.h and small_misc.h. */
void SzEffect_Clear(void);
int SzEffect_Destroy2(int a, int b);
int SzEffect_Entry(int a, int b, int c, int d, int e);
int SzEffect_Erase2(int a, int b);
int SzEffect_Sleep2(int a, int b, int flag);
extern int SzSound_PlaySE(int index, int soundEffectNumber);
extern int SzModel_GetModelNo(int modelId);
extern int SzModel_IsModelLoading(int modelId);
extern struct DotrDebugCard *SzDuel_GetCardInfo(int side, int pos);
extern int SzDuel_Xax2New(int index);
extern struct DotrDebugCardConst *SzDuel_GetCardConstInfo(int cardNo);
extern int SzModel_Preset(int modelId, int modelType, int modelNumber);
extern void *SzDuel_GetUnit(int side, int pos);
extern int SzModel_Load(int modelId, int modelType, int modelNo, int d);
extern void SzBattle_Entry(int side, int packed);
extern int SzDuel_SetBusyFlag(int side, int pos, int flag);
extern int SzDuel_SetUnitCtlModeM(int side, int pos, int value);
extern int SzDuel_SetUnitCtlRateM(int side, int pos, int pair, int value);
extern int SzModel_SetDeform(int modelId, int value);
extern int SzModel_SetStoneFlag(int modelId, int value);
extern int func_0019bef0(const char *string);
extern int SzModel_IsModelReady(int index);
extern int SzModel_IsModelDisp(int modelId);
extern int SzModel_GetDeform(int modelId);

/* 0x203e20 SzDebug_Act (name from the Japanese build's symbol table): per-frame debug menu
 * input. Assumed: no arguments, no result. */
void SzDebug_Act(void);

#endif
