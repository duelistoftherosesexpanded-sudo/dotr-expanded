#include "dotr/ee/hand_finds.h"

int TaLogic001_IsUseRevive(int side)
{
    int i;

    if (SzDuel_GetUnitMonsterCount(side) >= 5) {
        return -1;
    }
    /* Grave counts of card 0 (Blue-Eyes White Dragon) and card 2 (Blue-Eyes Ultimate Dragon). */
    if (TaLogic_GetGraveCardCount(side, 3, 0) > 0 || TaLogic_GetGraveCardCount(side, 3, 2) > 0) {
        /* No unit is card 0x179 (Hourglass of Life; search type 28 = card number). */
        if (SearchUnitByRange(0, 0, side, 1, 0x179001C, 14) == 0) {
            for (i = 0; i < SzDuel_GetGraveCount(side); i++) {
                if (SzDuel_InqGrave(side, i) == 2) {
                    return side << 16 | i;
                }
            }
            for (i = 0; i < SzDuel_GetGraveCount(side == 0 ? 1 : 0); i++) {
                if (SzDuel_InqGrave(side == 0 ? 1 : 0, i) == 2) {
                    return i | (side == 0 ? 1 : 0) << 16;
                }
            }
            for (i = 0; i < SzDuel_GetGraveCount(side); i++) {
                if (SzDuel_InqGrave(side, i) == 0) {
                    return side << 16 | i;
                }
            }
            for (i = 0; i < SzDuel_GetGraveCount(side == 0 ? 1 : 0); i++) {
                if (SzDuel_InqGrave(side == 0 ? 1 : 0, i) == 0) {
                    return i | (side == 0 ? 1 : 0) << 16;
                }
            }
        }
    }
    return -1;
}
