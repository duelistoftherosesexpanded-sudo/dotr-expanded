#include "dotr/ee/atk_efc.h"

int Do_Match_AtkEfcObj(int eObj, int nSide, int nID)
{
    /* eObj 0-19 name a monster kind (0 dragon ... 19 plant), 20-25 an attribute
     * (light, dark, fire, earth, water, wind). */
    switch (eObj) {
    case 0:
        return (SzDuel_GetUnitKind(nSide, nID) & 0x1F) == 0 ? 1 : -1;
    case 1:
        return (SzDuel_GetUnitKind(nSide, nID) & 0x1F) == 1 ? 1 : -1;
    case 2:
        return (SzDuel_GetUnitKind(nSide, nID) & 0x1F) == 2 ? 1 : -1;
    case 3:
        return (SzDuel_GetUnitKind(nSide, nID) & 0x1F) == 3 ? 1 : -1;
    case 4:
        return (SzDuel_GetUnitKind(nSide, nID) & 0x1F) == 4 ? 1 : -1;
    case 5:
        return (SzDuel_GetUnitKind(nSide, nID) & 0x1F) == 5 ? 1 : -1;
    case 6:
        return (SzDuel_GetUnitKind(nSide, nID) & 0x1F) == 6 ? 1 : -1;
    case 7:
        return (SzDuel_GetUnitKind(nSide, nID) & 0x1F) == 7 ? 1 : -1;
    case 8:
        return (SzDuel_GetUnitKind(nSide, nID) & 0x1F) == 8 ? 1 : -1;
    case 9:
        return (SzDuel_GetUnitKind(nSide, nID) & 0x1F) == 9 ? 1 : -1;
    case 10:
        return (SzDuel_GetUnitKind(nSide, nID) & 0x1F) == 10 ? 1 : -1;
    case 11:
        return (SzDuel_GetUnitKind(nSide, nID) & 0x1F) == 11 ? 1 : -1;
    case 12:
        return (SzDuel_GetUnitKind(nSide, nID) & 0x1F) == 12 ? 1 : -1;
    case 13:
        return (SzDuel_GetUnitKind(nSide, nID) & 0x1F) == 13 ? 1 : -1;
    case 14:
        return (SzDuel_GetUnitKind(nSide, nID) & 0x1F) == 14 ? 1 : -1;
    case 15:
        return (SzDuel_GetUnitKind(nSide, nID) & 0x1F) == 15 ? 1 : -1;
    case 16:
        return (SzDuel_GetUnitKind(nSide, nID) & 0x1F) == 16 ? 1 : -1;
    case 17:
        return (SzDuel_GetUnitKind(nSide, nID) & 0x1F) == 17 ? 1 : -1;
    case 18:
        return (SzDuel_GetUnitKind(nSide, nID) & 0x1F) == 18 ? 1 : -1;
    case 19:
        return (SzDuel_GetUnitKind(nSide, nID) & 0x1F) == 19 ? 1 : -1;
    case 20:
        return (SzDuel_GetUnitAttr(nSide, nID)) == 0 ? 1 : -1;
    case 21:
        return (SzDuel_GetUnitAttr(nSide, nID)) == 1 ? 1 : -1;
    case 22:
        return (SzDuel_GetUnitAttr(nSide, nID)) == 2 ? 1 : -1;
    case 23:
        return (SzDuel_GetUnitAttr(nSide, nID)) == 3 ? 1 : -1;
    case 24:
        return (SzDuel_GetUnitAttr(nSide, nID)) == 4 ? 1 : -1;
    case 25:
        return (SzDuel_GetUnitAttr(nSide, nID)) == 5 ? 1 : -1;
    }
    return 0;
}
