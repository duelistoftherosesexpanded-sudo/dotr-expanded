#include "dotr/ee/position_rules.h"

int TaLogic010_IsActiveMode(void)
{
    if (TaLogic010_GetEmptyLine(SzDuel_GetTurnOwner()) < 0) {
        return 1;
    }
    return 0;
}
