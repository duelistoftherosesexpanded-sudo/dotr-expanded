#include "dotr/ee/small_misc.h"

/* Resets one slot: clears the word and halfwords, sets the 26-bit field to
 * 255 and clears the six flag bits. Returns 0 for null, else 1. */
int SzDuel_ClearCardControl(struct DotrMiscSlot160 *p)
{
    if (p == 0) {
        return 0;
    }
    p->unknown_00 = 0;
    p->unknown_98 = 0;
    p->unknown_9a = 0;
    p->unknown_9c_bits_0_25 = 255;
    p->unknown_9f_bit_2 = 0;
    p->unknown_9f_bit_3 = 0;
    p->unknown_9f_bit_4 = 0;
    p->unknown_9f_bit_5 = 0;
    p->unknown_9f_bit_6 = 0;
    p->unknown_9f_bit_7 = 0;
    return 1;
}
