#include "dotr/ee/pick_copies.h"

int TaLogic005_GetSpecialCardPlay(int side, unsigned char *minTask, struct DotrStateBlock *unit, int targetCard)
{
    int ids[5];
    int i;
    int *p;

    TaHand_GetNextHand(side, ids);
    i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        p = ids;
        do {
            if ((unsigned short)targetCard == *p && TaHand_IsCardSummon2(side, *p) == 1) {
                minTask[6] = i;
                SzDuel_InitCardInfo(unit, ids[i]);
                return TaHand_GetCardKind(ids[i]) >> 5;
            }
            p++;
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    return -1;
}
