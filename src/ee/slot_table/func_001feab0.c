#include "dotr/ee/slot_table.h"

struct DotrSlotTableEntry *SzMsg_GetInfo(int index)
{
    struct DotrSlotTable *table = &gSzMsg_ml;
    if (index >= 0 && index < 30) {
        return &table->entries[index];
    }
    return 0;
}
