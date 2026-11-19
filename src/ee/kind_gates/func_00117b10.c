#include "dotr/ee/kind_gates.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int TaLogic001_IsLandNearCntr(int loc)
{
    struct DotrStateItem *item;

    if (loc == 0x303) {
        return 1;
    }
    item = SzDuel_GetMapUnit(3, 3);
    if (item != 0) {
        if (func_00250370_gate((struct DotrGateItem *)item) != SzDuel_GetTurnOwner()) {
            if ((MAX((loc & 0xff), 3) - MIN((loc & 0xff), 3)) + (MAX((loc >> 8), 3) - MIN((loc >> 8), 3)) == 1) {
                return 1;
            }
        }
    }
    return 0;
}
