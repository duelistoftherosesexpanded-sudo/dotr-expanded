#include "dotr/ee/state_records.h"

void SzDuel_InitGrave(int side)
{
    struct DotrStateRecordList120 *p;
    int i;

    p = &data_00317490_records[side].unknown_232;
    for (i = 0; i < 120; i++) {
        p->unknown_values[i] = 999;
    }
    p->unknown_count = 0;
}
