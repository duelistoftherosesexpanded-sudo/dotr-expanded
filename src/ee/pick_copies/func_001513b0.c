#include "dotr/ee/pick_copies.h"

int TaLogic015_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *block, int cardId)
{
    int hand[5];
    int i;
    int *currentCard;

    TaHand_GetNextHand(side, hand);
    i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        currentCard = hand;
        do {
            if ((unsigned short)cardId == *currentCard && TaHand_IsCardSummon2(side, *currentCard) == 1) {
                item[6] = i;
                SzDuel_InitCardInfo(block, hand[i]);
                return TaHand_GetCardKind(hand[i]) >> 5;
            }
            currentCard++;
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    return -1;
}
