#include "dotr/ee/pick_copies.h"

int TaLogic003_IsSpellPlay(int side, unsigned char *minTask, struct DotrPickBlock *unit)
{
    unsigned short ownerIdArray[24];
    int list[8];
    int i;
    unsigned short *w;
    int strongestUnit;
    int id;
    int *p;
    int j;

    w = ownerIdArray;
    for (i = 0; i < 22; i++) {
        *w++ = 0xFFFF;
    }
    TaUtil_GetUnitLst(side, 2, ownerIdArray);
    TaHand_GetNextHand(side, list);
    if (unit == 0) {
        return -1;
    }
    for (i = 0; i < 5; i++) {
        minTask[i + 6] = 0xFF;
    }
    strongestUnit = TaLogic_GetMaxApUnit(ownerIdArray);
    if (strongestUnit >= 0) {
        switch (SzDuel_GetUnitKind(strongestUnit >> 8, strongestUnit & 0xFF) & 0x1F) {
        case 14:
            id = 722; /* Stain Storm */
            break;
        case 2:
            id = 721; /* Eternal Rest */
            break;
        case 18:
            id = 724; /* Breath of Light */
            break;
        default:
            id = 999;
            break;
        }
        if (id != 999) {
            j = 0;
            if (0 < TaHand_GetNextHandNum(side)) {
                p = list;
                do {
                    if (id == *p) {
                        minTask[6] = j;
                    }
                    p++;
                    j++;
                } while (j < TaHand_GetNextHandNum(side));
            }
            return id;
        }
    }
    return -1;
}
