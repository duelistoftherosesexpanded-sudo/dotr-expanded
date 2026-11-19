#ifndef DOTR_EE_PAD_TOGGLES_H
#define DOTR_EE_PAD_TOGGLES_H

/* 0x1e40a0: pad-driven toggles of two bytes in the control block at 0x317490 (+0xd35,
 * +0xd36).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

#include "dotr/ee/state_records.h"

/* Callees as declared in small_state, sound_gate and side_table. */
extern int SzPad_Check(int unknown, int zero, int mask);
extern int SzSound_PlaySE(int index, int soundEffectNumber);
int SzData_SetOption(int side, int which, int flag);

/* The argument is only handed to SzPad_Check as its first argument. The two
 * bytes are the control view's unknown_d35 (kept 0 or 1) and unknown_d36 (a
 * bit set indexed by unknown_d35 + 1 when written, by 0..2 when read). */
int SzDuel_ActOptionPopup(int pad);

/* 0x1d95f0: pad-driven steps of four words in the block at 0x318200 (+0x5c, +0x60,
 * +0x64 by +/-0x400 modulo 0x10000; +0x70 by +/-1 modulo 854), then 0x205210 when
 * mask 16 is held under second argument 0. Assumed: the words are signed ints (the
 * remainders are signed) and 0x205210 takes no arguments. The original's signed
 * `% 0x10000` adjusts a negative remainder with `addiu v0,v0,0`, so target and host
 * results differ for negative sums; the host test stays non-negative. */
struct DotrPadSteps {
    unsigned char unknown_00[0x5c];
    int unknown_5c;
    int unknown_60;
    int unknown_64;
    unsigned char unknown_68[8];
    int unknown_70;
};
extern struct DotrPadSteps gSzDuel_dt;
extern void SzDebug_OutputFrameBuffer(void);
void SzDuel_ActCardTest(void);

/* 0x1c2540: two halfwords of the control block at 0x317490 (+0xc86, +0xc88) move by a
 * signed two-bit step (bits 0-1 and 2-3 of byte +0xd64, times 0x200 and 0x800), are
 * kept in 0..0x8000, and the step is negated at either end; a non-zero byte +0xd63
 * first sets the first step to 1. Then, only when SzGame_ActMemCardProc returns a negative
 * value, lSzDuel_ActTradeProc(0), and lSzDuel_ActTradeProc(1) when that returns 0. Assumed: the
 * bit-field layout (the code only shows sign-extended two-bit reads and masked writes), that
 * lSzDuel_ActTradeProc (0x1c2720, defined in trade_proc) takes one int and returns int, and
 * SzGame_ActMemCardProc (0x1ab1d0) as in movie_operations.
 * data_00317490_drift is a view of the storage of data_00317490_control (a cast of
 * that array does not give the absolute first reads the original has); +0xc88 is
 * also data_00318118_tint. */
struct DotrControlDrift {
    unsigned char unknown_000[0xc86];
    unsigned short unknown_c86;
    unsigned short unknown_c88;
    unsigned char unknown_c8a[0xd9];
    unsigned char restart;
    signed char step_c86 : 2;
    signed char step_c88 : 2;
};
extern struct DotrControlDrift data_00317490_drift;
extern int SzGame_ActMemCardProc(void);
extern int lSzDuel_ActTradeProc(int which);
void SzDuel_ActTrade(void);

#endif
