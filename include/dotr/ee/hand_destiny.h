#ifndef DOTR_EE_HAND_DESTINY_H
#define DOTR_EE_HAND_DESTINY_H

/* TaHand_IsDestinyCondition (0x26efd0, hand.c in the Japanese build), which reads the duel
 * state only through accessor functions.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* Effect entry returned by Ta_GetUnitEffect; layout and member names from the
 * Japanese build's debug information (unnamed tag there). */
struct jp_88243 {
    unsigned int dwID;
    unsigned int dwParam;
};

/* Assumed prototypes as the state_records, state_grid, state_queries,
 * small_misc and resource_support headers declare them. SearchUnitByRange
 * (0x24ca40): six integer arguments are passed and the result is used as a
 * unit list. Ta_GetUnitEffect is declared with the unsigned
 * pointer result state_queries.h declares and cast at the use. SzDuel_GetLP
 * takes the side here, as in effect_seqs.h. func_0019a5c8 (library block, no
 * name placed) returns the next pseudo-random number. */
extern int SzDuel_GetTurnOwner(void);
extern int SzDuel_GetPlayer(int side);
extern int SearchUnitByRange(int, int, int, int, int, int);
extern int Ta_GetUnitOwnerIDFromLstCount(int flagList, int n);
extern int Ta_IsUnitControl(int a, int b);
extern int SzDuel_GetUnitKind(int side, int pos);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int Ta_GetUnitNumFrom22bitFlagList(unsigned flagList);
extern int CheckLandAgreement2(int field, int side, int pos);
extern int TaUtil_GetUnitAP(int side, int pos, int col, int row);
extern int SzDuel_GetField(int x, int y);
extern unsigned *Ta_GetUnitEffect(int side, int pos, int effInd);
extern int SzDuel_InqHand(int side, int position);
extern int TaHand_GetCardAPDP(int id);
extern int SzDuel_GetLP(int side);
extern int SzDuel_Kind2Type(int kind);
extern int func_0019a5c8(void);

int TaHand_IsDestinyCondition(int nSide);

#endif
