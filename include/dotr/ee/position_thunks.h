#ifndef DOTR_EE_POSITION_THUNKS_H
#define DOTR_EE_POSITION_THUNKS_H

/* Seven AI rule functions (below the library block), one per cluster, that consist of a tail
 * jump to 0x24e410 with the argument registers untouched.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* Ta_GetSommonPiecePreference (summon_cells): two int arguments and an int result, as its own source
 * declares them; the thunks themselves do not show the signature. */
extern int Ta_GetSommonPiecePreference(int a, int direction);

/* Each is `j 0x24e410` with an empty delay slot: whatever is in the argument
 * registers is passed on and the callee returns to the caller. Assumed: the
 * two int parameters and the int result. */
int TaLogic_GetSommonPiecePreference__0010ed90(int side, int slot);
int TaLogic_GetSommonPiecePreference__0011a4c0(int side, int slot);
int TaLogic_GetSommonPiecePreference__0011f530(int side, int slot);
int TaLogic_GetSommonPiecePreference__00124f50(int side, int slot);
int TaLogic_GetSommonPiecePreference__0014da30(int side, int slot);
int TaLogic_GetSommonPiecePreference__001604a0(int side, int slot);
int TaLogic_GetSommonPiecePreference__00170610(int side, int slot);

#endif
