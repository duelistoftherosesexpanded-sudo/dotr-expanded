#include "dotr/ee/grid_scans.h"

int TaLogicCom_IsUseRevive(int side, int sidesToCount, int mode)
{
    struct DotrScanHeld unit;
    int i;
    int highestDeckCost;
    int result;
    int id;
    struct DotrScanPair *effect;
    int n;
    int r;
    /* mid is declared before hi: the order decides which saved register each gets. */
    int mid;
    int hi;

    highestDeckCost = 40;
    result = -1;
    mode = (unsigned)(mode - 29) < 2 ? mode : (func_0019a5c8() % 2 == 0 ? 29 : 30);
    if (mode == 29) { /* 29: revive a monster; 30: revive a spell */
        if (SzDuel_GetUnitMonsterCount(side) >= 5) {
            return -1;
        }
        r = TaLogic_GetMaxAPGraveCard(side, sidesToCount);
        hi = r >> 24;
        mid = r >> 16;
        id = SzDuel_InqGrave(hi, mid & 0xF);
        if (TaHand_GetCardKind(id) >> 5 == 0) {
            func_001dcd70_item((int *)&unit, id);
            if (SzDuel_GetUnitAP2((struct DotrStateItem *)&unit, 0) >= 2500) {
                return hi | (mid & 0xFF);
            }
        }
    } else {
        if (SzDuel_GetUnitSpellCount(side) >= 5) {
            return -1;
        }
        if (sidesToCount & 1) {
            for (i = 0; i < SzDuel_GetGraveCount(side); i++) {
                id = SzDuel_InqGrave(side, i);
                if (TaHand_GetCardKind(id) >> 5 != 0) {
                    func_001dcd70_item((int *)&unit, id);
                    effect = func_001e09e0_scan(&unit, 0);
                    n = unit.entry->deckCost;
                    if (n >= highestDeckCost && effect->word >> 8 != 20) {
                        highestDeckCost = n;
                        result = side << 16 | i;
                    }
                }
            }
        }
        if ((sidesToCount >> 1) & 1) {
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
        }
        return result;
    }
    return -1;
}
