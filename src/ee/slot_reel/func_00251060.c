#include "dotr/ee/slot_reel.h"

/* Locals are the Japanese build's, in the debug-information order. */
int TaSlot_SetSlotInitialDat(int nSide, struct jp_9a20 *pSlotOrder)
{
    unsigned short wGraveLst[120];
    struct jp_8d5b *pConstInfo;
    int bRank;
    int bKind;
    int nCardNo;
    int Count;
    int i;

    if (pSlotOrder == 0) {
        return -1;
    }
    for (i = 0; i < 120; i++) {
        wGraveLst[i] = 999;
    }
    SzDuel_AddGrave(nSide, SzData_GetLeader(nSide, SzData_GetCurrentDeck(nSide)));
    Count = SzDuel_GetGraveCount(nSide);
    if (Count <= 0) {
        return 0;
    }
    for (i = 0; i < Count; i++) {
        pConstInfo = SzDuel_GetCardConstInfo(SzDuel_InqGrave(nSide, i));
        if (!pConstInfo->fGetSlot) {
            SzDuel_DelGrave(nSide, i);
        }
    }
    SzDuel_PackGrave(nSide);
    Count = SzDuel_GetGraveCount(nSide);
    if (Count <= 0) {
        return 0;
    }
    for (i = 0; i < Count; i++) {
        wGraveLst[i] = SzDuel_InqGrave(nSide, i);
    }
    pSlotOrder->bCardMax = TaSlot_SetReelData(Count, wGraveLst, pSlotOrder);
    pSlotOrder->bMode = 2;
    if (SzDuel_InqDestinyFlag(nSide == 0 ? 1 : 0) & 0x80) {
        pSlotOrder->bMode = 0;
    }
    nCardNo = SzData_GetLeader(nSide == 0 ? 1 : 0, SzData_GetCurrentDeck(nSide == 0 ? 1 : 0));
    bKind = TaHand_GetCardKind(nCardNo) & 0x1F;
    bRank = SzData_GetRank(nSide == 0 ? 1 : 0, nCardNo);
    for (i = 0; i < 5; i++) {
        pSlotOrder->wLine[i] = 0xFFFE;
    }
    switch (bKind) {
    case 0:
    case 2:
    case 5:
    case 6:
    case 7:
    case 9:
    case 12:
    case 16:
    case 19:
    case 20:
        pSlotOrder->wLine[0] = 0xFFFE;
        for (i = 1; i < 5; i++) {
            pSlotOrder->wLine[i] = 0xFFFF;
        }
        break;
    default:
        if (bRank <= 0) {
            pSlotOrder->wLine[0] = 0xFFFF;
        }
        if (bRank < 4) {
            pSlotOrder->wLine[1] = 0xFFFF;
        }
        if (bRank < 7) {
            pSlotOrder->wLine[2] = 0xFFFF;
        }
        if (bRank < 10) {
            pSlotOrder->wLine[3] = 0xFFFF;
        }
        if (bRank < 11) {
            pSlotOrder->wLine[4] = 0xFFFF;
        }
        break;
    }
    return 1;
}
