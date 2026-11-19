#include "dotr/ee/kind_gates.h"

int Ta_GetLDParamOfs2(struct DotrGateItem *unit, int col, int row)
{
    int total;
    int enemySide;
    int kind;
    int side;

    total = 0;
    if (unit == 0) {
        return 0;
    }
    side = func_00250370_gate(unit);
    kind = unit->kind & 0x1f;
    enemySide = side == 0 ? 1 : 0;
    if (Ta_IsLDRangeAvilityFromLoc(row << 8 | col, side, kind, 10) != 0) {
        total += SzDuel_GetUnitLevel(side, 0, 0) * 100 >= 500 ? 500 : SzDuel_GetUnitLevel(side, 0, 0) * 100;
    }
    if (Ta_IsLDRange(enemySide, col, row) != 0) {
        if (Ta_IsLDAbl(14, enemySide) != 0 && Ta_IsLD_PDownObj(enemySide, kind) != 0) {
            total -= SzDuel_GetUnitLevel(enemySide, 0, 0) * 100 >= 500 ? 500 : SzDuel_GetUnitLevel(enemySide, 0, 0) * 100;
        }
    }
    return total;
}
