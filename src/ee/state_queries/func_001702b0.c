#include "dotr/ee/state_queries.h"

int TaLogic022_IsRefuseEmptyCombo(int id)
{
    int side;
    int ok;

    if (TaHand_GetCardKind(id) >> 5 != 0) {
        return 1;
    }
    if (TaHand_IsCardEffect(id) == 1) {
        return 1;
    }
    if (((unsigned)(id - 54) < 4 ? 1 : 0) == 1) {
        side = SzDuel_GetTurnOwner();
        if (SzDuel_GetUnitCardNo(side, 0) == 58 &&
            SzData_CheckDeck(side, SzData_GetCurrentDeck(side), 54) >= 0 &&
            SzData_CheckDeck(side, SzData_GetCurrentDeck(side), 55) >= 0 &&
            SzData_CheckDeck(side, SzData_GetCurrentDeck(side), 56) >= 0 &&
            SzData_CheckDeck(side, SzData_GetCurrentDeck(side), 57) >= 0) {
            ok = 1;
        } else {
            ok = 0;
        }
        if (ok == 1) {
            return 1;
        }
    }
    return 0;
}
