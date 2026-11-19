#include "dotr/ee/lower_rules.h"
int TaLogic_IsUnitCategory(int nSide, int nID, int sKind)
{
    (void)nSide;
    switch (sKind) {
    case 34:
        if (nID == 0) {
            return 1;
        }
        break;
    case 29:
        if (nID > 0 && nID < 6) {
            return 1;
        }
        break;
    case 30:
        if (nID > 5 && nID < 12) {
            return 1;
        }
        break;
    case 31:
        return 1;
    case 52:
        if (nID != 0) {
            return 1;
        }
        break;
    default:
        if (nID != 0) {
            return 1;
        }
        break;
    }
    return 0;
}
