/* Synthetic host tests for the packet_builds group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/packet_builds.h"

int data_0032a5e0[8];
DotrPacketWord128 data_0032a600;

/* Recording stubs: each logs its id, whether the packet argument is the object, and its
 * integer arguments. */
int data_00325f00[8];
DotrPacketWord128 data_00325ef0;
struct DotrPacketBase data_00326180_base;
static long pk_log[128];
static int pk_count;
static void *pk_object = data_0032a5e0;
static unsigned long pk_value[2];
static void pk(void *packet, int id, long a, long b)
{
    if (pk_object == 0) {
        pk_object = packet;
    }
    pk_log[pk_count++] = id;
    pk_log[pk_count++] = packet == pk_object;
    pk_log[pk_count++] = a;
    pk_log[pk_count++] = b;
}
void func_001875f8(void *packet, int cursor) { pk(packet, 0x5f8, cursor, 0); }
void func_00187660(void *packet, int unknown_1) { pk(packet, 0x660, unknown_1, 0); }
void func_001877f8(void *packet, int unknown_1) { pk(packet, 0x7f8, unknown_1, 0); }
void func_00187778(void *packet, int unknown_1, int unknown_2) { pk(packet, 0x778, unknown_1, unknown_2); }
void func_00187858(void *packet, int unknown_1) { pk(packet, 0x858, unknown_1, 0); }
void func_001876b8(void *packet, DotrPacketWord128 value)
{
    pk(packet, 0x6b8, 0, 0);
    memcpy(pk_value, &value, sizeof(pk_value));
}
void func_00187810(void *packet, int unknown_1) { pk(packet, 0x810, unknown_1, 0); }
void func_00187828(void *packet, int unknown_1, long unknown_2) { pk(packet, 0x828, unknown_1, unknown_2); }
void func_001876d0(void *packet) { pk(packet, 0x6d0, 0, 0); }
void func_001878b0(void *packet) { pk(packet, 0x8b0, 0, 0); }
int func_00187608(void *packet) { pk(packet, 0x608, 0, 0); return 0x1234; }

/* RhText_SetTextureFlush (straight-line coverage: every forwarded argument, both stored halves, the
 * by-value 128-bit argument and the return value). */
static void check_fixed_packet(void)
{
    static const long expect[10][3] = {
        {0x5f8, 77, 0}, {0x660, 0, 0}, {0x7f8, 0x11000000, 0}, {0x778, 2, 3}, {0x858, 0, 0},
        {0x6b8, 0, 0}, {0x828, 63, 1}, {0x6d0, 0, 0}, {0x8b0, 0, 0}, {0x608, 0, 0}
    };
    unsigned long halves[2];
    int i;

    pk_count = 0;
    assert(RhText_SetTextureFlush(77) == 0x1234 && pk_count == 40);
    for (i = 0; i < 10; i++) {
        assert(pk_log[4 * i] == expect[i][0] && pk_log[4 * i + 1] == 1);
        assert(pk_log[4 * i + 2] == expect[i][1] && pk_log[4 * i + 3] == expect[i][2]);
    }
    memcpy(halves, &data_0032a600, sizeof(halves));
    assert(halves[0] == 0x1000000000008000 && halves[1] == 14);
    assert(pk_value[0] == halves[0] && pk_value[1] == 14);
}

/* lRhBase_InitGsAndVif (straight-line coverage: the stored 64-bit word, every forwarded argument,
 * both stored halves and the by-value 128-bit argument; no result). */
static void check_base_packet(void)
{
    static const long expect[20][3] = {
        {0x5f8, 91, 0}, {0x660, 0, 0}, {0x7f8, 0x1000404, 0}, {0x810, 0x20000000, 0},
        {0x810, 0x5000000, 0}, {0x778, 2, 3}, {0x858, 0, 0}, {0x6b8, 0, 0}, {0x828, 70, 1},
        {0x828, 64, (long)639 << 16 | (long)447 << 48}, {0x828, 26, 1},
        {0x828, 59, 0x8000000000}, {0x828, 61, 0xc0c0c0}, {0x828, 69, 1},
        {0x828, 68, 0x7263051463721405}, {0x6d0, 0, 0}, {0x8b0, 0, 0}, {0x608, 0, 0}
    };
    unsigned long halves[2];
    int i;

    pk_count = 0;
    pk_object = data_00325f00;
    data_00326180_base.width = 640;
    data_00326180_base.height = 448;
    lRhBase_InitGsAndVif(91);
    assert(pk_count == 72 && data_00326180_base.test == 0x7263051463721405);
    for (i = 0; i < 18; i++) {
        assert(pk_log[4 * i] == expect[i][0] && pk_log[4 * i + 1] == 1);
        assert(pk_log[4 * i + 2] == expect[i][1] && pk_log[4 * i + 3] == expect[i][2]);
    }
    memcpy(halves, &data_00325ef0, sizeof(halves));
    assert(halves[0] == 0x1000000000008000 && halves[1] == 14);
    assert(pk_value[0] == halves[0] && pk_value[1] == 14);
}

static int pk_random;
int func_0019a5c8(void) { return pk_random; }

/* lRhBase_SetRandomDither (straight-line coverage, run for two random values: the rotated word, every
 * forwarded argument, one aligned packet address for all helpers, the by-value 128-bit
 * argument and the return value). Remainders of zero or below depend on the target's
 * shift-count masking and are not run on the host. */
static void check_dither_packet(void)
{
    static const int random[2] = {0x35, 0x7fffffff};
    static const unsigned long rotated[2] = {0x2140572630514637, 0x2630514637214057};
    long expect[10][3] = {
        {0x5f8, 55, 0}, {0x660, 0, 0}, {0x778, 2, 3}, {0x858, 0, 0}, {0x6b8, 0, 0},
        {0x828, 69, 1}, {0x828, 68, 0}, {0x6d0, 0, 0}, {0x8b0, 0, 0}, {0x608, 0, 0}
    };
    int i;
    int n;

    for (n = 0; n < 2; n++) {
        pk_count = 0;
        pk_object = 0;
        pk_random = random[n];
        data_00326180_base.test = 0x7263051463721405;
        expect[6][2] = (long)rotated[n];
        assert(lRhBase_SetRandomDither(55) == 0x1234 && pk_count == 40);
        assert(data_00326180_base.test == rotated[n]);
        assert(pk_object != 0 && ((size_t)pk_object & 15) == 0);
        for (i = 0; i < 10; i++) {
            assert(pk_log[4 * i] == expect[i][0] && pk_log[4 * i + 1] == 1);
            assert(pk_log[4 * i + 2] == expect[i][1] && pk_log[4 * i + 3] == expect[i][2]);
        }
        assert(pk_value[0] == 0x1000000000008000 && pk_value[1] == 14);
    }
}

/* The target reads the flag byte both as 0x325f40 and as +0x20 of 0x325f20: two host
 * objects here, set separately. */
struct DotrPacketFrameSetup data_00325f20_packet;
struct DotrPacketFlagPair data_00325f40_packet;
int data_00325e40[8];
DotrPacketWord128 data_00325e60;
union DotrPacketZbuf data_00325e70;
union DotrPacketZbuf data_00325fc0_packet;

/* RhBase_SetZBufferMode (branch coverage: both flags set, first flag clear, second flag clear; `a`
 * zero and nonzero; every forwarded argument, the stored halves and the return value). */
static void check_zbuffer_packet(void)
{
    static const int flags[4][3] = {{1, 1, 0}, {0, 1, 5}, {1, 0, 0}, {1, 1, 7}};
    long expect[10][3] = {
        {0x5f8, 66, 0}, {0x660, 0, 0}, {0x7f8, 0x11000000, 0}, {0x778, 2, 3}, {0x858, 0, 0},
        {0x6b8, 0, 0}, {0x828, 78, 0}, {0x6d0, 0, 0}, {0x8b0, 0, 0}, {0x608, 0, 0}
    };
    unsigned long halves[2];
    int i;
    int n;

    data_00325f20_packet.unknown_0c = 0x150;
    data_00325f20_packet.unknown_1c = 0x2b3;
    data_00325f20_packet.unknown_18 = 0x3a;
    for (n = 0; n < 4; n++) {
        long want;

        pk_count = 0;
        pk_object = data_00325e40;
        data_00325f40_packet.unknown_bit_0 = flags[n][0] ? -1 : 0;
        data_00325f20_packet.unknown_20.unknown_bit_1 = flags[n][1] ? -1 : 0;
        data_00325e70.word = 0x5a000000ff00fe00;
        data_00325fc0_packet.word = 0x1234567831000123;
        if (flags[n][0] && flags[n][1]) {
            want = 0x5a000000ff00fe00 & ~0x10f0001ffL;
            want |= (0x150 + 0x2b3) & 0x1ff;
            want |= 0xaL << 24;
        } else {
            want = 0x1234567831000123 & ~0x100000000L;
        }
        if (flags[n][2] == 0) {
            want |= 0x100000000L;
        }
        expect[6][2] = want;
        assert(RhBase_SetZBufferMode(66, flags[n][2]) == 0x1234 && pk_count == 40);
        assert(data_00325e70.word == want);
        for (i = 0; i < 10; i++) {
            assert(pk_log[4 * i] == expect[i][0] && pk_log[4 * i + 1] == 1);
            assert(pk_log[4 * i + 2] == expect[i][1] && pk_log[4 * i + 3] == expect[i][2]);
        }
        memcpy(halves, &data_00325e60, sizeof(halves));
        assert(halves[0] == 0x1000000000008000 && halves[1] == 14);
        assert(pk_value[0] == halves[0] && pk_value[1] == 14);
    }
}

int main(void)
{
    check_fixed_packet();
    check_base_packet();
    check_dither_packet();
    check_zbuffer_packet();
    return 0;
}
