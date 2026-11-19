#include "dotr/ee/state_records.h"

static inline struct DotrStateRecord *lookup(int i)
{
    struct DotrStateRecord *p = data_00317490_records;
    return p + i;
}

static inline struct DotrStateRecordList5 *table5(int i)
{
    return (struct DotrStateRecordList5 *)((char *)lookup(i) + 0x324);
}

int SzDuel_GetHandCount(int side)
{
    return table5(side)->unknown_count;
}
