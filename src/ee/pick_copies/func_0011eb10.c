#include "dotr/ee/pick_copies.h"

int TaLogic004_GetSpecialComboPlay(int side, unsigned char *task, struct DotrPickBlock *unit, unsigned char mode)
{
    int ids[8];
    int i;
    int found;

    found = 0;
    TaHand_GetNextHand(side, ids);
    switch (mode) {
    case 0:
        i = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            do {
                if (ids[i] == 799) { /* 799: Metalmorph */
                    found = 1;
                    task[6] = i;
                }
                i++;
            } while (i < TaHand_GetNextHandNum(side));
        }
        if (found == 1) {
            i = 0;
            if (0 < TaHand_GetNextHandNum(side)) {
                do {
                    if ((TaHand_GetCardKind(ids[i]) >> 5) == 0) {
                        if (Ta_GetHandEnchant(ids[i], 799) >= 0) {
                            if (TaHand_IsCardSummon2(side, ids[i]) == 1) {
                                if (ids[i] != 795 && ids[i] != 796) {
                                    task[7] = i;
                                    SzDuel_InitCardInfo((struct DotrStateBlock *)unit, ids[i]);
                                    return TaHand_GetCardKind(ids[i]) >> 5;
                                }
                            }
                        }
                    }
                    i++;
                } while (i < TaHand_GetNextHandNum(side));
            }
        }
        break;
    case 1:
        i = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            do {
                if (ids[i] == 514) { /* 514: Machine King, if affordable */
                    if (TaHand_IsCardSummon2(side, ids[i]) == 1) {
                        task[6] = i;
                        SzDuel_InitCardInfo((struct DotrStateBlock *)unit, ids[i]);
                        return TaHand_GetCardKind(ids[i]) >> 5;
                    }
                }
                i++;
            } while (i < TaHand_GetNextHandNum(side));
        }
        break;
    }
    return -1;
}
