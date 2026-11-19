#include "dotr/ee/pick_copies.h"

int TaLogic022_SetSpecialComboQue(int side, unsigned char *task, struct DotrPickTotals *unit)
{
    int ids[5];
    int i;
    int *p;
    unsigned char n;
    int *q;
    int j;

    for (i = 0; i < 5; i++) {
        task[i + 6] = 0xff;
    }
    TaHand_GetNextHand(side, ids);
    if (unit == 0) {
        return -1;
    }
    i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        p = ids;
        do {
            if (i != task[6] && i != task[7] && unit->id_3c == *p && TaHand_IsCardSummon2(side, *p) == 1) {
                task[6] = i;
            }
            p++;
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    n = 1;
    if (TaHand_GetCardKind(ids[task[6]]) >> 5 == 0) {
        j = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            q = ids;
            do {
                if (j != task[6] && j != task[7] && Ta_GetHandEnchant(ids[task[6]], *q) >= 0 &&
                    TaHand_GetCardKind(*q) >> 5 == 2 && *q != 795 && *q != 796) {
                    task[n + 6] = j;
                    n++;
                    unit->total_38 += (short)TaEnc_GetEncScore(*q);
                    unit->total_3a += (short)TaEnc_GetEncScore(*q);
                }
                q++;
                j++;
            } while (j < TaHand_GetNextHandNum(side));
        }
    }
    return 1;
}
