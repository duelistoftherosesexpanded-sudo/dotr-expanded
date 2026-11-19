#include "dotr/ee/small_misc.h"

int TaLogic002_IsActiveMode(void)
{
    unsigned short units[22];
    int side;
    int j;
    unsigned short *q;
    int i;
    unsigned short *p;
    unsigned short *fill;
    unsigned short cardId;

    side = SzDuel_GetTurnOwner();
    q = units;
    for (i = 0, fill = q; i < 22; i++, fill++) {
        *fill = 0xffff;
    }
    TaUtil_GetUnitLst(side, 1, units);
    for (i = 0, p = q; i < 22; i++, p++) {
        if (*p != 0xffff) {
            cardId = SzDuel_GetUnitCardNo(*p >> 8, *p & 0xff) & 0xffff;
            if (cardId == 402 || cardId == 403 || cardId == 422) {
                return 1;
            }
        }
    }
    for (j = 0; j < 22; j++, q++) {
        if (*q != 0xffff) {
            cardId = SzDuel_GetUnitCardNo(*q >> 8, *q & 0xff) & 0xffff;
            if (cardId == 848 || cardId == 798) {
                return 0;
            }
        }
    }
    for (j = 0; j < 5; j++) {
        cardId = SzDuel_InqHand(side, j) & 0xfff;
        if (cardId == 848 || cardId == 798) {
            return 0;
        }
    }
    for (j = 0; j < SzDuel_GetDeckCount(side); j++) {
        cardId = SzDuel_InqDeck(side, j) & 0xffff;
        if (cardId == 848 || cardId == 798) {
            return 0;
        }
    }
    return 1;
}
