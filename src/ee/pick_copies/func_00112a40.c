#include "dotr/ee/pick_copies.h"

int TaLogic_IsExzodia__00112a40(int side)
{
    int limbChecks[4];
    int k;
    int total;
    int i;
    int x;
    int y;
    int t;
    int lo;
    int hi;
    int cell;

    total = 0;
    if (SzDuel_GetUnitCardNo(side, 0) == 58) { /* leader is Exodia the Forbidden One */
    for (k = 0; k < 4; k++) {
        for (i = 0; i < 8; i++) {
            cell = Ta_GetSommonPiecePreference(side, i);
            y = cell & 0xff;
            x = cell >> 8;
            if (SzDuel_GetMapUnit(y, x) != 0) {
                cell = SzDuel_GetMapUnitID(y, x);
                if (cell >= 0) {
                    hi = cell >> 8;
                    lo = cell & 0xff;
                    t = SzDuel_GetUnitCardNo(hi, lo);
                    if (t == Ta_GetExzodiaPartsCardNo(k) && SzDuel_GetUnitCurseRest(hi, lo) == 0 && SzDuel_GetUnitSpellRest(hi, lo) == 0 && hi == side) {
                        limbChecks[k] = 1;
                    }
                }
            }
        }
        total += limbChecks[k];
    }
    return total;
    }
    return 0;
}
