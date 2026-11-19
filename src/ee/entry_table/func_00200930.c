#include "dotr/ee/entry_table.h"

/* Same logic as SzText_GetItem, which 0x200930 contains inline; the
 * original source relationship is unknown.
 */
static inline struct DotrEntryTableEntry *dotr_entry_table_find(int group, int id)
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

struct DotrEntryTableEntry *SzText_GetReadyItem(int group, int id)
{
    struct DotrEntryTableEntry *entry = dotr_entry_table_find(group, id);
    if (entry && !(entry->pointerToTextureSource & 0x80000000)) {
        return entry;
    }
    return 0;
}
