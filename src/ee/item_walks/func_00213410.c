#include "dotr/ee/item_walks.h"

/* Walks records from item up to item + size bytes. Each goes to lRhModelCpu_CalcSharedStrip
 * when bit 15 of its fourth word is set and to lRhModelCpu_CalcIndepStrip otherwise; the
 * next record is the scratchpad pointer at +0xfc, one word further when bit 15
 * of the record's second word is set, then past any zero words. */
int lRhModelCpu_TraceStaticBlock(int cursor, unsigned *item, int size)
{
    unsigned *end = (unsigned *)((char *)item + size);
    struct DotrWalkPad *pad = DOTR_WALK_PAD;
    unsigned *now;

    while (item < end) {
        now = item;
        if (!(item[3] & 0x8000)) {
            cursor = lRhModelCpu_CalcIndepStrip(cursor, item);
        } else {
            cursor = lRhModelCpu_CalcSharedStrip(cursor, item);
        }
        item = pad->next;
        if (now[1] & 0x8000) {
            item++;
        }
        while (*item == 0) {
            item++;
        }
    }
    return cursor;
}
