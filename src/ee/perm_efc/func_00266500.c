#include "dotr/ee/perm_efc.h"

int Ta_ManageNaturalEffectInStandbyPhase(int nSide, int nID, struct jp_65da1 *eInfo)
{
    int i;
    int eObj;
    int Num;
    int cNo;
    int Ofs;
    struct jp_65648 *temp;

    if ((temp = Ta_GetUnitEffect(nSide, nID, 2)) == 0) {
        return 0;
    }
    Ofs = temp->dwID;
    switch ((unsigned int)Ofs >> 8) {
    case 13: /* alter LP */
        eInfo->eNO = 8;
        eInfo->dwFrom = 0;
        eInfo->dwTo = Ta_GetCurrentSide(nSide, nID);
        eInfo->dwPar = ((temp->dwParam >> 16) << 16) | (temp->dwParam & 0xFFFF);
        return 1;
    case 77: /* weaken monsters */
        eObj = SearchUnitByRange(0, 0, Ta_GetCurrentSide(nSide, nID), ((unsigned int)Ofs >> 6) & 3, Ofs & 0x3F, 14);
        if (eObj == 0) {
            return 0;
        }
        eInfo->eNO = 57;
        eInfo->dwFrom = 0;
        eInfo->dwTo = eObj;
        eInfo->dwPar = temp->dwParam & 0xFFFF;
        return 1;
    case 78: /* buff monsters */
        eObj = SearchUnitByRange(0, 0, Ta_GetCurrentSide(nSide, nID), ((unsigned int)Ofs >> 6) & 3, Ofs & 0x3F, 14);
        if (eObj == 0) {
            return 0;
        }
        eInfo->eNO = 29;
        eInfo->dwFrom = 1;
        eInfo->dwTo = eObj;
        eInfo->dwPar = temp->dwParam & 0xFFFF;
        return 1;
    case 12: /* transform */
        if (SzDuel_GetUnitDelayCount(nSide, nID) == 1) {
            SzDuel_SetUnitDelayCount(nSide, nID, 0);
            eObj = ConvUnitIDtoFlagList(0, nSide, nID);
            eInfo->eNO = 60;
            eInfo->dwFrom = 0;
            eInfo->dwTo = eObj;
            eInfo->dwPar = temp->dwParam & 0xFFFF;
            return 1;
        }
        return 0;
    case 70: /* Jigen Bakudan */
        if (SzDuel_GetUnitDelayCount(nSide, nID) == 1) {
            SzDuel_SetUnitDelayCount(nSide, nID, 0);
            eInfo->eNO = 46;
            eInfo->dwFrom = (nSide << 8) | nID;
            eInfo->dwTo = 0;
            eInfo->dwPar = 1;
            return 1;
        }
        return 0;
    case 83: /* remove graveyard monsters and buff */
        eObj = ConvUnitIDtoFlagList(0, nSide, nID);
        Num = 0;
        cNo = SzDuel_GetGraveCount(0);
        for (i = 0; i < cNo; i++) {
            if ((TaHand_GetCardKind(SzDuel_InqGrave(0, i)) >> 5) == 0) {
                SzDuel_DelGrave(0, i);
                Num++;
            }
        }
        SzDuel_PackGrave(0);
        cNo = SzDuel_GetGraveCount(1);
        for (i = 0; i < cNo; i++) {
            if ((TaHand_GetCardKind(SzDuel_InqGrave(1, i)) >> 5) == 0) {
                SzDuel_DelGrave(1, i);
                Num++;
            }
        }
        SzDuel_PackGrave(1);
        eInfo->eNO = 29;
        eInfo->dwFrom = 1;
        eInfo->dwTo = eObj;
        eInfo->dwPar = Num * (temp->dwParam & 0xFFFF);
        return 1;
    case 84: /* move all cards towards the user */
        eObj = ConvUnitIDtoFlagList_OFF(
            SearchUnitByRange(0, 0, Ta_GetCurrentSide(nSide, nID), ((unsigned int)Ofs >> 6) & 3, Ofs & 0x3F, 14), nSide,
            nID);
        if (eObj == 0) {
            return 0;
        }
        eInfo->eNO = 72;
        eInfo->dwFrom = SzDuel_GetUnitLoc(nSide, nID);
        eInfo->dwTo = eObj;
        eInfo->dwPar = Ta_GetCurrentSide(nSide, nID);
        return 1;
    }
    return 0;
}
