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

int SzDuel_SetDeck(int side, int position, int cardNo)
{
    struct DotrStateRecordList *p = table(side);
    if (position < 0 || position >= 40) {
        return 0;
    }
    if (cardNo == 671) {
        cardNo = 999;
    }
    if (cardNo < 0 || cardNo >= 854) {
        return 0;
    }
    if (p->unknown_count >= 40) {
        return 0;
    }
    if (p->unknown_values[position] != 999) {
        return 0;
    }
    p->unknown_values[position] = cardNo;
    p->unknown_count++;
    return 1;
}
