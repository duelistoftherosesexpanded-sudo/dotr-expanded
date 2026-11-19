/* Synthetic host tests for the ex3d_clip group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/ex3d_clip.h"

static int divide_calls, divide_flags;
static float divide_t[8];
static union DotrEx3dVertex *divide_p0[8], *divide_p1[8];

void Ex3d_DivideN(union DotrEx3dVertex *p0, union DotrEx3dVertex *p1, int flags, union DotrEx3dVertex *out, float t)
{
    int i;

    if (divide_calls < 8) {
        divide_p0[divide_calls] = p0;
        divide_p1[divide_calls] = p1;
        divide_t[divide_calls] = t;
    }
    divide_calls++;
    divide_flags = flags;
    memset(out, 0, sizeof *out);
    for (i = 0; i < 4; i++) {
        out->v.pos[i] = p0->v.pos[i] + t * (p1->v.pos[i] - p0->v.pos[i]);
    }
    out->v.pair.unknown_0 = 77.0f;
}

static union DotrEx3dVertex verts[400], work[400];
static struct DotrEx3dPlane plane_x = { 1.0f, 0.0f, 0.0f, 0.0f, 2.0f }; /* distance = x / 2 */
static struct DotrEx3dPlane two[2] = { { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, -1.0f, 1.0f } };

static void set(int n, const float *xs)
{
    int i;

    memset(verts, 0, sizeof verts);
    memset(work, 0, sizeof work);
    for (i = 0; i < n; i++) {
        verts[i].v.pos[0] = xs[i];
        verts[i].v.pos[1] = (float)(i + 1);
        verts[i].v.pair.unknown_0 = (float)(10 + i);
        verts[i].v.pair.unknown_4 = (float)(20 + i);
    }
    divide_calls = 0;
}

static void check_clipping(void)
{
    static const float inside[3] = { 1.0f, 2.0f, 3.0f };
    static const float outside[3] = { -1.0f, -2.0f, -3.0f };
    static const float cross[3] = { 1.0f, -3.0f, 2.0f };
    static const float seg[2] = { 1.0f, -1.0f };
    static float many[341];
    int i;

    /* Fewer than two vertices: returned as given, nothing read. */
    set(3, inside);
    assert(Ex3d_Clipping(1, &plane_x, 1, verts, 0, work, 0) == 1);
    assert(Ex3d_Clipping(1, &plane_x, 0, verts, 0, work, 0) == 0);
    assert(divide_calls == 0 && verts[0].v.pos[0] == 1.0f);

    /* No planes: the count is still limited to 341; 341 itself and 2 pass unchanged. */
    assert(Ex3d_Clipping(0, &plane_x, 342, verts, 0, work, 0) == 341);
    assert(Ex3d_Clipping(0, &plane_x, 341, verts, 0, work, 1) == 341);
    assert(Ex3d_Clipping(0, &plane_x, 2, verts, 0, work, 1) == 2);

    /* All inside: copied through the work buffer unchanged. */
    assert(Ex3d_Clipping(1, &plane_x, 3, verts, 0, work, 0) == 3);
    assert(divide_calls == 0);
    assert(work[2].v.pos[0] == 3.0f && verts[2].v.pos[0] == 3.0f && verts[1].v.pair.unknown_4 == 21.0f);

    /* All outside: nothing kept, and the second plane sees no vertices. */
    set(3, outside);
    assert(Ex3d_Clipping(2, two, 3, verts, 0, work, 1) == 0);
    assert(divide_calls == 0 && verts[0].v.pos[0] == -1.0f);

    /* in -> out -> in: the first edge keeps its start and the cut, the second only the cut, the third
     * its start. */
    set(3, cross);
    assert(Ex3d_Clipping(1, &plane_x, 3, verts, 5, work, 3) == 4);
    assert(divide_calls == 2 && divide_flags == 3);
    assert(divide_p0[0] == &verts[0] && divide_p1[0] == &verts[1] && divide_t[0] == 0.25f);
    assert(divide_p0[1] == &verts[1] && divide_p1[1] == &verts[2] && divide_t[1] == 0.6f);
    assert(verts[0].v.pos[0] == 1.0f && verts[1].v.pos[0] == 0.0f && verts[2].v.pos[0] == 0.0f);
    assert(verts[3].v.pos[0] == 2.0f && verts[3].v.pos[1] == 3.0f);
    assert(verts[1].v.pair.unknown_0 == 77.0f);
    assert(verts[0].v.pair.unknown_0 == 10.0f && verts[0].v.pair.unknown_4 == 20.0f);

    /* Bit 0 of the flags clear: the first vertex's pair at +0x18 is put back after clipping. Here the first
     * vertex is outside, so slot 0 receives a cut (pair 77, 0) and then the saved pair. */
    set(3, cross);
    verts[0].v.pos[0] = -3.0f; verts[1].v.pos[0] = 1.0f;
    assert(Ex3d_Clipping(1, &plane_x, 3, verts, 0, work, 2) == 4);
    assert(work[0].v.pair.unknown_0 == 77.0f);
    assert(verts[0].v.pair.unknown_0 == 10.0f && verts[0].v.pair.unknown_4 == 20.0f);
    /* Bit 0 set: not put back. */
    set(3, cross);
    verts[0].v.pos[0] = -3.0f; verts[1].v.pos[0] = 1.0f;
    assert(Ex3d_Clipping(1, &plane_x, 3, verts, 0, work, 1) == 4);
    assert(verts[0].v.pair.unknown_0 == 77.0f && verts[0].v.pair.unknown_4 == 0.0f);

    /* Two vertices: three outputs (start, cut, cut) are limited to the input count. */
    set(2, seg);
    assert(Ex3d_Clipping(1, &plane_x, 2, verts, 0, work, 1) == 2);
    assert(divide_calls == 2 && verts[0].v.pos[0] == 1.0f && verts[1].v.pos[0] == 0.0f);
    /* Two vertices, both inside: two outputs, no limit applied. */
    set(2, inside);
    assert(Ex3d_Clipping(1, &plane_x, 2, verts, 0, work, 1) == 2);
    assert(divide_calls == 0 && verts[1].v.pos[0] == 2.0f);

    /* Output limit: 341 alternating vertices would give more than 341 outputs; the scan stops at 341. */
    for (i = 0; i < 341; i++) {
        many[i] = (i & 1) ? -1.0f : 1.0f;
    }
    set(341, many);
    assert(Ex3d_Clipping(1, &plane_x, 341, verts, 0, work, 1) == 341);
    assert(divide_calls == 227); /* 113 full pairs give 339 outputs; the next in -> out edge reaches 341 */

    /* Two planes: the second works on the first one's output and the plane pointer advances by 20 bytes. */
    set(3, inside);
    verts[0].v.pos[1] = 0.0f; /* y - 1 < 0 for vertex 0 only */
    assert(Ex3d_Clipping(2, two, 3, verts, 0, work, 1) == 4);
    assert(divide_calls == 2 && divide_p0[0] == &verts[0] && divide_t[0] == 0.5f);
}

static char order[8];
static int order_n, world_calls, round_calls;
static float *world_out, *pushed;
static struct DotrEx3dVector mul_in, mul_result;
static float round_log[4];

void RhView_GetWorldScreenMatrix(float *out)
{
    world_out = out;
    world_calls++;
}

void RhGeom_PushMatrix(float *matrix)
{
    pushed = matrix;
    order[order_n++] = 'P';
}

void RhGeom_MulMatrixVector(struct DotrEx3dVector *out, float *matrix, struct DotrEx3dVector *in)
{
    assert(out == in && matrix == 0);
    mul_in = *in;
    *out = mul_result;
    order[order_n++] = 'M';
}

static float *array_out, *array_matrix;
static struct DotrEx3dAutoIn *array_in;
static int array_count;
static float array_result[32];

void RhGeom_MulMatrixVectorArray(float *out, float *matrix, struct DotrEx3dAutoIn *in, int count)
{
    array_out = out;
    array_matrix = matrix;
    array_in = in;
    array_count = count;
    assert(count <= 8);
    memcpy(out, array_result, sizeof(float) * 4 * count);
    order[order_n++] = 'A';
}

void RhGeom_PopMatrix(void)
{
    order[order_n++] = 'O';
}

int func_001a23b0(float value)
{
    round_log[round_calls++ & 3] = value;
    return (int)value;
}

static int project(float x, float y, float z, float w, float *matrix, union DotrEx3dPacket *packet, float *w_out)
{
    static struct DotrEx3dVector in = { 1.0f, 2.0f, 3.0f, 9.0f };
    int r;

    mul_result.x = x; mul_result.y = y; mul_result.z = z; mul_result.w = w;
    order_n = 0; world_calls = 0; round_calls = 0;
    r = Ex3d_RotTransPers(&in, matrix, packet, w_out);
    assert(order_n == 3 && order[0] == 'P' && order[1] == 'M' && order[2] == 'O');
    assert(mul_in.x == 1.0f && mul_in.y == 2.0f && mul_in.z == 3.0f && mul_in.w == 1.0f);
    assert(in.w == 9.0f); /* the caller's vector is not written */
    return r;
}

static void check_rot_trans_pers(void)
{
    union DotrEx3dPacket packet;
    float matrix[16];
    float w = -1.0f;

    /* Own matrix: no world-screen lookup; x and y scaled by 16, z as is, the top eight bits kept. */
    memset(&packet, 0, sizeof packet);
    packet.xyz.unknown_56 = 0xab;
    assert(project(8.0f, 16.0f, 400.0f, 4.0f, matrix, &packet, &w) == 1);
    assert(world_calls == 0 && pushed == matrix);
    assert(round_calls == 3 && round_log[0] == 32.0f && round_log[1] == 64.0f && round_log[2] == 100.0f);
    assert(packet.xyz.x == 32 && packet.xyz.y == 64 && packet.xyz.z == 100 && packet.xyz.unknown_56 == 0xab);
    assert(w == 4.0f);

    /* Null matrix: the world-screen matrix is fetched into a local and pushed; null w is accepted. */
    assert(project(0.0f, 4095.0f, 1.0f, 1.0f, 0, &packet, 0) == 1);
    assert(world_calls == 1 && pushed == world_out && pushed != 0);
    assert(packet.xyz.x == 0 && packet.xyz.y == 4095 * 16 && packet.xyz.z == 1 && packet.xyz.unknown_56 == 0xab);
    assert(project(4095.0f, 0.0f, 16777215.0f, 1.0f, matrix, &packet, 0) == 1);
    assert(packet.xyz.x == 4095 * 16 && packet.xyz.y == 0 && packet.xyz.z == 0xffffff);

    /* Each rejection: z below 1 or above 16777215, x or y below 0 or above 4095. Nothing is written. */
    w = -1.0f;
    assert(project(1.0f, 1.0f, 0.5f, 1.0f, matrix, &packet, &w) == 0);
    assert(project(1.0f, 1.0f, 16777216.0f, 1.0f, matrix, &packet, &w) == 0);
    assert(project(-0.5f, 1.0f, 2.0f, 1.0f, matrix, &packet, &w) == 0);
    assert(project(4095.5f, 1.0f, 2.0f, 1.0f, matrix, &packet, &w) == 0);
    assert(project(1.0f, -0.5f, 2.0f, 1.0f, matrix, &packet, &w) == 0);
    assert(project(1.0f, 4095.5f, 2.0f, 1.0f, matrix, &packet, &w) == 0);
    assert(round_calls == 0 && w == -1.0f && packet.xyz.z == 0xffffff);
    /* The division by w applies before the tests. */
    assert(project(1.0f, 1.0f, 2.0f, 4.0f, matrix, &packet, &w) == 0);
}

static struct {
    struct DotrEx3dAutoPacket p;
    struct DotrEx3dAutoXyz more[7];
} auto_packet;
static struct DotrEx3dAutoIn auto_in[4];
static float auto_work[32];

/* One type-4 entry with w = 1: the result says whether the point is accepted. */
static int auto_one(float x, float y, float z)
{
    static float matrix[16];

    memset(&auto_packet, 0, sizeof auto_packet);
    auto_packet.p.count = 1;
    auto_packet.p.types = 4;
    array_result[0] = x, array_result[1] = y, array_result[2] = z, array_result[3] = 1.0f;
    order_n = 0;
    return Ex3d_RotTransPersAuto(auto_in, auto_work, matrix, &auto_packet.p, NULL);
}

static void check_rot_trans_pers_auto(void)
{
    static const float results[8] = { 2.0f, 4.0f, 8.0f, 2.0f, 30.0f, 60.0f, 90.0f, 3.0f };
    static float matrix[16];
    struct DotrEx3dAutoBits *bits;
    float avg = -1.0f;

    /* No matrix: the world-screen matrix is fetched and pushed. Entries 0 and 2 are type 4
     * (types 4, 2, 4): their inputs get w = 1, entry 1 and its input are left alone. */
    memset(&auto_packet, 0x55, sizeof auto_packet);
    memset(auto_in, 0, sizeof auto_in);
    auto_packet.p.count = 3;
    auto_packet.p.types = 0x424;
    memcpy(array_result, results, sizeof results);
    order_n = 0; world_calls = 0; round_calls = 0;
    assert(Ex3d_RotTransPersAuto(auto_in, auto_work, NULL, &auto_packet.p, &avg) == 1);
    assert(world_calls == 1 && pushed == world_out);
    assert(order_n == 3 && order[0] == 'P' && order[1] == 'A' && order[2] == 'O');
    assert(array_out == auto_work && array_matrix == NULL && array_in == auto_in && array_count == 2);
    assert(auto_in[0].w == 0x3f800000 && auto_in[1].w == 0x3f800000 && auto_in[2].w == 0);
    assert(round_calls == 6 && avg == 2.5f);
    /* (2,4,8)/2 and (30,60,90)/3: x and y in sixteenths, z in the 24 bits above them; the
     * top byte and the skipped entry keep their contents. */
    bits = (struct DotrEx3dAutoBits *)&auto_packet.p.xyz[0];
    assert(auto_packet.p.xyz[0].x == 16 && auto_packet.p.xyz[0].y == 32 && bits->z == 4);
    assert((unsigned)auto_packet.p.xyz[0].unknown_4 >> 24 == 0x55);
    assert(auto_packet.more[0].x == 0x5555 && auto_packet.more[0].unknown_4 == 0x55555555);
    bits = (struct DotrEx3dAutoBits *)&auto_packet.more[1];
    assert(auto_packet.more[1].x == 160 && auto_packet.more[1].y == 320 && bits->z == 30);
    assert(auto_work[0] == 1.0f && auto_work[1] == 2.0f && auto_work[2] == 4.0f && auto_work[3] == 2.0f);

    /* A given matrix is pushed as it is; a null `avg` is not written. An empty packet
     * still makes the three calls, with a count of 0. */
    world_calls = 0; round_calls = 0;
    assert(auto_one(1.0f, 2.0f, 3.0f) == 1 && world_calls == 0 && pushed != world_out);
    assert(order_n == 3 && array_count == 1 && round_calls == 3);
    assert(auto_packet.p.xyz[0].x == 16 && auto_packet.p.xyz[0].y == 32);
    memset(&auto_packet, 0, sizeof auto_packet);
    order_n = 0;
    assert(Ex3d_RotTransPersAuto(auto_in, auto_work, matrix, &auto_packet.p, NULL) == 1);
    assert(order_n == 3 && array_count == 0);

    /* Bounds: z in [1, 16777215], x and y in [0, 4095]; nothing is stored on a rejection. */
    assert(auto_one(1.0f, 1.0f, 1.0f) == 1 && auto_one(1.0f, 1.0f, 0.5f) == 0);
    assert(auto_packet.p.xyz[0].x == 0 && auto_packet.p.xyz[0].y == 0);
    assert(auto_one(1.0f, 1.0f, 16777215.0f) == 1 && auto_one(1.0f, 1.0f, 16777216.0f) == 0);
    assert(auto_one(0.0f, 1.0f, 2.0f) == 1 && auto_one(-0.5f, 1.0f, 2.0f) == 0);
    assert(auto_one(4095.0f, 1.0f, 2.0f) == 1 && auto_one(4095.5f, 1.0f, 2.0f) == 0);
    assert(auto_one(1.0f, 0.0f, 2.0f) == 1 && auto_one(1.0f, -0.5f, 2.0f) == 0);
    assert(auto_one(1.0f, 4095.0f, 2.0f) == 1 && auto_one(1.0f, 4095.5f, 2.0f) == 0);
    /* A rejected second entry returns 0 after the first has been stored; `avg` is left. */
    memset(&auto_packet, 0, sizeof auto_packet);
    auto_packet.p.count = 2;
    auto_packet.p.types = 0x44;
    memcpy(array_result, results, sizeof results);
    array_result[6] = 0.0f;
    avg = -1.0f;
    assert(Ex3d_RotTransPersAuto(auto_in, auto_work, matrix, &auto_packet.p, &avg) == 0);
    assert(auto_packet.p.xyz[0].x == 16 && auto_packet.more[0].x == 0 && avg == -1.0f);
}

int main(void)
{
    check_rot_trans_pers_auto();
    check_rot_trans_pers();
    check_clipping();
    return 0;
}
