#include "dotr/ee/small_misc.h"

/* Returns 1 when viBufCount on the span at +0x48 is zero and
 * func_0018ee68(p) is non-zero; otherwise 0. */
int videoDecIsFlushed(struct DotrMiscPlayer *p)
{
    int r = 0;
    if (viBufCount(&p->unknown_48) == 0) {
        if (func_0018ee68(p) != 0) {
            r = 1;
        }
    }
    return r;
}
