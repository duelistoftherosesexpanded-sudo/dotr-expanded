#include "dotr/ee/small_misc.h"

/* With a non-zero first word calls func_00192058 and passes the word at
 * +0x2c to func_00182a28; otherwise passes the word at +0x30 to
 * func_0017fd20. Returns 1. */
int strFileClose(struct DotrMiscClose *p)
{
    if (p->unknown_00 != 0) {
        func_00192058();
        func_00182a28(p->unknown_2c);
    } else {
        func_0017fd20(p->unknown_30);
    }
    return 1;
}
