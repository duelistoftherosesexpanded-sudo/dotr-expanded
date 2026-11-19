#include "dotr/ee/entry_table.h"

static inline int dotr_entry_table_release(struct DotrEntryTableEntry *entry)
{
    if (entry->fileNameItCameFrom) {
        SzMem_Free(entry->fileNameItCameFrom);
        entry->fileNameItCameFrom = 0;
    }
    if (entry->pointerToTextureSource) {
        if (entry->flag_2) {
            SzMem_Free(entry->pointerToTextureSource);
        }
        entry->pointerToTextureSource = 0;
    }
    return 1;
}

int SzText_Free(int group, int id)
{
    struct DotrEntryTableEntry *entry = gSzText_aItem;
    int result = 0;
    int i;

    for (i = 0; i < 1024; i++) {
        if (entry->pointerToTextureSource) {
            if ((group < 0 || entry->group == group) && (id < 0 || entry->id == id)
                && !(entry->pointerToTextureSource & 0x80000000)) {
                result = dotr_entry_table_release(entry);
            }
        }
        entry++;
    }
    return result;
}
