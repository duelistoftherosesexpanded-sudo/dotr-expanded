#include "dotr/ee/lk_helpers.h"

int LkDecrINT(int *value, int limit, int step)
{
    int current;

    current = *value;
    if (current >= limit + step) {
        *value = current - step;
        return 1;
    }
    *value = limit;
    return 0;
}
