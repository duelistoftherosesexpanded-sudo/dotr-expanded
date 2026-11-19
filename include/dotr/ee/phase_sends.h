#ifndef DOTR_EE_PHASE_SENDS_H
#define DOTR_EE_PHASE_SENDS_H

/* Routine 0x23bb00 that picks three values by the signed byte at 0x34f700, sends them through
 * 0x1a8320 and 0x214990, and advances that byte.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* Signed bytes, from the lb/sb accesses. */
extern signed char Rensyu_flg;
extern signed char Rensyu_END;

/* Defined elsewhere (menu_set.c, movie_operations); declared here for this caller. */
extern int SzGame_SetMenuNum(int index, int value);
extern void SzGame_Goto(int arg0, int arg1);
extern void SzGame_Push(int arg0, int arg1);
/* Two integer arguments, result unused: assumed from this call. */
extern void SzData_SetupFromConstDeck(int side, int deckIndex);
void Taku_Camp_Go_Rensyu_Duel(int flag);

#endif
