#ifndef DOTR_EE_KIND_SENDS_H
#define DOTR_EE_KIND_SENDS_H

/* Functions below 0x17b48c that, for a kind number, build a position from a side's block
 * (SzDuel_GetUnit, SzEffect_GetModelRot2) and send it with SzView_ReqCamera.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* Callee signatures are assumed views taken from the call sites. */
extern int SzView_FreeRequest(int a, int b);
/* Returns a side's block (struct DotrStateItem in state_records.h); read here only as
 * words, of which the one at +48 is used. */
extern void *SzDuel_GetUnit(int side, int pos);
/* Fills four words for a packed cell; only the first three are changed by the callers here. */
extern int func_001f6160_words(int packed, int *words);
extern void SzView_IV2V(int *in, float *out);
extern float SzEffect_GetRadius2(int packed);
extern int SzDuel_GetUnitModelID(int side, int pos);
extern void func_001fa8a0_start(int a, int b, int c, int d, int index, void *data);

void TaTuto_FocusUnit__00175ba0(int kind);
/* Same shape as 0x175ba0 with other kinds and unit slots; signature assumed as for it. */
void TaTuto_FocusUnit__00172510(int kind);
void TaTuto_FocusUnit__001745f0(int kind);

#endif
