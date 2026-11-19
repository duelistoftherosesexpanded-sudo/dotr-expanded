#include "dotr/ee/grid_scans.h"

int TaLogic015_IsUseRevive(int side)
{
    struct DotrScanHeld unit;
    int i;
    int highestDeckCost;
    int result;
    int id;
    struct DotrScanPair *effect;
    int n;

    highestDeckCost = 40;
    result = -1;
    if (SzDuel_GetUnitSpellCount(side) >= 5) {
        return result;
    }
    for (i = 0; i < SzDuel_GetGraveCount(side == 0 ? 1 : 0); i++) {
        id = SzDuel_InqGrave(side == 0 ? 1 : 0, i);
        if (TaHand_GetCardKind(id) >> 5 != 0) {
            func_001dcd70_item((int *)&unit, id);
            effect = func_001e09e0_scan(&unit, 0);
            n = unit.entry->deckCost;
            if (n >= highestDeckCost && effect->word >> 8 != 20) {
                highestDeckCost = n;
                result = i | (side == 0 ? 1 : 0) << 16;
            }
        }
    }
    return result;
}
