#include "dotr/ee/item_walks.h"

/* One-based record addresses: for the key reads, and (integer form) for the copies. */
#define DOTR_SORT_AT(i) (first + (i) - 1)
#define DOTR_SORT_COPY(k) ((struct DotrWalkPolygon *)(((k) << 7) + (DotrWalkAddr)first - 128))

/* lRhModelCpu_SortPolygon: heap sort of the 128-byte records in [first, end), ascending by
 * the unsigned key at +0x7c. Builds the heap from the middle down, then repeatedly moves
 * the first record behind the shrinking heap and sifts the displaced one down; the record
 * being placed is held in the scratchpad at +0x120. */
void lRhModelCpu_SortPolygon(struct DotrWalkPolygon *first, struct DotrWalkPolygon *end)
{
    struct DotrWalkSortPad *scratch = DOTR_WALK_SORT_PAD;
    int k;
    int j;
    int i;
    int n = end - first;
    struct DotrWalkPolygon *p;

    i = n >> 1;
    if (i > 0) {
        p = DOTR_SORT_COPY(i);
        do {
            k = i;
            RhMem_CopyMemory16(&scratch->temp, p, 8);
            while ((j = k * 2) <= n) {
                if (j < n && DOTR_SORT_AT(j)->key < DOTR_SORT_AT(j + 1)->key) {
                    j++;
                }
                if (!(scratch->temp.key < DOTR_SORT_AT(j)->key)) {
                    break;
                }
                RhMem_CopyMemory16(DOTR_SORT_COPY(k), DOTR_SORT_COPY(j), 8);
                k = j;
            }
            RhMem_CopyMemory16(DOTR_SORT_COPY(k), &scratch->temp, 8);
            i--;
            p--;
        } while (i > 0);
    }
    if (n > 1) {
        p = DOTR_SORT_COPY(n);
        do {
            RhMem_CopyMemory16(&scratch->temp, p, 8);
            RhMem_CopyMemory16(p, first, 8);
            p--;
            n--;
            k = 1;
            while ((j = k * 2) <= n) {
                if (j < n && DOTR_SORT_AT(j)->key < DOTR_SORT_AT(j + 1)->key) {
                    j++;
                }
                if (!(scratch->temp.key < DOTR_SORT_AT(j)->key)) {
                    break;
                }
                RhMem_CopyMemory16(DOTR_SORT_COPY(k), DOTR_SORT_COPY(j), 8);
                k = j;
            }
            RhMem_CopyMemory16(DOTR_SORT_COPY(k), &scratch->temp, 8);
        } while (n > 1);
    }
}
