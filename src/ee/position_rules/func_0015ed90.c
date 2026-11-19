#include "dotr/ee/position_rules.h"

int TaLogic019_GetLine(int index)
{
    int side;

    side = SzDuel_GetTurnOwner();
    switch (index) {
    case 0:
        return TaLogic_GetSommonPiecePreference__001604a0(side, 0);
    case 1:
        return TaLogic_GetSommonPiecePreference__001604a0(side, 3);
    case 2:
        return TaLogic_GetSommonPiecePreference__001604a0(side, 4);
    case 3:
        return TaLogic_GetSommonPiecePreference__001604a0(side, 5);
    default:
        return -1;
    }
}
