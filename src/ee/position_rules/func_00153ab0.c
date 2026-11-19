#include "dotr/ee/position_rules.h"

int TaLogic015_IsActiveMode(void)
{
    return (SzDuel_GetUnitCount(SzDuel_GetTurnOwner()) < 3) ? 0 : 1;
}
