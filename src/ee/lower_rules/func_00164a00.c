#include "dotr/ee/lower_rules.h"

int TaLogic020_GetTrapCardPlay(int index, unsigned char *minTask, void *unit)
{
    int ids[8];
    int i;

    TaHand_GetNextHand(index, ids);
    for (i = 0; i < TaHand_GetNextHandNum(index); i++) {
        if (SzDuel_Kind2Type(TaHand_GetCardKind(ids[i]) >> 5) == 2) {
            minTask[6] = i;
            SzDuel_InitCardInfo(unit, ids[i]);
            return TaHand_GetCardKind(ids[i]) >> 5;
        }
    }
    return -1;
}
