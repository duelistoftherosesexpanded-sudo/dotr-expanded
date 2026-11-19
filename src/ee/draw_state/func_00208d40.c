#include "dotr/ee/draw_state.h"

/* Written in word units so host and target agree on the layout.
 * The two-way tests on bit 4 store the same value on both sides in the
 * original as well. MATCHING DEVICE: the order of the blocks. */
struct DotrDrawTag *RhVu1Mac_ChangeDrawMethod(struct DotrDrawTag *tag, int flags)
{
    int *w = (int *)tag;
    w[0] = 0x10000001;
    w[1] = 0;
    w[2] = 0x10000000;
    w[3] = 0x68014000;
    if (!(flags & 2) || gRhVu1Mac_State.unknown_5c == 0) {
        if (flags & 0x40) {
            if (flags & 0x20) {
                w[4] = 628;
            } else if (flags & 0x10) {
                w[4] = 232;
            } else {
                w[4] = 232;
            }
        } else {
            if (flags & 0x20) {
                w[4] = 420;
            } else if (flags & 0x10) {
                w[4] = 48;
            } else {
                w[4] = 48;
            }
        }
    } else {
        if (flags & 0x20) {
            w[4] = 1748;
        } else {
            w[4] = 1408;
        }
    }
    w[5] = 0;
    w[6] = 0;
    w[7] = 0x10000000;
    gRhVu1Mac_State.unknown_00 = flags;
    return (struct DotrDrawTag *)(w + 8);
}
