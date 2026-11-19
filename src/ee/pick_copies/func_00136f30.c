#include "dotr/ee/pick_copies.h"

int TaLogic009_SetSpecialComboQue(int side, unsigned char *minTask, struct DotrPickTotals *actionUnit)
{
    int ids[5];
    int i;
    int *p;
    unsigned char n;
    int *q;
    int j;

    for (i = 0; i < 5; i++) {
        minTask[i + 6] = 0xff;
    }
    TaHand_GetNextHand(side, ids);
    if (actionUnit == 0) {
        return -1;
    }
    i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        p = ids;
        do {
            if (i != minTask[6] && i != minTask[7] && actionUnit->id_3c == *p && TaHand_IsCardSummon2(side, *p) == 1) {
                minTask[6] = i;
            }
            p++;
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    n = 1;
    if (TaHand_GetCardKind(ids[minTask[6]]) >> 5 == 0) {
        j = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            q = ids;
            do {
                if (j != minTask[6] && j != minTask[7] && Ta_GetHandEnchant(ids[minTask[6]], *q) >= 0 &&
                    TaHand_GetCardKind(*q) >> 5 == 2 && *q != 795 && *q != 796) { /* 795, 796: Paralyzing Potion, Cursebreaker */
                    minTask[n + 6] = j;
                    n++;
                }
                q++;
                j++;
            } while (j < TaHand_GetNextHandNum(side));
        }
    }
    return 1;
}
