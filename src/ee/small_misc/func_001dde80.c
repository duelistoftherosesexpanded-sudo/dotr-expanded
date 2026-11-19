#include "dotr/ee/small_misc.h"

void SzDuel_SetupMap(int index)
{
    int x;
    int y;
    int k;
    struct DotrMiscLargeGrids *entry;

    if (index >= 0 && index < 46) {
        for (y = 0; y < 7; y++) {
            for (x = 0; x < 7; x++) {
                SzDuel_SetField(x, y, gSzDuel_MapData[index][y][x]);
            }
        }
    } else if (index < 51) {
        entry = (struct DotrMiscLargeGrids *)SzData_GetData(0);
        k = index - 46;
        for (y = 0; y < 7; y++) {
            for (x = 0; x < 7; x++) {
                SzDuel_SetField(x, y, entry->grids[k][y][x]);
            }
        }
    } else if (index < 56) {
        entry = (struct DotrMiscLargeGrids *)SzData_GetData(1);
        k = index - 51;
        for (y = 0; y < 7; y++) {
            for (x = 0; x < 7; x++) {
                SzDuel_SetField(x, y, entry->grids[k][y][x]);
            }
        }
    }
}
