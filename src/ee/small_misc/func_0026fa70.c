#include "dotr/ee/small_misc.h"

int TaHand_GetCardSummonCost(int cardNo)
{
    return TaHand_GetCardSummonCost2(cardNo&0xfff);
}
