#include "dotr/ee/state_records.h"

static inline struct DotrStateRecord *lookup(int i)
{
    struct DotrStateRecord *p = data_00317490_records;
    return p + i;
}

static inline struct DotrStateRecordList120 *table120(int i)
{
    return (struct DotrStateRecordList120 *)((char *)lookup(i) + 0x232);
}

int SzDuel_GetGraveCount(int side)
{
    return table120(side)->unknown_count;
}
