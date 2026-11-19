#include "dotr/ee/small_misc.h"

int voBufIsFull(struct DotrMiscObject *p)
{
    return p->unknown_0c == p->unknown_10;
}
