/* Synthetic host tests for the leader_base group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/leader_base.h"

struct DotrLeaderBaseSide gSzDuel_Base[2];
struct DotrLeaderBaseControl data_00317490;
struct DotrLeaderBaseCorners data_00291590;
struct DotrLeaderBaseVertex dotr_leader_base_vertices_70002000[5];

static struct DotrLeaderBaseUnit unit;
static struct DotrLeaderBaseUnit *unit_result;
static struct DotrLeaderBaseCell cell;
static int textures[2];
static int calls;
static int nodes;
static int side_now;
static int level_now;
static float half_now;
static float centre[3] = {10.0f, 20.0f, 30.0f};

struct DotrLeaderBaseUnit *SzDuel_GetUnit(int side, int index)
{
    assert(calls == 0 && side == side_now && index == 0);
    calls++;
    return unit_result;
}

struct DotrLeaderBaseCell *SzDuel_GetMapInfo(int x, int y)
{
    assert(calls == 1 && x == 3 && y == 4);
    calls++;
    return &cell;
}

int SzModel_GetRot(int index, int *out)
{
    assert(calls == 2 && index == 7);
    out[0] = 11;
    out[1] = 22;
    out[2] = 33;
    calls++;
    return 0;
}

static float *seen_matrix;

void RhGeom_GetRotXYZMatrix(float *m, int *ang)
{
    assert(calls == 3 && ang[0] == 0 && ang[1] == 22 && ang[2] == 0);
    seen_matrix = m;
    calls++;
}

void RhGeom_PushMatrix(float *matrix)
{
    assert(calls == 4 && matrix == seen_matrix);
    calls++;
}

/* Leaves the vectors as they are; checks the values written before the rotation. */
void RhGeom_MulMatrixVectorArray(float *out, float *m, float *in, int count)
{
    assert(calls == 5 && m == 0 && out == in && count == 4);
    assert(in[0] == half_now && in[1] == 1.0f && in[2] == half_now && in[3] == 5.0f);
    assert(in[4] == half_now && in[5] == 2.0f && in[6] == -half_now);
    assert(in[8] == -half_now && in[9] == 3.0f && in[10] == -half_now);
    assert(in[12] == -half_now && in[13] == 4.0f && in[14] == half_now);
    calls++;
}

void RhGeom_PopMatrix(void)
{
    assert(calls == 6);
    calls++;
}

static struct DotrLeaderBasePacket *seen_packet;

int ExAssignAlpha(struct DotrLeaderBasePacket *packet, long value)
{
    assert(packet->unknown_00 == 0 && packet->unknown_04 == 0 && packet->unknown_08 == 0);
    if (calls == 7) {
        assert(packet->unknown_02 == 53 && value == 0x8000000044L);
        seen_packet = packet;
    } else {
        assert(calls == 12 && packet == seen_packet && packet->unknown_02 == 0x402 && value == 0x8000000048L);
    }
    calls++;
    return 0;
}

int ExAssignTexture(struct DotrLeaderBasePacket *packet, int *texture, int a)
{
    assert(calls == 8 && packet == seen_packet && texture == &textures[side_now] && a == 1);
    calls++;
    return 0;
}

int Ex3d_NodeL2V(void *m, int count, struct DotrLeaderBaseVertex *v, int a, int b, int c, float *average)
{
    static const float sx[5] = {1.0f, 1.0f, -1.0f, -1.0f, 1.0f};
    static const float sz[5] = {1.0f, -1.0f, -1.0f, 1.0f, 1.0f};
    int shade = (200 * level_now) >> 7;
    int i;

    assert(m == 0 && v == dotr_leader_base_vertices_70002000);
    assert(a == 170 && b == 0x70000000 && c == 15 && average == 0);
    if (calls == 9) {
        assert(count == 4);
        for (i = 0; i < 4; i++) {
            assert(v[i].colour[0] == shade && v[i].colour[1] == shade && v[i].colour[2] == shade);
            assert(v[i].colour[3] == 0x66 && v[i].unknown_1c == 1.0f);
            assert(v[i].uv[0] == (side_now ? 0.5f : 0.0f) + (i < 2 ? 0.0f : 0.5f));
            assert(v[i].uv[1] == ((i == 0 || i == 3) ? 0.0f : 1.0f));
        }
    } else {
        assert(calls == 13 && count == 5);
        assert(v[0].colour[0] == (30 * level_now) >> 7 && v[0].colour[1] == (20 * level_now) >> 7);
        assert(v[0].colour[2] == (10 * level_now) >> 7 && v[0].colour[3] == 0x66 && v[0].unknown_1c == 1.0f);
    }
    for (i = 0; i < count; i++) {
        assert(v[i].position[0] == 10.0f + sx[i] * half_now);
        assert(v[i].position[1] == 20.0f + (float)(i % 4 + 1));
        assert(v[i].position[2] == 30.0f + sz[i] * half_now);
    }
    calls++;
    return 0x70f0 + calls;
}

void Ex3d_NodeV2S(int a, int count, struct DotrLeaderBaseVertex *v)
{
    assert((calls == 10 || calls == 14) && a == 0 && count == 0x70f0 + calls);
    assert(v == dotr_leader_base_vertices_70002000);
    calls++;
}

int ExSortNode_Pipe(int a, struct DotrLeaderBasePacket *packet, int count, struct DotrLeaderBaseVertex *v, int e,
                    int f)
{
    assert(calls == 11 || calls == 15);
    assert(a == (calls == 11 ? 0x500 : 0x601) && packet == seen_packet && count == 0x70f0 + calls - 1);
    assert(v == dotr_leader_base_vertices_70002000 && e == 0 && f == 0);
    calls++;
    nodes++;
    return 0x600 + nodes;
}

static void reset(int side)
{
    int i;

    memset(&unit, 0, sizeof(unit));
    memset(&cell, 0, sizeof(cell));
    memset(dotr_leader_base_vertices_70002000, 0, sizeof(dotr_leader_base_vertices_70002000));
    for (i = 0; i < 4; i++) {
        data_00291590.v[i][0] = 99.0f;
        data_00291590.v[i][1] = (float)(i + 1);
        data_00291590.v[i][2] = 99.0f;
        data_00291590.v[i][3] = 5.0f;
    }
    data_00317490.unknown_d34 = 96;
    data_00317490.texture[0] = &textures[0];
    data_00317490.texture[1] = &textures[1];
    gSzDuel_Base[0].texture = 0;
    gSzDuel_Base[1].texture = 1;
    half_now = 16.666666f * 96.0f / 128.0f;
    unit.unknown_40 = 3;
    unit.unknown_41 = 4;
    unit.unknown_46 = 7;
    unit.unknown_4d = 200;
    cell.unknown_10 = 10;
    cell.unknown_11 = 20;
    cell.unknown_12 = 30;
    cell.unknown_1a = 64;
    unit_result = &unit;
    side_now = side;
    level_now = 64;
    calls = 0;
    nodes = 0;
}

/* One assertion per branch outcome: no unit, unit word set, each unit flag, both sides,
 * with and without the outline. */
static void check_leader_base(void)
{
    reset(0);
    unit_result = 0;
    assert(SzDuel_DrawLeaderBase_Pipe(0x500, 0, centre, 0x66, 1) == 0x500 && calls == 1);

    reset(0);
    unit.unknown_04 = 1;
    assert(SzDuel_DrawLeaderBase_Pipe(0x500, 0, centre, 0x66, 1) == 0x500 && calls == 1);

    reset(0);
    assert(SzDuel_DrawLeaderBase_Pipe(0x500, 0, centre, 0x66, 0) == 0x601 && calls == 12);

    reset(1);
    assert(SzDuel_DrawLeaderBase_Pipe(0x500, 1, centre, 0x66, 1) == 0x602 && calls == 16);

    reset(1);
    unit.unknown_57_7 = -1;
    level_now = 128;
    assert(SzDuel_DrawLeaderBase_Pipe(0x500, 1, centre, 0x66, 1) == 0x602 && calls == 16);

    reset(0);
    unit.unknown_58_1 = -1;
    level_now = 128;
    assert(SzDuel_DrawLeaderBase_Pipe(0x500, 0, centre, 0x66, 0) == 0x601 && calls == 12);
}

int main(void)
{
    check_leader_base();
    return 0;
}
