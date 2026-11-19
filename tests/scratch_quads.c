/* Synthetic host tests for the scratch_quads group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/scratch_quads.h"

struct DotrScratchVertex dotr_scratch_vertices_70002000[5];

static int calls;
static struct DotrScratchPacket *seen_packet;

/* mode 0: the fixed call sequence of lSzDuel_DrawFadeBoard_Pipe; mode 1: SzDuel_DrawCursor_Pipe, logged;
 * mode 2: SzDuel_DrawBoxCursor_Pipe, four faces checked against the tables below. */
static int mode;
static int faces, box_kind;
static const float box_x[4][4] = {{-24, -24, 24, 24}, {-24, -24, 24, 24}, {-24, -24, -24, -24}, {24, 24, 24, 24}};
static const float box_z[4][4] = {{-24, -24, -24, -24}, {24, 24, 24, 24}, {-24, -24, 24, 24}, {-24, -24, 24, 24}};
static int passes, alpha_calls, anchor_calls;
static float seen_x0[4];
static short seen_kind[4];
static int seen_block[4];
static int ot_word;

int func_002070f0_quad(void *m, int count, struct DotrScratchVertex *v, int a, int b, int c, float *average)
{
    if (mode == 2) {
        int i;

        assert(m == 0 && count == 4 && v == dotr_scratch_vertices_70002000);
        assert(a == 170 && b == 0x70000000 && c == 0 && average == 0);
        for (i = 0; i < 4; i++) {
            assert(v[i].position[0] == 100.0f + box_x[faces][i]);
            assert(v[i].position[1] == ((i == 0 || i == 3) ? 200.0f - 35.0f : -35.0f));
            assert(v[i].position[2] == 300.0f + box_z[faces][i]);
        }
        return 0x70f0 + faces;
    }
    if (mode == 1) {
        assert(m == 0 && count == 5 && v == dotr_scratch_vertices_70002000);
        assert(a == 170 && b == 0x70000000 && c == 0 && average == 0);
        seen_x0[passes] = v[0].position[0];
        return 0x70f0 + passes;
    }
    assert(calls == 0 && m == 0 && count == 5 && v == dotr_scratch_vertices_70002000);
    assert(a == 170 && b == 0x70000000 && c == 0 && average == 0);
    calls++;
    return 0x70f0;
}

void Ex3d_NodeV2S(int a, int count, struct DotrScratchVertex *v)
{
    if (mode == 2) {
        assert(a == 0 && count == 0x70f0 + faces && v == dotr_scratch_vertices_70002000);
        return;
    }
    if (mode == 1) {
        assert(a == 0 && count == 0x70f0 + passes && v == dotr_scratch_vertices_70002000);
        return;
    }
    assert(calls == 1 && a == 0 && count == 0x70f0 && v == dotr_scratch_vertices_70002000);
    calls++;
}

long ExGetAlphaVal(int index)
{
    if (mode == 1) {
        assert(index == 0x33);
        alpha_calls++;
        return 0x8d00;
    }
    assert(calls == 2 && index == 2);
    calls++;
    return 0x8d00;
}

int func_002086d0_quad(struct DotrScratchPacket *packet, long value)
{
    if (mode == 2) {
        assert(faces == 0 && value == 0x8000000048L && packet->unknown_02 == box_kind);
        alpha_calls++;
        return 0;
    }
    if (mode == 1) {
        assert(value == 0x8d00 && packet->unknown_02 == seen_kind[3]);
        alpha_calls += 0x10;
        return 0;
    }
    assert(calls == 3 && value == 0x8d00);
    assert(packet->unknown_00 == 0 && packet->unknown_02 == 5 && packet->unknown_04 == 0 && packet->unknown_08 == 0);
    seen_packet = packet;
    calls++;
    return 0;
}

int func_00207aa0_quad(int a, struct DotrScratchPacket *packet, int count, struct DotrScratchVertex *v, int e, int f)
{
    if (mode == 2) {
        assert(a == (faces == 0 ? 0x77 : 0x1000 + faces) && count == 0x70f0 + faces);
        assert(v == dotr_scratch_vertices_70002000 && e == 0 && f == 0);
        assert(packet->unknown_00 == 0 && packet->unknown_02 == box_kind);
        assert(packet->unknown_04 == 0 && packet->unknown_08 == 0);
        faces++;
        return 0x1000 + faces;
    }
    if (mode == 1) {
        assert(count == 0x70f0 + passes && v == dotr_scratch_vertices_70002000 && e == 0 && f == 0);
        assert(packet->unknown_00 == 0 && packet->unknown_04 == 0 && packet->unknown_08 == 0);
        seen_block[passes] = a;
        seen_kind[passes] = packet->unknown_02;
        passes++;
        return 0x1000 + passes;
    }
    assert(calls == 4 && a == 0x77 && packet == seen_packet && count == 0x70f0);
    assert(v == dotr_scratch_vertices_70002000 && e == 0 && f == 0);
    calls++;
    return 0;
}

int ExSetAnchor_Pipe(int block)
{
    assert(mode == 1 && block == 0x77 && passes == 0);
    anchor_calls++;
    return 0x1000;
}

int *ExGetMyOT(void)
{
    assert(mode == 1 && anchor_calls == 1);
    anchor_calls++;
    return &ot_word;
}

static float *want_centre;

int Ex3d_GetSampleZ(float *out, float dz)
{
    assert(mode == 1 && anchor_calls == 2 && out == want_centre && dz == 0.0f);
    anchor_calls++;
    return 0x5d10;
}

void ExSortAnchor_Pipe(int packet, int node, int *chain, int z)
{
    assert(mode == 1 && anchor_calls == 3);
    assert(packet == 0x1000 + passes && node == 0x77 && chain == &ot_word && z == 0x5d10);
    anchor_calls++;
}

/* Straight-line function: one call, every stored value and forwarded argument checked. */
static void check_quad(void)
{
    static const float want[5][3] = {
        {8.0f, 20.0f, 26.0f}, {12.0f, 20.0f, 26.0f}, {12.0f, 20.0f, 34.0f}, {8.0f, 20.0f, 34.0f}, {8.0f, 20.0f, 26.0f},
    };
    float centre[3] = {10.0f, 20.0f, 30.0f};
    struct DotrScratchVertex *v = dotr_scratch_vertices_70002000;
    int i, j;

    assert(sizeof(struct DotrScratchVertex) == 0x30);
    assert(offsetof(struct DotrScratchVertex, colour) == 0x18);
    assert(sizeof(struct DotrScratchPacket) == 16);
    memset(v, 0x55, sizeof(dotr_scratch_vertices_70002000));
    lSzDuel_DrawFadeBoard_Pipe(0x77, centre, 0x1c0, 2.0f, 4.0f);
    assert(calls == 5);
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 3; j++) {
            assert(v[i].position[j] == want[i][j]);
        }
        /* Bytes the function does not write stay as they were. */
        assert(v[i].unknown_0c[0] == 0x55 && v[i].unknown_20[15] == 0x55);
    }
    assert(v[0].colour[0] == 128 && v[0].colour[1] == 128 && v[0].colour[2] == 128 && v[0].colour[3] == 0xc0);
    assert(v[0].unknown_1c == 1.0f);
    assert(v[1].colour[0] == 0x55 && v[4].colour[3] == 0x55);
}

/* Branches and a bound: one call per outcome of `single`, of the null colour test, of the
 * alpha test (below 255 / 255) and the pass bound (one pass, three passes). */
static void check_cursor(void)
{
    static const float want[5][3] = {
        {8.0f, 20.0f, 26.0f}, {12.0f, 20.0f, 26.0f}, {12.0f, 20.0f, 34.0f}, {8.0f, 20.0f, 34.0f}, {8.0f, 20.0f, 26.0f},
    };
    float centre[3] = {10.0f, 20.0f, 30.0f};
    unsigned char colour[4] = {1, 2, 3, 255};
    struct DotrScratchVertex *v = dotr_scratch_vertices_70002000;
    int i, j;

    mode = 1;
    want_centre = centre;
    /* single pass, default colour: alpha 128 is below 255, so the alpha pair is called once. */
    memset(v, 0x55, sizeof(dotr_scratch_vertices_70002000));
    passes = alpha_calls = anchor_calls = 0;
    seen_kind[3] = 5;
    SzDuel_DrawCursor_Pipe(0x77, centre, 2.0f, 4.0f, 0x33, 0, 1);
    assert(passes == 1 && alpha_calls == 0x11 && anchor_calls == 4);
    assert(seen_block[0] == 0x1000 && seen_kind[0] == 5 && seen_x0[0] == 8.0f);
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 3; j++) {
            assert(v[i].position[j] == want[i][j]);
        }
        assert(v[i].unknown_0c[0] == 0x55 && v[i].unknown_20[15] == 0x55);
    }
    assert(v[0].colour[0] == 128 && v[0].colour[1] == 128 && v[0].colour[2] == 128 && v[0].colour[3] == 128);
    assert(v[0].unknown_1c == 1.0f && v[1].colour[0] == 0x55);

    /* three passes, given colour with alpha 255: no alpha calls, extents shrink by 0.2 per pass. */
    passes = alpha_calls = anchor_calls = 0;
    SzDuel_DrawCursor_Pipe(0x77, centre, 2.0f, 4.0f, 0x33, colour, 0);
    assert(passes == 3 && alpha_calls == 0 && anchor_calls == 4);
    assert(seen_block[0] == 0x1000 && seen_block[1] == 0x1001 && seen_block[2] == 0x1002);
    assert(seen_kind[0] == 0x402 && seen_kind[1] == 0x402 && seen_kind[2] == 0x402);
    assert(seen_x0[0] == 8.0f && seen_x0[1] > 8.19f && seen_x0[1] < 8.21f && seen_x0[2] > 8.39f && seen_x0[2] < 8.41f);
    assert(v[0].colour[0] == 3 && v[0].colour[1] == 2 && v[0].colour[2] == 1 && v[0].colour[3] == 255);
    assert(v[2].position[2] > 33.59f && v[2].position[2] < 33.61f);

    /* given colour with alpha below 255 and `single` set: one pass, alpha pair called. */
    colour[3] = 254;
    passes = alpha_calls = anchor_calls = 0;
    SzDuel_DrawCursor_Pipe(0x77, centre, 2.0f, 4.0f, 0x33, colour, 1);
    assert(passes == 1 && alpha_calls == 0x11 && anchor_calls == 4 && v[0].colour[3] == 254);
    mode = 0;
}

/* One case per branch outcome: colour null or given, `single` clear or set, alpha byte 255 or not. */
static void check_box_cursor(void)
{
    static float centre[3] = {100.0f, 200.0f, 300.0f};
    unsigned char colour[4] = {1, 2, 3, 255};
    struct DotrScratchVertex *v = dotr_scratch_vertices_70002000;

    mode = 2;
    memset(v, 0x55, sizeof(dotr_scratch_vertices_70002000));
    faces = alpha_calls = 0;
    box_kind = 2;
    assert(SzDuel_DrawBoxCursor_Pipe(0x77, centre, 0, 0) == 0x1004 && faces == 4 && alpha_calls == 0);
    assert(v[0].colour[0] == 0 && v[0].colour[1] == 0 && v[0].colour[2] == 0 && v[0].colour[3] == 255);
    assert(v[0].unknown_1c == 1.0f);

    faces = alpha_calls = 0;
    box_kind = 5;
    assert(SzDuel_DrawBoxCursor_Pipe(0x77, centre, colour, 1) == 0x1004 && faces == 4 && alpha_calls == 0);
    assert(v[0].colour[0] == 3 && v[0].colour[1] == 2 && v[0].colour[2] == 1 && v[0].colour[3] == 255);

    colour[3] = 254;
    faces = alpha_calls = 0;
    box_kind = 2;
    assert(SzDuel_DrawBoxCursor_Pipe(0x77, centre, colour, 0) == 0x1004 && faces == 4 && alpha_calls == 1);
    assert(v[0].colour[3] == 254);
    mode = 0;
}

int main(void)
{
    check_quad();
    check_cursor();
    check_box_cursor();
    return 0;
}
