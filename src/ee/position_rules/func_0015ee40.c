#include "dotr/ee/position_rules.h"

int TaLogic019_GetEmptyLine(void)
{
    int i;

    for (i = 0; i < 4; i++) {
        int pos = TaLogic019_GetLine(i);

        if ((SzDuel_GetMapUnit(pos & 0xFF, pos >> 8) == 0 ? 1 : 0) == 1) {
            int side = SzDuel_GetTurnOwner();
            int r;

            switch (i) {
            case 0:
                r = TaLogic_GetSommonPiecePreference__001604a0(side, 0);
                break;
            case 1:
                r = TaLogic_GetSommonPiecePreference__001604a0(side, 3);
                break;
            case 2:
                r = TaLogic_GetSommonPiecePreference__001604a0(side, 4);
                break;
            case 3:
                r = TaLogic_GetSommonPiecePreference__001604a0(side, 5);
                break;
            default:
                r = -1;
                break;
            }
            return r;
        }
    }
    return -1;
}
