#include "dotr/ee/block_table.h"

int SzFade_Entry(int i, int a, int b, int c, int d, struct DotrBlockQuad *in)
{
    struct DotrBlockEntry *e = &gSzFade_fl[i];
    if (!e->unknown_07.unknown_bit_6) {
        return 0;
    }
    e->unknown_07.unknown_bits_0_5 = a;
    e->unknown_04 = b;
    e->unknown_05 = c;
    e->unknown_06 = d;
    e->unknown_07.unknown_bit_6 = 0;
    e->unknown_07.unknown_bit_7 = 0;
    e->unknown_08.unknown_bit_0 = 0;
    if (in != 0) {
        e->unknown_00 = *in;
    } else {
        e->unknown_00.unknown_bytes[2] = 0;
        e->unknown_00.unknown_bytes[1] = 0;
        e->unknown_00.unknown_bytes[0] = 0;
        e->unknown_00.unknown_bytes[3] = 128;
    }
    return 1;
}
