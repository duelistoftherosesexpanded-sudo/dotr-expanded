#include "dotr/ee/small_misc.h"

static inline struct DotrMiscByteFlagEntry *find(int group, int id)
{
    struct DotrMiscByteFlagEntry *entry = func_002008a0_bytes(group, id);
    if (entry != 0 && !(entry->unknown_00 & 0x80000000)) {
        return entry;
    }
    return 0;
}

/* Sets bit 0 (a signed one-bit field, hence -1) and clears bit 1 of the found
 * entry's byte at +8, returning its first word. The lookup is an inline
 * helper; a direct spelling differs. */
int SzText_LoadRequest(int group, int id)
{
    struct DotrMiscByteFlagEntry *textItem;
    if (group < 0 || group >= 10) {
        return 0;
    }
    if (id < 0) {
        return 0;
    }
    textItem = find(group, id);
    if (textItem == 0) {
        return 0;
    }
    textItem->unknown_08.unknown_bit_0 = -1;
    textItem->unknown_08.unknown_bit_1 = 0;
    return textItem->unknown_00;
}
