#include "dotr/ee/pick_copies.h"

int TaLogic007_GetSpecCardPlay(int side, int *ids, struct DotrStateBlock *block, unsigned char *item, int id)
{
    int i;
    int *p;

    i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        p = ids;
        do {
            if (TaHand_IsCardSummon2(side, *p) == 1 && (unsigned short)id == *p && TaHand_IsCardSummon2(side, *p) == 1) {
                item[6] = i;
                SzDuel_InitCardInfo(block, ids[i]);
                return TaHand_GetCardKind(ids[i]) >> 5;
            }
            p++;
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    return -1;
}
