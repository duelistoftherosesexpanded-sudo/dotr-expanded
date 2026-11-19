#include "dotr/ee/entry_table.h"

struct DotrEntryTableEntry *SzText_GetItem(int group, int id)
{
    struct DotrEntryTableEntry *entry = gSzText_aItem;
    int i;
    if (group < 0 || group >= 10) {
        return 0;
    }
    if (id < 0) {
        return 0;
    }
    for (i = 0; i < 1024; i++) {
        if (entry->pointerToTextureSource) {
            if (entry->group == group && entry->id == id) {
                return entry;
            }
        }
        entry++;
    }
    return 0;
}
