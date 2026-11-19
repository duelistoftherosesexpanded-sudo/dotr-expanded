#include "dotr/ee/state_grid.h"

/* Applies operation (0 add, 1 sub, 2 mul, 3 divide, 4 set) of val to a unit's AP through the
 * read/write callees, clamping the add, sub and mul results to 0..9999; returns 1, or -1 for
 * operation 3 with val 0. */
int TaUtil_CalcUnitAP(int side, int pos, int operation, int val)
{
    int cur;
    switch (operation) {
    case 0:
        cur = SzDuel_GetUnitAP(side, pos, 0) + val;
        if (cur > 9999) {
            cur = SzDuel_GetUnitAP(side, pos, 1);
            SzDuel_CalcUnitAP(side, pos, 4, 9999 - cur);
        } else {
            SzDuel_CalcUnitAP(side, pos, 0, val);
        }
        break;
    case 1:
        cur = SzDuel_GetUnitAP(side, pos, 0) - val;
        if (cur < 0) {
            cur = SzDuel_GetUnitAP(side, pos, 1);
            SzDuel_CalcUnitAP(side, pos, 4, -cur);
        } else {
            SzDuel_CalcUnitAP(side, pos, 1, val);
        }
        break;
    case 2:
        cur = val * SzDuel_GetUnitAP(side, pos, 0);
        if (cur > 9999) {
            cur = SzDuel_GetUnitAP(side, pos, 1);
            SzDuel_CalcUnitAP(side, pos, 4, 9999 - cur);
        } else {
            SzDuel_CalcUnitAP(side, pos, 2, val);
        }
        break;
    case 3:
        if (val == 0) {
            return -1;
        }
        SzDuel_CalcUnitAP(side, pos, 3, val);
        break;
    case 4:
        SzDuel_CalcUnitAP(side, pos, 4, val);
        break;
    }
    return 1;
}
