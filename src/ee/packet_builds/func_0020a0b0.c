#include "dotr/ee/packet_builds.h"

/* RhBase_SetZBufferMode: builds one fixed packet at 0x325e40 that sets register 78 to the
 * 64-bit record at 0x325e70. The record takes its nine-bit and four-bit fields from the
 * frame setup at 0x325f20 when both flag bits are set, and is otherwise copied whole from
 * 0x325fc0; bit 32 is set when `a` is zero. Returns what the last helper returns. */
int RhBase_SetZBufferMode(int cursor, int a)
{
    struct DotrPacketFrameSetup *s = &data_00325f20_packet;

    func_001875f8(data_00325e40, cursor);
    func_00187660(data_00325e40, 0);
    func_001877f8(data_00325e40, 0x11000000);
    func_00187778(data_00325e40, 2, 3);
    func_00187858(data_00325e40, 0);
    ((unsigned long *)&data_00325e60)[0] = 0x1000000000008000;
    ((unsigned long *)&data_00325e60)[1] = 14;
    func_001876b8(data_00325e40, data_00325e60);
    if (data_00325f40_packet.unknown_bit_0 && s->unknown_20.unknown_bit_1) {
        data_00325e70.f.low_9 = s->unknown_0c + s->unknown_1c;
        data_00325e70.f.low_4 = s->unknown_18;
    } else {
        data_00325e70 = data_00325fc0_packet;
    }
    data_00325e70.f.mask = (a != 0) ^ 1;
    func_00187828(data_00325e40, 78, *(long *)&data_00325e70);
    func_001876d0(data_00325e40);
    func_001878b0(data_00325e40);
    return func_00187608(data_00325e40);
}
