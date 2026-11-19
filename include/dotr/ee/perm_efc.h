#ifndef DOTR_EE_PERM_EFC_H
#define DOTR_EE_PERM_EFC_H

/* Functions of the original file PermEfc.c (by the Japanese build's debug information) that
 * read a unit's variable record and the standing effects of the units on both sides:
 * Ta_GetParamOfs at 0x2660e0.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* Member names and offsets from the Japanese build's debug information (PermEfc.c); the tags
 * are unnamed there. struct jp_65767 (the card request, 0xa0 bytes) is not used here. */
struct jp_6537d {
    unsigned char bKind;
    unsigned char bKindOfs;
    unsigned char bAttr : 4;
    unsigned char bLevel : 4;
    unsigned char bDeckCost;
    unsigned short wEffIdx;
    unsigned short wEffNo;
    unsigned short wAP : 13;
    unsigned short fPermanent : 1;
    unsigned short fGetPasswd : 1;
    unsigned short fGetRecycle : 1;
    unsigned short wDP : 13;
    unsigned short fGetSlot : 1;
    unsigned short fGetFever : 1;
    unsigned short fAlternate : 1;
    unsigned char abPasswd[8];
};

struct jp_65648 {
    unsigned int dwID;
    unsigned int dwParam;
};

struct jp_65767;

struct jp_64b6d {
    struct jp_6537d *pConstInfo;
    struct jp_65767 *pCardReq;
    struct jp_65648 aVarEffIdx[5];
    int nRotY;
    short sOfsX;
    short sOfsZ;
    short sVarAP;
    short sVarDP;
    unsigned short wCardNo;
    unsigned char bVarKind;
    unsigned char bVarAttr;
    unsigned char bMapX;
    unsigned char bMapZ;
    unsigned char bPosX;
    unsigned char bPosZ;
    unsigned char bOwner;
    unsigned char bID;
    unsigned char bModelID;
    unsigned char bSummonOrder;
    unsigned char bSummonCost;
    unsigned char bCurseRest;
    unsigned char bSpellRest;
    unsigned char bDelayCount;
    unsigned char bMovable;
    unsigned char bDarkRate;
    unsigned char bCtlModeC;
    unsigned char bCtlModeM;
    unsigned char bCtlGoalC;
    unsigned char bCtlGoalM;
    unsigned char bCtlRateC;
    unsigned char bCtlRateM;
    unsigned char bOpenRate;
    unsigned char bDefenseRate;
    int nCtlFlipC : 2;
    int nCtlFlipM : 2;
    int nOpenFlip : 2;
    int nDefenseFlip : 2;
    int fFinish : 1;
    int fRevolt : 1;
    int fOpen : 1;
    int fDefense : 1;
    int fOpenF : 1;
    int fDefenseF : 1;
    int fPickup : 1;
    int fCardBusy : 1;
    int fModelBusy : 1;
    int fCardColBusy : 1;
    int fHand : 1;
    int fCurse : 1;
    int nEffHandle : 6;
};

/* Ta_GetParamOfs (0x2660e0): prototype and local names from the Japanese build's debug
 * information. Callee prototypes are assumed (from the argument registers and how results
 * are used) under this group's types; Ta_GetUnitAgreeWithVarInfo is declared as this caller uses it. */
extern int Ta_IsUnitControl2(struct jp_64b6d *vInfo);
extern int SzDuel_GetUnitDP(int side, int pos, int ignoreBoosts);
extern int SzDuel_GetUnitAP(int side, int pos, int ignoreBoosts);
extern struct jp_64b6d *SzDuel_GetUnit(int side, int pos);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern struct jp_65648 *Ta_GetUnitEffect(int side, int pos, int effInd);
extern int Ta_IsUnitControl(int a, int b);
extern int Ta_GetUnitAgreeWithVarInfo(struct jp_64b6d *vInfo, int side, int a, int b);
extern int SzDuel_GetField(int x, int y);
extern int CheckLandAgreement(int land, struct jp_64b6d *vInfo);
extern int Ta_GetLDParamOfs2(struct jp_64b6d *vInfo, int col, int row);
int Ta_GetParamOfs(struct jp_64b6d *vInfo, int nX, int nZ, int fAP);

/* GetMoveRev (0x2464f0, original file SetMoveArea.c): prototype and local names from the Japanese
 * build's debug information; its jp_47 / jp_142e have the layouts of jp_64b6d / jp_65648 above,
 * which are used here. The three callee prototypes are assumed from the argument registers alone. */
extern int Ta_IsLDAbl(int ability, int side);
extern int Ta_SearchUncTypeNaturalEffectUser(int kind, int n);
extern int Ta_IsLDRangeAvilityFromLoc(int packed, int side, int kind, int ability);
unsigned int GetMoveRev(struct jp_64b6d *vInfo, int nX, int nZ);

/* Ta_ManageNaturalEffectInStandbyPhase (0x266500): prototype, local names and struct jp_65da1
 * from the Japanese build's debug information. The callee prototypes are assumed (from argument
 * registers and how the results are used); SearchUnitByRange is declared as this caller uses it. */
struct jp_65da1 {
    int eNO;
    unsigned int dwFrom;
    unsigned int dwTo;
    unsigned int dwPar;
};
extern int Ta_GetCurrentSide(int side, int pos);
extern int SearchUnitByRange(int, int, int, int, int, int);
extern int SzDuel_GetUnitDelayCount(int side, int pos);
extern int SzDuel_SetUnitDelayCount(int side, int pos, int value);
extern unsigned ConvUnitIDtoFlagList(unsigned flagList, int teamId, int unitArrayPos);
extern int SzDuel_GetGraveCount(int side);
extern int SzDuel_InqGrave(int side, int position);
extern int TaHand_GetCardKind(int id);
extern int SzDuel_DelGrave(int side, int position);
extern void SzDuel_PackGrave(int side);
extern unsigned ConvUnitIDtoFlagList_OFF(unsigned flagList, int side, int pos);
extern int SzDuel_GetUnitLoc(int side, int pos);
int Ta_ManageNaturalEffectInStandbyPhase(int nSide, int nID, struct jp_65da1 *eInfo);
/* Callees of Ta_ManageNaturalEffectInStandbyPhase2 (0x266910), defined in small_misc and
 * state_grid; prototypes as declared there. The Japanese build's debug information has no
 * prototype for 0x266910; its parameter names are taken from its twin
 * Ta_ManageNaturalEffectInStandbyPhase. */
extern int Ta_GetUnitNumFrom22bitFlagList(unsigned flagList);
extern int Ta_GetUnitOwnerIDFromLstCount(int flagList, int n);
extern int SzDuel_GetUnitSpellRest(int side, int pos);
int Ta_ManageNaturalEffectInStandbyPhase2(int nSide, int nID, struct jp_65da1 *eInfo);

#endif
