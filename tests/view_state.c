/* Synthetic tests of the view-state accessors and vector routines. The
 * dependency 0x1a6c60 is stubbed as a square root (an assumption). */
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/view_state.h"

struct DotrViewEntry gRhView_avl[3];
int gRhView_nStackIndex[1];

static int root_calls;
static float root_argument;

/* Newton iteration; inputs used here are exact squares. */
float func_001a6c60(float value)
{
    float r = value > 1.0f ? value : 1.0f;
    int i;
    root_calls++;
    root_argument = value;
    for (i = 0; i < 40; i++) r = 0.5f * (r + value / r);
    return r;
}

static void check_vectors(void)
{
    struct DotrViewVector a = {3.0f, 4.0f, 12.0f, 7.0f}, b = {1.0f, 0.0f, 0.0f, 9.0f}, out;
    struct DotrViewVector x = {1.0f, 0.0f, 0.0f, 5.0f}, y = {0.0f, 1.0f, 0.0f, 6.0f};
    assert(RhGeom_InnerProduct(&a, &b) == 3.0f && RhGeom_InnerProduct(&a, &a) == 169.0f);
    root_calls = 0;
    assert(RhGeom_GetVectorScalor(&a) == 13.0f && root_calls == 1 && root_argument == 169.0f);
    /* (3,4,12) - (0,0,0) with b moved: distance to (3,0,12) is 4. */
    b.x = 3.0f;
    b.z = 12.0f;
    assert(RhGeom_GetPointsDistance(&a, &b) == 4.0f && root_argument == 16.0f);
    /* Cross product; the fourth component is taken from the first operand. */
    memset(&out, 0x7f, sizeof(out));
    RhGeom_OuterProduct(&out, &x, &y);
    assert(out.x == 0.0f && out.y == 0.0f && out.z == 1.0f && out.unknown_w == 5.0f);
    RhGeom_OuterProduct(&out, &y, &x);
    assert(out.z == -1.0f && out.unknown_w == 6.0f);
    /* Normalization divides the first three components and keeps the fourth. */
    a.x = 0.0f;
    a.y = 6.0f;
    a.z = 8.0f;
    RhGeom_NormalizeVector(&a);
    assert(a.x == 0.0f && a.y == 0.6f && a.z == 0.8f && a.unknown_w == 7.0f);
}

/* 0x2060f0 and 0x2061f0: mean of (origin - point), then the real normalization. */
static struct DotrViewMatrix *matrix_seen;
static int matrix_calls;

/* Stand-in for the VU0 routine 0x20bbb0: records the pointer and writes an
 * identity matrix, an assumption of this test only. */
void RhGeom_UnitMatrix(struct DotrViewMatrix *m)
{
    int i;
    matrix_seen = m;
    matrix_calls++;
    for (i = 0; i < 16; i++) m->unknown_values[i] = (i % 5 == 0) ? 1.0f : 0.0f;
}

static float mul_m[16], mul_other[16];
static struct DotrViewMatrix *mul_m_ptr, *mul_other_ptr;
void RhGeom_MulMatrix(struct DotrViewMatrix *m, struct DotrViewMatrix *other)
{
    mul_m_ptr = m; mul_other_ptr = other;
    memcpy(mul_m, m->unknown_values, sizeof(mul_m));
    memcpy(mul_other, other->unknown_values, sizeof(mul_other));
}

/* Stand-ins for the 0x20e260 helpers; each records its argument and call position. */
struct DotrViewMatrix InvertView;
static int install_order[8], install_count;
static void *copy_args[3];
static int copy_count;
static void *copy_log[4][2];
static int copy_log_count[4], copy_calls;
static void *ba90_arg, *bad0_arg;
void *RhMem_CopyMemory16(void *destination, void *source, int count)
{
    copy_args[0] = destination; copy_args[1] = source; copy_count = count;
    if (copy_calls < 4) {
        copy_log[copy_calls][0] = destination; copy_log[copy_calls][1] = source; copy_log_count[copy_calls] = count;
    }
    copy_calls++;
    install_order[install_count++] = 1;
    return destination;
}
int lRhView_ConstructMatrix(void) { install_order[install_count++] = 2; return 1; }
int RhGeom_ResetMatrixStack(void) { install_order[install_count++] = 3; return 0; }
void RhGeom_SetMatrix(struct DotrViewMatrix *m) { ba90_arg = m; install_order[install_count++] = 4; }
void RhGeom_GetMatrix(struct DotrViewBlock *block) { bad0_arg = block; install_order[install_count++] = 5; }

/* Draw-chain stand-ins for 0x20ec60: each returns a distinct tag so the threading is visible. */
static struct DotrDrawTag *chain_tag[6];
static void *chain_arg[6];
static float chain_f[6][4];
static int chain_mode, chain_n;
static struct DotrDrawTag *chain_ret(int n, struct DotrDrawTag *in, void *arg)
{
    chain_tag[n] = in; chain_arg[n] = arg; chain_n++;
    return (struct DotrDrawTag *)(intptr_t)(0x1000 * (n + 1));
}
struct DotrDrawTag *RhVu1Mac_SetViewScreenMatrix(struct DotrDrawTag *tag, void *source) { return chain_ret(0, tag, source); }
struct DotrDrawTag *RhVu1Mac_SetScreenWorldMatrix(struct DotrDrawTag *tag, void *source) { return chain_ret(1, tag, source); }
struct DotrDrawTag *RhVu1Mac_SetClipParameter(struct DotrDrawTag *tag, int mode, float x, float y, float z)
{
    chain_mode = mode; chain_f[2][0] = x; chain_f[2][1] = y; chain_f[2][2] = z;
    return chain_ret(2, tag, 0);
}
struct DotrDrawPacket *RhBase_SetFogColor(struct DotrDrawPacket *packet, unsigned char *bytes)
{
    return (struct DotrDrawPacket *)chain_ret(3, (struct DotrDrawTag *)packet, bytes);
}
struct DotrDrawTag *RhVu1Mac_SetFogParameterEx(struct DotrDrawTag *tag, float a, float b, float c, float d)
{
    chain_f[4][0] = a; chain_f[4][1] = b; chain_f[4][2] = c; chain_f[4][3] = d;
    return chain_ret(4, tag, 0);
}

static void check_translation(void)
{
    struct DotrViewMatrix m;
    struct DotrViewVector v = {2.0f, 3.0f, 4.0f, 9.0f};
    int i;
    memset(&m, 0x7f, sizeof(m));
    matrix_calls = 0;
    RhGeom_GetTransMatrix(&m, &v);
    assert(matrix_calls == 1 && matrix_seen == &m);
    assert(m.unknown_values[12] == 2.0f && m.unknown_values[13] == 3.0f && m.unknown_values[14] == 4.0f);
    assert(m.unknown_values[15] == 1.0f && v.unknown_w == 9.0f);
    for (i = 0; i < 12; i++) assert(m.unknown_values[i] == ((i % 5 == 0) ? 1.0f : 0.0f));
}

static void check_means(void)
{
    struct DotrViewVector origin = {0.0f, 10.0f, 10.0f, 3.0f}, out;
    struct DotrViewVector pts[2] = {{0.0f, -4.0f, -6.0f, 1.0f}, {0.0f, -8.0f, -10.0f, 1.0f}};
    struct DotrViewVector near[2] = {{0.0f, 2.0f, 0.0f, 1.0f}, {0.0f, 6.0f, 4.0f, 1.0f}};
    struct DotrViewVectorTriple tri[2], tri_near[2];
    int i;

    memset(&out, 0x7f, sizeof(out));
    root_calls = 0;
    assert(Ex3d_GetViewVector(&origin, 0, pts, &out) == 0);
    assert(out.x == 0.0f && out.y == 0.0f && out.z == 0.0f && out.unknown_w == 0.0f && root_calls == 0);
    memset(&out, 0x7f, sizeof(out));
    assert(Ex3d_GetViewVectorForNode(&origin, 0, tri, &out) == 0);
    assert(out.x == 0.0f && out.y == 0.0f && out.z == 0.0f && out.unknown_w == 0.0f && root_calls == 0);

    /* Null origin: mean of (0 - point) = (0, 6, 8), normalized. */
    assert(Ex3d_GetViewVector(0, 2, pts, &out) == 1 && root_calls == 1);
    assert(out.x == 0.0f && out.y == 0.6f && out.z == 0.8f && out.unknown_w == 0.0f);
    /* Origin given: (0,10,10) - near[i] averages to (0, 6, 8). */
    memset(&out, 0x7f, sizeof(out));
    assert(Ex3d_GetViewVector(&origin, 2, near, &out) == 1);
    assert(out.x == 0.0f && out.y == 0.6f && out.z == 0.8f && out.unknown_w == 0.0f);

    /* Same with a 48-byte stride: only the first 16 bytes of each element are points. */
    memset(tri, 0x7f, sizeof(tri));
    memset(tri_near, 0x7f, sizeof(tri_near));
    for (i = 0; i < 2; i++) {
        tri[i].unknown_00 = pts[i];
        tri_near[i].unknown_00 = near[i];
    }
    assert(Ex3d_GetViewVectorForNode(0, 2, tri, &out) == 1);
    assert(out.x == 0.0f && out.y == 0.6f && out.z == 0.8f && out.unknown_w == 0.0f);
    assert(Ex3d_GetViewVectorForNode(&origin, 2, tri_near, &out) == 1);
    assert(out.x == 0.0f && out.y == 0.6f && out.z == 0.8f && out.unknown_w == 0.0f);
}

static void check_entries(void)
{
    struct DotrViewEntry expected[3];
    struct DotrViewBlock block;
    struct DotrViewQuad quad = {9, 8, 7, 6}, quad_out;
    float a, b, c, d;
    int index;
    assert(RhView_Term() == 1);
    /* Index decrement: positive values step down and report 1. */
    gRhView_nStackIndex[0] = 2;
    assert(RhView_PopStack() == 1 && gRhView_nStackIndex[0] == 1);
    assert(RhView_PopStack() == 1 && gRhView_nStackIndex[0] == 0);
    assert(RhView_PopStack() == 0 && gRhView_nStackIndex[0] == 0);
    gRhView_nStackIndex[0] = -4;
    assert(RhView_PopStack() == 0 && gRhView_nStackIndex[0] == -4);
    for (index = 0; index < 3; index++) {
        struct DotrViewEntry *e = &expected[index];
        gRhView_nStackIndex[0] = index;
        memset(gRhView_avl, 0x24 + index, sizeof(gRhView_avl));
        memcpy(expected, gRhView_avl, sizeof(expected));
        /* Only the selected entry changes, and only the named fields. */
        e->unknown_160 = 1.0f;
        e->unknown_164 = 2.0f;
        e->unknown_168 = 3.0f;
        e->unknown_150 = 4.0f;
        e->unknown_154 = 5.0f;
        assert(RhView_SetScreen(1.0f, 2.0f, 3.0f, 4.0f, 5.0f) == 1);
        assert(memcmp(expected, gRhView_avl, sizeof(expected)) == 0);
        e->unknown_158 = 6.0f;
        e->unknown_15c = 7.0f;
        assert(RhView_SetAspect(6.0f, 7.0f) == 1 && memcmp(expected, gRhView_avl, sizeof(expected)) == 0);
        e->unknown_16c = 8.0f;
        e->unknown_170 = 9.0f;
        assert(RhView_SetClipZ(8.0f, 9.0f) == 1 && memcmp(expected, gRhView_avl, sizeof(expected)) == 0);
        e->unknown_174 = 10.0f;
        e->unknown_178 = 11.0f;
        e->unknown_17c = 12.0f;
        e->unknown_180 = 13.0f;
        e->unknown_184 = quad;
        assert(RhView_SetFogParameter(10.0f, 11.0f, 12.0f, 13.0f, &quad) == 1);
        assert(memcmp(expected, gRhView_avl, sizeof(expected)) == 0);
        assert(RhView_GetScreenCenter(&a, &b) == 1 && a == 4.0f && b == 5.0f);
        assert(RhView_GetScreenDist(&a) == 1 && a == 1.0f);
        assert(RhView_GetClipZ(&a, &b) == 1 && a == 8.0f && b == 9.0f);
        assert(RhView_GetFogParameter(&a, &b, &c, &d, &quad_out) == 1);
        assert(a == 10.0f && b == 11.0f && c == 12.0f && d == 13.0f);
        assert(memcmp(&quad_out, &quad, sizeof(quad)) == 0);
        memset(&gRhView_avl[index].unknown_0c0, 0x90 + index, sizeof(block));
        memset(&block, 0, sizeof(block));
        assert(RhView_GetCurView(&block) == 1);
        assert(memcmp(&block, &gRhView_avl[index].unknown_0c0, sizeof(block)) == 0);
    }
}

struct DotrViewControl gRhView_FakeMirror[1];

static void check_stack(void)
{
    static struct DotrViewEntry expected[3];
    struct DotrViewMatrix matrix;
    struct DotrViewBlock block;
    size_t i;
    for (i = 0; i < sizeof(gRhView_avl); i++) {
        ((unsigned char *)gRhView_avl)[i] = (unsigned char)(i * 7 + i / 416);
    }
    memcpy(expected, gRhView_avl, sizeof(expected));
    /* 0x20e460 copies the selected entry to the next one for depth 0 only. */
    gRhView_nStackIndex[0] = 1;
    assert(RhView_PushStack() == 0 && gRhView_nStackIndex[0] == 1);
    assert(memcmp(gRhView_avl, expected, sizeof(expected)) == 0);
    gRhView_nStackIndex[0] = 0;
    assert(RhView_PushStack() == 1 && gRhView_nStackIndex[0] == 1);
    expected[1] = expected[0];
    assert(memcmp(gRhView_avl, expected, sizeof(expected)) == 0);

    /* The three 64-byte copies follow the selected entry. */
    gRhView_avl[1].unknown_040.unknown_values[3] = 9.5f;
    RhView_GetWorldScreenMatrix(&matrix);
    assert(memcmp(&matrix, &gRhView_avl[1].unknown_000, sizeof(matrix)) == 0);
    RhView_GetWorldViewMatrix(&matrix);
    assert(memcmp(&matrix, &gRhView_avl[1].unknown_040, sizeof(matrix)) == 0 && matrix.unknown_values[3] == 9.5f);
    RhView_GetViewScreenMatrix(&matrix);
    assert(memcmp(&matrix, &gRhView_avl[1].unknown_080, sizeof(matrix)) == 0);
    assert(memcmp(&matrix, &gRhView_avl[1].unknown_040, sizeof(matrix)) != 0);

    /* 0x20e970 is gated by the control word, lowers a positive selector,
     * clears the word and always copies entry 0's block. */
    memset(gRhView_FakeMirror, 0x33, sizeof(gRhView_FakeMirror));
    gRhView_FakeMirror[0].unknown_44 = 0;
    memset(&block, 0, sizeof(block));
    assert(RhView_PopFakeMirror(&block) == 0 && gRhView_nStackIndex[0] == 1);
    gRhView_FakeMirror[0].unknown_44 = 5;
    assert(RhView_PopFakeMirror(&block) == 1 && gRhView_nStackIndex[0] == 0 && gRhView_FakeMirror[0].unknown_44 == 0);
    assert(memcmp(&block, &gRhView_avl[0].unknown_0c0, sizeof(block)) == 0);
    gRhView_FakeMirror[0].unknown_44 = 5;
    assert(RhView_PopFakeMirror(&block) == 1 && gRhView_nStackIndex[0] == 0);
    assert(gRhView_FakeMirror[0].unknown_00[0] == 0x33);
}

/* 0x20e1a0 branch coverage: explicit frame and null frame (entry 1). */
static void check_frame_defaults(void)
{
    static const float expect[20] = {0, 0, 0, 1, 0, 0, -1, 0, 0, 0, 0, 1, 0, 0, -1, 0, 0, 0, 0, 0};
    struct DotrViewFrame frame, *entry_frame;
    int i;
    memset(&frame, 0x55, sizeof(frame));
    matrix_calls = 0;
    assert(RhView_Reset(&frame) == 1 && matrix_calls == 1 && matrix_seen == &frame.unknown_00);
    for (i = 0; i < 20; i++) assert(frame.unknown_40[i] == expect[i]);
    gRhView_nStackIndex[0] = 1;
    entry_frame = (struct DotrViewFrame *)&gRhView_avl[1].unknown_0c0;
    memset(gRhView_avl, 0x55, sizeof(gRhView_avl));
    assert(RhView_Reset(0) == 1 && matrix_calls == 2 && matrix_seen == &entry_frame->unknown_00);
    for (i = 0; i < 20; i++) assert(entry_frame->unknown_40[i] == expect[i]);
}

static float stub_z, stub_w;
/* 0x205cb0 stubs: fixed float, and recorders for the two matrix helpers. */
static struct DotrViewVector transform_in;
static struct DotrViewMatrix *bde0_out, *bde0_in, *cdd0_m;
static struct DotrViewVector *cdd0_out, *cdd0_v;
static int helper_order;
static struct DotrViewMatrix *bde0_log[4][2];
static int bde0_calls;
float SzView_GetScreenDistance(int index) { return index == 0 ? 2.5f : -1.0f; }
void RhGeom_InvertMatrix(struct DotrViewMatrix *out, struct DotrViewMatrix *m)
{
    bde0_out = out;
    bde0_in = m;
    bde0_log[bde0_calls & 3][0] = out;
    bde0_log[bde0_calls & 3][1] = m;
    bde0_calls++;
    helper_order = helper_order * 10 + 1;
}
void RhGeom_MulMatrixVector(struct DotrViewVector *out, struct DotrViewMatrix *m, struct DotrViewVector *v)
{
    cdd0_out = out;
    cdd0_m = m;
    cdd0_v = v;
    transform_in = *v;
    out->z = stub_z;
    out->unknown_w = stub_w;
    helper_order = helper_order * 10 + 2;
}

/* 0x205cb0 straight-line coverage. */
static void check_point_transform(void)
{
    struct DotrViewVector out;
    memset(&out, 0x7f, sizeof(out));
    gRhView_nStackIndex[0] = 0;
    helper_order = 0;
    Ex3d_GetPointOnScreen(&out, 3.0f, -4.0f);
    assert(transform_in.x == 3.0f && transform_in.y == -4.0f && transform_in.z == 2.5f && transform_in.unknown_w == 1.0f);
    assert(helper_order == 12 && bde0_out == bde0_in && cdd0_m == bde0_in);
    assert(cdd0_out == &out && cdd0_v == &out);
}

/* 0x205d10 stubs: the transform helper's result z/w comes from test inputs. */
static float ratio_seen, ratio_ret, ratio_log[8];
static int ratio_calls;
int func_001a23b0(float ratio)
{
    ratio_seen = ratio;
    ratio_log[ratio_calls & 7] = ratio;
    ratio_calls++;
    return (int)ratio_ret;
}

/* 0x205d10 branch coverage: w<=0, z<=0, forwarded ratio. */
static void check_depth_ratio(void)
{
    struct DotrViewVector out;
    memset(&out, 0, sizeof(out));
    ratio_ret = 77;
    helper_order = 0;
    stub_w = 0.0f; stub_z = 5.0f; ratio_calls = 0;
    assert(Ex3d_GetSampleZ(&out, 1.0f) == 0xfffff6 && ratio_calls == 0);
    stub_w = -1.0f;
    assert(Ex3d_GetSampleZ(&out, 1.0f) == 0xfffff6);
    stub_w = 2.0f; stub_z = 0.0f;
    assert(Ex3d_GetSampleZ(&out, 0.0f) == 0 && ratio_calls == 0);
    stub_z = -3.0f;
    assert(Ex3d_GetSampleZ(&out, 0.0f) == 0);
    stub_z = 6.0f;
    assert(Ex3d_GetSampleZ(&out, 0.0f) == 77 && ratio_calls == 1 && ratio_seen == 3.0f);
    assert(out.unknown_w == 1.0f);
}

/* 0x205df0 straight-line coverage: both transforms run in order, z lowered by dz, w returned. */
static void check_depth_w(void)
{
    struct DotrViewVector out;
    memset(&out, 0, sizeof(out));
    stub_z = 9.0f; stub_w = 4.5f;
    helper_order = 0;
    assert(Ex3d_GetSampleW(&out, 2.0f) == 4.5f);
    assert(helper_order == 22 && out.unknown_w == 1.0f);
    assert(transform_in.z == 9.0f - 2.0f && cdd0_out == cdd0_v);
}

/* 0x205e70 branch coverage: n < 3 rejects with out zeroed; a triangle yields a unit normal. */
struct DotrViewVector data_00293d60 = {0.0f, 0.0f, 0.0f, 0.0f};
struct DotrViewVector data_00293d70 = {0.0f, 0.0f, 0.0f, 0.0f};

static void check_polygon_normal(void)
{
    struct DotrViewVector pts[3] = {{0, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 0, 1}};
    struct DotrViewVector out = {5, 5, 5, 5};
    assert(Ex3d_GetSurfaceNormal(2, pts, &out) == 0);
    assert(out.x == 0.0f && out.y == 0.0f && out.z == 0.0f && out.unknown_w == 0.0f);
    assert(Ex3d_GetSurfaceNormal(3, pts, &out) == 1);
    assert(out.x == 0.0f && out.y == 0.0f && out.z > 0.99f && out.z < 1.01f);
}

/* 0x2062f0 straight-line coverage: every output slot and the sqrt argument. */
static void check_plane(void)
{
    float a[3] = {0, 0, 0}, b[3] = {1, 0, 0}, c[3] = {0, 1, 0};
    float out[5] = {9, 9, 9, 9, 9};
    Ex3d_GetSurfaceC(a, b, c, out);
    assert(out[0] == 0.0f && out[1] == 0.0f && out[2] == 1.0f && out[3] == 0.0f);
    assert(root_argument == 1.0f && out[4] == func_001a6c60(1.0f));
}

/* 0x2070f0 branch coverage: null and non-null matrix, c < 0 skips Ex3d_Clipping (average NULL only),
 * average stored as sum of z / result, average NULL stores nothing. */
int gEx3d_aPlane[1];
static int baf0_arg, baf0_calls, bb50_calls, d30_calls, d30_args[7];
void RhGeom_PushMatrix(int unknown) { baf0_arg = unknown; baf0_calls++; }
void RhGeom_PopMatrix(void) { bb50_calls++; }
int Ex3d_Clipping(int a, int *b, int c, int d, int e, int f, int g)
{
    d30_args[0] = a; d30_args[1] = (int)(long)b; d30_args[2] = c; d30_args[3] = d;
    d30_args[4] = e; d30_args[5] = f; d30_args[6] = g;
    d30_calls++;
    return 4;
}

static void check_batch_transform(void)
{
    struct DotrViewMatrix m;
    struct DotrViewVector e[2 * 3];
    float average = -1.0f;
    int result;
    memset(e, 0, sizeof(e));
    stub_z = 6.0f;
    baf0_calls = d30_calls = bb50_calls = 0;
    result = Ex3d_NodeL2V(&m, 2, e, 7, 8, 9, &average);
    assert(baf0_arg == (int)(long)&m && baf0_calls == 1 && bb50_calls == 1 && d30_calls == 1);
    assert(d30_args[0] == 6 && d30_args[1] == (int)(long)gEx3d_aPlane && d30_args[2] == 2);
    assert(d30_args[3] == (int)(long)e && d30_args[4] == 7 && d30_args[5] == 8 && d30_args[6] == 9);
    assert(cdd0_v == &e[3] && e[0].unknown_w == stub_w && e[3].unknown_w == stub_w);
    assert(result == 4 && average == 12.0f / 4.0f);
    /* Null matrix: the 0x20eba0 copy (gRhView_avl[sel].unknown_040) is used; c < 0 skips 0x206d30. */
    d30_calls = 0;
    result = Ex3d_NodeL2V(NULL, 1, e, 1, 2, -1, NULL);
    assert(d30_calls == 0 && baf0_arg != (int)(long)&m && baf0_arg != 0);
    (void)result;
}

static int rot_phases[2], rot_calls;
static float rot_cos, rot_sin;
float RhMath_CosI(int phase) { rot_phases[0] = phase; rot_calls++; return rot_cos; }
float RhMath_SinI(int phase) { rot_phases[1] = phase; rot_calls++; return rot_sin; }

static void check_axis_rotation(void)
{
    /* Straight-line: one call per case, every stored element and both callee phases checked. */
    float m[16];
    struct DotrViewVector axis = {1.0f, 0.0f, 0.0f, 0.0f};
    int i;
    rot_cos = 1.0f; rot_sin = 0.0f; rot_calls = 0;
    RhGeom_GetRotVectorMatrix(m, &axis, 13);
    assert(rot_calls == 2 && rot_phases[0] == 13 && rot_phases[1] == 13);
    for (i = 0; i < 16; i++)
        assert(m[i] == ((i % 5) == 0 ? 1.0f : 0.0f));
    axis.x = 0.0f; axis.z = 1.0f;
    rot_cos = 0.0f; rot_sin = 1.0f;
    RhGeom_GetRotVectorMatrix(m, &axis, 2);
    assert(m[4] == -1.0f && m[1] == 1.0f && m[0] == 0.0f && m[5] == 0.0f && m[10] == 1.0f);
    assert(m[3] == 0.0f && m[7] == 0.0f && m[11] == 0.0f && m[12] == 0.0f && m[15] == 1.0f);
}

static void check_srt_matrix(void)
{
    /* Branch coverage: null optionals, pos only, rot only, scale only (identity stub for 0x20bbb0). */
    float m[16], scale[3] = {2.0f, 3.0f, 4.0f}, pos[3] = {5.0f, 6.0f, 7.0f};
    int rot[3] = {1, 2, 3}, i;
    matrix_calls = 0; rot_calls = 0;
    RhGeom_GetScaleRotXYZTransMatrix(m, NULL, NULL, NULL);
    assert(matrix_calls == 1 && matrix_seen == (struct DotrViewMatrix *)m && rot_calls == 0);
    for (i = 0; i < 16; i++)
        assert(m[i] == ((i % 5) == 0 ? 1.0f : 0.0f));
    RhGeom_GetScaleRotXYZTransMatrix(m, NULL, NULL, pos);
    assert(m[12] == 5.0f && m[13] == 6.0f && m[14] == 7.0f && m[15] == 1.0f && rot_calls == 0);
    rot_cos = 1.0f; rot_sin = 0.0f;
    RhGeom_GetScaleRotXYZTransMatrix(m, NULL, rot, NULL);
    assert(rot_calls == 6 && rot_phases[0] == 3 && rot_phases[1] == 3);
    assert(m[0] == 1.0f && m[5] == 1.0f && m[10] == 1.0f && m[2] == 0.0f && signbit(m[2]));
    assert(m[1] == 0.0f && m[4] == 0.0f && m[6] == 0.0f && m[8] == 0.0f && m[9] == 0.0f);
    RhGeom_GetScaleRotXYZTransMatrix(m, scale, NULL, NULL);
    assert(m[0] == 2.0f && m[5] == 3.0f && m[10] == 4.0f && m[1] == 0.0f && m[6] == 0.0f && m[9] == 0.0f);
    assert(m[3] == 0.0f && m[12] == 0.0f && m[15] == 1.0f);
}

static void check_projection(void)
{
    struct DotrViewMatrix m;
    int k;

    RhGeom_GetScreenMatrix(&m, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 1.0f, 9.0f, 2.0f, 4.0f);
    assert(mul_m_ptr == &m && mul_other_ptr != &m);
    for (k = 0; k < 16; k++) {
        float want = (k % 5 == 0) ? 1.0f : 0.0f;
        if (k == 0) want = 3.0f;
        if (k == 5) want = 4.0f;
        if (k == 10) want = 4.0f * 2.0f * (-1.0f + 9.0f) / (-2.0f + 4.0f);
        if (k == 12) want = 5.0f;
        if (k == 13) want = 6.0f;
        if (k == 14) want = (-9.0f * 2.0f + 1.0f * 4.0f) / (-2.0f + 4.0f);
        assert(mul_m[k] == want);
    }
    assert(mul_m[10] == 32.0f && mul_m[14] == -7.0f);
    for (k = 0; k < 16; k++) {
        float want = (k % 5 == 0) ? 1.0f : 0.0f;
        if (k == 0 || k == 5) want = 2.0f;
        if (k == 10 || k == 15) want = 0.0f;
        if (k == 11 || k == 14) want = 1.0f;
        assert(mul_other[k] == want);
    }
}

static void check_install_block(void)
{
    struct DotrViewBlock block;
    struct DotrViewEntry *e = &gRhView_avl[1];
    int k;

    memset(&block, 0x21, sizeof(block));
    gRhView_nStackIndex[0] = 1;
    e->unknown_160 = 3.0f;
    e->unknown_164 = 4.0f;
    e->unknown_168 = 6.0f;
    e->unknown_040.unknown_values[3] = 7.5f;
    memset(&InvertView, 0, sizeof(InvertView));
    install_count = 0;
    helper_order = 0;
    assert(RhView_Set(&block) == 1);
    assert(copy_args[0] == &e->unknown_0c0 && copy_args[1] == &block && copy_count == 9);
    assert(install_count == 5);
    for (k = 0; k < 5; k++)
        assert(install_order[k] == k + 1);
    assert(ba90_arg == &e->unknown_040 && bad0_arg == &block);
    assert(*(float *)&e->unknown_188[0] == 4186116.0f + 9.0f);
    assert(*(float *)&e->unknown_188[4] == 16.0f / 4.0f + 9.0f);
    assert(*(float *)&e->unknown_188[8] == 36.0f / 4.0f + 9.0f);
    assert(InvertView.unknown_values[3] == 7.5f);
    assert(bde0_out == &InvertView && bde0_in == &InvertView && helper_order == 1);
}

static void check_install_block_copy_back(void)
{
    struct DotrViewBlock block[2];
    struct DotrViewEntry *e = &gRhView_avl[1];
    static const int order[] = {1, 2, 4, 1};
    int k;

    gRhView_nStackIndex[0] = 1;
    e->unknown_160 = 3.0f;
    e->unknown_164 = 4.0f;
    e->unknown_168 = 6.0f;
    e->unknown_040.unknown_values[3] = 8.5f;
    memset(&InvertView, 0, sizeof(InvertView));
    install_count = 0;
    copy_calls = 0;
    helper_order = 0;
    assert(RhView_Modify(block) == 1);
    assert(copy_calls == 2 && install_count == 4);
    for (k = 0; k < 4; k++)
        assert(install_order[k] == order[k]);
    assert(copy_log[0][0] == &e->unknown_0c0 && copy_log[0][1] == block && copy_log_count[0] == 9);
    assert(copy_log[1][0] == block && copy_log[1][1] == &e->unknown_040 && copy_log_count[1] == 4);
    assert(ba90_arg == &e->unknown_040);
    assert(*(float *)&e->unknown_188[0] == 4186116.0f + 9.0f);
    assert(*(float *)&e->unknown_188[4] == 16.0f / 4.0f + 9.0f);
    assert(*(float *)&e->unknown_188[8] == 36.0f / 4.0f + 9.0f);
    assert(InvertView.unknown_values[3] == 8.5f);
    assert(bde0_out == &InvertView && bde0_in == &InvertView && helper_order == 1);
}

static void check_draw_chain(void)
{
    struct DotrViewEntry *e = &gRhView_avl[1];
    struct DotrDrawTag *result;

    gRhView_nStackIndex[0] = 1;
    e->unknown_160 = 1.5f; e->unknown_16c = 2.5f; e->unknown_170 = 3.5f;
    e->unknown_174 = 4.5f; e->unknown_178 = 5.5f; e->unknown_17c = 6.5f; e->unknown_180 = 7.5f;
    chain_n = 0;
    helper_order = 0;
    result = RhView_SendViewParameters((struct DotrDrawTag *)(intptr_t)0x77);
    assert(chain_n == 5 && result == (struct DotrDrawTag *)(intptr_t)0x5000);
    /* each call receives the tag returned by the previous one; the first gets the caller's */
    assert(chain_tag[0] == (struct DotrDrawTag *)(intptr_t)0x77 && chain_arg[0] == &e->unknown_080);
    assert(bde0_in == &e->unknown_000 && bde0_out == (struct DotrViewMatrix *)chain_arg[1]);
    assert(((intptr_t)bde0_out & 15) == 0 && chain_tag[1] == (struct DotrDrawTag *)(intptr_t)0x1000);
    assert(chain_mode == 1 && chain_f[2][0] == 1.5f && chain_f[2][1] == 2.5f && chain_f[2][2] == 3.5f);
    assert(chain_tag[2] == (struct DotrDrawTag *)(intptr_t)0x2000);
    assert(chain_arg[3] == (void *)&e->unknown_184 && chain_tag[3] == (struct DotrDrawTag *)(intptr_t)0x3000);
    assert(chain_f[4][0] == 4.5f && chain_f[4][1] == 5.5f && chain_f[4][2] == 6.5f && chain_f[4][3] == 7.5f);
    assert(chain_tag[4] == (struct DotrDrawTag *)(intptr_t)0x4000);
}

static void check_offset_ratio(void)
{
    struct DotrViewVector in = {1.0f, 2.0f, 9.0f, 0.5f};
    struct DotrViewEntry *e = &gRhView_avl[2];

    gRhView_nStackIndex[0] = 2;
    stub_z = 12.0f; stub_w = 4.0f;
    ratio_calls = 0; ratio_ret = 77.0f;
    assert(RhView_GetSampleZ(&in, 3.0f) == 77);
    assert(transform_in.x == 1.0f && transform_in.y == 2.0f && transform_in.z == 6.0f && transform_in.unknown_w == 0.5f);
    assert(cdd0_out == cdd0_v && cdd0_m == &e->unknown_080 && ((intptr_t)cdd0_v & 15) == 0 && cdd0_v != &in);
    assert(ratio_calls == 1 && ratio_seen == 3.0f);
    assert(in.z == 9.0f);
}

/* 0x209d60 stub for 0x20ddb0: stores two set ints. */
static int size_w, size_h, size_calls;
int RhBase_GetScreenSize(int *a, int *b)
{
    *a = size_w;
    *b = size_h;
    size_calls++;
    return 0;
}

/* RhView_Init (straight-line coverage: one call checking every stored value, the
 * forwarded frame and the answer; 0x20e1a0 is the real function over the
 * 0x20bbb0 stub). */
static void check_view_reset(void)
{
    static const float expect[20] = {0, 0, 0, 1, 0, 0, -1, 0, 0, 0, 0, 1, 0, 0, -1, 0, 0, 0, 0, 0};
    struct DotrViewEntry *v = &gRhView_avl[0];
    struct DotrViewFrame *frame = (struct DotrViewFrame *)&gRhView_avl[0].unknown_0c0;
    int i;

    memset(gRhView_avl, 0x55, sizeof(gRhView_avl));
    gRhView_nStackIndex[0] = 2;
    gRhView_FakeMirror[0].unknown_44 = 7;
    size_w = 640;
    size_h = 448;
    size_calls = 0;
    matrix_calls = 0;
    assert(RhView_Init() == 1 && size_calls == 1);
    assert(gRhView_nStackIndex[0] == 0 && gRhView_FakeMirror[0].unknown_44 == 0);
    assert(v->unknown_160 == 400.0f && v->unknown_164 == 640.0f && v->unknown_168 == 448.0f);
    assert(v->unknown_150 == 320.0f && v->unknown_154 == 224.0f);
    assert(v->unknown_158 == 1.0f && v->unknown_15c == 0.95f);
    assert(v->unknown_16c == 1.0f && v->unknown_170 == 10000.0f);
    assert(v->unknown_174 == 9000.0f && v->unknown_178 == 10000.0f && v->unknown_17c == 0.0f && v->unknown_180 == 1000.0f);
    assert(v->unknown_184.unknown_0 == 255 && v->unknown_184.unknown_1 == 255);
    assert(v->unknown_184.unknown_2 == 255 && v->unknown_184.unknown_3 == 255);
    assert(matrix_calls == 1 && matrix_seen == &frame->unknown_00);
    for (i = 0; i < 20; i++) assert(frame->unknown_40[i] == expect[i]);
}

/* Ex3d_NodeV2S (branch and loop coverage): a given matrix with count 0; a null
 * matrix with two vertices, checking every converted ratio and stored member. */
static void check_vertex_project(void)
{
    struct DotrViewMatrix matrix;
    struct DotrViewVertex vertices[3];
    int i;

    baf0_calls = bb50_calls = ratio_calls = 0;
    cdd0_out = 0;
    Ex3d_NodeV2S(&matrix, 0, vertices);
    assert(baf0_calls == 1 && baf0_arg == (int)(DotrViewWord)&matrix && bb50_calls == 1);
    assert(ratio_calls == 0 && cdd0_out == 0);

    memset(vertices, 0xff, sizeof(vertices));
    for (i = 0; i < 2; i++) {
        vertices[i].position.x = 2.0f + i;
        vertices[i].position.y = -6.0f;
    }
    stub_z = 10.0f;
    stub_w = 2.0f;
    ratio_ret = (float)0x234560;
    baf0_calls = bb50_calls = ratio_calls = 0;
    Ex3d_NodeV2S(0, 2, vertices);
    assert(baf0_calls == 1 && baf0_arg != (int)(DotrViewWord)&matrix && bb50_calls == 1 && ratio_calls == 6);
    assert(cdd0_out == &vertices[1].position && cdd0_v == cdd0_out && cdd0_m == 0 && transform_in.unknown_w == 1.0f);
    for (i = 0; i < 2; i++) {
        assert(ratio_log[3 * i] == 16.0f * (2.0f + i) * 0.5f && ratio_log[3 * i + 1] == -48.0f);
        assert(ratio_log[3 * i + 2] == 5.0f);
        assert(vertices[i].screen.x == 0x4560 && vertices[i].screen.y == 0x4560);
        assert(vertices[i].screen.z == 0x234560 && vertices[i].screen.f == 0);
        assert(vertices[i].unknown_18[0] == 0xff && vertices[i].unknown_18[23] == 0xff);
    }
    assert(vertices[2].screen.f == 0xff);
}

/* 0x206530 branch coverage: each flag bit clear and set; colour, q, float pair, 14-bit pair. */
static void check_divide(void)
{
    struct DotrViewDivideVertex a, b, o;
    int i;
    memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&o, 0x55, sizeof(o));
    for (i = 0; i < 4; i++) { a.pos[i] = (float)i; b.pos[i] = (float)(i + 4); }
    for (i = 0; i < 4; i++) { a.rgba[i] = (unsigned char)(200 + i); b.rgba[i] = (unsigned char)(100 + i); }
    a.q = 1.0f; b.q = 3.0f;
    a.t.st[0] = 2.0f; b.t.st[0] = 4.0f; a.t.st[1] = 8.0f; b.t.st[1] = 16.0f;
    ratio_ret = 7; ratio_calls = 0;
    Ex3d_DivideN(&a, &b, 0, &o, 0.5f);
    for (i = 0; i < 4; i++) assert(o.pos[i] == (float)i + 2.0f);
    assert(ratio_calls == 0 && o.rgba[0] == 0x55 && o.t.uv.u == 0x1555);
    Ex3d_DivideN(&a, &b, 1, &o, 0.5f);
    assert(ratio_calls == 4);
    for (i = 0; i < 4; i++) assert(o.rgba[i] == 7 && ratio_log[i] == (float)(150 + i));
    assert(o.q == 2.0f && o.t.uv.u == 0x1555);
    Ex3d_DivideN(&a, &b, 2, &o, 0.5f);
    assert(ratio_calls == 4 && o.t.st[0] == 3.0f && o.t.st[1] == 12.0f);
    memset(&a.t, 0, sizeof(a.t)); memset(&b.t, 0, sizeof(b.t)); memset(&o.t, 0xff, sizeof(o.t));
    a.t.uv.u = 0x3ffe; b.t.uv.u = 0x3ff0; a.t.uv.v = 16; b.t.uv.v = 48;
    ratio_calls = 0;
    Ex3d_DivideN(&a, &b, 0x10, &o, 0.5f);
    assert(ratio_calls == 2 && ratio_log[0] == -7.0f && ratio_log[1] == 16.0f);
    assert(o.t.uv.u == ((0x3ffe + 7) & 0x3fff) && o.t.uv.v == 23);
    assert((((unsigned short *)&o.t)[0] & 0xc000) == 0xc000 && (((unsigned short *)&o.t)[1] & 0xc000) == 0xc000);
}

struct DotrViewVector data_003269f0 __attribute__((aligned(16)));
struct DotrViewVector data_00326a00 __attribute__((aligned(16)));
struct DotrViewVector data_00326a10 __attribute__((aligned(16)));
static float *camrot_m;
static int camrot_ang[4];
void RhGeom_GetRotXYZMatrix(float *m, int *ang)
{
    int i;
    camrot_m = m;
    memcpy(camrot_ang, ang, 3 * sizeof(int));
    for (i = 0; i < 16; i++) m[i] = 100.0f + i;
}
/* Straight-line function: one call checks every stored value and forwarded argument. */
static void check_camera_matrix2(void)
{
    static float dir[4] __attribute__((aligned(16))) = {3.0f, 0.0f, 4.0f, 9.0f};
    float pos[3] = {11.0f, 12.0f, 13.0f};
    struct DotrViewMatrix out;
    static const float want[16] = {-0.8f, -0.0f, 0.6f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                                   0.6f, 0.0f, 0.8f, 0.0f, 11.0f, 12.0f, 13.0f, 1.0f};
    int i;
    memset(&out, 0x7f, sizeof(out));
    root_calls = 0;
    bde0_out = bde0_in = 0;
    mul_m_ptr = mul_other_ptr = 0;
    RhGeom_CreateCameraMatrix2(&out, pos, dir, 0x321);
    assert(root_calls == 3 && root_argument == 25.0f);
    assert(data_003269f0.x == 0.8f && data_003269f0.y == 0.0f && data_003269f0.z == -0.6f);
    assert(data_00326a00.x == 0.0f && data_00326a00.y == 1.0f && data_00326a00.z == 0.0f);
    assert(data_00326a10.x == 0.6f && data_00326a10.y == 0.0f && data_00326a10.z == 0.8f &&
           data_00326a10.unknown_w == 9.0f);
    for (i = 0; i < 16; i++) assert(out.unknown_values[i] == want[i]);
    assert(camrot_ang[0] == 0 && camrot_ang[1] == 0 && camrot_ang[2] == 0x321);
    assert(mul_m_ptr == &out && (float *)mul_other_ptr == camrot_m);
    for (i = 0; i < 16; i++) assert(mul_m[i] == want[i] && mul_other[i] == 100.0f + i);
    assert(bde0_out == &out && bde0_in == &out);
}

/* 0x20ede0 branch coverage: the stub leaves x and y of the output vector as set here and
 * supplies z. Entry: clip distances 1 and 1000, +0x160 = 1, scales 8/2 and 8/4, squared
 * normals 4, 64 and 16. The 18 cases reach both clip returns, both clip crossings and, for
 * each of the eight side planes, outside, crossing behind, crossing in front and clear. */
struct DotrViewVector data_0032a230;
static void check_view_volume(void)
{
    static const struct { float x, y, z, r; int expected; } cases[] = {
        { -2046.0f, -2050.0f, 1.0f, 0.25f, 4 },
        { 4092.0f, 2048.0f, 3.0f, 1.0f, 2 },
        { -6.0f, 4096.0f, 1004.0f, 1.0f, 4 },
        { 2046.0f, 0.0f, 1004.0f, 8.0f, 6 },
        { -2.0f, 2.0f, 3.0f, 0.25f, 3 },
        { 2.0f, 0.0f, 0.0f, 4.0f, 7 },
        { 2.0f, -4.0f, 0.0f, 4.0f, 7 },
        { 2.0f, -6000.0f, 999.0f, 0.5f, 2 },
        { 0.0f, -4096.0f, 1.0f, 2000.0f, 7 },
        { -6.0f, 2046.0f, -2.0f, 0.5f, 4 },
        { -6000.0f, -6.0f, 999.0f, 0.25f, 2 },
        { 2.0f, 6000.0f, -2.0f, 3.0f, 4 },
        { -2.0f, 6.0f, 1.0f, 2000.0f, 7 },
        { -6000.0f, -2.0f, 1.0f, 1.0f, 4 },
        { -2048.0f, -3.0f, 1.0f, 2.0f, 6 },
        { -2050.0f, 2.0f, 3.0f, 2000.0f, 7 },
        { 4092.0f, 1.0f, 1.0f, 1.0f, 4 },
        { 2048.0f, 2050.0f, 1.0f, 4.0f, 6 },
    };
    struct DotrViewEntry *e = &gRhView_avl[1];
    struct DotrViewVector centre;
    unsigned int i;
    memset(gRhView_avl, 0, sizeof(gRhView_avl));
    gRhView_nStackIndex[0] = 1;
    e->unknown_158 = 2.0f;
    e->unknown_15c = 4.0f;
    e->unknown_160 = 1.0f;
    e->unknown_164 = 8.0f;
    e->unknown_168 = 8.0f;
    e->unknown_16c = 1.0f;
    e->unknown_170 = 1000.0f;
    *(float *)&e->unknown_188[0] = 4.0f;
    *(float *)&e->unknown_188[4] = 16.0f;
    *(float *)&e->unknown_188[8] = 64.0f;
    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        memset(&centre, 0, sizeof(centre));
        data_0032a230.x = cases[i].x;
        data_0032a230.y = cases[i].y;
        stub_z = cases[i].z;
        cdd0_out = 0; cdd0_m = (struct DotrViewMatrix *)1; cdd0_v = 0;
        assert(RhView_CheckViewVolume(&centre, cases[i].r) == cases[i].expected);
        assert(centre.unknown_w == 1.0f && cdd0_out == &data_0032a230 && cdd0_m == 0 && cdd0_v == &centre);
    }
    gRhView_nStackIndex[0] = 0;
}

/* 0x20e520 branch coverage: mirror already active; pushed with room on the stack; pushed
 * with the stack full. NormalizeVector, GetVectorScalor, OuterProduct, GetWorldViewMatrix and
 * CreateCameraMatrix2 are the real functions; the inputs keep their arithmetic exact. */
static float a6c18_a, a6c18_b, a6c18_ret, a22c0_seen;
static int a22c0_ret, a6c18_calls;
float func_001a6c18(float a, float b) { a6c18_a = a; a6c18_b = b; a6c18_calls++; return a6c18_ret; }
int func_001a22c0(float value) { a22c0_seen = value; return a22c0_ret; }
static void check_fake_mirror(void)
{
    struct DotrViewMirror *mirror = (struct DotrViewMirror *)gRhView_FakeMirror;
    struct DotrViewFrame *frame = (struct DotrViewFrame *)&gRhView_avl[0].unknown_0c0;
    struct DotrViewVector pos = { 1.0f, 2.0f, 3.0f, 9.0f };
    struct DotrViewVector normal;
    struct DotrViewBlock out;
    struct DotrViewBlock before;
    struct DotrViewFrame *view = (struct DotrViewFrame *)&out;
    int depth;
    for (depth = 0; depth < 2; depth++) {
        unsigned int i;
        for (i = 0; i < sizeof(gRhView_avl); i++) {
            ((unsigned char *)gRhView_avl)[i] = (unsigned char)(i * 5 + i / 416);
        }
        memset(&gRhView_avl[0].unknown_000, 0, sizeof(struct DotrViewMatrix) * 3);
        memset(frame->unknown_40, 0, sizeof(frame->unknown_40));
        frame->unknown_40[0] = 1.0f; frame->unknown_40[1] = 7.0f; frame->unknown_40[2] = 3.0f;
        frame->unknown_40[6] = -1.0f;
        frame->unknown_40[9] = 21.0f;
        memset(mirror, 0, sizeof(*mirror));
        mirror->dir.unknown_w = 6.0f;
        normal.x = 0.0f; normal.y = 4.0f; normal.z = 0.0f; normal.unknown_w = 0.0f;
        memset(&out, 0x55, sizeof(out));
        memcpy(&before, &gRhView_avl[0].unknown_0c0, sizeof(before));
        gRhView_nStackIndex[0] = depth;
        stub_z = 0.0f; stub_w = 0.0f;
        a6c18_ret = 3.1415927f; a22c0_ret = 100; a6c18_calls = 0; bde0_calls = 0;
        assert(RhView_PushFakeMirror(&pos, &normal, &out) == 1);
        assert(mirror->active == 1 && mirror->dist == 5.0f);
        assert(mirror->pos.x == 1.0f && mirror->pos.y == 2.0f && mirror->pos.z == 3.0f && mirror->pos.unknown_w == 9.0f);
        assert(mirror->normal.x == 0.0f && mirror->normal.y == 1.0f && mirror->normal.z == 0.0f);
        assert(mirror->dir.x == 0.0f && mirror->dir.y == 0.0f && mirror->dir.z == -1.0f);
        assert(mirror->eye.x == 0.0f && mirror->eye.y == 1.0f && mirror->eye.z == 0.0f);
        /* The view block: matrix and later words copied, position mirrored to y = -3. */
        assert(memcmp(&out, &before, 64) == 0 && view->unknown_40[9] == 21.0f);
        assert(view->unknown_40[0] == 1.0f && view->unknown_40[1] == -3.0f && view->unknown_40[2] == 3.0f);
        assert(view->unknown_40[3] == 0.0f);
        assert(view->unknown_40[4] == 0.0f && view->unknown_40[5] == 0.0f && view->unknown_40[6] == -1.0f);
        assert(view->unknown_40[7] == 6.0f);
        /* Helpers: the inverse goes 64 bytes after the matrix, the matrix 64 bytes after the
         * second vector; that vector is up x direction before its transform. */
        assert(bde0_calls == 2 && (char *)bde0_log[0][0] - (char *)bde0_log[0][1] == 64);
        assert(bde0_log[1][0] == bde0_log[0][1] && bde0_log[1][1] == bde0_log[0][1] && cdd0_m == bde0_in);
        assert(cdd0_out == cdd0_v && (char *)cdd0_m - (char *)cdd0_out == 64 && ((intptr_t)cdd0_out & 15) == 0);
        assert(transform_in.x == -1.0f && transform_in.y == 0.0f && transform_in.z == 0.0f);
        assert(a6c18_calls == 1 && a6c18_a == 0.0f && a6c18_b == -1.0f && a22c0_seen == 32768.0f);
        assert(*(int *)&view->unknown_40[16] == 100 + 0x8000);
        if (depth == 0) {
            assert(gRhView_nStackIndex[0] == 1 && memcmp(&gRhView_avl[1], &gRhView_avl[0], sizeof(gRhView_avl[0])) == 0);
        } else {
            assert(gRhView_nStackIndex[0] == 1 && ((unsigned char *)&gRhView_avl[2])[5] == (unsigned char)((832 + 5) * 5 + 2));
            assert(memcmp(&gRhView_avl[0].unknown_0c0, &before, sizeof(before)) == 0);
        }
        /* Active: nothing happens. */
        memset(&out, 0x55, sizeof(out));
        a6c18_calls = 0;
        assert(RhView_PushFakeMirror(&pos, &normal, &out) == 0 && a6c18_calls == 0);
        assert(((unsigned char *)&out)[0] == 0x55 && ((unsigned char *)&out)[143] == 0x55);
        assert(gRhView_nStackIndex[0] == 1 && normal.y == 4.0f);
    }
    gRhView_nStackIndex[0] = 0;
    mirror->active = 0;
}

int main(void)
{
    check_vertex_project();
    check_view_reset();
    check_frame_defaults();
    check_point_transform();
    check_depth_ratio();
    check_depth_w();
    check_polygon_normal();
    check_plane();
    check_batch_transform();
    check_axis_rotation();
    check_srt_matrix();
    check_projection();
    check_install_block();
    check_install_block_copy_back();
    check_draw_chain();
    check_offset_ratio();
    check_vectors();
    check_means();
    check_translation();
    check_entries();
    check_stack();
    check_divide();
    check_camera_matrix2();
    check_view_volume();
    check_fake_mirror();
    puts("view_state: ok");
    return 0;
}
