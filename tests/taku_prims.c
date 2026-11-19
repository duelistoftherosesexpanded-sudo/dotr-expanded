/* Synthetic host tests for the taku_prims group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/taku_prims.h"

int *tex_data[8];

/* What ExSortPrim_Pipe is handed: the header and the accessed part of the packet. */
struct seen_packet {
    struct DotrPrimHeader header;
    unsigned char colour[4];
    float q;
    struct DotrPrimStVertex v[4];
};
static int order[8], calls;
static void *xywh_packet, *texture_packet, *alpha_packet;
static int xywh_args[16];
static int *seen_texture;
static int seen_texture_a;
static long seen_alpha;
static struct seen_packet seen;
static int seen_a1, seen_a2;

int *ExGetMyOT(void)
{
    order[calls++] = 1;
    return NULL;
}

void TAKU_FT4_XYWH(void *packet, int x0, int y0, int z0, int f0, int x1, int y1, int z1, int f1,
                   int x2, int y2, int z2, int f2, int x3, int y3, int z3, int f3)
{
    int a[16];

    a[0] = x0, a[1] = y0, a[2] = z0, a[3] = f0, a[4] = x1, a[5] = y1, a[6] = z1, a[7] = f1;
    a[8] = x2, a[9] = y2, a[10] = z2, a[11] = f2, a[12] = x3, a[13] = y3, a[14] = z3, a[15] = f3;
    memcpy(xywh_args, a, sizeof a);
    xywh_packet = packet;
    order[calls++] = 2;
}

int ExAssignTexture(struct DotrPrimHeader *packet, int *texture, int a)
{
    texture_packet = packet;
    seen_texture = texture;
    seen_texture_a = a;
    order[calls++] = 3;
    return 0;
}

int ExAssignAlpha(struct DotrPrimHeader *packet, long value)
{
    alpha_packet = packet;
    seen_alpha = value;
    order[calls++] = 4;
    return 0;
}

DotrPrimWord ExSortPrim_Pipe(DotrPrimWord cursor, void *packet, int a1, int a2)
{
    /* All callees get the same packet address. */
    assert(packet == xywh_packet && packet == texture_packet && packet == alpha_packet);
    memcpy(&seen, packet, sizeof seen);
    seen_a1 = a1;
    seen_a2 = a2;
    order[calls++] = 5;
    return cursor + 0x100;
}

/* Straight-line builder: one call checking the call order, every forwarded argument,
 * every stored field and the return value. */
static void check_ft4_st(void)
{
    static const int corners[16] = { 10, 20, 0xfffff8 + 7, 0, 30, 40, 0xfffff8 + 7, 0,
                                     50, 60, 0xfffff8 + 7, 0, 70, -80, 0xfffff8 + 7, 0 };
    int texture;
    int k;

    tex_data[5] = &texture;
    assert(Taku_Prim_Set_FT4_ST(0x1000, 10, 20, 30, 40, 50, 60, 70, -80, 0, 255, 16, 32, 64, 128,
                                -1, 100, 11, 22, 33, 44, 999, 5, 888, 7) == 0x1100);
    assert(calls == 5);
    for (k = 0; k < 5; k++) {
        assert(order[k] == k + 1);
    }
    assert(memcmp(xywh_args, corners, sizeof corners) == 0);
    assert(seen_texture == &texture && seen_texture_a == 0);
    assert(seen_alpha == 0x8000000088 && seen_a1 == 0 && seen_a2 == 0);
    assert(seen.header.unknown_00 == 9 && seen.header.unknown_02 == 36);
    assert(seen.header.unknown_04 == 0 && seen.header.unknown_08 == 0x424242421);
    assert(seen.colour[0] == 11 && seen.colour[1] == 22 && seen.colour[2] == 33);
    assert(seen.colour[3] == 44 && seen.q == 1.0f);
    assert(seen.v[0].s == 0.5f / 256.0f && seen.v[0].t == 255.5f / 256.0f);
    assert(seen.v[1].s == 16.5f / 256.0f && seen.v[1].t == 32.5f / 256.0f);
    assert(seen.v[2].s == 64.5f / 256.0f && seen.v[2].t == 128.5f / 256.0f);
    assert(seen.v[3].s == -0.5f / 256.0f && seen.v[3].t == 100.5f / 256.0f);
}

int main(void)
{
    check_ft4_st();
    return 0;
}
