#include "dotr/ee/low_leaves.h"
/* MATCHING DEVICE: the original has its own body expanded once at each of its two
 * recursive calls, with real calls only inside those copies. A plain recursive
 * function compiles to one body with the second call turned into a loop, so the
 * body is repeated here as a file-local inline that calls the function back. */
static inline void sort_once(unsigned short *values, int lo, int hi, int key, int *table)
{
    int i;
    int last;
    if (lo >= hi) {
        return;
    }
    TaLogic017_SortSwap((int *)values, lo, (lo + hi) >> 1);
    last = lo;
    for (i = lo + 1; i <= hi; i++) {
        if (TaHand_GetCardSummonCost2(table[values[i]], key) > TaHand_GetCardSummonCost2(table[values[lo]], key)) {
            TaLogic017_SortSwap((int *)values, ++last, i);
        }
    }
    TaLogic017_SortSwap((int *)values, lo, last);
    TaLogic017_SortTempArray(values, lo, last, key, table);
    TaLogic017_SortTempArray(values, last + 1, hi, key, table);
}

void TaLogic017_SortTempArray(unsigned short *values, int lo, int hi, int key, int *table)
{
    int i;
    int last;
    if (lo >= hi) {
        return;
    }
    TaLogic017_SortSwap((int *)values, lo, (lo + hi) >> 1);
    last = lo;
    for (i = lo + 1; i <= hi; i++) {
        if (TaHand_GetCardSummonCost2(table[values[i]], key) > TaHand_GetCardSummonCost2(table[values[lo]], key)) {
            TaLogic017_SortSwap((int *)values, ++last, i);
        }
    }
    TaLogic017_SortSwap((int *)values, lo, last);
    sort_once(values, lo, last, key, table);
    sort_once(values, last + 1, hi, key, table);
}
