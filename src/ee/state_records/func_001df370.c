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

int SzDuel_SetHandTag(int side, int position, int level)
{
    struct DotrStateRecordList5 *p = table5(side);
    int old;
    int i;

    if (position < 0 || position >= 5) {
        return 0;
    }
    if ((p->unknown_values[position] & 0xfff) == 999) {
        return 0;
    }
    old = (p->unknown_values[position] & 0xf000) >> 12;
    if (level == 0) {
        if (old == 0) {
            return 0;
        }
        p->unknown_values[position] &= 0xfff;
        for (i = 0; i < 5; i++) {
            int w = p->unknown_values[i];
            if (i != position) {
                if ((w & 0xfff) != 999) {
                    int l = (w & 0xf000) >> 12;
                    if (l != 0 && old < l) {
                        p->unknown_values[i] = ((l - 1) << 12) | (w & 0xfff);
                    }
                }
            }
        }
        return 1;
    }
    if (old != 0) {
        return 0;
    }
    p->unknown_values[position] = (level << 12) | (p->unknown_values[position] & 0xfff);
    return 1;
}
