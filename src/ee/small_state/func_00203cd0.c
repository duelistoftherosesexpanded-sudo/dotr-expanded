#include "dotr/ee/small_state.h"

/* Clears the 48 bytes at 0x325c80, then stores word 0 = 0, word 1 = the byte at 0x2d078c,
 * words 2-9 = 0, word 10 = 2 and clears bits 0-2 of the byte at +0x2c. Formats a 128-byte
 * local with func_0019b648(buffer, 0x2d07f4, SzFile_GetEnv(0x2d07fa), 0x2d07ff) (the result
 * is not used further) and sets bit 1 of +0x2c when func_00197548(0x2d07ff) is non-null. */
void SzDebug_Init(void)
{
    char buffer[128];
    /* MATCHING DEVICE: the last store goes through this pointer local (the original keeps
     * the address in s0 while every earlier store is absolute). */
    struct DotrSmallFlagsOwner *p = &data_00325c80_owner;

    RhMem_FillMemory(&data_00325c80_owner, 0, 48);
    data_00325c80_owner.unknown_00[0] = 0;
    data_00325c80_owner.unknown_00[1] = data_002d078c;
    data_00325c80_owner.unknown_00[2] = 0;
    data_00325c80_owner.unknown_00[3] = 0;
    data_00325c80_owner.unknown_00[4] = 0;
    data_00325c80_owner.unknown_00[5] = 0;
    data_00325c80_owner.unknown_00[6] = 0;
    data_00325c80_owner.unknown_00[7] = 0;
    data_00325c80_owner.unknown_00[8] = 0;
    data_00325c80_owner.unknown_00[9] = 0;
    data_00325c80_owner.unknown_00[10] = 2;
    data_00325c80_owner.unknown_2c.unknown_bit_0 = 0;
    data_00325c80_owner.unknown_2c.unknown_bit_1 = 0;
    data_00325c80_owner.unknown_2c.unknown_bit_2 = 0;
    func_0019b648(buffer, data_002d07f4, SzFile_GetEnv(data_002d07fa), data_002d07ff);
    if (func_00197548(data_002d07ff) != 0) {
        p->unknown_2c.unknown_bit_1 = -1; /* a signed one-bit field: -1 is the set value */
    }
}
