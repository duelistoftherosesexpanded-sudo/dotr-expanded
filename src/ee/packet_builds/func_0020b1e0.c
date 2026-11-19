#include "dotr/ee/packet_builds.h"

/* lRhBase_SetRandomDither: rotates the 64-bit word at 0x326180 right by 4 * (random % 16)
 * bits, then builds a packet in an aligned stack buffer that sets register 69 to 1 and
 * register 68 to the rotated word; returns what the last helper returns. A remainder of
 * zero relies on the target's 6-bit shift counts (shift by 64 leaves the word). */
int lRhBase_SetRandomDither(int cursor)
{
    char raw[63];
    int *packet = (int *)(((DotrPacketAddr)raw + 15) & ~15);
    DotrPacketWord128 *gif = (DotrPacketWord128 *)(packet + 8);
    int shift = func_0019a5c8() % 16;

    shift <<= 2;
    data_00326180_base.test = data_00326180_base.test >> shift | data_00326180_base.test << (64 - shift);
    func_001875f8(packet, cursor);
    func_00187660(packet, 0);
    func_00187778(packet, 2, 3);
    func_00187858(packet, 0);
    ((unsigned long *)gif)[0] = 0x1000000000008000;
    ((unsigned long *)gif)[1] = 14;
    func_001876b8(packet, *gif);
    func_00187828(packet, 69, 1);
    func_00187828(packet, 68, data_00326180_base.test);
    func_001876d0(packet);
    func_001878b0(packet);
    return func_00187608(packet);
}
