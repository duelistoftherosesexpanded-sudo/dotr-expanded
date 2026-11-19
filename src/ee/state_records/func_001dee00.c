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

int SzDuel_InqGrave(int side, int position)
{
    struct DotrStateRecordList120 *p = table120(side);
    if (position < 0 || position >= 120) {
        return -1;
    }
    return p->unknown_values[position];
}
