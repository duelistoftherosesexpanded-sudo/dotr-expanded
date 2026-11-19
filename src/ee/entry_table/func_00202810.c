#include "dotr/ee/entry_table.h"

static inline struct DotrEntryTableEntry *dotr_entry_table_lookup(int group, int id)
{
    struct DotrEntryTableEntry *entry = SzText_GetItem(group, id);
    if (entry && !(entry->pointerToTextureSource & 0x80000000)) {
        return entry;
    }
    return 0;
}

static inline struct DotrEntryTableEntry *dotr_entry_table_free(void)
{
    struct DotrEntryTableEntry *entry = gSzText_aItem;
    int i;
    for (i = 0; i < 1024; i++) {
        if (entry->pointerToTextureSource == 0) {
            return entry;
        }
        entry++;
    }
    return 0;
}

int SzText_AddFile(int group, int id, const char *name, int srcImageAddress)
{
    struct DotrEntryTableEntry *entry;

    if (group < 0 || group >= 10) {
        return 0;
    }
    if (id < 0) {
        return 0;
    }
    entry = dotr_entry_table_lookup(group, id);
    if (entry == 0) {
        entry = dotr_entry_table_free();
        if (entry == 0) {
            return 0;
        }
    } else {
        SzText_Free(group, id);
    }
    entry->fileNameItCameFrom = name ? SzMem_StrDup(name) : 0;
    entry->pointerToTextureSource = srcImageAddress;
    entry->flag_0 = 0;
    entry->flag_1 = 0;
    entry->flag_2 = 0;
    entry->group = group;
    entry->id = id;
    return 1;
}
