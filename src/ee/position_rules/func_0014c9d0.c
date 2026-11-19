#include "dotr/ee/position_rules.h"

int TaLogic014_GetLine(int index)
{
    int side;

    side = SzDuel_GetTurnOwner();
    switch (index) {
    case 0:
        return TaLogic_GetSommonPiecePreference__0014da30(side, 0);
    case 1:
        return TaLogic_GetSommonPiecePreference__0014da30(side, 3);
    case 2:
        return TaLogic_GetSommonPiecePreference__0014da30(side, 4);
    default:
        return -1;
    }
}
