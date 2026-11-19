#include "dotr/ee/small_misc.h"

/* ORs SzEffect_Destroy(i) over the 24 entries at 0x31bc50 whose 3-bit kind
 * equals a (any when a < 0) and whose 10-bit id equals b (any when b < 0). */
int SzEffect_Destroy2(int a, int b)
{
    struct DotrMiscEntry24 *e = data_0031bc50;
    int r = 0;
    int i;
    for (i = 0; i < 24; i++, e++) {
        if (a >= 0 && e->kind != a) {
            continue;
        }
        if (b >= 0 && e->id != b) {
            continue;
        }
        r |= SzEffect_Destroy(i);
    }
    return r;
}
