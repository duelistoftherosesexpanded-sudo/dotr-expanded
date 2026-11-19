#include "dotr/ee/state_records.h"

void SzDuel_InitDeck(int side)
{
    struct DotrStateRecordList *p;
    int i;

    p = &data_00317490_records[side].unknown_1e0;
    for (i = 0; i < 40; i++) {
        p->unknown_values[i] = 999;
    }
    p->unknown_count = 0;
}
