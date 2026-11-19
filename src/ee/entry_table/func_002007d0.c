#include "dotr/ee/entry_table.h"

void SzText_Init(void)
{
    struct DotrEntryTableEntry *entry = gSzText_aItem;
    int i;
    RhMem_FillMemory(gSzText_tl, 0, 6);
    for (i = 0; i < 1024; i++) {
        entry->pointerToTextureSource = 0;
        entry->fileNameItCameFrom = 0;
        entry->flag_0 = 0;
        entry->flag_1 = 0;
        entry->flag_2 = 0;
        entry->group = entry->id = -1;
        entry++;
    }
}
