#include "dotr/ee/pick_copies.h"

int TaLogic002_GetSpecialComboPlay(int side, unsigned char *minTask, struct DotrPickBlock *unit, unsigned char mode)
{
    int ids[8];
    int i;
    int found;

    found = 0;
    TaHand_GetNextHand(side, ids);
    switch (mode) {
    case 0:
        if (SzDuel_GetMapUnit(0, 0) || SzDuel_GetUnitLoc(side, 0) != 1) {
            return -1;
        }
        i = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            do {
                if (ids[i] == 798) {
                    found = 1;
                    minTask[6] = i;
                }
                i++;
            } while (i < TaHand_GetNextHandNum(side));
        }
        if (found == 1) {
            i = 0;
            if (0 < TaHand_GetNextHandNum(side)) {
                do {
                    if ((TaHand_GetCardKind(ids[i]) >> 5) == 0) {
                        if (Ta_GetHandEnchant(ids[i], 798) >= 0) {
                            if (TaHand_IsCardSummon(side, ids[i]) == 1) {
                                minTask[7] = i;
                                SzDuel_InitCardInfo((struct DotrStateBlock *)unit, ids[i]);
                                return TaHand_GetCardKind(ids[i]) >> 5;
                            }
                        }
                    }
                    i++;
                } while (i < TaHand_GetNextHandNum(side));
            }
        }
        break;
    }
    return -1;
}
