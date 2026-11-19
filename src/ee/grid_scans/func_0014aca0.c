#include "dotr/ee/grid_scans.h"

int TaLogic013_IsActiveMode(void)
{
    int side;
    int y;
    int i;
    int hlsFlagList;
    int x;
    int a;
    int y2;
    int x2;
    int a2;
    int hpdFlagList;
    int j;
    int ownerId;
    int g;

    side = SzDuel_GetTurnOwner();
    /* Search type 28 (card number): card 273 Harpie Lady Sisters, 30 Harpie's Pet Dragon,
     * 11 Serpent Night Dragon. */
    hlsFlagList = SearchUnitByRange(0, 0, side, 1, 0x111001C, 14);
    for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(hlsFlagList); i++) {
        ownerId = Ta_GetUnitOwnerIDFromLstCount(hlsFlagList, i);
        y = ownerId & 0xFF;
        x = ownerId >> 8;
        a = func_001e0290_try(x, y, 0);
        g = func_001e0290_try(x, y, 1) < a;
        if (g) {
            return 1;
        }
    }
    hpdFlagList = SearchUnitByRange(0, 0, side, 1, 0x1E001C, 14);
    for (j = 0; j < Ta_GetUnitNumFrom22bitFlagList(hpdFlagList); j++) {
        ownerId = Ta_GetUnitOwnerIDFromLstCount(hpdFlagList, j);
        y2 = ownerId & 0xFF;
        x2 = ownerId >> 8;
        a2 = func_001e0290_try(x2, y2, 0);
        g = func_001e0290_try(x2, y2, 1) < a2;
        if (g) {
            return 1;
        }
    }
    return SearchUnitByRange(0, 0, side, 1, 0xB001C, 14) ? 1 : 0;
}
