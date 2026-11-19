#include "dotr/ee/lower_rules.h"

int TaLogic019_IsUseSaint(int side)
{
    int i;

    if (SzDuel_GetUnitSpellCount(side) >= 5) {
        return -1;
    }
    if (TaLogic_GetGraveCardCount(side, 3, 700) > 0 || TaLogic_GetGraveCardCount(side, 3, 715) > 0 || TaLogic_GetGraveCardCount(side, 3, 827) > 0) {
        for (i = 0; i < SzDuel_GetGraveCount(side); i++) {
            if (SzDuel_InqGrave(side, i) == 827) {
                return side << 16 | i;
            }
        }
        for (i = 0; i < SzDuel_GetGraveCount(side == 0 ? 1 : 0); i++) {
            if (SzDuel_InqGrave(side == 0 ? 1 : 0, i) == 827) {
                return i | (side == 0 ? 1 : 0) << 16;
            }
        }
        for (i = 0; i < SzDuel_GetGraveCount(side); i++) {
            if (SzDuel_InqGrave(side, i) == 700) {
                return side << 16 | i;
            }
        }
        for (i = 0; i < SzDuel_GetGraveCount(side == 0 ? 1 : 0); i++) {
            if (SzDuel_InqGrave(side == 0 ? 1 : 0, i) == 700) {
                return i | (side == 0 ? 1 : 0) << 16;
            }
        }
        for (i = 0; i < SzDuel_GetGraveCount(side); i++) {
            if (SzDuel_InqGrave(side, i) == 715) {
                return side << 16 | i;
            }
        }
        for (i = 0; i < SzDuel_GetGraveCount(side == 0 ? 1 : 0); i++) {
            if (SzDuel_InqGrave(side == 0 ? 1 : 0, i) == 715) {
                return i | (side == 0 ? 1 : 0) << 16;
            }
        }
    }
    return -1;
}
