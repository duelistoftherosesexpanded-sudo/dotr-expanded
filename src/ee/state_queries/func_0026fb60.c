#include "dotr/ee/state_queries.h"

int TaHand_IsCardSummon(int index, int value)
{
    int needed = TaHand_GetCardSummonCost2(value & 0xfff, index);
    return SzDuel_GetSummonPower(index) < needed ? 0 : 1;
}
