#include "dotr/ee/state_queries.h"

int TaLogic_IsTactixExz__00112980(int side)
{
    if (SzDuel_GetUnitCardNo(side, 0) == 58 &&
        SzData_CheckDeck(side, SzData_GetCurrentDeck(side), 54) >= 0 &&
        SzData_CheckDeck(side, SzData_GetCurrentDeck(side), 55) >= 0 &&
        SzData_CheckDeck(side, SzData_GetCurrentDeck(side), 56) >= 0 &&
        SzData_CheckDeck(side, SzData_GetCurrentDeck(side), 57) >= 0) {
        return 1;
    }
    return 0;
}
