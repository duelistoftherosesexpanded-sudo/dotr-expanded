#ifndef DOTR_EE_DEST_EFC_H
#define DOTR_EE_DEST_EFC_H

/* Ta_Manage_DestEfc (EndBattle.c in the Japanese build): destruction-effect dispatcher over the
 * unit's effect record.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */
/* Layouts and member names from the Japanese build's debug information (EndBattle.c; the original
 * left the tags unnamed, so they keep the jp_<n> form). Callee prototypes are assumed,
 * as declared elsewhere; SearchUnitByRange takes six arguments at the call sites
 * and SzDuel_GetUnit is only tested for a null result here. */
struct jp_10241 {
    int eNO;
    unsigned int dwFrom;
    unsigned int dwTo;
    unsigned int dwPar;
};
struct jp_102e6 {
    unsigned int dwID;
    unsigned int dwParam;
};
extern void *SzDuel_GetUnit(int side, int pos);
extern struct jp_102e6 *Ta_GetUnitEffect(int side, int pos, int effInd);
extern int Ta_GetCurrentSide(int side, int pos);
extern unsigned ConvUnitIDtoFlagList(unsigned flagList, int teamId, int unitArrayPos);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int SearchUnitByRange(int, int, int, int, int, int);
extern unsigned ConvUnitIDtoFlagList_OFF(unsigned flagList, int side, int pos);
extern int Ta_GetEOwnerSide(int side, int mode);
extern int Ta_IsActiveTrapB(int side, int trapEffect);
extern int Ta_GetSummonablePiece(int a, unsigned char flag);
extern int SzDuel_GetLP(int side);
extern int Ta_GetLPRevByNaturalEffect(int lpChangeType, int lifeAmount);
extern int Ta_GetUnitNum(int side, unsigned char kind);
extern int TaUtil_GetGraveCardNum(int side, int kind);
extern int Ta_GetSummonablePieceWithOrder(int side, int i, int avoidLabyrinth);
extern int SzDuel_GetUnitCardNo(int side, int pos);

int Ta_Manage_DestEfc(int nSide_1, int nID_1, int nSide_2, int nID_2, int nX, int nZ,
                      struct jp_10241 *eInfo, unsigned int fFlag);

#endif
