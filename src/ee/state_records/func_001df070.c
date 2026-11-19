#include "dotr/ee/state_records.h"

void SzDuel_InitHand(int side)
{
    struct DotrStateRecordList5 *p;
    int i;

    p = &data_00317490_records[side].unknown_324;
    for (i = 0; i < 5; i++) {
        p->unknown_values[i] = 999;
    }
    p->unknown_count = 0;
}
