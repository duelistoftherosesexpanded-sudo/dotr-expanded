#ifndef DOTR_EE_KIND_DISPATCH_H
#define DOTR_EE_KIND_DISPATCH_H

/* Functions that select one of the per-kind routines by a kind number and follow some with
 * SzDuel_InitCustomDuel(-1) when the signed byte at 0x317250 + 0x21f is 9.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/game-modes.md. */

/* The storage at 0x317250 through its existing declaration (digit_rows.h), cast to
 * signed bytes at the use; only +0x21f is read here. */
#include "dotr/ee/digit_rows.h"

/* Callees, all declared in small_misc.h as void(void) except SzDuel_InitCustomDuel, which is
 * called with one argument here (assumed void(int)). */
extern void SzDuel_Term(void);
extern void SzDuel_InitCustomDuel(int unknown);
extern void SzBattle_Term(void);
extern void SzDuel_TermBuildDeck(void);
extern void SzDuel_Term3DView(void);
extern void SzDuel_TermTrade(void);
extern void SzDuel_TermCustomDuel(void);
extern void lSzGame_TermMemCheck(void);
extern void lSzGame_TermTitle(void);
extern void TkCamp_Term(void);
extern void ShFace_Term(void);
extern void SzEvent_Term(void);
extern void SzDuel_TermMapEdit(void);
extern void ShSlot_Term(void);
extern void SzView_TermCamera(void);

/* 0x1aad50 SzGame_ModeTerm: by kind, 1: SzDuel_Term; 5: SzBattle_Term; 6: SzDuel_TermBuildDeck;
 * 7: SzDuel_Term3DView; 8: SzDuel_TermTrade; 9: SzDuel_TermCustomDuel; 36: lSzGame_TermMemCheck;
 * 10: lSzGame_TermTitle; 11: TkCamp_Term then ShFace_Term; 12: SzEvent_Term; 13:
 * SzDuel_TermMapEdit; 14: ShFace_Term; 15: ShSlot_Term; 19: SzBattle_Term then
 * SzView_TermCamera; anything else: nothing. After the kinds 1, 6 and 15,
 * SzDuel_InitCustomDuel(-1) when the signed byte +0x21f of 0x317250 is 9. The case order (36
 * between 9 and 10) is the order the compare chain shows. Assumed: one int argument, no
 * result. */
void SzGame_ModeTerm(int kind);

/* SzGame_InitMemCardProc's view of the same storage (cast at the use): the short at +0x21c, a 3-bit
 * field at bits 3-5 of +0x22d, and in +0x22e a 3-bit field at bits 0-2 and one bit at
 * bit 3. Unsigned fields are assumed (the code only stores them). */
struct DotrKindStateView {
    unsigned char unknown_000[0x21c];
    short word_21c;
    unsigned char unknown_21e[0xf];
    unsigned char unknown_22d_0 : 3;
    unsigned char kind : 3;
    unsigned char unknown_22d_6 : 2;
    unsigned char state : 3;
    unsigned char second : 1;
    unsigned char unknown_22e_4 : 4;
};

/* Callees of SzGame_InitMemCardProc. The four mode routines are declared in small_misc.h and
 * SzFile_GetEnv in resource_helpers.h with these prototypes; TkMc_Trade_Save_Main (mode_steps)
 * is assumed to have the shape of its neighbours; func_0019b648 is the library formatter as
 * declared in resource_process.h. SzFile_ReadRequestParam comes from resource_submit.h. The text at 0x2be7d0
 * reads "HOME" and at 0x2be821 "%s/Merge/Icon.Mrg" in the executable. */
#include "dotr/ee/resource_submit.h"
extern int TkMc_Load_Main(int mode);
extern int TkMc_Custom_Main(int mode);
extern int TkMc_Trade_Load_Main(int mode);
extern int TkMc_Trade_Save_Main(int mode);
extern int TkMc_Check_Main(int mode);
extern char *SzFile_GetEnv(const char *label);
extern int func_0019b648(char *destination, const char *format, ...);
extern char data_002be7d0[];
extern char data_002be821[];

/* 0x1aafa0 SzGame_InitMemCardProc: by kind 0-6 (jump table at 0x2be750), 0:
 * TkMc_Load_Main(1), state 2; 1 and 2: format a 128-byte local name with
 * func_0019b648(name, 0x2be821, SzFile_GetEnv(0x2be7d0)) and queue it with
 * SzFile_ReadRequestParam(0, 0, name, 0x1e301e0, -1, 0, -1, 0, 0), state 1; 3:
 * TkMc_Custom_Main(1); 4: TkMc_Trade_Load_Main(1); 5: TkMc_Trade_Save_Main(1); 6:
 * TkMc_Check_Main(1), each with state 1. Always afterwards: the short +0x21c is cleared, the
 * kind field takes kind and the one bit is set only for kind 2. Assumed: one int argument, no
 * result; cases 1 and 2 are separate copies with their own local buffer in the original. */
void SzGame_InitMemCardProc(int kind);

#endif
