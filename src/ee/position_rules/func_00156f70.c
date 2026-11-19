#include "dotr/ee/position_rules.h"

int TaLogic017_GetEmptyTrapPiece(void)
{
    if (SzDuel_GetMapUnit(2, 3) == 0) {
        return 0x302;
    }
    if (SzDuel_GetMapUnit(4, 3) == 0) {
        return 0x304;
    }
    return -1;
}
