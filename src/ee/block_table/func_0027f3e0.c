#include "dotr/ee/block_table.h"

/* Resets the set through TaTuto_InitTutoData, then kind 32 calls TaTuto_Init032,
 * kind 35 calls SzDuel_CalcSummonPower(0, 4, 8) and (1, 4, 8), kind 36 calls
 * TaTuto_Init036. */
void TaTuto_InitTutorial(unsigned char kind, DotrSlotSet *set)
{
    TaTuto_InitTutoData(set);
    switch (kind) {
    case 32:
        TaTuto_Init032();
        break;
    case 35:
        SzDuel_CalcSummonPower(0, 4, 8);
        SzDuel_CalcSummonPower(1, 4, 8);
        break;
    case 36:
        TaTuto_Init036();
        break;
    }
}
