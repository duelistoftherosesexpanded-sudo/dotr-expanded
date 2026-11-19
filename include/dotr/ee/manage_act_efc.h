#ifndef DOTR_EE_MANAGE_ACT_EFC_H
#define DOTR_EE_MANAGE_ACT_EFC_H

/* The effect dispatcher Ta_Manage_ActEfc, which turns a card effect's id and parameter words
 * into an effect call record and a kind pair.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

#ifndef NULL
#define NULL 0
#endif

/* Views of the effect's id and parameter words, the effect call record it fills and the kind
 * pair it fills. Member names follow the Japanese build's debug information. Assumed: these
 * meanings. */
typedef struct DotrActEfcInfo {
    unsigned int dwID;
    unsigned int dwParam;
} DotrActEfcInfo;

typedef struct DotrActEfcEffect {
    int eNO;
    unsigned int dwFrom;
    unsigned int dwTo;
    unsigned int dwPar;
} DotrActEfcEffect;

typedef struct DotrActEfcKind {
    unsigned char bType;
    unsigned char bKind;
} DotrActEfcKind;

/* Callees. SzDuel_SetUnitDelayCount and SzDuel_GetUnitCardNo are declared inside the
 * function, without a prototype. */
extern int SzDuel_GetTurnOwner(void);
extern int SzDuel_GetUnitKind(int side, int pos);
extern void *SzDuel_GetUnitEffect(int side, int pos, int effInd);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int ConvUnitIDtoFlagList(unsigned int flagList, int teamId, int unitArrayPos);
extern int ConvUnitIDtoFlagList_OFF(unsigned int flagList, int side, int pos);
extern int SearchUnitByRange(int col, int row, int side, int compMode, int searchMode, int range);
extern int Ta_GetEOwnerSide(int side, int arg1);
extern int Ta_GetCurrentSide(int side, int pos);
extern int Ta_GetSummonablePiece(int arg0, int arg1);
extern int Ta_GetUnitNum(int side, unsigned char arg1);
extern int Ta_GetUnitNumFrom22bitFlagList(int flagList);
extern int Ta_GetAgreeOwnerIDon22bitFlagLst(int ownerId, unsigned int flagList);
extern int Ta_GetRandomUnit(int flagList);
extern int Ta_GetMinimumOrderUnitFromFlagList(int flagList);
extern int Ta_GetMaximumOrderUnitFromFlagList(int mask);
extern int Ta_IsUnitControl(int arg0, int arg1);
extern int TaUtil_GetGraveCardNum(int side, int arg1);
extern void TaUtil_DoOpen(int side1, int pos2, int arg2);
extern int Ta_SearchUncTypeNaturalEffectUserNum(int arg0);
extern int Ta_IsActiveTrapB(int side, int trapEffect);
extern int TaLogic_GetGraveCardCount(int side, int arg1, int arg2);

/* 0x263930 Ta_Manage_ActEfc(info, side, id, eSide, eID, x, z, effect, kind, heap): switches on
 * the effect id's high byte (and the parameter word) to pick the units concerned (searches,
 * random or ordered picks, grave counts, traps) and fills the effect call record and the kind
 * pair; returns 1 when an effect is to be run, 0 otherwise. */
int Ta_Manage_ActEfc(DotrActEfcInfo *eInfo, int nSide, int nID, int eSide, int eID, int nX, int nZ,
                     DotrActEfcEffect *Effect, DotrActEfcKind *pInfo, unsigned char bHeap);

#endif
