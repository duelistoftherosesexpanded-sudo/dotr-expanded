#include "dotr/ee/pick_copies.h"

int TaLogic007_GetCurseCardPlay(int side, int *ids, struct DotrPickBlock *unit, unsigned char *minTask)
{
    int i;

    i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        do {
            if (SzDuel_Kind2Type(TaHand_GetCardKind(ids[i]) >> 5) == 2) { /* any trap */
                minTask[6] = i;
                SzDuel_InitCardInfo((struct DotrStateBlock *)unit, ids[i]);
                return TaHand_GetCardKind(ids[i]) >> 5;
            }
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        do {
            if (TaHand_IsCardSummon2(side, ids[i]) == 1) {
                if (ids[i] == 795 || ids[i] == 684 || ids[i] == 311) { /* Paralyzing Potion, Time Seal, The Shadow Who Controls the Dark */
                    minTask[6] = i;
                    SzDuel_InitCardInfo((struct DotrStateBlock *)unit, ids[i]);
                    return TaHand_GetCardKind(ids[i]) >> 5;
                }
            }
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    return -1;
}
