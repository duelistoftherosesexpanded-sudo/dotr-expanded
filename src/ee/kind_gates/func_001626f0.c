#include "dotr/ee/kind_gates.h"

int TaLogic019_IsUseGFK(int side)
{
    int i;
    unsigned char count;
    int word;
    int pos;

    count = 0;
    if (TaLogic_IsAttackedNow(side, 0, 0) == 1) {
        return 1;
    }
    word = SearchUnitByRange(0, 0, side, 2, 52, 14); /* 52: search type non-leader */
    for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(word); i++) {
        pos = Ta_GetUnitOwnerIDFromLstCount(word, i);
        if (SzDuel_GetUnitStatus(pos >> 8, pos & 0xff, 4) == 0) {
            count++;
        }
    }
    return count < 5 ? 0 : 1;
}
