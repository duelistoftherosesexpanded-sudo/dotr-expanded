#include "dotr/ee/position_rules.h"

void TaLogic019_CommandLDGuardUnit(int row, int column, struct DotrPositionOrder *order)
{
    int cell;
    int found;

    func_00250360_pair(row, column);
    cell = SzDuel_GetUnitLoc(row, column);
    if (TaLogic019_IsLinePoint(cell & 0xff, cell >> 8) == 1) {
        order->kind = 0x3ff;
        order->flags.word = cell << 16 | order->flags.half;
    } else {
        order->kind = 0x3ff;
        order->flags.word = cell << 16 | order->flags.half;
    }
    found = TaLogic019_GetEmptyLine();
    if (found >= 0) {
        order->kind = 0x3ff;
        order->flags.word = found << 16 | order->flags.half;
    } else {
        TaLogic019_CommandBtlUnit(order);
    }
}
