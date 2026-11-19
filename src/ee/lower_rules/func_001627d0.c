#include "dotr/ee/lower_rules.h"

int TaLogic019_IsUseRevive(int side)
{
    int i;

    if (SzDuel_GetUnitMonsterCount(side) >= 5) {
        return -1;
    }
    if (TaLogic_GetGraveCardCount(side, 3, 60) > 0 || TaLogic_GetGraveCardCount(side, 3, 87) > 0 || TaLogic_GetGraveCardCount(side, 3, 43) > 0) {
        for (i = 0; i < SzDuel_GetGraveCount(side); i++) {
            if (SzDuel_InqGrave(side, i) == 87) {
                return side << 16 | i;
            }
        }
        for (i = 0; i < SzDuel_GetGraveCount(side == 0 ? 1 : 0); i++) {
            if (SzDuel_InqGrave(side == 0 ? 1 : 0, i) == 87) {
                return i | (side == 0 ? 1 : 0) << 16;
            }
        }
        for (i = 0; i < SzDuel_GetGraveCount(side); i++) {
            if (SzDuel_InqGrave(side, i) == 43) {
                return side << 16 | i;
            }
        }
        for (i = 0; i < SzDuel_GetGraveCount(side == 0 ? 1 : 0); i++) {
            if (SzDuel_InqGrave(side == 0 ? 1 : 0, i) == 43) {
                return i | (side == 0 ? 1 : 0) << 16;
            }
        }
        for (i = 0; i < SzDuel_GetGraveCount(side); i++) {
            if (SzDuel_InqGrave(side, i) == 60) {
                return side << 16 | i;
            }
        }
        for (i = 0; i < SzDuel_GetGraveCount(side == 0 ? 1 : 0); i++) {
            if (SzDuel_InqGrave(side == 0 ? 1 : 0, i) == 60) {
                return i | (side == 0 ? 1 : 0) << 16;
            }
        }
    }
    return -1;
}
