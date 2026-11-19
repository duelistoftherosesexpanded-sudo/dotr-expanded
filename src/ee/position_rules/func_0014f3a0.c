#include "dotr/ee/position_rules.h"
/* File-local helper: a spelling that reproduces the target's kept result and
 * final sign test, not a claim about an original function. */
static inline int find(void)
{
    int i;
    int spot;
    int r;
    int side;

    for (i = 0; i < 3; i++) {
        spot = TaLogic014_GetLine(i);
        if ((SzDuel_GetMapUnit(spot & 0xff, spot >> 8) == 0 ? 1 : 0) == 1) {
            side = SzDuel_GetTurnOwner();
            switch (i) {
            case 0:
                r = Ta_GetSommonPiecePreference(side, 0);
                break;
            case 1:
                r = Ta_GetSommonPiecePreference(side, 3);
                break;
            case 2:
                r = Ta_GetSommonPiecePreference(side, 4);
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
int TaLogic014_IsActiveMode(void)
{
    if (find() < 0) {
        return 1;
    }
    return 0;
}
