#ifndef DOTR_EE_ACTION_REACTION_H
#define DOTR_EE_ACTION_REACTION_H

/* TaLogic_GetActionReaction: classifies how a card play is answered by traps (trap B decision,
 * trap A check, natural effect 14).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* Layout and member names from the Japanese build's debug information for
 * Chk_TrapA's fifth parameter (the tag is unnamed there). */
struct jp_7b52 {
    int eNO;
    unsigned int dwFrom;
    unsigned int dwTo;
    unsigned int dwPar;
};

extern int SzDuel_GetUnitLoc(int side, int pos);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int Ta_IsUnitControl(int a, int b);
extern int Ta_SearchUncTypeNaturalEffectUserNum(int kind);
extern int Ta_IsActiveTrapB(int side, int trapEffect);
/* The definition takes `unsigned char IsView` (slot_seeks.h); this caller passes the
 * fourth argument without narrowing it, as if no prototype was in scope. */
extern int Ta_IsTrapBInDecideOperation2(int nSide, int nID, int stDefense, int IsView);
/* Prototype from the Japanese build's debug information; Chk_TrapA is defined in the trap_a group. */
extern int Chk_TrapA(int nSide, int nID, int nX, int nZ, struct jp_7b52 *eInfo, int PrNo);

/* Parameters: the acting unit (side1, pos1) and the target (side2, pos2); the fifth selects
 * the play kind and the sixth is used as a byte flag. */
int TaLogic_GetActionReaction(int side1, int pos1, int side2, int pos2, unsigned char n, int last);

#endif
