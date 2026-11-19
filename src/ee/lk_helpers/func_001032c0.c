#include "dotr/ee/lk_helpers.h"

int LkIncrBYTE(unsigned char *value, unsigned char limit, unsigned char step)
{
    unsigned char current;

    current = *value;
    if (current <= limit - step) {
        *value = current + step;
        return 1;
    }
    *value = limit;
    return 0;
}
