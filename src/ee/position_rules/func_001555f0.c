#include "dotr/ee/position_rules.h"

int TaLogic017_GetEmptyLine(int magicType)
{
    int i;
    for (i = 0; i < 7; i++) {
        int pos = TaLogic017_GetLine(i, magicType);
        if ((SzDuel_GetMapUnit(pos & 0xFF, pos >> 8) == 0 ? 1 : 0) == 1) {
            return TaLogic017_GetLine(i, magicType);
        }
    }
    return -1;
}
