#include "dotr/ee/driver_scans.h"

int TaLogic007_IsActiveMode(void)
{
    int own;
    int i;
    unsigned char bad;
    unsigned char other;
    unsigned char mine;

    own = SzDuel_GetTurnOwner();
    bad = 0;
    mine = 0;
    for (i = 0; i < 11; i++) {
        if (SzDuel_GetUnitCurseRest(own, i) > 0 || SzDuel_GetUnitSpellRest(own, i) > 0 ||
            own != Ta_IsUnitControl(own, i)) {
            bad++;
        }
        if (SzDuel_GetUnitCurseRest((own == 0 ? 1 : 0), i) > 0 || SzDuel_GetUnitSpellRest((own == 0 ? 1 : 0), i) > 0 ||
            own != Ta_IsUnitControl((own == 0 ? 1 : 0), i)) {
            bad++;
        }
    }
    if (bad >= 4) {
        return 0;
    }
    other = 0;
    for (i = 0; i < 11; i++) {
        if (own != Ta_IsUnitControl(own, i)) {
            other++;
        }
        if (own != Ta_IsUnitControl((own == 0 ? 1 : 0), i)) {
            other++;
        }
    }
    for (i = 0; i < 11; i++) {
        if (own == Ta_IsUnitControl(own, i)) {
            mine++;
        }
        if (own == Ta_IsUnitControl((own == 0 ? 1 : 0), i)) {
            mine++;
        }
    }
    if (other < 2 && mine >= 4) {
        return 1;
    }
    return 0;
}
