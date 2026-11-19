#include "dotr/ee/low_leaves.h"
int Ta_GetLDLvRev(int nSide)
{
    if (Ta_IsLDAbl(8, nSide)) {
        switch (SzDuel_GetUnitConstKind(nSide, 0) & 0x1f) {
        case 0:
        case 6:
        case 12:
        case 14:
            if (SzDuel_GetUnitLevel(nSide, 0, 0) > 6) {
                return 2;
            } else {
                return 1;
            }
            break;
        case 4:
        case 13:
            if (SzDuel_GetUnitLevel(nSide, 0, 0) > 5) {
                return 2;
            } else {
                return 1;
            }
            break;
        case 20:
            return 1;
            break;
        default:
            return 0;
            break;
        }
    } else {
        return 0;
    }
}
