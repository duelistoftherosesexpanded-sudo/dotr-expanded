#include "dotr/ee/state_queries.h"

int TaLogic005_Is2Head_ThunderDragon(int side, int *list)
{
    int i;
    unsigned char mask;

    mask = 0;
    for (i = 0; i < TaHand_GetNextHandNum(side); i++) {
        if (TaHand_IsCardSummon2(side, list[i]) == 1 && TaHand_GetCardKind(list[i]) >> 5 == 0) {
            mask += 1 << (4 - i);
            i = TaHand_GetNextHandNum(side);
        }
    }
    if (mask == 0) {
        return 0;
    }
    for (i = 0; i < TaHand_GetNextHandNum(side); i++) {
        if (TaHand_IsCardSummon2(side, list[i]) == 1 && TaHand_GetCardKind(list[i]) >> 5 == 18) {
            mask += 1 << (4 - i);
            i = TaHand_GetNextHandNum(side);
        }
    }
    if (mask == 2) {
        for (i = 0; i < TaHand_GetNextHandNum(side); i++) {
            if (TaHand_IsCardSummon2(side, list[i]) == 1 && TaHand_GetCardKind(list[i]) >> 5 == 15) {
                mask += 1 << (4 - i);
                i = TaHand_GetNextHandNum(side);
            }
        }
        return mask;
    }
    return 0;
}
