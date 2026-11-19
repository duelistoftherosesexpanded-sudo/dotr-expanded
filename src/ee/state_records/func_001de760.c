#include "dotr/ee/state_records.h"

static inline struct DotrStateRecord *lookup(int i)
{
    struct DotrStateRecord *p = data_00317490_records;
    return p + i;
}

void SzDuel_SetDestinyFlag(int side, int value)
{
    lookup(side)->unknown_344 = value;
}
