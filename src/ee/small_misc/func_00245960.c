#include "dotr/ee/small_misc.h"

void voBufDecCount(struct DotrMiscCounter *p)
{
    if (p->unknown_0c > 0) {
        p->unknown_0c--;
    }
}
