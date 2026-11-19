#include "dotr/ee/small_state.h"

/* For each of the two entries whose six-bit field at +0x5b is 5: when func_00192bb8(i, 0)
 * answers 2 or 6 and func_00192b38(i, 0, buffer) answers non-zero, copies buffer bytes 4-7
 * to +6 when bit 6 of +0x5b is set and bytes 8-19 to +0xa when bit 7 is set, and ORs the
 * complement of the 16-bit value (byte 2 high, byte 3 low) into the halfword at +4. */
int SzPad_VSync(void)
{
    unsigned char buffer[32];
    int i;
    struct DotrSmallEntry *e;
    int j;
    int state;

    for (i = 0; i < 2; i++) {
        e = &gSzPad_pl.unknown_200[i];
        if (e->unknown_5b.unknown_bits_0_5 == 5) {
            state = func_00192bb8(i, 0);
            if (state == 2 || state == 6) {
                if (func_00192b38(i, 0, buffer) != 0) {
                    if (e->unknown_5b.unknown_bit_6) {
                        for (j = 0; j < 4; j++) {
                            e->unknown_06[j] = buffer[4 + j];
                        }
                    }
                    if (e->unknown_5b.unknown_bit_7) {
                        for (j = 0; j < 12; j++) {
                            e->unknown_0a[j] = buffer[8 + j];
                        }
                    }
                    e->unknown_04 |= ((buffer[2] << 8) | buffer[3]) ^ 0xffff;
                }
            }
        }
    }
    return 1;
}
