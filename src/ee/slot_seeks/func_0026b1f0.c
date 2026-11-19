#include "dotr/ee/slot_seeks.h"

int Ta_IsTrapBInDecideOperation2(int nSide, int nID, int stDefense, unsigned char IsView) {
    int Unit;
    int Unit2;
    int Loc;
    int Self;

    Loc = SzDuel_GetUnitLoc(nSide, nID);
    Self = Ta_GetCurrentSide(nSide, nID);
    if (nID == 0) {
        return 0;
    }
    if (Ta_IsLDRangeAvilityWithLocation(nSide, nID, Loc & 0xff, Loc >> 8, 11)) {
        if (!Ta_IsUnitHide(SzDuel_GetUnit(nSide, nID), Ta_IsUnitControl(nSide, nID) == 0 ? 1 : 0)) {
            return 1;
        }
    }
    if (Ta_IsResistAll(nSide, nID)) {
        return 0;
    }
    switch (stDefense) {
    case 0:
        Unit = Ta_IsActiveTrapB(Self, 44); /* 44: stop attack move */
        if (Unit >= 0) {
            if ((IsView & 0xff) || SzDuel_GetUnitStatus(Unit >> 8, Unit & 0xff, 4)) {
                if (Ta_IsActiveTrapB(nSide == 0 ? 1 : 0, 46) >= 0) { /* 46: deny traps */
                    return 0;
                }
                return 0x10001;
            }
        }
        return 0;
    default:
        Unit = Ta_IsActiveTrapB(Self, 74); /* 74: Gorgon's eye */
        if (Unit >= 0) {
            if ((IsView & 0xff) || SzDuel_GetUnitStatus(Unit >> 8, Unit & 0xff, 4)) {
                if (Ta_IsActiveTrapB(nSide == 0 ? 1 : 0, 46) >= 0) {
                    return 0;
                }
                return 0x10001;
            }
            break;
        }
        Unit2 = Ta_IsActiveTrapB(Self, 35); /* 35: shift to attack */
        if (Unit2 >= 0) {
            if ((IsView & 0xff) || SzDuel_GetUnitStatus(Unit2 >> 8, Unit2 & 0xff, 4)) {
                if (Ta_IsActiveTrapB(nSide == 0 ? 1 : 0, 46) >= 0) {
                    return 0;
                }
                return 1;
            }
        }
        break;
    }
    return 0;
}
