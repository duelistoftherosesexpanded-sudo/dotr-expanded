#include "dotr/ee/small_misc.h"

/* The index local orders the count update before the size update. */
struct DotrMiscRecord24 *TaLogic_TreeAloc(unsigned short *allocedBytes, unsigned short *allocedBlocks)
{
    int index;
    if (*allocedBytes + 56u > 0x10000L) {
        return 0;
    }
    index = *allocedBlocks;
    *allocedBlocks = index + 1;
    *allocedBytes += 24;
    return &DOTR_MISC_RECORDS_01ED0000[index];
}
