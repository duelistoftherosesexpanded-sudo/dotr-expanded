/* Synthetic host tests for the taku_quads group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
/* (Taku_Prim_Set_GT4 only; Taku_Prim_Set_FT4 has a full test.) */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/taku_quads.h"

void *tex_data[8];

/* What ExSortPrim_Pipe is handed for Taku_Prim_Set_FT4: the header and the accessed part of
 * the packet. */
struct seen_packet {
    DotrQuadFt4Header header;
    unsigned char colour[4];
    float q;
    unsigned char vertices[0x40];
};
static int order[8], calls;
static void *xywh_packet, *texture_packet, *alpha_packet;
static int xywh_args[16];
static void *seen_texture;
static int seen_texture_a;
static long long seen_alpha;
static struct seen_packet seen;
static int seen_a1, seen_a2;
static DotrQuadGt4Packet seen_gt4;
/* Size of the packet local the function under test builds: ExSortPrim_Pipe copies only that
 * much, so the FT4 packet (smaller than a GT4 one) is never read past its end. */
static size_t packet_size;

unsigned char *ExGetMyOT(void)
{
    order[calls++] = 1;
    return NULL;
}

void TAKU_FT4_XYWH(void *packet, short x0, short y0, int z0, unsigned char f0, short x1, short y1, int z1,
                   unsigned char f1, short x2, short y2, int z2, unsigned char f2, short x3, short y3,
                   int z3, unsigned char f3)
{
    int a[16];

    a[0] = x0, a[1] = y0, a[2] = z0, a[3] = f0, a[4] = x1, a[5] = y1, a[6] = z1, a[7] = f1;
    a[8] = x2, a[9] = y2, a[10] = z2, a[11] = f2, a[12] = x3, a[13] = y3, a[14] = z3, a[15] = f3;
    memcpy(xywh_args, a, sizeof a);
    xywh_packet = packet;
    order[calls++] = 2;
}

void TAKU_GT4_XYWH(void *packet, short x0, short y0, int z0, unsigned char fog0, short x1, short y1,
                   int z1, unsigned char fog1, short x2, short y2, int z2, unsigned char fog2,
                   short x3, short y3, int z3, unsigned char fog3)
{
    TAKU_FT4_XYWH(packet, x0, y0, z0, fog0, x1, y1, z1, fog1, x2, y2, z2, fog2, x3, y3, z3, fog3);
}

int ExAssignTexture(void *packet, void *texture, int a)
{
    texture_packet = packet;
    seen_texture = texture;
    seen_texture_a = a;
    order[calls++] = 3;
    return 0;
}

int ExAssignAlpha(void *packet, long long value)
{
    alpha_packet = packet;
    seen_alpha = value;
    order[calls++] = 4;
    return 0;
}

long long ExGetAlphaVal(int index)
{
    return 0x100 + index;
}

DotrQuadWord ExSortPrim_Pipe(DotrQuadWord cursor, DotrQuadWord packet, int a2, int a3)
{
    /* All callees get the same packet address. */
    assert((void *)packet == xywh_packet && (void *)packet == texture_packet && (void *)packet == alpha_packet);
    assert(packet_size != 0);
    memset(&seen, 0, sizeof seen);
    memset(&seen_gt4, 0, sizeof seen_gt4);
    memcpy(&seen, (void *)packet, packet_size < sizeof seen ? packet_size : sizeof seen);
    memcpy(&seen_gt4, (void *)packet, packet_size < sizeof seen_gt4 ? packet_size : sizeof seen_gt4);
    seen_a1 = a2;
    seen_a2 = a3;
    order[calls++] = 5;
    return cursor + 0x100;
}

/* Taku_Prim_Set_FT4, straight-line builder: one call checking the call order, every
 * forwarded argument, every stored field (texel fields through their 14-bit masks, the
 * two bits above each are whatever the stack held) and the return value. */
static void check_ft4(void)
{
    static const int corners[16] = { 10, 20, 0xfffff8 + 7, 0, 30, 40, 0xfffff8 + 7, 0,
                                     50, 60, 0xfffff8 + 7, 0, 70, -80, 0xfffff8 + 7, 0 };
    static const unsigned short texels[8] = { 8, 4088, 264, 520, 1032, 2056, 0x3ff8, 1608 };
    unsigned short half[4];
    int texture;
    int k;

    calls = 0;
    tex_data[5] = &texture;
    packet_size = sizeof(DotrQuadFt4Packet);
    assert(Taku_Prim_Set_FT4((void *)0x1000, 10, 20, 30, 40, 50, 60, 70, -80, 0, 255, 16, 32, 64, 128,
                             -1, 100, 11, 22, 33, 44, 99, 5, 88, 7) == (void *)0x1100);
    assert(calls == 5);
    for (k = 0; k < 5; k++) {
        assert(order[k] == k + 1);
    }
    assert(memcmp(xywh_args, corners, sizeof corners) == 0);
    assert(seen_texture == &texture && seen_texture_a == 0);
    assert(seen_alpha == 0x8000000088 && seen_a1 == 0 && seen_a2 == 0);
    assert(seen.header.count == 9 && seen.header.flags == (36 | 0x800));
    assert(seen.header.pad == 0 && seen.header.registers == 0x424242421);
    assert(seen.colour[0] == 11 && seen.colour[1] == 22 && seen.colour[2] == 33);
    assert(seen.colour[3] == 44 && seen.q == 1.0f);
    for (k = 0; k < 4; k++) {
        memcpy(half, seen.vertices + 16 * k, sizeof half);
        assert((half[0] & 0x3fff) == texels[2 * k] && (half[1] & 0x3fff) == texels[2 * k + 1]);
    }
}

/* Taku_Prim_Set_GT4: one ordinary call (texel spans under 0x101, so the 256 divisor); asserts
 * the first vertex's s coordinate. */
static void smoke_gt4(void)
{
    int texture;

    calls = 0;
    tex_data[2] = &texture;
    packet_size = sizeof(DotrQuadGt4Packet);
    Taku_Prim_Set_GT4((void *)0x2000, 0, 0, 16, 0, 0, 16, 16, 16, 8, 0, 24, 0, 8, 16, 24, 16,
                      1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 0, 2, 0, 5);
    assert(seen_gt4.vertex[0].s == 8.5f / 256.0f);
}

int main(void)
{
    check_ft4();
    smoke_gt4();
    return 0;
}
