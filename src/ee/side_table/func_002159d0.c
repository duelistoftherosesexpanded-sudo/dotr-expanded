#include "dotr/ee/side_table.h"

int SzData_GetLeader(int side, int deckNo)
{
    struct DotrSideList *q = DOTR_SIDE_LIST(DOTR_SIDE(side), deckNo);
    if (side < 0 || side >= 2) {
        return -1;
    }
    return q->leader;
}
