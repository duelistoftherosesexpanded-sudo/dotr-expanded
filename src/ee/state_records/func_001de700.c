#include "dotr/ee/state_records.h"

static inline struct DotrStateRecord *lookup(int i)
{
    struct DotrStateRecord *p = data_00317490_records;
    return p + i;
}

void SzDuel_SetHandPhase(int side, int value)
{
    struct DotrStateRecord *p = lookup(side);
    p->unknown_346.unknown_bit_0 = (signed char)(value ? 1 : 0);
}
