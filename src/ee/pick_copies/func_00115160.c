#include "dotr/ee/pick_copies.h"

int TaLogic001_SetSpecialComboQue(int side, unsigned char *task, struct DotrPickBlock *unit)
{
    int list[8];
    int i;
    int *p;
    unsigned char n;
    int *q;
    int j;

    for (i = 0; i < 5; i++) {
        task[i + 6] = 0xFF;
    }
    TaHand_GetNextHand(side, list);
    if (unit == 0) {
        return -1;
    }
    i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        p = list;
        do {
            if (i != task[6] && i != task[7] && unit->cardNo == *p) {
                task[6] = i;
            }
            p++;
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    n = 1;
    if (TaHand_GetCardKind(list[task[6]]) >> 5 == 0) {
        j = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            q = list;
            do {
                if (j != task[6] && j != task[7] && Ta_GetHandEnchant(list[task[6]], *q) >= 0
                    && TaHand_GetCardKind(*q) >> 5 == 2 && *q != 795 && *q != 796) {
                    task[n + 6] = j;
                    n++;
                }
                q++;
                j++;
            } while (j < TaHand_GetNextHandNum(side));
        }
    }
    return 1;
}
