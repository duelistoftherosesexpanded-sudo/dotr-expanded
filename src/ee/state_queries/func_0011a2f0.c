#include "dotr/ee/state_queries.h"

void TaLogic_ConvEmptyCombo__0011a2f0(int side, unsigned short *fusionTree, int *table)
{
    unsigned char buf[5];
    int count;
    unsigned char *p;
    unsigned char *q;
    int i;
    int id;
    int lim;
    unsigned char limit;
    unsigned char *fill;
    unsigned r;

    if (fusionTree != 0) {
        limit = TaHand_GetCardSummonCost2(fusionTree[0], side);
        q = buf;
        for (i = 0, fill = q; i < 5; i++, fill++) {
            *fill = 0xff;
        }
        i = 0;
        count = 0;
        if (0 < TaHand_GetNextHandNum(side)) {
            p = q;
            do {
                if ((TaLogic_IsUseHandInCombo(i, (unsigned char *)(fusionTree + 1)) & 0xff) == 0) {
                    *p++ = i;
                    count++;
                }
                i++;
            } while (i < TaHand_GetNextHandNum(side));
        }
        if (count != 0) {
            lim = limit;
            for (i = 0; i < (TaLogic_GetArrayLength(buf) & 0xff); q++, i++) {
                id = table[*q];
                if (id != 999 && lim >= TaHand_GetCardSummonCost2(id, side) &&
                    (r = TaHand_GetCardAPDP(id), (r & 0xffff) + (r >> 16) * 2 <= 3500) &&
                    TaHand_IsCardEffect(id) == 2 && (TaLogic_GetHandFusion(fusionTree[0], id) & 0xffff) == 999) {
                    TaLogic_AddEmptyCombo((unsigned char *)(fusionTree + 1), *q);
                }
            }
        }
    }
}
