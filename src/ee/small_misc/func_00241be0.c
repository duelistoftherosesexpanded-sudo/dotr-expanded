#include "dotr/ee/small_misc.h"

int readBufEndGet(struct DotrMiscLarge *p, int count)
{
    count = count < p->unknown_50004 ? count : p->unknown_50004;
    p->unknown_50004 -= count;
    return count;
}
