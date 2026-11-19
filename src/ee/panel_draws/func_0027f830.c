#include "dotr/ee/panel_draws.h"

int TaTuto_IsTuto(int side)
{
    int ok = 0;

    if (SzDuel_GetPlayer(side) >= 32 && SzDuel_GetPlayer(side) < 100) {
        ok = 1;
    }
    return ok ? 1 : 0;
}
