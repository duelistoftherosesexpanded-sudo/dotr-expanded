#include "dotr/ee/state_records.h"

static inline struct DotrStateRecord *lookup(int i)
{
    struct DotrStateRecord *p = data_00317490_records;
    return p + i;
}

static inline struct DotrStateRecordList *table40(int i)
{
    return (struct DotrStateRecordList *)((char *)lookup(i) + 0x1e0);
}

int SzDuel_AddDeck(int side, int cardNo)
{
    struct DotrStateRecordList *p = table40(side);
    unsigned short *slot;
    int i;
    if (cardNo == 671) {
        cardNo = 999;
    }
    if (cardNo < 0 || cardNo >= 854) {
        return -1;
    }
    if (p->unknown_count >= 40) {
        return -1;
    }
    do {
        i = func_001a22c0(40.0f * (func_0019a5c8() / 2147483648.0f));
        slot = &p->unknown_values[i];
    } while (*slot != 999);
    *slot = cardNo;
    p->unknown_count++;
    return i;
}
