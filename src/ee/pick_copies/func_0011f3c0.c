#include "dotr/ee/pick_copies.h"

int TaLogic_SetSpecialComboQue__0011f3c0(int side, unsigned char *item, struct DotrPickBlock *unit, unsigned char mode)
{
    int list[8];
    int i;
    int *p;

    if (unit == 0) {
        return -1;
    }
    for (i = 0; i < 5; i++) {
        item[i + 6] = 0xFF;
    }
    TaHand_GetNextHand(side, list);
    if (mode == 0) {
        TaLogic004_GetSpecialComboPlay(side, item, unit, 0);
        i = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            p = list;
            do {
                if (i != item[6] && i != item[7] && Ta_GetHandEnchant(488, *p) >= 0 && TaHand_GetCardKind(*p) >> 5 == 2
                    && *p != 795 && *p != 796) {
                    item[8] = i;
                }
                p++;
                i++;
            } while (i < TaHand_GetNextHandNum(side));
        }
    } else if (mode == 1) {
        TaLogic004_GetSpecialComboPlay(side, item, unit, 0);
    }
    return 0;
}
