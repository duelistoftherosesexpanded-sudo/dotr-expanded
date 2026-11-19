#include "dotr/ee/small_misc.h"

int readBufBeginPut(struct DotrMiscLarge *p, char **out)
{
    int free = p->unknown_50008 - p->unknown_50004;
    if (free != 0) {
        *out = (char *)p + p->unknown_50000;
    }
    return free;
}
