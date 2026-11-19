#include "dotr/ee/position_rules.h"

static inline int func_0014ca60_pick(int i)
{
    int side = SzDuel_GetTurnOwner();
    int r;

    switch (i) {
    case 0:
        r = TaLogic_GetSommonPiecePreference__0014da30(side, 0);
        break;
    case 1:
        r = TaLogic_GetSommonPiecePreference__0014da30(side, 3);
        break;
    case 2:
        r = TaLogic_GetSommonPiecePreference__0014da30(side, 4);
        break;
    default:
        r = -1;
        break;
    }
    return r;
}

int TaLogic014_GetEmptyLine(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        int pos = func_0014ca60_pick(i);

        if ((SzDuel_GetMapUnit(pos & 0xFF, pos >> 8) == 0 ? 1 : 0) == 1) {
            return func_0014ca60_pick(i);
        }
    }
    return -1;
}
