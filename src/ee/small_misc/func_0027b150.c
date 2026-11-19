#include "dotr/ee/small_misc.h"

/* TaLogic_TreeAloc (0x2788d0) for 56-byte records: the same bound, with the used size
 * advanced by 56. Both routines address records from the literal 0x1ed0000. */
struct DotrMiscRecord56 *TaLogic_RootTreeAloc(unsigned short *allocedBytes, unsigned short *allocedBlocks)
{
    int index;
    if (*allocedBytes + 56u > 0x10000L) {
        return 0;
    }
    index = *allocedBlocks;
    *allocedBlocks = index + 1;
    *allocedBytes += 56;
    return &DOTR_MISC_RECORDS56_01ED0000[index];
}
