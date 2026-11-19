#include "dotr/ee/state_queries.h"

int TaLogic001_IsUseLordOfDragon(int side)
{
    if (SearchUnitByRange(0, 0, side, 1, 0x2001c, 14) != 0) {
        return 1;
    }
    return SearchUnitByRange(0, 0, side, 1, 0x1c, 14) != 0 ? 1 : 0;
}
