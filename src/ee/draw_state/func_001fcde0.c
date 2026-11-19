#include "dotr/ee/draw_state.h"
#include "dotr/ee/block_table.h"

int SzFade_Disp(int index)
{
    struct DotrBlockEntry *e;
    unsigned char buf[4];
    int v;
    if (index < 0 || index >= 2) {
        return 0;
    }
    e = &gSzFade_fl[index];
    if (e->unknown_07.unknown_bit_6) {
        return 0;
    }
    v = e->unknown_05;
    if (e->unknown_07.unknown_bits_0_5 == 0) {
        v = (unsigned char)(e->unknown_06 - e->unknown_05);
    }
    buf[2] = e->unknown_00.unknown_bytes[2];
    buf[1] = e->unknown_00.unknown_bytes[1];
    buf[0] = e->unknown_00.unknown_bytes[0];
    buf[3] = (e->unknown_00.unknown_bytes[3] * (v & 0xff)) / e->unknown_06;
    SzFade_Draw(0, (DotrDrawWord)buf, 0xffffff);
    if (e->unknown_05 >= e->unknown_06) {
        if (e->unknown_07.unknown_bit_7) {
            e->unknown_07.unknown_bit_6 = -1;
        }
    } else if (!e->unknown_08.unknown_bit_0) {
        v = e->unknown_05 + e->unknown_04;
        if (e->unknown_06 < v) {
            e->unknown_05 = e->unknown_06;
        } else {
            e->unknown_05 = v;
        }
    }
    return 1;
}
