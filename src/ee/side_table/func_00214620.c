#include "dotr/ee/side_table.h"

void SzData_ClearData(int side)
{
    int i;
    struct DotrSide *saveData = DOTR_SIDE(side);
    struct DotrSideList *q;
    int j;
    int k;

    for (i = 0; i < 854; i++) {
        for (j = 0; j < 9; j++) {
            SzData_ClearCardInfo(&saveData->cards[i].copies[j]);
        }
        saveData->cards[i].rank = 0;
        saveData->cards[i].exp = 0;
        saveData->cards[i].unknown_48_bit_14 = 0;
        saveData->cards[i].unknown_48_bit_15 = 0;
    }
    for (i = 0; i < 5; i++) {
        saveData->newcomers[i] = 999;
    }
    for (i = 0; i < 3; i++) {
        q = DOTR_SIDE_LIST(saveData, i);
        for (j = 0; j < 40; j++) {
            q->cards[j] = 999;
        }
        q->leader = 999;
        q->deckCost = 0;
    }
    for (i = 0; i < 16; i++) {
        saveData->ownedAltArts[i] = 0;
    }
    for (i = 0; i < 22; i++) {
        saveData->treasureFlags[i] = 0;
    }
    for (k = 0; k < 5; k++) {
        for (i = 0; i < 7; i++) {
            for (j = 0; j < 7; j++) {
                saveData->unknown_fea3[k][i][j] = 7;
            }
        }
    }
    saveData->unknown_ff98 = 0;
    saveData->flags.unknown_bits_0_1 = 0;
    saveData->flags.unknown_bit_2 = 1;
    saveData->flags.unknown_bit_3 = 1;
    saveData->flags.unknown_bit_4 = 1;
    saveData->flags.unknown_bit_5 = 0;
}
