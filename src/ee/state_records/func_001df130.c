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

int SzDuel_AddHand(int side, int cardId)
{
    struct DotrStateRecordList5 *p = table5(side);
    if (cardId == 671) {
        cardId = 999;
    }
    if (cardId < 0 || cardId >= 854) {
        return -1;
    }
    if (p->unknown_count >= 5) {
        return -1;
    }
    SzDuel_PackHand(side);
    p->unknown_values[p->unknown_count] = cardId & 0xfff;
    return p->unknown_count++;
}
