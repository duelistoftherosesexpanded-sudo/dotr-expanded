/* Synthetic host tests for the paint_prims group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/paint_prims.h"

static struct DotrPaintPacket *alpha_packet;
static long alpha_value;
static int alpha_calls;
static int trans_result, trans_calls, sort_calls;
static void *trans_source, *trans_out;
static int trans_base, trans_arg2, sort_arg1, sort_arg2;
static struct DotrPaintWidePacket *trans_packet, *sort_packet;

int ExAssignAlpha(struct DotrPaintPacket *packet, long value)
{
    alpha_packet = packet;
    alpha_value = value;
    alpha_calls++;
    return 7;
}

int Ex3d_RotTransPersAuto(void *source, int base, int arg2,
                          struct DotrPaintWidePacket *packet, void *out)
{
    trans_source = source;
    trans_base = base;
    trans_arg2 = arg2;
    trans_packet = packet;
    trans_out = out;
    trans_calls++;
    return trans_result;
}

int ExSortPrim(struct DotrPaintWidePacket *packet, int arg1, int arg2)
{
    sort_packet = packet;
    sort_arg1 = arg1;
    sort_arg2 = arg2;
    sort_calls++;
    return 9;
}

/* Branch coverage: one case per outcome of the transform result. */
static void check_lg2(void)
{
    struct DotrPaintWidePacket packet;
    int source;
    int i;

    for (i = 0; i < 2; i++) {
        memset(&packet, 0, sizeof(packet));
        packet.entries[0].unknown_low = 0x11111111;
        packet.entries[0].unknown_high = 0x22;
        packet.entries[0].color = 0x123456;
        packet.entries[1].unknown_low = 0x33333333;
        packet.entries[1].unknown_high = 0x44;
        packet.entries[1].color = 0x654321;
        trans_result = i;
        trans_calls = sort_calls = 0;
        assert(PAINT_PIRM_LG2(&source, &packet, 5, 6) == 0);
        assert(trans_calls == 1 && trans_source == &source && trans_base == 0x70000000);
        assert(trans_arg2 == 5 && trans_packet == &packet && trans_out != NULL);
        assert(packet.entries[0].unknown_low == 0x11111111 && packet.entries[0].unknown_high == 0x22);
        assert(packet.entries[1].unknown_low == 0x33333333 && packet.entries[1].unknown_high == 0x44);
        if (i == 0) {
            assert(sort_calls == 0);
            assert(packet.entries[0].color == 0x123456 && packet.entries[1].color == 0x654321);
        } else {
            assert(sort_calls == 1 && sort_packet == &packet && sort_arg1 == 6 && sort_arg2 == 0);
            assert(packet.entries[0].color == 0xffffff && packet.entries[1].color == 0xffffff);
        }
    }
}

/* Straight-line coverage: every stored value, the forwarded arguments and the result. */
static void check_2g(void)
{
    struct DotrPaintPacket packet;
    unsigned char first[4] = { 1, 2, 3, 4 };
    unsigned char second[4] = { 5, 6, 7, 8 };

    memset(&packet, 0x5a, sizeof(packet));
    alpha_calls = 0;
    assert(PAINT__2G_SET_IN(&packet, first, second) == 0);
    assert(packet.flags == 0x11 && packet.count == 4 && packet.regs == 0x4141 && packet.unknown_04 == 0);
    assert(alpha_calls == 1 && alpha_packet == &packet && alpha_value == 0x8000000048);
    assert(memcmp(packet.entries[0].rgba, first, 4) == 0);
    assert(memcmp(packet.entries[1].rgba, second, 4) == 0);
    assert(packet.entries[0].unknown_04 == 0x3f800000 && packet.entries[1].unknown_04 == 0x3f800000);
    assert(packet.entries[0].unknown_0f == 0 && packet.entries[1].unknown_0f == 0);
    assert(packet.entries[0].unknown_08[6] == 0x5a && packet.entries[2].rgba[0] == 0x5a);
}

/* Straight-line coverage. */
static void check_gt4(void)
{
    struct DotrPaintPacket packet;

    memset(&packet, 0x5a, sizeof(packet));
    alpha_calls = 0;
    assert(PAINT__GT4_SET_IN(&packet) == 0);
    assert(packet.flags == 0x834 && packet.count == 12 && packet.regs == 0x421421421421);
    assert(packet.unknown_04 == 0);
    assert(alpha_calls == 1 && alpha_packet == &packet && alpha_value == 0x8000000048);
    assert(packet.entries[0].rgba[0] == 0x5a);
}

/* Branch coverage: one case per mode outcome. */
static void check_g4(void)
{
    struct DotrPaintPacket packet;
    int i, j;

    for (i = 0; i < 2; i++) {
        memset(&packet, 0x5a, sizeof(packet));
        alpha_calls = 0;
        assert(PAINT__G4_SET_IN(&packet, i) == 0);
        assert(packet.flags == 0x14 && packet.count == 8 && packet.regs == 0x41414141);
        assert(packet.unknown_04 == 0);
        assert(alpha_calls == 1 && alpha_packet == &packet);
        assert(alpha_value == (i == 0 ? 0x8000000048 : 0x8000000042));
        for (j = 0; j < 4; j++) {
            assert(packet.entries[j].unknown_04 == 0 && packet.entries[j].unknown_0f == 0);
            assert(packet.entries[j].rgba[0] == 0x5a && packet.entries[j].unknown_08[6] == 0x5a);
        }
    }
}

static struct DotrPaintVector *inin_rows[4];
static struct DotrPaintVector *inin_copy, inin_seen;
static int inin_calls, inin_args_ok;

int MATRIX_XYZ_ININ33(struct DotrPaintVector *row, struct DotrPaintVector *v, int arg2, int arg3)
{
    assert(inin_calls < 4);
    inin_rows[inin_calls] = row;
    if (inin_calls == 0) {
        inin_copy = v;
        inin_seen = *v;
    }
    inin_args_ok += v == inin_copy && arg2 == 11 && arg3 == 22;
    inin_calls++;
    return 0;
}

/* Straight-line coverage: every forwarded argument and the return value. */
static void check_matrix_4(void)
{
    struct DotrPaintVector rows[4];
    struct DotrPaintVector v = { 1.0f, 2.0f, 3.0f, 4.0f };

    assert(MATRIX_XYZ_ININ_4(rows, &v, 11, 22) == 0);
    assert(inin_calls == 4 && inin_args_ok == 4 && inin_copy != &v);
    assert(inin_rows[0] == &rows[0] && inin_rows[1] == &rows[1] && inin_rows[2] == &rows[2] && inin_rows[3] == &rows[3]);
    assert(inin_seen.x == 1.0f && inin_seen.y == 2.0f && inin_seen.z == 3.0f && inin_seen.w == 4.0f);
}

/* Branch coverage: both modes; only the 24-bit fields change. */
static void check_xyz_zin(void)
{
    struct DotrPaintGt4Packet gt4, gt4_want;
    struct DotrPaintWidePacket g4, g4_want;
    int i;

    memset(&gt4, 0x5a, sizeof(gt4));
    gt4_want = gt4;
    for (i = 0; i < 4; i++) {
        gt4_want.entries[i].color = 0xffffff;
    }
    assert(GT4_Xyz_ZIN(&gt4, 0) == 0);
    assert(memcmp(&gt4, &gt4_want, sizeof(gt4)) == 0);
    assert(((unsigned char *)&gt4)[32 + 4] == 0xff && ((unsigned char *)&gt4)[32 + 7] == 0x5a);

    memset(&g4, 0x5a, sizeof(g4));
    g4_want = g4;
    for (i = 0; i < 4; i++) {
        g4_want.entries[i].color = 0xffffff;
    }
    assert(GT4_Xyz_ZIN(&g4, 1) == 0);
    assert(memcmp(&g4, &g4_want, sizeof(g4)) == 0);
    assert(((unsigned char *)&g4)[24 + 4] == 0xff && ((unsigned char *)&g4)[24 + 3] == 0x5a);
}

static float to_int_log[4];
static int to_int_calls;
int func_001a22c0(float value)
{
    to_int_log[to_int_calls++ & 3] = value;
    return (int)value;
}

/* Straight-line function: one call checks every stored field, the helper arguments and the return value. */
static void check_box_in_2dmap(void)
{
    struct DotrPaintXyzPacket packet;
    struct DotrPaintVector size = {64.0f, 32.0f, 7.0f, 8.0f};
    struct DotrPaintVector pos = {100.0f, -10.0f, 5.0f, 6.0f};
    static const int want[4][2] = {
        {(2048 - 32 + 100) << 4, ((2048 - 16 + 0xfff6) << 4) & 0xffff},
        {(2048 + 32 + 100) << 4, ((2048 - 16 + 0xfff6) << 4) & 0xffff},
        {(2048 - 32 + 100) << 4, ((2048 + 16 + 0xfff6) << 4) & 0xffff},
        {(2048 + 32 + 100) << 4, ((2048 + 16 + 0xfff6) << 4) & 0xffff},
    };
    int i;

    memset(&packet, 0, sizeof(packet));
    for (i = 0; i < 4; i++) {
        packet.entries[i].unknown_00 = 0x1111 + i;
        packet.entries[i].unknown_08 = 0x2222 + i;
        packet.entries[i].unknown_high = 0xaa;
    }
    packet.header[0] = 0x55;
    packet.header[1] = 0x66;
    to_int_calls = 0;
    /* The source (src/ee/gt4_box) converts with C casts, which the target compiler
     * turns into calls to 0x1a22c0 and the host compiles inline: no helper calls on the host. */
    assert(GT4_BOX_IN__2DMAP(&packet, &size, &pos) == 0);
    assert(to_int_calls == 0);
    assert(packet.header[0] == 0x55 && packet.header[1] == 0x66);
    for (i = 0; i < 4; i++) {
        assert(packet.entries[i].unknown_00 == 0x1111 + i && packet.entries[i].unknown_08 == 0x2222 + i);
        assert((int)packet.entries[i].x == want[i][0] && (int)packet.entries[i].y == want[i][1]);
        assert(packet.entries[i].z == 0xffffff && packet.entries[i].unknown_high == 0);
    }
}

/* Straight-line leaf: one call, every stored value, the untouched bytes and the return. */
static void check_g4_box(void)
{
    static const unsigned short xs[4] = { 0x6c00, 0x9400, 0x6c00, 0x9400 };
    static const unsigned short ys[4] = { 0x7100, 0x7100, 0x8f00, 0x8f00 };
    struct DotrPaintPacket packet;
    unsigned char *bytes = (unsigned char *)&packet;
    unsigned short word;
    int i;
    int j;

    assert(sizeof(struct DotrPaintBoxEntry) == 16 && sizeof(packet) == 80);
    memset(&packet, 0xaa, sizeof(packet));
    assert(G4_BOX_IN__(&packet) == 0);
    for (j = 0; j < 16; j++) {
        assert(bytes[j] == 0xaa);
    }
    for (i = 0; i < 4; i++) {
        unsigned char *e = bytes + 16 + i * 16;

        for (j = 0; j < 4; j++) {
            assert(e[j] == 0xaa && e[4 + j] == 0);
        }
        memcpy(&word, e + 8, 2);
        assert(word == xs[i]);
        memcpy(&word, e + 10, 2);
        assert(word == ys[i]);
        assert(e[12] == 0xff && e[13] == 0xff && e[14] == 0xff && e[15] == 0);
    }
}

/* Smoke test only (docs/DECOMPILATION.md, Tests).
 * PAINT_PIRM_G4, PAINT_PIRM_GT4 and R3D_RRR_XYZ: one ordinary path each. */
static float depth_arg;
int func_001a23b0(float value)
{
    depth_arg = value;
    return 33;
}

static void check_pirm_smoke(void)
{
    struct DotrPaintWidePacket packet;
    struct DotrPaintVector a = { 5.0f, 7.0f, 9.0f, 1.5f }, b = { 1.0f, 2.0f, 3.0f, 4.0f }, out;
    int source = 0;

    trans_result = 1;
    sort_calls = 0;
    assert(PAINT_PIRM_G4(&source, &packet, 3, 4) == 0);
    assert(sort_calls == 1 && sort_packet == &packet && sort_arg1 == 4 && sort_arg2 == 33);
    assert(trans_base == 0x70000000 && trans_arg2 == 3 && trans_packet == &packet);
    assert(PAINT_PIRM_GT4(&source, &packet, 5, 6) == 0);
    assert(sort_calls == 2 && sort_arg1 == 6 && sort_arg2 == 33 && trans_arg2 == 5);

    R3D_RRR_XYZ(&out, &a, &b);
    assert(out.x == 4.0f && out.y == 5.0f && out.z == 6.0f && out.w == 1.5f);
}

int main(void)
{
    check_pirm_smoke();
    check_box_in_2dmap();
    check_lg2();
    check_2g();
    check_gt4();
    check_g4();
    check_matrix_4();
    check_xyz_zin();
    check_g4_box();
    return 0;
}
