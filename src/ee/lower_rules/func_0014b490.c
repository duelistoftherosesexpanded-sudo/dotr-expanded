#include "dotr/ee/lower_rules.h"

int TaLogic013_IsUseRevive(int side)
{
    int i;

    if (SzDuel_GetUnitMonsterCount(side) >= 5) {
        return -1;
    }
    if (TaLogic_GetGraveCardCount(side, 3, 273) > 0 || TaLogic_GetGraveCardCount(side, 3, 30) > 0 || TaLogic_GetGraveCardCount(side, 3, 11) > 0) {
        for (i = 0; i < SzDuel_GetGraveCount(side); i++) {
            if (SzDuel_InqGrave(side, i) == 11) {
                return side << 16 | i;
            }
        }
        for (i = 0; i < SzDuel_GetGraveCount(side == 0 ? 1 : 0); i++) {
            if (SzDuel_InqGrave(side == 0 ? 1 : 0, i) == 11) {
                return i | (side == 0 ? 1 : 0) << 16;
            }
        }
        for (i = 0; i < SzDuel_GetGraveCount(side); i++) {
            if (SzDuel_InqGrave(side, i) == 30) {
                return side << 16 | i;
            }
        }
        for (i = 0; i < SzDuel_GetGraveCount(side == 0 ? 1 : 0); i++) {
            if (SzDuel_InqGrave(side == 0 ? 1 : 0, i) == 30) {
                return i | (side == 0 ? 1 : 0) << 16;
            }
        }
        for (i = 0; i < SzDuel_GetGraveCount(side); i++) {
            if (SzDuel_InqGrave(side, i) == 273) {
                return side << 16 | i;
            }
        }
        for (i = 0; i < SzDuel_GetGraveCount(side == 0 ? 1 : 0); i++) {
            if (SzDuel_InqGrave(side == 0 ? 1 : 0, i) == 273) {
                return i | (side == 0 ? 1 : 0) << 16;
            }
        }
    }
    return -1;
}
