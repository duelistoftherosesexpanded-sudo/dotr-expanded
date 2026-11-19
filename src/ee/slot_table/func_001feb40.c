#include "dotr/ee/slot_table.h"

void SzMsg_FreeAll(void)
{
    struct DotrSlotTableEntry *entry = gSzMsg_ml.entries;
    int i;
    for (i = 0; i < 30; i++) {
        entry->flag = 0;
        entry++;
    }
}
