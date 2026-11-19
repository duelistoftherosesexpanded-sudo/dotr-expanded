#include "dotr/ee/slot_table.h"

void SzMsg_Free(int index)
{
    struct DotrSlotTable *table = &gSzMsg_ml;
    if (index >= 0 && index < 30) {
        table->entries[index].flag = 0;
    }
}
