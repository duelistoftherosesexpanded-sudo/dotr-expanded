#include "dotr/ee/pick_copies.h"

int TaLogic019_GetSpecialComboPlay(int side, unsigned char *item, struct DotrPickBlock *block)
{
    int ids[5];
    int i;
    int found;

    TaHand_GetNextHand(side, ids);
    i = 0;
    found = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        do {
            if (ids[i] == 790) {
                found = 1;
                item[6] = i;
            }
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    if (found == 0) {
        return -1;
    }
    i = 0;
    found = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        do {
            if (ids[i] == 297) {
                if (TaHand_IsCardSummon2(side, ids[i]) == 1) {
                    item[7] = i;
                    found = 1;
                }
            }
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    if (found == 0) {
        return -1;
    }
    SzDuel_InitCardInfo((struct DotrStateBlock *)block, 297);
    return 1;
}
