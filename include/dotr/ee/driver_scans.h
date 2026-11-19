#ifndef DOTR_EE_DRIVER_SCANS_H
#define DOTR_EE_DRIVER_SCANS_H

/* Functions at 0x27bbc0-0x27c670 that build or walk the unit control list (TaLogic_GetUnitCtlLst)
 * and test attackers against a board cell; original file driver.c by the Japanese build's debug
 * information.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */
/* Callee prototypes are assumed, except TaLogic_GetUnitCtlLst's, which
 * comes from the Japanese build's debug information. The unit pointer is opaque here. */
extern void *SzDuel_GetUnit(int side, int pos);
extern int Ta_IsUnitControl(int a, int b);
extern void TaLogic_GetUnitCtlLst(int nSide, unsigned short *wUnitLst, int sKind, unsigned char fView);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int TaLogic_GetUnitCtlLstLen(unsigned short *units);
extern unsigned GetMoveRev(void *item, int x, int y);
extern int TaLogic_IsPassRoot(int side, int pos, int col, int row, int destCol, int destRow, unsigned short *atkTargetPtr);
extern int TaLogic_IsUnitWin(int unit1OwnerId, int unit2, int col, int row);
extern int SzDuel_GetUnitCurseRest(int side, int pos);
extern int SzDuel_GetUnitSpellRest(int side, int pos);
extern int TaLogic_IsSafeTrap(int ownerId, int cx, int cy);

/* 0x27bbc0. Prototype and local names from the Japanese build's debug information. */
int TaLogic_IsAttackedNow(int nSide, int nID, unsigned char fView);
/* 0x27be10. No debug information: parameters assumed from the body (the cell replaces
 * the unit's own location), locals named after 0x27bbc0. */
int TaLogic_IsAttackedNow2(int nSide, int nID, int x, int y, unsigned char fView);

/* TaLogic_DoAdjustLogicData (0x275e80): prototype, the locals tSide and i and the structure
 * layouts from the Japanese build's debug information (driver.c). The three TaLogic callees
 * are declared in small_misc.h under offset-view parameter types for the same task table;
 * here they take the original structure. Other callee prototypes are assumed. */
struct jp_9c0ff {
    float x;
    float y;
    float z;
    float w;
};
struct jp_9b17b {
    struct jp_9c0ff pntCP;
    struct jp_9c0ff vecSP;
};
struct jp_9add8 {
    unsigned short wUnit;
    unsigned short wOrder;
    unsigned short wPurpose;
    unsigned int dwGoal;
    unsigned char bCond : 2;
};
struct jp_9b352 {
    struct jp_9add8 nTask[22];
    unsigned short wPadQue[32];
    unsigned char bCount : 5;
    unsigned char bActionUnit : 4;
    unsigned short wPhase;
    unsigned short wTime;
    struct jp_9b17b camDat;
};
extern int SzDuel_GetTurnOwner(void);
extern int TaTuto_IsTuto(int side);
extern int Ta_GetCurrentSide(int side, int pos);
extern int SzDuel_GetUnitSummonOrder(int side, int pos);
extern int TaLogic_IsUnitAssign(unsigned short ownerId, struct jp_9b352 *pCtl);
extern int TaLogic_DoUnitAssign(unsigned short ownerId, struct jp_9b352 *pCtl);
extern void TaLogic_DoSortTaskList(struct jp_9b352 *pCtl);
int TaLogic_DoAdjustLogicData(struct jp_9b352 *pCtl);

/* TaLogic_GetNearCard2 (0x276f10): prototype, local names and struct jp_9b50e from the Japanese
 * build's debug information (driver.c); the locals are declared in the reverse of the debug
 * order. struct jp_9a533 and struct PieceTreetag are only passed on and stay opaque here.
 * TaLogic_GetNearUnitL3's prototype is the debug information of 0x277160 (driver_near.h);
 * TaLogic_GetNearUnitL2 and TaLogic_InitIgnoreLst are declared in small_misc.h under other
 * assumed types; their parameters here are assumed from this caller. */
struct jp_9a533;
struct PieceTreetag;
struct jp_9b50e {
    unsigned short nUnitID;
    unsigned char bDist;
};
extern unsigned int SzGame_GetHSyncCount(void);
extern void TaLogic_InitIgnoreLst(unsigned short *wIgnoreLst);
extern struct PieceTreetag *TaLogic_GetNearUnitL2(struct PieceTreetag *pTree, struct jp_9a533 *pVarInfo, int nX, int nZ,
                                                  int nSide, unsigned char bChase, int fView, int bWarnLvl,
                                                  struct jp_9b50e *nUnitLst, unsigned short *wUnitNum,
                                                  unsigned short *wSize, unsigned short *wNodeCount,
                                                  unsigned short *wIgnoreLst);
extern int TaLogic_GetNearUnitL3(struct jp_9a533 *pVarInfo, int nX, int nZ, struct jp_9b50e *nUnitLst,
                                 unsigned char bChase, int sKey, unsigned char fOpen, unsigned char fDef,
                                 unsigned char fView, int bWarnLvl, unsigned short wUnitNum);
int TaLogic_GetNearCard2(struct jp_9a533 *pVarInfo, int nX, int nZ, int nSide, unsigned char bChase, int sKind,
                         int fOpen, int fDef, int fView, int bWarnLvl);

/* TaLogic_GetRoot2 (0x2792c0) and TaLogic_GetInvadeRoot2 (0x27a130): prototypes and local names
 * from the Japanese build's debug information (driver.c); pTree is declared first and the other
 * locals in the reverse of the debug order. struct jp_9b2bd is only passed on and stays opaque.
 * TaLogic_GetRootTree2's prototype is the debug information of 0x2794e0 (its definition in
 * driver_tree.h has a prototype of its own, with 32-bit word parameters);
 * TaLogic_GetInvadeRootTree2 (0x27a360) is declared as this caller passes it (driver_tree.h holds
 * the definition's prototype, from the same debug information).
 * TaLogic_GetShortRoot and TaLogic_IsConnectRoot are declared in small_misc.h under other
 * assumed types; here they take the tree pointer. */
struct jp_9b2bd;
extern int TaLogic_GetShortRoot(struct jp_9b2bd *pMovePiece, struct PieceTreetag *pTree, int gX, int gZ);
extern int TaLogic_IsConnectRoot(struct PieceTreetag *pTree, unsigned char bDepth, unsigned char bDist);
extern struct PieceTreetag *TaLogic_GetRootTree2(struct PieceTreetag *pTree, struct PieceTreetag *pPrev,
                                                 struct jp_9a533 *pVarInfo, int nX, int nZ, int gX, int gZ,
                                                 unsigned char bDist, unsigned char *EndCond,
                                                 unsigned short *gUnitID, unsigned char bDepth,
                                                 unsigned short *wSize, unsigned short *wNodeCount,
                                                 unsigned short *wIgnoreLst);
extern struct PieceTreetag *TaLogic_GetInvadeRootTree2(struct PieceTreetag *pTree, struct PieceTreetag *pPrev,
                                                       struct jp_9a533 *pVarInfo, int nX, int nZ, int gX, int gZ,
                                                       unsigned char bDist, unsigned char *EndCond, int *gUnitID,
                                                       int fView, unsigned short bWarnLvl, unsigned char bDepth,
                                                       unsigned short *wSize, unsigned short *wNodeCount,
                                                       unsigned short *wIgnoreLst);
int TaLogic_GetRoot2(struct jp_9a533 *pVarInfo, struct jp_9b2bd *pMovePiece, int nX, int nZ, int gX, int gZ,
                     unsigned short gUnitID, unsigned char bAbort);
int TaLogic_GetInvadeRoot2(struct jp_9a533 *pVarInfo, struct jp_9b2bd *pMovePiece, int nX, int nZ, int gX, int gZ,
                           int gUnitID, int fView, unsigned char bAbort, unsigned short bWarnLvl);

/* TaLogic_GetFusionTree (0x278a90): prototype, local names and struct TreeTag from the Japanese
 * build's debug information; the locals are declared in the reverse of the debug order. Res is
 * given the fused root number here (an assumption: the debug information only names it). The
 * p_fTree argument is not read. Callee signatures are assumed from this caller; the callees are
 * declared in other groups under assumed types. */
struct TreeTag {
    unsigned short nRoot;
    unsigned char nComboLink[5];
    struct TreeTag *pNextLink[4];
};
extern void TaHand_GetNextHand(int nSide, int *arNextHand);
extern int TaHand_IsCardSummon2(int nSide, int nCardNo);
extern int TaLogic_GetHandFusion(unsigned short cardNo, unsigned short equipNo);
extern struct TreeTag *TaLogic_TreeAloc(unsigned short *wSize, unsigned short *wCount);
extern void TaLogic_InitTree(struct TreeTag *pTree, unsigned short nRoot, unsigned char *pCombo, unsigned char nOrder);
extern int TaLogic_GetArrayLength(unsigned char *array);
extern int TaLogic_IsUseHandInCombo(unsigned char handSlot, unsigned char *combo);
struct TreeTag *TaLogic_GetFusionTree(int nSide, struct TreeTag *p_fTree, unsigned char nOrder, unsigned short pRoot,
                                      unsigned char *pCombo, unsigned short *wSize, unsigned short *wCount);

/* TaLogic_DoGetShortRoot (0x27acb0): prototype, local names and the two structure layouts from the
 * Japanese build's debug information (driver.c). TaLogic_GetNextPiece is declared in small_misc.h
 * under an assumed pointer type; here it takes the tree node. Its result is kept in an unsigned
 * char, so the negative test in the source can never hold. */
struct jp_9b2bd {
    unsigned char nX : 4;
    unsigned char nZ : 4;
};
struct PieceTreetag {
    unsigned char bPiece;
    struct PieceTreetag *pNextP[12];
    struct PieceTreetag *pPrevP;
};
int TaLogic_GetNextPiece(struct PieceTreetag *tree, unsigned char depth);
void TaLogic_DoGetShortRoot(struct jp_9b2bd *pMovePiece, int *Count, unsigned char *Dist, struct PieceTreetag *pTree,
                            int gX, int gZ, struct jp_9b2bd *p1stLoc, unsigned char bDepth);

/* TaLogic_GetFollowNearUnit (0x2774f0): prototype, struct jp_9aeb0 and the local names v1, pInfo,
 * Dist, Loc and i from the Japanese build's debug information (driver.c). The callee prototypes
 * are the assumed ones of their own groups; SzDuel_GetCardConstInfo's result type is assumed
 * from the bDeckCost read. */
struct jp_9aeb0 {
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
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern struct jp_9aeb0 *SzDuel_GetCardConstInfo(int cardNo);
extern int TaLogic_IsStatusMatch(int actualStatus, int targetStatus);
extern int TaLogic_IsUnitCategory(int nSide, int nID, int sKind);
int TaLogic_GetFollowNearUnit(int nX, int nZ, struct jp_9b50e *nUnitLst, unsigned char bChase, int sKey,
                              unsigned char fOpen, unsigned char fDef, unsigned short wUnitNum);

/* 0x130cb0: no parameters observed; the three counters are 8-bit (andi 0xff); int result 0 or 1. */
int TaLogic007_IsActiveMode(void);

#endif
