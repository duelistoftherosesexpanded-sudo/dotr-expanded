#include "dotr/ee/state_queries.h"

int TaHand_IsCardSummon2(int side, int cardNo)
{
    int needed = TaHand_GetCardSummonCost2(cardNo, side);
    return SzDuel_GetSummonPower(side) < needed ? 0 : 1;
}
