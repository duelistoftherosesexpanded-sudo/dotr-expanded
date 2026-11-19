#include "dotr/ee/small_misc.h"

int Ta_IsLD_DestObj(int side, int objKind)
{
    switch (SzDuel_GetUnitConstKind(side, 0) & 0x1f) {
    case 8:
        if (objKind == 7) {
            return 1;
        }
        return 0;
    case 11:
        if (objKind == 9) {
            return 1;
        }
        return 0;
    case 17:
        if (objKind == 2 || objKind == 19) {
            return 1;
        }
        return 0;
    case 18:
        if (objKind == 10) {
            return 1;
        }
        return 0;
    case 19:
        if (objKind == 18) {
            return 1;
        }
        return 0;
    default:
        return 0;
    }
}
