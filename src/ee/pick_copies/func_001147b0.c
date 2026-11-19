#include "dotr/ee/pick_copies.h"

/* The body of TaLogic001_GetSpecCardPlay with a constant id, expanded five times in the original. */
static inline int func_001147b0_try(int side, unsigned char *task, struct DotrStateBlock *unit, int id)
{
    int ids[5];
    int *p;
    int i;

    TaHand_GetNextHand(side, ids);
    i = 0;
    if (0 < TaHand_GetNextHandNum(side)) {
        p = ids;
        do {
            if ((unsigned short)id == *p && TaHand_IsCardSummon2(side, *p) == 1) {
                task[6] = i;
                SzDuel_InitCardInfo(unit, ids[i]);
                return TaHand_GetCardKind(ids[i]) >> 5;
            }
            p++;
            i++;
        } while (i < TaHand_GetNextHandNum(side));
    }
    return -1;
}

int TaLogic001_GetProtectCardPlay(int side, unsigned char *task, struct DotrStateBlock *unit)
{
    int result;

    result = func_001147b0_try(side, task, unit, 823);
    if (result >= 0) {
        return result;
    }
    result = func_001147b0_try(side, task, unit, 821);
    if (result >= 0) {
        return result;
    }
    result = func_001147b0_try(side, task, unit, 824);
    if (result >= 0) {
        return result;
    }
    result = func_001147b0_try(side, task, unit, 828);
    if (result >= 0) {
        return result;
    }
    result = func_001147b0_try(side, task, unit, 825);
    if (result >= 0) {
        return result;
    }
    return -1;
}
