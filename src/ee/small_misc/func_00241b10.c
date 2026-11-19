#include "dotr/ee/small_misc.h"

int readBufEndPut(struct DotrMiscLarge *p, int count)
{
    int free = p->unknown_50008 - p->unknown_50004;
    count = count < free ? count : free;
    p->unknown_50000 = (p->unknown_50000 + count) % p->unknown_50008;
    p->unknown_50004 += count;
    return count;
}
