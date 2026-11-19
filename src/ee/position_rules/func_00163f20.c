#include "dotr/ee/position_rules.h"

void TaLogic020_CommandInvadeUnit(struct DotrPositionOrder *order)
{
    int cell;

    func_00250360_pair((int)(unsigned)order->position >> 8, order->position & 0xff);
    cell = SzDuel_GetUnitLoc((int)(unsigned)order->position >> 8, order->position & 0xff);
    order->kind = 0x6ff;
    order->flags.word = order->flags.half | (TaLogic020_GetNextGrassPoint(cell & 0xff, cell >> 8) << 16);
}
