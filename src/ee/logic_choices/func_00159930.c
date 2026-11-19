#include "dotr/ee/logic_choices.h"

/* TaLogic018_GetVirusMove (0x159930): the cell (row << 8 | column) a virus unit at (arg0, arg1)
 * moves to: among the cells within its move range (GetMoveRev, low four bits) whose field is 9
 * and to which TaLogic_IsPassRoot answers 1, skipping cells TaLogic_IsAttackedNow2 reports for a
 * non-zero arg1, the lowest TaLogic_GetUnitWarning value (on a tie an empty cell replaces an
 * occupied one); 0x303 when none.
 * Compiled with #pragma opt_propagation off (docs/DECOMPILATION.md). */

static inline int min_00159930(int a, int b)
{
    return (a >= b + 1) ? b : a;
}

#ifdef __MWERKS__
#pragma opt_propagation off
#endif

int TaLogic018_GetVirusMove(int arg0, int arg1)
{
    int best;
    short tmp;
    int pos;
    int range;
    unsigned char xMin;
    unsigned char xMax;
    unsigned char yMin;
    unsigned char yMax;
    int i;
    int j;
    int d;
    int result;

    best = 0xFFFF;
    result = 0xFF;
    tmp = best;
    pos = SzDuel_GetUnitLoc(arg0, arg1);
    range = GetMoveRev(SzDuel_GetUnit(arg0, arg1), pos & 0xFF, pos >> 8);
    xMin = GetCap(0, (pos & 0xFF) - (range & 0xF));
    xMax = min_00159930((pos & 0xFF) + (range & 0xF), 6);
    yMin = GetCap(0, (pos >> 8) - (range & 0xF));
    yMax = min_00159930((pos >> 8) + (range & 0xF), 6);
    for (i = xMin; i <= xMax; i++) {
        for (j = yMin; j <= yMax; j++) {
            if (((pos & 0xFF) > i ? (pos & 0xFF) : i) - ((pos & 0xFF) < i ? (pos & 0xFF) : i) +
                    (((pos >> 8) > j ? (pos >> 8) : j) - ((pos >> 8) < j ? (pos >> 8) : j)) > (range & 0xF)) {
                continue;
            }
            if (SzDuel_GetField(i, j) != 9) {
                continue;
            }
            if (TaLogic_IsPassRoot(arg0, arg1, pos & 0xFF, pos >> 8, i, j, &tmp) != 1) {
                continue;
            }
            if (TaLogic_IsAttackedNow2(arg0, arg1, i, j, 0) != 0 && arg1 != 0) {
                continue;
            }
            d = TaLogic_GetUnitWarning(i, j, (arg0 << 8) | arg1, 3, (unsigned short)TaLogic_GetWarnHideLvl__0015d120(arg0), 0);
            if (d < best) {
                best = d;
                result = (j << 8) | i;
            } else if (d == best && SzDuel_GetMapUnit(i, j) == 0 && SzDuel_GetMapUnit(result & 0xFF, result >> 8) != 0) {
                best = d;
                result = (j << 8) | i;
            }
        }
    }
    if (result != 0xFF) {
        return result;
    }
    return 0x303;
}

#ifdef __MWERKS__
#pragma opt_propagation reset
#endif
