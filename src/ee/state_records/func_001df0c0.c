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

int SzDuel_InqHand(int side, int position)
{
    struct DotrStateRecordList5 *p = table5(side);
    if (position < 0 || position >= 5) {
        return -1;
    }
    return p->unknown_values[position];
}
