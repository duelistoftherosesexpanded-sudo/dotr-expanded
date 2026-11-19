#ifndef DOTR_EE_RECORD_CALLS_H
#define DOTR_EE_RECORD_CALLS_H

/* Functions near 0x1cecf0 that pick one of the 848-byte records at 0x317490 and act on its
 * kind byte at +0x33c.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* One 848-byte record of the array at 0x317490 (other groups view the same storage under
 * their own data_00317490_* names); only the three bytes read by lSzDuel_GetControl are
 * named. */
struct DotrCallRecord {
    unsigned char unknown_000[0x33c];
    unsigned char kind;
    unsigned char lo;
    unsigned char hi;
    unsigned char unknown_33f[0x11];
};
typedef char DotrCallRecordStrideMustBe350[(sizeof(struct DotrCallRecord) == 0x350) ? 1 : -1];
extern struct DotrCallRecord data_00317490_callrecs[];
/* Table of routines indexed by the kind byte (kinds below 100); the argument and result
 * types are assumed from the one call. */
extern int (*g_aLogFunc[])(int, struct DotrCallRecord *);
/* As declared in phase_sends.h. */
extern signed char Rensyu_flg;
extern signed char Rensyu_END;
struct DotrBlockSlot;
extern int SzPad_Check(int index, int mode, int mask);
extern int TaTuto_IsTuto(int side);
extern struct DotrBlockSlot *SzView_InqRequest(int a, int b);
extern void Taku_Camp_Go_Rensyu_Duel(int flag);
extern void SzGame_Pop(void);
/* The second argument is only passed through to SzPad_Check (register a1 is never written). */
int lSzDuel_GetControl(int side, int other);

#endif
