#include "dotr/ee/state_records.h"

void SzDuel_PackDeck(int side)
{
    struct DotrStateRecordList *p;
    struct DotrStateRecordList temporary;
    int i;

    p = &data_00317490_records[side].unknown_1e0;
    temporary.unknown_count = 0;
    for (i = 0; i < 40; i++) {
        if (p->unknown_values[i] != 999) {
            temporary.unknown_values[temporary.unknown_count++] = p->unknown_values[i];
        }
    }
    for (i = temporary.unknown_count; i < 40; i++) {
        temporary.unknown_values[i] = 999;
    }
    *p = temporary;
}
