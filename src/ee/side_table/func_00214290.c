#include "dotr/ee/side_table.h"

static inline void reset(struct DotrSideEntry *e)
{
    e->unknown_00_bits_0_30 = 0;
    e->unknown_04_bits_0_29 = 0;
    e->unknown_04_bits_30_31 = 3;
    e->unknown_00_bit_31 = 0;
}

void SzData_Initialize(void)
{
    int i;
    struct DotrSide *saveData;
    struct DotrSideList *q;
    int j;
    int k;
    int a;

    RhMem_FillMemory(gSzData_All_sides, 0, sizeof(gSzData_All_sides));
    for (a = 0; a < 2; a++) {
        saveData = &gSzData_All_sides[a];
        for (i = 0; i < 854; i++) {
            for (j = 0; j < 9; j++) {
                reset(&saveData->cards[i].copies[j]);
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
        saveData->flags.unknown_bits_0_1 = 0;
        saveData->flags.unknown_bit_2 = 1;
        saveData->flags.unknown_bit_3 = 1;
        saveData->flags.unknown_bit_4 = 1;
        saveData->flags.unknown_bit_5 = 1;
    }
}
