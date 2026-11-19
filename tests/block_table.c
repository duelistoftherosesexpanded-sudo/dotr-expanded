/* Synthetic tests of the block, slot and entry helpers. All storage is host
 * fixtures; no game data is used. */
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/block_table.h"

struct DotrBlock *gSzView_hView_blocks[1];
struct DotrBlockSlot gSzView_CameraReq[8];
struct DotrBlockEntry gSzFade_fl[2];
struct DotrBlockDelta *gSzView_hView_delta[1];

static struct DotrBlockVector4 *normalized;
static int normalize_calls;

void RhGeom_NormalizeVector(struct DotrBlockVector4 *v)
{
    normalized = v;
    normalize_calls++;
}

static struct DotrBlock block, expected;
static const int bad_index[] = {INT_MIN, -1, 1, INT_MAX};
static const int bad_position[] = {INT_MIN, -1, 10, INT_MAX};

static void fill(int value)
{
    memset(&block, value, sizeof(block));
    expected = block;
    gSzView_hView_blocks[0] = &block;
}

static int same(void)
{
    return memcmp(&block, &expected, sizeof(expected)) == 0;
}

static void check_layout(void)
{
    assert(offsetof(struct DotrBlock, modelViewData) == 0x60 && offsetof(struct DotrBlock, unknown_600) == 0x600);
    assert(offsetof(struct DotrBlock, unknown_7e0) == 0x7e0 && offsetof(struct DotrBlock, unknown_830) == 0x830);
    assert(offsetof(struct DotrBlock, fogColour) == 0x880 && offsetof(struct DotrBlock, unknown_8a8) == 0x8a8);
    assert(offsetof(struct DotrBlock, viewMode) == 0x8b0 && offsetof(struct DotrBlock, unknown_040) == 0x40);
}

static void check_rejections(void)
{
    struct DotrBlockQuad quad = {{1, 2, 3, 4}};
    struct DotrBlockPair pair = {1.0f, 2.0f};
    struct DotrBlockVector4 vector = {1.0f, 2.0f, 3.0f, 4.0f};
    union DotrBlockMatrix48 matrix;
    size_t i;
    int pass;
    memset(&matrix, 0, sizeof(matrix));
    for (pass = 0; pass < 2; pass++) {
        fill(0x5a);
        if (pass) gSzView_hView_blocks[0] = 0;
        for (i = 0; i < sizeof(bad_index) / sizeof(bad_index[0]); i++) {
            int index = pass ? 0 : bad_index[i];
            assert(SzView_SetFogColor(index, 0, &quad) == 0 && SzView_SetFogParamZ(index, 0, &pair) == 0);
            assert(SzView_SetFogParamY(index, 0, &pair) == 0 && SzView_SetStraightLightColor(index, 0, 0, &vector) == 0);
            assert(SzView_GetStraightLightVector(index, 0, 0, &vector) == 0 && SzView_InqStraightLight(index, 0) == 0);
            assert(SzView_InqPointLight(index, 0) == 0 && SzView_SetPointLight(index, 0, &matrix) == 0);
            assert(SzView_GetScreenDistance(index) == 0.0f && SzView_SetScreenDistance(index, 1.0f) == 0 && SzView_GetCP(index) == 0);
            assert(SzView_GetRel(index) == 0);
            assert(SzView_SetRoll(index, 1) == 0 && SzView_GetLook(index) == 0 && SzView_IsFogZ(index, 0) == 0);
            assert(SzView_IsFogY(index, 0) == 0 && SzView_GetMode(index) == -1 && SzView_SetMode(index, 1) == 0);
        }
        assert(same());
    }
    fill(0x5a);
    for (i = 0; i < sizeof(bad_position) / sizeof(bad_position[0]); i++) {
        int k = bad_position[i];
        assert(SzView_SetFogColor(0, k, &quad) == 0 && SzView_SetFogParamZ(0, k, &pair) == 0 && SzView_SetFogParamY(0, k, &pair) == 0);
        assert(SzView_SetStraightLightColor(0, k, 0, &vector) == 0 && SzView_GetStraightLightVector(0, k, 0, &vector) == 0);
        assert(SzView_InqStraightLight(0, k) == 0 && SzView_InqPointLight(0, k) == 0 && SzView_SetPointLight(0, k, &matrix) == 0);
        assert(SzView_IsFogZ(0, k) == 0 && SzView_IsFogY(0, k) == 0);
    }
    assert(SzView_SetStraightLightColor(0, 0, -1, &vector) == 0 && SzView_SetStraightLightColor(0, 0, 3, &vector) == 0);
    assert(SzView_GetStraightLightVector(0, 0, -1, &vector) == 0 && SzView_GetStraightLightVector(0, 0, 3, &vector) == 0 && same());
    assert(vector.x == 1.0f && vector.unknown_w == 4.0f);
}

static void check_block(void)
{
    struct DotrBlockQuad quad = {{1, 2, 3, 4}};
    struct DotrBlockPair pair = {7.0f, 9.0f};
    struct DotrBlockVector4 vector = {1.0f, 2.0f, 3.0f, 4.0f}, out;
    union DotrBlockMatrix48 matrix;
    int k, part, second;

    fill(0);
    assert(SzView_GetLook(0) == &block && SzView_GetCP(0) == (void *)block.cameraPoint);
    assert(SzView_GetRel(0) == (void *)(block.unknown_000 + 0x10));
    assert(SzView_SetScreenDistance(0, 2.5f) == 1 && SzView_GetScreenDistance(0) == 2.5f);
    assert(SzView_SetRoll(0, -77) == 1 && SzView_SetMode(0, 0x1234) == 1 && SzView_GetMode(0) == 0x1234);
    expected.screenDistance = 2.5f;
    expected.unknown_040 = -77;
    expected.viewMode = 0x1234;
    assert(same());

    for (k = 0; k < 10; k++) {
        /* Quads: a null source stores 255 in three bytes only. */
        fill(0x11);
        assert(SzView_SetFogColor(0, k, &quad) == 1);
        expected.fogColour[k] = quad;
        assert(same() && SzView_SetFogColor(0, k, 0) == 1);
        expected.fogColour[k].unknown_bytes[0] = 255;
        expected.fogColour[k].unknown_bytes[1] = 255;
        expected.fogColour[k].unknown_bytes[2] = 255;
        assert(same());

        /* Two pair tables with their own bit halfwords. Source 1 sets the
         * bit without copying; a null source stores (2000, 4000). */
        for (second = 0; second < 2; second++) {
            struct DotrBlockPair *want = second ? &expected.unknown_830[k] : &expected.unknown_7e0[k];
            unsigned short *bits = second ? &expected.unknown_8ae : &expected.unknown_8ac;
            int (*set)(int, int, struct DotrBlockPair *) = second ? SzView_SetFogParamY : SzView_SetFogParamZ;
            int (*test)(int, int) = second ? SzView_IsFogY : SzView_IsFogZ;
            fill(0);
            assert(test(0, k) == 0 && set(0, k, &pair) == 1 && test(0, k) == 1);
            *want = pair;
            *bits = (unsigned short)(1 << k);
            assert(same() && set(0, k, (struct DotrBlockPair *)1) == 1 && same());
            assert(set(0, k, 0) == 1);
            want->x = 2000.0f;
            want->y = 4000.0f;
            *bits = 0;
            assert(same() && test(0, k) == 0);
            /* Bit 9 answers for every position. */
            if (second) block.unknown_8ae = 0x200; else block.unknown_8ac = 0x200;
            assert(test(0, k) == 1);
        }

        /* Element vectors. */
        for (part = 0; part < 3; part++) {
            fill(0);
            assert(SzView_SetStraightLightColor(0, k, part, &vector) == 1);
            expected.modelViewData[k].unknown_00[part].unknown_00[0] = vector;
            assert(same() && SzView_SetStraightLightColor(0, k, part, 0) == 1);
            expected.modelViewData[k].unknown_00[part].unknown_00[0].y = part ? 0.0f : 0.5f;
            expected.modelViewData[k].unknown_00[part].unknown_00[0].z = part ? 0.0f : 0.5f;
            expected.modelViewData[k].unknown_00[part].unknown_00[0].unknown_w = part ? 0.0f : 0.5f;
            assert(same());
            block.modelViewData[k].unknown_00[part].unknown_00[2] = vector;
            memset(&out, 0, sizeof(out));
            assert(SzView_GetStraightLightVector(0, k, part, 0) == 1 && SzView_GetStraightLightVector(0, k, part, &out) == 1);
            assert(memcmp(&out, &vector, sizeof(out)) == 0);
        }

        /* 48-byte copies with the second vector's last float forced to 1. */
        fill(0);
        memset(&matrix, 0x3c, sizeof(matrix));
        assert(SzView_InqPointLight(0, k) == 0 && SzView_SetPointLight(0, k, &matrix) == 1 && SzView_InqPointLight(0, k) == 1);
        expected.unknown_600[k] = matrix;
        expected.unknown_600[k].unknown_vectors[1].unknown_w = 1.0f;
        expected.lightingFlags = (unsigned short)(1 << k);
        assert(same() && SzView_SetPointLight(0, k, (union DotrBlockMatrix48 *)1) == 1 && same());
        assert(SzView_SetPointLight(0, k, 0) == 1);
        expected.lightingFlags = 0;
        assert(same());
        block.unknown_8a8 = (unsigned short)(1 << k);
        assert(SzView_InqStraightLight(0, k) == 1 && SzView_InqStraightLight(0, (k + 1) % 10) == 0);
    }
}

static void check_slots(void)
{
    struct DotrBlockSlot want[8];
    int i;
    memset(gSzView_CameraReq, 0, sizeof(gSzView_CameraReq));
    assert(SzView_InqRequest(-1, -1) == 0 && SzView_PauseRequest(-1, -1, 1) == 0 && SzView_FreeRequest(-1, -1) == 0);
    for (i = 1; i < 8; i++) {
        gSzView_CameraReq[i].unknown_00.unknown_bit_0 = i != 4 ? -1 : 0;
        gSzView_CameraReq[i].unknown_02 = (signed char)(i & 1);
        gSzView_CameraReq[i].unknown_03 = (signed char)(i / 2);
    }
    memcpy(want, gSzView_CameraReq, sizeof(want));
    assert(SzView_InqRequest(-1, -1) == &gSzView_CameraReq[1] && SzView_InqRequest(0, -1) == &gSzView_CameraReq[2]);
    assert(SzView_InqRequest(-1, 3) == &gSzView_CameraReq[6] && SzView_InqRequest(1, 3) == &gSzView_CameraReq[7]);
    assert(SzView_InqRequest(0, 2) == 0 && SzView_InqRequest(5, -1) == 0);
    assert(memcmp(want, gSzView_CameraReq, sizeof(want)) == 0);
    assert(SzView_PauseRequest(1, -1, 0x100) == 1);
    for (i = 1; i < 8; i += 2) {
        want[i].unknown_01.unknown_bit_0 = -1;
    }
    assert(memcmp(want, gSzView_CameraReq, sizeof(want)) == 0);
    assert(SzView_PauseRequest(-1, 1, 0) == 1);
    want[3].unknown_01.unknown_bit_0 = 0;
    assert(memcmp(want, gSzView_CameraReq, sizeof(want)) == 0 && SzView_PauseRequest(0, 2, 1) == 0);
    assert(SzView_FreeRequest(0, -1) == 1);
    want[2].unknown_00.unknown_bit_0 = 0;
    want[6].unknown_00.unknown_bit_0 = 0;
    assert(memcmp(want, gSzView_CameraReq, sizeof(want)) == 0 && SzView_FreeRequest(0, -1) == 0);
}

static void check_entries(void)
{
    struct DotrBlockQuad quad = {{9, 8, 7, 6}};
    struct DotrBlockEntry want[2];
    int i;
    memset(gSzFade_fl, 0, sizeof(gSzFade_fl));
    SzFade_Init();
    memset(want, 0, sizeof(want));
    want[0].unknown_07.unknown_bit_6 = -1;
    want[1].unknown_07.unknown_bit_6 = -1;
    assert(memcmp(want, gSzFade_fl, sizeof(want)) == 0);
    /* Vacant entries (bit 6) reject the per-entry routines. */
    assert(SzFade_SetKillFlag(0) == 0 && SzFade_SetPauseFlag(0, 1) == 0 && SzFade_GetRestCount(0) == -1);
    assert(SzFade_SetKillFlag(-1) == 0 && SzFade_SetKillFlag(2) == 0 && SzFade_SetPauseFlag(2, 1) == 0 && SzFade_GetRestCount(2) == -1);
    assert(SzFade_IsDoing(0) == 0 && SzFade_IsDoing(1) == 0);
    for (i = 0; i < 2; i++) {
        memset(gSzFade_fl, 0xff, sizeof(gSzFade_fl));
        memcpy(want, gSzFade_fl, sizeof(want));
        assert(SzFade_Entry(i, 0x45, 0x101, 3, 10, i ? &quad : 0) == 1);
        want[i].unknown_07.unknown_bits_0_5 = 5;
        want[i].unknown_04 = 1;
        want[i].unknown_05 = 3;
        want[i].unknown_06 = 10;
        want[i].unknown_07.unknown_bit_6 = 0;
        want[i].unknown_07.unknown_bit_7 = 0;
        want[i].unknown_08.unknown_bit_0 = 0;
        if (i) {
            want[i].unknown_00 = quad;
        } else {
            memset(&want[i].unknown_00, 0, 3);
            want[i].unknown_00.unknown_bytes[3] = 128;
        }
        assert(memcmp(want, gSzFade_fl, sizeof(want)) == 0);
        /* An occupied entry is not replaced. */
        assert(SzFade_Entry(i, 1, 2, 3, 4, 0) == 0 && memcmp(want, gSzFade_fl, sizeof(want)) == 0);
        assert(SzFade_GetRestCount(i) == 7 && SzFade_IsDoing(8) == 1 && SzFade_IsDoing(0) == 1);
        assert(SzFade_SetKillFlag(i) == 1 && SzFade_SetPauseFlag(i, 0x101) == 1);
        want[i].unknown_07.unknown_bit_7 = -1;
        want[i].unknown_08.unknown_bit_0 = -1;
        assert(memcmp(want, gSzFade_fl, sizeof(want)) == 0);
        gSzFade_fl[i].unknown_06 = 3;
        /* Zero pending: only a zero argument still reports 1. */
        assert(SzFade_GetRestCount(i) == 0 && SzFade_IsDoing(0) == 1 && SzFade_IsDoing(5) == 0);
    }
}

static void check_convert(void)
{
    int in[4] = {-3, 32768, -16384, 7};
    float out[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    SzView_IV2V(in, out);
    assert(out[0] == -3.0f && out[3] == 7.0f);
    assert(out[1] == 3.1415927f && out[2] == 3.1415927f * -16384.0f / 32768.0f);
}

/* 0x1f90e0: branch coverage. */
static void check_delta(void)
{
    struct DotrBlockDelta delta;
    static const int rejected[] = {INT_MIN, -1, 1, INT_MAX};
    int i;

    memset(&delta, 0, sizeof(delta));
    delta.relativeDirection.x = delta.relativeDirection.y = delta.relativeDirection.z = 99.0f;
    delta.relativeDirection.unknown_w = 99.0f;
    delta.sourcePoint.x = 1.0f;
    delta.sourcePoint.y = 2.0f;
    delta.sourcePoint.z = 3.0f;
    delta.cameraPoint.x = 5.0f;
    delta.cameraPoint.y = 4.5f;
    delta.cameraPoint.z = -1.0f;
    gSzView_hView_delta[0] = &delta;
    normalize_calls = 0;
    assert(lSzView_CalcRel(0) == 1 && normalize_calls == 1 && normalized == &delta.relativeDirection);
    assert(delta.relativeDirection.x == 4.0f && delta.relativeDirection.y == 2.5f && delta.relativeDirection.z == -4.0f);
    assert(delta.relativeDirection.unknown_w == 0.0f);
    for (i = 0; i < 4; i++) assert(lSzView_CalcRel(rejected[i]) == 0);
    assert(normalize_calls == 1);
    gSzView_hView_delta[0] = 0;
    assert(lSzView_CalcRel(0) == 0 && normalize_calls == 1);
}

static int polar_calls;

float func_001a6c60(float value)
{
    polar_calls++;
    return value * 0.5f;
}

float func_001a6c18(float a, float b)
{
    polar_calls++;
    return a * 4.0f + b;
}

float func_001a6c30(float a, float b)
{
    polar_calls++;
    assert(b == 6.2831855f);
    return a - b;
}

static void check_polar(void)
{
    struct DotrBlockDelta delta;
    struct DotrBlockVector4 *out = (struct DotrBlockVector4 *)delta.unknown_00;
    static const int rejected[] = {INT_MIN, -1, 1, INT_MAX};
    float angle;
    int i;

    memset(&delta, 0, sizeof(delta));
    delta.sourcePoint.x = 3.0f;
    delta.sourcePoint.y = 2.0f;
    delta.sourcePoint.z = 5.0f;
    delta.cameraPoint.x = 1.0f;
    delta.cameraPoint.y = 5.0f;
    delta.cameraPoint.z = 1.0f;
    gSzView_hView_delta[0] = &delta;
    polar_calls = 0;
    assert(lSzView_CalcLook(0) == 1 && polar_calls == 5);
    /* differences: x 2, y -3, z 4 */
    angle = 6.2831855f + (4.0f * 4.0f + 2.0f);
    assert(out->x == (4.0f + 9.0f + 16.0f) * 0.5f);
    assert(out->y == angle - 6.2831855f);
    assert(out->z == -3.0f * 4.0f + (16.0f + 4.0f) * 0.5f);
    for (i = 0; i < 4; i++) assert(lSzView_CalcLook(rejected[i]) == 0);
    gSzView_hView_delta[0] = 0;
    assert(lSzView_CalcLook(0) == 0 && polar_calls == 5);
}

static int sin_calls, cos_calls;

float RhMath_Sin(float rad)
{
    sin_calls++;
    return rad * 2.0f;
}

float RhMath_Cos(float rad)
{
    cos_calls++;
    return rad + 1.0f;
}

/* One pass of the roll as the stubs above compute it. */
static void roll_expect(float *polar, struct DotrBlockVector4 *src, struct DotrBlockVector4 *dst, float yaw,
                        float pitch, float dist, int dir)
{
    float xz, y, z;

    polar[0] = polar[0] + dist;
    polar[1] = (6.2831855f + (polar[1] + yaw)) - 6.2831855f;
    polar[2] = (polar[2] + pitch) - 6.2831855f;
    xz = polar[0] * (polar[2] + 1.0f);
    y = polar[0] * (polar[2] * 2.0f);
    z = xz * (polar[1] * 2.0f);
    xz = xz * (polar[1] + 1.0f);
    dst->x = src->x + xz * dir;
    dst->y = src->y + y * dir;
    dst->z = src->z + z * dir;
}

static void check_roll_camera(void)
{
    static union {
        struct DotrBlockDelta delta;
        unsigned char bytes[0x8b4];
        int align;
    } object, want;
    static const int rejected[] = {INT_MIN, -1, 1, INT_MAX};
    struct DotrBlockDelta *delta = &object.delta;
    float *polar = (float *)delta->unknown_00;
    float *wpolar = (float *)want.delta.unknown_00;
    int *mode = (int *)(object.bytes + 0x8b0);
    int i;

    for (i = 0; i < 4; i++) assert(SzView_RollCamera(rejected[i], 0, 1, 0.0f, 0.0f, 0.0f) == 0);
    gSzView_hView_delta[0] = 0;
    assert(SzView_RollCamera(0, 0, 1, 0.0f, 0.0f, 0.0f) == 0);
    gSzView_hView_delta[0] = delta;

    /* Mode 1: no limits. A positive direction moves +0x20 from +0x30, any other direction the reverse. */
    for (i = 0; i < 3; i++) {
        static const int dirs[] = {1, 0, -2};
        int dir = dirs[i];
        memset(&object, 0, sizeof(object));
        *mode = 1;
        polar[0] = 2.0f; polar[1] = 0.5f; polar[2] = 3.0f;
        delta->sourcePoint.x = 1.0f; delta->sourcePoint.y = 2.0f; delta->sourcePoint.z = 3.0f;
        delta->cameraPoint.x = 8.0f; delta->cameraPoint.y = 16.0f; delta->cameraPoint.z = 32.0f;
        delta->relativeDirection.unknown_w = 9.0f;
        want = object;
        if (dir > 0) roll_expect(wpolar, &want.delta.cameraPoint, &want.delta.sourcePoint, 0.25f, 4.0f, 1.0f, dir);
        else roll_expect(wpolar, &want.delta.sourcePoint, &want.delta.cameraPoint, 0.25f, 4.0f, 1.0f, dir);
        polar_calls = sin_calls = cos_calls = normalize_calls = 0;
        assert(SzView_RollCamera(0, 0, dir, 0.25f, 4.0f, 1.0f) == 1);
        assert(polar_calls == 2 && sin_calls == 2 && cos_calls == 2 && normalize_calls == 1);
        assert(normalized == &delta->relativeDirection);
        assert(polar[0] == 3.0f && polar[1] == wpolar[1] && polar[2] == wpolar[2]);
        assert(memcmp(&delta->sourcePoint, &want.delta.sourcePoint, 12) == 0);
        assert(memcmp(&delta->cameraPoint, &want.delta.cameraPoint, 12) == 0);
        assert(delta->relativeDirection.x == delta->cameraPoint.x - delta->sourcePoint.x);
        assert(delta->relativeDirection.y == delta->cameraPoint.y - delta->sourcePoint.y);
        assert(delta->relativeDirection.z == delta->cameraPoint.z - delta->sourcePoint.z);
        assert(delta->relativeDirection.unknown_w == 0.0f);
    }

    /* Modes 0 and 2: a distance below the limit (50 / 10) is first made up by a nested pass. */
    for (i = 0; i < 6; i++) {
        static const struct { int mode, nested; float start, dist, end; int passes; } c[] = {
            {0, 0, 10.0f, 5.0f, 50.0f, 2}, {0, 0, 45.0f, 5.0f, 50.0f, 1}, {0, 1, 10.0f, 5.0f, 15.0f, 1},
            {2, 0, 2.0f, 3.0f, 10.0f, 2},  {2, 0, 7.0f, 3.0f, 10.0f, 1},  {2, 0, 10.0f, 5.0f, 15.0f, 1},
        };
        memset(&object, 0, sizeof(object));
        *mode = c[i].mode;
        polar[0] = c[i].start;
        polar_calls = sin_calls = cos_calls = normalize_calls = 0;
        assert(SzView_RollCamera(0, c[i].nested, 1, 0.0f, 0.0f, c[i].dist) == 1);
        assert(polar[0] == c[i].end);
        assert(polar_calls == 2 * c[i].passes && sin_calls == 2 * c[i].passes);
        assert(cos_calls == 2 * c[i].passes && normalize_calls == c[i].passes);
    }

    /* Modes 0 and 2 keep pitch within +-1.5706217; mode 1 does not. */
    for (i = 0; i < 5; i++) {
        static const struct { int mode; float start, pitch, sum; } c[] = {
            {0, 1.0f, 1.0f, 1.5706217f}, {2, -1.0f, -1.0f, -1.5706217f}, {0, 0.5f, 0.25f, 0.75f},
            {0, -0.5f, -0.25f, -0.75f},  {1, 1.0f, 1.0f, 2.0f},
        };
        float sum = c[i].sum;
        if (sum == 1.5706217f) sum = c[i].start + (1.5706217f - c[i].start);
        if (sum == -1.5706217f) sum = c[i].start + (-1.5706217f - c[i].start);
        memset(&object, 0, sizeof(object));
        *mode = c[i].mode;
        polar[0] = 60.0f;
        polar[2] = c[i].start;
        assert(SzView_RollCamera(0, 0, 1, 0.0f, c[i].pitch, 0.0f) == 1);
        assert(polar[2] == sum - 6.2831855f);
    }
    gSzView_hView_delta[0] = 0;
}

static void check_slot_set_reset(void)
{
    static const float first[4] = {1.5f, -2.5f, 3.5f, -4.5f}, second[4] = {10.0f, 20.0f, 30.0f, 40.0f};
    DotrSlotSet set;
    int i;

    assert(sizeof(DotrSlotSet) == 464 && offsetof(DotrSlotSet, ids) == 352 && offsetof(DotrSlotSet, h418) == 418);
    assert(offsetof(DotrSlotSet, v0) == 432 && offsetof(DotrSlotSet, v1) == 448);
    /* block 0: vector 0 is read at +0x30 and vector 1 at the block start */
    fill(0);
    memcpy((unsigned char *)&block + 0x30, first, sizeof(first));
    memcpy(&block, second, sizeof(second));
    expected = block;
    memset(&set, 0x5a, sizeof(set));
    TaTuto_InitTutoData(&set);
    for (i = 0; i < 22; i++) {
        assert(set.entries[i].a == 0xffff && set.entries[i].b == 255 && set.entries[i].c == 0xffff);
        assert(set.entries[i].d == -1 && set.entries[i].kind == 0);
    }
    assert(set.ids[0] == 0xffff && set.ids[1] == 0);
    for (i = 2; i < 32; i++)
        assert(set.ids[i] == 0xffff);
    assert(set.mode == 0 && set.h418 == 0 && set.h420 == 0);
    assert(memcmp(&set.v0, first, sizeof(first)) == 0 && memcmp(&set.v1, second, sizeof(second)) == 0);
    assert(same());
    /* null set: nothing to write and no crash */
    TaTuto_InitTutoData(0);
    assert(same());
}

static int kind_log[4][4], kind_count;
static void kind_note(int id, int a, int b, int c)
{
    int v[4] = {id, a, b, c};
    memcpy(kind_log[kind_count++ & 3], v, sizeof(v));
}
void TaTuto_Init032(void) { kind_note(1, 0, 0, 0); }
void TaTuto_Init036(void) { kind_note(2, 0, 0, 0); }
int SzDuel_CalcSummonPower(int index, int operation, int value) { kind_note(3, index, operation, value); return 0; }

/* TaTuto_InitTutorial (branch coverage): kinds 32, 35, 36 and their neighbours; the
 * set is reset first (checked once with a real set, otherwise null). */
static void check_kind_dispatch(void)
{
    static const int quiet[] = {0, 31, 33, 34, 37, 255};
    static const int pair[2][4] = {{3, 0, 4, 8}, {3, 1, 4, 8}};
    DotrSlotSet set;
    size_t i;
    memset(&set, 0x5a, sizeof(set));
    kind_count = 0;
    TaTuto_InitTutorial(32, &set);
    assert(kind_count == 1 && kind_log[0][0] == 1 && set.entries[0].a == 0xffff && set.mode == 0);
    kind_count = 0;
    TaTuto_InitTutorial(36, 0);
    assert(kind_count == 1 && kind_log[0][0] == 2);
    kind_count = 0;
    TaTuto_InitTutorial(35, 0);
    assert(kind_count == 2 && memcmp(kind_log, pair, sizeof(pair)) == 0);
    kind_count = 0;
    for (i = 0; i < sizeof(quiet) / sizeof(quiet[0]); i++) {
        TaTuto_InitTutorial((unsigned char)quiet[i], 0);
    }
    assert(kind_count == 0);
}

static int gate_result, gate_arg, kind_result, kind_arg, rank_args[2], rank_calls;
int TaTuto_IsTuto(int value) { gate_arg = value; return gate_result; }
int SzDuel_GetPlayer(int index) { kind_arg = index; return kind_result; }
int SzDuel_GetUnitSummonOrder(int a, int b) { rank_calls++; rank_args[0] = a; rank_args[1] = b; return 0x1234; }

/* TaLogic_InitLogicData (branch and loop coverage): gate 1 forwards the low byte of the kind (0x124
 * dispatches as 36) and the set; gate 0 and 2 with a null set return -1; with a set
 * they empty all 22 entries and 32 ids and write the first entry. */
static void check_side_set(void)
{
    DotrSlotSet set;
    int i, g;

    kind_count = 0;
    gate_result = 1;
    kind_result = 0x124;
    rank_calls = 0;
    assert(TaLogic_InitLogicData(5, 0) == 1 && gate_arg == 5 && kind_arg == 5);
    assert(kind_count == 1 && kind_log[0][0] == 2 && rank_calls == 0);
    for (g = 0; g <= 2; g += 2) {
        gate_result = g;
        kind_count = 0;
        assert(TaLogic_InitLogicData(3, 0) == -1 && kind_count == 0 && rank_calls == 0);
        memset(&set, 0x5a, sizeof(set));
        assert(TaLogic_InitLogicData(3, &set) == 1 && rank_calls == 1 && rank_args[0] == 3 && rank_args[1] == 0);
        rank_calls = 0;
        assert(set.entries[0].a == 0x300 && set.entries[0].b == 0x1234 && set.entries[0].c == 0xffff);
        for (i = 1; i < 22; i++) {
            assert(set.entries[i].a == 0xffff && set.entries[i].b == 255 && set.entries[i].c == 0xffff);
        }
        for (i = 0; i < 22; i++) {
            assert(set.entries[i].d == -1 && set.entries[i].kind == 0 && set.entries[i].pad6[0] == 0x5a);
        }
        for (i = 0; i < 32; i++) {
            assert(set.ids[i] == 0xffff);
        }
        assert(set.mode == 0 && set.low_1a1 == 0 && set.h418 == 0 && set.h420 == 0x5a5a);
        assert(((unsigned char *)&set)[0x1a0] == 0x40 && ((unsigned char *)&set)[0x1a1] == 0x50 && set.v0.x != 0.0f);
    }
}

float gSzView_vecUnit[4];
static float *turn_matrix;
static int *turn_angles;
static int turn_calls, apply_calls;
static int light_test, light_calls;
static struct LightCall {
    char kind;
    void *p[3];
    int slot;
    float f[4];
} light_log[16];
static struct DotrViewVector *apply_out, *apply_vector;
static struct DotrViewMatrix *apply_matrix;

void RhGeom_GetRotXYZMatrix(float *m, int *ang)
{
    turn_matrix = m;
    turn_angles = ang;
    turn_calls++;
}

void RhGeom_MulMatrixVector(struct DotrViewVector *out, struct DotrViewMatrix *m, struct DotrViewVector *v)
{
    if (light_test) {
        struct LightCall *c = &light_log[light_calls++];
        c->kind = 'M'; c->p[0] = out; c->p[1] = m; c->p[2] = v;
        c->f[0] = ((struct DotrBlockVector4 *)v)->unknown_w;
        ((struct DotrBlockVector4 *)out)->unknown_w = 5.0f;
        return;
    }
    assert(turn_calls == apply_calls + 1); /* after 0x20c630 */
    apply_out = out;
    apply_matrix = m;
    apply_vector = v;
    apply_calls++;
}

/* 0x1f95e0 branch coverage: every rejection, both boundaries of each range, with and without a
 * source vector. */
static void check_part_turn(void)
{
    struct DotrBlockVector4 vector = {1.0f, 2.0f, 3.0f, 4.0f};
    static const int ks[] = {0, 9};
    static const int parts[] = {0, 2};
    unsigned i, j, pass;

    turn_calls = apply_calls = 0;
    for (pass = 0; pass < 2; pass++) {
        fill(0x5a);
        if (pass) gSzView_hView_blocks[0] = 0;
        for (i = 0; i < sizeof(bad_index) / sizeof(bad_index[0]); i++) {
            assert(SzView_SetStraightLightDirection(pass ? 0 : bad_index[i], 0, 0, &vector) == 0);
        }
        assert(same());
    }
    fill(0x5a);
    for (i = 0; i < sizeof(bad_position) / sizeof(bad_position[0]); i++) {
        assert(SzView_SetStraightLightDirection(0, bad_position[i], 0, &vector) == 0);
    }
    assert(SzView_SetStraightLightDirection(0, 0, -1, &vector) == 0 && SzView_SetStraightLightDirection(0, 0, 3, &vector) == 0);
    assert(SzView_SetStraightLightDirection(0, 0, INT_MIN, &vector) == 0 && SzView_SetStraightLightDirection(0, 0, INT_MAX, &vector) == 0);
    assert(same() && turn_calls == 0 && apply_calls == 0);

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            struct DotrBlockTriple *t = &block.modelViewData[ks[i]].unknown_00[parts[j]];
            fill(0);
            turn_calls = apply_calls = 0;
            /* no source vector: nothing stored, both callees still run */
            assert(SzView_SetStraightLightDirection(0, ks[i], parts[j], 0) == 1 && same() && turn_calls == 1 && apply_calls == 1);
            assert(turn_angles == (int *)&t->unknown_00[1] && turn_matrix == (float *)apply_matrix);
            assert(apply_out == (struct DotrViewVector *)&t->unknown_00[2]);
            assert(apply_vector == (struct DotrViewVector *)gSzView_vecUnit);
            /* source vector: stored as vector 1 of the part first */
            assert(SzView_SetStraightLightDirection(0, ks[i], parts[j], &vector) == 1 && turn_calls == 2 && apply_calls == 2);
            expected.modelViewData[ks[i]].unknown_00[parts[j]].unknown_00[1] = vector;
            assert(same() && turn_angles == (int *)&t->unknown_00[1]);
            assert(apply_out == (struct DotrViewVector *)&t->unknown_00[2]);
        }
    }
}

static int view_calls, view_order[8];
static float view_floats[13];
static struct DotrViewQuad *view_quad;
static struct DotrBlockViewArgs view_args;
static struct DotrDrawTag *view_tag;

int RhView_SetScreen(float a, float b, float c, float d, float e)
{
    view_order[view_calls++] = 1;
    view_floats[0] = a; view_floats[1] = b; view_floats[2] = c; view_floats[3] = d; view_floats[4] = e;
    return 0;
}
int RhView_SetAspect(float a, float b) { view_order[view_calls++] = 2; view_floats[5] = a; view_floats[6] = b; return 0; }
int RhView_SetClipZ(float a, float b) { view_order[view_calls++] = 3; view_floats[7] = a; view_floats[8] = b; return 0; }
void Ex3d_InitClipPlane(void) { view_order[view_calls++] = 4; }
int RhView_SetFogParameter(float a, float b, float c, float d, struct DotrViewQuad *q)
{
    view_order[view_calls++] = 5;
    view_floats[9] = a; view_floats[10] = b; view_floats[11] = c; view_floats[12] = d;
    view_quad = q;
    return 0;
}
int RhView_Set(struct DotrViewBlock *block)
{
    view_order[view_calls++] = 6;
    memcpy(&view_args, block, sizeof(view_args));
    return 0;
}
struct DotrDrawTag *RhView_SendViewParameters(struct DotrDrawTag *tag)
{
    view_order[view_calls++] = 7;
    view_tag = tag;
    return (struct DotrDrawTag *)((char *)tag + 8);
}

/* SzView_Set_Pipe with all seven callees stubbed: one assertion per branch
 * outcome (index below 0, index above 0, empty slot, live block); the live
 * call checks every forwarded argument, the call order and the return. */
static void check_view_setup(void)
{
    static struct DotrBlock b;
    static char tags[16];
    struct DotrDrawTag *tag = (struct DotrDrawTag *)tags;
    struct DotrBlock *saved = gSzView_hView_blocks[0];
    struct DotrBlockHead *h = (struct DotrBlockHead *)&b;
    int i;

    assert(sizeof(struct DotrBlockViewArgs) == 0x90 && offsetof(struct DotrBlockViewArgs, unknown_80) == 0x80);
    assert(offsetof(struct DotrBlockHead, screenDistance) == 0x44 && offsetof(struct DotrBlock, unknown_7e0[9]) == 0x828);
    assert(offsetof(struct DotrBlock, unknown_830[9]) == 0x878 && offsetof(struct DotrBlock, fogColour[9]) == 0x8a4);
    memset(&b, 0, sizeof(b));
    for (i = 0; i < 4; i++) {
        (&h->relativeDirection.x)[i] = (float)(10 + i);
        (&h->sourcePoint.x)[i] = (float)(20 + i);
    }
    h->unknown_40 = 0x1234567;
    h->screenDistance = 1.5f;
    h->xAspectRatio = 2.5f;
    h->yAspectRatio = 3.5f;
    h->minZClip = 4.5f;
    h->maxZClip = 5.5f;
    b.unknown_7e0[9].x = 6.5f;
    b.unknown_7e0[9].y = 7.5f;
    b.unknown_830[9].x = 8.5f;
    b.unknown_830[9].y = 9.5f;
    view_calls = 0;
    gSzView_hView_blocks[0] = &b;
    assert(SzView_Set_Pipe(tag, -1) == tag && SzView_Set_Pipe(tag, 1) == tag && view_calls == 0);
    gSzView_hView_blocks[0] = 0;
    assert(SzView_Set_Pipe(tag, 0) == tag && view_calls == 0);
    gSzView_hView_blocks[0] = &b;
    assert(SzView_Set_Pipe(tag, 0) == (struct DotrDrawTag *)(tags + 8) && view_calls == 7 && view_tag == tag);
    for (i = 0; i < 7; i++) {
        assert(view_order[i] == i + 1);
    }
    assert(view_floats[0] == 1.5f && view_floats[1] == 640.0f && view_floats[2] == 448.0f);
    assert(view_floats[3] == 320.0f && view_floats[4] == 224.0f);
    assert(view_floats[5] == 2.5f && view_floats[6] == 3.5f && view_floats[7] == 4.5f && view_floats[8] == 5.5f);
    assert(view_floats[9] == 6.5f && view_floats[10] == 7.5f && view_floats[11] == 8.5f && view_floats[12] == 9.5f);
    assert(view_quad == (struct DotrViewQuad *)&b.fogColour[9]);
    assert(memcmp(&view_args.unknown_40, &h->sourcePoint, 16) == 0);
    assert(memcmp(&view_args.unknown_50, &h->relativeDirection, 16) == 0 && view_args.unknown_80 == 0x1234567);
    gSzView_hView_blocks[0] = saved;
}

/* 0x1f9770 branch coverage: every rejection, both position bounds, and the three source cases (0,
 * 1, pointer). */
static void check_element_set(void)
{
    static union DotrBlockElement144 in;
    static const int ks[] = {0, 9};
    unsigned i, pass;

    memset(&in, 0x33, sizeof(in));
    assert(sizeof(in) == 144 && offsetof(struct DotrBlockElements, modelViewData) == 0x60);
    for (pass = 0; pass < 2; pass++) {
        fill(0x5a);
        if (pass) gSzView_hView_blocks[0] = 0;
        for (i = 0; i < sizeof(bad_index) / sizeof(bad_index[0]); i++) {
            assert(SzView_SetStraightLight(pass ? 0 : bad_index[i], 0, &in) == 0);
        }
        assert(same());
    }
    fill(0x5a);
    for (i = 0; i < sizeof(bad_position) / sizeof(bad_position[0]); i++) {
        assert(SzView_SetStraightLight(0, bad_position[i], &in) == 0);
    }
    assert(same());
    for (i = 0; i < 2; i++) {
        fill(0);
        turn_calls = apply_calls = 0;
        block.unknown_8a8 = expected.unknown_8a8 = 0x8004;
        /* 1: only the element's bit is set */
        assert(SzView_SetStraightLight(0, ks[i], (union DotrBlockElement144 *)1) == 1 && turn_calls == 0);
        expected.unknown_8a8 = 0x8004 | (1 << ks[i]);
        assert(same());
        /* 0: the bit is cleared */
        assert(SzView_SetStraightLight(0, ks[i], 0) == 1 && turn_calls == 0);
        expected.unknown_8a8 = 0x8004;
        assert(same());
        /* pointer: element copied, each of its three parts turned, bit set */
        assert(SzView_SetStraightLight(0, ks[i], &in) == 1 && turn_calls == 3 && apply_calls == 3);
        assert(memcmp(&block.modelViewData[ks[i]], &in, sizeof(in)) == 0);
        assert(turn_angles == (int *)&block.modelViewData[ks[i]].unknown_00[2].unknown_00[1]);
        assert(block.unknown_8a8 == (0x8004 | (1 << ks[i])));
    }
}

struct DotrBlockVector4 gSzView_pntCPDef = {1.0f, 5.0f, 1.0f, 9.0f};

static void check_far_end(void)
{
    struct DotrBlockDelta delta;
    struct DotrBlockVector4 *out = (struct DotrBlockVector4 *)delta.unknown_00;
    struct DotrBlockVector4 in = {1.0f, 5.0f, 1.0f, 7.0f};
    static const int rejected[] = {INT_MIN, -1, 1, INT_MAX};
    float angle = 6.2831855f + (4.0f * 4.0f + 2.0f);
    int pass;
    int i;

    /* pass 0: the given vector; pass 1: a null pointer takes the default */
    for (pass = 0; pass < 2; pass++) {
        memset(&delta, 0, sizeof(delta));
        delta.relativeDirection.unknown_w = 99.0f;
        delta.sourcePoint.x = 3.0f;
        delta.sourcePoint.y = 2.0f;
        delta.sourcePoint.z = 5.0f;
        gSzView_hView_delta[0] = &delta;
        normalize_calls = polar_calls = 0;
        assert(SzView_SetCP(0, pass ? 0 : &in) == 1);
        assert(delta.cameraPoint.x == 1.0f && delta.cameraPoint.y == 5.0f && delta.cameraPoint.z == 1.0f);
        assert(delta.cameraPoint.unknown_w == (pass ? 9.0f : 7.0f));
        assert(normalize_calls == 1 && normalized == &delta.relativeDirection && polar_calls == 5);
        assert(delta.relativeDirection.x == -2.0f && delta.relativeDirection.y == 3.0f && delta.relativeDirection.z == -4.0f);
        assert(delta.relativeDirection.unknown_w == 0.0f);
        assert(out->x == (4.0f + 9.0f + 16.0f) * 0.5f);
        assert(out->y == angle - 6.2831855f);
        assert(out->z == -3.0f * 4.0f + (16.0f + 4.0f) * 0.5f);
    }
    for (i = 0; i < 4; i++) assert(SzView_SetCP(rejected[i], &in) == 0);
    gSzView_hView_delta[0] = 0;
    assert(SzView_SetCP(0, &in) == 0 && normalize_calls == 1 && polar_calls == 5);
}

static void check_rel_sp(void)
{
    struct DotrBlockDelta delta;
    struct DotrBlockVector4 *out = (struct DotrBlockVector4 *)delta.unknown_00;
    struct DotrBlockVector4 in = {2.0f, 3.0f, 4.0f, 7.0f};
    static const int rejected[] = {INT_MIN, -1, 1, INT_MAX};
    int i;

    memset(&delta, 0, sizeof(delta));
    out->x = 2.0f;
    delta.sourcePoint.unknown_w = 99.0f;
    delta.cameraPoint.x = delta.cameraPoint.y = delta.cameraPoint.z = 10.0f;
    gSzView_hView_delta[0] = &delta;
    normalize_calls = polar_calls = 0;
    assert(SzView_SetRelSP(0, &in) == 1);
    assert(normalize_calls == 1 && normalized == &delta.relativeDirection && polar_calls == 5);
    assert(delta.relativeDirection.x == 2.0f && delta.relativeDirection.y == 3.0f && delta.relativeDirection.z == 4.0f);
    assert(delta.relativeDirection.unknown_w == 7.0f);
    assert(delta.sourcePoint.x == 6.0f && delta.sourcePoint.y == 4.0f && delta.sourcePoint.z == 2.0f);
    assert(delta.sourcePoint.unknown_w == 0.0f);
    assert(out->x == (16.0f + 36.0f + 64.0f) * 0.5f);
    /* a null vector, a rejected index and an empty slot change nothing */
    assert(SzView_SetRelSP(0, 0) == 0 && normalize_calls == 1);
    for (i = 0; i < 4; i++) assert(SzView_SetRelSP(rejected[i], &in) == 0);
    gSzView_hView_delta[0] = 0;
    assert(SzView_SetRelSP(0, &in) == 0 && normalize_calls == 1 && polar_calls == 5);
}

struct DotrBlockVector4 gSzView_pntSPDef = {3.0f, 2.0f, 5.0f, 8.0f};

/* 0x1f9ad0, 0x1f9b20 and 0x1f9fe0: the near-end copies of the routine above.
 * mode bit 0: null near vector (default taken); bit 1: null far vector. */
static void check_near_end(void)
{
    struct DotrBlockDelta delta;
    struct DotrBlockVector4 *out = (struct DotrBlockVector4 *)delta.unknown_00;
    struct DotrBlockVector4 near_in = {3.0f, 2.0f, 5.0f, 6.0f};
    struct DotrBlockVector4 far_in = {1.0f, 5.0f, 1.0f, 7.0f};
    static const int rejected[] = {INT_MIN, -1, 1, INT_MAX};
    float angle = 6.2831855f + (4.0f * 4.0f + 2.0f);
    int both;
    int mode;
    int i;

    for (both = 0; both < 2; both++) {
        for (mode = 0; mode < (both ? 4 : 2); mode++) {
            memset(&delta, 0, sizeof(delta));
            delta.relativeDirection.unknown_w = 99.0f;
            delta.cameraPoint = far_in;
            delta.cameraPoint.unknown_w = 4.0f;
            gSzView_hView_delta[0] = &delta;
            normalize_calls = polar_calls = 0;
            if (both) {
                assert(SzView_SetCamera(0, (mode & 1) ? 0 : &near_in, (mode & 2) ? 0 : &far_in) == 1);
                assert(delta.cameraPoint.unknown_w == ((mode & 2) ? 9.0f : 7.0f));
            } else {
                assert(SzView_SetSP(0, (mode & 1) ? 0 : &near_in) == 1);
                assert(delta.cameraPoint.unknown_w == 4.0f);
            }
            assert(delta.sourcePoint.x == 3.0f && delta.sourcePoint.y == 2.0f && delta.sourcePoint.z == 5.0f);
            assert(delta.sourcePoint.unknown_w == ((mode & 1) ? 8.0f : 6.0f));
            assert(delta.cameraPoint.x == 1.0f && delta.cameraPoint.y == 5.0f && delta.cameraPoint.z == 1.0f);
            assert(normalize_calls == 1 && normalized == &delta.relativeDirection && polar_calls == 5);
            assert(delta.relativeDirection.x == -2.0f && delta.relativeDirection.y == 3.0f && delta.relativeDirection.z == -4.0f);
            assert(delta.relativeDirection.unknown_w == 0.0f);
            assert(out->x == (4.0f + 9.0f + 16.0f) * 0.5f);
            assert(out->y == angle - 6.2831855f);
            assert(out->z == -3.0f * 4.0f + (16.0f + 4.0f) * 0.5f);
        }
    }
    assert(SzView_GetSP(0) == (void *)&delta.sourcePoint);
    for (i = 0; i < 4; i++) {
        assert(SzView_GetSP(rejected[i]) == 0);
        assert(SzView_SetSP(rejected[i], &near_in) == 0);
        assert(SzView_SetCamera(rejected[i], &near_in, &far_in) == 0);
    }
    gSzView_hView_delta[0] = 0;
    assert(SzView_GetSP(0) == 0 && SzView_SetSP(0, &near_in) == 0);
    assert(SzView_SetCamera(0, &near_in, &far_in) == 0 && normalize_calls == 1 && polar_calls == 5);
}

static int angle_calls, angle_result;
static int *angle_out;

int func_001fafb0_angles(int index, int *out)
{
    assert(index == 0);
    angle_out = out;
    angle_calls++;
    return angle_result;
}

static void check_polar_ends(int (*f)(int, struct DotrBlockVector4 *), int far_end)
{
    struct DotrBlockDelta delta;
    struct DotrBlockVector4 in = {2.0f, 1.0f, 3.0f, 4.0f};
    struct DotrBlockVector4 *polar = (struct DotrBlockVector4 *)delta.unknown_00;
    struct DotrBlockVector4 *base = far_end ? &delta.sourcePoint : &delta.cameraPoint;
    struct DotrBlockVector4 *end = far_end ? &delta.cameraPoint : &delta.sourcePoint;
    static const int rejected[] = {INT_MIN, -1, 1, INT_MAX};
    float wrapped = 6.2831855f + 1.0f;
    int i, result;

    wrapped = wrapped - 6.2831855f;
    for (result = 0; result < 2; result++) {
        memset(&delta, 0, sizeof(delta));
        delta.relativeDirection.x = 1.0f; delta.relativeDirection.y = 2.0f; delta.relativeDirection.z = -3.0f;
        base->x = 10.0f; base->y = 20.0f; base->z = 30.0f;
        end->unknown_w = 99.0f;
        gSzView_hView_delta[0] = &delta;
        polar_calls = normalize_calls = angle_calls = apply_calls = 0;
        turn_calls = result ? 0 : 1; /* the 0x20cdd0 stub expects one turn ahead */
        angle_result = result;
        assert(f(0, &in) == 1 && polar_calls == 1 && angle_calls == 1 && turn_calls == 1 && apply_calls == 1);
        assert(polar->x == 2.0f && polar->y == wrapped && polar->z == 3.0f && polar->unknown_w == 4.0f);
        if (result) assert((void *)turn_matrix == (void *)apply_matrix && turn_angles == angle_out);
        assert((void *)apply_out == (void *)&delta.relativeDirection && (void *)apply_vector == (void *)gSzView_vecUnit);
        if (far_end) assert(end->x == 8.0f && end->y == 16.0f && end->z == 36.0f);
        else assert(end->x == 12.0f && end->y == 24.0f && end->z == 24.0f);
        assert(end->unknown_w == 0.0f && normalize_calls == 1 && normalized == &delta.relativeDirection);
        assert(delta.relativeDirection.x == -2.0f && delta.relativeDirection.y == -4.0f && delta.relativeDirection.z == 6.0f);
    }
    polar_calls = 0;
    assert(f(0, 0) == 0 && polar_calls == 0);
    for (i = 0; i < 4; i++) assert(f(rejected[i], &in) == 0);
    gSzView_hView_delta[0] = 0;
    assert(f(0, &in) == 0 && polar_calls == 0 && normalize_calls == 1);
}

struct DotrDrawTag *RhVu1Mac_SetStraightLightToVu1_Ex(struct DotrDrawTag *tag, struct DotrBlockTriple *light,
                                                      struct DotrBlockVector4 *dir, int slot)
{
    struct LightCall *c = &light_log[light_calls++];
    c->kind = 'S'; c->p[0] = tag; c->p[1] = light; c->p[2] = dir; c->slot = slot;
    return tag ? (struct DotrDrawTag *)((char *)tag + 16) : (struct DotrDrawTag *)0;
}

struct DotrDrawTag *RhVu1Mac_SetPointLightToVu1(struct DotrDrawTag *tag, float *a, float *b, float x, float y, float z)
{
    struct LightCall *c = &light_log[light_calls++];
    c->kind = 'P'; c->p[0] = tag; c->p[1] = a; c->p[2] = b;
    c->f[0] = x; c->f[1] = y; c->f[2] = z; c->f[3] = b ? b[3] : -1.0f;
    return (struct DotrDrawTag *)((char *)tag + 32);
}

/* One run of 0x1f8d30: light_k / point_k are the slots expected to be used; lights / point say whether the
 * straight lights and the point light are expected to be submitted. */
static void check_light_run(int k, unsigned on, unsigned point_on, unsigned point_off, int light_k, int point_k,
                            int lights, int point)
{
    static struct DotrBlock block;
    static char buf[64];
    struct DotrDrawTag *tag = (struct DotrDrawTag *)buf;
    struct DotrBlockElement *e = &block.modelViewData[light_k];
    union DotrBlockMatrix48 *q = &block.unknown_600[point_k];
    struct LightCall *c;
    int i, n = 0;

    memset(&block, 0, sizeof block);
    block.unknown_8a8 = (unsigned short)on;
    block.lightingFlags = (unsigned short)point_on;
    block.unknown_8ae = (unsigned short)point_off;
    for (i = 0; i < 3; i++) {
        e->unknown_00[i].unknown_00[2].unknown_w = 7.0f;
    }
    q->unknown_vectors[2].x = 2.0f; q->unknown_vectors[2].y = 3.0f; q->unknown_vectors[2].z = 4.0f;
    gSzView_hView_blocks[0] = &block;
    light_calls = 0;
    assert(SzView_SetLight_Pipe(tag, 0, k) == (struct DotrDrawTag *)(buf + 48));
    c = &light_log[n++];
    assert(c->kind == 'S' && c->p[0] == 0 && c->p[1] == 0 && c->p[2] == 0 && c->slot == -1);
    for (i = 0; i < 3; i++) {
        if (!lights) {
            assert(e->unknown_00[i].unknown_00[2].unknown_w == 7.0f);
            continue;
        }
        c = &light_log[n++];
        assert(c->kind == 'M' && c->p[1] == 0 && c->p[2] == &e->unknown_00[i].unknown_00[2] && c->f[0] == 0.0f);
        assert(c[1].kind == 'S' && c[1].p[0] == 0 && c[1].p[1] == &e->unknown_00[i] && c[1].p[2] == c->p[0]);
        assert(c[1].slot == i);
        n++;
    }
    c = &light_log[n++];
    assert(c->kind == 'S' && c->p[0] == tag && c->p[1] == 0 && c->p[2] == 0 && c->slot == -1);
    if (point) {
        c = &light_log[n++];
        assert(c->kind == 'M' && c->p[1] == 0 && c->p[2] == &q->unknown_vectors[1]);
        assert(c[1].kind == 'P' && c[1].p[0] == buf + 16 && c[1].p[1] == q && c[1].p[2] == c->p[0]);
        assert(c[1].f[0] == 2.0f && c[1].f[1] == 3.0f && c[1].f[2] == 4.0f && c[1].f[3] == 1.0f);
        n++;
    } else {
        c = &light_log[n++];
        assert(c->kind == 'P' && c->p[0] == buf + 16 && c->p[1] == 0 && c->p[2] == 0);
        assert(c->f[0] == 0.0f && c->f[1] == 0.0f && c->f[2] == 0.0f);
    }
    assert(light_calls == n);
}

static void check_set_light(void)
{
    static struct DotrBlock block;
    struct DotrDrawTag *tag = (struct DotrDrawTag *)&block;

    light_test = 1;
    light_calls = 0;
    gSzView_hView_blocks[0] = &block;
    assert(SzView_SetLight_Pipe(tag, -1, 0) == tag);
    assert(SzView_SetLight_Pipe(tag, 1, 0) == tag);
    assert(SzView_SetLight_Pipe(tag, 0, -1) == tag);
    assert(SzView_SetLight_Pipe(tag, 0, 10) == tag);
    gSzView_hView_blocks[0] = 0;
    assert(SzView_SetLight_Pipe(tag, 0, 0) == tag);
    assert(light_calls == 0);
    check_light_run(9, 0x200, 0x200, 0, 9, 9, 1, 1);     /* slot 9 skips the fallback tests */
    check_light_run(9, 0, 0, 0x200, 9, 9, 0, 0);
    check_light_run(0, 1, 1, 0, 0, 0, 1, 1);             /* both bits set: own slot */
    check_light_run(2, 0x200, 4, 0, 9, 2, 1, 1);         /* light bit clear: slot 9's lights */
    check_light_run(2, 0, 4, 0, 9, 2, 0, 1);             /* and slot 9 off as well */
    check_light_run(2, 4, 0x200, 0, 2, 9, 1, 1);         /* point bit clear: slot 9's point light */
    check_light_run(2, 4, 4, 4, 2, 9, 1, 0);             /* point bit set but masked at +0x8ae, slot 9 off */
    check_light_run(3, 8, 8, 4, 3, 3, 1, 1);             /* another slot's mask bit does not count */
    light_test = 0;
}

static struct DotrBlockVector4 center_value;
static int center_calls, center_index;
int SzModel_GetCenter(int index, struct DotrBlockVector4 *out)
{
    center_calls++;
    center_index = index;
    *out = center_value;
    return 1;
}

/* The polar triple as the 0x1a6c60 / 0x1a6c18 / 0x1a6c30 stubs above produce it. */
static void camera_polar_expect(struct DotrBlockDelta *d)
{
    float *o = (float *)d->unknown_00;
    float dx = d->sourcePoint.x - d->cameraPoint.x;
    float dy = d->sourcePoint.y - d->cameraPoint.y;
    float dz = d->sourcePoint.z - d->cameraPoint.z;
    float xz = (dz * dz + dx * dx) * 0.5f;
    float yaw = 6.2831855f + (dz * 4.0f + dx);

    o[1] = yaw - 6.2831855f;
    o[2] = dy * 4.0f + xz;
    o[0] = (dx * dx + dy * dy + dz * dz) * 0.5f;
}

static void camera_rel_expect(struct DotrBlockDelta *d)
{
    d->relativeDirection.x = d->cameraPoint.x - d->sourcePoint.x;
    d->relativeDirection.y = d->cameraPoint.y - d->sourcePoint.y;
    d->relativeDirection.z = d->cameraPoint.z - d->sourcePoint.z;
    d->relativeDirection.unknown_w = 0.0f;
}

/* Branch coverage: bad indices, empty slot, modes 0..3 (1 and 2 act, others return 1 unchanged),
 * each with a given base and with the null base that selects the current +0x30 point. */
static void check_set_view_camera(void)
{
    static union {
        struct DotrBlockDelta delta;
        unsigned char bytes[0x8b4];
        int align;
    } object, want;
    static const int rejected[] = {INT_MIN, -1, 1, INT_MAX};
    struct DotrBlockDelta *d = &object.delta, *w = &want.delta;
    float *polar = (float *)d->unknown_00, *wpolar = (float *)w->unknown_00;
    struct DotrBlockVector4 base = {4.0f, 8.0f, 12.0f, 77.0f};
    int i, mode;

    for (i = 0; i < 4; i++) assert(SzView_SetViewCamera(rejected[i], 5, &base, 1, 0.25f) == 0);
    gSzView_hView_delta[0] = 0;
    assert(SzView_SetViewCamera(0, 5, &base, 1, 0.25f) == 0);
    gSzView_hView_delta[0] = d;
    center_value.x = 16.0f; center_value.y = 32.0f; center_value.z = 64.0f; center_value.unknown_w = 5.0f;
    for (mode = 0; mode <= 3; mode++) {
        for (i = 0; i < 2; i++) {
            struct DotrBlockVector4 from;
            float old[3];

            memset(&object, 0, sizeof(object));
            *(int *)(object.bytes + 0x8b0) = 1;
            polar[0] = 2.0f; polar[1] = 0.5f; polar[2] = 3.0f; polar[3] = 6.0f;
            d->sourcePoint.x = 1.0f; d->sourcePoint.y = 2.0f; d->sourcePoint.z = 3.0f;
            d->cameraPoint.x = 8.0f; d->cameraPoint.y = 16.0f; d->cameraPoint.z = 32.0f;
            d->cameraPoint.unknown_w = 9.0f;
            want = object;
            center_calls = polar_calls = normalize_calls = sin_calls = cos_calls = 0;
            assert(SzView_SetViewCamera(0, 5, i ? 0 : &base, mode, 0.25f) == 1);
            if (mode != 1 && mode != 2) {
                assert(memcmp(&object, &want, sizeof(object)) == 0);
                assert(center_calls == 0 && polar_calls == 0 && normalize_calls == 0);
                continue;
            }
            from = i ? w->cameraPoint : base;
            assert(center_calls == 1 && center_index == 5);
            w->cameraPoint.x = 16.0f * 0.25f + from.x * 0.75f;
            w->cameraPoint.y = from.y * 0.75f + 32.0f * 0.25f;
            w->cameraPoint.z = from.z * 0.75f + 64.0f * 0.25f;
            w->cameraPoint.unknown_w = 1.0f;
            camera_rel_expect(w);
            memcpy(old, wpolar, sizeof(old));
            camera_polar_expect(w);
            if (mode == 1) {
                assert(polar_calls == 5 && normalize_calls == 1 && sin_calls == 0 && cos_calls == 0);
            } else {
                float yaw = old[1] - wpolar[1], pitch = old[2] - wpolar[2], dist = old[0] - wpolar[0];

                roll_expect(wpolar, &w->cameraPoint, &w->sourcePoint, yaw, pitch, dist, 1);
                camera_rel_expect(w);
                assert(polar_calls == 7 && normalize_calls == 2 && sin_calls == 2 && cos_calls == 2);
            }
            assert(normalized == &d->relativeDirection);
            assert(memcmp(polar, wpolar, 12) == 0);
            assert(memcmp(&d->relativeDirection, &w->relativeDirection, 48) == 0);
        }
    }
    gSzView_hView_delta[0] = 0;
}

static struct { void *out, *keys; int n; float t; } interp_log[2];
static int interp_calls;
void lSzView_CalcInterpCameraMotion(struct DotrKeyOut *out, struct DotrKeyTriple *keys, int n, float t)
{
    float *o = (float *)out;
    assert(interp_calls < 2);
    interp_log[interp_calls].out = out; interp_log[interp_calls].keys = keys;
    interp_log[interp_calls].n = n; interp_log[interp_calls].t = t;
    o[0] = 1.0f + 10.0f * interp_calls; o[1] = 2.0f + 10.0f * interp_calls; o[2] = 3.0f + 10.0f * interp_calls;
    interp_calls++;
}
static struct { void *keys; int n; float t; } single_log[2];
static int single_calls;
static float single_value[2];
float lSzView_CalcInterpCameraMotion1(struct DotrKeySingle *keys, int n, float t)
{
    assert(single_calls < 2);
    single_log[single_calls].keys = keys; single_log[single_calls].n = n; single_log[single_calls].t = t;
    return single_value[single_calls++];
}
static float angle_arg;
static int angle_calls;
int func_001a22c0(float value) { angle_arg = value; angle_calls++; return 77; }
static int rot_result, rot_calls, rot_index, pos_result, pos_calls, pos_index;
static void *rot_out;
int SzModel_GetRot(int index, struct DotrObjectVector4 *out)
{
    rot_index = index; rot_out = out; rot_calls++;
    return rot_result;
}
int SzModel_GetPos(int index, struct DotrObjectVector4 *out)
{
    float *o = (float *)out;
    pos_index = index; pos_calls++;
    if (pos_result) { o[0] = 100.0f; o[1] = 200.0f; o[2] = 400.0f; o[3] = 800.0f; }
    return pos_result;
}
static int push_calls, pop_calls;
void RhGeom_PushMatrix(float *matrix) { assert(matrix == turn_matrix && light_calls == 0); push_calls++; }
void RhGeom_PopMatrix(void) { assert(light_calls == 2); pop_calls++; }

/* 0x1fb940 branch coverage: every rejection (four bad indices, zero frame total, null motion, wrong tag,
 * empty slot), then each outcome of the field-of-view floor, the screen-distance floor, the mirror flag,
 * the model test (-1 and 0) and both results of the rotation and position queries, plus a zero frame
 * count (the unsigned conversion of 0xffffffff). */
static void check_interp_camera_motion(void)
{
    static union {
        struct DotrBlockCamera camera;
        unsigned char bytes[0x8b4];
        int align;
    } object, want;
    static union {
        struct DotrCameraMotion motion;
        int words[64];
    } block;
    static const int rejected[] = {INT_MIN, -1, 1, INT_MAX};
    struct DotrCameraMotion *m = &block.motion;
    struct DotrBlockCamera *c = &object.camera, *w = &want.camera;
    int i, mirror, model, low_fov, low_screen, wide;

    memset(&block, 0, sizeof(block));
    m->magic = 0x544f4d43;
    m->frames = 5;
    m->track[0].offset = 1; m->track[0].count = 11;
    m->track[1].offset = 3; m->track[1].count = 12;
    m->track[2].offset = 6; m->track[2].count = 13;
    m->track[3].offset = 9; m->track[3].count = 14;
    gSzView_hView_delta[0] = &c->delta;
    interp_calls = 0;
    for (i = 0; i < 4; i++) assert(SzView_SetInterpCameraMotion(rejected[i], m, 6, 3, -1, 0) == 0);
    assert(SzView_SetInterpCameraMotion(0, m, 0, 3, -1, 0) == 0);
    assert(SzView_SetInterpCameraMotion(0, 0, 6, 3, -1, 0) == 0);
    m->magic++;
    assert(SzView_SetInterpCameraMotion(0, m, 6, 3, -1, 0) == 0);
    m->magic--;
    gSzView_hView_delta[0] = 0;
    assert(SzView_SetInterpCameraMotion(0, m, 6, 3, -1, 0) == 0);
    assert(interp_calls == 0);
    gSzView_hView_delta[0] = &c->delta;
    light_test = 1;
    for (i = 0; i < 96; i++) {
        float t, f, screen;

        mirror = i & 1; low_fov = (i >> 1) & 1; low_screen = (i >> 2) & 1; wide = (i >> 3) & 1;
        model = (i >> 4) ? 0 : -1;
        rot_result = (i >> 4) == 2 || (i >> 4) == 4;
        pos_result = (i >> 4) >= 3;
        if ((i >> 4) == 5) { model = 7; rot_result = 0; pos_result = 0; }
        m->frames = wide ? 0 : 5;
        memset(&object, 0, sizeof(object));
        c->delta.sourcePoint.unknown_w = 21.0f; c->delta.cameraPoint.unknown_w = 31.0f;
        c->screen_min = low_screen ? 1.0e9f : 1.0f;
        want = object;
        single_value[0] = 45.0f; single_value[1] = low_fov ? 0.0f : 60.0f;
        interp_calls = single_calls = angle_calls = rot_calls = pos_calls = push_calls = pop_calls = 0;
        light_calls = polar_calls = normalize_calls = sin_calls = cos_calls = 0;
        assert(SzView_SetInterpCameraMotion(0, m, 6, 3, model, mirror) == 1);
        t = (wide ? 4294967296.0f : 4.0f) * 3.0f / 6.0f;
        assert(interp_calls == 2 && single_calls == 2 && angle_calls == 1);
        assert(interp_log[0].out == &c->delta.sourcePoint && interp_log[0].keys == &m->data[1]);
        assert(interp_log[0].n == 11 && interp_log[0].t == t);
        assert(interp_log[1].out == &c->delta.cameraPoint && interp_log[1].keys == &m->data[3]);
        assert(interp_log[1].n == 12 && interp_log[1].t == t);
        assert(single_log[0].keys == &m->data[6] && single_log[0].n == 13 && single_log[0].t == t);
        assert(single_log[1].keys == &m->data[9] && single_log[1].n == 14 && single_log[1].t == t);
        assert(angle_arg == 32768.0f * 45.0f / 180.0f);
        w->delta.sourcePoint.x = 1.0f; w->delta.sourcePoint.y = 2.0f; w->delta.sourcePoint.z = 3.0f;
        w->delta.cameraPoint.x = 11.0f; w->delta.cameraPoint.y = 12.0f; w->delta.cameraPoint.z = 13.0f;
        w->roll = 77;
        f = 3.1415927f * single_value[1] / 180.0f / 2.0f;
        if (low_fov) f = 1e-6f;
        screen = 224.0f * ((f + 1.0f) / (f * 2.0f));
        w->screen = low_screen ? 1.0e9f : screen;
        assert(sin_calls == 1 && cos_calls == 1);
        if (mirror) {
            w->delta.sourcePoint.x = -1.0f; w->delta.cameraPoint.x = -11.0f; w->roll = -77;
        }
        if (model < 0) {
            assert(rot_calls == 0 && pos_calls == 0);
        } else {
            assert(rot_calls == 1 && pos_calls == 1 && rot_index == model && pos_index == model);
        }
        if (model >= 0 && rot_result) {
            assert(push_calls == 1 && pop_calls == 1 && light_calls == 2 && turn_angles == rot_out);
            assert(light_log[0].kind == 'M' && light_log[0].p[0] == &c->delta.sourcePoint);
            assert(light_log[0].p[1] == 0 && light_log[0].p[2] == &c->delta.sourcePoint);
            assert(light_log[1].kind == 'M' && light_log[1].p[0] == &c->delta.cameraPoint);
            assert(light_log[1].p[1] == 0 && light_log[1].p[2] == &c->delta.cameraPoint);
            w->delta.sourcePoint.unknown_w = 5.0f; w->delta.cameraPoint.unknown_w = 5.0f; /* the stub's mark */
        } else {
            assert(push_calls == 0 && pop_calls == 0 && light_calls == 0);
        }
        if (model >= 0 && pos_result) {
            w->delta.sourcePoint.x += 100.0f; w->delta.sourcePoint.y += 200.0f; w->delta.sourcePoint.z += 400.0f;
            w->delta.cameraPoint.x += 100.0f; w->delta.cameraPoint.y += 200.0f; w->delta.cameraPoint.z += 400.0f;
        }
        camera_rel_expect(&w->delta);
        camera_polar_expect(&w->delta);
        assert(polar_calls == 5 && normalize_calls == 1 && normalized == &c->delta.relativeDirection);
        assert(memcmp(&object, &want, 12) == 0);
        assert(memcmp(object.bytes + 0x10, want.bytes + 0x10, sizeof(object) - 0x10) == 0);
    }
    light_test = 0;
    gSzView_hView_delta[0] = 0;
}

static int fog_calls, fog_param_calls, fog_scale_result;
static float fog_scale_arg, fog_params[4];
static unsigned char fog_color[4];
static void *fog_color_tag, *fog_param_tag;
int func_001a23b0(float value)
{
    fog_scale_arg = value;
    return fog_scale_result;
}
struct DotrDrawPacket *RhBase_SetFogColor(struct DotrDrawPacket *packet, unsigned char *bytes)
{
    fog_calls++;
    fog_color_tag = packet;
    memcpy(fog_color, bytes, 4);
    return (struct DotrDrawPacket *)((char *)packet + 16);
}
struct DotrDrawTag *RhVu1Mac_SetFogParameterEx(struct DotrDrawTag *tag, float a, float b, float c, float d)
{
    fog_param_calls++;
    fog_param_tag = tag;
    fog_params[0] = a;
    fog_params[1] = b;
    fog_params[2] = c;
    fog_params[3] = d;
    return (struct DotrDrawTag *)((char *)tag + 32);
}

/* SzView_SetFog_Pipe (branch coverage): index and block rejections, k bounds, each enable bit alone, both
 * clear (fallback to entry 9 with and without its bits), k == 9, and the scaled colour. */
/* Host forwarders for the views SzView_SetFog_Pipe calls (block_table.h). */
void *func_0020a340_fog(void *packet, void *bytes) { return RhBase_SetFogColor(packet, bytes); }
void *func_002093e0_fog(void *tag, float a, float b, float c, float d) { return RhVu1Mac_SetFogParameterEx(tag, a, b, c, d); }

static void check_set_fog(void)
{
    static struct DotrBlock b;
    struct DotrDrawTag *tag = (struct DotrDrawTag *)&b;
    struct DotrDrawTag *done = (struct DotrDrawTag *)((char *)&b + 48);
    int i;

    for (i = 0; i < 10; i++) {
        b.unknown_7e0[i].x = 100.0f + i;
        b.unknown_7e0[i].y = 200.0f + i;
        b.unknown_830[i].x = 300.0f + i;
        b.unknown_830[i].y = 400.0f + i;
        b.fogColour[i].unknown_bytes[0] = 0x80;
        b.fogColour[i].unknown_bytes[1] = 0x40 + i;
        b.fogColour[i].unknown_bytes[2] = 0xff;
        b.fogColour[i].unknown_bytes[3] = 0x10 + i;
    }
    fog_scale_result = 0x140;
    b.unknown_8ac = b.unknown_8ae = 0x3ff;
    gSzView_hView_blocks[0] = &b;
    assert(SzView_SetFog_Pipe(tag, -1, 0, 1.0f) == tag && SzView_SetFog_Pipe(tag, 1, 0, 1.0f) == tag);
    assert(SzView_SetFog_Pipe(tag, 0, -1, 1.0f) == tag && SzView_SetFog_Pipe(tag, 0, 10, 1.0f) == tag);
    gSzView_hView_blocks[0] = 0;
    assert(SzView_SetFog_Pipe(tag, 0, 0, 1.0f) == tag && fog_calls == 0 && fog_param_calls == 0);
    gSzView_hView_blocks[0] = &b;

    /* Both bits of entry 3 set: its own pairs and colour; the scale is the low byte of
     * 128 * 0.5 (0x40). The function converts with a cast, which the target compiles as a
     * func_001a23b0 call and the host as a plain conversion, so the stub is not reached here. */
    assert(SzView_SetFog_Pipe(tag, 0, 3, 0.5f) == done && fog_calls == 1 && fog_param_calls == 1);
    assert(fog_color_tag == (void *)tag && fog_param_tag == (char *)tag + 16);
    assert(fog_color[0] == 0x40 && fog_color[1] == 0x21 && fog_color[2] == 0x7f && fog_color[3] == 0x13);
    assert(fog_params[0] == 103.0f && fog_params[1] == 203.0f && fog_params[2] == 303.0f && fog_params[3] == 403.0f);

    /* Near bit clear: near pair from entry 9, far pair and colour stay entry 3. */
    b.unknown_8ac = 0x3ff & ~8;
    assert(SzView_SetFog_Pipe(tag, 0, 3, 0.5f) == done && fog_calls == 2);
    assert(fog_params[0] == 109.0f && fog_params[1] == 209.0f && fog_params[2] == 303.0f && fog_color[3] == 0x13);

    /* Far bit clear: far pair from entry 9. */
    b.unknown_8ac = 0x3ff;
    b.unknown_8ae = 0x3ff & ~8;
    assert(SzView_SetFog_Pipe(tag, 0, 3, 0.5f) == done && fog_calls == 3);
    assert(fog_params[0] == 103.0f && fog_params[2] == 309.0f && fog_params[3] == 409.0f && fog_color[3] == 0x13);

    /* Both clear: everything from entry 9, colour included. */
    b.unknown_8ac = 0x3ff & ~8;
    assert(SzView_SetFog_Pipe(tag, 0, 3, 0.5f) == done && fog_calls == 4);
    assert(fog_params[0] == 109.0f && fog_params[2] == 309.0f && fog_color[1] == 0x24 && fog_color[3] == 0x19);

    /* Entry 9 itself: one bit is enough, none returns the tag unchanged. */
    b.unknown_8ac = 0;
    b.unknown_8ae = 0x200;
    assert(SzView_SetFog_Pipe(tag, 0, 9, 0.5f) == done && fog_calls == 5 && fog_color[3] == 0x19);
    assert(SzView_SetFog_Pipe(tag, 0, 3, 0.5f) == done && fog_calls == 6 && fog_color[3] == 0x19);
    b.unknown_8ac = 0x200;
    b.unknown_8ae = 0;
    assert(SzView_SetFog_Pipe(tag, 0, 9, 0.5f) == done && fog_calls == 7);
    b.unknown_8ac = 0;
    assert(SzView_SetFog_Pipe(tag, 0, 9, 0.5f) == tag && SzView_SetFog_Pipe(tag, 0, 3, 0.5f) == tag);
    assert(fog_calls == 7 && fog_param_calls == 7);
    gSzView_hView_blocks[0] = &block;
}

int main(void)
{
    check_side_set();
    check_near_end();
    check_far_end();
    check_rel_sp();
    check_layout();
    check_rejections();
    check_block();
    check_slots();
    check_entries();
    check_convert();
    check_delta();
    check_polar();
    check_element_set();
    check_slot_set_reset();
    check_kind_dispatch();
    check_part_turn();
    check_view_setup();
    check_polar_ends(SzView_SetLookSP, 0);
    check_polar_ends(SzView_SetLookCP, 1);
    check_set_light();
    check_roll_camera();
    check_set_view_camera();
    check_interp_camera_motion();
    check_set_fog();
    puts("block_table: ok");
    return 0;
}
