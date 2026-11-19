#include "dotr/ee/position_rules.h"

void TaLogic005_GetNearDefensePoint(int word, int column, int row, struct DotrSpotBest *best)
{
    struct DotrSpotNibbles bytes[12];
    int i;
    struct DotrSpotNibbles *p;
    int spot;
    int y;
    int x;
    int id;
    int cost;

    best->spot = -1;
    best->cost = 0xff;
    for (i = 0, p = bytes; i < 12; i++, p++) p->low = p->high = 0xf;
    for (i = 0; i < 3; i++) {
        if (TaLogic005_IsEmptyDefensePoint(i) == 1) {
            spot = TaLogic005_GetDefensePoint(i);
            x = spot & 0xff;
            y = spot >> 8;
            id = SzDuel_GetMapUnitID(x, y);
            if (id < 0) {
                id = 0xffff;
            }
            cost = func_002792c0_spot(word, bytes, column, row, x, y, id, 7);
            if (cost < best->cost && cost >= 0) {
                best->cost = cost;
                best->spot = spot;
                return;
            }
        }
    }
}
