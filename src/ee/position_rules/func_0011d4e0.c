#include "dotr/ee/position_rules.h"

void TaLogic004_GetNearAtkLineEdge(int unit, int column, int row, struct DotrSpotBest *best)
{
    struct DotrSpotNibbles bytes[12];
    int i;
    struct DotrSpotNibbles *p;
    int spot;
    int y;
    int x;
    int id;
    int cost;
    int y2;
    int x2;

    best->spot = -1;
    best->cost = 0xff;
    for (i = 0, p = bytes; i < 12; i++, p++) p->low = p->high = 0xf;
    for (i = 0; i < 4; i++) {
        if (TaLogic004_IsEmptyAtkLine(0, i) == 1) {
            spot = TaLogic004_GetAtkLineStart(0, i);
            x = spot & 0xff;
            y = spot >> 8;
            id = SzDuel_GetMapUnitID(x, y);
            if (id < 0) {
                id = 0xffff;
            }
            cost = func_002792c0_spot(unit, bytes, column, row, x, y, id, 7);
            if (cost < best->cost && cost >= 0) {
                best->cost = cost;
                best->spot = spot;
            }
            spot = TaLogic004_GetAtkLineEnd(0, i);
            x2 = spot & 0xff;
            y2 = spot >> 8;
            id = SzDuel_GetMapUnitID(x2, y2);
            if (id < 0) {
                id = 0xffff;
            }
            cost = func_002792c0_spot(unit, bytes, column, row, x2, y2, id, 7);
            if (cost < best->cost && cost >= 0) {
                best->cost = cost;
                best->spot = spot;
            }
        }
    }
}

