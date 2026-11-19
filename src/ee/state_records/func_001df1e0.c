#include "dotr/ee/state_records.h"

void SzDuel_PackHand(int side)
{
    struct DotrStateRecordList5 *p;
    struct DotrStateRecordList5 temporary;
    int i;

    p = &data_00317490_records[side].unknown_324;
    temporary.unknown_count = 0;
    for (i = 0; i < 5; i++) {
        if ((p->unknown_values[i] & 0xfff) != 999) {
            temporary.unknown_values[temporary.unknown_count++] = p->unknown_values[i];
        }
    }
    for (i = temporary.unknown_count; i < 5; i++) {
        temporary.unknown_values[i] = 999;
    }
    *p = temporary;
}
