#include "dotr/ee/state_records.h"

static inline struct DotrStateRecord *lookup(int i)
{
    struct DotrStateRecord *p = data_00317490_records;
    return p + i;
}

static inline struct DotrStateRecordList *table(int i)
{
    return (struct DotrStateRecordList *)((char *)lookup(i) + 0x1e0);
}

int SzDuel_GetDeckCount(int side)
{
    return table(side)->unknown_count;
}
