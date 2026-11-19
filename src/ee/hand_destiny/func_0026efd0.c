#include "dotr/ee/hand_destiny.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* Rates the board for the side nSide when the turn owner's player number is
 * 100 or more: 1 to 5 name the first situation found (see
 * docs/modules/duel-rules.md), -1 none while the side has 250 life points or more, otherwise 6 or
 * 7 from the parity of the next pseudo-random number. Parameter, local and
 * structure names are from the Japanese build's debug information; the
 * declaration order below is the one that reproduces the saved registers,
 * not the recorded one. */
int TaHand_IsDestinyCondition(int nSide)
{
    int i;
    int j;
    int Unit;
    int Loc;
    int Loc2;
    int Num;
    int Num2;
    int tempAP;
    int AP;
    int temp;
    struct jp_88243 *eInfo;
    int Player;
    unsigned int eObj;

    Player = SzDuel_GetPlayer(SzDuel_GetTurnOwner());
    if (Player > 99) {
        eObj = SearchUnitByRange(0, 0, nSide, 3, 33, 14);
        Unit = Ta_GetUnitOwnerIDFromLstCount(eObj, 0);
        if (Ta_IsUnitControl(Unit >> 8, Unit & 0xff) != nSide) {
            if ((SzDuel_GetUnitKind(Unit >> 8, Unit & 0xff) == 7) & 0x1f) {
                Loc = SzDuel_GetUnitLoc(Unit >> 8, Unit & 0xff);
                Loc2 = SzDuel_GetUnitLoc(nSide, 0);
                if ((MAX(Loc & 0xff, Loc2 & 0xff) - MIN(Loc & 0xff, Loc2 & 0xff)) +
                        (MAX(Loc >> 8, Loc2 >> 8) - MIN(Loc >> 8, Loc2 >> 8)) <= 2) {
                    if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, nSide, 2, 7, 14)) >= 3) {
                        return 1;
                    }
                }
            }
        }
        eObj = SearchUnitByRange(0, 0, nSide, 2, 29, 14);
        Num = 0;
        if (Ta_GetUnitNumFrom22bitFlagList(eObj) >= 3) {
            if ((SzDuel_GetUnitKind(nSide, 0) == 9) & 0x1f) {
                for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(eObj); i++) {
                    Unit = Ta_GetUnitOwnerIDFromLstCount(eObj, i);
                    Loc = SzDuel_GetUnitLoc(Unit >> 8, Unit & 0xff);
                    if (CheckLandAgreement2(0, Unit >> 8, Unit & 0xff) != 1) {
                        if (TaUtil_GetUnitAP(Unit >> 8, Unit & 0xff, Loc & 0xff, Loc >> 8) >= 3000 &&
                            TaUtil_GetUnitAP(Unit >> 8, Unit & 0xff, Loc & 0xff, Loc >> 8) <= 3999) {
                            Num++;
                        }
                    }
                }
                if (Num >= 3) {
                    return 2;
                }
            }
        }
        eObj = SearchUnitByRange(0, 0, nSide, 3, 29, 14);
        Num2 = 0;
        Num = 0;
        if (Ta_GetUnitNumFrom22bitFlagList(eObj) >= 3) {
            for (i = 0; i < 7; i++) {
                for (j = 0; j < 7; j++) {
                    if (SzDuel_GetField(i, j) == 8) {
                        Num++;
                    }
                }
            }
            if (Num > 30) {
                i = 0;
                for (j = 0; j < Ta_GetUnitNumFrom22bitFlagList(eObj); j++) {
                    Unit = Ta_GetUnitOwnerIDFromLstCount(eObj, j);
                    eInfo = (struct jp_88243 *)Ta_GetUnitEffect(Unit >> 8, Unit & 0xff, 1);
                    if (eInfo != 0) {
                        if (eInfo->dwID >> 8 == 8 || eInfo->dwID >> 8 == 9 || eInfo->dwID >> 8 == 10) {
                            if (Ta_IsUnitControl(Unit >> 8, Unit & 0xff) == nSide) {
                                i++;
                            } else {
                                Num2++;
                            }
                        }
                    }
                }
                if (i == 0 && Num2 >= 3) {
                    return 3;
                }
            }
        }
        eObj = SearchUnitByRange(0, 0, nSide, 3, 33, 14);
        Unit = Ta_GetUnitOwnerIDFromLstCount(eObj, 0);
        if (Ta_IsUnitControl(Unit >> 8, Unit & 0xff) != nSide) {
            Loc = SzDuel_GetUnitLoc(Unit >> 8, Unit & 0xff);
            AP = -1;
            for (i = 0; i < 5; i++) {
                temp = SzDuel_InqHand(nSide, i);
                if (temp != 999) {
                    tempAP = (unsigned int)TaHand_GetCardAPDP(temp) >> 16;
                    if (tempAP > AP) {
                        AP = tempAP;
                    }
                }
            }
            if (TaUtil_GetUnitAP(Unit >> 8, Unit & 0xff, Loc & 0xff, Loc >> 8) > AP) {
                tempAP = TaUtil_GetUnitAP(Unit >> 8, Unit & 0xff, Loc & 0xff, Loc >> 8) - AP;
                if (SzDuel_GetLP(nSide == 0 ? 1 : 0) >> 1 > tempAP) {
                    return 4;
                }
            }
        }
        eObj = SearchUnitByRange(0, 0, nSide, 3, 33, 14);
        Unit = Ta_GetUnitOwnerIDFromLstCount(eObj, 0);
        Num = 0;
        if (Ta_IsUnitControl(Unit >> 8, Unit & 0xff) == nSide) {
            eObj = SearchUnitByRange(0, 0, nSide, 2, 30, 14);
            for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(eObj); i++) {
                Unit = Ta_GetUnitOwnerIDFromLstCount(eObj, i);
                if (SzDuel_Kind2Type(SzDuel_GetUnitKind(Unit >> 8, Unit & 0xff) >> 5) == 2) {
                    Num++;
                }
            }
            if (Num >= 4) {
                return 5;
            }
        }
    }
    if (Player > 99) {
        if (SzDuel_GetLP(nSide) <= 249) {
            return func_0019a5c8() % 2 == 0 ? 6 : 7;
        }
    } else {
        return func_0019a5c8() % 2 == 0 ? 6 : 7;
    }
    return -1;
}
