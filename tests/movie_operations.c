#include "dotr/ee/movie_operations.h"

#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

_Static_assert(CHAR_BIT == 8 && sizeof(int) == 4, "fixture byte/int widths");
_Static_assert(offsetof(struct DotrMovieOperationsStatePrefix, field_00c) == 12, "word offset");
_Static_assert(offsetof(struct DotrMovieOperationsStatePrefix, bytes_1f1) == 0x1f1, "byte access offset");
_Static_assert(offsetof(struct DotrMovieOperationsStatePrefix, bytes_1f9) == 0x1f9, "byte access offset");
_Static_assert(offsetof(struct DotrMovieOperationsStatePrefix, field_21e) == 0x21e, "signed byte offset");
_Static_assert(offsetof(struct DotrMovieOperationsStatePrefix, field_222) == 0x222, "unsigned byte offset");
_Static_assert(offsetof(struct DotrMovieOperationsStatePrefix, field_22c) == 0x22c, "flags offset");
_Static_assert(offsetof(struct DotrMovieOperationsStatePrefix, field_22e) == 0x22e, "flags offset");

struct DotrMovieOperationsStatePrefix dotr_ee_movie_operations_state_00317250;

struct Event {
    const char *name;
    int args[6];
    uint32_t floats[3];
};
static struct Event events[1100];
static size_t event_count;
/* An optional side effect of a stub checks that index reads after the call see the
 * updated state. It is a synthetic callback, not game behaviour.
 */
static int replacement_index = -1;

static void record(const char *name, int a, int b, int c)
{
    struct Event *event;
    assert(event_count < sizeof(events) / sizeof(events[0]));
    event = &events[event_count++];
    memset(event, 0, sizeof(*event));
    event->name = name;
    event->args[0] = a;
    event->args[1] = b;
    event->args[2] = c;
}

static uint32_t float_bits(float value)
{
    uint32_t bits;
    memcpy(&bits, &value, sizeof(bits));
    return bits;
}

#define STUB0(name) void name(void) { record(#name, 0, 0, 0); }
#define STUB1(name) void name(int a) { record(#name, a, 0, 0); }
#define STUB2(name) void name(int a, int b) { record(#name, a, b, 0); }
#define STUB3(name) void name(int a, int b, int c) { record(#name, a, b, c); }
#define STUBF(name) void name(int a, float value) { \
    record(#name, a, 0, 0); events[event_count - 1].floats[0] = float_bits(value); }

void SzFade_Entry(int a, int b, int c, int d, int e, int f)
{
    record("SzFade_Entry", a, b, c);
    events[event_count - 1].args[3] = d;
    events[event_count - 1].args[4] = e;
    events[event_count - 1].args[5] = f;
    if (replacement_index >= 0)
        dotr_ee_movie_operations_state_00317250.field_222 = (unsigned char)replacement_index;
}
STUB2(SzGame_ReqBGM)
STUB1(SzGame_ModeTerm)
STUB1(SzGame_ModeInit)
STUB3(SzDuel_InitMapColor)
STUB0(SzDuel_InitFieldRate)
STUB2(SzView_SetRoll)
STUBF(SzView_SetScreenDistance)
STUB3(SzView_SetCamera)
void SzView_RollCamera(int a, int b, int c, float x, float y, float z)
{
    record("SzView_RollCamera", a, b, c);
    events[event_count - 1].floats[0] = float_bits(x);
    events[event_count - 1].floats[1] = float_bits(y);
    events[event_count - 1].floats[2] = float_bits(z);
}
STUB3(SzView_SetFogParamZ)
STUB3(SzView_SetFogParamY)
STUB3(SzView_SetStraightLight)
STUB3(SzView_SetPointLight)
STUB2(SzModel_SetDeform)
STUB2(SzModel_SetDeformRatio)
STUB2(SzModel_SetDrawFlag)
STUB2(SzModel_SetScale)
STUB2(SzModel_SetRotC)
STUB2(SzModel_SetAnimDirect)
STUB2(SzModel_SetAnimDir)
STUBF(SzModel_SetAnimSpeed)
STUB3(SzModel_SetRgba)
STUB3(SzModel_SetFilter)
STUB2(SzModel_SetIntensity)
STUB1(SzEffect_SetBlackOutForMonster)
STUB1(SzEffect_SetBlackOutForDuel)
STUB1(SzEffect_SetBlackOutForBattle)

short SelectedBattleArena[8];
signed char data_00299e10[16];

int SzGame_SetMenuNum(int index, int value)
{
    record("SzGame_SetMenuNum", index, value, 0);
    return 0;
}

static void expect(size_t *cursor, const char *name, int a, int b, int c)
{
    const struct Event *event;
    assert(*cursor < event_count);
    event = &events[(*cursor)++];
    assert(strcmp(event->name, name) == 0);
    assert(event->args[0] == a && event->args[1] == b && event->args[2] == c);
}

static void expect_setup(size_t *cursor)
{
    expect(cursor, "SzFade_Entry", 0, 1, 1);
    assert(events[*cursor - 1].args[3] == 0);
    assert(events[*cursor - 1].args[4] == 60);
    assert(events[*cursor - 1].args[5] == 0);
}

static void expect_reset(size_t *cursor)
{
    int i;
    expect(cursor, "SzDuel_InitMapColor", 0, 0, 1);
    expect(cursor, "SzDuel_InitFieldRate", 0, 0, 0);
    expect(cursor, "SzView_SetRoll", 0, 0, 0);
    expect(cursor, "SzView_SetScreenDistance", 0, 0, 0);
    assert(events[*cursor - 1].floats[0] == UINT32_C(0x44480000));
    expect(cursor, "SzView_SetCamera", 0, 0, 0);
    expect(cursor, "SzView_RollCamera", 0, 0, 1);
    assert(events[*cursor - 1].floats[0] == UINT32_C(0x4042c75c));
    assert(events[*cursor - 1].floats[1] == UINT32_C(0x3e490fdb));
    assert(events[*cursor - 1].floats[2] == 0);
    for (i = 0; i < 10; ++i) {
        expect(cursor, "SzView_SetFogParamZ", 0, i, 0);
        expect(cursor, "SzView_SetFogParamY", 0, i, 0);
        expect(cursor, "SzView_SetStraightLight", 0, i, 0);
        expect(cursor, "SzView_SetPointLight", 0, i, 0);
    }
    for (i = 20; i < 30; ++i) {
        expect(cursor, "SzModel_SetDeform", i, 0, 0);
        expect(cursor, "SzModel_SetDeformRatio", i, 0, 0);
    }
    for (i = 30; i < 32; ++i) {
        expect(cursor, "SzModel_SetDeform", i, 0, 0);
        expect(cursor, "SzModel_SetDeformRatio", i, 0, 0);
    }
    for (i = 0; i < 84; ++i) {
        expect(cursor, "SzModel_SetDrawFlag", i, 0, 0);
        expect(cursor, "SzModel_SetScale", i, 0, 0);
        expect(cursor, "SzModel_SetRotC", i, 0, 0);
        expect(cursor, "SzModel_SetDeform", i, 0, 0);
        expect(cursor, "SzModel_SetDeformRatio", i, 0, 0);
        expect(cursor, "SzModel_SetAnimDirect", i, 0, 0);
        expect(cursor, "SzModel_SetAnimDir", i, 1, 0);
        expect(cursor, "SzModel_SetAnimSpeed", i, 0, 0);
        assert(events[*cursor - 1].floats[0] == UINT32_C(0x3f800000));
        expect(cursor, "SzModel_SetRgba", i, 0, 0);
        expect(cursor, "SzModel_SetFilter", i, 1, 0);
        expect(cursor, "SzModel_SetIntensity", i, 0, 0);
    }
    expect(cursor, "SzEffect_SetBlackOutForMonster", 128, 0, 0);
    expect(cursor, "SzEffect_SetBlackOutForDuel", 128, 0, 0);
    expect(cursor, "SzEffect_SetBlackOutForBattle", 128, 0, 0);
}

static void seed(int index)
{
    unsigned char *bytes = (unsigned char *)&dotr_ee_movie_operations_state_00317250;
    memset(bytes, 0x5a, sizeof(dotr_ee_movie_operations_state_00317250));
    dotr_ee_movie_operations_state_00317250.field_222 = (unsigned char)index;
    event_count = 0;
    replacement_index = -1;
}

static unsigned test_small_operations(void)
{
    static const int inputs[] = {INT_MIN, -42, -1, 0, 1, 2, 127, 128, 300, INT_MAX};
    static const int indices[] = {0, 1, 6, 7};
    unsigned cases = 0;
    size_t i, j;
    for (i = 0; i < sizeof(indices) / sizeof(indices[0]); ++i) {
        for (j = 0; j < sizeof(inputs) / sizeof(inputs[0]); ++j) {
            unsigned char expected[sizeof(dotr_ee_movie_operations_state_00317250)];
            size_t cursor = 0;
            int index = indices[i], input = inputs[j], arg0 = input ^ 0x13579bdf;
            seed(index);
            memcpy(expected, &dotr_ee_movie_operations_state_00317250, sizeof(expected));
            expected[0x21f] = 0xfe;
            expected[0x220] = (unsigned char)arg0;
            expected[0x221] = 0;
            if (input >= 0)
                expected[0x1f9 + index] = (unsigned char)input;
            SzGame_Goto(arg0, input);
            expect_setup(&cursor);
            if (input < 2)
                expect(&cursor, "SzGame_ReqBGM", -2, 1, 0);
            assert(cursor == event_count);
            assert(memcmp(expected, &dotr_ee_movie_operations_state_00317250, sizeof(expected)) == 0);
            ++cases;

            seed(index);
            cursor = 0;
            memcpy(expected, &dotr_ee_movie_operations_state_00317250, sizeof(expected));
            if (index < 7) {
                expected[0x21f] = 0xfe;
                expected[0x220] = (unsigned char)arg0;
                expected[0x221] = 1;
                expected[0x1fa + index] = (unsigned char)input;
            }
            SzGame_Push(arg0, input);
            if (index < 7) {
                expect_setup(&cursor);
                if (input < 2)
                    expect(&cursor, "SzGame_ReqBGM", -2, 1, 0);
            }
            assert(cursor == event_count);
            assert(memcmp(expected, &dotr_ee_movie_operations_state_00317250, sizeof(expected)) == 0);
            ++cases;

            seed(index);
            cursor = 0;
            ((unsigned char *)&dotr_ee_movie_operations_state_00317250)[0x1f0 + index] = (unsigned char)input;
            ((unsigned char *)&dotr_ee_movie_operations_state_00317250)[0x1f1 + index] = (unsigned char)input;
            memcpy(expected, &dotr_ee_movie_operations_state_00317250, sizeof(expected));
            if (index > 0) {
                expected[0x21f] = 0xfe;
                expected[0x220] = (unsigned char)input;
                expected[0x221] = 0xff;
            }
            SzGame_Pop();
            if (index > 0) {
                expect_setup(&cursor);
                if ((signed char)input < 2)
                    expect(&cursor, "SzGame_ReqBGM", -2, 1, 0);
            }
            assert(cursor == event_count);
            assert(memcmp(expected, &dotr_ee_movie_operations_state_00317250, sizeof(expected)) == 0);
            ++cases;
        }
    }
    /* Unsigned gate and a fresh index after the setup callback. */
    seed(255);
    SzGame_Push(0, 0);
    assert(event_count == 0 && dotr_ee_movie_operations_state_00317250.field_222 == 255);
    ++cases;
    seed(1);
    replacement_index = 6;
    SzGame_Push(42, 3);
    assert(((unsigned char *)&dotr_ee_movie_operations_state_00317250)[0x200] == 3);
    assert(((unsigned char *)&dotr_ee_movie_operations_state_00317250)[0x1fb] == 0x5a);
    ++cases;
    seed(1);
    replacement_index = 7;
    ((signed char *)&dotr_ee_movie_operations_state_00317250)[0x1f7] = -42;
    ((signed char *)&dotr_ee_movie_operations_state_00317250)[0x1f8] = 3;
    SzGame_Pop();
    assert(dotr_ee_movie_operations_state_00317250.field_220 == -42 && event_count == 1);
    ++cases;
    return cases;
}

static unsigned test_transition(void)
{
    static const int inputs[] = {0, 15, 36, 10, -1, -2, -3, 127, 128};
    static const int old_values[] = {-3, 1, 5};
    unsigned cases = 0;
    int step, prior_flag, new_flag;
    size_t i, j;
    for (step = -1; step <= 1; ++step)
        for (prior_flag = 0; prior_flag <= 2; prior_flag += 2)
            for (new_flag = 0; new_flag <= 2; new_flag += 2)
                for (i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
                    for (j = 0; j < sizeof(old_values) / sizeof(old_values[0]); ++j) {
                        unsigned char expected[sizeof(dotr_ee_movie_operations_state_00317250)];
                        struct DotrMovieOperationsStatePrefix *state = &dotr_ee_movie_operations_state_00317250;
                        size_t cursor = 0;
                        int index = 3, next = index + step, input = inputs[i];
                        int old = old_values[j], previous_flag, current_flag, flag = 1;
                        seed(index);
                        state->bytes_1f1[index] = (signed char)old;
                        state->bytes_1f9[index] = (unsigned char)prior_flag;
                        state->bytes_1f9[next] = (unsigned char)new_flag;
                        previous_flag = state->bytes_1f9[index];
                        current_flag = state->bytes_1f9[next];
                        memcpy(expected, state, sizeof(expected));
                        expected[0x21e] = (unsigned char)old;
                        expected[0x222] = (unsigned char)next;
                        expected[0x1f1 + next] = (unsigned char)input;
                        expected[0x21f] = (unsigned char)input;
                        if (current_flag != 0 && step > 0)
                            flag = 0;
                        if (previous_flag != 0 && step < 0)
                            flag = 0;
                        if ((input == 15 && old == 1) || input == 36 || input == 10 ||
                            input == -1 || input == -2 || input == -3)
                            flag = 0;
                        expected[0x221] = 0;
                        expected[0x22c] = (unsigned char)((expected[0x22c] & 0xc3) | 0x34);
                        expected[0x22e] = (unsigned char)((expected[0x22e] & 0xef) | (flag << 4));
                        memset(expected + 12, 0, 4);
                        expected[0x229] = 0;
                        expected[0x228] = 0;
                        SzGame_SetMode(input, step);
                        if (current_flag == 0 || step <= 0)
                            expect(&cursor, "SzGame_ModeTerm", old, 0, 0);
                        expect_reset(&cursor);
                        if (previous_flag == 0 || step >= 0)
                            expect(&cursor, "SzGame_ModeInit", (signed char)input, 0, 0);
                        assert(cursor == event_count);
                        assert(memcmp(expected, state, sizeof(expected)) == 0);
                        ++cases;
                    }
    return cases;
}

/* 0x2336f0 branch coverage: flag byte zero versus nonzero, signed short value. */
static void test_menu_defaults(void)
{
    size_t cursor = 0;
    seed(0);
    event_count = 0;
    SelectedBattleArena[0] = -5;
    data_00299e10[0] = 0;
    Taku_Camp_Set_Go_Duel();
    expect(&cursor, "SzGame_SetMenuNum", 1, -5, 0);
    expect(&cursor, "SzGame_SetMenuNum", 2, 0, 0);
    expect(&cursor, "SzGame_SetMenuNum", 3, 1100, 0);
    expect(&cursor, "SzGame_SetMenuNum", 4, 995, 0);
    expect(&cursor, "SzFade_Entry", 0, 1, 1);
    expect(&cursor, "SzGame_ReqBGM", -2, 1, 0);
    assert(cursor == event_count);
    event_count = 0;
    cursor = 0;
    SelectedBattleArena[0] = 7;
    data_00299e10[0] = -1;
    Taku_Camp_Set_Go_Duel();
    expect(&cursor, "SzGame_SetMenuNum", 1, 7, 0);
    expect(&cursor, "SzGame_SetMenuNum", 2, -1, 0);
    expect(&cursor, "SzGame_SetMenuNum", 3, 1007, 0);
    expect(&cursor, "SzGame_SetMenuNum", 4, 1100, 0);
    event_count = 0;
}

static int probe_result;
int SzGame_ActMemCardProc(void)
{
    record("SzGame_ActMemCardProc", 0, 0, 0);
    return probe_result;
}

/* 0x1ac560 branch coverage: probe result negative, zero and positive; the
 * index byte selects which byte of the +0x1f9 span is cleared. */
static void test_probe_reset(void)
{
    static const int results[] = {INT_MIN, -1, 0, 1};
    static const int indices[] = {0, 7};
    size_t i, j;

    for (i = 0; i < sizeof(results) / sizeof(results[0]); ++i)
        for (j = 0; j < sizeof(indices) / sizeof(indices[0]); ++j) {
            struct DotrMovieOperationsStatePrefix *state = &dotr_ee_movie_operations_state_00317250;
            unsigned char expected[sizeof(*state)];
            size_t cursor = 0;
            seed(indices[j]);
            probe_result = results[i];
            memcpy(expected, state, sizeof(expected));
            lSzGame_ActMemCheck();
            expect(&cursor, "SzGame_ActMemCardProc", 0, 0, 0);
            if (results[i] < 0) {
                struct DotrMovieOperationsStatePrefix *want = (struct DotrMovieOperationsStatePrefix *)expected;
                want->field_160 = 0;
                want->field_21f = -2;
                want->field_220 = 10;
                want->field_221 = 0;
                want->bytes_1f9[indices[j]] = 0;
                expect(&cursor, "SzFade_Entry", 0, 1, 1);
                assert(events[cursor - 1].args[3] == 0 && events[cursor - 1].args[4] == 60);
                assert(events[cursor - 1].args[5] == 0);
                expect(&cursor, "SzGame_ReqBGM", -2, 1, 0);
            }
            assert(cursor == event_count);
            assert(memcmp(expected, state, sizeof(expected)) == 0);
        }
    event_count = 0;
}

/* SzGame_ModeAct (0x1aa260). Scripted results for its callees; every call is recorded. */
static int act_mode, act_file_busy, act_loading, act_standby, act_entry[2], act_fading, act_debug_effect, act_duel_more;
static int act_pressed[8][3];
static int act_pressed_count;

int SzGame_GetMode(int a) { record("SzGame_GetMode", a, 0, 0); return act_mode; }
int SzFile_InqRequestEx(void) { record("SzFile_InqRequestEx", 0, 0, 0); return act_file_busy; }
int SzSound_IsLoadingBGM(void) { record("SzSound_IsLoadingBGM", 0, 0, 0); return act_loading; }
int SzText_LoadMerge(int a, int b) { record("SzText_LoadMerge", a, b, 0); return 0; }
int SzFade_SetPauseFlag(int index, int flag) { record("SzFade_SetPauseFlag", index, flag, 0); return 0; }
int SzSound_IsStandby(void) { record("SzSound_IsStandby", 0, 0, 0); return act_standby; }
int SzBattle_InqEntry(int index) { record("SzBattle_InqEntry", index, 0, 0); return act_entry[index]; }
int SzDuel_GetUnitModelID(int a, int b) { record("SzDuel_GetUnitModelID", a, b, 0); return a * 100 + b; }
int SzModel_ReqSE(int channel, int index) { record("SzModel_ReqSE", channel, index, 0); return 0; }
unsigned char SzModel_Motion(int index) { record("SzModel_Motion", index, 0, 0); return 0; }
int SzFade_IsDoing(int flag) { record("SzFade_IsDoing", flag, 0, 0); return act_fading; }
int SzDebug_IsEffect(void) { record("SzDebug_IsEffect", 0, 0, 0); return act_debug_effect; }
int SzDuel_Act(void) { record("SzDuel_Act", 0, 0, 0); return act_duel_more-- > 0; }
int SzPad_Check(int unknown, int zero, int mask)
{
    int i;

    record("SzPad_Check", unknown, zero, mask);
    for (i = 0; i < act_pressed_count; ++i)
        if (act_pressed[i][0] == unknown && act_pressed[i][1] == zero && act_pressed[i][2] == mask)
            return 1;
    return 0;
}
STUB0(SzEffect_ReqSE)
STUB0(SzEffect_TrimModule)
STUB0(SzGame_ActMenu)
STUB0(SzDebug_Act)
STUB1(SzModel_Act)
STUB1(SzView_Act)
STUB0(SzDuel_ActBuildDeck)
STUB0(SzDuel_Act3DView)
STUB0(SzDuel_ActTrade)
STUB0(SzDuel_ActCustomDuel)
STUB0(TkCamp_Act)
STUB0(ShFace_Act)
STUB0(SzDuel_ActMapEdit)
STUB0(SzDuel_ActCardTest)
STUB0(SzView_ActCamera)
STUB0(ShSlot_Act)

static void act_reset(int mode, int status)
{
    struct DotrMovieOperationsStatePrefix *state = &dotr_ee_movie_operations_state_00317250;

    memset(state, 0, sizeof(*state));
    state->field_22c.unknown_0 = 1;
    state->field_22c.field_2 = -2;
    state->field_22c.field_4 = (unsigned char)status;
    state->field_22c.unknown_6 = 1;
    state->field_00c = 99;
    event_count = 0;
    act_mode = mode;
    act_file_busy = act_loading = act_standby = act_fading = act_debug_effect = act_duel_more = 0;
    act_entry[0] = act_entry[1] = -1;
    act_pressed_count = 0;
    probe_result = 0;
}
static void act_press(int unknown, int zero, int mask)
{
    act_pressed[act_pressed_count][0] = unknown;
    act_pressed[act_pressed_count][1] = zero;
    act_pressed[act_pressed_count][2] = mask;
    act_pressed_count++;
}
/* The other three fields of byte 0x22c must survive every status write. */
static void act_expect_status(int status)
{
    struct DotrMovieOperationsStatePrefix *state = &dotr_ee_movie_operations_state_00317250;

    assert(state->field_22c.field_4 == status);
    assert(state->field_22c.unknown_0 == 1 && state->field_22c.field_2 == -2 && state->field_22c.unknown_6 == 1);
}
/* The part that runs every frame once start-up is over, with no sound standby. */
static void act_expect_frame(size_t *cursor)
{
    int i;

    expect(cursor, "SzSound_IsStandby", 0, 0, 0);
    for (i = 0; i < 84; ++i)
        expect(cursor, "SzModel_Motion", i, 0, 0);
    expect(cursor, "SzEffect_TrimModule", 0, 0, 0);
    expect(cursor, "SzFade_IsDoing", 1, 0, 0);
}
static size_t act_run_mode(int mode)
{
    size_t cursor = 0;

    act_reset(mode, 0);
    SzGame_ModeAct();
    expect(&cursor, "SzGame_GetMode", 0, 0, 0);
    act_expect_frame(&cursor);
    return cursor;
}
static void act_expect_debug_view(size_t *cursor, int effect)
{
    int i;

    expect(cursor, "SzDebug_Act", 0, 0, 0);
    expect(cursor, "SzDebug_IsEffect", 0, 0, 0);
    if (!effect)
        for (i = 0; i < 84; ++i)
            expect(cursor, "SzModel_Act", i, 0, 0);
    expect(cursor, "SzView_Act", 0, 0, 0);
}
/* Mode 17 with the given buttons already pressed; returns the index of its first pad query. */
static size_t act_run_pad(void)
{
    size_t cursor = 0;

    event_count = 0;
    SzGame_ModeAct();
    expect(&cursor, "SzGame_GetMode", 0, 0, 0);
    act_expect_frame(&cursor);
    return cursor;
}
static void act_expect_pads(size_t *cursor, const int (*want)[3], size_t count)
{
    size_t i;

    for (i = 0; i < count; ++i)
        expect(cursor, "SzPad_Check", want[i][0], want[i][1], want[i][2]);
}

/* 0x1aa260 branch coverage: each status with each of its exits, both standby outcomes with
 * the entry sign boundary, the fade exit, every mode case plus unlisted modes, and for mode
 * 17 each button alone with the 0 / 255 bounds and the inlined pop's index and music bounds. */
static void test_mode_act(void)
{
    struct DotrMovieOperationsStatePrefix *state = &dotr_ee_movie_operations_state_00317250;
    static const int single[][2] = {{-2, 0}, {-1, 0}, {6, 1}, {7, 2}, {8, 3}, {9, 4}, {10, 5}, {13, 6},
                                    {18, 7}, {14, 8}, {15, 9}};
    static const char *const single_name[] = {"SzGame_ActMenu", "SzGame_ActMenu", "SzDuel_ActBuildDeck", "SzDuel_Act3DView",
                                              "SzDuel_ActTrade", "SzDuel_ActCustomDuel", "SzGame_ActMemCardProc", /* first call of the real lSzGame_ActTitle */
                                              "SzDuel_ActMapEdit", "SzDuel_ActCardTest", "ShFace_Act", "ShSlot_Act"};
    static const int none[] = {-3, -4, 2, 3, 4, 16, 20, 35, 37};
    static const int debug_modes[] = {0, 5, 12};
    static const int idle[][3] = {{0, 0, 16}, {0, 1, 32}, {0, 1, 128}, {0, 1, 0xf000}, {1, 1, 0x1000}, {1, 2, 129},
                                  {1, 2, 34}, {1, 2, 65}, {1, 2, 18}, {1, 2, 5}, {1, 2, 10}, {0, 0, 32}};
    static const int moved[][3] = {{0, 0, 16}, {0, 1, 32}, {0, 1, 128}, {0, 1, 0xf000}, {0, 1, 0x8000}, {0, 1, 0x2000},
                                   {0, 1, 0x1000}, {0, 1, 0x4000}, {1, 1, 0x1000}, {1, 2, 128}, {1, 2, 32}, {0, 0, 32}};
    static const int dirs[][3] = {{0x8000, 0, -1}, {0x2000, 0, 1}, {0x1000, 1, -1}, {0x4000, 1, 1}};
    static const int fine[][3] = {{129, 2, -1}, {34, 2, 1}, {65, 1, -1}, {18, 1, 1}, {5, 0, -1}, {10, 0, 1}};
    size_t cursor, i;
    int j;

    /* Status 3: waiting for the file request. */
    act_reset(7, 3);
    act_file_busy = 1;
    SzGame_ModeAct();
    cursor = 0;
    expect(&cursor, "SzGame_GetMode", 0, 0, 0);
    expect(&cursor, "SzFile_InqRequestEx", 0, 0, 0);
    assert(cursor == event_count);
    act_expect_status(3);
    /* Status 3: no music word, bit 4 of 0x22e clear. */
    act_reset(7, 3);
    state->field_228 = 5;
    SzGame_ModeAct();
    assert(event_count == 2 && state->field_228 == 5);
    act_expect_status(2);
    /* Status 3: music word and bit 4 set. */
    act_reset(7, 3);
    state->field_190 = 1;
    state->field_22e.field_4 = -1;
    SzGame_ModeAct();
    cursor = 2;
    expect(&cursor, "SzGame_ReqBGM", 7, 1, 0);
    assert(cursor == event_count && (unsigned char)state->field_228 == 128);
    act_expect_status(2);

    /* Status 2: bit 5 set and the music still loading. */
    act_reset(1, 2);
    state->field_22e.field_5 = -1;
    act_loading = 1;
    SzGame_ModeAct();
    cursor = 1;
    expect(&cursor, "SzSound_IsLoadingBGM", 0, 0, 0);
    assert(cursor == event_count);
    act_expect_status(2);
    /* Status 2: bit 5 set, loaded, mode 1 loads four text sets. */
    act_reset(1, 2);
    state->field_22e.field_5 = -1;
    SzGame_ModeAct();
    cursor = 1;
    expect(&cursor, "SzSound_IsLoadingBGM", 0, 0, 0);
    expect(&cursor, "SzText_LoadMerge", 4, 1, 0);
    expect(&cursor, "SzText_LoadMerge", 1, 0, 0);
    expect(&cursor, "SzText_LoadMerge", 2, 0, 0);
    expect(&cursor, "SzText_LoadMerge", 3, 0, 0);
    assert(cursor == event_count);
    act_expect_status(1);
    /* Status 2: bit 5 clear (no loading query); modes 6 and 8 load set 0, mode 7 nothing. */
    for (j = 6; j <= 8; ++j) {
        act_reset(j, 2);
        act_loading = 1;
        SzGame_ModeAct();
        cursor = 1;
        if (j != 7)
            expect(&cursor, "SzText_LoadMerge", 0, 0, 0);
        assert(cursor == event_count);
        act_expect_status(1);
    }

    /* Status 1: byte 0x228 not yet zero. */
    act_reset(7, 1);
    state->field_228 = -128;
    SzGame_ModeAct();
    assert(event_count == 1 && state->field_00c == 99);
    /* Status 1: byte 0x228 zero: pause released, word 0xc cleared, the frame goes on. */
    act_reset(7, 1);
    SzGame_ModeAct();
    cursor = 1;
    expect(&cursor, "SzFade_SetPauseFlag", 1, 0, 0);
    act_expect_frame(&cursor);
    expect(&cursor, "SzDuel_Act3DView", 0, 0, 0);
    assert(cursor == event_count && state->field_00c == 0);
    act_expect_status(1);

    /* Sound standby: entry -1 skipped, entry 0 (boundary) and 0x0305 requested. */
    act_reset(-3, 0);
    act_standby = 1;
    act_entry[0] = 0;
    act_entry[1] = 0x0305;
    SzGame_ModeAct();
    cursor = 1;
    expect(&cursor, "SzSound_IsStandby", 0, 0, 0);
    expect(&cursor, "SzBattle_InqEntry", 0, 0, 0);
    expect(&cursor, "SzDuel_GetUnitModelID", 0, 0, 0);
    expect(&cursor, "SzModel_ReqSE", 2, 0, 0);
    expect(&cursor, "SzBattle_InqEntry", 1, 0, 0);
    expect(&cursor, "SzDuel_GetUnitModelID", 3, 5, 0);
    expect(&cursor, "SzModel_ReqSE", 3, 305, 0);
    expect(&cursor, "SzEffect_ReqSE", 0, 0, 0);
    expect(&cursor, "SzModel_Motion", 0, 0, 0);
    assert(event_count == cursor + 83 + 2);
    act_reset(-3, 0);
    act_standby = 1;
    SzGame_ModeAct();
    cursor = 2;
    expect(&cursor, "SzBattle_InqEntry", 0, 0, 0);
    expect(&cursor, "SzBattle_InqEntry", 1, 0, 0);
    expect(&cursor, "SzEffect_ReqSE", 0, 0, 0);
    /* Fade 1 running: the mode function is not reached. */
    act_reset(7, 0);
    act_fading = 1;
    SzGame_ModeAct();
    cursor = 1;
    act_expect_frame(&cursor);
    assert(cursor == event_count);

    /* Modes with one call, with none, and the three debug-view modes. */
    for (i = 0; i < sizeof(single) / sizeof(single[0]); ++i) {
        cursor = act_run_mode(single[i][0]);
        expect(&cursor, single_name[i], 0, 0, 0);
        assert(cursor == event_count);
    }
    for (i = 0; i < sizeof(none) / sizeof(none[0]); ++i)
        assert(act_run_mode(none[i]) == event_count);
    for (i = 0; i < sizeof(debug_modes) / sizeof(debug_modes[0]); ++i)
        for (j = 0; j < 2; ++j) {
            cursor = 0;
            act_reset(debug_modes[i], 0);
            act_debug_effect = j;
            SzGame_ModeAct();
            cursor = 1;
            act_expect_frame(&cursor);
            act_expect_debug_view(&cursor, j);
            assert(cursor == event_count);
        }
    /* Mode 1 repeats SzDuel_Act while it returns non-zero. */
    for (j = 0; j < 3; j += 2) {
        act_reset(1, 0);
        act_duel_more = j;
        SzGame_ModeAct();
        cursor = 1;
        act_expect_frame(&cursor);
        for (i = 0; i < (size_t)j + 1; ++i)
            expect(&cursor, "SzDuel_Act", 0, 0, 0);
        assert(cursor == event_count);
    }
    cursor = act_run_mode(36);
    expect(&cursor, "SzGame_ActMemCardProc", 0, 0, 0);
    assert(cursor == event_count);
    cursor = act_run_mode(11);
    expect(&cursor, "TkCamp_Act", 0, 0, 0);
    expect(&cursor, "ShFace_Act", 0, 0, 0);
    assert(cursor == event_count);
    cursor = act_run_mode(19);
    expect(&cursor, "SzView_ActCamera", 0, 0, 0);
    expect(&cursor, "SzView_Act", 0, 0, 0);
    assert(cursor == event_count);

    /* Mode 17, nothing pressed: twelve queries, nothing written. */
    {
        unsigned char before[sizeof(*state)];

        act_reset(17, 0);
        memcpy(before, state, sizeof(before));
        cursor = act_run_pad();
        act_expect_pads(&cursor, idle, 12);
        assert(cursor == event_count && memcmp(before, state, sizeof(before)) == 0);
    }
    /* The first button toggles bit 0 of byte 0x22f and leaves its other bits. */
    act_reset(17, 0);
    act_press(0, 0, 16);
    *(unsigned char *)&state->field_22f = 0xa4;
    act_run_pad();
    assert(*(unsigned char *)&state->field_22f == 0xa5);
    act_run_pad();
    assert(*(unsigned char *)&state->field_22f == 0xa4);
    /* Direction group: each direction alone, at step 1 and at step 2 from either button. */
    for (i = 0; i < 4; ++i)
        for (j = 0; j < 3; ++j) {
            act_reset(17, 0);
            act_press(0, 1, 0xf000);
            act_press(0, 1, dirs[i][0]);
            if (j)
                act_press(0, 1, j == 1 ? 32 : 128);
            state->field_214 = 100;
            state->field_218 = 200;
            act_run_pad();
            assert(state->field_214 == 100 + (dirs[i][1] == 0 ? dirs[i][2] * (j ? 2 : 1) : 0));
            assert(state->field_218 == 200 + (dirs[i][1] == 1 ? dirs[i][2] * (j ? 2 : 1) : 0));
        }
    /* A direction without the group mask is not even queried. */
    act_reset(17, 0);
    act_press(0, 1, 0x8000);
    state->field_214 = 100;
    cursor = act_run_pad();
    act_expect_pads(&cursor, idle, 12);
    assert(state->field_214 == 100);
    /* Group and second-pad mask pressed: the query list of that path; byte 3 down and up. */
    act_reset(17, 0);
    act_press(0, 1, 0xf000);
    act_press(1, 1, 0x1000);
    cursor = act_run_pad();
    act_expect_pads(&cursor, moved, 12);
    assert(cursor == event_count);
    for (j = 0; j < 4; ++j) {
        static const int start[] = {0, 1, 254, 255};

        act_reset(17, 0);
        act_press(1, 1, 0x1000);
        act_press(1, 2, 128);
        state->bytes_20c[3] = (unsigned char)start[j];
        act_run_pad();
        assert(state->bytes_20c[3] == (start[j] > 0 ? start[j] - 1 : 0));
        act_reset(17, 0);
        act_press(1, 1, 0x1000);
        act_press(1, 2, 32);
        state->bytes_20c[3] = (unsigned char)start[j];
        act_run_pad();
        assert(state->bytes_20c[3] == (start[j] < 255 ? start[j] + 1 : 255));
        /* Without the second-pad mask the six fine buttons step bytes 2, 1 and 0. */
        for (i = 0; i < 6; ++i) {
            int want = start[j] + fine[i][2];

            act_reset(17, 0);
            act_press(1, 2, fine[i][0]);
            state->bytes_20c[fine[i][1]] = (unsigned char)start[j];
            act_run_pad();
            assert(state->bytes_20c[fine[i][1]] == (want < 0 ? 0 : want > 255 ? 255 : want));
            assert(state->bytes_20c[3] == 0);
        }
    }
    /* Leaving: index 0 does nothing; index 2 pops, with the music bound at 1 / 2. */
    act_reset(17, 0);
    act_press(0, 0, 32);
    cursor = act_run_pad();
    act_expect_pads(&cursor, idle, 12);
    assert(cursor == event_count && state->field_21f == 0);
    for (j = 1; j <= 2; ++j) {
        act_reset(17, 0);
        act_press(0, 0, 32);
        state->field_222 = 2;
        state->bytes_1f1[1] = 44;
        state->bytes_1f1[2] = (signed char)j;
        cursor = act_run_pad();
        act_expect_pads(&cursor, idle, 12);
        expect(&cursor, "SzFade_Entry", 0, 1, 1);
        assert(events[cursor - 1].args[3] == 0 && events[cursor - 1].args[4] == 60 && events[cursor - 1].args[5] == 0);
        if (j < 2)
            expect(&cursor, "SzGame_ReqBGM", -2, 1, 0);
        assert(cursor == event_count);
        assert(state->field_21f == -2 && state->field_220 == 44 && state->field_221 == -1);
    }
    event_count = 0;
    act_pressed_count = 0;
}

/* lSzGame_ActTitle (0x1ac700). */
char data_002be80f[] = "label";
static char *title_label;
static int title_playing, title_option;
char *func_00197548(const char *label)
{
    assert(label == data_002be80f);
    record("func_00197548", 0, 0, 0);
    return title_label;
}
int SzData_GetOption(int side, int which) { record("SzData_GetOption", side, which, 0); return title_option; }
int SzData_SetOption(int side, int which, int flag) { record("SzData_SetOption", side, which, flag); return 0; }
int SzSound_SetOutput(int flag) { record("SzSound_SetOutput", flag, 0, 0); return 0; }
int SzSound_PlaySE(int index, int value) { record("SzSound_PlaySE", index, value, 0); return 0; }
int SzSound_IsPlaySE(void) { record("SzSound_IsPlaySE", 0, 0, 0); return title_playing; }
void SzGame_InitMemCardProc(int kind) { record("SzGame_InitMemCardProc", kind, 0, 0); }

static struct DotrMovieOperationsStatePrefix *title_reset(int probe)
{
    struct DotrMovieOperationsStatePrefix *state = &dotr_ee_movie_operations_state_00317250;

    act_reset(10, 0);
    memset(state, 0, sizeof(*state));
    replacement_index = -1;
    probe_result = probe;
    title_label = 0;
    title_playing = 0;
    title_option = 0;
    state->field_222 = 2;
    state->bytes_1f9[2] = 55;
    state->bytes_1f9[3] = 55;
    return state;
}

static void title_expect_push(size_t *cursor, int mode)
{
    struct DotrMovieOperationsStatePrefix *state = &dotr_ee_movie_operations_state_00317250;

    expect_setup(cursor);
    expect(cursor, "SzGame_ReqBGM", -2, 1, 0);
    assert(state->field_21f == -2 && state->field_220 == mode && state->field_221 == 1);
    assert(state->bytes_1f9[3] == 0 && state->bytes_1f9[2] == 55);
}

static void test_act_title(void)
{
    struct DotrMovieOperationsStatePrefix *state;
    static const int push_mode[4] = {0, 11, 9, 8};
    static const int kinds[4] = {0, 0, 3, 4};
    static const int clamp[][2] = {{0, 4}, {120, 124}, {123, 127}, {124, 128}, {127, 128}};
    static const int swing[][3] = {{124, 128, -1}, {125, 129, 1}, {235, 239, 1}, {236, 240, -1}};
    size_t cursor;
    int i, full;

    /* Memory card procedure result 0 or 2: nothing else happens. */
    for (i = 0; i <= 2; i += 2) {
        state = title_reset(i);
        state->field_22f.field_1 = -1;
        state->field_224 = 128;
        lSzGame_ActTitle();
        assert(event_count == 1 && state->field_227 == 0);
    }
    /* Result 1: the selected entry decides the pushed mode; the sound output is set in every case. */
    for (i = 0; i < 4; ++i) {
        for (full = 0; full < 2; ++full) {
            state = title_reset(1);
            state->field_225 = (unsigned char)i;
            state->field_21c = 0x05ff;
            state->field_15c = 77;
            state->field_164 = 77;
            title_option = 6;
            if (full) {
                state->field_222 = 7;
            }
            lSzGame_ActTitle();
            cursor = 0;
            expect(&cursor, "SzGame_ActMemCardProc", 0, 0, 0);
            if (i != 0 && !full)
                title_expect_push(&cursor, push_mode[i]);
            else
                assert(state->field_220 == 0 && state->field_21f == 0);
            expect(&cursor, "SzData_GetOption", 0, 2, 0);
            expect(&cursor, "SzSound_SetOutput", 6, 0, 0);
            assert(cursor == event_count);
            assert(state->field_164 == (i == 1 ? 1 : 77));
            assert(state->field_15c == (i == 2 ? 4 : 77));
        }
    }
    /* Other results, bit 1 of 0x22f set, 0x224 below 128: it steps by 4 up to 128 unless the
     * sound is standing by with no effect playing. */
    for (i = 0; i < 5; ++i) {
        state = title_reset(3);
        state->field_22f.field_1 = -1;
        state->field_224 = (unsigned char)clamp[i][0];
        lSzGame_ActTitle();
        cursor = 0;
        expect(&cursor, "SzGame_ActMemCardProc", 0, 0, 0);
        expect(&cursor, "SzSound_IsStandby", 0, 0, 0);
        assert(cursor == event_count && state->field_224 == clamp[i][1]);
    }
    state = title_reset(3);
    state->field_22f.field_1 = -1;
    state->field_224 = 8;
    act_standby = 1;
    lSzGame_ActTitle();
    assert(event_count == 3 && strcmp(events[2].name, "SzSound_IsPlaySE") == 0 && state->field_224 == 8);
    title_playing = 1;
    lSzGame_ActTitle();
    assert(state->field_224 == 12);
    /* 0x224 at 128: 0x227 swings by four times the two-bit step, which is negated at or below
     * 128 and at or above 240. */
    for (i = 0; i < 4; ++i) {
        state = title_reset(3);
        state->field_22f.field_0 = -1;
        state->field_22f.field_1 = -1;
        state->field_224 = 128;
        state->field_227 = (unsigned char)swing[i][0];
        state->field_22c.unknown_0 = 1;
        state->field_22c.field_4 = 2;
        lSzGame_ActTitle();
        cursor = 0;
        expect(&cursor, "SzGame_ActMemCardProc", 0, 0, 0);
        expect(&cursor, "func_00197548", 0, 0, 0);
        expect(&cursor, "SzPad_Check", 0, 0, 2048);
        assert(cursor == event_count);
        assert(state->field_227 == swing[i][1] && state->field_22c.unknown_0 == swing[i][2]);
        assert(state->field_22c.field_4 == 2 && state->field_22f.field_1 == -1 && state->field_008 == 0);
    }
    /* The pad bit clears bit 1 only. */
    state = title_reset(3);
    state->field_22f.field_0 = -1;
    state->field_22f.field_1 = -1;
    state->field_224 = 200;
    state->field_227 = 200;
    act_press(0, 0, 2048);
    lSzGame_ActTitle();
    cursor = 3;
    expect(&cursor, "SzSound_PlaySE", 0, 4, 0);
    assert(cursor == event_count && state->field_22f.field_1 == 0 && state->field_22f.field_0 == -1);
    /* With the label present the counter runs; at 900 the state goes to mode 10 and returns. */
    state = title_reset(3);
    state->field_22f.field_1 = -1;
    state->field_224 = 128;
    state->field_227 = 200;
    state->field_008 = 898;
    state->field_160 = 9;
    title_label = data_002be80f;
    lSzGame_ActTitle();
    assert(event_count == 3 && state->field_008 == 899 && state->field_160 == 9 && state->field_220 == 0);
    event_count = 0;
    lSzGame_ActTitle();
    cursor = 0;
    expect(&cursor, "SzGame_ActMemCardProc", 0, 0, 0);
    expect(&cursor, "func_00197548", 0, 0, 0);
    expect_setup(&cursor);
    expect(&cursor, "SzGame_ReqBGM", -2, 1, 0);
    assert(cursor == event_count && state->field_008 == 900 && state->field_160 == 0);
    assert(state->field_21f == -2 && state->field_220 == 10 && state->field_221 == 0);
    assert(state->bytes_1f9[2] == 0 && state->bytes_1f9[3] == 55);
    /* Bit 1 clear, 0x226 below 128: it steps by 4 up to 128. */
    for (i = 0; i < 5; ++i) {
        state = title_reset(3);
        state->field_226 = (unsigned char)clamp[i][0];
        lSzGame_ActTitle();
        assert(event_count == 1 && state->field_226 == clamp[i][1]);
    }
    /* 0x226 at 128, direction bits: the selection moves back with 0x1000, forward otherwise. */
    for (i = 0; i < 4; ++i) {
        for (full = 0; full < 2; ++full) {
            state = title_reset(3);
            state->field_226 = 128;
            state->field_225 = (unsigned char)i;
            act_press(0, 2, 20480);
            if (full)
                act_press(0, 2, 4096);
            lSzGame_ActTitle();
            cursor = 0;
            expect(&cursor, "SzGame_ActMemCardProc", 0, 0, 0);
            expect(&cursor, "SzPad_Check", 0, 2, 20480);
            expect(&cursor, "SzPad_Check", 0, 2, 4096);
            expect(&cursor, "SzSound_PlaySE", 0, 3, 0);
            assert(cursor == event_count && state->field_225 == ((i + (full ? 3 : 1)) & 3));
        }
    }
    /* No pad bit: two checks only. */
    state = title_reset(3);
    state->field_226 = 128;
    lSzGame_ActTitle();
    cursor = 0;
    expect(&cursor, "SzGame_ActMemCardProc", 0, 0, 0);
    expect(&cursor, "SzPad_Check", 0, 2, 20480);
    expect(&cursor, "SzPad_Check", 0, 0, 192);
    assert(cursor == event_count);
    /* Confirm bits: entry 0 pushes mode 11 and sets three options, entries 1..3 start the
     * memory card procedure with 0, 3 and 4; another value only plays the cue. */
    for (i = 0; i < 5; ++i) {
        for (full = 0; full < 2; ++full) {
            state = title_reset(3);
            state->field_226 = 128;
            state->field_225 = (unsigned char)i;
            state->field_164 = 77;
            if (full)
                state->field_222 = 7;
            act_press(0, 0, 192);
            lSzGame_ActTitle();
            cursor = 0;
            expect(&cursor, "SzGame_ActMemCardProc", 0, 0, 0);
            expect(&cursor, "SzPad_Check", 0, 2, 20480);
            expect(&cursor, "SzPad_Check", 0, 0, 192);
            expect(&cursor, "SzSound_PlaySE", 0, 4, 0);
            if (i == 0) {
                if (!full)
                    title_expect_push(&cursor, 11);
                expect(&cursor, "SzData_SetOption", 0, 0, 1);
                expect(&cursor, "SzData_SetOption", 0, 1, 1);
                expect(&cursor, "SzData_SetOption", 0, 2, 1);
                assert(state->field_164 == 0);
            } else {
                if (i < 4)
                    expect(&cursor, "SzGame_InitMemCardProc", kinds[i], 0, 0);
                assert(state->field_164 == 77);
            }
            assert(cursor == event_count && state->field_225 == i);
        }
    }
    event_count = 0;
}

int main(void)
{
    test_act_title();
    test_menu_defaults();
    test_probe_reset();
    test_mode_act();
    unsigned small = test_small_operations();
    unsigned transitions;
    unsigned char before[sizeof(dotr_ee_movie_operations_state_00317250)];
    size_t cursor = 0;
    seed(3);
    memcpy(before, &dotr_ee_movie_operations_state_00317250, sizeof(before));
    SzGame_ComInit();
    expect_reset(&cursor);
    assert(cursor == event_count && event_count == 997);
    assert(memcmp(before, &dotr_ee_movie_operations_state_00317250, sizeof(before)) == 0);
    transitions = test_transition();
    printf("movie operations: %u byte-state cases, %u transitions, 997-call reset passed\n", small, transitions);
    return 0;
}
