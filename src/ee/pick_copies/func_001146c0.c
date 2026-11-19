#include "dotr/ee/pick_copies.h"

int TaLogic001_GetSpecCardPlay(int side, unsigned char *task, struct DotrStateBlock *unit, int cardNo)
{
    int ids[5];
    int i;
    int *p;

    TaHand_GetNextHand(side, ids);
    i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        p = ids;
        do {
            if ((unsigned short)cardNo == *p && TaHand_IsCardSummon2(side, *p) == 1) {
                task[6] = i;
                SzDuel_InitCardInfo(unit, ids[i]);
                return TaHand_GetCardKind(ids[i]) >> 5;
            }
            p++;
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    return -1;
}
