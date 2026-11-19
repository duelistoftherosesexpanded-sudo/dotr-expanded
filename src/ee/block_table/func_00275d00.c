#include "dotr/ee/block_table.h"

/* When TaTuto_IsTuto(side) is 1, hands the byte from SzDuel_GetPlayer(side) and the set to
 * TaTuto_InitTutorial and returns 1. Otherwise returns -1 for a null set, or empties the 22
 * entries and 32 ids, clears the mode, the low four bits at +0x1a1 and the halfword at
 * +0x1a2, stores side << 8 and SzDuel_GetUnitSummonOrder(side, 0) in the first entry and returns 1. */
int TaLogic_InitLogicData(int side, DotrSlotSet *set)
{
    int i;

    if (TaTuto_IsTuto(side) == 1) {
        TaTuto_InitTutorial(SzDuel_GetPlayer(side), set);
        return 1;
    }
    if (set != 0) {
        for (i = 0; i < 22; i++) {
            set->entries[i].a = 0xffff;
            set->entries[i].b = 255;
            set->entries[i].d = -1;
            set->entries[i].c = 0xffff;
            set->entries[i].kind = 0;
        }
        for (i = 0; i < 32; i++) {
            set->ids[i] = 0xffff;
        }
        set->mode = 0;
        set->low_1a1 = 0;
        set->h418 = 0;
        set->entries[0].a = side << 8;
        set->entries[0].b = SzDuel_GetUnitSummonOrder(side, 0);
        return 1;
    }
    return -1;
}
