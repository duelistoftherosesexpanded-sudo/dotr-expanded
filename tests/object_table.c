/* Synthetic tests of the object-table accessors. Objects are host fixtures;
 * host offsets after the pointer member differ from the target layout. */
#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include "dotr/ee/object_table.h"
#include "dotr/ee/resource_helpers.h"
#include "dotr/ee/resource_submit.h"

struct DotrObject *gSzModel_ahModel[84];
struct DotrResourceSlots *gSzModel_ahModel_slots[84];

static struct DotrObject object;
static struct DotrObjectLink link;

static const int bad_index[] = {INT_MIN, -1, 84, INT_MAX, 7};

static void reset(void)
{
    memset(gSzModel_ahModel, 0, sizeof(gSzModel_ahModel));
    memset(&object, 0, sizeof(object));
    memset(&link, 0, sizeof(link));
    gSzModel_ahModel[83] = &object; /* index 7 stays a null entry */
}

static int flag_calls, flag_args[3], flag_result;

int func_001f3910_result(int a, int b, int c)
{
    flag_args[0] = a;
    flag_args[1] = b;
    flag_args[2] = c;
    flag_calls++;
    return flag_result;
}

/* 0x1f1070: kinds 4..6 clear link bit 0x400 and report the call result as
 * 0/1; other kinds return 1 without either effect. */
static void check_kind_call(void)
{
    static const int results[] = {0, 1, 7, -1, INT_MIN};
    size_t i;
    int kind;
    reset();
    flag_calls = 0;
    for (i = 0; i < sizeof(bad_index) / sizeof(bad_index[0]); i++) {
        assert(SzModel_LoadOverlayB(bad_index[i]) == 0);
    }
    object.type = 4;
    assert(SzModel_LoadOverlayB(83) == 0 && flag_calls == 0); /* null link */
    object.modelBaseData = &link;
    for (kind = -128; kind < 128; kind++) {
        for (i = 0; i < sizeof(results) / sizeof(results[0]); i++) {
            int selected = kind >= 4 && kind <= 6;
            object.type = (signed char)kind;
            object.modelNo = (short)(kind * 100 - 7);
            link.unknown_00 = 0x1234;
            link.unknown_04 = -1;
            flag_calls = 0;
            flag_result = results[i];
            assert(SzModel_LoadOverlayB(83) == (selected ? results[i] != 0 : 1));
            assert(flag_calls == selected && link.unknown_00 == 0x1234);
            assert(link.unknown_04 == (selected ? ~0x400 : -1));
            if (selected) {
                assert(flag_args[0] == 1 && flag_args[1] == kind * 100 - 7 && flag_args[2] == 0);
            }
        }
    }
}

static void check_rejections(void)
{
    struct DotrObjectVector3 v3 = {1.0f, 2.0f, 3.0f};
    struct DotrObjectVector4 v4;
    float f = 5.0f;
    size_t i;
    reset();
    for (i = 0; i < sizeof(bad_index) / sizeof(bad_index[0]); i++) {
        int index = bad_index[i];
        assert(SzModel_GetRadius(index) == -1.0f && SzModel_GetEffTextureB(index) == 0);
        assert(SzModel_GetOverlayM(index) == 0 && SzModel_GetOverlayB(index) == 0 && SzModel_GetCamera(index, 20) == 0);
        assert(SzModel_IsModelReady(index) == 0 && SzModel_IsModelLoading(index) == 0);
        assert(SzModel_IsModelStandby(index) == 0 && SzModel_IsModelDisp(index) == 0);
        assert(SzModel_GetAnimParam(index, 0) == -1 && SzModel_GetAnim(index) == -1);
        assert(SzModel_SetAnimParam(index, 0, 0) == 0 && SzModel_SetAnim(index, 0) == 0);
        assert(SzModel_SetMotionDelay(index, 1) == 0 && SzModel_GetAnimDir(index) == -1 && SzModel_SetAnimDir(index, 1) == 0);
        assert(SzModel_GetIntensity(index, &v3) == 0 && SzModel_GetIntensityOnly(index, &f) == 0);
        assert(SzModel_SetIntensity(index, &v3) == 0 && SzModel_SetIntensityOnly(index, 1.0f) == 0);
        assert(SzModel_GetPos(index, &v4) == 0);
    }
    assert(v3.x == 1.0f && v3.y == 2.0f && v3.z == 3.0f && f == 5.0f);
}

static void check_links(void)
{
    reset();
    object.OverlayM = 0x1111;
    object.OverlayB = 0x2222;
    object.pCameraMotion = 0x3333;
    object.pCameraMotion2 = 0x4444;
    /* Null link word. */
    assert(SzModel_GetOverlayM(83) == 0 && SzModel_GetOverlayB(83) == 0 && SzModel_GetCamera(83, 20) == 0);
    object.modelBaseData = &link;
    assert(SzModel_GetOverlayM(83) == 0 && SzModel_GetOverlayB(83) == 0);
    link.unknown_04 = 0x100;
    assert(SzModel_GetOverlayM(83) == 0x1111 && SzModel_GetOverlayB(83) == 0);
    link.unknown_04 = ~0x100;
    assert(SzModel_GetOverlayM(83) == 0 && SzModel_GetOverlayB(83) == 0x2222);
    assert(SzModel_GetCamera(83, 19) == 0x3333 && SzModel_GetCamera(83, 20) == 0x4444);
    assert(SzModel_GetCamera(83, 18) == 0 && SzModel_GetCamera(83, 21) == 0 && SzModel_GetCamera(83, -1) == 0);
}

static void check_bits_and_bytes(void)
{
    struct DotrObject expected;
    int value, slot;
    reset();
    for (value = 0; value < 2; value++) {
        object.drawFlag.unknown_bit_2 = value;
        object.drawFlag.unknown_bit_3 = !value;
        object.drawFlag.unknown_bit_1 = value;
        object.modelId = 0;
        assert(SzModel_IsModelReady(83) == value && SzModel_IsModelLoading(83) == !value);
        assert(SzModel_IsModelStandby(83) == 1 && SzModel_IsModelDisp(83) == value);
        /* A negative byte at +0x1f2 forces both results to zero. */
        object.modelId = -1;
        assert(SzModel_IsModelStandby(83) == 0 && SzModel_IsModelDisp(83) == 0);
        object.modelId = 127;
        assert(SzModel_IsModelStandby(83) == 1);
    }
    /* Slot bytes: signed read, guarded store. */
    for (slot = 0; slot < 4; slot++) {
        object.unknown_1e0[slot] = (signed char)(-3 - slot);
        assert(SzModel_GetAnimParam(83, slot) == -3 - slot);
    }
    assert(SzModel_GetAnim(83) == -3);
    assert(SzModel_GetAnimParam(83, -1) == -1 && SzModel_GetAnimParam(83, 4) == -1);
    memcpy(&expected, &object, sizeof(expected));
    assert(SzModel_SetAnimParam(83, 2, 1) == 0); /* table word for value 2 is zero */
    assert(SzModel_SetAnimParam(83, -1, 1) == 0 && SzModel_SetAnimParam(83, 8, 1) == 0);
    object.animationDataPointers[2] = 1;
    expected.animationDataPointers[2] = 1;
    assert(SzModel_SetAnimParam(83, 2, -1) == 0 && SzModel_SetAnimParam(83, 2, 4) == 0);
    assert(memcmp(&expected, &object, sizeof(expected)) == 0);
    expected.unknown_1e0[1] = 2;
    assert(SzModel_SetAnimParam(83, 2, 1) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
    expected.unknown_1e0[3] = 2;
    assert(SzModel_SetAnim(83, 2) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
    /* Byte +0x1fd and the two-bit field at +0x1f7. */
    expected.motionDelay = 0x9a;
    assert(SzModel_SetMotionDelay(83, 0x39a) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
    for (value = -2; value < 6; value++) {
        memset(&object.animDir, value & 1 ? 0xff : 0, 1);
        memcpy(&expected, &object, sizeof(expected));
        expected.animDir.unknown_bits_6_7 = value;
        assert(SzModel_SetAnimDir(83, value) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
        /* The field is read back as a signed two-bit value. */
        assert(SzModel_GetAnimDir(83) == expected.animDir.unknown_bits_6_7);
        assert(SzModel_GetAnimDir(83) >= -2 && SzModel_GetAnimDir(83) <= 1);
    }
}

static void check_floats(void)
{
    struct DotrObject expected;
    struct DotrObjectVector3 in = {1.5f, -2.5f, 3.5f}, out3 = {0.0f, 0.0f, 0.0f};
    struct DotrObjectVector4 out4 = {9.0f, 9.0f, 9.0f, 9.0f};
    float one = 0.0f;
    reset();
    object.radius = 42.25f;
    assert(SzModel_GetRadius(83) == 42.25f);
    assert(SzModel_GetEffTextureB(83) == (void *)object.effectTextureB);
    memcpy(&expected, &object, sizeof(expected));
    /* A null source stores the constants 0.8, 0.8, 1.0. */
    expected.intensity.x = 0.8f;
    expected.intensity.y = 0.8f;
    expected.intensity.z = 1.0f;
    assert(SzModel_SetIntensity(83, 0) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
    expected.intensity = in;
    assert(SzModel_SetIntensity(83, &in) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
    assert(SzModel_GetIntensity(83, &out3) == 1 && memcmp(&out3, &in, sizeof(in)) == 0);
    assert(SzModel_GetIntensityOnly(83, &one) == 1 && one == 3.5f);
    expected.intensity.z = -8.0f;
    assert(SzModel_SetIntensityOnly(83, -8.0f) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
    object.pos.x = 1.0f;
    object.pos.y = 2.0f;
    object.pos.z = 3.0f;
    object.pos.unknown_w = 4.0f;
    /* A null destination succeeds without copying. */
    assert(SzModel_GetPos(83, 0) == 1);
    assert(SzModel_GetPos(83, &out4) == 1 && memcmp(&out4, &object.pos, sizeof(out4)) == 0);
}

/* Stub for SzModel_SetAnimIntrReverseParam (0x1f16c0), which is tested with its own group. */
static int reverse_index, reverse_frames, reverse_result;
int SzModel_SetAnimIntrReverseParam(int index, int frames)
{
    reverse_index = index;
    reverse_frames = frames;
    return reverse_result;
}

static void check_thunks(void)
{
    /* 0x1f16b0 forwards the index with a second argument of 6 and returns the callee's result. */
    reverse_result = 0x1234;
    assert(SzModel_SetAnimIntrReverse(41) == 0x1234 && reverse_index == 41 && reverse_frames == 6);
    /* 0x1f1550 forwards to 0x1f1560 with a third argument of 6. */
    reset();
    object.animationDataPointers[2] = 1;
    assert(SzModel_SetAnimIntr(5, 2) == 0 && SzModel_SetAnimIntr(83, 8) == 0);
    assert(SzModel_SetAnimIntr(83, 2) == 1 && object.blendTickTotal == 6 && object.unknown_1e0[0] == 2);
}

static void check_slots(void)
{
    struct DotrObject expected;
    struct DotrObjectQuad quad = {5, 6, 7, 8};
    int slot, value;
    reset();
    /* Rejections: index, slot range (where checked), null entry, a negative
     * slot byte, and a zero word for the selected value. */
    assert(SzModel_SetAnimSpeedParam(7, 0, 1.0f) == 0 && SzModel_SetAnimSpeedParam(84, 0, 1.0f) == 0);
    assert(SzModel_GetAnimFrameParam(7, 0) == -1.0f && SzModel_GetAnimFrameParam(-1, 0) == -1.0f);
    assert(SzModel_SetAnimFrameParam(7, 0, 1.0f) == 0 && SzModel_GetAnimFrameMax(7, 0) == -1 && SzModel_GetAnimFrameMax(84, 0) == -1);
    assert(SzModel_SetFilter(7, 0, &quad) == 0 && SzModel_GetStoneFlag(7) == 0 && SzModel_GetStoneRatio(7) == -1);
    assert(SzModel_SetAnimSpeedParam(83, -1, 1.0f) == 0 && SzModel_SetAnimSpeedParam(83, 4, 1.0f) == 0);
    assert(SzModel_GetAnimFrameParam(83, -1) == -1.0f && SzModel_GetAnimFrameParam(83, 4) == -1.0f);
    for (slot = 0; slot < 2; slot++) {
        reset();
        object.unknown_1e0[slot] = -1;
        memcpy(&expected, &object, sizeof(expected));
        assert(SzModel_SetAnimSpeedParam(83, slot, 2.0f) == 0 && SzModel_GetAnimFrameParam(83, slot) == -1.0f);
        assert(SzModel_SetAnimFrameParam(83, slot, 2.0f) == 0);
        object.unknown_1e0[slot] = expected.unknown_1e0[slot] = 5;
        assert(SzModel_SetAnimSpeedParam(83, slot, 2.0f) == 0 && SzModel_GetAnimFrameParam(83, slot) == -1.0f);
        assert(SzModel_SetAnimFrameParam(83, slot, 2.0f) == 0);
        assert(memcmp(&expected, &object, sizeof(expected)) == 0);
        /* Selected value 5 becomes valid; its limit word is 10. */
        object.animationDataPointers[5] = expected.animationDataPointers[5] = 1;
        object.animationFrameCountArray[5] = expected.animationFrameCountArray[5] = 10;
        expected.unknown_198[slot].x = 2.5f;
        assert(SzModel_SetAnimSpeedParam(83, slot, 2.5f) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
        /* The second float is clamped to 1.0 .. (limit word - 1). */
        expected.unknown_198[slot].y = 1.0f;
        assert(SzModel_SetAnimFrameParam(83, slot, -3.0f) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
        expected.unknown_198[slot].y = 4.5f;
        assert(SzModel_SetAnimFrameParam(83, slot, 4.5f) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
        expected.unknown_198[slot].y = 9.0f;
        assert(SzModel_SetAnimFrameParam(83, slot, 100.0f) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
        assert(SzModel_GetAnimFrameParam(83, slot) == 9.0f);
    }
    /* The thunks use slot 0 and pass the float through. */
    reset();
    object.unknown_1e0[0] = 5;
    object.animationDataPointers[5] = 1;
    object.animationFrameCountArray[5] = 10;
    assert(SzModel_SetAnimSpeed(83, 7.0f) == 1 && object.unknown_198[0].x == 7.0f);
    assert(SzModel_SetAnimFrame(83, 3.0f) == 1 && object.unknown_198[0].y == 3.0f);
    assert(SzModel_GetAnimFrame(83) == 3.0f && SzModel_GetAnimFrame(7) == -1.0f);

    /* Limit word minus one; a negative value argument selects slot byte 0. */
    assert(SzModel_GetAnimFrameMax(83, 5) == 9 && SzModel_GetAnimFrameMax(83, -1) == 9);
    assert(SzModel_GetAnimFrameMax(83, 8) == -1 && SzModel_GetAnimFrameMax(83, 4) == -1);
    object.unknown_1e0[0] = -1;
    assert(SzModel_GetAnimFrameMax(83, -1) == -1);

    /* Second byte quad and two-bit field. */
    memset(&object, 0x7e, sizeof(object));
    memcpy(&expected, &object, sizeof(expected));
    for (value = -1; value < 6; value++) {
        if (value >= 0 && value < 4) expected.alphaMode.unknown_bits_2_3 = (unsigned char)value;
        expected.filters = quad;
        assert(SzModel_SetFilter(83, value, &quad) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
    }
    memset(&expected.filters, 0, sizeof(expected.filters));
    assert(SzModel_SetFilter(83, -1, 0) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);

    /* Bit 1 of byte +0x20d gates byte +0x200. */
    reset();
    object.stoneRatio = 0xc3;
    assert(SzModel_GetStoneFlag(83) == 0 && SzModel_GetStoneRatio(83) == -1);
    object.stateFlags.unknown_bit_1 = -1; /* signed one-bit field: set */
    assert(SzModel_GetStoneFlag(83) == 1 && SzModel_GetStoneRatio(83) == 0xc3);
}

typedef int (*IndexValue)(int, int);
typedef int (*IndexOnly)(int);

static void check_more_fields(void)
{
    struct DotrObject expected;
    struct DotrObjectVector4 in = {1.0f, -2.0f, 3.0f, -4.0f}, out;
    struct DotrObjectQuad quad = {10, 20, 30, 40}, quad_out;
    static const struct { IndexValue set; IndexOnly test; int byte; int bit; } bits[] = {
        {SzModel_SetDrawFlag, SzModel_GetDrawFlag, 0, 1}, {SzModel_SetCalcFlag, SzModel_GetCalcFlag, 0, 6},
        {SzModel_SetFogFlag, 0, 0, 7}, {SzModel_SetPauseFlag, SzModel_GetPauseFlag, 1, 0}, {SzModel_SetStoneFlag, 0, 1, 1},
        {SzModel_SetDeform, SzModel_GetDeform, 0, 4},
    };
    size_t i;
    int value, fill;
    for (i = 0; i < sizeof(bad_index) / sizeof(bad_index[0]); i++) {
        int index = bad_index[i];
        reset();
        assert(SzModel_SetPos(index, &in) == 0 && SzModel_SetRot(index, &in) == 0 && SzModel_SetRotC(index, &in) == 0);
        assert(SzModel_GetScale(index, &out) == 0 && SzModel_SetScale(index, &in) == 0);
        assert(SzModel_GetRgba(index, &quad_out) == -1 && SzModel_SetRgba(index, 1, &quad) == 0);
        assert(SzModel_GetAlpha(index) == -1 && SzModel_SetAlpha(index, 1) == 0);
        assert(SzModel_GetModelNo(index) == -1 && SzModel_GetModelType(index) == -1);
        assert(SzModel_SetDeform(index, 1) == 0 && SzModel_GetDeform(index) == 0 && SzModel_SetDeformRatio(index, 1) == 0);
        assert(SzModel_GetRefNum(index) == -1 && SzModel_InqRefID(index, 0) == -1);
        assert(SzModel_SetMaxOT(index, 1) == 0 && SzModel_GetMaxOT(index) == -1);
        assert(SzModel_SetDrawFlag(index, 1) == 0 && SzModel_GetDrawFlag(index) == 0 && SzModel_SetCalcFlag(index, 1) == 0);
        assert(SzModel_GetCalcFlag(index) == 0 && SzModel_SetFogFlag(index, 1) == 0 && SzModel_SetPauseFlag(index, 1) == 0);
        assert(SzModel_GetPauseFlag(index) == 0 && SzModel_SetStoneFlag(index, 1) == 0);
    }
    reset();
    memset(&object, 0x7e, sizeof(object));
    memcpy(&expected, &object, sizeof(expected));
    /* Vector stores; a null source keeps +0, zeroes +0x10/+0x20, and writes
     * ones to +0x30. */
    assert(SzModel_SetPos(83, 0) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
    expected.pos = in;
    assert(SzModel_SetPos(83, &in) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
    expected.rot = in;
    assert(SzModel_SetRot(83, &in) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
    expected.rotC = in;
    assert(SzModel_SetRotC(83, &in) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
    expected.scale = in;
    assert(SzModel_SetScale(83, &in) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
    assert(SzModel_GetScale(83, 0) == 1);
    assert(SzModel_GetRot(83, 0) == 1 && SzModel_GetRotC(83, 0) == 1);
    assert(SzModel_GetRot(83, &out) == 1 && memcmp(&out, &in, sizeof(in)) == 0);
    memset(&out, 0, sizeof(out));
    assert(SzModel_GetRotC(83, &out) == 1 && memcmp(&out, &in, sizeof(in)) == 0);
    assert(SzModel_GetRot(7, &out) == 0 && SzModel_GetRotC(84, &out) == 0);
    assert(SzModel_GetScale(83, &out) == 1 && memcmp(&out, &in, sizeof(in)) == 0);
    memset(&expected.rot, 0, sizeof(expected.rot));
    assert(SzModel_SetRot(83, 0) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
    memset(&expected.rotC, 0, sizeof(expected.rotC));
    assert(SzModel_SetRotC(83, 0) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
    expected.scale.x = expected.scale.y = expected.scale.z = expected.scale.unknown_w = 1.0f;
    assert(SzModel_SetScale(83, 0) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);

    /* Four bytes at +0x12c with the two-bit field at +0x1f8. */
    for (value = -1; value < 6; value++) {
        if (value >= 0 && value < 4) expected.alphaMode.unknown_bits_0_1 = (unsigned char)value;
        expected.rgba = quad;
        assert(SzModel_SetRgba(83, value, &quad) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
        assert(SzModel_GetRgba(83, &quad_out) == expected.alphaMode.unknown_bits_0_1);
        assert(memcmp(&quad_out, &quad, sizeof(quad)) == 0);
        assert(SzModel_GetRgba(83, 0) == expected.alphaMode.unknown_bits_0_1);
    }
    memset(&expected.rgba, 128, sizeof(expected.rgba));
    assert(SzModel_SetRgba(83, -1, 0) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
    expected.rgba.unknown_3 = 0x5c;
    assert(SzModel_SetAlpha(83, 0x15c) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
    assert(SzModel_GetAlpha(83) == 0x5c);

    /* Plain fields. */
    object.modelNo = expected.modelNo = -1234;
    object.type = expected.type = -5;
    assert(SzModel_GetModelNo(83) == -1234 && SzModel_GetModelType(83) == -5);
    expected.deformRatio = 0xa7;
    assert(SzModel_SetDeformRatio(83, 0x2a7) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
    expected.maxOT = 0xfedc;
    assert(SzModel_SetMaxOT(83, 0x1fedc) == 1 && memcmp(&expected, &object, sizeof(expected)) == 0);
    assert(SzModel_GetMaxOT(83) == 0xfedc);
    for (value = 0; value < 8; value++) {
        object.boneRefIds[value] = expected.boneRefIds[value] = (unsigned short)(0xff00 + value);
        assert(SzModel_InqRefID(83, value) == 0xff00 + value);
    }
    assert(SzModel_InqRefID(83, -1) == -1 && SzModel_InqRefID(83, 8) == -1);
    /* Byte +0x1fe is returned only while signed byte +0x1f2 is non-negative. */
    object.refBoneNum = expected.refBoneNum = 0xee;
    object.modelId = expected.modelId = 0;
    assert(SzModel_GetRefNum(83) == 0xee);
    object.modelId = expected.modelId = -128;
    assert(SzModel_GetRefNum(83) == -1);

    /* Single-bit setters and tests at +0x20c and +0x20d. */
    for (i = 0; i < sizeof(bits) / sizeof(bits[0]); i++) {
        for (fill = 0; fill < 2; fill++) {
            for (value = 0; value < 2; value++) {
                struct DotrObjectBits *field = bits[i].byte ? &expected.stateFlags : &expected.drawFlag;
                memset(&object.drawFlag, fill ? 0xff : 0, 1);
                memset(&object.stateFlags, fill ? 0xff : 0, 1);
                memcpy(&expected, &object, sizeof(expected));
                switch (bits[i].bit) {
                case 0: field->unknown_bit_0 = value; break;
                case 1: field->unknown_bit_1 = value; break;
                case 4: field->unknown_bit_4 = value; break;
                case 6: field->unknown_bit_6 = value; break;
                default: field->unknown_bit_7 = value; break;
                }
                assert(bits[i].set(83, value) == 1);
                assert(memcmp(&expected, &object, sizeof(expected)) == 0);
                if (bits[i].test) assert(bits[i].test(83) == value);
            }
        }
    }
}

static void check_sweeps(void)
{
    static struct DotrObject objects[4];
    struct DotrObjectVector3 in = {2.0f, 4.0f, 6.0f};
    int i;
    memset(gSzModel_ahModel, 0, sizeof(gSzModel_ahModel));
    memset(objects, 0, sizeof(objects));
    /* Entries 0, 40, 41 and 83 hold kinds 4, 5, 4 and -1. */
    gSzModel_ahModel[0] = &objects[0];
    gSzModel_ahModel[40] = &objects[1];
    gSzModel_ahModel[41] = &objects[2];
    gSzModel_ahModel[83] = &objects[3];
    objects[0].type = 4;
    objects[1].type = 5;
    objects[2].type = 4;
    objects[3].type = -1;
    SzEffect_SetIntensity(4, &in);
    assert(memcmp(&objects[0].intensity, &in, sizeof(in)) == 0);
    assert(memcmp(&objects[2].intensity, &in, sizeof(in)) == 0);
    assert(objects[1].intensity.x == 0.0f && objects[3].intensity.x == 0.0f);
    SzEffect_SetIntensityOnly(5, -3.0f);
    assert(objects[1].intensity.z == -3.0f && objects[0].intensity.z == 6.0f);
    assert(objects[2].intensity.z == 6.0f && objects[3].intensity.z == 0.0f);
    /* A negative kind selects every entry; null entries are skipped by the
     * called routines. A null vector stores the defaults. */
    SzEffect_SetIntensityOnly(-1, 9.0f);
    for (i = 0; i < 4; i++) assert(objects[i].intensity.z == 9.0f);
    SzEffect_SetIntensity(INT_MIN, 0);
    for (i = 0; i < 4; i++) {
        assert(objects[i].intensity.x == 0.8f && objects[i].intensity.y == 0.8f && objects[i].intensity.z == 1.0f);
    }
    /* Kind -1 as stored in an object is not selectable: a negative argument
     * means "all". No kind matches 6 here. */
    SzEffect_SetIntensityOnly(6, 1.5f);
    for (i = 0; i < 4; i++) assert(objects[i].intensity.z == 1.0f);
}

struct DotrObjectEntry112 gSzModel_aBlur[32];
int gSzEvent_nSave[1], gSzModel_nBlurMode[1], gSzModel_fPacking[1];
char data_002cf9d0[5] = "abcd", data_002cf9d5[5] = "efgh";

static struct DotrObject objects[84];
static int fan_calls, fan_args[3], label_mask, live_result, live_argument, free_calls;
static void *freed;
static int inner_calls, inner_args[7], inner_flag, chain_args[7], chain_local;
static DotrObjectWord chain_result = 0x77;
static int chain_calls;
static void *inner_pointer;

/* Recording stubs with assumed semantics. */
void SzView_SetStraightLight(int unknown, int index, int value)
{
    fan_calls++;
    fan_args[0] = unknown;
    fan_args[1] = index;
    fan_args[2] = value;
}

char *func_00197548(const char *label)
{
    static char found[1];
    if (label == data_002cf9d0) return (label_mask & 1) ? found : 0;
    assert(label == data_002cf9d5);
    return (label_mask & 2) ? found : 0;
}

int lSzModel_FreeModel(int index)
{
    live_argument = index;
    return live_result;
}

void SzMem_Free(void *pointer)
{
    free_calls++;
    freed = pointer;
}

static int inner_log_index[16], inner_vector[4];
static void *inner_log_pointer[16];
struct DotrObjectVector16 data_0029abb0 = {{0x11, 0x22, 0x33, 0x44}};
static struct DotrObjectVector16 forwarded_vector;
short data_01ed53f4;
struct DotrObjectBank dotr_object_bank_01ed0000;
int SzModel_Draw_Pipe(int cursor, int index, int c, void *d, int e, int f, int g)
{
    inner_log_index[inner_calls & 15] = index;
    inner_log_pointer[inner_calls & 15] = d;
    if (d != 0) {
        memcpy(inner_vector, d, sizeof(inner_vector));
    }
    inner_calls++;
    inner_args[0] = cursor;
    inner_args[1] = index;
    inner_args[2] = c;
    inner_pointer = d;
    inner_args[4] = e;
    inner_args[5] = f;
    inner_args[6] = g;
    inner_flag = SzModel_GetCalcFlag(index);
    return cursor + 1;
}

DotrObjectWord lSzModel_TraceGNode(int a, int b, int c, int *d, int e, int *f, int g)
{
    chain_calls++;
    chain_args[0] = a;
    chain_args[1] = b;
    chain_args[2] = c;
    chain_local = *d;
    chain_args[4] = e;
    chain_args[5] = f != 0;
    chain_args[6] = g;
    return chain_result;
}

static void check_table_control(void)
{
    struct DotrObjectEntry112 expected[32];
    int i, mask, flag;

    /* 0x1e77e0 */
    for (i = 0; i < 84; i++) {
        memset(&objects[i], 0xff, sizeof(objects[i]));
        gSzModel_ahModel[i] = &objects[i];
    }
    gSzEvent_nSave[0] = 4;
    fan_calls = 0;
    SzEvent_Init(9);
    assert(fan_calls == 1 && fan_args[0] == 0 && fan_args[1] == 9 && fan_args[2] == 1);
    assert(gSzEvent_nSave[0] == 9);
    for (i = 0; i < 84; i++) {
        int want = (i >= 30 && i < 32) || (i >= 34 && i < 74);
        assert((objects[i].drawFlag.unknown_bit_1 != 0) == want);
        assert(objects[i].drawFlag.unknown_bit_0 != 0 && objects[i].drawFlag.unknown_bit_2 != 0);
    }

    /* 0x1e7a60 */
    for (mask = 0; mask < 4; mask++) {
        memset(gSzModel_aBlur, 0, sizeof(gSzModel_aBlur));
        memset(expected, 0, sizeof(expected));
        for (i = 0; i < 32; i++) {
            gSzModel_aBlur[i].unknown_68_bits_27_31 = 5;
            expected[i].unknown_68_bits_27_31 = 5;
            expected[i].unknown_68_bits_0_26 = -1;
        }
        gSzModel_nBlurMode[0] = 3;
        gSzModel_fPacking[0] = 8;
        label_mask = mask;
        assert(SzModel_Init() == 1);
        for (i = 0; i < 84; i++) {
            assert(gSzModel_ahModel[i] == 0);
        }
        assert(memcmp(gSzModel_aBlur, expected, sizeof(expected)) == 0);
        assert(gSzModel_nBlurMode[0] == 0 && gSzModel_fPacking[0] == (mask ? 1 : 8));
    }

    /* 0x1e7de0 */
    reset();
    free_calls = 0;
    live_result = 1;
    for (i = 0; i < (int)(sizeof(bad_index) / sizeof(bad_index[0])); i++) {
        assert(SzModel_Destroy(bad_index[i]) == 0);
    }
    live_result = 0;
    assert(SzModel_Destroy(83) == 0 && live_argument == 83 && free_calls == 0 && gSzModel_ahModel[83] == &object);
    live_result = 5;
    assert(SzModel_Destroy(83) == 1 && free_calls == 1 && freed == &object && gSzModel_ahModel[83] == 0);

    /* 0x1edf20: entry 0 is never visited; negative fields stay. */
    for (flag = 0; flag < 2; flag++) {
        memset(gSzModel_aBlur, 0, sizeof(gSzModel_aBlur));
        for (i = 0; i < 32; i++) {
            gSzModel_aBlur[i].unknown_68_bits_0_26 = i < 4 ? 6 : i == 4 ? -2 : i + 100;
            gSzModel_aBlur[i].unknown_68_bits_27_31 = 3;
        }
        memcpy(expected, gSzModel_aBlur, sizeof(expected));
        SzModel_CancelMotionBlur(flag ? -1 : 6);
        for (i = 1; i < 32; i++) {
            if (i < 4 || (flag && i != 4)) expected[i].unknown_68_bits_0_26 = -1;
        }
        assert(memcmp(gSzModel_aBlur, expected, sizeof(expected)) == 0);
    }

    /* 0x1efa80: the flag is clear during the inner call and restored after. The pointer is
     * forwarded untouched; the stub reads a 16-byte vector through it, so pass one. */
    for (flag = 0; flag < 2; flag++) {
        reset();
        object.drawFlag.unknown_bit_6 = flag ? -1 : 0;
        inner_calls = 0;
        assert(SzModel_DrawWithOutCalc_Pipe(0x500, 83, 1, &forwarded_vector, 3, 4, 5) == 0x501 && inner_calls == 1);
        assert(inner_args[0] == 0x500 && inner_args[1] == 83 && inner_args[2] == 1 && inner_pointer == &forwarded_vector);
        assert(inner_args[4] == 3 && inner_args[5] == 4 && inner_args[6] == 5 && inner_flag == 0);
        assert(SzModel_GetCalcFlag(83) == flag);
    }

    /* 0x1efb40 */
    reset();
    flag = 0;
    for (i = 0; i < (int)(sizeof(bad_index) / sizeof(bad_index[0])); i++) {
        assert(lSzModel_GetGNode(bad_index[i], 2, &flag) == 0 && flag == 0);
    }
    object.GNode = 0x4321;
    chain_local = -1;
    assert(lSzModel_GetGNode(83, 2, &flag) == 0x77 && flag == 1 && chain_local == 0);
    assert(chain_args[0] == 2 && chain_args[1] == 0x4321 && chain_args[2] == 0x4321);
    assert(chain_args[4] == 0 && chain_args[5] == 1 && chain_args[6] == 0);
    assert(lSzModel_GetGNode(83, 3, 0) == 0x77 && chain_args[0] == 3 && chain_args[5] == 0);
}

static int wait_calls, wait_remaining, submit_calls, submit_args[4], side_calls, side_args[2];
static int lookup_result, lookup_args[2], notify_calls, notify_args[6];
static char *submit_name;
static char submit_names[12][64];
static int submit_words[12];
static DotrResourceSubmitCallback submit_callbacks[12];
static DotrResourceSubmitCallback submit_callback;
static struct DotrResourceQueueRecord *submit_result;

void lSzModel_LoadModelCB(struct DotrResourceSubmitRecord *record, int completed)
{
    (void)record;
    (void)completed;
}

int SzFile_DoRequestS(void)
{
    wait_calls++;
    if (--wait_remaining == 0) {
        object.drawFlag.unknown_bit_3 = 0;
    }
    return 0;
}

struct DotrResourceQueueRecord *SzFile_ReadRequest(int type, char *name, int unknown_014, int byte_size,
                                              DotrResourceSubmitCallback callback, int unknown_01c)
{
    submit_calls++;
    submit_args[0] = type;
    submit_name = name;
    submit_args[1] = unknown_014;
    submit_args[2] = byte_size;
    submit_callback = callback;
    submit_args[3] = unknown_01c;
    strncpy(submit_names[(submit_calls - 1) % 12], name, 63);
    submit_words[(submit_calls - 1) % 12] = unknown_01c;
    submit_callbacks[(submit_calls - 1) % 12] = callback;
    return submit_result;
}


static int param_calls, param_args[7];
static char param_name[64];

struct DotrResourceQueueRecord *SzFile_ReadRequestParam(int type, int priority, char *name, int unknown_014,
                                              int byte_size, int byte_offset, int unknown_010,
                                              DotrResourceSubmitCallback callback, int unknown_01c)
{
    param_calls++;
    strncpy(param_name, name, 63);
    param_args[0] = type;
    param_args[1] = priority;
    submit_name = name;
    param_args[2] = unknown_014;
    param_args[3] = byte_size;
    param_args[4] = byte_offset;
    param_args[5] = unknown_010;
    submit_callback = callback;
    param_args[6] = unknown_01c;
    return submit_result;
}

/* SzModel_LoadMerge's integer-word view of SzFile_ReadRequestParam (object_table.h): recorded like the
 * pointer view above; the name and callback arrive as address words. */
static DotrMergeAddr merge_callback;

int func_001fecb0_merge(int type, int priority, DotrMergeAddr name, int addr, int size, int offset, int size2,
                        DotrMergeAddr callback, int index)
{
    param_calls++;
    strncpy(param_name, (char *)name, 63);
    param_args[0] = type;
    param_args[1] = priority;
    param_args[2] = addr;
    param_args[3] = size;
    param_args[4] = offset;
    param_args[5] = size2;
    merge_callback = callback;
    param_args[6] = index;
    return submit_result != 0;
}

int SzBattle_InqEntry(int index)
{
    assert(side_calls < 2);
    side_args[side_calls++] = index;
    return 0x0203;
}

int SzDuel_GetUnitModelID(int a, int b)
{
    lookup_args[0] = a;
    lookup_args[1] = b;
    return lookup_result;
}

void SzView_ReqCamera(int a, int b, int c, int d, int index, int unknown)
{
    notify_calls++;
    notify_args[0] = a;
    notify_args[1] = b;
    notify_args[2] = c;
    notify_args[3] = d;
    notify_args[4] = index;
    notify_args[5] = unknown;
}

static void check_slot_state(void)
{
    static const unsigned counts[] = {10u, 0x80000001u};
    struct DotrObject expected;
    struct DotrObjectVector4 out4;
    int pair, mode, i;
    size_t c;

    /* 0x1f1560 and 0x1f1830 */
    for (mode = 0; mode < 2; mode++) {
        reset();
        memset(&object, 0x11, sizeof(object));
        object.animationDataPointers[3] = 0;
        expected = object;
        for (i = 0; i < (int)(sizeof(bad_index) / sizeof(bad_index[0])); i++) {
            assert((mode ? SzModel_SetAnimDirect(bad_index[i], 3) : SzModel_SetAnimIntrParam(bad_index[i], 3, 9)) == 0);
        }
        assert((mode ? SzModel_SetAnimDirect(83, -1) : SzModel_SetAnimIntrParam(83, -1, 9)) == 0);
        assert((mode ? SzModel_SetAnimDirect(83, 8) : SzModel_SetAnimIntrParam(83, 8, 9)) == 0);
        assert((mode ? SzModel_SetAnimDirect(83, 3) : SzModel_SetAnimIntrParam(83, 3, 9)) == 0);
        assert(memcmp(&object, &expected, sizeof(expected)) == 0);
        for (pair = -2; pair < 2; pair++) {
            for (c = 0; c < sizeof(counts) / sizeof(counts[0]); c++) {
                float f = (float)counts[c];
                memset(&object, 0x11, sizeof(object));
                object.animationDataPointers[3] = 0x1234;
                object.animationFrameCountArray[3] = counts[c];
                object.unknown_198[0].x = 2.0f;
                object.unknown_198[0].y = 3.0f;
                object.unknown_198[0].z = 4.0f;
                object.unknown_1e0[0] = 5;
                object.animDir.unknown_bits_6_7 = pair;
                expected = object;
                expected.unknown_198[0].y = pair == -1 ? f - 1.0f : 1.0f;
                expected.unknown_1e0[0] = 3;
                expected.motionDelay = 0;
                if (mode) {
                    expected.unknown_198[0].z = 1.0f;
                    expected.unknown_1e0[1] = -1;
                    assert(SzModel_SetAnimDirect(83, 3) == 1);
                } else {
                    expected.unknown_198[1] = object.unknown_198[0];
                    expected.unknown_198[1].z = 1.0f;
                    expected.unknown_198[0].z = 0.0f;
                    expected.unknown_1e0[1] = 5;
                    expected.blendTickTotal = 0x77;
                    assert(SzModel_SetAnimIntrParam(83, 3, 0x177) == 1);
                }
                assert(memcmp(&object, &expected, sizeof(expected)) == 0);
            }
        }
    }

    /* 0x1f0c10 */
    reset();
    memset(&object, 0x11, sizeof(object));
    object.animationDataPointers[7] = 0;
    object.animationDataPointers[0] = 0x1234;
    expected = object;
    free_calls = 0;
    assert(lSzModel_FreeMotion(7, 0) == 0 && lSzModel_FreeMotion(83, -1) == 0 && lSzModel_FreeMotion(83, 8) == 0);
    assert(lSzModel_FreeMotion(83, 7) == 1 && free_calls == 0 && memcmp(&object, &expected, sizeof(expected)) == 0);
    assert(lSzModel_FreeMotion(83, 0) == 1 && free_calls == 1 && freed == (void *)(DotrObjectWord)0x1234);
    expected.animationDataPointers[0] = 0;
    expected.animationFrameCountArray[0] = 1;
    assert(memcmp(&object, &expected, sizeof(expected)) == 0);

    /* 0x1e7d00, which runs the real 0x1f2c60 reset path. */
    reset();
    assert(lSzModel_Clean(7) == 0);
    memset(&object, 0x5a, sizeof(object));
    expected = object;
    assert(lSzModel_Clean(83) == 1);
    expected.unknown_198[0].x = expected.unknown_198[0].y = expected.unknown_198[0].z = 1.0f;
    expected.unknown_198[1].x = expected.unknown_198[1].y = 1.0f;
    expected.unknown_198[1].z = 0.0f;
    expected.unknown_1e0[1] = -1;
    expected.unknown_1e0[0] = 0;
    expected.stateFlags.unknown_bit_1 = 0;
    expected.stoneRatio = 0;
    expected.maxOT = 0;
    expected.blendTickTotal = 6;
    expected.blendTickCurrent = 0;
    expected.motionDelay = 0;
    expected.drawFlag.unknown_bit_5 = 0;
    expected.coordIndex = 32767;
    expected.boneRefSlotCursor = 1;
    for (i = 0; i < 8; i++) {
        expected.boneRefIds[i] = 32767;
    }
    assert(memcmp(&object, &expected, sizeof(expected)) == 0);

    /* 0x1e7e70 waits while bit 3 is set. */
    reset();
    assert(SzModel_Preset(7, 1, 2) == 0);
    for (mode = 0; mode < 2; mode++) {
        memset(&object, 0, sizeof(object));
        object.type = 0x44;
        object.drawFlag.unknown_bit_3 = mode ? -1 : 0;
        expected = object;
        wait_calls = 0;
        wait_remaining = 3;
        live_argument = -1;
        assert(SzModel_Preset(83, mode ? -1 : 0x105, 0x12345) == 1);
        assert(wait_calls == (mode ? 3 : 0) && live_argument == 83);
        expected.drawFlag.unknown_bit_3 = 0;
        expected.modelId = -1;
        if (!mode) expected.type = 5;
        expected.modelNo = 0x2345;
        expected.drawFlag.unknown_bit_2 = -1;
        assert(memcmp(&object, &expected, sizeof(expected)) == 0);
    }

    /* 0x1eff70 */
    {
        static char name[] = "name";
        static struct DotrResourceQueueRecord *const some = (struct DotrResourceQueueRecord *)name;
        reset();
        submit_calls = 0;
        assert(lSzModel_LoadModel(7, name) == 0);
        object.drawFlag.unknown_bit_3 = -1;
        assert(lSzModel_LoadModel(83, name) == 0 && submit_calls == 0);
        for (mode = 0; mode < 2; mode++) {
            memset(&object, 0, sizeof(object));
            expected = object;
            submit_result = mode ? some : 0;
            submit_calls = 0;
            live_argument = -1;
            assert(lSzModel_LoadModel(83, name) == 1 && submit_calls == 1 && live_argument == 83);
            assert(submit_args[0] == 83 && submit_name == name && submit_args[1] == 0 && submit_args[2] == -1);
            assert(submit_callback == lSzModel_LoadModelCB && submit_args[3] == 83);
            expected.drawFlag.unknown_bit_0 = -1;
            if (mode) expected.drawFlag.unknown_bit_3 = -1;
            assert(memcmp(&object, &expected, sizeof(expected)) == 0);
        }
    }

    /* 0x1f0060 */
    {
        static char name[] = "pack";
        static struct DotrResourceQueueRecord *const some = (struct DotrResourceQueueRecord *)name;
        static const int expect[7] = {83, 0, 0, 22, 33, 44, 83};
        reset();
        param_calls = 0;
        assert(lSzModel_LoadModelPack(-1, name, 0, 22, 33, 44) == 0);
        assert(lSzModel_LoadModelPack(84, name, 0, 22, 33, 44) == 0);
        assert(lSzModel_LoadModelPack(7, name, 0, 22, 33, 44) == 0);
        object.drawFlag.unknown_bit_3 = -1;
        assert(lSzModel_LoadModelPack(83, name, 0, 22, 33, 44) == 0 && param_calls == 0);
        /* bit 0 of mode: a record comes back; bit 1: non-zero third argument clears flag bit 0 */
        for (mode = 0; mode < 4; mode++) {
            memset(&object, 0, sizeof(object));
            object.drawFlag.unknown_bit_0 = (mode & 2) ? -1 : 0;
            expected = object;
            submit_result = (mode & 1) ? some : 0;
            param_calls = 0;
            live_argument = -1;
            assert(lSzModel_LoadModelPack(83, name, (mode & 2) ? 11 : 0, 22, 33, 44) == 1);
            assert(param_calls == 1 && live_argument == 83 && submit_name == name);
            for (i = 0; i < 7; i++) assert(param_args[i] == (i == 2 && (mode & 2) ? 11 : expect[i]));
            assert(submit_callback == lSzModel_LoadModelPackCB);
            expected.drawFlag.unknown_bit_0 = (mode & 2) ? 0 : -1;
            if (mode & 1) expected.drawFlag.unknown_bit_3 = -1;
            assert(memcmp(&object, &expected, sizeof(expected)) == 0);
        }
    }

    /* 0x1f0e50 */
    reset();
    for (mode = 0; mode < 4; mode++) {
        object.pCameraMotion2 = (mode & 1) ? 0x4242 : 0;
        side_calls = notify_calls = 0;
        lookup_result = 84;
        assert(SzModel_BattleSeq2(1, 1, 5) == 0 && side_calls == 2 && side_args[0] == 1 && side_args[1] == 0);
        assert(lookup_args[0] == 2 && lookup_args[1] == 3 && notify_calls == 0);
        side_calls = 0;
        lookup_result = 7;
        assert(SzModel_BattleSeq2(0, 1, 5) == 0 && notify_calls == 0);
        side_calls = 0;
        lookup_result = 83;
        assert(SzModel_BattleSeq2(0, 0, 5) == 1 && notify_calls == 0);
        side_calls = 0;
        assert(SzModel_BattleSeq2(0, 1, (mode & 2) ? 5 : 0) == 1 && notify_calls == (mode & 1));
        if (mode & 1) {
            assert(notify_args[0] == 0 && notify_args[1] == 1 && notify_args[2] == ((mode & 2) ? 9 : 8));
            assert(notify_args[3] == 0 && notify_args[4] == 83 && notify_args[5] == 0x4242);
        }
    }

    /* 0x1f2050 and 0x1f2ec0 */
    reset();
    for (i = 0; i < 4; i++) {
        (&object.center.x)[i] = (float)(i + 1);
        (&object.boneRefWorldPos[6].x)[i] = (float)(i + 20);
    }
    object.boneRefIds[6] = 0x8000;
    for (mode = 0; mode < 8; mode++) {
        int ok = mode == 7;
        object.modelId = (mode & 1) ? 0 : -1;
        object.refBoneNum = (mode & 2) ? 1 : 0;
        object.drawFlag.unknown_bit_5 = (mode & 4) ? -1 : 0;
        memset(&out4, 0, sizeof(out4));
        assert(SzModel_GetCenter(83, &out4) == ok && SzModel_GetCenter(83, 0) == ok && SzModel_GetCenter(7, &out4) == 0);
        assert((memcmp(&out4, &object.center, sizeof(out4)) == 0) == ok);
        memset(&out4, 0, sizeof(out4));
        ok = (mode & 5) == 5;
        assert(SzModel_GetRef(83, 6, &out4) == ok && SzModel_GetRef(7, 6, &out4) == 0);
        assert((memcmp(&out4, &object.boneRefWorldPos[6], sizeof(out4)) == 0) == ok);
        assert(SzModel_GetRef(83, 5, &out4) == 0 && SzModel_GetRef(83, -1, &out4) == 0 && SzModel_GetRef(83, 8, &out4) == 0);
    }

    /* 0x1f2dd0 */
    reset();
    object.refBoneNum = 4;
    for (i = 0; i < 8; i++) {
        object.boneRefIds[i] = 32767;
    }
    expected = object;
    assert(SzModel_SetRefID_RefNo(7, 1, 0) == 0 && SzModel_SetRefID_RefNo(83, 1, -1) == 0 && SzModel_SetRefID_RefNo(83, 1, 8) == 0);
    assert(SzModel_SetRefID_RefNo(83, 0x0204, 2) == 0 && memcmp(&object, &expected, sizeof(expected)) == 0);
    assert(SzModel_SetRefID_RefNo(83, 0x0203, 2) == 1 && object.boneRefIds[2] == 0x0203);
    assert(SzModel_SetRefID_RefNo(83, 0x0301, 2) == 0 && object.boneRefIds[2] == 0x0203);
    assert(SzModel_SetRefID_RefNo(83, 0x8201, 2) == 1 && object.boneRefIds[2] == 0x8201);
    assert(SzModel_SetRefID_RefNo(83, 0x0301, 2) == 0 && SzModel_SetRefID_RefNo(83, -5, 2) == 1 && object.boneRefIds[2] == 32767);
    assert(SzModel_SetRefID_RefNo(83, 0xffff, 3) == 0 && SzModel_SetRefID_RefNo(83, 0x7f03, 3) == 1 && object.boneRefIds[3] == 0x7f03);

    /* 0x1f2c60 */
    reset();
    object.refBoneNum = 4;
    for (i = 0; i < 8; i++) {
        object.boneRefIds[i] = (unsigned short)(i < 4 ? 0x0200 + (i & 3) : 0x0300);
    }
    object.boneRefSlotCursor = 8;
    assert(SzModel_SetRefID(7, 1, 1) == -1 && SzModel_SetRefID(83, 0x0201, 0) == -1);
    object.boneRefSlotCursor = 5;
    assert(SzModel_SetRefID(83, 0x0304, 0) == -1 && SzModel_SetRefID(83, 0x0201, 0) == -1 && object.boneRefSlotCursor == 5);
    assert(SzModel_SetRefID(83, 0x0301, 0) == 5 && object.boneRefSlotCursor == 6 && object.boneRefIds[5] == 0x0301);
    assert(SzModel_SetRefID(83, -1, 0) == 6 && object.boneRefSlotCursor == 7 && object.boneRefIds[6] == 32767);
    /* A restart with a value clears only entries of the same kind. */
    assert(SzModel_SetRefID(83, 0x0203, 1) == 0 && object.boneRefSlotCursor == 1 && object.boneRefIds[0] == 0x0203);
    for (i = 1; i < 8; i++) {
        assert(object.boneRefIds[i] == (i < 4 ? 32767 : i == 5 ? 0x0301 : i == 6 ? 32767 : 0x0300));
    }
    assert(SzModel_SetRefID(83, -1, 1) == 0 && object.boneRefSlotCursor == 1);
    for (i = 0; i < 8; i++) {
        assert(object.boneRefIds[i] == 32767);
    }
}

struct DotrInitView gSzGame_gl_init;
int data_003173b0[16];

static void check_init_all(void)
{
    static struct DotrObject objects[84];
    int i, mode;
    for (mode = -1; mode <= 3; mode++) {
        memset(objects, 0, sizeof(objects));
        memset(&gSzGame_gl_init, 0x11, sizeof(gSzGame_gl_init));
        memset(data_003173b0, 0x22, sizeof(data_003173b0));
        for (i = 0; i < 84; i++) {
            gSzModel_ahModel[i] = &objects[i];
        }
        gSzModel_ahModel[40] = 0; /* a missing object is skipped */
        objects[3].drawFlag.unknown_bit_1 = -1;
        lSzGame_InitTitle(mode);
        for (i = 0; i < 84; i++) {
            int expected = (i >= 34 && i < 74 && i != 40) ? 1 : 0;
            assert((objects[i].drawFlag.unknown_bit_1 != 0) == expected);
        }
        if (mode < 2) {
            assert(data_003173b0[0] == 2 && data_003173b0[1] == 0x22222222);
            assert(gSzGame_gl_init.byte_224 == 0 && gSzGame_gl_init.byte_225 == 0);
            assert(gSzGame_gl_init.byte_226 == 0 && gSzGame_gl_init.byte_227 == 0x80);
            assert(gSzGame_gl_init.bits_22c.lo == 1 && gSzGame_gl_init.bits_22f.bit1 != 0);
            assert(gSzGame_gl_init.word_008 == 0 && gSzGame_gl_init.word_004 == 0x11111111);
        } else {
            assert(data_003173b0[0] == 0x22222222 && gSzGame_gl_init.byte_227 == 0x11);
            assert(gSzGame_gl_init.word_008 == 0x11111111);
        }
    }
}

static void *pack_calls[8];
static int pack_ncalls;

void RhText_BindAddress(void *header)
{
    pack_calls[pack_ncalls++] = header;
}

static void check_pack_table(void)
{
    static struct {
        int count, pad[3];
        struct DotrObjPackEntry entries[3];
    } table;
    static struct DotrObjPackDesc desc;
    static struct DotrObjPackView view;
    static const int bad[] = {INT_MIN, -1, 84, INT_MAX};
    unsigned i;

    for (i = 0; i < sizeof(bad) / sizeof(bad[0]); i++) {
        assert(SzModel_BindTextureFile(bad[i]) == 0);
    }
    memset(gSzModel_ahModel, 0, sizeof(gSzModel_ahModel));
    assert(SzModel_BindTextureFile(5) == 0); /* null slot */

    gSzModel_ahModel[5] = (struct DotrObject *)&view;
    assert(SzModel_BindTextureFile(5) == 1); /* base zero */

    view.base = 0x1000;
    view.desc = &desc;
    assert(SzModel_BindTextureFile(5) == 1); /* desc +0x2c zero */
    assert(pack_ncalls == 0);

    memset(&table, 0, sizeof(table));
    table.count = 3;
    table.entries[0].a = 0x10; table.entries[0].b = 7; table.entries[0].c = 0x20; table.entries[0].d = 9;
    table.entries[1].b = 8; table.entries[1].c = 0x30; table.entries[1].d = 4; /* a zero: untouched */
    table.entries[2].a = 0x40; table.entries[2].d = 5;
    view.base = 0;
    view.base = (DotrObjectWord)(intptr_t)&table;
    desc.unknown_28 = 0;
    desc.unknown_2c = 1;
    /* entries hold offsets from base, so rebase test values onto table address */
    assert(SzModel_BindTextureFile(5) == 1);
    assert(pack_ncalls == 2);
    assert(table.entries[0].a == view.base + 0x10 && pack_calls[0] == (void *)(intptr_t)table.entries[0].a);
    assert(table.entries[0].b == 7 && table.entries[0].c == view.base + 0x20 && table.entries[0].d == 0);
    assert(table.entries[1].a == 0 && table.entries[1].b == 8);
    assert(table.entries[1].c == 0x30 && table.entries[1].d == 4);
    assert(table.entries[2].a == view.base + 0x40 && pack_calls[1] == (void *)(intptr_t)table.entries[2].a);
    assert(table.entries[2].c == 0 && table.entries[2].d == 0);
}

static struct DotrSlotHead *rebase_arg;
static int rebase_calls;

int NjaReloc_Adjust_M3DMOT(struct DotrSlotHead *head)
{
    rebase_arg = head;
    rebase_calls++;
    return 0;
}

static void check_submit_object(void)
{
    int t, s;
    static struct DotrObject obj;

    /* range guards: every rejection leaves the stubs untouched */
    memset(gSzModel_ahModel, 0, sizeof(gSzModel_ahModel));
    gSzModel_ahModel[0] = gSzModel_ahModel[83] = &obj;
    submit_calls = 0;
    assert(lSzModel_LoadMotion(-1, 0, "n") == 0 && lSzModel_LoadMotion(84, 0, "n") == 0);
    assert(lSzModel_LoadMotion(0, -1, "n") == 0 && lSzModel_LoadMotion(0, 8, "n") == 0);
    assert(lSzModel_LoadMotion(5, 0, "n") == 0); /* null entry */
    assert(submit_calls == 0);

    /* boundaries accepted: (type<<8)|slot is the callback cookie */
    for (t = 0; t <= 83; t += 83) {
        for (s = 0; s <= 7; s += 7) {
            submit_calls = 0;
            assert(lSzModel_LoadMotion(t, s, "name") == 1 && submit_calls == 1);
            assert(submit_args[0] == t && submit_args[1] == 0 && submit_args[2] == -1);
            assert(submit_name != 0 && strcmp(submit_name, "name") == 0);
            assert(submit_callback == lSzModel_LoadMotionCB && submit_args[3] == ((t << 8) | s));
        }
    }
}

static void check_submit_callback(void)
{
    /* head and its first word share one low (32-bit addressable) page */
    unsigned char *page = mmap(0, 4096, PROT_READ | PROT_WRITE,
                               MAP_PRIVATE | MAP_ANONYMOUS | MAP_32BIT, -1, 0);
    struct DotrSlotHead *head = (struct DotrSlotHead *)page;
    int *words = (int *)(page + 256);
    struct DotrResourceSlots obj;
    struct DotrResourceSubmitRecord rec;

    assert(page != MAP_FAILED);
    memset(&obj, 0, sizeof(obj));
    memset(&rec, 0, sizeof(rec));
    words[1] = 0x1234;
    head->first = (int)(intptr_t)words;
    rec.unknown_014 = (int)(intptr_t)head;
    memset(gSzModel_ahModel_slots, 0, sizeof(gSzModel_ahModel_slots));
    gSzModel_ahModel_slots[83] = &obj;

    /* type out of range, null object, completed == 0: no writes, no call */
    rebase_calls = 0;
    rec.unknown_01c = (84 << 8) | 3;
    lSzModel_LoadMotionCB(&rec, 1);
    rec.unknown_01c = (7 << 8) | 3;
    lSzModel_LoadMotionCB(&rec, 1);
    rec.unknown_01c = (83 << 8) | 3;
    lSzModel_LoadMotionCB(&rec, 0);
    assert(rebase_calls == 0 && obj.heads[3] == 0 && obj.values[3] == 0);

    /* valid: head stored, rebased, then the value read from its first word */
    lSzModel_LoadMotionCB(&rec, 1);
    assert(obj.heads[3] == head && rebase_calls == 1 && rebase_arg == head);
    assert(obj.values[3] == 0x1234 && obj.values[2] == 0 && obj.heads[2] == 0);
    munmap(page, 4096);
}

static int clear_calls, clear_arg, clear_last_bit;

void SzGame_InitMemCardProc(int unknown)
{
    clear_calls++;
    clear_arg = unknown;
    clear_last_bit = gSzModel_ahModel[83]->drawFlag.unknown_bit_1;
}

/* lSzGame_InitMemCheck (loop coverage): entries 0 and 83 are both cleared, a null entry
 * is skipped, and the final call follows the loop with 6. */
static void check_clear_all(void)
{
    static struct DotrObject objects[84];
    int i;
    memset(objects, 0, sizeof(objects));
    for (i = 0; i < 84; i++) {
        objects[i].drawFlag.unknown_bit_1 = -1;
        gSzModel_ahModel[i] = &objects[i];
    }
    gSzModel_ahModel[40] = 0;
    clear_calls = 0;
    clear_last_bit = -1;
    lSzGame_InitMemCheck();
    for (i = 0; i < 84; i++) {
        assert((objects[i].drawFlag.unknown_bit_1 != 0) == (i == 40));
    }
    assert(clear_calls == 1 && clear_arg == 6 && clear_last_bit == 0);
}

static int shade_calls;
static unsigned short shade_seen[8];

int func_00241a00_shade(float scale, unsigned short color, int e)
{
    assert(scale == 1.0f && e == 6 && shade_calls < 8);
    shade_seen[shade_calls++] = color;
    return 0x10000 | (color + 0x100);
}

/* SzModel_SetStoneRatio: branch coverage. The repeated +0x138 test cannot fail after
 * the first and is not reachable separately. */
static void check_shade_levels(void)
{
    static struct DotrObject o;
    static struct DotrObjectShadeHeader header;
    static long long list_words[16];
    static long long node_words[2][32];
    int *list = (int *)list_words;
    struct DotrObjectShadeSlot *slots = (struct DotrObjectShadeSlot *)(list + 4);
    struct DotrObjectShadeNode *nodes[2];
    unsigned short *words[2];
    int i;

    memset(gSzModel_ahModel, 0, 84 * sizeof(gSzModel_ahModel[0]));
    memset(&o, 0, sizeof(o));
    for (i = 0; i < 2; i++) {
        nodes[i] = (struct DotrObjectShadeNode *)node_words[i];
        nodes[i]->unknown_10.unknown_08 = 8;
        words[i] = (unsigned short *)((char *)nodes[i] + 8 + 96);
    }
    header.unknown_28 = 16;
    header.unknown_2c = 1;
    list = (int *)((char *)list_words + 16);
    slots = (struct DotrObjectShadeSlot *)(list + 4);
    *(struct DotrObjectShadeHeader **)o.unknown_13c = &header;
    o.modelBaseData = (struct DotrObjectLink *)list_words;
    o.stateFlags.unknown_bit_1 = -1;
    o.stoneRatio = 2;
    list[0] = 0;

    shade_calls = 0;
    assert(SzModel_SetStoneRatio(-1, 5) == 0 && SzModel_SetStoneRatio(84, 5) == 0 && SzModel_SetStoneRatio(83, 5) == 0);
    gSzModel_ahModel[83] = &o;
    o.modelId = -1;
    assert(SzModel_SetStoneRatio(83, 5) == 0);
    o.modelId = 0;
    o.modelBaseData = 0;
    assert(SzModel_SetStoneRatio(83, 5) == 0);
    o.modelBaseData = (struct DotrObjectLink *)list_words;
    o.drawFlag.unknown_bit_3 = -1;
    assert(SzModel_SetStoneRatio(83, 5) == 0);
    o.drawFlag.unknown_bit_3 = 0;
    *(struct DotrObjectShadeHeader **)o.unknown_13c = 0;
    assert(SzModel_SetStoneRatio(83, 5) == 0);
    *(struct DotrObjectShadeHeader **)o.unknown_13c = &header;
    header.unknown_2c = 0;
    assert(SzModel_SetStoneRatio(83, 5) == 0);
    header.unknown_2c = 1;
    o.stateFlags.unknown_bit_1 = 0;
    assert(SzModel_SetStoneRatio(83, 5) == 0);
    o.stateFlags.unknown_bit_1 = -1;
    assert(SzModel_SetStoneRatio(83, 1) == 0 && SzModel_SetStoneRatio(83, 0x101) == 0);
    assert(o.stoneRatio == 2 && shade_calls == 0);

    /* Empty list: only the level byte changes. */
    assert(SzModel_SetStoneRatio(83, 2) == 1 && o.stoneRatio == 2 && shade_calls == 0);
    assert(SzModel_SetStoneRatio(0, 0) == 0);

    /* Null node, node without data, node with data; equal level is one word. */
    list[0] = 3;
    slots[0].node = 0;
    slots[1].node = nodes[0];
    slots[2].node = nodes[1];
    nodes[0]->unknown_10.unknown_0c = 0;
    nodes[1]->unknown_10.unknown_0c = 1;
    for (i = 0; i < 8; i++) {
        words[0][i] = (unsigned short)(0x10 + i);
        words[1][i] = (unsigned short)(0x20 + i);
    }
    assert(SzModel_SetStoneRatio(83, 2) == 1 && shade_calls == 1 && shade_seen[0] == 0x22);
    assert(words[1][2] == 0x122 && words[1][1] == 0x21 && words[1][3] == 0x23 && o.stoneRatio == 2);

    /* Higher level in the low byte only: words 2..5 of every node with data. */
    shade_calls = 0;
    nodes[0]->unknown_10.unknown_0c = 1;
    assert(SzModel_SetStoneRatio(83, 0x305) == 1 && shade_calls == 8 && o.stoneRatio == 5);
    for (i = 0; i < 8; i++) {
        int in = i >= 2 && i <= 5;
        assert(words[0][i] == 0x10 + i + (in ? 0x100 : 0));
        assert(words[1][i] == 0x20 + i + (in ? 0x100 : 0) + (i == 2 ? 0x100 : 0));
    }
    assert(shade_seen[0] == 0x12 && shade_seen[3] == 0x15 && shade_seen[4] == 0x122 && shade_seen[7] == 0x25);
    gSzModel_ahModel[83] = 0;
}

static int cue_step, cue_frame, cue_frame_arg, cue_frame_calls, cue_calls;
static int cue_log[8][2];

int func_001a8760_int(void)
{
    return cue_step;
}

int SzEffect_GetCount(int index)
{
    cue_frame_arg = index;
    cue_frame_calls++;
    return cue_frame;
}

int func_00217710_cue(int index, int value)
{
    assert(cue_calls < 8);
    cue_log[cue_calls][0] = index;
    cue_log[cue_calls][1] = value;
    cue_calls++;
    return 0;
}

/* SzModel_ReqSE: branch coverage, stubbed callees. */
static void check_cues(void)
{
    static struct DotrObject o;
    static struct DotrObjectCue cues[65];
    int *frame = (int *)&o.unknown_1e8[4];
    int i;

    memset(gSzModel_ahModel, 0, 84 * sizeof(gSzModel_ahModel[0]));
    memset(&o, 0, sizeof(o));
    memset(cues, 0, sizeof(cues));
    o.modelBaseData = &link;
    o.drawFlag.unknown_bit_1 = -1;
    *frame = -1;
    o.animDir.unknown_bits_6_7 = 1;
    *(struct DotrObjectCue **)o.ptrBlock = cues;
    o.unknown_1e0[0] = 3;
    o.unknown_198[0].y = 10.0f;
    cue_step = 4;
    gSzModel_ahModel[83] = &o;
    for (i = 0; i < 65; i++) {
        cues[i].kind = 1;
    }
    cue_calls = 0;

    /* Rejections, one per test. */
    assert(SzModel_ReqSE(1, 83) == 0 && SzModel_ReqSE(4, 83) == 0);
    assert(SzModel_ReqSE(2, -1) == 0 && SzModel_ReqSE(3, 84) == 0 && SzModel_ReqSE(2, 0) == 0);
    o.modelId = -1;
    assert(SzModel_ReqSE(2, 83) == 0);
    o.modelId = 0;
    o.modelBaseData = 0;
    assert(SzModel_ReqSE(2, 83) == 0);
    o.modelBaseData = &link;
    o.drawFlag.unknown_bit_3 = -1;
    assert(SzModel_ReqSE(2, 83) == 0);
    o.drawFlag.unknown_bit_3 = 0;
    o.drawFlag.unknown_bit_1 = 0;
    assert(SzModel_ReqSE(2, 83) == 0);
    o.drawFlag.unknown_bit_1 = -1;
    o.stateFlags.unknown_bit_0 = -1;
    assert(SzModel_ReqSE(2, 83) == 0);
    o.stateFlags.unknown_bit_0 = 0;
    o.animDir.unknown_bits_6_7 = 0;
    assert(SzModel_ReqSE(2, 83) == 0);
    o.animDir.unknown_bits_6_7 = 1;
    *(struct DotrObjectCue **)o.ptrBlock = 0;
    assert(SzModel_ReqSE(2, 83) == 0);
    *(struct DotrObjectCue **)o.ptrBlock = cues;
    assert(cue_calls == 0 && cue_frame_calls == 0);

    /* Object time 10, step 4: window [6, 10) for kind 3; kind 15 ends. */
    cues[0].kind = 3; cues[0].time = 5;  cues[0].code = 0x100;
    cues[1].kind = 3; cues[1].time = 6;  cues[1].code = 0x101;
    cues[2].kind = 2; cues[2].time = 7;  cues[2].code = 0x102;
    cues[3].kind = 3; cues[3].time = 9;  cues[3].code = 0xffff;
    cues[4].kind = 3; cues[4].time = 10; cues[4].code = 0x104;
    cues[5].kind = 15;
    cues[6].kind = 3; cues[6].time = 7;  cues[6].code = 0x106;
    assert(SzModel_ReqSE(2, 83) == 1 && cue_calls == 2 && cue_frame_calls == 0);
    assert(cue_log[0][0] == 2 && cue_log[0][1] == 0x101 && cue_log[1][0] == 2 && cue_log[1][1] == 0xffff);

    /* Frame source set: kind 2 and the callee's time, whatever +0x1f7 holds;
     * no terminator, so exactly 64 cues are read. */
    cue_calls = 0;
    *frame = 7;
    cue_frame = 4000;
    cue_step = 100;
    o.animDir.unknown_bits_6_7 = 0;
    cues[5].kind = 1;
    cues[2].time = 3900;
    cues[63].kind = 2; cues[63].time = 3999; cues[63].code = 0x163;
    cues[64].kind = 2; cues[64].time = 3950; cues[64].code = 0x164;
    assert(SzModel_ReqSE(3, 83) == 1 && cue_frame_calls == 1 && cue_frame_arg == 7 && cue_calls == 2);
    assert(cue_log[0][0] == 3 && cue_log[0][1] == 0x102 && cue_log[1][1] == 0x163);
    gSzModel_ahModel[83] = 0;
}

struct DotrObjectCopy112 data_0031ad00_copy[32];
static int rows_calls, rows_count;
static void *rows_destination, *rows_source;

void *RhMem_CopyMemory16(void *destination, void *source, int count)
{
    rows_calls++;
    rows_destination = destination;
    rows_source = source;
    rows_count = count;
    return destination;
}

/* SzModel_ReqMotionBlur (branch and loop coverage). */
static void check_entry_fill(void)
{
    static struct DotrObject object;
    struct DotrObjectQuad a = {1, 2, 3, 4}, b = {5, 6, 7, 8};
    struct DotrObjectCopy112 *e = data_0031ad00_copy;
    struct DotrObjectCopy112 before;
    int i, kind;

    memset(gSzModel_ahModel, 0, sizeof(gSzModel_ahModel));
    memset(&object, 0, sizeof(object));
    memset(data_0031ad00_copy, 0, sizeof(data_0031ad00_copy));
    object.unknown_198[0].x = 1.5f, object.unknown_198[0].y = 2.5f, object.unknown_198[0].z = 3.5f;
    object.unknown_198[1].x = 4.5f, object.unknown_198[1].y = 5.5f, object.unknown_198[1].z = 6.5f;
    object.unknown_1e0[0] = -3, object.unknown_1e0[1] = 9, object.unknown_1e0[2] = 77;
    object.rgba.unknown_0 = 0x11, object.rgba.unknown_1 = 0x22;
    object.rgba.unknown_2 = 0x33, object.rgba.unknown_3 = 0x44;
    object.alphaMode.unknown_bits_0_1 = 2, object.alphaMode.unknown_bits_2_3 = 3;
    e[0].index = -1; /* entry 0 is never used */
    e[6].index = -1;
    e[31].index = -1;
    /* a null object; kinds other than 4 and 5 */
    rows_calls = 0;
    assert(SzModel_ReqMotionBlur(9, 1, 1, &a, &b, 1) == 0);
    gSzModel_ahModel[9] = &object;
    for (kind = 3; kind <= 6; kind += 3) {
        object.type = kind;
        assert(SzModel_ReqMotionBlur(9, 1, 1, &a, &b, 1) == 0);
    }
    assert(rows_calls == 0 && e[6].index == -1 && e[0].index == -1);
    /* kind 4: the first free entry from 1 on, both quads given, mode and flag given */
    object.type = 4;
    e[6].unknown_bits = 3;
    e[6].unknown_6c[0] = 0x5a;
    assert(SzModel_ReqMotionBlur(9, 0x1234, 1, &a, &b, 1) == 1);
    assert(rows_calls == 1 && rows_destination == &e[6] && rows_source == &object && rows_count == 4);
    assert(e[6].unknown_40[0].x == 1.5f && e[6].unknown_40[0].y == 2.5f && e[6].unknown_40[0].z == 3.5f);
    assert(e[6].unknown_40[1].x == 4.5f && e[6].unknown_40[1].y == 5.5f && e[6].unknown_40[1].z == 6.5f);
    assert(e[6].unknown_58[0] == -3 && e[6].unknown_58[1] == 9);
    assert(memcmp(&e[6].unknown_5a, &a, 4) == 0 && memcmp(&e[6].unknown_5e, &b, 4) == 0);
    assert(e[6].unknown_62 == 0x1234 && e[6].unknown_64 == 0 && e[6].index == 9);
    assert(e[6].mode == 1 && e[6].flag == -1 && e[6].unknown_bits == 3 && e[6].unknown_6c[0] == 0x5a);
    assert(e[0].index == -1 && e[31].index == -1 && e[5].index == 0 && e[7].index == 0);
    /* kind 5: the last entry; default quads, the object's mode, an even flag */
    object.type = 5;
    e[31].unknown_64 = 7;
    e[31].flag = -1;
    assert(SzModel_ReqMotionBlur(9, -2, -1, 0, 0, 2) == 1);
    assert(rows_calls == 2 && rows_destination == &e[31] && rows_count == 4);
    assert(e[31].unknown_5a.unknown_0 == 0x11 && e[31].unknown_5a.unknown_1 == 0x22);
    assert(e[31].unknown_5a.unknown_2 == 0x33 && e[31].unknown_5a.unknown_3 == 0x44);
    assert(e[31].unknown_5e.unknown_0 == 0x11 && e[31].unknown_5e.unknown_1 == 0x22);
    assert(e[31].unknown_5e.unknown_2 == 0x33 && e[31].unknown_5e.unknown_3 == 0);
    assert(e[31].unknown_62 == 0xfffe && e[31].unknown_64 == 0 && e[31].index == 9 && e[31].mode == 2 && e[31].flag == 0);
    /* only quad b missing keeps a; only quad a missing leaves b's fourth byte */
    e[3].index = -5;
    assert(SzModel_ReqMotionBlur(9, 0, 0, &a, 0, 0) == 1 && e[3].index == 9 && e[3].mode == 0);
    assert(memcmp(&e[3].unknown_5a, &a, 4) == 0 && e[3].unknown_5e.unknown_0 == 0x11 && e[3].unknown_5e.unknown_3 == 0);
    e[3].index = -5;
    assert(SzModel_ReqMotionBlur(9, 0, 3, 0, &b, 0) == 1 && e[3].mode == 3);
    assert(e[3].unknown_5a.unknown_3 == 0x44 && memcmp(&e[3].unknown_5e, &b, 4) == 0);
    /* nothing free: entry 0 does not count */
    memcpy(&before, &e[0], sizeof(before));
    rows_calls = 0;
    assert(SzModel_ReqMotionBlur(9, 1, 1, &a, &b, 1) == 0 && rows_calls == 0 && memcmp(&before, &e[0], sizeof(before)) == 0);
    for (i = 1; i < 32; i++) {
        assert(e[i].index >= 0);
    }
    memset(gSzModel_ahModel, 0, sizeof(gSzModel_ahModel));
}

static struct DotrObject made_object;
static int make_fail, make_calls, make_bytes, wipe_calls, wipe_value, wipe_count;
static int *wipe_destination;

void *SzMem_Alloc(int byte_count)
{
    make_calls++;
    make_bytes = byte_count;
    return make_fail ? NULL : &made_object;
}
int *RhMem_FillMemory4(int *destination, int value, int count)
{
    wipe_calls++;
    wipe_destination = destination;
    wipe_value = value;
    wipe_count = count;
    memset(destination, value, (size_t)count * 4);
    return destination;
}

/* SzModel_Create (branch coverage), through the real 0x1e7d00. */
static void check_make_object(void)
{
    static struct DotrObject expected;
    static const int index[2] = {0, 83};
    int i;

    reset();
    memset(gSzModel_ahModel, 0, sizeof(gSzModel_ahModel));
    make_calls = wipe_calls = make_fail = 0;
    /* outside 0-83: no allocation */
    assert(SzModel_Create(-1) == 0 && SzModel_Create(84) == 0 && make_calls == 0);
    /* a failed allocation leaves the table entry */
    make_fail = 1;
    assert(SzModel_Create(5) == 0 && make_calls == 1 && make_bytes == (int)sizeof(struct DotrObject));
    assert(wipe_calls == 0 && gSzModel_ahModel[5] == 0);
    make_fail = 0;
    for (i = 0; i < 2; i++) {
        reset();
        make_calls = wipe_calls = 0;
        memset(&made_object, 0x5a, sizeof(made_object));
        assert(SzModel_Create(index[i]) == 1 && gSzModel_ahModel[index[i]] == &made_object);
        assert(make_calls == 1 && make_bytes == (int)sizeof(struct DotrObject));
        assert(wipe_calls == 1 && wipe_destination == (int *)&made_object && wipe_value == 0);
        assert(wipe_count == (int)(sizeof(struct DotrObject) / 4));
        /* the same values set on a cleared object, then the real 0x1e7d00 */
        memset(&expected, 0, sizeof(expected));
        expected.unknown_1e0[1] = -1;
        expected.animDir.unknown_bits_6_7 = 1;
        expected.modelId = -1;
        expected.modelNo = -1;
        *(int *)&expected.unknown_1e8[4] = -1;
        expected.scale.x = expected.scale.y = expected.scale.z = expected.scale.unknown_w = 1.0f;
        expected.intensity.x = expected.intensity.y = 0.8f;
        expected.intensity.z = 1.0f;
        memset(&expected.rgba, 0x80, 4);
        expected.alphaMode.unknown_bits_2_3 = 1;
        expected.drawFlag.unknown_bit_0 = expected.drawFlag.unknown_bit_1 = -1;
        expected.drawFlag.unknown_bit_6 = expected.drawFlag.unknown_bit_7 = -1;
        reset();
        gSzModel_ahModel[index[i]] = &expected;
        assert(lSzModel_Clean(index[i]) == 1);
        assert(memcmp(&made_object, &expected, sizeof(expected)) == 0);
        /* spot values, independent of the comparison object */
        assert(made_object.scale.unknown_w == 1.0f && made_object.intensity.y == 0.8f);
        assert(made_object.rgba.unknown_3 == 0x80 && made_object.filters.unknown_0 == 0);
        assert(made_object.drawFlag.unknown_bit_0 == -1 && made_object.drawFlag.unknown_bit_2 == 0);
        assert(made_object.drawFlag.unknown_bit_6 == -1 && made_object.drawFlag.unknown_bit_7 == -1);
        assert(made_object.animDir.unknown_bits_6_7 == 1 && made_object.alphaMode.unknown_bits_2_3 == 1);
        assert(made_object.modelNo == -1 && made_object.modelId == -1 && made_object.coordIndex == 32767);
    }
    reset();
}

/* SzEffect_SetModelPauseForType and SzEffect_StoneProc on the real 0x1f2980, 0x1f32f0, 0x1f3690 and
 * 0x1f34a0: branch and loop coverage. Objects have an empty shade list, so
 * 0x1f34a0 changes only the level byte. */
static void check_table_walks(void)
{
    static struct DotrObject o[8];
    static struct DotrObjectShadeHeader header;
    static long long list_words[16];
    static const int at[8] = {3, 10, 83, 19, 20, 25, 29, 30};
    int i;

    memset(gSzModel_ahModel, 0, 84 * sizeof(gSzModel_ahModel[0]));
    memset(o, 0, sizeof(o));
    memset(list_words, 0, sizeof(list_words));
    header.unknown_28 = 16;
    header.unknown_2c = 1;
    for (i = 0; i < 8; i++) {
        *(struct DotrObjectShadeHeader **)o[i].unknown_13c = &header;
        o[i].modelBaseData = (struct DotrObjectLink *)list_words;
        o[i].stateFlags.unknown_bit_1 = -1;
        gSzModel_ahModel[at[i]] = &o[i];
    }
    o[0].type = 5;
    o[1].type = 6;
    o[2].type = 5;
    for (i = 3; i < 8; i++) {
        o[i].type = 9;
    }

    /* A non-negative id: only objects with that +0x1f3 byte get the bit. */
    SzEffect_SetModelPauseForType(5, 1);
    assert(o[0].stateFlags.unknown_bit_0 != 0 && o[2].stateFlags.unknown_bit_0 != 0);
    for (i = 1; i < 8; i++) {
        assert(i == 2 || o[i].stateFlags.unknown_bit_0 == 0);
    }
    /* An id no object has: nothing changes. */
    SzEffect_SetModelPauseForType(7, 0);
    assert(o[0].stateFlags.unknown_bit_0 != 0 && o[2].stateFlags.unknown_bit_0 != 0);
    /* A negative id: every object, first and last index included. */
    SzEffect_SetModelPauseForType(-1, 1);
    for (i = 0; i < 8; i++) {
        assert(o[i].stateFlags.unknown_bit_0 != 0);
    }
    SzEffect_SetModelPauseForType(-3, 0);
    for (i = 0; i < 8; i++) {
        assert(o[i].stateFlags.unknown_bit_0 == 0 && o[i].stateFlags.unknown_bit_1 != 0);
    }

    /* Levels: index 19 is below the walked range, 20 and 29 are its ends, 30 is
     * the first of the second range; 31 is empty, 32 is added below. */
    o[3].stoneRatio = 7;   /* 19: not walked */
    o[4].stoneRatio = 0;   /* 20: 0 -> 1 */
    o[5].stoneRatio = 253; /* 25: 253 -> 254 */
    o[6].stoneRatio = 254; /* 29: 254 -> 255, the clamped arm */
    o[7].stoneRatio = 255; /* 30: already 255, not raised */
    SzEffect_StoneProc();
    assert(o[3].stoneRatio == 7 && o[4].stoneRatio == 1 && o[5].stoneRatio == 254);
    assert(o[6].stoneRatio == 255 && o[7].stoneRatio == 255);
    /* Second range ends at 31; a clear bit 1 (answer -1) and index 32 are left. */
    gSzModel_ahModel[3] = 0;
    gSzModel_ahModel[10] = 0;
    gSzModel_ahModel[83] = 0;
    gSzModel_ahModel[30] = &o[0];
    gSzModel_ahModel[31] = &o[1];
    gSzModel_ahModel[32] = &o[2];
    o[0].stoneRatio = 40;
    o[1].stoneRatio = 41;
    o[2].stoneRatio = 42;
    o[6].stateFlags.unknown_bit_1 = 0;
    SzEffect_StoneProc();
    assert(o[0].stoneRatio == 41 && o[1].stoneRatio == 42 && o[2].stoneRatio == 42);
    assert(o[4].stoneRatio == 2 && o[5].stoneRatio == 255 && o[6].stoneRatio == 255 && o[3].stoneRatio == 7);
    memset(gSzModel_ahModel, 0, 84 * sizeof(gSzModel_ahModel[0]));
}

static void check_pair_fade(void)
{
    static struct DotrObject objects[3];
    static const int slots[3] = {1, 5, 6};
    struct DotrObject *saved[3];
    int i;
    for (i = 0; i < 3; i++) {
        saved[i] = gSzModel_ahModel[slots[i]];
        memset(&objects[i], 0, sizeof(objects[i]));
        gSzModel_ahModel[slots[i]] = &objects[i];
    }
    /* Negative: x and y of slots 1, 5 and 6 go down by 0.08, stopping at 0. */
    objects[0].intensity.x = 0.5f;  objects[0].intensity.y = 0.04f; objects[0].intensity.z = 3.0f;
    objects[1].intensity.x = 0.04f; objects[1].intensity.y = 0.5f;  objects[1].intensity.z = 4.0f;
    objects[2].intensity.x = 0.0f;  objects[2].intensity.y = 0.08f; objects[2].intensity.z = 5.0f;
    Ta109_BlackOutForExz(-1);
    assert(objects[0].intensity.x == 0.5f - 0.08f && objects[0].intensity.y == 0.0f);
    assert(objects[1].intensity.x == 0.0f && objects[1].intensity.y == 0.5f - 0.08f);
    assert(objects[2].intensity.x == 0.0f && objects[2].intensity.y == 0.08f - 0.08f);
    assert(objects[0].intensity.z == 3.0f && objects[1].intensity.z == 4.0f);
    assert(objects[2].intensity.z == 5.0f);
    /* Zero and positive: x and y become 0.8; z is kept. */
    Ta109_BlackOutForExz(0);
    for (i = 0; i < 3; i++) {
        assert(objects[i].intensity.x == 0.8f && objects[i].intensity.y == 0.8f);
        assert(objects[i].intensity.z == 3.0f + i);
        objects[i].intensity.x = objects[i].intensity.y = 0.25f;
    }
    Ta109_BlackOutForExz(7);
    for (i = 0; i < 3; i++) {
        assert(objects[i].intensity.x == 0.8f && objects[i].intensity.y == 0.8f);
        assert(objects[i].intensity.z == 3.0f + i);
        gSzModel_ahModel[slots[i]] = saved[i];
    }
}

/* SzModel_GetCenterOfs with the 0x1efbd0 stub: one assertion per branch outcome
 * (bad index, empty slot, negative byte 0x1f2, zero byte 0x1fe, nothing
 * found, found). */
static void check_found_scale(void)
{
    static float found[11], scales[22];
    struct DotrObjectVector4 out, zero;
    float *scale_pointer = scales;

    reset();
    memset(&zero, 0, sizeof(zero));
    found[8] = 6.0f;
    found[9] = -3.0f;
    found[10] = 1.5f;
    scales[21] = 2.0f;
    memcpy(object.unknown_13c, &scale_pointer, sizeof(scale_pointer));
    object.GNode = 0x4321;
    object.modelId = 0;
    object.refBoneNum = 1;
    chain_result = (DotrObjectWord)found;
    chain_calls = 0;
    memset(&out, 0, sizeof(out));
    assert(SzModel_GetCenterOfs(-1, &out) == 0 && SzModel_GetCenterOfs(84, &out) == 0 && SzModel_GetCenterOfs(7, &out) == 0);
    object.modelId = -1;
    assert(SzModel_GetCenterOfs(83, &out) == 0);
    object.modelId = 0;
    object.refBoneNum = 0;
    assert(SzModel_GetCenterOfs(83, &out) == 0 && chain_calls == 0 && memcmp(&out, &zero, sizeof(out)) == 0);
    object.refBoneNum = 1;
    chain_result = 0;
    chain_local = -1;
    assert(SzModel_GetCenterOfs(83, &out) == 1 && chain_calls == 1 && memcmp(&out, &zero, sizeof(out)) == 0);
    assert(chain_args[0] == 1 && chain_args[1] == 0x4321 && chain_args[2] == 0x4321 && chain_local == 0);
    assert(chain_args[4] == 0 && chain_args[5] == 0 && chain_args[6] == 0);
    chain_result = (DotrObjectWord)found;
    assert(SzModel_GetCenterOfs(83, &out) == 1 && chain_calls == 2);
    assert(out.x == 3.0f && out.y == -1.5f && out.z == 0.75f && out.unknown_w == 1.0f);
    chain_result = 0x77;
    reset();
}

/* Taku_Camp_STON (branch and loop coverage): byte 1 passes the local copy on all ten calls,
 * byte 0 and byte 2 pass null; ten indices 34..43; the two-index tail clears the bit on
 * index 30 and tolerates an empty index 31. */
static void check_cursor_run(void)
{
    static struct DotrObject object;
    struct DotrObject *keep30 = gSzModel_ahModel[30], *keep31 = gSzModel_ahModel[31];
    static const signed char modes[3] = {1, 0, 2};
    int m, i;

    for (m = 0; m < 3; m++) {
        gSzModel_ahModel[30] = &object;
        gSzModel_ahModel[31] = 0;
        object.drawFlag.unknown_bit_1 = -1;
        dotr_object_bank_01ed0000.unknown_53b3 = modes[m];
        data_01ed53f4 = -5;
        inner_calls = 0;
        memset(inner_vector, 0, sizeof(inner_vector));
        assert(Taku_Camp_STON(100) == 110 && inner_calls == 10);
        for (i = 0; i < 10; i++) {
            assert(inner_log_index[i] == 34 + i && (inner_log_pointer[i] != 0) == (modes[m] == 1));
        }
        assert(inner_args[0] == 109 && inner_args[2] == 0 && inner_args[4] == 0 && inner_args[5] == 0);
        assert(inner_args[6] == 0 && object.drawFlag.unknown_bit_1 == 0);
        if (modes[m] == 1) {
            assert(inner_vector[0] == 0x11 && inner_vector[1] == -5 && inner_vector[2] == 0x33 && inner_vector[3] == 0x44);
        }
        assert(data_0029abb0.unknown_words[1] == 0x22);
    }
    gSzModel_ahModel[30] = keep30;
    gSzModel_ahModel[31] = keep31;
    inner_calls = 0;
}

struct DotrLoadObject *gSzModel_ahModel_load[84];
char *gSzModel_apchBase[8];
char data_002cfa17[] = "DBDIR";
char data_002cfb4f[] = "%s/%s/TEX/M%%03d.rh2";
char data_002cfb64[] = "%s/%s/TEX/B%%03d_%%d.rh2";
static int label_calls;
static const char *label_arg;

static int env_busy;
char *SzFile_GetEnv(const char *label)
{
    label_calls++;
    label_arg = label;
    if (env_busy) {
        object.drawFlag.unknown_bit_3 = -1; /* makes the loader that follows refuse */
    }
    return "root";
}

int func_0019b648(char *destination, const char *format, ...)
{
    va_list args;
    int n;

    va_start(args, format);
    n = vsnprintf(destination, 128, format, args);
    va_end(args);
    return n;
}

/* Branch coverage: index bounds on both sides, missing object, the +0x138
 * early return, and the full path with every submitted name and word. The
 * callback gets both outcomes of `completed`. Synthetic strings and objects. */
static void check_texture_submit(void)
{
    /* the object sits on a low page: its field addresses travel in an int */
    struct DotrLoadObject *obj = mmap(0, 4096, PROT_READ | PROT_WRITE,
                                      MAP_PRIVATE | MAP_ANONYMOUS | MAP_32BIT, -1, 0);
    struct DotrResourceSubmitRecord rec;
    char expect[64];
    int i;

    assert(obj != MAP_FAILED);
    assert(offsetof(struct DotrLoadObject, main_part) == 0x140);
    assert(offsetof(struct DotrLoadObject, kind) == 0x1f3);
    assert(offsetof(struct DotrLoadObject, number) == 0x1f4);
    memset(gSzModel_ahModel_load, 0, sizeof(gSzModel_ahModel_load));
    gSzModel_apchBase[2] = "kd";
    obj->kind = 2;
    obj->number = 7;
    obj->main_part = 5;
    for (i = 0; i < 8; i++) {
        obj->parts[i] = 9;
    }

    submit_calls = 0;
    label_calls = 0;
    assert(SzModel_LoadEffectTextureFile(-1) == 0 && SzModel_LoadEffectTextureFile(84) == 0);
    assert(SzModel_LoadEffectTextureFile(83) == 0 && SzModel_LoadEffectTextureFile(0) == 0);
    assert(label_calls == 0 && submit_calls == 0);

    /* +0x138 clear: label looked up, nothing submitted or cleared */
    gSzModel_ahModel_load[83] = obj;
    gSzModel_ahModel_load[0] = obj;
    assert(SzModel_LoadEffectTextureFile(83) == 1);
    assert(label_calls == 1 && label_arg == data_002cfa17);
    assert(submit_calls == 0 && obj->main_part == 5 && obj->parts[0] == 9);

    obj->modelBaseData = 1;
    assert(SzModel_LoadEffectTextureFile(0) == 1);
    assert(label_calls == 2 && submit_calls == 9);
    assert(submit_args[0] == 0 && submit_args[1] == 0 && submit_args[2] == -1);
    assert(submit_callback == lSzModel_LoadEffectTextureFileCB);
    assert(strcmp(submit_names[0], "root/kd/TEX/M007.rh2") == 0);
    assert(submit_words[0] == (int)(intptr_t)&obj->main_part && obj->main_part == 0);
    for (i = 0; i < 8; i++) {
        /* the format takes the number twice; the loop counter is passed but unused */
        strcpy(expect, "root/kd/TEX/B007_7.rh2");
        assert(strcmp(submit_names[i + 1], expect) == 0);
        assert(submit_words[i + 1] == (int)(intptr_t)&obj->parts[i] && obj->parts[i] == 0);
    }

    memset(&rec, 0, sizeof(rec));
    rec.unknown_014 = 0x4560;
    rec.unknown_01c = (int)(intptr_t)&obj->parts[3];
    pack_ncalls = 0;
    lSzModel_LoadEffectTextureFileCB(&rec, 0);
    assert(pack_ncalls == 0 && obj->parts[3] == 0);
    lSzModel_LoadEffectTextureFileCB(&rec, 1);
    assert(pack_ncalls == 1 && pack_calls[0] == (void *)(intptr_t)0x4560);
    assert(obj->parts[3] == 0x4560 && obj->parts[2] == 0 && obj->parts[4] == 0);
    munmap(obj, 4096);
}

static int flag_all_calls, flag_all_args[3];

int SzView_PauseRequest(int a, int b, int flag)
{
    flag_all_calls++;
    flag_all_args[0] = a;
    flag_all_args[1] = b;
    flag_all_args[2] = flag;
    return 0;
}

/* SzEffect_SetPauseFor3DBattle on the real SzEffect_SetModelPauseForType / SzModel_SetPauseFlag
 * (straight-line coverage: which objects get the bit, and the forwarded triple).
 * SzEffect_SetModelPause2: one assertion per bound of row and column, the negative lookup, and both
 * 0x1f32f0 answers. */
static void check_bit_wrappers(void)
{
    static struct DotrObject o[6];
    static const unsigned char kind[6] = {4, 5, 2, 3, 1, 6};
    int i;

    memset(gSzModel_ahModel, 0, 84 * sizeof(gSzModel_ahModel[0]));
    memset(o, 0, sizeof(o));
    for (i = 0; i < 6; i++) {
        o[i].type = kind[i];
        gSzModel_ahModel[10 + i] = &o[i];
    }
    flag_all_calls = 0;
    SzEffect_SetPauseFor3DBattle(1);
    for (i = 0; i < 6; i++) {
        assert((o[i].stateFlags.unknown_bit_0 != 0) == (i < 4));
    }
    assert(flag_all_calls == 1 && flag_all_args[0] == -1 && flag_all_args[1] == -1 && flag_all_args[2] == 1);
    SzEffect_SetPauseFor3DBattle(0);
    for (i = 0; i < 6; i++) {
        assert(o[i].stateFlags.unknown_bit_0 == 0);
    }
    assert(flag_all_calls == 2 && flag_all_args[2] == 0);

    lookup_result = 14;
    lookup_args[0] = lookup_args[1] = -9;
    assert(SzEffect_SetModelPause2(-0x100, 1) == 0 && SzEffect_SetModelPause2(0x300, 1) == 0);
    assert(SzEffect_SetModelPause2(0x00b, 1) == 0 && SzEffect_SetModelPause2(0x2ff, 1) == 0);
    assert(lookup_args[0] == -9 && o[4].stateFlags.unknown_bit_0 == 0);
    assert(SzEffect_SetModelPause2(0x20a, 1) == 1 && lookup_args[0] == 2 && lookup_args[1] == 10);
    assert(o[4].stateFlags.unknown_bit_0 != 0);
    assert(SzEffect_SetModelPause2(0x000, 0) == 1 && lookup_args[0] == 0 && lookup_args[1] == 0);
    assert(o[4].stateFlags.unknown_bit_0 == 0);
    lookup_result = 20;   /* an empty slot: 0x1f32f0 answers 0 */
    assert(SzEffect_SetModelPause2(0x105, 1) == 0 && lookup_args[0] == 1 && lookup_args[1] == 5);
    lookup_result = -1;
    assert(SzEffect_SetModelPause2(0x105, 1) == 0 && o[4].stateFlags.unknown_bit_0 == 0);
}

/* 0x1e8e90 branch coverage: index bounds, no object, no link (release and clear),
 * on == 0 and on != 0 for each attachment. */
static void check_attach(void)
{
    static struct DotrObjectAttachView view;
    static struct DotrObjectLink attach_link;
    static unsigned short data[8];
    struct DotrObjectAttachMessage m;
    struct DotrObject *saved83 = gSzModel_ahModel[83], *saved5 = gSzModel_ahModel[5];

    memset(&m, 0, sizeof(m));
    data[6] = 0x8000;
    m.unknown_14 = (DotrObjectWord)data;
    gSzModel_ahModel[83] = (struct DotrObject *)&view;
    gSzModel_ahModel[5] = 0;
    free_calls = 0;
    view.modelBaseData = &attach_link;
    view.pCameraMotion = view.pCameraMotion2 = 7;
    /* index 84 is outside; index 5 has no object */
    m.unknown_1c = 84;
    lSzModel_LoadCameraCB(&m, 1);
    m.unknown_1c = 5;
    lSzModel_LoadCameraCB(&m, 1);
    assert(view.pCameraMotion == 7 && view.pCameraMotion2 == 7 && free_calls == 0);
    /* first attachment, on: bits above the low eight are ignored */
    m.unknown_1c = 83 | 0x100;
    attach_link.unknown_04 = -1;
    lSzModel_LoadCameraCB(&m, 1);
    assert(view.pCameraMotion == (DotrObjectWord)data && view.pCameraMotion2 == 7);
    assert(view.cameraFrameTotal == 0x7fff && view.cameraFrameCurrent == 0x7fff);
    assert(attach_link.unknown_04 == ~0x80000 && free_calls == 0);
    /* second attachment, on */
    m.unknown_1c = 83 | 0x80;
    attach_link.unknown_04 = -1;
    view.cameraFrameCurrent = view.cameraFrameTotal = 3;
    lSzModel_LoadCameraCB(&m, 1);
    assert(view.pCameraMotion2 == (DotrObjectWord)data && attach_link.unknown_04 == ~0x100000);
    assert(view.cameraFrameCurrent == 3 && view.cameraFrameTotal == 3);
    /* off: only the selected word is cleared */
    lSzModel_LoadCameraCB(&m, 0);
    assert(view.pCameraMotion2 == 0 && view.pCameraMotion == (DotrObjectWord)data);
    m.unknown_1c = 83;
    lSzModel_LoadCameraCB(&m, 0);
    assert(view.pCameraMotion == 0 && attach_link.unknown_04 == ~0x100000 && free_calls == 0);
    /* no link: the address is released and the word cleared even with on set */
    view.modelBaseData = 0;
    view.pCameraMotion = view.pCameraMotion2 = 7;
    lSzModel_LoadCameraCB(&m, 1);
    assert(free_calls == 1 && freed == data && view.pCameraMotion == 0 && view.pCameraMotion2 == 7);
    m.unknown_1c = 83 | 0x80;
    lSzModel_LoadCameraCB(&m, 1);
    assert(free_calls == 2 && view.pCameraMotion2 == 0);
    gSzModel_ahModel[83] = saved83;
    gSzModel_ahModel[5] = saved5;
}

/* Branch coverage: id below 0, id at the bound, empty entry, live entries 0 and 83. */
static void check_effect_callback(void)
{
    static struct DotrObject first, last;
    struct DotrObject *saved0 = gSzModel_ahModel[0];
    struct DotrObject *saved83 = gSzModel_ahModel[83];

    *(int *)&first.unknown_1e8[4] = 5;
    *(int *)&last.unknown_1e8[4] = 6;
    gSzModel_ahModel[0] = &first;
    gSzModel_ahModel[83] = &last;
    lSzModel_EffectCB(-1);
    lSzModel_EffectCB(84);
    assert(*(int *)&first.unknown_1e8[4] == 5 && *(int *)&last.unknown_1e8[4] == 6);
    lSzModel_EffectCB(0);
    assert(*(int *)&first.unknown_1e8[4] == -1 && *(int *)&last.unknown_1e8[4] == 6);
    lSzModel_EffectCB(83);
    assert(*(int *)&last.unknown_1e8[4] == -1);
    gSzModel_ahModel[0] = 0;
    lSzModel_EffectCB(0);
    gSzModel_ahModel[0] = saved0;
    gSzModel_ahModel[83] = saved83;
}

static void *bind_model_arg;
static int bind_model_calls, bind_loadcb_arg, bind_loadcb_calls;
int RhModel_BindAddress(void *header) { bind_model_arg = header; bind_model_calls++; return 0; }
void lSzModel_LoadCB(int index) { bind_loadcb_arg = index; bind_loadcb_calls++; }

/* 0x1f0500 branch coverage: empty slot; every flag bit clear; motion bits 0 and 7, text bits
 * 11 and 18 and each single-bit block set, with the cursor advance checked after each. */
static void check_bind_model_by_addr(void)
{
    static int file[0x6000];
    static int firsts[2][2] = {{0, 31}, {0, 47}};
    static struct DotrBindObject obj;
    struct DotrBindHeader *h = (struct DotrBindHeader *)file;
    char *p, *m0, *m7, *b164, *text, *b168, *t0, *t7, *s16c, *s170;
    int i;

    memset(gSzModel_ahModel, 0, sizeof(gSzModel_ahModel));
    bind_model_calls = bind_loadcb_calls = 0;
    assert(lSzModel_BindModelByAddr(6, h) == 0 && bind_model_calls == 0 && bind_loadcb_calls == 0);

    gSzModel_ahModel[6] = (struct DotrObject *)&obj;
    memset(file, 0, sizeof(file));
    memset(&obj, 0x33, sizeof(obj));
    obj.sized_16c = (struct DotrBindSized *)&obj; obj.sized_170 = (struct DotrBindSized *)&obj;
    h->motion_offset = 0x100;
    pack_ncalls = 0; rebase_calls = 0;
    assert(lSzModel_BindModelByAddr(6, h) == 1);
    assert(obj.base == h && obj.desc == (char *)h + 16 && bind_model_arg == h && bind_model_calls == 1);
    assert(bind_loadcb_calls == 1 && bind_loadcb_arg == 6 && pack_ncalls == 0 && rebase_calls == 0);
    for (i = 0; i < 8; i++) assert(obj.values[i] == 1 && obj.heads[i] == 0 && obj.texts[i] == 0);
    assert(obj.block_164 == 0 && obj.text == 0 && obj.block_168 == 0 && obj.block_174 == 0);
    assert(obj.sized_16c == (struct DotrBindSized *)&obj && obj.sized_170 == (struct DotrBindSized *)&obj);

    h->flags = 0x81 | 0x100 | 0x200 | 0x400 | 0x800 | 0x40000 | 0x80000 | 0x100000 | 0x200000;
    p = (char *)h + 0x100;
    m0 = p; ((int *)p)[1] = 64; ((struct DotrBindMotion *)p)->first = firsts[0]; p += 64;
    m7 = p; ((int *)p)[1] = 96; ((struct DotrBindMotion *)p)->first = firsts[1]; p += 96;
    b164 = p; p += 0x8000;
    text = p; ((struct DotrBindSized *)p)->size = 32; p += 32;
    b168 = p; p += 0x8000;
    t0 = p; ((struct DotrBindSized *)p)->size = 16; p += 16;
    t7 = p; ((struct DotrBindSized *)p)->size = 48; p += 48;
    s16c = p; ((struct DotrBindSized *)p)->size = 17; p += 32;
    s170 = p; ((struct DotrBindSized *)p)->size = 16; p += 16;
    memset(&obj, 0x33, sizeof(obj));
    pack_ncalls = 0; rebase_calls = 0; bind_model_calls = bind_loadcb_calls = 0;
    assert(lSzModel_BindModelByAddr(6, h) == 1);
    assert(bind_model_calls == 1 && bind_loadcb_calls == 1 && rebase_calls == 2);
    assert(rebase_arg == (struct DotrSlotHead *)m7);
    assert(obj.heads[0] == (struct DotrBindMotion *)m0 && obj.values[0] == 31);
    assert(obj.heads[7] == (struct DotrBindMotion *)m7 && obj.values[7] == 47);
    for (i = 1; i < 7; i++) assert(obj.heads[i] == 0 && obj.values[i] == 1 && obj.texts[i] == 0);
    assert(obj.block_164 == b164 && obj.text == (struct DotrBindSized *)text && obj.block_168 == b168);
    assert(obj.texts[0] == (struct DotrBindSized *)t0 && obj.texts[7] == (struct DotrBindSized *)t7);
    assert(pack_ncalls == 3 && pack_calls[0] == text && pack_calls[1] == t0 && pack_calls[2] == t7);
    assert(obj.sized_16c == (struct DotrBindSized *)s16c && obj.sized_170 == (struct DotrBindSized *)s170);
    assert(obj.block_174 == p);
    gSzModel_ahModel[6] = 0;
    pack_ncalls = 0; rebase_calls = 0;
}

char data_002cfa1d[] = "%s/%s/%s.rh0";

/* 0x1f0390 branch coverage: index below 0 and at the bound, empty slot, read not completed
 * (request refused and accepted), completed with a wrong first word, completed with 0x304852. */
static void check_model_pack_callback(void)
{
    static union { struct DotrObject o; struct DotrBindObject b; } u;
    static int file[0x100];
    struct DotrResourceSubmitRecord rec;
    char *saved_kind = gSzModel_apchBase[3];

    memset(&rec, 0, sizeof(rec));
    memset(&u, 0, sizeof(u));
    memset(file, 0, sizeof(file));
    memset(gSzModel_ahModel, 0, sizeof(gSzModel_ahModel));
    gSzModel_apchBase[3] = "kd";
    rec.unknown_014 = (int)(intptr_t)file;
    submit_calls = 0; label_calls = 0; bind_model_calls = 0; bind_loadcb_calls = 0;
    rec.unknown_01c = -1; lSzModel_LoadModelPackCB(&rec, 0);
    rec.unknown_01c = 84; lSzModel_LoadModelPackCB(&rec, 0);
    rec.unknown_01c = 9; lSzModel_LoadModelPackCB(&rec, 0);
    assert(submit_calls == 0 && label_calls == 0 && bind_model_calls == 0);

    gSzModel_ahModel[9] = &u.o;
    u.o.type = 3; u.o.modelNo = 12;
    submit_result = 0;
    lSzModel_LoadModelPackCB(&rec, 0);
    assert(label_calls == 1 && label_arg == data_002cfa17 && submit_calls == 1);
    assert(strcmp(submit_names[0], "root/kd/kd.rh0") == 0);
    assert(submit_args[0] == 9 && submit_args[1] == 0 && submit_args[2] == -1 && submit_args[3] == 9);
    assert(submit_callback == lSzModel_LoadModelCB);
    assert(u.o.drawFlag.unknown_bit_0 != 0 && u.o.drawFlag.unknown_bit_3 == 0);

    submit_result = (struct DotrResourceQueueRecord *)&rec;
    u.o.drawFlag.unknown_bit_0 = 0;
    lSzModel_LoadModelPackCB(&rec, 0);
    assert(submit_calls == 2 && u.o.drawFlag.unknown_bit_0 != 0 && u.o.drawFlag.unknown_bit_3 != 0);

    file[0] = 0x304853;
    lSzModel_LoadModelPackCB(&rec, 1);
    assert(submit_calls == 3 && bind_model_calls == 0);

    file[0] = 0x304852; file[3] = 0x40;
    lSzModel_LoadModelPackCB(&rec, 1);
    assert(submit_calls == 3 && bind_model_calls == 1 && bind_model_arg == file);
    assert(u.b.base == (struct DotrBindHeader *)file && bind_loadcb_calls == 1 && bind_loadcb_arg == 9);

    submit_result = 0; submit_calls = 0; label_calls = 0; pack_ncalls = 0; rebase_calls = 0;
    gSzModel_ahModel[9] = 0;
    gSzModel_apchBase[3] = saved_kind;
}

static int cache_calls, cache_mode, sort_calls, sort_args[7], sort_mode, sort_low, sort_keep;
static struct DotrObjectQuad sort_quad;
static struct DotrObjectVector3 sort_vectors[2];
static signed char sort_bytes[2];
void SzModel_SetDrawCache(int v)
{
    assert(v == 0);
    if (cache_calls++ == 0) {
        cache_mode = gSzModel_nBlurMode[0];
    }
}
int SzModel_ExSort_Pipe(int cursor, int index, struct DotrSideVector *v, int a, int b, int c, int d)
{
    struct DotrObject *o = gSzModel_ahModel[index];

    sort_calls++;
    sort_args[0] = cursor;
    sort_args[1] = index;
    sort_args[2] = v != 0;
    sort_args[3] = a;
    sort_args[4] = b;
    sort_args[5] = c;
    sort_args[6] = d;
    sort_mode = gSzModel_nBlurMode[0];
    sort_quad = o->rgba;
    sort_low = o->alphaMode.unknown_bits_0_1;
    sort_keep = o->alphaMode.unknown_bits_2_3;
    memcpy(sort_vectors, o->unknown_198, sizeof(sort_vectors));
    memcpy(sort_bytes, o->unknown_1e0, 2);
    return cursor + 10;
}

/* SzModel_DrawMotionBlur_Pipe (branch and loop coverage): free entry, other index, entry whose model is not
 * displayed (released), flag against the sign of the request both ways, blended draw through
 * each of the two draw calls, restore, frame count below and at the total, cache call once
 * before the first draw and once after the loop or not at all. The null-object branch after a
 * successful display test cannot be reached with the real SzModel_IsModelDisp of this group. */
static void check_draw_motion_blur(void)
{
    static struct DotrObject o5, o6;
    struct DotrObjectCopy112 *e = data_0031ad00_copy;
    struct DotrObject *saved5 = gSzModel_ahModel[5], *saved6 = gSzModel_ahModel[6], *saved9 = gSzModel_ahModel[9];
    struct DotrObjectVector3 own[2] = {{1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}};
    struct DotrObjectVector3 blur[2] = {{11.0f, 12.0f, 13.0f}, {14.0f, 15.0f, 16.0f}};
    struct DotrObjectQuad from = {10, 20, 30, 40}, to = {110, 120, 130, 140}, mine = {1, 2, 3, 4};
    int i;

    memset(&o5, 0, sizeof(o5));
    memset(&o6, 0, sizeof(o6));
    memset(e, 0, 32 * sizeof(*e));
    for (i = 0; i < 32; i++) {
        e[i].index = -1;
    }
    o5.drawFlag.unknown_bit_1 = -1;
    o6.drawFlag.unknown_bit_1 = -1;
    memcpy(o5.unknown_198, own, sizeof(own));
    o5.unknown_1e0[0] = -3;
    o5.unknown_1e0[1] = -4;
    o5.rgba = mine;
    o5.alphaMode.unknown_bits_0_1 = 1;
    o5.alphaMode.unknown_bits_2_3 = 3;
    gSzModel_ahModel[5] = &o5;
    gSzModel_ahModel[6] = &o6;
    gSzModel_ahModel[9] = 0;

    /* Nothing active: the cursor comes back, no cache call, mode word cleared. */
    gSzModel_nBlurMode[0] = 7;
    cache_calls = sort_calls = inner_calls = 0;
    assert(SzModel_DrawMotionBlur_Pipe(100, -1) == 100 && cache_calls == 0 && gSzModel_nBlurMode[0] == 0);

    memcpy(e[3].unknown_40, blur, sizeof(blur));
    e[3].unknown_58[0] = 7;
    e[3].unknown_58[1] = 8;
    e[3].unknown_5a = from;
    e[3].unknown_5e = to;
    e[3].unknown_62 = 4;
    e[3].unknown_64 = 1;
    e[3].index = 5;
    e[3].mode = 2;
    e[3].flag = -1;
    e[4].unknown_62 = 2;
    e[4].unknown_64 = 1;
    e[4].index = 6;
    e[7].index = 9;
    e[7].mode = 3;

    /* Negative request: only flagged entries; entry 7's model is not displayed and is released. */
    rows_calls = 0;
    assert(SzModel_DrawMotionBlur_Pipe(100, -1) == 110 && sort_calls == 1 && inner_calls == 0);
    assert(cache_calls == 2 && cache_mode == 2 && sort_mode == 2 && gSzModel_nBlurMode[0] == 0);
    assert(sort_args[0] == 100 && sort_args[1] == 5 && sort_args[2] == 0 && sort_args[3] == 0 && sort_args[4] == 0);
    assert(sort_args[5] == 0 && sort_args[6] == (int)0x80fffff5);
    assert(sort_quad.unknown_0 == 35 && sort_quad.unknown_1 == 45 && sort_quad.unknown_2 == 55 && sort_quad.unknown_3 == 65);
    assert(sort_low == 2 && sort_keep == 3 && sort_bytes[0] == 7 && sort_bytes[1] == 8);
    assert(memcmp(sort_vectors, blur, sizeof(blur)) == 0);
    assert(memcmp(o5.unknown_198, own, sizeof(own)) == 0 && o5.unknown_1e0[0] == -3 && o5.unknown_1e0[1] == -4);
    assert(memcmp(&o5.rgba, &mine, 4) == 0 && o5.alphaMode.unknown_bits_0_1 == 1 && o5.alphaMode.unknown_bits_2_3 == 3);
    assert(memcmp(&e[0].unknown_5a, &mine, 4) == 0 && e[0].mode == 1 && memcmp(e[0].unknown_40, own, sizeof(own)) == 0);
    assert(rows_calls == 3 && rows_destination == (void *)&o5 && rows_source == (void *)&e[0] && rows_count == 4);
    assert(e[3].unknown_64 == 2 && e[3].index == 5 && e[4].unknown_64 == 1 && e[4].index == 6);
    assert(e[7].index == -1 && e[7].mode == 3);

    /* Request for model 5: its only entry is flagged, so nothing is drawn. */
    cache_calls = sort_calls = 0;
    assert(SzModel_DrawMotionBlur_Pipe(100, 5) == 100 && cache_calls == 0 && sort_calls == 0 && inner_calls == 0);
    assert(e[3].unknown_64 == 2);

    /* Request for model 6: the unflagged entry goes through the plain draw; its frame count
     * reaches the total and the entry is released. */
    assert(SzModel_DrawMotionBlur_Pipe(100, 6) == 101 && inner_calls == 1 && sort_calls == 0);
    assert(cache_calls == 2 && cache_mode == 1 && gSzModel_nBlurMode[0] == 0);
    assert(inner_args[0] == 100 && inner_args[1] == 6 && inner_args[2] == 0 && inner_pointer == 0);
    assert(inner_args[4] == 0 && inner_args[5] == 0 && inner_args[6] == (int)0x80fffff5);
    assert(e[4].unknown_64 == 2 && e[4].index == -1 && e[3].index == 5 && e[3].unknown_64 == 2);

    /* Entry 3 fades on until its count reaches the total of 4. */
    assert(SzModel_DrawMotionBlur_Pipe(0, -1) == 10 && sort_quad.unknown_0 == 60 && e[3].unknown_64 == 3 && e[3].index == 5);
    assert(SzModel_DrawMotionBlur_Pipe(0, -1) == 10 && sort_quad.unknown_0 == 85 && e[3].unknown_64 == 4 && e[3].index == -1);
    assert(SzModel_DrawMotionBlur_Pipe(0, -1) == 0);

    gSzModel_ahModel[5] = saved5;
    gSzModel_ahModel[6] = saved6;
    gSzModel_ahModel[9] = saved9;
}

static struct DotrObject motion_object;
static struct DotrObjectLink motion_link;

static struct DotrObject *motion_reset(void)
{
    struct DotrObject *p = &motion_object;
    memset(p, 0, sizeof(*p));
    p->modelBaseData = &motion_link;
    p->drawFlag.unknown_bit_1 = -1;
    p->unknown_1e0[1] = -1;
    p->unknown_1e0[3] = -1;
    gSzModel_ahModel[82] = p;
    cue_step = 1;
    return p;
}

static struct DotrObject *motion_blend(int direction, float y)
{
    struct DotrObject *p = motion_reset();
    p->unknown_1e0[1] = 1;
    p->animationDataPointers[1] = 1;
    p->animationFrameCountArray[1] = 11;
    p->unknown_198[1].x = 2.0f;
    p->unknown_198[1].y = y;
    p->animDir.unknown_bits_6_7 = direction;
    p->blendTickTotal = 4;
    p->blendTickCurrent = 1;
    return p;
}

static struct DotrObject *motion_main(int current, int direction, float y)
{
    struct DotrObject *p = motion_reset();
    p->unknown_1e0[0] = current;
    p->animationDataPointers[current] = 1;
    p->animationFrameCountArray[current] = 21;
    p->animationFrameCountArray[5] = 31;
    p->unknown_198[0].x = 3.0f;
    p->unknown_198[0].y = y;
    p->unknown_198[0].z = 0.5f;
    p->animDir.unknown_bits_6_7 = direction;
    p->motionDelay = 7;
    return p;
}

/* SzModel_Motion: one assertion group per branch outcome and boundary. */
static void check_motion(void)
{
    struct DotrObject *p;
    struct DotrObject *saved = gSzModel_ahModel[82];

    /* Rejections. */
    p = motion_reset();
    assert(SzModel_Motion(-1) == 0 && SzModel_Motion(84) == 0);
    gSzModel_ahModel[82] = 0;
    assert(SzModel_Motion(82) == 0);
    p = motion_reset();
    p->modelId = -1;
    assert(SzModel_Motion(82) == 0);
    p = motion_reset();
    p->modelBaseData = 0;
    assert(SzModel_Motion(82) == 0);
    p = motion_reset();
    p->drawFlag.unknown_bit_3 = -1;
    assert(SzModel_Motion(82) == 0);
    p = motion_reset();
    p->drawFlag.unknown_bit_1 = 0;
    assert(SzModel_Motion(82) == 0);
    /* Paused: returns 1 before any update. */
    p = motion_reset();
    p->stateFlags.unknown_bit_0 = -1;
    p->deformRatio = 5;
    assert(SzModel_Motion(82) == 1 && p->deformRatio == 5);

    /* Blend slot, forward: wrap at the last frame, advance, clamp. */
    p = motion_blend(1, 10.0f);
    assert(SzModel_Motion(82) == 1 && p->unknown_198[1].y == 1.0f);
    p = motion_blend(1, 5.0f);
    assert(SzModel_Motion(82) == 1 && p->unknown_198[1].y == 7.0f);
    p = motion_blend(1, 9.0f);
    assert(SzModel_Motion(82) == 1 && p->unknown_198[1].y == 10.0f);
    p = motion_blend(1, 5.0f);
    cue_step = 2;
    assert(SzModel_Motion(82) == 1 && p->unknown_198[1].y == 9.0f);
    /* Blend slot, backward: wrap at frame 1, step back, clamp. */
    p = motion_blend(-1, 1.0f);
    assert(SzModel_Motion(82) == 1 && p->unknown_198[1].y == 10.0f);
    p = motion_blend(-1, 5.0f);
    assert(SzModel_Motion(82) == 1 && p->unknown_198[1].y == 3.0f);
    p = motion_blend(-1, 2.0f);
    assert(SzModel_Motion(82) == 1 && p->unknown_198[1].y == 1.0f);
    /* Blend slot, stopped: frame kept; the blend ratio still advances. */
    p = motion_blend(0, 5.0f);
    assert(SzModel_Motion(82) == 1 && p->unknown_198[1].y == 5.0f);
    assert(p->unknown_198[0].z == 0.25f && p->blendTickCurrent == 2 && p->unknown_198[1].z == 0.75f);
    assert(p->unknown_1e0[1] == 1);
    /* Blend ratio reaches its length: the blend slot is released. */
    p = motion_blend(0, 5.0f);
    p->blendTickCurrent = 3;
    assert(SzModel_Motion(82) == 1 && p->unknown_1e0[1] == -1 && p->blendTickCurrent == 0);
    assert(p->unknown_198[0].z == 1.0f && p->unknown_198[1].z == 0.0f);
    /* No blend slot, or a blend slot without data: nothing advances. */
    p = motion_blend(1, 5.0f);
    p->unknown_1e0[1] = -1;
    assert(SzModel_Motion(82) == 1 && p->unknown_198[1].y == 5.0f && p->blendTickCurrent == 1);
    p = motion_blend(1, 5.0f);
    p->animationDataPointers[1] = 0;
    assert(SzModel_Motion(82) == 1 && p->unknown_198[1].y == 5.0f && p->blendTickCurrent == 1);

    /* Current slot without data: skipped. */
    p = motion_main(2, 1, 5.0f);
    p->animationDataPointers[2] = 0;
    assert(SzModel_Motion(82) == 1 && p->unknown_198[0].y == 5.0f);
    /* Stopped: frame kept. */
    p = motion_main(2, 0, 5.0f);
    assert(SzModel_Motion(82) == 1 && p->unknown_198[0].y == 5.0f);
    /* Forward: advance, clamp. */
    p = motion_main(2, 1, 5.0f);
    assert(SzModel_Motion(82) == 1 && p->unknown_198[0].y == 8.0f);
    p = motion_main(2, 1, 19.0f);
    assert(SzModel_Motion(82) == 1 && p->unknown_198[0].y == 20.0f);
    /* Forward at the last frame: wrap; no next slot, same slot, new slot. */
    p = motion_main(2, 1, 20.0f);
    assert(SzModel_Motion(82) == 1 && p->unknown_198[0].y == 1.0f && p->unknown_1e0[0] == 2 && p->motionDelay == 7);
    p = motion_main(2, 1, 20.0f);
    p->unknown_1e0[3] = 2;
    assert(SzModel_Motion(82) == 1 && p->unknown_198[0].y == 1.0f && p->unknown_1e0[0] == 2 && p->motionDelay == 7);
    p = motion_main(2, 1, 20.0f);
    p->unknown_1e0[3] = 5;
    p->unknown_1e0[1] = 1; /* no data in slot 1: the blend block is skipped */
    assert(SzModel_Motion(82) == 1 && p->unknown_1e0[0] == 5 && p->unknown_1e0[1] == -1 && p->motionDelay == 0);
    assert(p->unknown_198[1].x == 3.0f && p->unknown_198[1].y == 1.0f && p->unknown_198[1].z == 0.5f);
    assert(p->unknown_198[0].y == 1.0f);
    /* Backward: step back, clamp. */
    p = motion_main(2, -1, 5.0f);
    assert(SzModel_Motion(82) == 1 && p->unknown_198[0].y == 2.0f);
    p = motion_main(2, -1, 3.0f);
    assert(SzModel_Motion(82) == 1 && p->unknown_198[0].y == 1.0f);
    /* Backward at frame 1: wrap; no next slot, same slot, new slot. */
    p = motion_main(2, -1, 1.0f);
    assert(SzModel_Motion(82) == 1 && p->unknown_198[0].y == 20.0f && p->unknown_1e0[0] == 2 && p->motionDelay == 7);
    p = motion_main(2, -1, 1.0f);
    p->unknown_1e0[3] = 2;
    assert(SzModel_Motion(82) == 1 && p->unknown_198[0].y == 20.0f && p->unknown_1e0[0] == 2 && p->motionDelay == 7);
    p = motion_main(2, -1, 1.0f);
    p->unknown_1e0[3] = 5;
    assert(SzModel_Motion(82) == 1 && p->unknown_1e0[0] == 5 && p->unknown_1e0[1] == -1 && p->motionDelay == 0);
    assert(p->unknown_198[1].x == 3.0f && p->unknown_198[1].y == 20.0f && p->unknown_198[1].z == 0.5f);
    assert(p->unknown_198[0].y == 30.0f);
    /* Slots 3 and 4 stop at the last frame instead of wrapping. */
    p = motion_main(3, 1, 20.0f);
    assert(SzModel_Motion(82) == 1 && p->animDir.unknown_bits_6_7 == 0 && p->unknown_198[0].y == 20.0f);
    p = motion_main(4, -1, 20.0f);
    assert(SzModel_Motion(82) == 1 && p->animDir.unknown_bits_6_7 == 0 && p->unknown_198[0].y == 20.0f);
    p = motion_main(3, 1, 5.0f);
    assert(SzModel_Motion(82) == 1 && p->animDir.unknown_bits_6_7 == 1 && p->unknown_198[0].y == 8.0f);
    /* Slot 6 holds at frame 10 for 15 calls. */
    p = motion_main(6, 1, 10.0f);
    p->motionDelay = 14;
    assert(SzModel_Motion(82) == 1 && p->motionDelay == 15 && p->unknown_198[0].y == 10.0f);
    p = motion_main(6, 1, 10.0f);
    p->motionDelay = 15;
    assert(SzModel_Motion(82) == 1 && p->motionDelay == 15 && p->unknown_198[0].y == 13.0f);
    p = motion_main(6, 1, 9.0f);
    assert(SzModel_Motion(82) == 1 && p->motionDelay == 7 && p->unknown_198[0].y == 12.0f);
    p = motion_main(6, 0, 10.0f);
    assert(SzModel_Motion(82) == 1 && p->motionDelay == 7 && p->unknown_198[0].y == 10.0f);

    /* Trailing counter: up to 32 while the bit is set, down to 0 otherwise. */
    p = motion_reset();
    p->drawFlag.unknown_bit_4 = -1;
    p->deformRatio = 31;
    assert(SzModel_Motion(82) == 1 && p->deformRatio == 32);
    assert(SzModel_Motion(82) == 1 && p->deformRatio == 32);
    p->drawFlag.unknown_bit_4 = 0;
    p->deformRatio = 1;
    assert(SzModel_Motion(82) == 1 && p->deformRatio == 0);
    assert(SzModel_Motion(82) == 1 && p->deformRatio == 0);
    gSzModel_ahModel[82] = saved;
}

/* SzModel_Load (branch and loop coverage). The loaders are the real ones of this group; a
 * loader is made to refuse by letting the SzFile_GetEnv stub set the object's busy bit. */
unsigned short gSzModel_awIgnore[8];
int anOfs_MODEL[90];
struct DotrObjectPackTriple data_00292f20 = { { 3, 13, 23 } };
struct DotrObjectPackTriple data_00292f30 = { { 0x1000000, 0x2000000, 0x3000000 } };
char data_002cf9dd[] = "one-%s", data_002cf9f0[] = "PACK", data_002cf9f5[] = "pack-%s";
char data_002cfa0a[] = "%s;%s;%s;%%d;%%d"; /* data_002cfa1d above has no number field */
static int xax_calls, xax_arg, xax_result;
int SzDuel_Xax2New(int value)
{
    xax_calls++;
    xax_arg = value;
    return xax_result;
}
static int run_load(int kind, int number, int packed, int busy)
{
    int r;

    submit_calls = param_calls = label_calls = xax_calls = wait_calls = 0;
    submit_result = 0;
    param_name[0] = 0;
    memset(submit_names, 0, sizeof(submit_names));
    env_busy = busy;
    r = SzModel_Load(83, kind, number, packed);
    env_busy = 0;
    object.drawFlag.unknown_bit_3 = 0;
    return r;
}
static void check_model_load(void)
{
    int i, kind;

    reset();
    for (i = 0; i < 8; i++) {
        gSzModel_awIgnore[i] = 1000;
        gSzModel_apchBase[i] = "kd";
    }
    for (i = 0; i < 90; i++) {
        anOfs_MODEL[i] = i * i * 16;
    }
    gSzModel_fPacking[0] = 0;
    label_mask = 0;
    /* index bounds and the empty entry: nothing is touched */
    assert(SzModel_Load(-1, 2, 5, 0) == 0 && SzModel_Load(84, 2, 5, 0) == 0 && SzModel_Load(7, 2, 5, 0) == 0);
    assert(object.modelId == 0);
    /* waits while the busy bit is set; stores class and number; clears bit 2 */
    object.drawFlag.unknown_bit_3 = -1;
    object.drawFlag.unknown_bit_2 = -1;
    wait_remaining = 3;
    submit_calls = wait_calls = 0;
    memset(submit_names, 0, sizeof(submit_names));
    assert(SzModel_Load(83, 2, 5, 0) == 1 && wait_calls == 3 && submit_calls == 1);
    assert(object.modelId == -1 && object.type == 2 && object.modelNo == 5);
    assert(object.drawFlag.unknown_bit_2 == 0 && label_arg == data_002cfa17);
    assert(strcmp(submit_names[0], "root/kd/kd.rh0") == 0);
    /* negative class and number keep the stored ones */
    assert(run_load(-1, -1, 0, 0) == 1 && wait_calls == 0 && object.type == 2 && object.modelNo == 5);
    assert(strcmp(submit_names[0], "root/kd/kd.rh0") == 0);
    /* the number against the class's count, and a stored negative number */
    gSzModel_awIgnore[2] = 6;
    assert(run_load(2, 6, 0, 0) == 0 && submit_calls == 0 && object.modelNo == 6);
    assert(run_load(2, 5, 0, 0) == 1 && submit_calls == 1);
    object.modelNo = -3;
    assert(run_load(2, -1, 0, 0) == 0 && submit_calls == 0);
    gSzModel_awIgnore[2] = 1000;
    /* the plain loader refuses */
    assert(run_load(2, 5, 0, 1) == 0 && submit_calls == 0);
    /* packed without the packing flag: the plain loader */
    assert(run_load(2, 5, 1, 0) == 1 && submit_calls == 1 && param_calls == 0);
    /* packing flag, label absent: the whole file through the pack loader */
    gSzModel_fPacking[0] = 1;
    assert(run_load(4, 5, 1, 0) == 1 && submit_calls == 0 && param_calls == 1 && xax_calls == 0);
    assert(strcmp(param_name, "root;kd;kd;5;5") == 0 && label_arg == data_002cfa17);
    assert(param_args[2] == 0 && param_args[3] == -1 && param_args[4] == 0 && param_args[5] == -1);
    assert(run_load(4, 5, 1, 1) == 0 && param_calls == 0);
    /* packing flag without `packed`: the plain loader */
    assert(run_load(4, 5, 0, 0) == 1 && submit_calls == 1 && param_calls == 0);
    /* label present: classes 4..6 read a megabyte per monster from the pack */
    label_mask = 2;
    for (kind = 4; kind <= 6; kind++) {
        xax_result = 682;
        assert(run_load(kind, 5, 1, 0) == 1 && xax_calls == 1 && xax_arg == 5 && param_calls == 1);
        assert(label_arg == data_002cf9f0 && strcmp(param_name, "pack-root") == 0);
        assert(param_args[2] == (kind - 3) * 0x1000000 + ((83 - (3 + (kind - 4) * 10)) << 20));
        assert(param_args[3] == 0x100000 && param_args[4] == 682 << 20 && param_args[5] == 0x100000);
    }
    xax_result = 683;
    assert(run_load(4, 5, 1, 0) == 0 && xax_calls == 1 && param_calls == 0);
    /* number 737 is monster 671 without asking */
    assert(run_load(4, 737, 1, 0) == 1 && xax_calls == 0 && param_args[4] == 671 << 20);
    assert(run_load(4, 737, 1, 1) == 0 && param_calls == 0);
    /* class 1: numbers 0..20 and 100..120 are entries 39.. and 60.. of the offset table */
    assert(run_load(1, 0, 1, 0) == 1 && param_calls == 1 && strcmp(param_name, "one-root") == 0);
    assert(param_args[2] == 0x10aa800 && param_args[4] == 39 * 39 * 16 && param_args[3] == (40 * 40 - 39 * 39) * 16);
    assert(param_args[5] == param_args[3] && xax_calls == 0);
    assert(run_load(1, 20, 1, 0) == 1 && param_args[4] == 59 * 59 * 16);
    assert(run_load(1, 120, 1, 0) == 1 && param_args[2] == 0x10aa800 + 0xa000 && param_args[4] == 80 * 80 * 16);
    assert(run_load(1, 21, 1, 0) == 0 && param_calls == 0);
    assert(run_load(1, 200, 1, 0) == 0 && param_calls == 0);
    assert(run_load(1, 0, 1, 1) == 0 && param_calls == 0);
    /* label present, another class: the whole file */
    assert(run_load(2, 5, 1, 0) == 1 && param_calls == 1 && strcmp(param_name, "root;kd;kd;5;5") == 0);
    gSzModel_fPacking[0] = 0;
    label_mask = 0;
    reset();
}

struct DotrModelShare aShareData[7];
struct DotrModelMergeWords data_00292be0;
struct DotrModelMergeInts data_00292c10, data_00292c40, data_00292c70;
short gSzDuel_DuelData[2][5], gSzDuel_RingData[2][1], gSzDuel_StageData[2][12], gSzDuel_EventData[2][40], gSzDuel_FieldData[2][10];
static int item_calls, item_args[2], item_found, free_calls, free_args[8][2], del_calls, del_args[24][2];

struct DotrEntryTableEntry *SzText_GetItem(int group, int id)
{
    item_calls++;
    item_args[0] = group;
    item_args[1] = id;
    return item_found ? (struct DotrEntryTableEntry *)&item_found : 0;
}

int SzText_Free(int group, int id)
{
    free_args[free_calls][0] = group;
    free_args[free_calls][1] = id;
    free_calls++;
    return 0;
}

int SzDuel_DelUnit(int a, int b)
{
    del_args[del_calls][0] = a;
    del_args[del_calls][1] = b;
    del_calls++;
    return 0;
}

void lSzModel_LoadMergeCB(struct DotrResourceSubmitRecord *record, int completed)
{
    (void)record;
    (void)completed;
}

static int run_merge(int type, int no, int force)
{
    label_calls = item_calls = free_calls = del_calls = param_calls = 0;
    return SzModel_LoadMerge(type, no, force);
}

char data_002cfa30[] = "%s/%s/%%d";
char data_002cfa3a[] = ".rh2";
char data_002cfa3f[] = ".anm";
static int append_calls;

char *func_0019b9d0(char *destination, const char *source)
{
    append_calls++;
    return strcat(destination, source);
}

void lSzModel_LoadTextureFileCB(struct DotrResourceSubmitRecord *record, int completed)
{
    (void)record;
    (void)completed;
}

void lSzModel_LoadTexAnimFileCB(struct DotrResourceSubmitRecord *record, int completed)
{
    (void)record;
    (void)completed;
}

/* Branch coverage: index bounds on both sides, empty slot, clear base, clear
 * descriptor word, a table of zero entries and one of two with every submitted
 * name, callback and word. Synthetic strings and objects; the test format takes
 * the number as %d (the image's format has %s there). */
static void check_texture_file(void)
{
    unsigned char *page = mmap(0, 8192, PROT_READ | PROT_WRITE,
                               MAP_PRIVATE | MAP_ANONYMOUS | MAP_32BIT, -1, 0);
    struct DotrLoadObject *obj = (struct DotrLoadObject *)page;
    struct DotrObjPackView *view = (struct DotrObjPackView *)page;
    struct DotrObjPackDesc *desc = (struct DotrObjPackDesc *)(page + 0x800);
    struct DotrObjPackTable *table = (struct DotrObjPackTable *)(page + 0x1000);
    int tag;
    int i;

    assert(page != MAP_FAILED);
    assert(offsetof(struct DotrObjPackView, base) == 0x138);
    memset(gSzModel_ahModel_load, 0, sizeof(gSzModel_ahModel_load));
    gSzModel_apchBase[2] = "kd";
    env_busy = 0;
    obj->kind = 2;
    obj->number = 7;
    view->desc = desc;
    desc->unknown_28 = 0x1000 - 0x400;
    for (i = 0; i < 2; i++) {
        table->entries[i].a = 1;
        table->entries[i].b = 2;
        table->entries[i].c = 3;
        table->entries[i].d = 4;
    }

    submit_calls = 0;
    label_calls = 0;
    append_calls = 0;
    assert(SzModel_LoadTextureFile(-1) == 0 && SzModel_LoadTextureFile(84) == 0);
    assert(SzModel_LoadTextureFile(0) == 0 && SzModel_LoadTextureFile(83) == 0);
    assert(label_calls == 0 && submit_calls == 0);

    /* base clear: label looked up, nothing built or submitted */
    gSzModel_ahModel_load[0] = obj;
    gSzModel_ahModel_load[83] = obj;
    assert(SzModel_LoadTextureFile(83) == 1);
    assert(label_calls == 1 && label_arg == data_002cfa17 && submit_calls == 0);

    /* descriptor word clear */
    view->base = (DotrObjectWord)(intptr_t)(page + 0x400);
    desc->unknown_2c = 0;
    assert(SzModel_LoadTextureFile(0) == 1);
    assert(label_calls == 2 && submit_calls == 0 && append_calls == 0);

    /* empty table: the format is built, no entry is touched */
    desc->unknown_2c = 1;
    table->count = 0;
    assert(SzModel_LoadTextureFile(0) == 1);
    assert(submit_calls == 0 && append_calls == 0 && table->entries[0].a == 1);

    table->count = 2;
    assert(SzModel_LoadTextureFile(83) == 1);
    assert(submit_calls == 4 && append_calls == 4);
    tag = (int)((unsigned int)(intptr_t)table->entries & 0x3ffffff);
    for (i = 0; i < 2; i++) {
        assert(strcmp(submit_names[i * 2], "root/kd/7.rh2") == 0);
        assert(strcmp(submit_names[i * 2 + 1], "root/kd/7.anm") == 0);
        assert(submit_callbacks[i * 2] == lSzModel_LoadTextureFileCB);
        assert(submit_callbacks[i * 2 + 1] == lSzModel_LoadTexAnimFileCB);
        assert(submit_words[i * 2] == (tag | (i << 26)));
        assert(submit_words[i * 2 + 1] == (tag | (i << 26)));
        assert(table->entries[i].a == 0 && table->entries[i].b == 2);
        assert(table->entries[i].c == 0 && table->entries[i].d == 0);
    }
    assert(submit_args[0] == 83 && submit_args[1] == 0 && submit_args[2] == -1);
    munmap(page, 8192);
}

static void check_model_load_merge(void)
{
    static struct DotrResourceQueueRecord some;
    static const int kinds[9] = { 1, 2, 3, 5, 6, 7, 0, 0, 4 };
    int i;

    reset();
    env_busy = 0;
    wait_remaining = 0;
    for (i = 0; i < 90; i++) {
        anOfs_MODEL[i] = i * i * 16;
    }
    for (i = 0; i < 8; i++) {
        gSzModel_awIgnore[i] = 1000;
    }
    for (i = 0; i < 7; i++) {
        aShareData[i].addr = 0x1000 * (i + 1);
        aShareData[i].ofs = i + 2;
        aShareData[i].mask = 0;
    }
    for (i = 0; i < 9; i++) {
        data_00292c10.v[i] = kinds[i];
        data_00292c40.v[i] = 0;
        data_00292c70.v[i] = 7; /* a null entry unless set below */
    }
    aShareData[0].mask = 0x100; /* type 0 checks group 8, class 4, the one object 83 */
    data_00292c40.v[8] = 1;
    data_00292c70.v[8] = 83;
    data_00292c40.v[0] = 1; /* type 0's own group: one object, 83, class 1 */
    data_00292c70.v[0] = 83;
    gSzDuel_DuelData[1][0] = 5; /* the number wanted for `no` 1 */
    submit_result = &some;

    /* type bounds: nothing is called */
    assert(run_merge(-1, 1, 1) == 0 && run_merge(7, 1, 1) == 0 && label_calls == 0 && free_calls == 0);
    /* nothing standing by and the wanted number loaded: 1 without a request */
    object.modelId = -1;
    object.modelNo = 5;
    assert(run_merge(0, 1, 0) == 1 && param_calls == 0 && del_calls == 0);
    assert(label_calls == 1 && label_arg == data_002cf9f0);
    /* the text item is absent: only the four releases of types other than 3 */
    assert(item_calls == 1 && item_args[0] == 0 && item_args[1] == 0 && free_calls == 4);
    assert(free_args[0][0] == 0 && free_args[0][1] == 5 && free_args[1][0] == 0 && free_args[1][1] == 6);
    assert(free_args[2][0] == 7 && free_args[2][1] == -1 && free_args[3][0] == 8 && free_args[3][1] == -1);
    /* the text item is present: two more releases first */
    item_found = 1;
    assert(run_merge(0, 1, 0) == 1 && free_calls == 6);
    assert(free_args[0][0] == 0 && free_args[0][1] == 0 && free_args[1][0] == 1 && free_args[1][1] == -1);
    assert(free_args[2][1] == 5);
    /* a wanted number at or above the class's count is not compared */
    object.modelNo = 6;
    gSzModel_awIgnore[1] = 5;
    assert(run_merge(0, 1, 0) == 1 && param_calls == 0);
    /* a different loaded number: presets and requests; class 4 deletes units 1..10 of both sides */
    gSzModel_awIgnore[1] = 6;
    assert(run_merge(0, 1, 0) == 1 && param_calls == 1 && del_calls == 20);
    assert(del_args[0][0] == 0 && del_args[0][1] == 1 && del_args[1][0] == 1 && del_args[1][1] == 1);
    assert(del_args[19][0] == 1 && del_args[19][1] == 10);
    assert(object.modelNo == 5 && object.modelId == -1);
    assert(object.drawFlag.unknown_bit_2 == 0 && object.drawFlag.unknown_bit_3 != 0);
    assert(strcmp(param_name, "one-root") == 0 && param_args[0] == 0 && param_args[1] == 0);
    /* entry no + ofs = 3 of the offsets: address, size, start, size, callback, entry */
    assert(param_args[2] == 0x1000 && param_args[3] == (16 - 9) * 16 && param_args[4] == 9 * 16);
    assert(param_args[5] == (16 - 9) * 16 && param_args[6] == 3);
    assert(merge_callback == (DotrMergeAddr)lSzModel_LoadMergeCB);
    /* a standing-by object forces it too; a refused request gives 0 and leaves the bits */
    reset();
    object.modelId = 0;
    object.modelNo = 5;
    submit_result = 0;
    assert(run_merge(0, 1, 0) == 0 && param_calls == 1 && del_calls == 20);
    assert(object.drawFlag.unknown_bit_2 != 0 && object.drawFlag.unknown_bit_3 == 0);
    submit_result = &some;
    /* force skips both checks; a group of another class deletes no unit; type 3 keeps the four
     * releases away */
    reset();
    object.modelId = -1;
    object.modelNo = 5;
    aShareData[3].mask = 1;
    assert(run_merge(3, 0, 0) == 1 && param_calls == 0 && free_calls == 2);
    assert(run_merge(3, 0, 1) == 1 && param_calls == 1 && del_calls == 0 && free_calls == 2);
    assert(object.modelNo == 999 && param_args[0] == 3 && param_args[2] == 0x4000 && param_args[6] == 5);
    /* type 4 asks for no text item and gives group 7 nine objects (75..83 here) */
    reset();
    object.modelId = 0;
    aShareData[4].mask = 0x80;
    aShareData[5].mask = 0x80;
    data_00292c70.v[7] = 75;
    assert(run_merge(5, 0, 0) == 1 && param_calls == 0 && item_calls == 1);
    assert(run_merge(4, 0, 0) == 1 && param_calls == 1 && item_calls == 0 && free_calls == 4);
    assert(object.modelNo == 999 && param_args[0] == 4);
    item_found = 0;
}

int main(void)
{
    check_bit_wrappers();
    check_cursor_run();
    check_found_scale();
    check_pair_fade();
    check_table_walks();
    check_rejections();
    check_links();
    check_bits_and_bytes();
    check_floats();
    check_thunks();
    check_slots();
    check_more_fields();
    check_sweeps();
    check_table_control();
    check_slot_state();
    check_kind_call();
    check_init_all();
    check_pack_table();
    check_submit_object();
    check_submit_callback();
    check_clear_all();
    check_shade_levels();
    check_cues();
    check_entry_fill();
    check_make_object();
    check_texture_submit();
    check_attach();
    check_effect_callback();
    check_bind_model_by_addr();
    check_model_pack_callback();
    check_draw_motion_blur();
    check_motion();
    check_model_load();
    check_model_load_merge();
    check_texture_file();
    puts("object_table: ok");
    return 0;
}
