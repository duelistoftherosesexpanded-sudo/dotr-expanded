#include "dotr/ee/small_misc.h"

void SortByOrder(int *ownerIds, int low, int high)
{
    unsigned int word;
    int key;
    int pivot_key;
    int i;
    int last;
    unsigned int *p;

    if (low < high) {
        Swap(ownerIds, low, (low + high) >> 1);
        i = low + 1;
        last = low;
        if (i <= high) {
            p = (unsigned int *)&ownerIds[i];
            do {
                key = SzDuel_GetUnitSummonOrder((int)*p >> 8, *p & 0xff);
                word = (unsigned int)ownerIds[low];
                pivot_key = SzDuel_GetUnitSummonOrder((int)word >> 8, word & 0xff);
                if (pivot_key < key) {
                    last = last + 1;
                    Swap(ownerIds, last, i);
                }
                i = i + 1;
                p = p + 1;
            } while (i <= high);
        }
        Swap(ownerIds, low, last);
        SortByOrder(ownerIds, low, last);
        SortByOrder(ownerIds, last + 1, high);
    }
}
