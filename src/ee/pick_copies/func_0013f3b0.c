#include "dotr/ee/pick_copies.h"

int TaLogic011_GetSpecComboPlay(int side, unsigned char *minTask, struct DotrPickBlock *targetUnit)
{
    int ids[8];
    int i;
    int found;

    found = 0;
    TaHand_GetNextHand(side, ids);
    i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        do {
            if (ids[i] == 800) { /* 800: Insect Imitation */
                minTask[7] = i;
                SzDuel_InitCardInfo((struct DotrStateBlock *)targetUnit, ids[i]);
                found = 1;
            }
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    if (found == 1) {
        i = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            do {
                if ((TaHand_GetCardKind(ids[i]) >> 5) == 0) {
                    if (Ta_GetHandEnchant(ids[i], 800) >= 0) {
                        if (TaHand_IsCardSummon(side, ids[i]) == 1) {
                            if (ids[i] != 795 && ids[i] != 796) { /* 795, 796: Paralyzing Potion, Cursebreaker */
                                minTask[7] = i;
                                SzDuel_InitCardInfo((struct DotrStateBlock *)targetUnit, ids[i]);
                                return TaHand_GetCardKind(ids[i]) >> 5;
                            }
                        }
                    }
                }
                i++;
            } while (i < TaHand_GetNextHandNum(side));
        }
    }
    return -1;
}
