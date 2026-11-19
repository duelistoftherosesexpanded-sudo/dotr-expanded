#include "dotr/ee/small_misc.h"

int Ta_Do_HandDestroyEfc(struct DotrMiscTag *p, int a, int b)
{
    p->unknown_00 = 150;
    p->unknown_04 = 0;
    p->unknown_08 = (a << 8) | b;
    p->unknown_0c = 0;
    return 1;
}
