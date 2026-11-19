/* Synthetic tests of the packed-position wrappers; every routine outside the
 * group is a stub that records its arguments. */
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/packed_item.h"

struct DotrPackedTable gSzEffect_el;
int data_0031be98[1], data_0031be9c[1];

static int calls, last_target, last_row, last_column, last_flag;
static void *last_pointer;
static struct DotrStateItem item, *lookup_result;
static int lookup_row, lookup_column, probe_argument;

static int word_result, word_after_probe, comparison_result, comparison_flag;
static int comparison_order[2], comparison_calls;

int SzBattle_InqEntry(int index)
{
    assert(index == 0 && comparison_calls == 0);
    comparison_order[comparison_calls++] = 1;
    data_0031be9c[0] = word_after_probe;
    return word_result;
}

int SzEffect_GetPlayMotion(int flag)
{
    assert(comparison_calls == 1);
    comparison_order[comparison_calls++] = 2;
    comparison_flag = flag;
    return comparison_result;
}

static int note(int target, int row, int column, int flag, void *pointer)
{
    calls++;
    last_target = target;
    last_row = row;
    last_column = column;
    last_flag = flag;
    last_pointer = pointer;
    return 0x200 + target;
}

int SzDuel_SetBusyFlag(int a, int b, int flag) { return note(0, a, b, flag, 0); }
int SzDuel_SetCardBusyFlag(int a, int b, int flag) { return note(1, a, b, flag, 0); }
int SzDuel_SetCardColBusyFlag(int a, int b, int flag) { return note(2, a, b, flag, 0); }
int SzDuel_SetModelBusyFlag(int a, int b, int flag) { return note(3, a, b, flag, 0); }
int SzDuel_SetCardPos(int a, int b, struct DotrStateVector *v) { return note(4, a, b, 0, v); }
int SzDuel_GetCardPos(int a, int b, struct DotrStateVector *v) { return note(5, a, b, 0, v); }
int SzDuel_SetCardRot(int a, int b, struct DotrStateVector *v) { return note(6, a, b, 0, v); }
int SzDuel_GetCardRot(int a, int b, struct DotrStateVector *v) { return note(7, a, b, 0, v); }
int SzDuel_SetCardScale(int a, int b, struct DotrStateVector *v) { return note(8, a, b, 0, v); }
int SzDuel_GetCardScale(int a, int b, struct DotrStateVector *v) { return note(9, a, b, 0, v); }
int SzDuel_SetCardRgba(int a, int b, struct DotrStateCellQuad *v) { return note(10, a, b, 0, v); }
int SzDuel_GetCardRgba(int a, int b, struct DotrStateCellQuad *v) { return note(11, a, b, 0, v); }

struct DotrStateItem *SzDuel_GetUnit(int a, int b)
{
    lookup_row = a;
    lookup_column = b;
    return lookup_result;
}

struct DotrStateItem *SzDuel_GetCardInfo(int a, int b)
{
    (void)a;
    (void)b;
    return 0; /* only referenced by an unused inline helper in the header */
}

int SzModel_IsModelDisp(int index)
{
    probe_argument = index;
    return 0x77;
}

static int object_result, object_row, object_column, object_calls;
static int t_calls, t_id, t_index, t_value;
static float t_float;
static void *t_pointer;

int SzDuel_GetUnitModelID(int a, int b)
{
    object_calls++;
    object_row = a;
    object_column = b;
    return object_result;
}

static int target(int id, int index, int value, float f, void *pointer)
{
    t_calls++;
    t_id = id;
    t_index = index;
    t_value = value;
    t_float = f;
    t_pointer = pointer;
    return 0x400 + id;
}

int SzModel_SetPos(int i, struct DotrObjectVector4 *v) { return target(0, i, 0, 0.0f, v); }
int SzModel_GetPos(int i, struct DotrObjectVector4 *v) { return target(1, i, 0, 0.0f, v); }
int SzModel_GetCenter(int i, struct DotrObjectVector4 *v) { return target(24, i, 0, 0.0f, v); }
int SzModel_SetRot(int i, struct DotrObjectVector4 *v) { return target(2, i, 0, 0.0f, v); }
int SzModel_SetRotC(int i, struct DotrObjectVector4 *v) { return target(3, i, 0, 0.0f, v); }
int SzModel_GetRot(int i, struct DotrObjectVector4 *v) { return target(4, i, 0, 0.0f, v); }
int SzModel_GetRotC(int i, struct DotrObjectVector4 *v) { return target(5, i, 0, 0.0f, v); }
int SzModel_SetScale(int i, struct DotrObjectVector4 *v) { return target(6, i, 0, 0.0f, v); }
int SzModel_GetScale(int i, struct DotrObjectVector4 *v) { return target(7, i, 0, 0.0f, v); }
int SzModel_SetRgba(int i, int n, struct DotrObjectQuad *q) { return target(8, i, n, 0.0f, q); }
int SzModel_GetRgba(int i, struct DotrObjectQuad *q) { return target(9, i, 0, 0.0f, q) - 0x409; }
int SzModel_SetFilter(int i, int n, struct DotrObjectQuad *q) { return target(10, i, n, 0.0f, q); }
static int step_on, step_state, step_random, step_random_calls, step_ids[4], step_count;
static float step_level, step_floats[4];
static int step(int id, float f, int result)
{
    if (step_on && step_count < 4) {
        step_ids[step_count] = id;
        step_floats[step_count++] = f;
    }
    return result;
}
int func_0019a5c8(void) { step_random_calls++; return step_random; }
int SzModel_GetAnim(int i) { return step(11, 0.0f, step_on ? step_state : target(11, i, 0, 0.0f, 0)); }
int SzModel_GetRefNum(int i) { return target(16, i, 0, 0.0f, 0); }
float SzModel_GetRadius(int i) { target(17, i, 0, 0.0f, 0); return -2.5f; }
int SzModel_GetModelNo(int i) { return target(18, i, 0, 0.0f, 0); }
int SzDuel_Xax2New(int value) { return target(19, value, 0, 0.0f, 0); }
int SzDuel_GetCamDist(int index) { return target(20, index, 0, 0.0f, 0); }
int SzModel_SetRefID(int i, int n, int m) { t_float = (float)m; return target(21, i, n, (float)m, 0); }
int SzModel_GetRef(int i, int n, struct DotrObjectVector4 *v) { return target(22, i, n, 0.0f, v); }
int SzModel_SetMaxOT(int i, int n) { return target(23, i, n, 0.0f, 0); }
static int slot_word[8], slot_cleared[8], slot_reads;
int SzModel_InqRefID(int i, int slot)
{
    assert(i == object_result && slot == slot_reads && slot < 8);
    slot_reads++;
    return slot_word[slot];
}
int SzModel_SetRefID_RefNo(int i, int n, int slot)
{
    assert(i == object_result && n == -1 && slot == slot_reads - 1);
    slot_cleared[slot]++;
    return 1;
}
int SzModel_SetAnimIntr(int i, int n) { return step(12, (float)n, target(12, i, n, 0.0f, 0)); }
int SzModel_SetAnimDir(int i, int n) { return target(13, i, n, 0.0f, 0); }
float SzModel_GetAnimFrame(int i) { target(14, i, 0, 0.0f, 0); step(14, 0.0f, 0); return step_on ? step_level : 6.5f; }
int SzModel_SetAnimFrame(int i, float f) { return step(15, f, target(15, i, 0, f, 0)); }

static const int rejected[] = {-1, -256, 0x300, 0x20b, 0x00ff, INT_MIN, INT_MAX};
static const int accepted[][3] = {{0x0000, 0, 0}, {0x020a, 2, 10}, {0x0105, 1, 5}};

typedef int (*FlagWrapper)(int, int);
typedef int (*VectorWrapper)(int, struct DotrStateVector *);

static void check_guarded(void)
{
    static const FlagWrapper flags[] = {SzEffect_SetBusyFlag2, SzEffect_SetCardBusyFlag2, SzEffect_SetCardColBusyFlag2, SzEffect_SetModelBusyFlag2};
    struct DotrStateVector v, zero_w1 = {0.0f, 0.0f, 0.0f, 1.0f}, zeros = {0.0f, 0.0f, 0.0f, 0.0f}, kept;
    struct DotrStateCellQuad q, q128 = {128, 128, 128, 128}, q_kept = {1, 2, 3, 4};
    static const struct { VectorWrapper wrapper; int target; int defaults; } vectors[] = {
        {SzEffect_SetCardPos2, 4, 0}, {SzEffect_GetCardPos2, 5, 1}, {SzEffect_SetCardRot2, 6, 0},
        {SzEffect_GetCardRot2, 7, 2}, {SzEffect_SetCardScale2, 8, 0}, {SzEffect_GetCardScale2, 9, 1},
    };
    size_t i, j;
    for (i = 0; i < sizeof(flags) / sizeof(flags[0]); i++) {
        calls = 0;
        for (j = 0; j < sizeof(rejected) / sizeof(rejected[0]); j++) assert(flags[i](rejected[j], 1) == 0);
        assert(calls == 0);
        for (j = 0; j < sizeof(accepted) / sizeof(accepted[0]); j++) {
            assert(flags[i](accepted[j][0], -9) == 0x200 + (int)i);
            assert(last_target == (int)i && last_row == accepted[j][1] && last_column == accepted[j][2] && last_flag == -9);
        }
    }
    for (i = 0; i < sizeof(vectors) / sizeof(vectors[0]); i++) {
        calls = 0;
        for (j = 0; j < sizeof(rejected) / sizeof(rejected[0]); j++) {
            kept.x = 5.0f;
            kept.y = 6.0f;
            kept.z = 7.0f;
            kept.unknown_w = 8.0f;
            v = kept;
            assert(vectors[i].wrapper(rejected[j], &v) == 0);
            /* The getters preset their output even when the position is
             * rejected; the setters leave the vector alone. */
            if (vectors[i].defaults == 1) assert(memcmp(&v, &zero_w1, sizeof(v)) == 0);
            else if (vectors[i].defaults == 2) assert(memcmp(&v, &zeros, sizeof(v)) == 0);
            else assert(memcmp(&v, &kept, sizeof(v)) == 0);
        }
        assert(calls == 0);
        for (j = 0; j < sizeof(accepted) / sizeof(accepted[0]); j++) {
            assert(vectors[i].wrapper(accepted[j][0], &v) == 0x200 + vectors[i].target);
            assert(last_row == accepted[j][1] && last_column == accepted[j][2] && last_pointer == (void *)&v);
        }
    }
    calls = 0;
    for (j = 0; j < sizeof(rejected) / sizeof(rejected[0]); j++) {
        q = q_kept;
        assert(SzEffect_SetCardRgba2(rejected[j], &q) == 0 && memcmp(&q, &q_kept, sizeof(q)) == 0);
        assert(SzEffect_GetCardRgba2(rejected[j], &q) == 0 && memcmp(&q, &q128, sizeof(q)) == 0);
    }
    assert(calls == 0);
    assert(SzEffect_SetCardRgba2(0x0203, &q) == 0x20a && last_row == 2 && last_column == 3 && last_pointer == (void *)&q);
    q = q_kept;
    assert(SzEffect_GetCardRgba2(0x0203, &q) == 0x20b && memcmp(&q, &q128, sizeof(q)) == 0);
}

static void check_lookups_and_words(void)
{
    struct DotrStateItemObject object;
    int index;
    lookup_result = 0;
    assert(SzEffect_IsCardStandby2(0x1234) == 0 && lookup_row == 0x12 && lookup_column == 0x34);
    assert(SzEffect_GetCardRequest2(-1) == 0 && lookup_row == -1 && lookup_column == 0xff);
    assert(SzEffect_IsModelStandby2(0x0102) == 0);
    lookup_result = &item;
    memset(&item, 0, sizeof(item));
    assert(SzEffect_IsCardStandby2(0) == 0 && SzEffect_GetCardRequest2(0) == 0);
    item.cardModel = &object;
    item.modelId = 83;
    assert(SzEffect_IsCardStandby2(0) == 1 && SzEffect_GetCardRequest2(0) == &object);
    assert(SzEffect_IsModelStandby2(0x0209) == 0x77 && probe_argument == 83 && lookup_row == 2 && lookup_column == 9);

    data_0031be98[0] = data_0031be9c[0] = 0;
    SzEffect_SetTarget(0x0107);
    assert(data_0031be98[0] == 0x0107 && data_0031be9c[0] == 0 && SzEffect_GetTarget() == 0x0107);
    SzEffect_SetFrom(-5);
    assert(data_0031be98[0] == 0x0107 && data_0031be9c[0] == -5);
    assert(SzEffect_IsModelStandby() == 0x77 && lookup_row == 1 && lookup_column == 7);

    memset(&gSzEffect_el, 0, sizeof(gSzEffect_el));
    for (index = 0; index < 24; index++) {
        gSzEffect_el.unknown_100[index].unknown_10 = 1000 + index;
        gSzEffect_el.unknown_100[index].unknown_bits_0_2 = 7;
        gSzEffect_el.unknown_100[index].unknown_bits_3_15 = (unsigned short)(0x1fff - index);
        assert(SzEffect_GetStatus(index) == 1000 + index && SzEffect_GetCount(index) == 0x1fff - index);
    }
    assert(SzEffect_GetStatus(-1) == -1 && SzEffect_GetStatus(24) == -1);
    assert(SzEffect_GetCount(-1) == -1 && SzEffect_GetCount(24) == -1);
}

typedef int (*ObjectVector)(int, struct DotrObjectVector4 *);

static void check_object_wrappers(void)
{
    static const struct { ObjectVector wrapper; int id; int preset; } vectors[] = {
        {SzEffect_SetModelPos2, 0, 0}, {SzEffect_GetModelPos2, 1, 1}, {SzEffect_SetModelRot2, 2, 0}, {SzEffect_SetModelRotC2, 3, 0},
        {SzEffect_GetModelRot2, 4, 2}, {SzEffect_GetModelRotC2, 5, 2}, {SzEffect_SetModelScale2, 6, 0}, {SzEffect_GetModelScale2, 7, 1},
    };
    struct DotrObjectVector4 v, kept = {5.0f, 6.0f, 7.0f, 8.0f}, zero_w1 = {0.0f, 0.0f, 0.0f, 1.0f}, zeros = {0.0f, 0.0f, 0.0f, 0.0f};
    struct DotrObjectQuad q, q_kept = {1, 2, 3, 4}, q128 = {128, 128, 128, 128};
    size_t i, j;
    for (i = 0; i < sizeof(vectors) / sizeof(vectors[0]); i++) {
        /* Rejected position: no lookup at all. Negative object index: lookup
         * but no forwarded call. Both return 0 after any preset. */
        for (j = 0; j < sizeof(rejected) / sizeof(rejected[0]) + 1; j++) {
            int packed = j < sizeof(rejected) / sizeof(rejected[0]) ? rejected[j] : 0x0102;
            object_calls = t_calls = 0;
            object_result = -1;
            v = kept;
            assert(vectors[i].wrapper(packed, &v) == 0 && t_calls == 0);
            assert(object_calls == (packed == 0x0102));
            if (vectors[i].preset == 1) assert(memcmp(&v, &zero_w1, sizeof(v)) == 0);
            else if (vectors[i].preset == 2) assert(memcmp(&v, &zeros, sizeof(v)) == 0);
            else assert(memcmp(&v, &kept, sizeof(v)) == 0);
        }
        object_result = 83;
        assert(vectors[i].wrapper(0x020a, &v) == 0x400 + vectors[i].id);
        assert(object_row == 2 && object_column == 10 && t_index == 83 && t_pointer == (void *)&v);
    }
    /* Byte-quad wrappers. */
    object_result = 7;
    t_calls = 0;
    assert(SzEffect_SetModelRgbaX(0x0001, 3, &q) == 0x408 && t_index == 7 && t_value == 3 && t_pointer == (void *)&q);
    assert(SzEffect_SetModelRgba2(0x0001, &q) == 0x408 && t_value == -1);
    assert(SzEffect_SetModelFilterX(0x0001, 2, &q) == 0x40a && t_value == 2);
    assert(SzEffect_SetModelFilter2(0x0001, &q) == 0x40a && t_value == -1);
    assert(SzEffect_SetModelRgbaX(0x0300, 3, &q) == 0 && SzEffect_SetModelFilterX(0x000b, 3, &q) == 0);
    q = q_kept;
    /* The stubbed 0x1f2660 returns 0 here. */
    assert(SzEffect_GetModelRgbaX(0x0001, &q) == 0 && memcmp(&q, &q128, sizeof(q)) == 0 && t_id == 9);
    q = q_kept;
    assert(SzEffect_GetModelRgbaX(0x0300, &q) == -1 && memcmp(&q, &q128, sizeof(q)) == 0);
    object_result = -1;
    assert(SzEffect_GetModelRgbaX(0x0001, &q) == -1);
    assert(SzEffect_GetModelRgba2(0x0001, &q) == 0); /* negative result becomes 0 */
    object_result = 7;
    assert(SzEffect_GetModelRgba2(0x0001, &q) == 1);

    /* Scalar wrappers. */
    assert(SzEffect_GetAnim2(0x0105) == 0x40b && object_row == 1 && object_column == 5 && t_index == 7);
    assert(SzEffect_SetAnimIntr2(0x0105, -4) == 0x40c && t_value == -4);
    assert(SzEffect_SetAnimDir2(0x0105, 9) == 0x40d && t_value == 9);
    assert(SzEffect_GetAnimFrame2(0x0105) == 6.5f && t_id == 14);
    assert(SzEffect_SetAnimFrame2(0x0105, 2.25f) == 0x40f && t_float == 2.25f);
    assert(SzEffect_GetAnim2(0x0300) == -1 && SzEffect_SetAnimIntr2(-1, 1) == 0 && SzEffect_SetAnimDir2(0x000b, 1) == 0);
    assert(SzEffect_GetAnimFrame2(0x0300) == 0.0f && SzEffect_SetAnimFrame2(0x0300, 1.0f) == 0);
    object_result = -1;
    t_calls = 0;
    assert(SzEffect_GetAnim2(0x0105) == -1 && SzEffect_SetAnimIntr2(0x0105, 1) == 0 && SzEffect_SetAnimDir2(0x0105, 1) == 0);
    assert(SzEffect_GetAnimFrame2(0x0105) == 0.0f && SzEffect_SetAnimFrame2(0x0105, 1.0f) == 0 && t_calls == 0);

    /* Thunks that take the packed position from the stored word. */
    object_result = 7;
    data_0031be98[0] = 0x0203;
    assert(SzEffect_SetModelRotC(&v) == 0x403 && object_row == 2 && object_column == 3 && t_pointer == (void *)&v);
    assert(SzEffect_GetModelRotC(&v) == 0x405 && object_row == 2 && object_column == 3);
    assert(SzEffect_GetModelRgba(&q) == 1 && t_id == 9 && t_pointer == (void *)&q);
    assert(SzEffect_SetModelFilter(&q) == 0x40a && t_value == -1 && object_column == 3);
}

static void check_more_object_wrappers(void)
{
    struct DotrObjectVector4 v, zero_w1 = {0.0f, 0.0f, 0.0f, 1.0f};
    object_result = 7;
    t_calls = 0;
    assert(SzEffect_GetRefNum2(0x0105) == 0x410 && t_index == 7);
    assert(SzEffect_GetRadius2(0x0105) == -2.5f && t_id == 17 && t_index == 7);
    /* Three chained calls: each result feeds the next. */
    t_calls = 0;
    assert(SzEffect_GetCamDist2(0x0105) == 0x414 && t_calls == 3 && t_index == 0x413);
    memset(&v, 0x7f, sizeof(v));
    assert(SzEffect_GetRef2(0x0105, -9, &v) == 0x416 && t_value == -9 && t_pointer == (void *)&v);
    assert(memcmp(&v, &zero_w1, sizeof(v)) == 0);
    /* A non-negative value is combined with the low seven bits of the stored
     * word at +0x340; a negative value is passed through. */
    gSzEffect_el.unknown_340 = 0x1a5;
    assert(SzEffect_SetRefID2(0x0105, 0x234, 77) == 0x415 && t_value == (0x2500 | 0x34) && t_float == 77.0f);
    assert(SzEffect_SetRefID2(0x0105, -3, 0) == 0x415 && t_value == -3);
    /* Rejections. */
    t_calls = 0;
    assert(SzEffect_GetRefNum2(0x0300) == -1 && SzEffect_GetRadius2(0x0300) == 0.0f && SzEffect_GetCamDist2(-1) == -1);
    assert(SzEffect_SetRefID2(0x000b, 1, 1) == -1);
    memset(&v, 0x7f, sizeof(v));
    assert(SzEffect_GetRef2(0x0300, 1, &v) == 0 && memcmp(&v, &zero_w1, sizeof(v)) == 0);
    object_result = -1;
    assert(SzEffect_GetRefNum2(0x0105) == -1 && SzEffect_GetRadius2(0x0105) == 0.0f && SzEffect_GetCamDist2(0x0105) == -1);
    assert(SzEffect_SetRefID2(0x0105, 1, 1) == -1 && SzEffect_GetRef2(0x0105, 1, &v) == 0 && t_calls == 0);
}

static void check_stored_word_comparison(void)
{
    static const int stored_values[] = {INT_MIN, -1, 0, 1, INT_MAX};
    static const int results[] = {INT_MIN, -1, 0, 3, 4, 5, INT_MAX};
    size_t i, j;
    int unequal;
    for (i = 0; i < sizeof(stored_values) / sizeof(stored_values[0]); i++) {
        for (unequal = 0; unequal <= 1; unequal++) {
            word_result = stored_values[i] ^ unequal;
            /* Change the stored word so a reread after the first call would
             * reverse the equality result in both cases. */
            word_after_probe = unequal ? word_result : word_result ^ 1;
            for (j = 0; j < sizeof(results) / sizeof(results[0]); j++) {
                data_0031be9c[0] = stored_values[i];
                comparison_result = results[j];
                comparison_calls = 0;
                assert(SzEffect_InqDuelResult() == (results[j] == 4));
                assert(comparison_calls == 2 && comparison_order[0] == 1 && comparison_order[1] == 2);
                assert(comparison_flag == unequal && data_0031be9c[0] == word_after_probe);
            }
        }
    }
}

/* SzEffect_ClearRefID2, SzEffect_SetMaxOT2 (branch and loop coverage) and the two tail-jump
 * forwarders SzEffect_SetMaxOT and SzEffect_GetRadius (one call each, arguments and result). */
static void check_slot_clear_and_stored(void)
{
    static const int words[8] = {-1, 0, 7, 8, 0x100, 5, 0x7fff, 3};
    static const int bad[4] = {-1, 0x0300, 0x01ff, 0x000b};
    int saved_98 = data_0031be98[0], saved_9c = data_0031be9c[0];
    int i;

    memcpy(slot_word, words, sizeof(words));
    object_result = 7;
    for (i = 0; i < 4; i++) {
        object_calls = slot_reads = t_calls = 0;
        assert(SzEffect_ClearRefID2(bad[i], 0) == 0 && SzEffect_SetMaxOT2(bad[i], 5) == 0);
        assert(object_calls == 0 && slot_reads == 0 && t_calls == 0);
    }
    /* First and last accepted row and column. */
    object_calls = 0;
    assert(SzEffect_SetMaxOT2(0x0000, 5) == 0x417 && SzEffect_SetMaxOT2(0x020a, 5) == 0x417 && object_calls == 2);
    object_result = -1;
    object_calls = slot_reads = t_calls = 0;
    assert(SzEffect_ClearRefID2(0x0105, 0) == 0 && SzEffect_SetMaxOT2(0x0105, 5) == 0);
    assert(object_calls == 2 && slot_reads == 0 && t_calls == 0);

    object_result = 7;
    memset(slot_cleared, 0, sizeof(slot_cleared));
    slot_reads = object_calls = 0;
    assert(SzEffect_ClearRefID2(0x020a, 0) == 1 && slot_reads == 8 && object_row == 2 && object_column == 10);
    for (i = 0; i < 8; i++) {
        /* Words 0, 7, 5 and 3 are in 0..7; -1, 8, 0x100 and 0x7fff are not. */
        assert(slot_cleared[i] == (i == 1 || i == 2 || i == 5 || i == 7));
    }
    /* A word in 0..7 has zero bits 8..14, so another value clears nothing. */
    memset(slot_cleared, 0, sizeof(slot_cleared));
    slot_reads = 0;
    assert(SzEffect_ClearRefID2(0x0000, 1) == 1 && slot_reads == 8);
    for (i = 0; i < 8; i++) {
        assert(slot_cleared[i] == 0);
    }

    t_calls = 0;
    assert(SzEffect_SetMaxOT2(0x0105, 0x66) == 0x417 && t_calls == 1 && t_id == 23 && t_index == 7 && t_value == 0x66);
    assert(object_row == 1 && object_column == 5);
    data_0031be9c[0] = 0x0203;
    data_0031be98[0] = 0x0109;
    t_calls = 0;
    assert(SzEffect_SetMaxOT(0x55) == 0x417 && t_calls == 1 && t_index == 7 && t_value == 0x55);
    assert(object_row == 2 && object_column == 3);
    t_calls = 0;
    assert(SzEffect_GetRadius() == -2.5f && t_calls == 1 && t_id == 17 && t_index == 7);
    assert(object_row == 1 && object_column == 9);
    data_0031be98[0] = saved_98;
    data_0031be9c[0] = saved_9c;
}

static void check_kind_vector(void)
{
    static const int outside[] = {-0x100, 0x0300, 0x000b, 0x00ff, 0x030b};
    struct DotrObjectVector4 v;
    size_t i;
    int kind;

    /* Getter and the stored-position forwarder. */
    data_0031be9c[0] = 0x1234;
    assert(SzEffect_GetFrom() == 0x1234);

    /* Outside 3 x 11 (x -1, x 3, y 11, y 255, both): preset only, no lookup. */
    for (i = 0; i < sizeof(outside) / sizeof(outside[0]); i++) {
        object_calls = t_calls = calls = 0;
        v.x = v.y = v.z = v.unknown_w = 9.0f;
        assert(SzEffect_GetPos2(0, outside[i], &v) == 0 && object_calls == 0 && t_calls == 0 && calls == 0);
        assert(v.x == 0.0f && v.y == 0.0f && v.z == 0.0f && v.unknown_w == 1.0f);
    }

    /* Last position inside (2, 10), no object there: lookup, no forwarded call. */
    object_calls = t_calls = calls = 0;
    object_result = -1;
    assert(SzEffect_GetPos2(1, 0x020a, &v) == 0 && object_calls == 1 && object_row == 2 && object_column == 10);
    assert(t_calls == 0 && calls == 0);

    /* Object index 0 is accepted. kind 0: grid vector at (x, y). */
    object_result = 0;
    object_calls = t_calls = calls = 0;
    assert(SzEffect_GetPos2(0, 0x0105, &v) == 0x205 && calls == 1 && last_row == 1 && last_column == 5);
    assert(last_pointer == &v && t_calls == 0);
    /* kind 1: the object's vector. */
    object_result = 6;
    assert(SzEffect_GetPos2(1, 0x0000, &v) == 0x418 && t_id == 24 && t_index == 6 && t_pointer == &v);
    /* kind 2: object 8 whatever the lookup gave. */
    assert(SzEffect_GetPos2(2, 0x0000, &v) == 0x401 && t_id == 1 && t_index == 8 && t_pointer == &v);
    /* Other kinds: 0, nothing forwarded. */
    for (kind = -1; kind <= 3; kind += 4) {
        t_calls = calls = 0;
        assert(SzEffect_GetPos2(kind, 0x0000, &v) == 0 && t_calls == 0 && calls == 0);
    }

    /* 0x1f53b0: same with the position from 0x31be98. */
    data_0031be98[0] = 0x0207;
    object_result = 3;
    object_calls = 0;
    v.unknown_w = 9.0f;
    assert(SzEffect_GetPos(1, &v) == 0x418 && object_row == 2 && object_column == 7 && t_index == 3);
    assert(t_pointer == &v && v.unknown_w == 1.0f);
}

/* TaEffect_SetOuchMotion on the real wrappers with the 0x1f13f0/0x1f1550/0x1f1b50/
 * 0x1f1c40/0x19a5c8 stubs: one assertion per branch outcome and per side of
 * the 16 and 11 bounds. Returns the number of recorded calls. */
static int step_case(int state, float level, int random)
{
    object_result = 7;
    step_on = 1;
    step_state = state;
    step_level = level;
    step_random = random;
    step_random_calls = 0;
    step_count = 0;
    TaEffect_SetOuchMotion(0x0105);
    step_on = 0;
    assert(object_row == 1 && object_column == 5 && t_index == 7);
    return step_count;
}

static void check_step(void)
{
    /* state other than 4: 4 is stored and nothing else is read */
    assert(step_case(3, 20.0f, 0) == 2 && step_ids[1] == 12 && step_floats[1] == 4.0f && step_random_calls == 0);
    assert(step_case(5, 20.0f, 0) == 2 && step_ids[1] == 12 && step_floats[1] == 4.0f);
    /* 16 and above: set to 11 */
    assert(step_case(4, 16.0f, 0) == 3 && step_ids[1] == 14 && step_ids[2] == 15 && step_floats[2] == 11.0f);
    assert(step_random_calls == 0);
    /* just under 16: one chance in three to drop by 2 */
    assert(step_case(4, 15.5f, 6) == 3 && step_ids[2] == 15 && step_floats[2] == 13.5f && step_random_calls == 1);
    assert(step_case(4, 15.5f, 7) == 2 && step_ids[1] == 14 && step_random_calls == 1);
    assert(step_case(4, 11.5f, 0) == 3 && step_floats[2] == 9.5f && step_random_calls == 1);
    /* 11 and below: untouched, no random draw */
    assert(step_case(4, 11.0f, 0) == 2 && step_random_calls == 0);
    assert(step_case(4, 3.0f, 0) == 2 && step_random_calls == 0);
}

int main(void)
{
    check_kind_vector();
    check_slot_clear_and_stored();
    check_guarded();
    check_lookups_and_words();
    check_object_wrappers();
    check_more_object_wrappers();
    check_stored_word_comparison();
    check_step();
    puts("packed_item: ok");
    return 0;
}
