#ifndef DOTR_EE_SLOT_REEL_H
#define DOTR_EE_SLOT_REEL_H

/* TaSlot_* functions of util.c (Japanese build debug information) that fill the slot order
 * record struct jp_9a20.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* Slot order record and card constants (Japanese build debug information; member names and
 * offsets are the original's, the jp_<n> tags stand for types the original left unnamed).
 * _pad is this project's name for six bytes no code reads. */
struct jp_9a20 {
    unsigned char bCardMax;
    unsigned char bMode;
    unsigned char _pad[6];
    unsigned short wReelBuf[3][21];
    unsigned short wLine[5];
};
struct jp_8d5b {
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

/* Callee prototypes are assumed (from the argument registers and how results are used);
 * TaSlot_SetReelData (0x251370) is defined in this group; its parameter names are assumed from its
 * caller TaSlot_SetSlotInitialDat. */
extern int SzData_GetCurrentDeck(int side);
extern int SzData_GetLeader(int side, int deckNo);
extern int SzDuel_AddGrave(int side, int cardNo);
extern int SzDuel_GetGraveCount(int side);
extern int SzDuel_InqGrave(int side, int position);
extern struct jp_8d5b *SzDuel_GetCardConstInfo(int cardNo);
extern int SzDuel_DelGrave(int side, int position);
extern void SzDuel_PackGrave(int side);
int TaSlot_SetReelData(int Count, unsigned short *wGraveLst, struct jp_9a20 *pSlotOrder);
extern int SzDuel_InqDestinyFlag(int side);
extern int TaHand_GetCardKind(int id);
extern int SzData_GetRank(int side, int cardNo);

/* TaSlot_SetSlotInitialDat (0x251060, util.c): prototype and local names from the Japanese
 * build's debug information. */
int TaSlot_SetSlotInitialDat(int nSide, struct jp_9a20 *pSlotOrder);

#endif
