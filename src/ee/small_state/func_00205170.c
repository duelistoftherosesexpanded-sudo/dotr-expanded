#include "dotr/ee/small_state.h"

/* Returns 1 if bit 2 or bit 0 of the flag byte at +0x2c is set, else 0. */
int SzDebug_IsHide(void)
{
    struct DotrSmallFlagsOwner *p = &data_00325c80_owner;
    if (p->unknown_2c.unknown_bit_2) {
        return 1;
    }
    return p->unknown_2c.unknown_bit_0 ? 1 : 0;
}
