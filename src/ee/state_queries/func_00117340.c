#include "dotr/ee/state_queries.h"

int TaLogic001_IsActiveMode(void)
{
    if (TaLogic_GetGraveCardCount(SzDuel_GetTurnOwner(), 3, 2) != 0 || TaLogic_GetGraveCardCount(SzDuel_GetTurnOwner(), 3, 0x345) != 0
        || SearchUnitByRange(0, 0, SzDuel_GetTurnOwner(), 1, 0x2001c, 14) != 0) {
        return 1;
    }
    return 0;
}
