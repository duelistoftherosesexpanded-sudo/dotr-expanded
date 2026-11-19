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

int SzDuel_AddGrave(int side, int cardNo)
{
    struct DotrStateRecordList120 *p = table120(side);
    if (cardNo == 671) {
        cardNo = 999;
    }
    if (cardNo < 0 || cardNo >= 854) {
        return -1;
    }
    if (p->unknown_count >= 120) {
        return -1;
    }
    SzDuel_PackGrave(side);
    p->unknown_values[p->unknown_count] = cardNo;
    return p->unknown_count++;
}
