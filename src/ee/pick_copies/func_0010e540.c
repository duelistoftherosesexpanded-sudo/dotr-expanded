#include "dotr/ee/pick_copies.h"

int TaLogic_SetSpecialComboQue__0010e540(int side, unsigned char *item, struct DotrPickBlock *block, unsigned char mode)
{
    int list[8];
    int i;
    int n;
    int *p;

    if (block == 0) {
        return -1;
    }
    for (i = 0; i < 5; i++) {
        item[i + 6] = 0xFF;
    }
    TaHand_GetNextHand(side, list);
    if (mode == 0) {
        TaLogic002_GetSpecialComboPlay(side, item, block, 0);
        n = 2;
        if (3 < Taku_Get_WinCount()) {
            i = 0;
            if (0 < TaHand_GetNextHandNum(side)) {
                p = list;
                do {
                    if (i != item[6] && i != item[7] && Ta_GetHandEnchant(427, *p) >= 0 && TaHand_GetCardKind(*p) >> 5 == 2
                        && *p != 795 && *p != 796) {
                        item[n++ + 6] = i;
                    }
                    p++;
                    i++;
                } while (i < TaHand_GetNextHandNum(side));
            }
        }
    }
    return 0;
}
