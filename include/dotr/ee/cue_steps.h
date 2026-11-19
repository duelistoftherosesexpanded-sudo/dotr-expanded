#ifndef DOTR_EE_CUE_STEPS_H
#define DOTR_EE_CUE_STEPS_H

/* Functions that step a 16-bit phase word at +0x160 of a record between an id range and the
 * markers 0xffff / 0xfffe, sending cues through SzSound_PlaySE (TaTuto_OpenTutoMsg, 0x27fc20).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* The fields TaTuto_OpenTutoMsg touches; the record's size and owner are not established. */
struct DotrCueStepRecord {
    unsigned char unknown_000[0x160];
    unsigned short unknown_160;
    unsigned char unknown_162[0x40];
    unsigned short unknown_1a2;
    unsigned short unknown_1a4;
};

/* Callees defined in state_queries (TaTuto_OpenMsgPopup 0x27fb30 and TaTuto_CloseMsgPopup
 * 0x27fba0: their first parameter is `void *` there and is only passed on; here the caller
 * passes an id), side_table (TaTuto_GetCardNameMsgID 0x27fda0, called here without setting an
 * argument), state_grid (SzDuel_OpenMsgPopup), small_state (SzPad_Check, with the prototype
 * state_grid.h declares) and sound_gate (SzSound_PlaySE). All are stubbed in this group's
 * tests. */
extern int func_0027fb30_id(int id, unsigned short *p);
extern int func_0027fba0_id(int id, unsigned short *p);
extern int func_0027fda0_void(void);
extern void SzDuel_OpenMsgPopup(int operation, int a, int b, int value);
extern int SzPad_Check(int unknown, int zero, int mask);
extern int SzSound_PlaySE(int index, int soundEffectNumber);

/* first and last are compared with the 16-bit phase word as ints. */
extern int TaTuto_OpenTutoMsg(int first, int last, struct DotrCueStepRecord *rec);

#endif
