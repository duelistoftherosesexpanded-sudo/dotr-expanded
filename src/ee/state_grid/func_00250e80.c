#include "dotr/ee/state_grid.h"

/* Applies op (0 add, 1 sub, 2 mul, 3 divide, 4 set) of val to a counter clamped to 0..9999 via the
 * read/write callees; returns 1, or -1 for op 3 with val 0. */
int TaUtil_CalcUnitDP(int a, int b, int op, int val)
{
    int cur;
    switch (op) {
    case 0:
        cur = SzDuel_GetUnitDP(a, b, 0) + val;
        if (cur > 9999) {
            cur = SzDuel_GetUnitDP(a, b, 1);
            SzDuel_CalcUnitDP(a, b, 4, 9999 - cur);
        } else {
            SzDuel_CalcUnitDP(a, b, 0, val);
        }
        break;
    case 1:
        cur = SzDuel_GetUnitDP(a, b, 0) - val;
        if (cur < 0) {
            cur = SzDuel_GetUnitDP(a, b, 1);
            SzDuel_CalcUnitDP(a, b, 4, -cur);
        } else {
            SzDuel_CalcUnitDP(a, b, 1, val);
        }
        break;
    case 2:
        cur = val * SzDuel_GetUnitDP(a, b, 0);
        if (cur > 9999) {
            cur = SzDuel_GetUnitDP(a, b, 1);
            SzDuel_CalcUnitDP(a, b, 4, 9999 - cur);
        } else {
            SzDuel_CalcUnitDP(a, b, 2, val);
        }
        break;
    case 3:
        if (val == 0) {
            return -1;
        }
        SzDuel_CalcUnitDP(a, b, 3, val);
        break;
    case 4:
        SzDuel_CalcUnitDP(a, b, 4, val);
        break;
    }
    return 1;
}
