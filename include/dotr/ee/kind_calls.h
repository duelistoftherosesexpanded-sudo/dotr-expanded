#ifndef DOTR_EE_KIND_CALLS_H
#define DOTR_EE_KIND_CALLS_H

/* lSzDuel_DrawCustomDuel_Menu_Pipe (0x1c8590), which picks a callee by the kind byte found
 * at displacement 0xa9c002 from an index argument.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* The original reads the kind byte with the index register as the base and 0xa9c002 as a
 * large displacement (lui at / addu at,index,at / lbu lo(at)). MATCHING DEVICE: the C applies
 * a constant index to the argument used as a pointer; a symbol indexed by the argument, or a
 * literal pointer plus the index, compiles to a different form. The host build uses an array
 * instead. Whether 0xa9c002 is an object or a member of a larger one starting lower is not
 * known. */
#ifdef __MWERKS__
#define DOTR_KIND_BYTE(index) (((unsigned char *)(index))[0xa9c002])
#else
extern unsigned char dotr_kind_bytes_00a9c002[16];
#define DOTR_KIND_BYTE(index) (dotr_kind_bytes_00a9c002[index])
#endif

/* Callees: four arguments forwarded and an int result, from the call sites. */
extern int lSzDuel_DrawCustomDuel_Menu_Top_Pipe(int a, int index, int c, int d);
extern int lSzDuel_DrawCustomDuel_Menu_SelectDeck_Pipe(int a, int index, int c, int d);
extern int lSzDuel_DrawCustomDuel_Menu_LifeSet_Pipe(int a, int index, int c, int d);
extern int lSzDuel_DrawCustomDuel_Menu_SummonCharge_Pipe(int a, int index, int c, int d);
extern int lSzDuel_DrawCustomDuel_Menu_LeaderAbility_Pipe(int a, int index, int c, int d);
extern int lSzDuel_DrawCustomDuel_Menu_CardDisp_Pipe(int a, int index, int c, int d);
extern int lSzDuel_DrawCustomDuel_Menu_SelectMapEdit_Pipe(int a, int index, int c, int d);

/* The result replaces the first argument, which is returned unchanged for kinds 0x18-0x1f. */
extern int lSzDuel_DrawCustomDuel_Menu_Pipe(int a, int index, int c, int d);

#endif
