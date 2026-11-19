#include "dotr/ee/kind_gates.h"

int Ta_IsLD_PDownObj(int side, int objKind)
{
    switch (SzDuel_GetUnitConstKind(side, 0) & 0x1f) {
    case 1:
        if (objKind == 7) {
            return 1;
        }
        return 0;
    case 2:
        if (objKind == 3) {
            return 1;
        }
        return 0;
    case 3:
        if (objKind == 0 || objKind == 1) {
            return 1;
        }
        return 0;
    case 5:
        if (objKind == 12) {
            return 1;
        }
        return 0;
    case 6:
        if (objKind == 9 || objKind == 12) {
            return 1;
        }
        return 0;
    case 7:
        if (objKind == 1) {
            return 1;
        }
        return 0;
    case 9:
        if (objKind == 13) {
            return 1;
        }
        return 0;
    case 11:
        if (objKind == 5 || objKind == 6) {
            return 1;
        }
        return 0;
    case 12:
        if (objKind == 15) {
            return 1;
        }
        return 0;
    case 15:
        if (objKind == 2 || objKind == 14) {
            return 1;
        }
        return 0;
    case 16:
        if (objKind == 14 || objKind == 17) {
            return 1;
        }
        return 0;
    case 17:
        if (objKind == 4) {
            return 1;
        }
        return 0;
    }
    return 0;
}
