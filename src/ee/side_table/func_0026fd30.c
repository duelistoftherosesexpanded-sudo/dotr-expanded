#include "dotr/ee/side_table.h"

void TaHand_GetNextHand(int index, int *out)
{
    int i;
    int n;
    int *pHand;
    int cardId;

    for (i = 0, n = 0, pHand = out; i < 5; i++) {
        cardId = SzDuel_InqHand(index, i) & 0xfff;
        if (cardId != 999) {
            *pHand++ = cardId;
            n++;
        }
    }
    i = SzDuel_GetHandCount(index);
    for (n = 0; i < TaHand_GetNextHandNum(index); i++, n++) {
        out[i] = SzDuel_InqDeck(index, n);
    }
}
