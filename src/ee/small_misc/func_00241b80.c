#include "dotr/ee/small_misc.h"

int readBufBeginGet(struct DotrMiscLarge *p, char **out)
{
    int used = p->unknown_50004;
    if (used != 0) {
        *out = (char *)p + (p->unknown_50000 - used + p->unknown_50008) % p->unknown_50008;
    }
    return p->unknown_50004;
}
