#include "dotr/ee/state_records.h"
int TaTuto_DbgInit034(void)
{
    int i, n, r;
    n = SzDuel_GetDeckCount(0);
    for (i = 0; i < n; i++) {
        SzDuel_DelDeck(0, i);
    }
    n = SzDuel_GetDeckCount(1);
    for (i = 0; i < n; i++) {
        SzDuel_DelDeck(1, i);
    }
    SzDuel_SetDeck(0, 0, 321);
    SzDuel_SetDeck(0, 1, 285);
    SzDuel_SetDeck(0, 2, 156);
    SzDuel_SetDeck(0, 3, 249);
    SzDuel_SetDeck(0, 4, 505);
    SzDuel_SetDeck(1, 0, 21);
    SzDuel_SetDeck(1, 1, 423);
    SzDuel_SetDeck(1, 2, 522);
    SzDuel_SetDeck(1, 3, 12);
    SzDuel_SetDeck(1, 4, 433);
    for (i = 0; i < 5; i++) {
        SzDuel_DelHand(0, i);
    }
    for (i = 0; i < 5; i++) {
        r = SzDuel_DelHand(1, i);
    }
    return r;
}
