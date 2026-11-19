#include "dotr/ee/grid_scans.h"

int TaLogic009_IsUseCopyCat(int side)
{
    int i;

    if (SzDuel_GetUnitSpellCount(side) < 5) {
        for (i = 0; i < SzDuel_GetGraveCount(side); i++) {
            if (SzDuel_InqGrave(side, i) == 713) {
                return side << 16 | i;
            }
        }
        for (i = 0; i < SzDuel_GetGraveCount(side == 0 ? 1 : 0); i++) {
            if (SzDuel_InqGrave((side == 0 ? 1 : 0), i) == 713) {
                return i | (side == 0 ? 1 : 0) << 16;
            }
        }
    }
    return -1;
}
