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

int SzDuel_ClearHandTag(int side)
{
    struct DotrStateRecordList5 *p = table5(side);
    int i;
    for (i = 0; i < 5; i++) {
        p->unknown_values[i] &= 0xfff;
    }
    return 1;
}
