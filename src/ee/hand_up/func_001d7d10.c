#include "dotr/ee/hand_up.h"

void lSzDuel_InitHandUp(int side, int draw)
{
    unsigned char *duel = data_00317490.unknown_000;
    unsigned char *player = data_00317490.unknown_000 + side * 848;
    int i;
    int x;
    int card;
    int nextHandSize;
    int deckCount;
    int canDestinyDraw;
    unsigned short ddCardPool[4];
    short ddPoolOdds[4];

    SzDuel_PackHand(side);
    lSzDuel_InitHandInfo(side, 0, 999, 0, 0, 0, 0);
    x = -250;
    for (i = 0; i < duel[3401]; i++) {
        card = SzDuel_InqHand(side, i);
        if (card < 0) {
            card = 999;
        } else {
            card &= 0xfff;
        }
        lSzDuel_InitHandInfo(side, i + 1, card, x, 448, x, -8);
        x += 125;
    }
    if (draw != 0) {
        nextHandSize = SzDuel_GetHandCount(side);
        deckCount = SzDuel_GetDeckCount(side);
        nextHandSize = nextHandSize + deckCount < 5 ? nextHandSize + deckCount : 5;
        canDestinyDraw = 0;
        /* MATCHING DEVICE: the `!= 0` makes the compiler keep the first two tests as a value. */
        if ((deckCount > 0 && player[836] > 0) != 0 && !(player[836] & 0x80)) {
            canDestinyDraw = 1;
        }
        if (canDestinyDraw) {
            nextHandSize = nextHandSize + 1 < 5 ? nextHandSize + 1 : 5;
        }
        for (; i < nextHandSize; i++) {
            card = 999;
            if (canDestinyDraw && i + 1 >= nextHandSize && Ta_GetDestinyDraw(side, ddCardPool, ddPoolOdds, 0) >= 0) {
                short roll = func_001a22c0(100.0f * ((float)func_0019a5c8() / 2147483648.0f));
                int pick = 2;
                if (roll < ddPoolOdds[0]) {
                    pick = 0;
                } else if (roll < ddPoolOdds[0] + ddPoolOdds[1]) {
                    pick = 1;
                }
                card = ddCardPool[pick];
            }
            if (card == 999) {
                card = SzDuel_DrawDeck(side);
            }
            SzDuel_AddHand(side, card);
            lSzDuel_InitHandInfo(side, i + 1, card, 750, 448, 750, -8);
        }
        for (; i < 5; i++) {
            SzDuel_AddHand(side, 999);
            lSzDuel_InitHandInfo(side, i + 1, 999, 0, 0, 0, 0);
        }
    } else {
        for (; i < 5; i++) {
            lSzDuel_InitHandInfo(side, i + 1, 999, 0, 0, 0, 0);
        }
    }
    duel[3402] = SzDuel_GetHandCount(side);
    for (i = 0; i < duel[3402]; i++) {
        duel[3372 + i] = i + 1;
    }
}
