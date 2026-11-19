#include "dotr/ee/position_rules.h"

int TaLogic019_IsEmptyTrapGuard(int a, int b)
{
    int cell;
    int other;
    int side;

    side = func_00250360_pair(a, b);
    if (SearchUnitByRange(0, 0, side, 1, 0x33b001c, 14) != 0) {
        cell = SzDuel_GetMapUnitID(0, 5);
        if (cell >= 0) {
            if (cell == ((a << 8) | b)) {
                return 0x500;
            }
            other = func_00250360_pair(cell >> 8, cell & 0xff);
            if (other != func_00250360_pair(a, b)) {
                return 0x500;
            }
        } else {
            return 0x500;
        }
    }
    if (SearchUnitByRange(0, 0, side, 1, 0x2ed001c, 14) != 0) {
        cell = SzDuel_GetMapUnitID(6, 3);
        if (cell >= 0) {
            if (cell == ((a << 8) | b)) {
                return 0x306;
            }
            other = func_00250360_pair(cell >> 8, cell & 0xff);
            if (other != func_00250360_pair(a, b)) {
                return 0x306;
            }
        } else {
            return 0x306;
        }
    }
    return -1;
}
