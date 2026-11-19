#include "dotr/ee/state_records.h"

void SzDuel_PackGrave(int side)
{
    struct DotrStateRecordList120 *p;
    struct DotrStateRecordList120 temporary;
    int i;

    p = &data_00317490_records[side].unknown_232;
    temporary.unknown_count = 0;
    for (i = 0; i < 120; i++) {
        if (p->unknown_values[i] != 999) {
            temporary.unknown_values[temporary.unknown_count++] = p->unknown_values[i];
        }
    }
    for (i = temporary.unknown_count; i < 120; i++) {
        temporary.unknown_values[i] = 999;
    }
    *p = temporary;
}
