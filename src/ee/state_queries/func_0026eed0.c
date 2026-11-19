#include "dotr/ee/state_queries.h"

int Ta_IsDestinyDraw(int side)
{
    int k;
    int v;
    k = SzDuel_GetPlayer(SzDuel_GetTurnOwner());
    if (k < 32) {
        if (k != 19 && k != 1) {
            return -1;
        }
    }
    v = SzData_GetRank(side, SzData_GetLeader(side, SzData_GetCurrentDeck(side)));
    if (k > 99) {
        if (Ta_IsLDAbl(3, side) == 0) {
            return -1;
        }
    }
    if (SzDuel_GetLP(side) < 1000) {
        if ((k = TaHand_IsDestinyCondition(side)) >= 0) {
            return k | (GetCap(0, v - 8) << 4);
        }
    }
    return -1;
}
