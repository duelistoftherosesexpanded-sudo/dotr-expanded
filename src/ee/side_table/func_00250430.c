#include "dotr/ee/side_table.h"

int Ta_GetRecycle(int nSide, int cNo)
{
    int i;
    int Num;
    int ReqCost;
    int fCreature;
    int fFinish;
    struct jp_8d5b *pInfo;
    int Card[854];

    for (i = 0; i < 854; i++) {
        Card[i] = 999;
    }
    pInfo = (struct jp_8d5b *)SzDuel_GetCardConstInfo(cNo);
    if ((pInfo->bKind >> 5) == 0) {
        fCreature = func_0019a5c8() % 10 < 8 ? 1 : 0;
    } else {
        fCreature = func_0019a5c8() % 10 < 4 ? 0 : 1;
    }
    fFinish = 0;
    Num = 0;
    ReqCost = Ta_GetRecycleRequestLvl(nSide, pInfo->bDeckCost);
    if (ReqCost > 0) {
        do {
            for (i = fCreature ? 0 : 683; i < (fCreature ? 683 : 854); i++) {
                if (i == cNo) {
                    continue;
                }
                pInfo = (struct jp_8d5b *)SzDuel_GetCardConstInfo(i);
                if (pInfo != 0 && pInfo->bDeckCost == ReqCost && pInfo->fGetRecycle) {
                    Card[Num++] = i;
                }
            }
            if (Num > 0 || --ReqCost == 0) {
                fFinish = 1;
            } else {
                Num = 0;
            }
        } while (!fFinish);
    }
    if (Num == 0) {
        return 820;
    }
    return Card[func_0019a5c8() % Num];
}
