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

int SzDuel_DelDeck(int side, int position)
{
    struct DotrStateRecordList *p = table(side);
    if (position < 0 || position >= 40) {
        return 0;
    }
    if (p->unknown_values[position] == 999) {
        return 0;
    }
    p->unknown_values[position] = 999;
    p->unknown_count--;
    return 1;
}
