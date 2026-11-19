#include "dotr/ee/pick_copies.h"

int TaLogic021_GetSpecCardPlay(int side, unsigned char *item, struct DotrStateBlock *unit, int id)
{
    int ids[5];
    int i;
    int *p;

    TaHand_GetNextHand(side, ids);
    i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        p = ids;
        do {
            if ((unsigned short)id == *p && TaHand_IsCardSummon2(side, *p) == 1) {
                item[6] = i;
                SzDuel_InitCardInfo(unit, ids[i]);
                return TaHand_GetCardKind(ids[i]) >> 5;
            }
            p++;
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    return -1;
}
