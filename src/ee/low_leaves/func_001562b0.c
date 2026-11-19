#include "dotr/ee/low_leaves.h"

void TaLogic017_SortSwap(int *values, int a, int b)
{
    int saved;
    int *first;
    int *second;

    first = &values[a];
    second = &values[b];
    saved = *first;
    *first = *second;
    *second = saved;
}
