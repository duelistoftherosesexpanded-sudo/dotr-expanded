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

int SzDuel_DelHand(int index, int position)
{
    struct DotrStateRecordList5 *p = table5(index);
    if (position < 0 || position >= 5) {
        return 0;
    }
    if ((p->unknown_values[position] & 0xfff) == 999) {
        return 0;
    }
    p->unknown_values[position] = 999;
    p->unknown_count--;
    return 1;
}
