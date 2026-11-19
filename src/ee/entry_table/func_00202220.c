/* Evidence: docs/modules/engine.md. Names and prototypes are assumed. */
#include "dotr/ee/entry_table.h"

/* The lookup and the free-entry scan are inlined in the target, as in SzText_AddFile
 * (0x202810); these are copies of that source's helpers. */
static inline struct DotrEntryTableEntry *dotr_entry_table_lookup_ex(int group, int id)
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

static inline struct DotrEntryTableEntry *dotr_entry_table_free_ex(void)
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

int SzText_LoadFileEx(int group, int id, const char *name, int address, int byte_size, int byte_offset, int unknown)
{
    struct DotrEntryTableEntry *entry;
    char path[128];

    if (group < 0 || group >= 10) {
        return 0;
    }
    if (id < 0) {
        return 0;
    }
    if (dotr_entry_table_lookup_ex(group, id)) {
        return 0;
    }
    entry = dotr_entry_table_free_ex();
    if (entry == 0) {
        return 0;
    }
    if ((data_002bd0c1[(int)name[0]] & 3) && name[1] == ':') {
        func_0019bdd8(path, name);
    } else {
        func_0019b648(path, data_002d0760, SzFile_GetEnv(data_002d074b), name);
    }
    if (SzFile_ReadRequestParam(group | 0x1000, 0, path, address, byte_size, byte_offset, unknown, lSzText_LoadFileCB, entry) == 0) {
        return 0;
    }
    entry->fileNameItCameFrom = SzMem_StrDup(name);
    entry->pointerToTextureSource = 0x80000000;
    entry->flag_0 = 0;
    entry->flag_1 = 0;
    entry->flag_2 = (char)(address ? 0 : 1);
    entry->group = group;
    entry->id = id;
    return 1;
}
