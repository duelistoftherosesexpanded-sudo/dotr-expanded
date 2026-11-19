#include "dotr/ee/state_records.h"

/* MATCHING DEVICE: the offset cast; the member spelling
 * data_00317490_records[side].unknown_1e0 compiles to different code here. */
#define LIST(i) ((struct DotrStateRecordList *)((char *)(data_00317490_records + (i)) + 0x1e0))

int SzDuel_DrawDeck(int side)
{
    int value = LIST(side)->unknown_values[0];
    struct DotrStateRecordList *p;

    if (value < 0 || value >= 999) {
        return 999;
    }
    p = LIST(side);
    if (p->unknown_values[0] != 999) {
        p->unknown_values[0] = 999;
        p->unknown_count--;
    }
    SzDuel_PackDeck(side);
    return value;
}
