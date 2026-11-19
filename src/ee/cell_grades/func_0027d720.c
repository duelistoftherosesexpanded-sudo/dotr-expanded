#include "dotr/ee/cell_grades.h"

int TaLogic_CommandGunnerUnit(struct DotrGradeOrder *order, int mode)
{
    unsigned short words[22];
    int side;
    int i;
    unsigned short *q;
    int r;

    side = Ta_GetCurrentSide(order->position >> 8, order->position & 0xff);
    for (i = 0, q = words; i < 22; i++) {
        *q++ = 0xffff;
    }
    switch (SzDuel_GetUnitKind(order->position >> 8, order->position & 0xff) >> 5) {
    case 0:
        if (SzDuel_GetUnitStatus(order->position >> 8, order->position & 0xff, 4) != 0) {
            return 0;
        }
        break;
    default:
        break;
    }
    TaLogic_GetUnitCtlLst2(side == 0 ? 1 : 0, words, 52, 0, 0);
    r = TaLogic_GetMaxApUnit3(words, side == 0 ? 1 : 0);
    if (r < 0) {
        return 0;
    }
    TaLogic_SetTaskAtkOrMove(order, r, mode);
    return 1;
}
