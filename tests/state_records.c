/* Synthetic offset/byte tests. Separate fixtures for aliasing target views:
 * this program does not reproduce their shared physical EE storage. */
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/state_records.h"

/* A fifth record so that the control bytes at +3415/+3427 exist in this fixture. */
struct DotrStateRecord data_00317490_records[5];
struct DotrStateSetupBytes data_00a9c08c;
struct DotrStateVector data_002914b0 = {1.0f, 2.0f, 3.0f, 4.0f};
struct DotrStateVector data_002914c0 = {5.0f, 6.0f, 7.0f, 8.0f};
static int setup_mode, setup_calls, setup_sets;
struct DotrStateControl data_00317490_control[1];
unsigned char turnFirst[1], turnCount[1], turnOwner[1];
struct DotrStateRecordBits data_003181f6[1];

static int calls, expected_index, expected_x, expected_z, expected_y;
static const struct DotrStateVector saved_value = {4.0f, 5.0f, 6.0f, 7.0f};

/* The compactors assign a whole list structure from a stack temporary, so the
 * structure's trailing padding byte is indeterminate afterwards. Reset it
 * before whole-fixture comparisons; this program does not check that byte. */
static void reset_list_padding(void)
{
    int index;
    for (index = 0; index < 4; index++) {
        unsigned char *bytes = (unsigned char *)&data_00317490_records[index];
        bytes[0x1e0 + 81] = 0xa5;
        bytes[0x232 + 241] = 0xa5;
        bytes[0x324 + 11] = 0xa5;
    }
}

void SzView_IV2V(void *source, struct DotrStateVector *saved)
{
    assert(calls++ == 0);
    assert(source == (unsigned char *)&data_00317490_records[expected_index] + 0x1d0);
    memcpy(saved, source, sizeof(*saved));
}

int SzModel_SetDrawFlag(int index, int value)
{
    int n = setup_sets++;
    if (n < 84) {
        assert(index == n && value == 0);
    } else if (n < 89) {
        assert(index == n - 84 && value == 1);
    } else {
        assert(n < 99 && index == n - 89 + 74 && value == 1);
    }
    return 0;
}

void SzDuel_InitMapColor2(int a, int b, int c, int d)
{
    assert(setup_calls++ == 2 && a == 0 && b == 0 && c == 1 && d == 0);
    assert(data_00a9c08c.unknown_3 == 3 && data_00a9c08c.unknown_4 == 3);
    assert(data_00a9c08c.unknown_5 == 0 && data_00a9c08c.unknown_6_bit0 == 0);
}

void SzView_SetCP(int unknown, struct DotrStateVector *position)
{
    if (setup_mode) {
        assert(setup_calls++ == 0 && setup_sets == 99 && unknown == 0);
        assert(memcmp(position, &data_002914b0, sizeof(*position)) == 0);
        return;
    }
    const unsigned char *record = (const unsigned char *)&data_00317490_records[expected_index];
    assert(calls++ == 1 && unknown == 0);
    assert(record[0x33d] == expected_x && record[0x33e] == expected_z);
    assert(position->x == (float)((expected_x - 3) * 50));
    assert(position->z == (float)((expected_z - 3) * 50));
    assert(position->y == (float)expected_y);
    /* The routine never initializes position->unknown_w. */
}

void SzView_SetLookSP(int unknown, struct DotrStateVector *saved)
{
    if (setup_mode) {
        assert(setup_calls++ == 1 && unknown == 0);
        assert(memcmp(saved, &data_002914c0, sizeof(*saved)) == 0);
        return;
    }
    assert(calls++ == 2 && unknown == 0);
    assert(memcmp(saved, &saved_value, sizeof(*saved)) == 0);
}

static struct DotrStateItem lookup_object, *lookup_result;
static int lookup_a, lookup_b;
static int block_calls, block_argument, forward_calls, forward_args[4];
static struct DotrStateBlock *block_pointer;

struct DotrStateItem *SzDuel_GetCardInfo(int a, int b)
{
    lookup_a = a;
    lookup_b = b;
    return lookup_result;
}

void SzDuel_InitCardInfo(struct DotrStateBlock *block, int unknown)
{
    int i;
    assert(forward_calls == 0);
    block_calls++;
    block_pointer = block;
    block_argument = unknown;
    for (i = 0; i < 24; i++) block->unknown_words[i] = 0x1000 + i;
}

int SzDuel_AddUnit2(int a, int b, int c, struct DotrStateBlock *block, int e)
{
    int i;
    assert(block_calls == 1 && block == block_pointer);
    for (i = 0; i < 24; i++) assert(block->unknown_words[i] == 0x1000 + i);
    forward_calls++;
    forward_args[0] = a;
    forward_args[1] = b;
    forward_args[2] = c;
    forward_args[3] = e;
    return 0;
}

static void check_layout(void)
{
    assert(offsetof(struct DotrStateRecord, unknown_1e0) == 0x1e0);
    assert(offsetof(struct DotrStateRecordList, unknown_count) == 0x50);
    assert(offsetof(struct DotrStateRecord, unknown_232) == 0x232);
    assert(offsetof(struct DotrStateRecordList120, unknown_count) == 0xf0);
    assert(offsetof(struct DotrStateRecord, unknown_33c) == 0x33c);
    assert(offsetof(struct DotrStateRecord, unknown_341) == 0x341);
    assert(offsetof(struct DotrStateRecord, unknown_342) == 0x342);
    assert(offsetof(struct DotrStateRecord, unknown_344) == 0x344);
    assert(offsetof(struct DotrStateRecord, unknown_346) == 0x346);
    assert(offsetof(struct DotrStateControl, unknown_d53) == 0xd53);
    assert(offsetof(struct DotrStateControl, unknown_d66) == 0xd66);
    assert(offsetof(struct DotrStateControl, unknown_d67) == 0xd67);
}

static void check_list(void)
{
    static const int invalid[] = {INT_MIN, -1, 40, INT_MAX};
    int index, position, count;
    size_t i;
    for (index = 0; index < 4; index++) {
        for (position = 0; position < 40; position++) {
            unsigned short value = (unsigned short)(0xffff - index * 1000 - position);
            data_00317490_records[index].unknown_1e0.unknown_values[position] = value;
            assert(SzDuel_InqDeck(index, position) == value);
        }
        for (position = 0; position < 120; position++) {
            unsigned short value = (unsigned short)(0xffff - index * 1000 - position);
            data_00317490_records[index].unknown_232.unknown_values[position] = value;
            assert(SzDuel_InqGrave(index, position) == value);
        }
        assert(SzDuel_InqGrave(index, INT_MIN) == -1);
        assert(SzDuel_InqGrave(index, -1) == -1);
        assert(SzDuel_InqGrave(index, 120) == -1);
        assert(SzDuel_InqGrave(index, INT_MAX) == -1);
        for (i = 0; i < sizeof(invalid) / sizeof(invalid[0]); i++) {
            assert(SzDuel_InqDeck(index, invalid[i]) == -1);
        }
        for (count = 0; count < 256; count++) {
            data_00317490_records[index].unknown_1e0.unknown_count = (unsigned char)count;
            assert(SzDuel_GetDeckCount(index) == count);
            data_00317490_records[index].unknown_232.unknown_count = (unsigned char)count;
            assert(SzDuel_GetGraveCount(index) == count);
        }
    }
}

static void check_list_updates(void)
{
    static const int invalid[] = {INT_MIN, -1, 671, 854, 999, INT_MAX};
    static const int valid[] = {0, 670, 672, 853};
    static const int counts[] = {0, 1, 40, 119};
    struct DotrStateRecord expected[4];
    int index, position, n, j;
    size_t v, c;
    for (index = 0; index < 4; index++) {
        struct DotrStateRecordList *list = &data_00317490_records[index].unknown_1e0;
        struct DotrStateRecordList120 *large = &data_00317490_records[index].unknown_232;
        memset(data_00317490_records, 0xa5, sizeof(data_00317490_records));
        for (position = 0; position < 40; position++) list->unknown_values[position] = 999;
        list->unknown_count = 0;
        memcpy(expected, data_00317490_records, sizeof(expected));
        for (v = 0; v < sizeof(invalid) / sizeof(invalid[0]); v++) {
            assert(SzDuel_SetDeck(index, 0, invalid[v]) == 0);
            assert(SzDuel_AddGrave(index, invalid[v]) == -1);
            assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);
        }
        assert(SzDuel_SetDeck(index, -1, 1) == 0);
        assert(SzDuel_SetDeck(index, 40, 1) == 0);
        assert(SzDuel_DelDeck(index, INT_MIN) == 0);
        assert(SzDuel_DelDeck(index, INT_MAX) == 0);
        assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);
        for (position = 0; position < 40; position++) {
            for (v = 0; v < sizeof(valid) / sizeof(valid[0]); v++) {
                memcpy(data_00317490_records, expected, sizeof(expected));
                expected[index].unknown_1e0.unknown_values[position] = (unsigned short)valid[v];
                expected[index].unknown_1e0.unknown_count = 1;
                assert(SzDuel_SetDeck(index, position, valid[v]) == 1);
                assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);
                assert(SzDuel_SetDeck(index, position, 1) == 0);
                assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);
                expected[index].unknown_1e0.unknown_values[position] = 999;
                expected[index].unknown_1e0.unknown_count = 0;
                assert(SzDuel_DelDeck(index, position) == 1);
                assert(SzDuel_DelDeck(index, position) == 0);
                assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);
            }
        }
        list->unknown_count = 39;
        assert(SzDuel_SetDeck(index, 39, 853) == 1 && list->unknown_count == 40);
        memcpy(expected, data_00317490_records, sizeof(expected));
        assert(SzDuel_SetDeck(index, 0, 1) == 0);
        assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);
        list->unknown_count = 0;
        assert(SzDuel_DelDeck(index, 39) == 1 && list->unknown_count == 255);

        for (c = 0; c < sizeof(counts) / sizeof(counts[0]); c++) {
            n = counts[c];
            for (position = 0; position < 120; position++) large->unknown_values[position] = 999;
            /* A hole at the front, followed by exactly n occupied slots. */
            for (position = 1; position <= n; position++) large->unknown_values[position] = (unsigned short)(position + 10);
            large->unknown_count = (unsigned char)n;
            memcpy(expected, data_00317490_records, sizeof(expected));
            for (j = 0; j < n; j++) expected[index].unknown_232.unknown_values[j] = (unsigned short)(j + 11);
            expected[index].unknown_232.unknown_values[n] = 853;
            for (j = n + 1; j < 120; j++) expected[index].unknown_232.unknown_values[j] = 999;
            expected[index].unknown_232.unknown_count = (unsigned char)(n + 1);
            assert(SzDuel_AddGrave(index, 853) == n);
            reset_list_padding();
            assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);
        }
        memcpy(expected, data_00317490_records, sizeof(expected));
        assert(SzDuel_AddGrave(index, 1) == -1);
        assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);
    }
}

static void check_clear_compact(void)
{
    /* Occupied positions before compaction; the stored count is deliberately
     * wrong because both compactors recompute it. */
    static const int holes[][4] = {{0, 1, 2, 3}, {1, 5, 38, 39}, {0, 20, 21, 39}, {39, 39, 39, 39}};
    static const int first[] = {999, 1000, 0xffff};
    struct DotrStateRecord expected[4];
    int index, position, j, n;
    size_t h, f;
    for (index = 0; index < 4; index++) {
        struct DotrStateRecordList *list = &data_00317490_records[index].unknown_1e0;
        struct DotrStateRecordList120 *large = &data_00317490_records[index].unknown_232;
        memset(data_00317490_records, 0xa5, sizeof(data_00317490_records));
        memcpy(expected, data_00317490_records, sizeof(expected));
        for (position = 0; position < 40; position++) expected[index].unknown_1e0.unknown_values[position] = 999;
        expected[index].unknown_1e0.unknown_count = 0;
        SzDuel_InitDeck(index);
        assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);
        for (position = 0; position < 120; position++) expected[index].unknown_232.unknown_values[position] = 999;
        expected[index].unknown_232.unknown_count = 0;
        SzDuel_InitGrave(index);
        assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);

        /* Compacting an empty list keeps it empty. */
        list->unknown_count = 7;
        large->unknown_count = 7;
        SzDuel_PackDeck(index);
        SzDuel_PackGrave(index);
        reset_list_padding();
        assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);

        for (h = 0; h < sizeof(holes) / sizeof(holes[0]); h++) {
            unsigned short kept[4];
            n = 0;
            SzDuel_InitDeck(index);
            SzDuel_InitGrave(index);
            for (j = 0; j < 4; j++) {
                list->unknown_values[holes[h][j]] = (unsigned short)(100 + holes[h][j]);
                large->unknown_values[holes[h][j] * 3] = (unsigned short)(100 + holes[h][j]);
            }
            for (position = 0; position < 40; position++) {
                if (list->unknown_values[position] != 999) kept[n++] = list->unknown_values[position];
            }
            list->unknown_count = 200;
            large->unknown_count = 200;
            memcpy(expected, data_00317490_records, sizeof(expected));
            for (position = 0; position < 40; position++) {
                expected[index].unknown_1e0.unknown_values[position] = position < n ? kept[position] : 999;
            }
            for (position = 0; position < 120; position++) {
                expected[index].unknown_232.unknown_values[position] = position < n ? kept[position] : 999;
            }
            expected[index].unknown_1e0.unknown_count = (unsigned char)n;
            expected[index].unknown_232.unknown_count = (unsigned char)n;
            SzDuel_PackDeck(index);
            SzDuel_PackGrave(index);
            reset_list_padding();
            assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);
        }

        /* Full lists are unchanged apart from the recomputed count. */
        for (position = 0; position < 40; position++) list->unknown_values[position] = (unsigned short)position;
        for (position = 0; position < 120; position++) large->unknown_values[position] = (unsigned short)position;
        list->unknown_count = 0;
        large->unknown_count = 0;
        memcpy(expected, data_00317490_records, sizeof(expected));
        expected[index].unknown_1e0.unknown_count = 40;
        expected[index].unknown_232.unknown_count = 120;
        SzDuel_PackDeck(index);
        SzDuel_PackGrave(index);
        reset_list_padding();
        assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);

        /* Front removal: an empty or out-of-range first value returns 999 and
         * neither stores nor compacts (the holes below stay in place). */
        for (f = 0; f < sizeof(first) / sizeof(first[0]); f++) {
            SzDuel_InitDeck(index);
            list->unknown_values[0] = (unsigned short)first[f];
            list->unknown_values[5] = 12;
            list->unknown_count = 3;
            memcpy(expected, data_00317490_records, sizeof(expected));
            assert(SzDuel_DrawDeck(index) == 999);
            assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);
        }
        /* A valid first value is returned and removed; the compactor then
         * closes the holes and recomputes the count. */
        SzDuel_InitDeck(index);
        list->unknown_values[0] = 998;
        list->unknown_values[1] = 0;
        list->unknown_values[3] = 853;
        list->unknown_count = 3;
        memcpy(expected, data_00317490_records, sizeof(expected));
        expected[index].unknown_1e0.unknown_values[0] = 0;
        expected[index].unknown_1e0.unknown_values[1] = 853;
        expected[index].unknown_1e0.unknown_values[3] = 999;
        expected[index].unknown_1e0.unknown_count = 2;
        assert(SzDuel_DrawDeck(index) == 998);
        reset_list_padding();
        assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);
        expected[index].unknown_1e0.unknown_values[0] = 853;
        expected[index].unknown_1e0.unknown_values[1] = 999;
        expected[index].unknown_1e0.unknown_count = 1;
        assert(SzDuel_DrawDeck(index) == 0);
        reset_list_padding();
        assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);
    }
}

static void check_arithmetic(void)
{
    /* Division by zero and INT_MIN / -1 have no C semantics and are not run;
     * operands are kept small enough that no host arithmetic overflows. */
    static const int values[] = {-1000, -13, -1, 0, 1, 2, 5, 11, 12, 13, 255, 1000};
    static const int bad_operations[] = {INT_MIN, -1, 5, 6, INT_MAX};
    struct DotrStateRecord expected[4];
    int index, n, operation, r;
    size_t v, b;
    for (index = 0; index < 4; index++) {
        for (n = 0; n < 256; n += (n < 16 ? 1 : 17)) {
            for (v = 0; v < sizeof(values) / sizeof(values[0]); v++) {
                for (b = 0; b < sizeof(bad_operations) / sizeof(bad_operations[0]); b++) {
                    memset(data_00317490_records, 0x5a, sizeof(data_00317490_records));
                    data_00317490_records[index].unknown_341 = (unsigned char)n;
                    memcpy(expected, data_00317490_records, sizeof(expected));
                    assert(SzDuel_CalcSummonPower(index, bad_operations[b], values[v]) == 0);
                    assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);
                }
                for (operation = 0; operation < 5; operation++) {
                    if (operation == 3 && values[v] == 0) continue;
                    r = operation == 0 ? n + values[v] : operation == 1 ? n - values[v]
                      : operation == 2 ? n * values[v] : operation == 3 ? n / values[v] : values[v];
                    r = r < 0 ? 0 : r > 12 ? 12 : r;
                    memset(data_00317490_records, 0x5a, sizeof(data_00317490_records));
                    data_00317490_records[index].unknown_341 = (unsigned char)n;
                    memcpy(expected, data_00317490_records, sizeof(expected));
                    expected[index].unknown_341 = (unsigned char)r;
                    assert(SzDuel_CalcSummonPower(index, operation, values[v]) == 1);
                    assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);
                }
            }
        }
    }
}

#define EXPECT_SAME() assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0)

static void check_small_list(void)
{
    static const int invalid_values[] = {INT_MIN, -1, 671, 854, 999, INT_MAX};
    static const int invalid_positions[] = {INT_MIN, -1, 5, INT_MAX};
    struct DotrStateRecord expected[4];
    int index, position, other, level;
    size_t v;
    assert(offsetof(struct DotrStateRecord, unknown_324) == 0x324);
    assert(offsetof(struct DotrStateRecord, unknown_324.unknown_count) == 0x32e);
    assert(offsetof(struct DotrStateRecord, unknown_33a) == 0x33a);
    assert(sizeof(struct DotrStateRecordList5) == 12);
    for (index = 0; index < 4; index++) {
        struct DotrStateRecordList5 *list = &data_00317490_records[index].unknown_324;
        struct DotrStateRecordList5 *want = &expected[index].unknown_324;
        struct DotrStateRecordList120 *large = &data_00317490_records[index].unknown_232;

        /* 120-slot removal by position. */
        memset(data_00317490_records, 0xa5, sizeof(data_00317490_records));
        SzDuel_InitGrave(index);
        large->unknown_values[0] = 3;
        large->unknown_values[119] = 853;
        large->unknown_count = 2;
        memcpy(expected, data_00317490_records, sizeof(expected));
        assert(SzDuel_DelGrave(index, -1) == 0 && SzDuel_DelGrave(index, 120) == 0);
        assert(SzDuel_DelGrave(index, INT_MIN) == 0 && SzDuel_DelGrave(index, INT_MAX) == 0);
        assert(SzDuel_DelGrave(index, 1) == 0);
        EXPECT_SAME();
        expected[index].unknown_232.unknown_values[119] = 999;
        expected[index].unknown_232.unknown_count = 1;
        assert(SzDuel_DelGrave(index, 119) == 1 && SzDuel_DelGrave(index, 119) == 0);
        EXPECT_SAME();
        expected[index].unknown_232.unknown_values[0] = 999;
        expected[index].unknown_232.unknown_count = 0;
        assert(SzDuel_DelGrave(index, 0) == 1);
        EXPECT_SAME();
        large->unknown_values[7] = 1;
        assert(SzDuel_DelGrave(index, 7) == 1 && large->unknown_count == 255);

        /* Five-slot clear, reads and count. */
        memset(data_00317490_records, 0xa5, sizeof(data_00317490_records));
        memcpy(expected, data_00317490_records, sizeof(expected));
        for (position = 0; position < 5; position++) want->unknown_values[position] = 999;
        want->unknown_count = 0;
        SzDuel_InitHand(index);
        EXPECT_SAME();
        assert(SzDuel_GetHandCount(index) == 0);
        for (position = 0; position < 5; position++) {
            list->unknown_values[position] = (unsigned short)(0xf000 + position);
            assert(SzDuel_InqHand(index, position) == 0xf000 + position);
        }
        for (v = 0; v < sizeof(invalid_positions) / sizeof(invalid_positions[0]); v++) {
            assert(SzDuel_InqHand(index, invalid_positions[v]) == -1);
        }
        for (position = 0; position < 256; position += 51) {
            list->unknown_count = (unsigned char)position;
            assert(SzDuel_GetHandCount(index) == position);
        }

        /* Append: rejected values and a full list change nothing. */
        SzDuel_InitHand(index);
        list->unknown_values[1] = 0x2005;
        list->unknown_count = 1;
        memcpy(expected, data_00317490_records, sizeof(expected));
        for (v = 0; v < sizeof(invalid_values) / sizeof(invalid_values[0]); v++) {
            assert(SzDuel_AddHand(index, invalid_values[v]) == -1);
            EXPECT_SAME();
        }
        list->unknown_count = 5;
        memcpy(expected, data_00317490_records, sizeof(expected));
        assert(SzDuel_AddHand(index, 1) == -1);
        EXPECT_SAME();
        /* Accepted: compaction first (levels kept), then the value at the
         * recomputed count; the previous count is returned. */
        list->unknown_count = 1;
        want->unknown_values[0] = 0x2005;
        want->unknown_values[1] = 853;
        want->unknown_count = 2;
        assert(SzDuel_AddHand(index, 853) == 1);
        reset_list_padding();
        EXPECT_SAME();
        want->unknown_values[2] = 0;
        want->unknown_count = 3;
        assert(SzDuel_AddHand(index, 0) == 2);
        reset_list_padding();
        EXPECT_SAME();

        /* Compaction treats an entry as vacant by its low 12 bits and writes
         * plain 999 into the freed tail. */
        list->unknown_values[0] = 0x13e7;
        list->unknown_values[1] = 0xf001;
        list->unknown_values[2] = 999;
        list->unknown_values[3] = 0x0002;
        list->unknown_values[4] = 0x33e7;
        list->unknown_count = 77;
        memcpy(expected, data_00317490_records, sizeof(expected));
        want->unknown_values[0] = 0xf001;
        want->unknown_values[1] = 0x0002;
        want->unknown_values[2] = want->unknown_values[3] = want->unknown_values[4] = 999;
        want->unknown_count = 2;
        SzDuel_PackHand(index);
        reset_list_padding();
        EXPECT_SAME();

        /* Removal by position; the vacancy test ignores the level bits. */
        list->unknown_values[4] = 0x23e7;
        memcpy(expected, data_00317490_records, sizeof(expected));
        for (v = 0; v < sizeof(invalid_positions) / sizeof(invalid_positions[0]); v++) {
            assert(SzDuel_DelHand(index, invalid_positions[v]) == 0);
        }
        assert(SzDuel_DelHand(index, 2) == 0 && SzDuel_DelHand(index, 4) == 0);
        EXPECT_SAME();
        want->unknown_values[0] = 999;
        want->unknown_count = 1;
        assert(SzDuel_DelHand(index, 0) == 1 && SzDuel_DelHand(index, 0) == 0);
        EXPECT_SAME();

        /* Level clear keeps the low 12 bits of every entry. */
        for (position = 0; position < 5; position++) {
            list->unknown_values[position] = (unsigned short)(0xa000 + 0x111 * position);
        }
        memcpy(expected, data_00317490_records, sizeof(expected));
        for (position = 0; position < 5; position++) {
            want->unknown_values[position] = (unsigned short)(0x111 * position);
        }
        assert(SzDuel_ClearHandTag(index) == 1);
        EXPECT_SAME();

        /* Level update. */
        for (position = 0; position < 5; position++) {
            SzDuel_InitHand(index);
            list->unknown_count = 3;
            memcpy(expected, data_00317490_records, sizeof(expected));
            for (v = 0; v < sizeof(invalid_positions) / sizeof(invalid_positions[0]); v++) {
                assert(SzDuel_SetHandTag(index, invalid_positions[v], 1) == 0);
            }
            /* Vacant entry, with or without level bits: rejected. */
            assert(SzDuel_SetHandTag(index, position, 1) == 0 && SzDuel_SetHandTag(index, position, 0) == 0);
            list->unknown_values[position] = 0x33e7;
            want->unknown_values[position] = 0x33e7;
            assert(SzDuel_SetHandTag(index, position, 1) == 0 && SzDuel_SetHandTag(index, position, 0) == 0);
            EXPECT_SAME();
            /* Setting a level needs a zero level; clearing needs a nonzero one. */
            list->unknown_values[position] = want->unknown_values[position] = 0x0123;
            assert(SzDuel_SetHandTag(index, position, 0) == 0);
            EXPECT_SAME();
            for (level = 1; level < 16; level++) {
                list->unknown_values[position] = 0x0123;
                want->unknown_values[position] = (unsigned short)((level << 12) | 0x123);
                assert(SzDuel_SetHandTag(index, position, level) == 1);
                EXPECT_SAME();
                assert(SzDuel_SetHandTag(index, position, level) == 0);
                EXPECT_SAME();
            }
            /* Clearing level 2 lowers only higher levels of other occupied
             * entries; lower or equal levels and vacant entries are kept. */
            other = 0;
            for (level = 0; level < 5; level++) {
                static const unsigned short before[] = {0x1001, 0x2002, 0x3003, 0xf004, 0x53e7};
                static const unsigned short after[] = {0x1001, 0x2002, 0x2003, 0xe004, 0x53e7};
                if (level == position) continue;
                list->unknown_values[level] = before[other];
                want->unknown_values[level] = after[other];
                other++;
            }
            list->unknown_values[position] = 0x2123;
            want->unknown_values[position] = 0x0123;
            assert(SzDuel_SetHandTag(index, position, 0) == 1);
            EXPECT_SAME();
        }

        /* Halfword getter and arithmetic update at +0x33a. */
        {
            static const int stored[] = {0, 1, 12, 9998, 9999, 65535};
            static const int operands[] = {-30000, -1, 0, 1, 2, 7, 9999, 10000, 30000};
            static const int bad_operations[] = {INT_MIN, -1, 5, INT_MAX};
            size_t n, o, b;
            int operation, r;
            for (n = 0; n < sizeof(stored) / sizeof(stored[0]); n++) {
                for (o = 0; o < sizeof(operands) / sizeof(operands[0]); o++) {
                    memset(data_00317490_records, 0x5a, sizeof(data_00317490_records));
                    data_00317490_records[index].unknown_33a = (unsigned short)stored[n];
                    memcpy(expected, data_00317490_records, sizeof(expected));
                    assert(SzDuel_GetLP(index) == stored[n]);
                    for (b = 0; b < sizeof(bad_operations) / sizeof(bad_operations[0]); b++) {
                        assert(SzDuel_CalcLP(index, bad_operations[b], operands[o]) == 0);
                        EXPECT_SAME();
                    }
                    for (operation = 0; operation < 5; operation++) {
                        if (operation == 3 && operands[o] == 0) continue;
                        r = operation == 0 ? stored[n] + operands[o] : operation == 1 ? stored[n] - operands[o]
                          : operation == 2 ? stored[n] * operands[o] : operation == 3 ? stored[n] / operands[o] : operands[o];
                        r = r < 0 ? 0 : r > 9999 ? 9999 : r;
                        data_00317490_records[index].unknown_33a = (unsigned short)stored[n];
                        expected[index].unknown_33a = (unsigned short)r;
                        assert(SzDuel_CalcLP(index, operation, operands[o]) == 1);
                        EXPECT_SAME();
                    }
                }
            }
        }
    }
}

static void check_wrappers(void)
{
    int value;
    lookup_result = 0;
    assert(SzDuel_GetUnit(-7, 9) == 0 && lookup_a == -7 && lookup_b == 9);
    lookup_result = &lookup_object;
    for (value = 0; value < 256; value++) {
        lookup_object.slot = (unsigned char)value;
        assert(SzDuel_GetUnit(value, -value) == (value < 11 ? &lookup_object : 0));
        assert(lookup_a == value && lookup_b == -value);
    }
    block_calls = forward_calls = 0;
    SzDuel_AddUnit(11, -22, 33, 44, -55);
    assert(block_calls == 1 && forward_calls == 1 && block_argument == 44);
    assert(forward_args[0] == 11 && forward_args[1] == -22 && forward_args[2] == 33 && forward_args[3] == -55);
}

static void check_coordinates(void)
{
    static const int invalid[] = {INT_MIN, -1, 7, INT_MAX};
    struct DotrStateRecord expected[4];
    int index, x, z, height;
    size_t i;
    memset(data_00317490_records, 0xa5, sizeof(data_00317490_records));
    memcpy(expected, data_00317490_records, sizeof(expected));
    for (index = 0; index < 4; index++) {
        for (i = 0; i < sizeof(invalid) / sizeof(invalid[0]); i++) {
            calls = 0;
            assert(SzDuel_SetPlayerCur(index, invalid[i], 3) == 0);
            assert(SzDuel_SetPlayerCur(index, 3, invalid[i]) == 0);
            assert(calls == 0);
            assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);
        }
    }
    for (index = 0; index < 4; index++) {
        for (x = 0; x < 7; x++) {
            for (z = 0; z < 7; z++) {
                for (height = 0; height < 256; height++) {
                    unsigned char *record = (unsigned char *)&data_00317490_records[index];
                    memset(data_00317490_records, 0xa5, sizeof(data_00317490_records));
                    memcpy(record + 0x1d0, &saved_value, sizeof(saved_value));
                    record[0x345] = (unsigned char)height;
                    memcpy(expected, data_00317490_records, sizeof(expected));
                    ((unsigned char *)&expected[index])[0x33d] = (unsigned char)x;
                    ((unsigned char *)&expected[index])[0x33e] = (unsigned char)z;
                    calls = 0;
                    expected_index = index;
                    expected_x = x;
                    expected_z = z;
                    expected_y = height < 128 ? height : height - 256;
                    assert(SzDuel_SetPlayerCur(index, x, z) == 1 && calls == 3);
                    assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);
                }
            }
        }
    }
}

static void check_record_bytes(void)
{
    static const int values[] = {INT_MIN, -256, -128, -1, 0, 1, 127, 128,
                                 255, 256, 511, INT_MAX};
    struct DotrStateRecord expected[4];
    int index, old;
    size_t v;
    for (index = 0; index < 4; index++) {
        for (old = 0; old < 256; old++) {
            struct DotrStateRecord *record = &data_00317490_records[index];
            unsigned char *bytes = (unsigned char *)record;
            memset(data_00317490_records, 0xa5, sizeof(data_00317490_records));
            record->unknown_33c = (unsigned char)old;
            record->unknown_341 = (unsigned char)old;
            record->unknown_342 = (unsigned char)old;
            record->unknown_344 = (unsigned char)old;
            bytes[0x346] = (unsigned char)old;
            memcpy(expected, data_00317490_records, sizeof(expected));
            assert(SzDuel_GetPlayer(index) == old);
            assert(SzDuel_GetSummonPower(index) == old);
            assert(SzDuel_GetSummonCharge(index) == old);
            assert(SzDuel_InqDestinyFlag(index) == old);
            assert(SzDuel_InqHandPhase(index) == (old & 1));
            assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);

            for (v = 0; v < sizeof(values) / sizeof(values[0]); v++) {
                int value = values[v];
                /* Equality compares the low byte before the 0x7f mask. */
                memcpy(data_00317490_records, expected, sizeof(expected));
                expected[index].unknown_33c = old == (unsigned char)value
                    ? (unsigned char)old : (unsigned char)(value & 0x7f);
                SzDuel_SetPlayer(index, value);
                assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);
                expected[index].unknown_33c = (unsigned char)old;

                memcpy(data_00317490_records, expected, sizeof(expected));
                expected[index].unknown_342 = (unsigned char)value;
                SzDuel_SetSummonCharge(index, value);
                assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);
                expected[index].unknown_342 = (unsigned char)old;

                memcpy(data_00317490_records, expected, sizeof(expected));
                expected[index].unknown_344 = (unsigned char)value;
                SzDuel_SetDestinyFlag(index, value);
                assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);
                expected[index].unknown_344 = (unsigned char)old;

                memcpy(data_00317490_records, expected, sizeof(expected));
                ((unsigned char *)&expected[index])[0x346] =
                    (unsigned char)((old & 0xfe) | (value != 0));
                SzDuel_SetHandPhase(index, value);
                assert(memcmp(expected, data_00317490_records, sizeof(expected)) == 0);
                assert(SzDuel_InqHandPhase(index) == (value != 0));
                ((unsigned char *)&expected[index])[0x346] = (unsigned char)old;
            }
        }
    }
}

static void check_control_bytes(void)
{
    static const int values[] = {INT_MIN, -1, 0, 1, 2, 254, 255, 256, INT_MAX};
    unsigned char expected[sizeof(struct DotrStateControl)];
    unsigned char *control = (unsigned char *)data_00317490_control;
    int old;
    size_t v;
    for (old = 0; old < 256; old++) {
        turnFirst[0] = (unsigned char)old;
        turnCount[0] = (unsigned char)old;
        turnOwner[0] = (unsigned char)old;
        ((unsigned char *)data_003181f6)[0] = (unsigned char)old;
        assert(SzDuel_GetTurnOwner() == old);
        assert(SzDuel_GetTurnFirst() == old);
        assert(SzDuel_GetTurnCount() == old);
        assert(SzDuel_GetWindowDisp() == ((old >> 7) & 1));
        for (v = 0; v < sizeof(values) / sizeof(values[0]); v++) {
            int value = values[v];
            memset(control, old, sizeof(expected));
            memcpy(expected, control, sizeof(expected));
            expected[0xd53] = (unsigned char)(value < 0 ? 0 : value > 255 ? 255 : value);
            SzDuel_SetTurnCount(value);
            assert(memcmp(control, expected, sizeof(expected)) == 0);

            memset(control, old, sizeof(expected));
            memcpy(expected, control, sizeof(expected));
            expected[0xd66] = (unsigned char)((old & 0x7f) | ((value != 0) << 7));
            SzDuel_SetWindowDisp(value);
            assert(memcmp(control, expected, sizeof(expected)) == 0);

            memset(control, old, sizeof(expected));
            memcpy(expected, control, sizeof(expected));
            expected[0xd67] = (unsigned char)((old & 0xfe) | (value != 0));
            SzDuel_SetWindowDraw(value);
            assert(memcmp(control, expected, sizeof(expected)) == 0);

            memset(control, old, sizeof(expected));
            memcpy(expected, control, sizeof(expected));
            expected[0xd67] = (unsigned char)((old & 0xfd) | ((value != 0) << 1));
            SzDuel_SetWindowBusy(value);
            assert(memcmp(control, expected, sizeof(expected)) == 0);
        }
    }
}

static int random_calls, random_count, random_values[4];

/* Assumed dependency semantics: a 31-bit generator and float truncation. */
int func_0019a5c8(void)
{
    assert(random_calls < random_count);
    return random_values[random_calls++];
}

int func_001a22c0(float value)
{
    return (int)value;
}

static void check_random_insert(void)
{
    static const int invalid_values[] = {INT_MIN, -1, 671, 854, 999, INT_MAX};
    struct DotrStateRecord expected[4];
    int index, n;
    size_t v;
    for (index = 0; index < 4; index++) {
        struct DotrStateRecordList *list = &data_00317490_records[index].unknown_1e0;
        memset(data_00317490_records, 0xa5, sizeof(data_00317490_records));
        for (n = 0; n < 40; n++) {
            list->unknown_values[n] = 999;
        }
        list->unknown_values[7] = 5;
        list->unknown_count = 1;
        memcpy(expected, data_00317490_records, sizeof(expected));
        random_calls = random_count = 0;
        for (v = 0; v < sizeof(invalid_values) / sizeof(invalid_values[0]); v++) {
            assert(SzDuel_AddDeck(index, invalid_values[v]) == -1);
        }
        list->unknown_count = 40;
        assert(SzDuel_AddDeck(index, 12) == -1);
        list->unknown_count = 255;
        assert(SzDuel_AddDeck(index, 12) == -1);
        list->unknown_count = 1;
        assert(memcmp(data_00317490_records, expected, sizeof(expected)) == 0);
        /* 0.1875 * 40 selects occupied slot 7; 0.984375 * 40 selects vacant slot 39. */
        random_values[0] = 0x18000000;
        random_values[1] = 0x7e000000;
        random_count = 2;
        assert(SzDuel_AddDeck(index, 853) == 39 && random_calls == 2);
        expected[index].unknown_1e0.unknown_values[39] = 853;
        expected[index].unknown_1e0.unknown_count = 2;
        assert(memcmp(data_00317490_records, expected, sizeof(expected)) == 0);
        random_values[0] = 0;
        random_calls = 0;
        random_count = 1;
        assert(SzDuel_AddDeck(index, 0) == 0 && random_calls == 1);
        expected[index].unknown_1e0.unknown_values[0] = 0;
        expected[index].unknown_1e0.unknown_count = 3;
        assert(memcmp(data_00317490_records, expected, sizeof(expected)) == 0);
    }
}

static int level_calls, level_args[8][2], level_results[8];

int TaHand_GetCardSummonCost(int word, int index)
{
    assert(level_calls < 8);
    level_args[level_calls][0] = word;
    level_args[level_calls][1] = index;
    return level_results[level_calls++];
}

static void check_best_level(void)
{
    struct DotrStateRecordList5 *list = &data_00317490_records[3].unknown_324;
    int old = turnOwner[0];

    turnOwner[0] = 3;
    memset(list, 0, sizeof(*list));
    level_calls = 0;
    assert(Ta_GetSummonCost() == 0 && level_calls == 0); /* count 0 */

    /* Entries with a zero level nibble are skipped; the others are scored with (word, index). */
    list->unknown_count = 5;
    list->unknown_values[0] = 0x0fff;
    list->unknown_values[1] = 0x1001;
    list->unknown_values[2] = 0x3002;
    list->unknown_values[3] = 0x0003;
    list->unknown_values[4] = 0xf004;
    level_results[0] = 7; level_results[1] = 9; level_results[2] = 4;
    assert(Ta_GetSummonCost() == 9 && level_calls == 3);
    assert(level_args[0][0] == 0x1001 && level_args[1][0] == 0x3002 && level_args[2][0] == 0xf004);
    assert(level_args[0][1] == 3 && level_args[1][1] == 3 && level_args[2][1] == 3);

    /* A later larger result replaces; negative results leave 0. */
    level_calls = 0;
    level_results[0] = 2; level_results[1] = 5; level_results[2] = 11;
    assert(Ta_GetSummonCost() == 11);
    level_calls = 0;
    level_results[0] = -4; level_results[1] = -1; level_results[2] = 0;
    assert(Ta_GetSummonCost() == 0 && level_calls == 3);
    turnOwner[0] = (unsigned char)old;
}

/* Declared in state_queries.h, which this test does not include. */
int Ta_IsLDRange(int side, int col, int row);
int SzDuel_GetUnitLoc(int side, int pos);
int Ta_IsLDAbl(int a, int b);
int Ta_IsSquareRange(int a, int b, int c, int d, int e);
static int cell_word, mode_flag, test_args[5], test_result, test_calls;
int SzDuel_GetUnitLoc(int a, int b) { assert(a == 6 && b == 0); return cell_word; }
int Ta_IsLDAbl(int a, int b) { assert(a == 9 && b == 6); return mode_flag; }
int Ta_IsSquareRange(int a, int b, int c, int d, int e)
{
    int v[5] = {a, b, c, d, e};
    memcpy(test_args, v, sizeof(v));
    test_calls++;
    return test_result;
}

/* Ta_IsLDRange (branch coverage): both modes, zero and non-zero callee result,
 * and a negative cell word (arithmetic shift). */
static void check_cell_test(void)
{
    cell_word = 0x1234;
    mode_flag = 1;
    test_result = 7;
    test_calls = 0;
    assert(Ta_IsLDRange(6, 20, 30) == 1 && test_calls == 1);
    assert(test_args[0] == 0x34 && test_args[1] == 0x12 && test_args[2] == 20 && test_args[3] == 30 && test_args[4] == 5);
    test_result = 0;
    assert(Ta_IsLDRange(6, 20, 30) == 0 && test_args[4] == 5);
    mode_flag = 0;
    cell_word = -256;
    assert(Ta_IsLDRange(6, 20, 30) == 0 && test_args[4] == 3 && test_args[0] == 0 && test_args[1] == -1);
    test_result = -1;
    assert(Ta_IsLDRange(6, 20, 30) == 1 && test_calls == 4);
}

/* TaHand_GetCardEffect (branch coverage): 999 returns 0 without the call; any other
 * value fills the local block. The non-zero result is the address of an
 * automatic object, which a host compiler may replace, so only the call and
 * its arguments are checked for that outcome. */
static void check_pair_address(void)
{
    block_calls = forward_calls = 0;
    assert(TaHand_GetCardEffect(999, 3) == 0 && block_calls == 0);
    (void)TaHand_GetCardEffect(998, 3);
    assert(block_calls == 1 && block_argument == 998);
}

/* SzDuel_InitMapEdit: branch coverage, one run per outcome of the flag test. */
static void check_setup(void)
{
    unsigned char *bytes = (unsigned char *)data_00317490_records;
    int flag;
    for (flag = 0; flag < 2; flag++) {
        memset(&data_00a9c08c, 0xff, sizeof(data_00a9c08c));
        bytes[3415] = bytes[3427] = 0x55;
        setup_mode = 1;
        setup_calls = setup_sets = 0;
        SzDuel_InitMapEdit(flag);
        setup_mode = 0;
        assert(setup_calls == 3 && setup_sets == 99);
        assert(bytes[3415] == 99 && bytes[3427] == 0);
        assert(data_00a9c08c.unknown_2 == 0xff);
        assert(data_00a9c08c.unknown_1 == (flag ? 5 : 0xff));
        assert(data_00a9c08c.unknown_0 == (flag ? 3 : 0xff));
    }
}

/* TaTuto_DbgInit034 (branch coverage): filled and empty decks, occupied and empty last hand slot. */
static void check_tuto_init034(void)
{
    static const int deck0[5] = { 321, 285, 156, 249, 505 };
    static const int deck1[5] = { 21, 423, 522, 12, 433 };
    int i, pass;

    for (i = 0; i < 2; i++) {
        SzDuel_InitDeck(i);
        SzDuel_InitHand(i);
    }
    for (i = 0; i < 7; i++) {
        assert(SzDuel_SetDeck(0, i, 100 + i) == 1);
    }
    for (i = 0; i < 5; i++) {
        SzDuel_AddHand(0, 200 + i);
        SzDuel_AddHand(1, 300 + i);
    }
    assert(SzDuel_GetDeckCount(0) == 7 && SzDuel_GetDeckCount(1) == 0);
    assert(SzDuel_GetHandCount(0) == 5 && SzDuel_GetHandCount(1) == 5);
    for (pass = 0; pass < 2; pass++) {
        /* first pass: hand slot 4 of side 1 is occupied; second pass: it is already empty */
        assert(TaTuto_DbgInit034() == (pass == 0));
        assert(SzDuel_GetDeckCount(0) == 5 && SzDuel_GetDeckCount(1) == 5);
        assert(SzDuel_GetHandCount(0) == 0 && SzDuel_GetHandCount(1) == 0);
        for (i = 0; i < 5; i++) {
            assert(SzDuel_InqDeck(0, i) == deck0[i] && SzDuel_InqDeck(1, i) == deck1[i]);
        }
        assert(SzDuel_SetDeck(0, 5, 1) == 1 && SzDuel_DelDeck(0, 5) == 1);
    }
}

/* TaTuto_SetDeck035 (branch coverage): every case label, and a selector with no case (hand only packed). */
static void check_tuto_deck035(void)
{
    static const struct { unsigned short n; int hand[5]; } rows[9] = {
        { 37, { 321, 7, 156, 249, 294 } }, { 59, { 321, 7, 156, 249, 294 } }, { 208, { 321, 7, 156, 249, 294 } },
        { 72, { 15, 5, 156, 249, 294 } },  { 93, { 321, 7, 156, 249, 766 } }, { 172, { 321, 7, 156, 249, 766 } },
        { 113, { 321, 7, 156, 249, 752 } }, { 135, { 321, 7, 156, 766, 766 } }, { 152, { 15, 5, 294, 766, 766 } },
    };
    int i, j;

    for (i = 0; i < 9; i++) {
        SzDuel_InitHand(0);
        SzDuel_AddHand(0, 400);
        TaTuto_SetDeck035(rows[i].n);
        assert(SzDuel_GetHandCount(0) == 5);
        for (j = 0; j < 5; j++) {
            assert(SzDuel_InqHand(0, j) == rows[i].hand[j]);
        }
    }
    SzDuel_InitHand(0);
    SzDuel_AddHand(0, 400);
    TaTuto_SetDeck035(36);
    assert(SzDuel_GetHandCount(0) == 1 && SzDuel_InqHand(0, 0) == 400);
    TaTuto_SetDeck035(153);
    assert(SzDuel_GetHandCount(0) == 1 && SzDuel_InqHand(0, 0) == 400);
}

int main(void)
{
    check_tuto_init034();
    check_layout();
    check_record_bytes();
    check_control_bytes();
    check_list();
    check_list_updates();
    check_clear_compact();
    check_arithmetic();
    check_small_list();
    check_wrappers();
    check_coordinates();
    check_random_insert();
    check_best_level();
    check_pair_address();
    check_cell_test();
    check_setup();
    check_tuto_deck035();
    puts("state_records: ok");
    return 0;
}
