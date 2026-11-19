#include "dotr/ee/state_records.h"

static inline struct DotrStateRecord *lookup(int i)
{
    struct DotrStateRecord *p = data_00317490_records;
    return p + i;
}

int SzDuel_GetPlayer(int side)
{
    return lookup(side)->unknown_33c;
}
