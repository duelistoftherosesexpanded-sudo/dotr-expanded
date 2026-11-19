#ifndef DOTR_EE_HAND_COMBO_H
#define DOTR_EE_HAND_COMBO_H

/* hand.c functions (Japanese build debug information) that read a side's hand through
 * SzDuel_InqHand and card records through SzDuel_InitCardInfo.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* The card record of hand.c (Japanese build debug information; member names and offsets are the
 * original's, the jp_<n> tags stand for types the original left unnamed). Same layout as
 * perm_efc.h's jp_64b6d; only bVarKind is read by the code here. */
struct jp_87592;
struct jp_87b56;
struct jp_88243 {
    unsigned int dwID;
    unsigned int dwParam;
};

struct jp_86d82 {
    struct jp_87592 *pConstInfo;
    struct jp_87b56 *pCardReq;
    struct jp_88243 aVarEffIdx[5];
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

/* Callee prototypes are assumed (from the argument registers and how results are used). */
extern int SzDuel_GetHandCount(int side);
extern int SzDuel_InqHand(int side, int position);
extern void SzDuel_InitCardInfo(struct jp_86d82 *unit, int cardId);
extern int Fusion(int cardA, int cardB);
extern unsigned int Do_Chk_Enc(int cNO, int eNO);

/* Ta_GetComboResult (0x26eb10, hand.c): prototype and local names from the Japanese build's
 * debug information. */
int Ta_GetComboResult(int nSide, int nCardNo, int nCardNo2);

/* Ta_SideMatch (0x24dd80): assumed prototype as small_misc.h declares it. */
extern int Ta_SideMatch(int sideA, int sideB, int compMode);

/* Ta_GetUnitAgreeWithVarInfo (0x24ecd0, util.c): prototype and local names from the Japanese
 * build's debug information; vInfo is the card record above under this group's tag. */
int Ta_GetUnitAgreeWithVarInfo(struct jp_86d82 *vInfo, int eOwner, int cSide, int cKey);

#endif
