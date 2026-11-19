#ifndef DOTR_EE_MODE_STEPS_H
#define DOTR_EE_MODE_STEPS_H

/* Step dispatchers over the halfword +0x30 of 0x34fb00 (TkMc_Save_Main and
 * TkMc_Trade_Save_Main) whose callees are defined in link_step.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* The storage at 0x34fb00 through its existing declaration (struct DotrMiscLink,
 * small_misc.h). The halfword at +0x58 is a byte array there and is read and written
 * through a short cast at the use. */
#include "dotr/ee/small_misc.h"

/* Callees not declared by small_misc.h, all defined in link_step. 0x2406c0 is
 * declared there as int(void); this call site sets a0 to mode before each call, so it
 * is declared here with one int parameter (link_step.h is not included alongside). */
extern int TakuTrade_Save_card_kansi(int mode);
extern int TakuSave_card_file_open(void);
extern int TakuSave_make_file(void);
extern int TakuTrade_Save_card_file_kakikomi_Ver2(void);

/* 0x23ca10: mode 1 first runs TakuCard_inti(1); a non-zero result sets the halfword +0x58
 * and returns 0x600, otherwise +0x2be is cleared. A non-zero +0x58 returns 0x600. Then
 * by the step +0x30 (jump table at 0x30f2a0, 15 entries): 0: TakuTrade_Save_card_kansi(mode), step 0 on
 * result 2; 1: TakuSave_card_filename_check(0); 2: TakuSave_card_file_open(); 3: TakuSave_card_kansi(0);
 * 4: TakuSave_make_file(); 5: TakuTrade_Save_card_file_kakikomi_Ver2(); 10: TakuSave_card_format(); 11
 * and 13: TakuTrade_Save_card_kansi(mode), then when +0x2be is 14 (step 11) or 1 (step 13) and mode is
 * 2: step 10 / 5, +0x34 and +0x40 cleared, result 11 / 9; 14: TakuTrade_Save_card_kansi(mode), result 2
 * becomes 10, and mode 2 clears +0x30, +0x34, +0x40 with result 0x700. Mode 1 then clears +0x34, +0x40,
 * +0x30. A non-zero result is stored in +0x2be; 256 is returned as is; otherwise +0x2be gains 0x80 when
 * the word +8 is 1 and is returned. Assumed: int(int), as its siblings 0x23c770 / 0x23c8b0. */
int TkMc_Trade_Save_Main(int mode);

/* 0x23de90 is declared in link_step as int(void); 0x23e6d0 comes from small_misc.h. */
extern int TakuSave_card_file_kakikomi(void);

/* 0x23c560: mode 1 first runs TakuCard_inti(0) and clears +0x2be. Then by the step +0x30
 * (jump table at 0x30f260, 16 entries): 0: TakuSave_card_kansi(0), step 0 on result 2;
 * 1: TakuSave_card_filename_check(0); 2: TakuSave_card_file_open(); 3: TakuSave_card_kansi(0);
 * 4: TakuSave_make_file(); 5: TakuSave_card_file_kakikomi(); 10: TakuSave_card_format();
 * 15: TakuSave_card_unformat(); 11, 13 and 14 as in 0x23ca10 but with TakuSave_card_kansi(0)
 * as the step call. Mode 1 then clears +0x34, +0x40, +0x30. A non-zero result is stored in +0x2be; 256 is
 * returned as is, otherwise +0x2be (no 0x80 flag here). The case order 10, 15, 11 is the order of the code.
 * Assumed: int(int). */
int TkMc_Save_Main(int mode);

#endif
