#include "dotr/ee/small_misc.h"

/* The first argument is not read. The routine is called twice with the same
 * halfword; the upper half of the first result joins the lower half of the
 * second. */
unsigned TaLogic_GetEnchantQue2(int unused, unsigned short *id)
{
    unsigned high;
    (void)unused;
    high = (unsigned)TaHand_GetCardAPDP(*id) >> 16;
    return high << 16 | (TaHand_GetCardAPDP(*id) & 0xffff);
}
