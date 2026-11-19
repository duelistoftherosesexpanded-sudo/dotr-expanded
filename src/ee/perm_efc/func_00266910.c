#include "dotr/ee/perm_efc.h"

int Ta_ManageNaturalEffectInStandbyPhase2(int nSide, int nID, struct jp_65da1 *eInfo)
{
    int i;
    int eObj;
    int Num;
    int Lst;
    int Ofs;
    int id;
    int side;
    int owner;
    struct jp_65648 *temp;

    temp = Ta_GetUnitEffect(nSide, nID, 2);
    Ofs = temp->dwID;
    Lst = 0;
    switch ((unsigned int)Ofs >> 8) {
    case 23: /* spellbind */
        eObj = SearchUnitByRange(0, 0, Ta_GetCurrentSide(nSide, nID), ((unsigned int)Ofs >> 6) & 3, Ofs & 0x3F, 14);
        if (eObj == 0) {
            return 0;
        }
        Num = Ta_GetUnitNumFrom22bitFlagList(eObj);
        for (i = 0; i < Num; i++) {
            owner = Ta_GetUnitOwnerIDFromLstCount(eObj, i);
            side = owner >> 8;
            id = owner & 0xFF;
            if (SzDuel_GetUnitSpellRest(side, id) == 0) {
                Lst = ConvUnitIDtoFlagList(Lst, side, id);
            }
        }
        if (Ta_GetUnitNumFrom22bitFlagList(Lst) == 0) {
            return 0;
        }
        eInfo->eNO = 0;
        eInfo->dwFrom = 1;
        eInfo->dwTo = Lst;
        eInfo->dwPar = temp->dwParam & 0xFFFF;
        return 1;
    }
    return 0;
}
