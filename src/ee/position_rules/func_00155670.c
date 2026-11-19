#include "dotr/ee/position_rules.h"

int TaLogic017_IsLineTop(int a, int b)
{
    int i;
    int magicType;
    int next;
    int pos;
    int cell;

    SzDuel_GetUnitLoc(a, b);
    func_00250360_pair(a, b);
    magicType = SzDuel_GetUnitKind(a, b) >> 5;
    next = magicType == 0 ? magicType + 1 : 0;
    a = (a << 8) | b;
    for (i = 0; i < 7; i++) {
        pos = TaLogic017_GetLine(i, magicType);
        cell = SzDuel_GetMapUnitID(pos & 0xff, pos >> 8);
        if (cell < 0) {
            return 0;
        }
        if (cell == a) {
            return 1;
        }
    }
    for (i = 0; i < 7; i++) {
        pos = TaLogic017_GetLine(i, next);
        cell = SzDuel_GetMapUnitID(pos & 0xff, pos >> 8);
        if (cell < 0) {
            return 0;
        }
        if (cell == a) {
            return 1;
        }
    }
    return 0;
}
