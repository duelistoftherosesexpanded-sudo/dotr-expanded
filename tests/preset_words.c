/* Synthetic tests of the four-word preset writers and the object initializer.
 * The selector routine is a stub; values are checked against the constants
 * in the executable. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/preset_words.h"

static int selector, selector_calls;

int SzDuel_GetTurnOwner(void)
{
    selector_calls++;
    return selector;
}

/* Stubs for the other-group callees of 0x255960, 0x255ac0, 0x255b50. */
static float float_a[4] = {1.5f, 2.5f, 3.5f, 4.5f};
static float float_b[4] = {-1.0f, -2.0f, -3.0f, -4.0f};
static int float_index[2];
float *func_001f9d30_floats(int index)
{
    float_index[0] = index + 100;
    return float_a;
}
float *func_001faf60_floats(int index)
{
    float_index[1] = index + 200;
    return float_b;
}

static int result_value, result_args[3], result_calls;
int func_001f3910_result(int a, int b, int c)
{
    result_calls++;
    result_args[0] = a;
    result_args[1] = b;
    result_args[2] = c;
    return result_value;
}

static unsigned char pool_states[24];
static int pool_calls, pool_args[24][2];
struct DotrJobPool *SzEffect_GetModuleInfo(int a, int b)
{
    pool_args[pool_calls][0] = a;
    pool_args[pool_calls][1] = b;
    pool_calls++;
    return pool_states[b & 0xff] == 0xff ? 0 : (struct DotrJobPool *)&pool_states[b & 0xff];
}

struct Case {
    void (*plain)(int *);
    void (*valued)(int *, int);
    int first[2];
    int second;
    int third;
};

static void check_presets(void)
{
    static const struct Case cases[] = {
        {Ta_GetStdCompleteView, 0, {500, 500}, 16383, 6143},
        {Ta_GetStdTopDownView, 0, {500, 700}, 16383, 16384},
        {Ta_GetFlatCompleteView, 0, {500, 500}, 8192, 1024},
        {Ta_GetDestinyView, 0, {122, 122}, 20480, -4096},
        {0, Ta_GetStdFocusView, {-31, -31}, 16383, 4096},
        {0, Ta_GetStdFocusView2, {-31, -31}, 20479, 6144},
        {Ta_GetTopView, 0, {250, 250}, 8192, -6144},
        {Ta_GetSkyView, 0, {500, 500}, 24576, -2048},
    };
    size_t i;
    int which;
    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        for (which = 0; which < 2; which++) {
            int out[5] = {-1, -1, -1, -1, 0x5a5a};
            selector = which ? -7 : 0;
            selector_calls = 0;
            if (cases[i].plain) {
                cases[i].plain(out);
            } else {
                cases[i].valued(out, -31);
            }
            /* A non-zero selector adds 0x8000 to the second word. */
            assert(selector_calls == 1 && out[0] == cases[i].first[which]);
            assert(out[1] == cases[i].second + (which ? 0x8000 : 0));
            assert(out[2] == cases[i].third && out[3] == 0 && out[4] == 0x5a5a);
        }
    }
}

static void check_object(void)
{
    struct DotrPresetObject object, expected;
    int i, fill;
    for (fill = 0; fill < 2; fill++) {
        memset(&object, fill ? 0xff : 0, sizeof(object));
        expected = object;
        TaEffect_ClearEffectTemp(&object);
        expected.unknown_80.unknown_bit_0 = 0;
        expected.unknown_80.unknown_bits_1_5 = 0;
        for (i = 0; i < 24; i++) {
            expected.unknown_82[i] = 0xffff;
            expected.unknown_b2[i] = 0;
        }
        assert(memcmp(&object, &expected, sizeof(expected)) == 0);
    }
}

static int step, submit_kinds[2], submit_words[4], vector_z = 1000;
static struct DotrPresetVector *vector_seen;
static float *converted_seen;

int SzView_FreeRequest(int a, int b)
{
    assert(step == 0 && a == 0 && b == 0);
    step++;
    return 0;
}

void SzDuel_CalcUnitPos(int x, int y, int z, struct DotrPresetVector *v)
{
    assert(step == 1 && x == 3 && y == 3 && z == vector_z);
    v->x = 1.0f;
    v->y = 2.0f;
    v->z = 3.0f;
    v->unknown_w = 4.0f;
    vector_seen = v;
    step++;
}

void SzView_IV2V(int *in, float *out)
{
    int i;
    assert(step == 2 && selector_calls == 1);
    for (i = 0; i < 4; i++) {
        submit_words[i] = in[i];
        out[i] = (float)(i + 10);
    }
    converted_seen = out;
    step++;
}

void SzView_ReqCamera(int a, int b, int kind, int d, int e, void *data)
{
    assert(step == 3 || step == 4);
    assert(a == 0 && b == 0 && d == 0 && e == 0);
    submit_kinds[step - 3] = kind;
    if (step == 3) {
        struct DotrPresetVector *v = data;
        assert(v == vector_seen && v->x == 1.0f && v->y == 2.0f && v->z == 3.0f && v->unknown_w == 4.0f);
    } else {
        float *f = data;
        assert(f == converted_seen && f[0] == 10.0f && f[3] == 13.0f);
    }
    step++;
}

static int submit_tag;

/* The 16-bit view used by 0x260740, 0x2607d0 and 0x260860: same checks, and
 * the fourth argument is the caller's byte. */
void func_001fa8a0_tag(int a, int b, int kind, unsigned short d, int e, void *data)
{
    assert(d == submit_tag);
    SzView_ReqCamera(a, b, kind, 0, e, data);
}

/* TaView_ReqStdCompleteView, TaView_ReqStdTopDownView, TaView_ReqFlatCompleteView (straight-line
 * coverage): call order, the constant arguments, the preset words for both selector outcomes and
 * the byte forwarded to both submits. */
static void check_tagged_submitters(void)
{
    static const struct { void (*f)(unsigned char); void (*preset)(int *); int z; } cases[3] = {
        {TaView_ReqStdCompleteView, Ta_GetStdCompleteView, 0},
        {TaView_ReqStdTopDownView, Ta_GetStdTopDownView, 0},
        {TaView_ReqFlatCompleteView, Ta_GetFlatCompleteView, 100},
    };
    int want[4];
    int r;

    for (r = 0; r < 3; r++) {
        vector_z = cases[r].z;
        for (selector = 0; selector < 2; selector++) {
            cases[r].preset(want);
            step = selector_calls = 0;
            submit_tag = selector ? 0xff : 0;
            cases[r].f((unsigned char)submit_tag);
            assert(step == 5 && submit_kinds[0] == 3 && submit_kinds[1] == 4);
            assert(memcmp(submit_words, want, sizeof(want)) == 0);
        }
    }
    selector = 0;
    vector_z = 1000;
    step = 0;
}

/* 0x261580 and 0x261600: call order, the constant arguments, the preset words
 * handed to the converter and the two submitted locals. */
static void check_submitters(void)
{
    static void (*const routines[2])(void) = {TaView_ReqTopView, TaView_ReqSkyView};
    static const int expected[2][2][4] = {
        {{250, 8192, -6144, 0}, {250, 0xa000, -6144, 0}},
        {{500, 24576, -2048, 0}, {500, 0xe000, -2048, 0}},
    };
    int r;
    for (r = 0; r < 2; r++) {
        for (selector = 0; selector < 2; selector++) {
            step = selector_calls = 0;
            routines[r]();
            assert(step == 5 && submit_kinds[0] == 3 && submit_kinds[1] == 4);
            assert(memcmp(submit_words, expected[r][selector], sizeof(submit_words)) == 0);
        }
    }
    selector = 0;
}

/* 0x2608f0 branch coverage: the low byte of the argument zero (five calls,
 * 0x255e90 words for both selector outcomes) or non-zero (0x1fa810 only). */
static void check_flagged_submitter(void)
{
    static const int expected[2][4] = {{122, 20480, -4096, 0}, {122, 0xd000, -4096, 0}};

    vector_z = 400;
    for (selector = 0; selector < 2; selector++) {
        step = selector_calls = 0;
        TaView_SetDestinyCamera(0);
        assert(step == 5 && submit_kinds[0] == 3 && submit_kinds[1] == 4);
        assert(memcmp(submit_words, expected[selector], sizeof(submit_words)) == 0);
    }
    selector = 0;
    step = selector_calls = 0;
    TaView_SetDestinyCamera(1);
    assert(step == 1 && selector_calls == 0);
    step = 0;
    TaView_SetDestinyCamera(255);
    assert(step == 1 && selector_calls == 0);
    vector_z = 1000;
    step = 0;
}

static void check_init_and_ids(void)
{
    struct DotrPresetInit init;
    struct DotrPresetObject obj;
    int i;

    /* 0x255960: straight-line initializer, one whole-object check. */
    memset(&init, 0x5a, sizeof(init));
    InitEffectTemp(&init);
    assert(init.word_00 == 0 && init.word_02 == 0 && init.word_04 == 0 && init.byte_d4 == 0);
    for (i = 0; i < 24; i++) {
        assert(init.bytes_06[i] == 255 && init.words_20[i] == 0);
    }
    assert(init.word_cc == 0 && init.word_d0 == 0 && init.byte_100 == 0 && init.word_102 == 0);
    assert(float_index[0] == 100 && float_index[1] == 200);
    assert(memcmp(init.vec_e0, float_a, 16) == 0 && memcmp(init.vec_f0, float_b, 16) == 0);

    /* 0x255ac0: callee refuses -> -1, nothing changes. */
    memset(&obj, 0, sizeof(obj));
    obj.unknown_80.unknown_bits_1_5 = 3;
    result_value = 0;
    assert(TaEffect_LoadModule(&obj, 7) == -1);
    assert(result_args[0] == 0 && result_args[1] == 7 && result_args[2] == 0);
    assert(obj.unknown_80.unknown_bits_1_5 == 3 && obj.unknown_82[3] == 0);
    /* Accepted: id appended, count incremented. */
    result_value = 1;
    assert(TaEffect_LoadModule(&obj, 9) == 1);
    assert(obj.unknown_82[3] == 9 && obj.unknown_80.unknown_bits_1_5 == 4);
    /* Count 23 -> 24 is the last in-range append; the 5-bit count wraps at 32. */
    obj.unknown_80.unknown_bits_1_5 = 23;
    assert(TaEffect_LoadModule(&obj, 5) == 1 && obj.unknown_82[23] == 5 && obj.unknown_80.unknown_bits_1_5 == 24);

    /* 0x255b50: empty list sets bit 0 and returns 1. */
    memset(&obj, 0, sizeof(obj));
    pool_calls = 0;
    assert(TaEffect_IsLoadModule(&obj) == 1 && obj.unknown_80.unknown_bit_0 == -1 && pool_calls == 0);
    /* Mixed list: 0xffff skipped, NULL entry untouched, state 0 -> 0, state 3 -> 3. */
    memset(&obj, 0, sizeof(obj));
    memset(obj.unknown_b2, 0x77, sizeof(obj.unknown_b2));
    obj.unknown_80.unknown_bits_1_5 = 4;
    obj.unknown_82[0] = 0xffff;
    obj.unknown_82[1] = 1;
    obj.unknown_82[2] = 2;
    obj.unknown_82[3] = 3;
    memset(pool_states, 0xf8, sizeof(pool_states));
    pool_states[1] = 0xff;
    pool_states[2] = 0x00;
    pool_states[3] = 0x03;
    assert(TaEffect_IsLoadModule(&obj) == 1 && obj.unknown_80.unknown_bit_0 == -1);
    assert(pool_calls == 3 && pool_args[0][0] == 0 && pool_args[0][1] == 1 && pool_args[2][1] == 3);
    assert(obj.unknown_b2[0] == 0x77 && obj.unknown_b2[1] == 0x77);
    assert(obj.unknown_b2[2] == 0 && obj.unknown_b2[3] == 3);
    /* State 1 returns -1 at once; later slots and bit 0 stay untouched. */
    memset(obj.unknown_b2, 0x77, sizeof(obj.unknown_b2));
    obj.unknown_80.unknown_bit_0 = 0;
    pool_states[2] = 0x01;
    assert(TaEffect_IsLoadModule(&obj) == -1 && obj.unknown_80.unknown_bit_0 == 0);
    assert(obj.unknown_b2[2] == 0x77 && obj.unknown_b2[3] == 0x77);
}

static void check_kind_dispatch(void)
{
    static const struct { unsigned kind; int id; } cases[] = {
        {536, 536}, {540, 540}, {541, 541}, {542, 542},
        {0, 536}, {537, 536}, {539, 536}, {543, 536}, {0x3ffff, 536}
    };
    struct DotrPresetKindView view;
    struct DotrPresetObject obj;
    unsigned i;

    memset(&view, 0, sizeof(view));
    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        memset(&obj, 0, sizeof(obj));
        view.unknown_0c = (cases[i].kind << 14) | 0x3fff; /* low bits never matter */
        result_value = 1;
        assert(Ta_Eff_036_LoadEffect(&view, &obj) == 1);
        assert(result_args[1] == cases[i].id && obj.unknown_80.unknown_bits_1_5 == 1);
    }
    result_value = 0;
    view.unknown_0c = 541u << 14;
    assert(Ta_Eff_036_LoadEffect(&view, &obj) == -1 && result_args[1] == 541);
}

int main(void)
{
    check_presets();
    check_object();
    check_submitters();
    check_flagged_submitter();
    check_tagged_submitters();
    check_init_and_ids();
    check_kind_dispatch();
    puts("preset_words: ok");
    return 0;
}
