#include "dotr/ee/low_leaves.h"

void TaLogic018_GetZoneSide(int x, int y, unsigned short *list)
{
    int i;
    int j;

    if (TaLogic018_IsZonePast((y << 8) | x, list) != 1) {
        if (SzDuel_GetField(x, y) == 9) {
            list[TaLogic018_GetZoneSize(list)] = (y << 8) | x;
            for (i = GetCap(0, x - 1); i <= (x + 1 >= 7 ? 6 : x + 1); i++) {
                for (j = GetCap(0, y - 1); j <= (y + 1 >= 7 ? 6 : y + 1); j++) {
                    if (((x < i ? i : x) - (i < x ? i : x)) + ((y < j ? j : y) - (j < y ? j : y)) == 1) {
                        TaLogic018_GetZoneSide(i, j, list);
                    }
                }
            }
        }
    }
}
