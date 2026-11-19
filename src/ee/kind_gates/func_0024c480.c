#include "dotr/ee/kind_gates.h"

int TaTrapA_IsTrapAct(int x, int y, int a, int b, int c, int d)
{
    switch (SzDuel_GetUnitKindOfs(x, y)) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        if (!(SzDuel_GetUnitKind(a, b) >> 5)) {
            return Ta_IsLDRangeAvilityWithLocation(a, b, c, d, 11) ? 0 : 1;
        }
        break;
    case 6:
        if (!(SzDuel_GetUnitKind(a, b) >> 5) && TaUtil_GetUnitAP(a, b, c, d) < 501) {
            return Ta_IsLDRangeAvilityWithLocation(a, b, c, d, 11) ? 0 : 1;
        }
        break;
    case 7:
        if (!(SzDuel_GetUnitKind(a, b) >> 5) && TaUtil_GetUnitAP(a, b, c, d) < 1001) {
            return Ta_IsLDRangeAvilityWithLocation(a, b, c, d, 11) ? 0 : 1;
        }
        break;
    case 8:
        if (!(SzDuel_GetUnitKind(a, b) >> 5) && func_001e0290_try(a, b, 0) < 1501) {
            return Ta_IsLDRangeAvilityWithLocation(a, b, c, d, 11) ? 0 : 1;
        }
        break;
    case 9:
        if (!(SzDuel_GetUnitKind(a, b) >> 5) && TaUtil_GetUnitAP(a, b, c, d) < 2001) {
            return Ta_IsLDRangeAvilityWithLocation(a, b, c, d, 11) ? 0 : 1;
        }
        break;
    case 10:
        if (!(SzDuel_GetUnitKind(a, b) >> 5) && TaUtil_GetUnitAP(a, b, c, d) < 3001) {
            return Ta_IsLDRangeAvilityWithLocation(a, b, c, d, 11) ? 0 : 1;
        }
        break;
    case 11:
        return 1;
    case 12:
        if (SzDuel_GetUnitKind(a, b) >> 5) {
            return Ta_IsLDRangeAvilityWithLocation(a, b, c, d, 11) ? 0 : 1;
        }
        break;
    default:
        return 0;
    }
    return 0;
}
