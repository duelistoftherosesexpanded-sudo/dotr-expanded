#include "dotr/ee/grid_scans.h"

void TaLogic020_CommandSpellUnit(struct DotrScanOrder *order)
{
    int i;
    int spot;
    int found;

    if (func_001df540_scan(order->position >> 8, order->position & 0xff) == 0) {
        return;
    }
    Ta_GetCurrentSide(order->position >> 8, order->position & 0xff);
    SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    if (func_001df540_scan(order->position >> 8, order->position & 0xff)->unknown_3c == 688) {
        order->unknown_04 = 0xcff;
    } else {
        order->unknown_04 = 0x3ff;
    }
    for (i = 0; i < 5; i++) {
        spot = TaLogic020_GetLinePiece((unsigned char)i) & 0xffff;
        found = SzDuel_GetMapUnitID(spot & 0xff, spot >> 8);
        if (found == order->position || found < 0) {
            order->flags.word = spot << 16 | order->flags.half;
            return;
        }
    }
}
