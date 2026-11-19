#include "dotr/ee/grid_scans.h"

int TaLogic007_IsRevivePlay(int side)
{
    int i;
    int id;
    struct DotrScanEntry *constInfo;
    struct DotrScanPair *effect;

    if (SzDuel_GetUnitSpellCount(side) >= 5) {
        return -1;
    }
    for (i = 0; i < SzDuel_GetGraveCount(side); i++) {
        id = SzDuel_InqGrave(side, i);
        if (TaHand_GetCardKind(id) >> 5 != 0) { /* not a monster */
            constInfo = func_001dd0e0_scan(id);
            effect = func_0026fc90_scan(id, 0);
            if (effect != 0 && constInfo == 0 && constInfo->deckCost >= 15 && SzDuel_GetUnitSpellCount(side) < 5 && effect->word >> 8 != 20) {
                return side << 16 | i;
            }
        }
    }
    return -1;
}
