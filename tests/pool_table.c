/* Synthetic tests of the pool-table routines. Dependencies outside the group
 * are stubs; the fill routine's semantics are an assumption. */
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/pool_table.h"

struct DotrPool gSzEffect_el_pool;
struct DotrPoolList gSzEffect_aEffFuncTbl[3];

static int release_calls, fill_calls, byte_calls[3], byte_values[3];
static void *released[32];

void SzMem_Free(void *pointer)
{
    assert(release_calls < 32);
    released[release_calls++] = pointer;
}

void RhMem_FillMemory(void *destination, int value, int size)
{
    fill_calls++;
    assert(destination == (void *)&gSzEffect_el_pool && value == 0);
    assert(size == (int)sizeof(gSzEffect_el_pool));
    memset(destination, value, (size_t)size);
}

void SzEffect_SetBlackOutForMonster(int value) { byte_calls[0]++; byte_values[0] = value; }
void SzEffect_SetBlackOutForDuel(int value) { byte_calls[1]++; byte_values[1] = value; }
void SzEffect_SetBlackOutForBattle(int value) { byte_calls[2]++; byte_values[2] = value; }

static void check_status(void)
{
    int index, kind;
    memset(&gSzEffect_el_pool, 0, sizeof(gSzEffect_el_pool));
    assert(SzEffect_IsUse(-1) == 0 && SzEffect_IsUse(24) == 0);
    for (index = 0; index < 24; index++) {
        for (kind = 0; kind < 8; kind++) {
            gSzEffect_el_pool.unknown_100[index].unknown_00_bits_0_2 = (unsigned short)kind;
            assert(SzEffect_IsUse(index) == (kind == 1 ? 1 : kind == 2 ? 2 : 0));
        }
        gSzEffect_el_pool.unknown_100[index].unknown_00_bits_0_2 = 0;
    }
    /* Entry 3: kind 1, selectors (5, 700); entry 20: kind 2, selectors (5, 9). */
    gSzEffect_el_pool.unknown_100[3].unknown_00_bits_0_2 = 1;
    gSzEffect_el_pool.unknown_100[3].unknown_02_bits_0_2 = 5;
    gSzEffect_el_pool.unknown_100[3].unknown_02_bits_3_12 = 700;
    gSzEffect_el_pool.unknown_100[20].unknown_00_bits_0_2 = 2;
    gSzEffect_el_pool.unknown_100[20].unknown_02_bits_0_2 = 5;
    gSzEffect_el_pool.unknown_100[20].unknown_02_bits_3_12 = 9;
    assert(SzEffect_IsUse2(-1, -1) == 3 && SzEffect_IsUse2(5, -1) == 3);
    assert(SzEffect_IsUse2(5, 700) == 1 && SzEffect_IsUse2(5, 9) == 2 && SzEffect_IsUse2(-1, 9) == 2);
    assert(SzEffect_IsUse2(4, -1) == 0 && SzEffect_IsUse2(5, 8) == 0);
}

static void check_small_entries(void)
{
    struct DotrPoolEntry8 *e = gSzEffect_el_pool.unknown_000;
    memset(&gSzEffect_el_pool, 0, sizeof(gSzEffect_el_pool));
    /* Search: the first entry matching both selectors; a negative selector
     * matches anything. All-zero entries match (0, 0). */
    assert(SzEffect_GetModuleInfo(-1, -1) == &e[0] && SzEffect_GetModuleInfo(0, 0) == &e[0]);
    assert(SzEffect_GetModuleInfo(1, -1) == 0 && SzEffect_GetModuleInfo(-1, 1) == 0);
    e[7].unknown_bits_3_5 = 6;
    e[7].unknown_bits_6_15 = 1000;
    e[31].unknown_bits_3_5 = 6;
    e[31].unknown_bits_6_15 = 1;
    assert(SzEffect_GetModuleInfo(6, -1) == &e[7] && SzEffect_GetModuleInfo(6, 1) == &e[31]);
    assert(SzEffect_GetModuleInfo(-1, 1000) == &e[7] && SzEffect_GetModuleInfo(6, 2) == 0);

    /* Entries in state 2 move to state 3 when no 24-byte entry with the same
     * selectors is in state 1 or 2. */
    e[7].unknown_bits_0_2 = 2;
    e[31].unknown_bits_0_2 = 2;
    e[5].unknown_bits_0_2 = 1;
    gSzEffect_el_pool.unknown_100[2].unknown_00_bits_0_2 = 2;
    gSzEffect_el_pool.unknown_100[2].unknown_02_bits_0_2 = 6;
    gSzEffect_el_pool.unknown_100[2].unknown_02_bits_3_12 = 1000;
    SzEffect_TrimModule();
    assert(e[7].unknown_bits_0_2 == 2 && e[31].unknown_bits_0_2 == 3 && e[5].unknown_bits_0_2 == 1);
    assert(e[31].unknown_bits_3_5 == 6 && e[31].unknown_bits_6_15 == 1);
}

static void check_reset(void)
{
    static int marker_a, marker_b;
    static int list_a[] = {5, 6, 7, 0}, list_b[] = {0}, list_c[] = {1, 0};
    struct DotrPoolEntry8 *e = gSzEffect_el_pool.unknown_000;
    int i;
    memset(&gSzEffect_el_pool, 0xff, sizeof(gSzEffect_el_pool));
    for (i = 0; i < 32; i++) e[i].unknown_04 = 0;
    e[4].unknown_04 = &marker_a;
    e[30].unknown_04 = &marker_b;
    release_calls = 0;
    SzEffect_Clear();
    assert(release_calls == 2 && released[0] == (void *)&marker_a && released[1] == (void *)&marker_b);
    for (i = 0; i < 32; i++) {
        /* Only the three state bits and the pointer are cleared. */
        assert(e[i].unknown_bits_0_2 == 0 && e[i].unknown_04 == 0);
        assert(e[i].unknown_bits_3_5 == 7 && e[i].unknown_bits_6_15 == 0x3ff);
    }
    for (i = 0; i < 24; i++) {
        assert(gSzEffect_el_pool.unknown_100[i].unknown_00_bits_0_2 == 0);
        assert(gSzEffect_el_pool.unknown_100[i].unknown_00_bits_3_15 == 0x1fff);
        assert(gSzEffect_el_pool.unknown_100[i].unknown_10 == -1);
    }
    assert(gSzEffect_el_pool.unknown_340 == -1 && gSzEffect_el_pool.unknown_344 == -1);
    assert(gSzEffect_el_pool.unknown_348 == 0 && gSzEffect_el_pool.unknown_34c == 0);
    assert(gSzEffect_el_pool.unknown_350 == -1);

    /* Full initializer: counts the three zero-terminated lists, fills the
     * pool, resets it and sets the three bytes to 128. */
    gSzEffect_aEffFuncTbl[0].unknown_list = list_a;
    gSzEffect_aEffFuncTbl[1].unknown_list = list_b;
    gSzEffect_aEffFuncTbl[2].unknown_list = list_c;
    for (i = 0; i < 3; i++) {
        gSzEffect_aEffFuncTbl[i].unknown_04 = 0x55;
        gSzEffect_aEffFuncTbl[i].unknown_count = 99;
    }
    fill_calls = release_calls = 0;
    memset(byte_calls, 0, sizeof(byte_calls));
    SzEffect_Init();
    assert(gSzEffect_aEffFuncTbl[0].unknown_count == 3 && gSzEffect_aEffFuncTbl[1].unknown_count == 0);
    assert(gSzEffect_aEffFuncTbl[2].unknown_count == 1 && gSzEffect_aEffFuncTbl[1].unknown_04 == 0x55);
    assert(fill_calls == 1 && release_calls == 0);
    assert(gSzEffect_el_pool.unknown_340 == -1 && gSzEffect_el_pool.unknown_344 == -1);
    assert(gSzEffect_el_pool.unknown_348 == 0 && gSzEffect_el_pool.unknown_350 == 0);
    for (i = 0; i < 3; i++) assert(byte_calls[i] == 1 && byte_values[i] == 128);
}

int *gSzView_hView[1];
static int block_storage[560], allocate_argument, allocate_fail, follow_calls, follow_index, clear_calls;

void *SzMem_Alloc(int byte_count)
{
    allocate_argument = byte_count;
    return allocate_fail ? 0 : block_storage;
}

int *RhMem_FillMemory4(int *destination, int value, int count)
{
    clear_calls++;
    assert(value == 0 && count == 560);
    return destination;
}

void lSzView_InitVars(int index)
{
    follow_calls++;
    follow_index = index;
}

static void check_entry_flag_and_block(void)
{
    struct DotrPool expected;
    int index, state, flag;
    for (index = -1; index <= 24; index++) {
        for (state = 0; state < 8; state++) {
            for (flag = 0; flag < 3; flag++) {
                int valid = index >= 0 && index < 24 && (flag ? state == 1 : state == 2);
                memset(&gSzEffect_el_pool, 0, sizeof(gSzEffect_el_pool));
                if (index >= 0 && index < 24) {
                    gSzEffect_el_pool.unknown_100[index].unknown_00_bits_0_2 = (unsigned short)state;
                    gSzEffect_el_pool.unknown_100[index].unknown_02_bit_13 = flag ? 0 : -1;
                }
                expected = gSzEffect_el_pool;
                assert(SzEffect_Sleep(index, flag) == valid);
                if (valid) {
                    expected.unknown_100[index].unknown_00_bits_0_2 = 1;
                    expected.unknown_100[index].unknown_02_bit_13 = (flag & 1) ? -1 : 0;
                }
                assert(memcmp(&gSzEffect_el_pool, &expected, sizeof(expected)) == 0);
            }
        }
    }

    /* 0x1f7720: only index 0 is accepted. */
    release_calls = 0;
    gSzView_hView[0] = 0;
    assert(SzView_Destroy(-1) == 0 && SzView_Destroy(1) == 0 && SzView_Destroy(0) == 0 && release_calls == 0);
    gSzView_hView[0] = block_storage;
    assert(SzView_Destroy(0) == 1 && release_calls == 1 && released[0] == block_storage);
    assert(gSzView_hView[0] == 0);

    /* 0x1f74c0 */
    follow_calls = clear_calls = allocate_argument = 0;
    assert(SzView_Create(-1) == 0 && SzView_Create(1) == 0 && allocate_argument == 0);
    allocate_fail = 1;
    assert(SzView_Create(0) == 0 && allocate_argument == 2240 && gSzView_hView[0] == 0 && clear_calls == 0);
    allocate_fail = 0;
    assert(SzView_Create(0) == 1 && gSzView_hView[0] == block_storage);
    assert(clear_calls == 1 && follow_calls == 1 && follow_index == 0);
}

static void check_filtered_flag(void)
{
    struct DotrPoolEntry24 *e = gSzEffect_el_pool.unknown_100;
    int i;
    memset(&gSzEffect_el_pool, 0, sizeof(gSzEffect_el_pool));
    /* No entry is in state 1 or 2: nothing matches. */
    assert(SzEffect_Sleep2(-1, -1, 1) == 0);
    for (i = 0; i < 24; i++) {
        e[i].unknown_00_bits_0_2 = 1;
        e[i].unknown_02_bits_0_2 = i % 3;
        e[i].unknown_02_bits_3_12 = i % 5;
    }
    /* a<0, b<0: every entry is forwarded; flag 1 moves state-1 entries. */
    assert(SzEffect_Sleep2(-1, -1, 1) == 1);
    for (i = 0; i < 24; i++) {
        assert(e[i].unknown_02_bit_13 == -1);
        e[i].unknown_00_bits_0_2 = 2;
        e[i].unknown_02_bit_13 = -1;
    }
    /* a filter only: entries 0,3,... selected, flag 0 clears bit 13. */
    assert(SzEffect_Sleep2(0, -1, 0) == 1);
    for (i = 0; i < 24; i++) {
        assert(e[i].unknown_00_bits_0_2 == (i % 3 == 0 ? 1 : 2));
        assert(e[i].unknown_02_bit_13 == (i % 3 == 0 ? 0 : -1));
    }
    /* b filter only; selects i%5==4 among remaining state-2 entries. */
    assert(SzEffect_Sleep2(-1, 4, 0) == 1);
    assert(e[4].unknown_00_bits_0_2 == 1 && e[9].unknown_00_bits_0_2 == 1);
    /* both filters: no entry has a=2,b=3 in state 2 except i=8 (8%3=2, 8%5=3). */
    assert(e[8].unknown_00_bits_0_2 == 2);
    assert(SzEffect_Sleep2(2, 3, 0) == 1 && e[8].unknown_00_bits_0_2 == 1);
    /* No matching filter: result 0. */
    assert(SzEffect_Sleep2(7, -1, 0) == 0 && SzEffect_Sleep2(-1, 31, 0) == 0);
}

static int cue_back, cue_calls, cue_values[80];

int func_001a8760_int(void) { return cue_back; }
int func_00217710_cue(int index, int value)
{
    assert(index == 1 && cue_calls < 80);
    cue_values[cue_calls++] = value;
    return 1;
}

/* SzEffect_ReqSE (branch and loop coverage). */
static void check_cues(void)
{
    static struct DotrPoolCue cues[70];
    struct DotrPoolEntry8 *e = gSzEffect_el_pool.unknown_000;
    struct DotrPoolEntry24 *t = gSzEffect_el_pool.unknown_100;
    int i;

    memset(&gSzEffect_el_pool, 0, sizeof(gSzEffect_el_pool));
    memset(cues, 0, sizeof(cues));
    /* times 96..104, then a tag of 15 */
    for (i = 0; i < 9; i++) {
        cues[i].tag = i;
        cues[i].time = 96 + i;
        cues[i].value = 500 + i;
    }
    cues[9].tag = 15;
    cues[9].time = 100;
    cues[10].time = 100;
    e[4].unknown_bits_6_15 = 77;
    e[4].unknown_bits_0_2 = 2;
    e[4].unknown_04 = cues;
    t[23].unknown_00_bits_0_2 = 1;
    t[23].unknown_00_bits_3_15 = 103;
    t[23].unknown_02_bits_3_12 = 77;
    cue_back = 5;
    /* the window is 98 <= time < 103; the walk stops at the tag */
    cue_calls = 0;
    SzEffect_ReqSE();
    assert(cue_calls == 5 && cue_values[0] == 502 && cue_values[4] == 506);
    /* an empty window */
    cue_back = 0;
    cue_calls = 0;
    SzEffect_ReqSE();
    assert(cue_calls == 0);
    cue_back = 5;
    /* the 24-byte entry: state other than 1, or a +2 selector other than 0 */
    t[23].unknown_00_bits_0_2 = 2;
    SzEffect_ReqSE();
    assert(cue_calls == 0);
    t[23].unknown_00_bits_0_2 = 1;
    t[23].unknown_02_bits_0_2 = 1;
    SzEffect_ReqSE();
    assert(cue_calls == 0);
    t[23].unknown_02_bits_0_2 = 0;
    /* the 8-byte entry: no id match, selector not 0, state not 2, null pointer */
    e[4].unknown_bits_6_15 = 78;
    SzEffect_ReqSE();
    assert(cue_calls == 0);
    e[4].unknown_bits_6_15 = 77;
    e[4].unknown_bits_3_5 = 1;
    SzEffect_ReqSE();
    assert(cue_calls == 0);
    e[4].unknown_bits_3_5 = 0;
    e[4].unknown_bits_0_2 = 3;
    SzEffect_ReqSE();
    assert(cue_calls == 0);
    e[4].unknown_bits_0_2 = 2;
    e[4].unknown_04 = 0;
    SzEffect_ReqSE();
    assert(cue_calls == 0);
    e[4].unknown_04 = cues;
    /* the first matching 8-byte entry is the one used, also when it is not in state 2 */
    e[2].unknown_bits_6_15 = 77;
    SzEffect_ReqSE();
    assert(cue_calls == 0);
    e[2].unknown_bits_6_15 = 0;
    /* without a tag the walk ends after 64 records; two entries are both served */
    for (i = 0; i < 70; i++) {
        cues[i].tag = 0;
        cues[i].time = 100;
        cues[i].value = i;
    }
    t[0].unknown_00_bits_0_2 = 1;
    t[0].unknown_00_bits_3_15 = 101;
    t[0].unknown_02_bits_3_12 = 77;
    cue_back = 1;
    t[23].unknown_00_bits_3_15 = 100;
    SzEffect_ReqSE();
    assert(cue_calls == 64 && cue_values[0] == 0 && cue_values[63] == 63);
    t[23].unknown_00_bits_3_15 = 101;
    cue_calls = 0;
    cues[1].tag = 15;
    SzEffect_ReqSE();
    assert(cue_calls == 2 && cue_values[0] == 0 && cue_values[1] == 0);
    memset(&gSzEffect_el_pool, 0, sizeof(gSzEffect_el_pool));
}

/* 0x1f56d0 stub and the entry callback for 0x1f4cb0. */
static int packed_calls, packed_args[2][2], hook_calls, hook_arg, hook_state;

int SzEffect_ClearRefID2(int packed, int value)
{
    assert(packed_calls < 2);
    packed_args[packed_calls][0] = packed;
    packed_args[packed_calls][1] = value;
    packed_calls++;
    return 0;
}

static void hook(int value)
{
    hook_calls++;
    hook_arg = value;
    /* the entry's state is already cleared when the callback runs */
    hook_state = gSzEffect_el_pool.unknown_100[3].unknown_00_bits_0_2;
}

static int drop(int selector, int small_state, int with_hook, int other_state)
{
    struct DotrPoolEntry24 *q = &gSzEffect_el_pool.unknown_100[3];
    struct DotrPoolEntry8 *e = &gSzEffect_el_pool.unknown_000[5];

    memset(&gSzEffect_el_pool, 0, sizeof(gSzEffect_el_pool));
    q->unknown_00_bits_0_2 = 1;
    q->unknown_02_bits_0_2 = (unsigned short)selector;
    q->unknown_02_bits_3_12 = 700;
    q->unknown_04 = 0x444;
    q->unknown_06 = 0x666;
    q->unknown_08 = with_hook ? hook : 0;
    q->unknown_0c = 0x77;
    /* a second 24-byte entry with the same selector and id */
    gSzEffect_el_pool.unknown_100[7].unknown_00_bits_0_2 = (unsigned short)other_state;
    gSzEffect_el_pool.unknown_100[7].unknown_02_bits_0_2 = (unsigned short)selector;
    gSzEffect_el_pool.unknown_100[7].unknown_02_bits_3_12 = 700;
    /* small entries: 4 has another id, 5 matches (state 7 = leave every small entry unmatched) */
    gSzEffect_el_pool.unknown_000[4].unknown_bits_3_5 = (unsigned short)selector;
    gSzEffect_el_pool.unknown_000[4].unknown_bits_6_15 = 699;
    gSzEffect_el_pool.unknown_000[4].unknown_bits_0_2 = 2;
    if (small_state != 7) {
        e->unknown_bits_3_5 = (unsigned short)selector;
        e->unknown_bits_6_15 = 700;
        e->unknown_bits_0_2 = (unsigned short)small_state;
    }
    packed_calls = hook_calls = 0;
    hook_state = -1;
    return SzEffect_Destroy(3);
}

/* SzEffect_Destroy (branch coverage: both index bounds, selector 1 / 2 / other,
 * callback set and null, small entry missing / not in state 2 / in state 2
 * with 0x1f5240 answering 0 and not 0). 0x1f5240 is the real function. */
static void check_drop(void)
{
    struct DotrPoolEntry24 *q = &gSzEffect_el_pool.unknown_100[3];
    struct DotrPoolEntry8 *e = &gSzEffect_el_pool.unknown_000[5];

    memset(&gSzEffect_el_pool, 0, sizeof(gSzEffect_el_pool));
    packed_calls = 0;
    assert(SzEffect_Destroy(-1) == 0 && SzEffect_Destroy(24) == 0 && packed_calls == 0);

    /* selector 1: both halfwords forwarded, state cleared, callback, small entry 2 -> 3 */
    assert(drop(1, 2, 1, 0) == 1);
    assert(packed_calls == 2 && packed_args[0][0] == 0x666 && packed_args[0][1] == 1);
    assert(packed_args[1][0] == 0x444 && packed_args[1][1] == 1);
    assert(q->unknown_00_bits_0_2 == 0 && hook_calls == 1 && hook_arg == 0x77 && hook_state == 0);
    assert(e->unknown_bits_0_2 == 3 && gSzEffect_el_pool.unknown_000[4].unknown_bits_0_2 == 2);
    /* selector 2 as well; a live second entry keeps the small entry in state 2 */
    assert(drop(2, 2, 1, 1) == 1 && packed_calls == 2 && packed_args[1][1] == 2);
    assert(e->unknown_bits_0_2 == 2);
    assert(drop(2, 2, 1, 2) == 1 && e->unknown_bits_0_2 == 2);
    /* a second entry in another state does not count */
    assert(drop(2, 2, 1, 3) == 1 && e->unknown_bits_0_2 == 3);
    /* other selectors (0 and 3): no 0x1f56d0 call; null callback */
    assert(drop(3, 2, 0, 0) == 1 && packed_calls == 0 && hook_calls == 0);
    assert(q->unknown_00_bits_0_2 == 0 && e->unknown_bits_0_2 == 3);
    assert(drop(0, 1, 1, 0) == 1 && packed_calls == 0 && hook_calls == 1);
    /* small entry not in state 2: untouched; no small entry: 1 */
    assert(e->unknown_bits_0_2 == 1);
    assert(drop(1, 3, 1, 0) == 1 && e->unknown_bits_0_2 == 3);
    assert(drop(1, 7, 1, 0) == 1 && e->unknown_bits_0_2 == 0 && q->unknown_00_bits_0_2 == 0);
    assert(gSzEffect_el_pool.unknown_000[4].unknown_bits_0_2 == 2);
}

/* SzEffect_Erase2 (branch / loop coverage: a and b negative and non-negative,
 * matching and not; states 0, 1, 2 and 3; first and last entry). 0x1f4cb0 is
 * the real function. */
static void check_stop(void)
{
    struct DotrPoolEntry24 *q = gSzEffect_el_pool.unknown_100;
    int i;

    memset(&gSzEffect_el_pool, 0, sizeof(gSzEffect_el_pool));
    /* nothing live: 0 for every filter */
    assert(SzEffect_Erase2(-1, -1) == 0 && SzEffect_Erase2(0, 0) == 0);
    for (i = 0; i < 24; i++) {
        q[i].unknown_02_bits_0_2 = 3;
        q[i].unknown_02_bits_3_12 = (unsigned short)(100 + i);
        q[i].unknown_02_bit_13 = -1;
    }
    q[0].unknown_00_bits_0_2 = 1;
    q[5].unknown_00_bits_0_2 = 3;
    q[9].unknown_00_bits_0_2 = 1;
    q[9].unknown_02_bits_0_2 = 4;
    q[23].unknown_00_bits_0_2 = 2;
    /* selector and id both given: other state answers 0 and changes nothing */
    assert(SzEffect_Erase2(3, 105) == 0 && q[5].unknown_00_bits_0_2 == 3 && q[5].unknown_02_bit_14 == 0);
    /* selector mismatch, id mismatch */
    assert(SzEffect_Erase2(4, 100) == 0 && SzEffect_Erase2(3, 109) == 0);
    assert(q[0].unknown_02_bit_14 == 0 && q[9].unknown_02_bit_14 == 0);
    /* state 1, first entry: the bit is set, neighbours of the bit kept */
    assert(SzEffect_Erase2(3, 100) == 1 && q[0].unknown_02_bit_14 == -1);
    assert(q[0].unknown_00_bits_0_2 == 1 && q[0].unknown_02_bit_13 == -1 && q[0].unknown_02_bit_15 == 0);
    assert(q[0].unknown_02_bits_3_12 == 100 && q[9].unknown_02_bit_14 == 0);
    /* any selector with an id */
    assert(SzEffect_Erase2(-1, 109) == 1 && q[9].unknown_02_bit_14 == -1);
    q[9].unknown_02_bit_14 = 0;
    /* state 2, last entry: the real 0x1f4cb0 clears the state */
    assert(SzEffect_Erase2(3, -1) == 1 && q[23].unknown_00_bits_0_2 == 0);
    assert(q[9].unknown_02_bit_14 == 0 && q[5].unknown_00_bits_0_2 == 3);
    /* both negative: every entry is visited */
    q[23].unknown_00_bits_0_2 = 2;
    q[0].unknown_02_bit_14 = 0;
    assert(SzEffect_Erase2(-1, -1) == 1);
    assert(q[0].unknown_02_bit_14 == -1 && q[9].unknown_02_bit_14 == -1 && q[23].unknown_00_bits_0_2 == 0);
    assert(q[1].unknown_02_bit_14 == 0 && q[23].unknown_02_bit_14 == 0);
}

static int convert_calls;
static float convert_seen[2];

int func_001a22c0(float value)
{
    convert_seen[convert_calls++ & 1] = value;
    return (int)value;
}

/* 0x1fafb0: one assertion per early exit (index below and above 0, no block, no
 * out) and the filled case with both conversions and all four stored words. */
static void check_block_angles(void)
{
    static float floats[3];
    int *saved = gSzView_hView[0];
    int out[4] = { 9, 9, 9, 9 };
    float pi = 3.1415927f;

    floats[1] = 0.5f; floats[2] = -0.25f;
    gSzView_hView[0] = (int *)floats;
    convert_calls = 0;
    assert(SzView_GetLookVector(-1, out) == 0 && SzView_GetLookVector(1, out) == 0);
    assert(SzView_GetLookVector(0, 0) == 0);
    gSzView_hView[0] = 0;
    assert(SzView_GetLookVector(0, out) == 0);
    assert(convert_calls == 0 && out[0] == 9 && out[1] == 9 && out[2] == 9 && out[3] == 9);
    gSzView_hView[0] = (int *)floats;
    assert(SzView_GetLookVector(0, out) == 1 && convert_calls == 2);
    assert(convert_seen[0] == 32768.0f * -0.25f / pi && convert_seen[1] == 32768.0f * 0.5f / pi);
    assert(out[0] == -(int)convert_seen[0] && out[1] == 16384 - (int)convert_seen[1]);
    assert(out[2] == 0 && out[3] == 0);
    gSzView_hView[0] = saved;
}

static int matrix_calls, matrix_angles[4], matrix_first[4];
static float *matrix_out, *matrix_first_out;

void RhGeom_GetRotXYZMatrix(float *m, int *ang)
{
    if (matrix_calls == 0) {
        matrix_first_out = m;
        memcpy(matrix_first, ang, sizeof(matrix_first));
    }
    matrix_calls++;
    matrix_out = m;
    memcpy(matrix_angles, ang, sizeof(matrix_angles));
}

/* 0x1fb0b0: each early exit (no m, index below and above 0, no block) and the
 * forwarding case with the four words handed on. */
static void check_block_matrix(void)
{
    static float floats[3];
    int *saved = gSzView_hView[0];
    float m[16], pi = 3.1415927f;

    floats[1] = 1.5f; floats[2] = 0.75f;
    gSzView_hView[0] = (int *)floats;
    matrix_calls = convert_calls = 0;
    assert(SzView_GetLookMatrix(0, 0) == 0 && SzView_GetLookMatrix(-1, m) == 0 && SzView_GetLookMatrix(1, m) == 0);
    gSzView_hView[0] = 0;
    assert(SzView_GetLookMatrix(0, m) == 0 && matrix_calls == 0 && convert_calls == 0);
    gSzView_hView[0] = (int *)floats;
    assert(SzView_GetLookMatrix(0, m) == 1 && matrix_calls == 1 && convert_calls == 2 && matrix_out == m);
    assert(matrix_angles[0] == -(int)(32768.0f * 0.75f / pi));
    assert(matrix_angles[1] == 16384 - (int)(32768.0f * 1.5f / pi));
    assert(matrix_angles[2] == 0 && matrix_angles[3] == 0);
    gSzView_hView[0] = saved;
}

struct DotrPoolAngles data_00293060 = { { 11, 22, 33, 44 } };
static int join_calls;
static float *join_m, *join_other;

void RhGeom_MulMatrix(float *m, float *other)
{
    join_calls++;
    join_m = m;
    join_other = other;
}

/* 0x1fb1d0 branch coverage: each early exit and the full case with all three
 * calls. The third word of the local copy is only observable in the full case. */
static void check_block_turn(void)
{
    static float floats[17];
    int *saved = gSzView_hView[0];
    float m[16], pi = 3.1415927f;

    floats[1] = 1.5f; floats[2] = 0.75f;
    ((int *)floats)[16] = 700;
    gSzView_hView[0] = (int *)floats;
    matrix_calls = convert_calls = join_calls = 0;
    assert(SzView_GetLookMatrixWithRoll(0, 0) == 0 && SzView_GetLookMatrixWithRoll(-1, m) == 0 && SzView_GetLookMatrixWithRoll(1, m) == 0);
    gSzView_hView[0] = 0;
    assert(SzView_GetLookMatrixWithRoll(0, m) == 0 && matrix_calls == 0 && convert_calls == 0 && join_calls == 0);
    gSzView_hView[0] = (int *)floats;
    assert(SzView_GetLookMatrixWithRoll(0, m) == 1 && matrix_calls == 2 && convert_calls == 2 && join_calls == 1);
    assert(matrix_first_out == m && matrix_first[0] == -(int)(32768.0f * 0.75f / pi));
    assert(matrix_first[1] == 16384 - (int)(32768.0f * 1.5f / pi));
    assert(matrix_first[2] == 0 && matrix_first[3] == 0);
    assert(matrix_out != m && join_m == m && join_other == matrix_out);
    assert(matrix_angles[0] == 11 && matrix_angles[1] == 22 && matrix_angles[2] == -700 && matrix_angles[3] == 44);
    assert(data_00293060.v[2] == 33);
    gSzView_hView[0] = saved;
}

/* Branch coverage: state 1 and another state, completed and not completed. */
static void check_load_sectl_callback(void)
{
    struct DotrPoolLoadRecord record = { { 0, 0, 0, 0, 0 }, 0x1234, 0, 31 };
    struct DotrPoolEntryState *entry =
        (struct DotrPoolEntryState *)&gSzEffect_el_pool.unknown_000[31];
    unsigned char *raw = (unsigned char *)entry;

    memset(entry, 0, sizeof(*entry));
    raw[0] = 0xf9;
    entry->unknown_04 = 9;
    lSzEffect_LoadSECtlCB(&record, 0);
    assert(raw[0] == 0xfb && entry->unknown_04 == 9);
    lSzEffect_LoadSECtlCB(&record, 1);
    assert(raw[0] == 0xfb && entry->unknown_04 == 0x1234);
    raw[0] = 0x02;
    record.unknown_14 = 0x55;
    lSzEffect_LoadSECtlCB(&record, 1);
    assert(raw[0] == 0x02 && entry->unknown_04 == 0x55);
    memset(entry, 0, sizeof(*entry));
}

char data_002cfbc6[] = "env";
char data_002cfbcb[] = "tag";
char data_002cfbcf[] = "fmt";
static int compare_result, compare_calls, format_calls, request_calls;
static void *request_result;
static char *format_destination;

int func_0019c100(const char *a, const char *b, int count)
{
    compare_calls++;
    assert(a != 0 && b == data_002cfbcb && count == 3);
    return compare_result;
}

char *SzFile_GetEnv(const char *label)
{
    assert(label == data_002cfbc6);
    return data_002cfbc6 + 1;
}

int func_0019b648(char *destination, const char *format, ...)
{
    va_list args;

    va_start(args, format);
    assert(format == data_002cfbcf && va_arg(args, char *) == data_002cfbc6 + 1);
    va_end(args);
    format_calls++;
    format_destination = destination;
    return 0;
}

void *SzFile_ReadRequestParam(int type, int priority, char *name, int unknown_014, int byte_size, int byte_offset,
                              int unknown_010, void (*callback)(struct DotrPoolLoadRecord *record, int completed),
                              int unknown_01c)
{
    request_calls++;
    assert(type == 3072 && priority == 0 && name == format_destination && unknown_014 == 0);
    assert(byte_size == 2048 && byte_offset == (0x155 << 11) && unknown_010 == 2048);
    assert(callback == lSzEffect_LoadSECtlCB && unknown_01c == 30);
    return request_result;
}

/* Branch coverage: not completed; a failed comparison with and without the release
 * flag; a state other than 1; a non-zero selector; the request answering a record
 * and answering null. */
static void check_load_overlay_callback(void)
{
    static char loaded[4];
    struct DotrPoolOverlayRecord record;
    struct DotrPoolEntry8 *entry = &gSzEffect_el_pool.unknown_000[30];
    unsigned char *raw = (unsigned char *)entry;

    memset(&record, 0, sizeof(record));
    memset(entry, 0, sizeof(*entry));
    record.byte_size = 0x1234;
    record.unknown_14 = loaded;
    record.unknown_1c = 30;
    entry->unknown_bits_6_15 = 0x155;
    release_calls = compare_calls = format_calls = request_calls = 0;

    entry->unknown_bits_0_2 = 1;
    lSzEffect_LoadOverlayCB(&record, 0);
    assert(entry->unknown_bits_0_2 == 0 && compare_calls == 0 && release_calls == 0);
    assert(raw[2] == 0 && raw[3] == 0 && entry->unknown_bits_6_15 == 0x155);

    entry->unknown_bits_0_2 = 1;
    compare_result = 1;
    record.flag_00c = -1;
    lSzEffect_LoadOverlayCB(&record, 1);
    assert(entry->unknown_bits_0_2 == 0 && compare_calls == 1);
    assert(release_calls == 1 && released[0] == (void *)loaded);

    entry->unknown_bits_0_2 = 1;
    record.flag_00c = 0;
    lSzEffect_LoadOverlayCB(&record, 1);
    assert(entry->unknown_bits_0_2 == 0 && compare_calls == 2 && release_calls == 1);

    compare_result = 0;
    entry->unknown_bits_0_2 = 2;
    lSzEffect_LoadOverlayCB(&record, 1);
    assert(entry->unknown_bits_0_2 == 0 && raw[2] == 0 && raw[3] == 0);

    entry->unknown_bits_0_2 = 1;
    entry->unknown_bits_3_5 = 4;
    lSzEffect_LoadOverlayCB(&record, 1);
    assert(entry->unknown_bits_0_2 == 3 && entry->unknown_bits_3_5 == 4);
    assert(*(short *)entry->unknown_02 == 0x1234 && request_calls == 0 && format_calls == 0);

    entry->unknown_bits_0_2 = 1;
    entry->unknown_bits_3_5 = 0;
    request_result = loaded;
    lSzEffect_LoadOverlayCB(&record, 1);
    assert(entry->unknown_bits_0_2 == 1 && request_calls == 1 && format_calls == 1);

    request_result = 0;
    lSzEffect_LoadOverlayCB(&record, 1);
    assert(entry->unknown_bits_0_2 == 3 && request_calls == 2 && format_calls == 2);
    assert(release_calls == 1 && entry->unknown_bits_6_15 == 0x155);
    memset(entry, 0, sizeof(*entry));
    release_calls = 0;
}

/* 0x1f4900 stubs: every callee outside the group records its arguments. */
int dotr_pool_overlay[8];
static int ec_log[16], ec_n, ec_model_args[2], ec_overlay_arg, ec_load_args[3], ec_copy_count, ec_init_size;
static int ec_target, ec_from, ec_fn_arg, ec_fn_result, ec_fn_word, ec_hook_calls, ec_hook_arg;
static void *ec_copy_dst, *ec_copy_src, *ec_init_address, *ec_push_arg;
static DotrPoolAddr ec_fn_address, ec_overlay_b, ec_overlay_m;
static struct DotrPoolEntry8 *ec_load_result;

int SzDuel_GetUnitModelID(int a, int b) { ec_log[ec_n++] = 1; ec_model_args[0] = a; ec_model_args[1] = b; return 77; }
int SzModel_GetOverlayB(int index) { ec_log[ec_n++] = 2; ec_overlay_arg = index; return (int)ec_overlay_b; }
int SzModel_GetOverlayM(int index) { ec_log[ec_n++] = 3; ec_overlay_arg = index; return (int)ec_overlay_m; }
struct DotrPoolEntry8 *SzEffect_LoadModule(int a, int b, int c)
{
    ec_log[ec_n++] = 4;
    ec_load_args[0] = a; ec_load_args[1] = b; ec_load_args[2] = c;
    return ec_load_result;
}
void *RhMem_CopyMemory16(void *destination, void *source, int count)
{
    ec_log[ec_n++] = 5;
    ec_copy_dst = destination; ec_copy_src = source; ec_copy_count = count;
    return destination;
}
void func_0017bb90(int unknown) { assert(unknown == 0); ec_log[ec_n++] = 6; }
void mwOverlayInit(void *address, int initialized_size) { ec_log[ec_n++] = 7; ec_init_address = address; ec_init_size = initialized_size; }
void SzEffect_SetTarget(int value) { ec_log[ec_n++] = 8; ec_target = value; }
void SzEffect_SetFrom(int value) { ec_log[ec_n++] = 9; ec_from = value; }
void RhGeom_PushMatrix(float *matrix) { ec_log[ec_n++] = 10; ec_push_arg = matrix; }
void RhGeom_PopMatrix(void) { ec_log[ec_n++] = 12; }
static int ec_effect(DotrPoolAddr address, int arg)
{
    ec_log[ec_n++] = 11;
    ec_fn_address = address; ec_fn_arg = arg;
    return ec_fn_result;
}
int (*dotr_pool_effect_function(int word))(DotrPoolAddr, int) { ec_fn_word = word; return ec_effect; }
static void ec_hook(int value) { ec_hook_calls++; ec_hook_arg = value; }

static void ec_reset(int fill_state)
{
    static int list0[3] = { 100, 101, 102 }, list1[2] = { 200, 201 }, list2[2] = { 300, 301 };
    int i;
    memset(&gSzEffect_el_pool, 0, sizeof(gSzEffect_el_pool));
    for (i = 0; i < 24; i++) {
        gSzEffect_el_pool.unknown_100[i].unknown_00_bits_0_2 = (unsigned short)fill_state;
    }
    gSzEffect_el_pool.unknown_340 = -1;
    gSzEffect_el_pool.unknown_344 = -1;
    gSzEffect_aEffFuncTbl[0].unknown_list = list0; gSzEffect_aEffFuncTbl[0].unknown_count = 3;
    gSzEffect_aEffFuncTbl[1].unknown_list = list1; gSzEffect_aEffFuncTbl[1].unknown_count = 2;
    gSzEffect_aEffFuncTbl[2].unknown_list = list2; gSzEffect_aEffFuncTbl[2].unknown_count = 2;
    dotr_pool_overlay[7] = 0x1e00000 + 0x4321;
    ec_n = 0; ec_fn_result = 0; ec_hook_calls = 0; ec_fn_word = 0; packed_calls = 0;
    ec_overlay_b = 0; ec_overlay_m = 0;
    ec_load_result = &gSzEffect_el_pool.unknown_000[2];
}

static void check_entry_callback(void)
{
    static const int full[] = { 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    struct DotrPoolEntry24 *q;
    int i;

    /* type and id bounds: nothing is called */
    ec_reset(0);
    assert(SzEffect_EntryCallback(-1, 0, 0, 0, 0, 0, 0) == -1);
    assert(SzEffect_EntryCallback(3, 0, 0, 0, 0, 0, 0) == -1);
    assert(SzEffect_EntryCallback(0, -1, 0, 0, 0, 0, 0) == -1);
    assert(SzEffect_EntryCallback(0, 3, 0, 0, 0, 0, 0) == -1);
    assert(SzEffect_EntryCallback(2, 2, 0, 0, 0, 0, 0) == -1);
    assert(ec_n == 0);

    /* type 1 while any type-1 entry is in use: refused before any call */
    ec_reset(0);
    gSzEffect_el_pool.unknown_100[5].unknown_00_bits_0_2 = 2;
    gSzEffect_el_pool.unknown_100[5].unknown_02_bits_0_2 = 1;
    assert(SzEffect_EntryCallback(1, 0, 0, 0, 0, 0, 0) == -1 && ec_n == 0);

    /* type 0, the module load fails */
    ec_reset(0);
    ec_load_result = 0;
    assert(SzEffect_EntryCallback(0, 2, 0, 0, 0, 0, 0) == -1);
    assert(ec_n == 1 && ec_log[0] == 4);
    assert(ec_load_args[0] == 0 && ec_load_args[1] == 2 && ec_load_args[2] == 1);

    /* type 0, module loaded, no free entry */
    ec_reset(1);
    assert(SzEffect_EntryCallback(0, 2, 0, 0, 0, 0, 0) == -1 && ec_n == 1);

    /* type 0, first free entry is 3 (boundary: entries 0-2 busy): full path */
    ec_reset(1);
    gSzEffect_el_pool.unknown_100[3].unknown_00_bits_0_2 = 0;
    q = &gSzEffect_el_pool.unknown_100[3];
    q->unknown_02_bit_13 = -1; q->unknown_02_bit_14 = -1; q->unknown_02_bit_15 = -1;
    q->unknown_00_bits_3_15 = 0x1fff; q->unknown_10 = 9;
    assert(SzEffect_EntryCallback(0, 2, 0x1234, 0x0506, 0x0708, ec_hook, 55) == 3);
    assert(ec_n == 9);
    for (i = 0; i < 9; i++) {
        assert(ec_log[i] == full[i]);
    }
    assert(ec_copy_dst == (void *)dotr_pool_overlay && ec_copy_count == 2048);
    assert(ec_copy_src == (void *)(DotrPoolAddr)(2 * 0x8000 + 0x1ee0000));
    assert(ec_init_address == (void *)dotr_pool_overlay && ec_init_size == 0x4321);
    assert(gSzEffect_el_pool.unknown_340 == 0 && gSzEffect_el_pool.unknown_344 == 2);
    assert(ec_target == 0x0506 && ec_from == 0x0708 && ec_push_arg == 0);
    assert(ec_fn_word == 102 && ec_fn_address == 3 * 0x8000 + 0x1e10000 && ec_fn_arg == 0x1234);
    assert(q->unknown_00_bits_0_2 == 1 && q->unknown_00_bits_3_15 == 0);
    assert(q->unknown_02_bits_0_2 == 0 && q->unknown_02_bits_3_12 == 2);
    assert(q->unknown_02_bit_13 == 0 && q->unknown_02_bit_14 == 0 && q->unknown_02_bit_15 == 0);
    assert(q->unknown_04 == 0x0506 && q->unknown_06 == 0x0708);
    assert(q->unknown_08 == ec_hook && q->unknown_0c == 55 && q->unknown_10 == 0 && q->unknown_14 == 0x1234);
    assert(ec_hook_calls == 0);

    /* the same type and id are already in the overlay window: no copy; last entry (23) */
    ec_reset(1);
    gSzEffect_el_pool.unknown_100[23].unknown_00_bits_0_2 = 0;
    gSzEffect_el_pool.unknown_340 = 0;
    gSzEffect_el_pool.unknown_344 = 1;
    assert(SzEffect_EntryCallback(0, 1, 1, 2, 3, 0, 0) == 23);
    assert(ec_n == 6 && ec_log[0] == 4 && ec_log[1] == 8 && ec_log[5] == 12);
    assert(ec_fn_word == 101 && ec_fn_address == 23 * 0x8000 + 0x1e10000);
    /* same type, other id: copied again */
    ec_reset(1);
    gSzEffect_el_pool.unknown_100[0].unknown_00_bits_0_2 = 0;
    gSzEffect_el_pool.unknown_340 = 0;
    gSzEffect_el_pool.unknown_344 = 1;
    assert(SzEffect_EntryCallback(0, 0, 1, 2, 3, 0, 0) == 0 && ec_n == 9);
    assert(gSzEffect_el_pool.unknown_344 == 0 && ec_fn_address == 0x1e10000);

    /* type 1: module from the B overlay of the unit model, fixed work address, no load */
    ec_reset(0);
    ec_overlay_b = 0x1f00000;
    assert(SzEffect_EntryCallback(1, 1, 4, 5, 0x0a0b, 0, 0) == 0);
    assert(ec_log[0] == 1 && ec_log[1] == 2 && ec_log[2] == 5 && ec_n == 10);
    assert(ec_model_args[0] == 0x0a && ec_model_args[1] == 0x0b && ec_overlay_arg == 77);
    assert(ec_copy_src == (void *)(DotrPoolAddr)0x1f00000);
    assert(ec_fn_word == 201 && ec_fn_address == 0x1ed0000);
    assert(gSzEffect_el_pool.unknown_100[0].unknown_02_bits_0_2 == 1);
    assert(gSzEffect_el_pool.unknown_340 == 1 && gSzEffect_el_pool.unknown_344 == 1);

    /* type 2: M overlay; a zero answer falls back to the module load */
    ec_reset(0);
    ec_overlay_m = 0x1f80000;
    assert(SzEffect_EntryCallback(2, 0, 4, 5, 0x0c0d, 0, 0) == 0);
    assert(ec_log[0] == 1 && ec_log[1] == 3 && ec_log[2] == 5 && ec_n == 10);
    assert(ec_model_args[0] == 0x0c && ec_model_args[1] == 0x0d);
    assert(ec_copy_src == (void *)(DotrPoolAddr)0x1f80000 && ec_fn_word == 300 && ec_fn_address == 0x1e10000);
    ec_reset(0);
    assert(SzEffect_EntryCallback(2, 1, 4, 5, 0x0c0d, 0, 0) == 0);
    assert(ec_log[2] == 4 && ec_n == 11 && ec_load_args[0] == 2 && ec_load_args[1] == 1);
    assert(ec_copy_src == (void *)(DotrPoolAddr)(2 * 0x8000 + 0x1ee0000));

    /* the effect answers 8: the entry is destroyed at once (its callback runs) and -1 is answered */
    ec_reset(0);
    ec_fn_result = 8;
    assert(SzEffect_EntryCallback(0, 0, 1, 2, 3, ec_hook, 66) == -1);
    assert(ec_hook_calls == 1 && ec_hook_arg == 66);
    assert(gSzEffect_el_pool.unknown_100[0].unknown_00_bits_0_2 == 0);
    /* any other answer keeps it */
    ec_reset(0);
    ec_fn_result = 7;
    assert(SzEffect_EntryCallback(0, 0, 1, 2, 3, ec_hook, 66) == 0 && ec_hook_calls == 0);
    assert(gSzEffect_el_pool.unknown_100[0].unknown_00_bits_0_2 == 1);
}

/* 0x1f40d0 stubs: each appends one letter to dr_s; arguments are kept for the checks. */
static char dr_s[64];
static int dr_n, dr_motion, dr_anim, dr_param, dr_dir, dr_max, dr_rand, dr_cam_n;
static int dr_cam[3][3], dr_intr, dr_intr_param[2], dr_stone, dr_delay, dr_max_arg;
static float dr_frame, dr_set_frame, dr_speed[2];
static int dr_speed_n;
static void dr_put(char c) { dr_s[dr_n++] = c; dr_s[dr_n] = 0; }
int SzGame_GetMode(int a) { assert(a == 1); dr_put('G'); return 0; }
int func_0019a5c8(void) { dr_put('r'); return dr_rand; }
int SzEffect_GetPlayMotion(int a) { assert(a == 0); dr_put('m'); return dr_motion; }
int SzModel_GetAnim(int index) { assert(index == 77); dr_put('a'); return dr_anim; }
int SzModel_GetAnimParam(int index, int slot) { assert(index == 77 && slot == 1); dr_put('p'); return dr_param; }
int SzModel_GetAnimDir(int index) { assert(index == 77); dr_put('d'); return dr_dir; }
float SzModel_GetAnimFrame(int index) { assert(index == 77); dr_put('f'); return dr_frame; }
int SzModel_GetAnimFrameMax(int index, int value) { assert(index == 77); dr_put('X'); dr_max_arg = value; return dr_max; }
int SzView_FreeRequest(int a, int b) { assert(a == 0 && b == 1); dr_put('R'); return 0; }
void SzView_ReqCamera(int a, int b, int c, unsigned short d, int index, void *data)
{
    assert(a == 0 && b == 2 && data == 0 && dr_cam_n < 3);
    dr_put('C');
    dr_cam[dr_cam_n][0] = c; dr_cam[dr_cam_n][1] = d; dr_cam[dr_cam_n][2] = index;
    dr_cam_n++;
}
int SzModel_SetStoneFlag(int index, int value) { assert(index == 77); dr_put('S'); dr_stone = value; return 0; }
int SzModel_SetMotionDelay(int index, int value) { assert(index == 77); dr_put('D'); dr_delay = value; return 0; }
int SzModel_SetAnimFrame(int index, float value) { assert(index == 77); dr_put('F'); dr_set_frame = value; return 0; }
int SzModel_SetAnimSpeed(int index, float value) { assert(index == 77); dr_put('s'); dr_speed[dr_speed_n++ & 1] = value; return 0; }
int SzModel_SetAnimIntrReverse(int index) { assert(index == 77); dr_put('v'); return 0; }
int SzModel_SetAnimIntr(int index, int value) { assert(index == 77); dr_put('i'); dr_intr = value; return 0; }
int SzModel_SetAnimIntrParam(int index, int slot, int unknown)
{
    assert(index == 77);
    dr_put('P');
    dr_intr_param[0] = slot; dr_intr_param[1] = unknown;
    return 0;
}

static struct DotrPoolEntry24 *dr_entry(int index, int selector, int id, int state)
{
    struct DotrPoolEntry24 *q = &gSzEffect_el_pool.unknown_100[index];
    q->unknown_00_bits_0_2 = (unsigned short)state;
    q->unknown_02_bits_0_2 = (unsigned short)selector;
    q->unknown_02_bits_3_12 = (unsigned short)id;
    q->unknown_04 = 0x0506;
    q->unknown_06 = 0x0a0b;
    return q;
}

static void dr_clear(void)
{
    dr_n = 0; dr_s[0] = 0; dr_cam_n = 0; dr_speed_n = 0; ec_n = 0;
    dr_intr = dr_stone = dr_delay = -99;
}

/* One selector-1 entry (index 3, B overlay present) run once with the given answers. */
static struct DotrPoolEntry24 *dr_unit(int result, int motion, int anim, int param, int dir, float frame)
{
    struct DotrPoolEntry24 *q;
    ec_reset(0);
    dr_clear();
    q = dr_entry(3, 1, 1, 1);
    ec_overlay_b = 0x1f00000;
    ec_fn_result = result;
    dr_motion = motion; dr_anim = anim; dr_param = param; dr_dir = dir; dr_frame = frame;
    SzEffect_Draw(1);
    return q;
}

static void check_draw(void)
{
    struct DotrPoolEntry24 *q;
    struct DotrPoolEntry8 *e;

    /* no entry in state 1: mode, push and pop only */
    ec_reset(0);
    dr_clear();
    SzEffect_Draw(-1);
    assert(strcmp(dr_s, "G") == 0 && ec_n == 2 && ec_log[0] == 10 && ec_log[1] == 12 && ec_push_arg == 0);
    /* state 2 is not run; state 1 with another selector than type is not run */
    ec_reset(2);
    dr_clear();
    SzEffect_Draw(-1);
    assert(ec_n == 2);
    ec_reset(0);
    dr_clear();
    dr_entry(3, 1, 1, 1);
    SzEffect_Draw(0);
    assert(ec_n == 2);

    /* selector 0: no 8-byte entry with that id, or one not in state 2: skipped */
    ec_reset(0);
    dr_clear();
    q = dr_entry(3, 0, 2, 1);
    gSzEffect_el_pool.unknown_000[0].unknown_bits_6_15 = 9;
    for (e = &gSzEffect_el_pool.unknown_000[1]; e < &gSzEffect_el_pool.unknown_000[32]; e++) {
        e->unknown_bits_6_15 = 9;
    }
    SzEffect_Draw(-1);
    assert(ec_n == 2 && q->unknown_00_bits_3_15 == 0);
    e = &gSzEffect_el_pool.unknown_000[2];
    e->unknown_bits_6_15 = 2;
    e->unknown_bits_0_2 = 1;
    dr_clear();
    SzEffect_Draw(0);
    assert(ec_n == 2 && q->unknown_00_bits_3_15 == 0);
    /* in state 2: copied from its module address, run with the entry's work address and -1 */
    e->unknown_bits_0_2 = 2;
    ec_fn_result = 5;
    dr_clear();
    SzEffect_Draw(0);
    assert(ec_n == 8 && ec_log[1] == 5 && ec_log[2] == 6 && ec_log[3] == 7 && ec_log[4] == 8);
    assert(ec_log[5] == 9 && ec_log[6] == 11 && ec_log[7] == 12);
    assert(ec_copy_dst == (void *)dotr_pool_overlay && ec_copy_count == 2048);
    assert(ec_copy_src == (void *)(DotrPoolAddr)(2 * 0x8000 + 0x1ee0000));
    assert(ec_init_address == (void *)dotr_pool_overlay && ec_init_size == 0x4321);
    assert(gSzEffect_el_pool.unknown_340 == 0 && gSzEffect_el_pool.unknown_344 == 2);
    assert(ec_target == 0x0506 && ec_from == 0x0a0b);
    assert(ec_fn_word == 102 && ec_fn_address == 3 * 0x8000 + 0x1e10000 && ec_fn_arg == -1);
    assert(q->unknown_10 == 5 && q->unknown_00_bits_3_15 == 1 && q->unknown_00_bits_0_2 == 1);
    assert(strcmp(dr_s, "G") == 0);
    /* already in the window: no copy; bit 13 gives -2, bit 14 gives -3 (it wins) */
    q->unknown_02_bit_13 = -1;
    dr_clear();
    SzEffect_Draw(-1);
    assert(ec_n == 5 && ec_log[1] == 8 && ec_fn_arg == -2 && q->unknown_00_bits_3_15 == 2);
    q->unknown_02_bit_14 = -1;
    dr_clear();
    SzEffect_Draw(-1);
    assert(ec_n == 5 && ec_fn_arg == -3);
    /* same selector, other id in the window: copied again */
    gSzEffect_el_pool.unknown_344 = 1;
    dr_clear();
    SzEffect_Draw(-1);
    assert(ec_n == 8 && gSzEffect_el_pool.unknown_344 == 2);
    /* answer 8: with bit 14 (-3) the entry is destroyed; with bit 13 alone it goes to state 2 */
    ec_fn_result = 8;
    dr_clear();
    SzEffect_Draw(-1);
    assert(q->unknown_00_bits_0_2 == 0);
    q->unknown_00_bits_0_2 = 1;
    q->unknown_02_bit_14 = 0;
    e->unknown_bits_0_2 = 2;
    dr_clear();
    SzEffect_Draw(-1);
    assert(q->unknown_00_bits_0_2 == 2 && q->unknown_10 == 8);
    /* last entry (23), no flag bits, answer 8: destroyed; its work address */
    ec_reset(0);
    dr_clear();
    q = dr_entry(23, 0, 0, 1);
    gSzEffect_el_pool.unknown_000[0].unknown_bits_0_2 = 2;
    ec_fn_result = 8;
    SzEffect_Draw(-1);
    assert(q->unknown_00_bits_0_2 == 0 && ec_fn_address == 23 * 0x8000 + 0x1e10000);
    assert(ec_copy_src == (void *)(DotrPoolAddr)0x1ee0000);

    /* type 2: M overlay of the unit model at `from`; work address by index */
    ec_reset(0);
    dr_clear();
    q = dr_entry(1, 2, 1, 1);
    ec_overlay_m = 0x1f80000;
    SzEffect_Draw(2);
    assert(ec_log[1] == 1 && ec_log[2] == 3 && ec_model_args[0] == 0x0a && ec_model_args[1] == 0x0b);
    assert(ec_copy_src == (void *)(DotrPoolAddr)0x1f80000 && ec_fn_word == 301);
    assert(ec_fn_address == 0x8000 + 0x1e10000 && strcmp(dr_s, "G") == 0);
    /* type 1: B overlay, fixed work address; then the unit queries on the target cell */
    ec_reset(0);
    dr_clear();
    q = dr_entry(3, 1, 1, 1);
    ec_overlay_b = 0x1f00000;
    ec_fn_result = 9;
    dr_motion = 0; dr_anim = 0; dr_param = 0; dr_dir = 0; dr_frame = 0.0f;
    SzEffect_Draw(1);
    assert(ec_log[1] == 1 && ec_log[2] == 2 && ec_copy_src == (void *)(DotrPoolAddr)0x1f00000);
    assert(ec_fn_word == 201 && ec_fn_address == 0x1ed0000);
    assert(ec_model_args[0] == 0x05 && ec_model_args[1] == 0x06);
    assert(strcmp(dr_s, "Gmapdf") == 0 && q->unknown_00_bits_0_2 == 1);

    /* answer 1 becomes 6 when the play motion is 6, else 0 */
    q = dr_unit(1, 6, 3, 0, 1, 10.0f);
    assert(strcmp(dr_s, "GmapdfD") == 0 && dr_delay == 0); /* frame below 11: no step back */
    dr_unit(1, 6, 3, 0, 0, 20.0f);
    assert(strcmp(dr_s, "GmapdfD") == 0); /* direction not 1 */
    dr_rand = 0x40000000;
    dr_unit(1, 6, 4, 0, 1, 11.0f);
    assert(strcmp(dr_s, "GmapdfDrF") == 0 && dr_set_frame == 9.0f); /* 11..16: by chance */
    dr_rand = 0;
    dr_unit(1, 6, 6, 0, 1, 15.0f);
    assert(strcmp(dr_s, "GmapdfDr") == 0);
    dr_unit(1, 6, 6, 0, 1, 16.0f);
    assert(strcmp(dr_s, "GmapdfDF") == 0 && dr_set_frame == 14.0f); /* 16 and above: always */
    dr_unit(6, 5, 3, 0, 1, 16.0f);
    assert(strcmp(dr_s, "GmapdfF") == 0); /* motion not 6: no delay call */
    dr_unit(1, 5, 4, 0, 1, 16.0f);
    assert(strcmp(dr_s, "Gmapdf") == 0); /* 1 -> 0, motion not 3 */
    /* answer 0: motion 3, animation 4 and frame at least 16 start the interrupt */
    dr_unit(0, 3, 4, -1, 0, 16.0f);
    assert(strcmp(dr_s, "GmapdfsPs") == 0 && dr_speed[0] == 0.0f && dr_speed[1] == 1.0f);
    assert(dr_intr_param[0] == 0 && dr_intr_param[1] == 16);
    dr_unit(0, 3, 4, 0, 0, 16.0f);
    assert(strcmp(dr_s, "Gmapdfsvs") == 0);
    dr_unit(0, 3, 4, 0, 0, 15.0f);
    assert(strcmp(dr_s, "Gmapdf") == 0);
    dr_unit(0, 3, 3, 0, 0, 16.0f);
    assert(strcmp(dr_s, "Gmapdf") == 0);
    dr_unit(0, 4, 4, 0, 0, 16.0f);
    assert(strcmp(dr_s, "Gmapdf") == 0);

    /* answers 3, 4 and 8 request the camera once (bit 15); the length is at least 30 */
    dr_max = 100;
    q = dr_unit(3, 5, 5, 0, 0, 40.0f);
    assert(strcmp(dr_s, "GmapdffXRCCCsvs") == 0 && dr_max_arg == 5 && q->unknown_02_bit_15 != 0);
    assert(dr_cam[0][0] == 0 && dr_cam[0][1] == 60 && dr_cam[0][2] == 800);
    assert(dr_cam[1][0] == 1 && dr_cam[1][1] == 60 && dr_cam[1][2] == 0);
    assert(dr_cam[2][0] == 6 && dr_cam[2][1] == 60 && dr_cam[2][2] == 77);
    assert(dr_speed[0] == 1.0f && dr_speed[1] == 1.0f);
    dr_clear();
    ec_fn_result = 3;
    SzEffect_Draw(1);
    assert(strcmp(dr_s, "Gmapdfsvs") == 0 && q->unknown_00_bits_3_15 == 2); /* bit 15 set: not again */
    dr_max = 71;
    dr_unit(3, 5, 5, 0, 0, 41.0f);
    assert(dr_cam[0][1] == 30); /* 30 stays */
    dr_max = 70;
    dr_unit(3, 5, 5, 0, 0, 41.0f);
    assert(dr_cam[0][1] == 30); /* 29 is raised */
    dr_unit(3, 5, 4, 0, 0, 41.0f);
    assert(strcmp(dr_s, "GmapdfXRCCC") == 0 && dr_cam[2][1] == 70); /* other animation: from frame 0 */
    dr_unit(4, 3, 4, -1, 0, 16.0f);
    assert(strcmp(dr_s, "GmapdfXRCCCsPs") == 0); /* 4 continues as 0 */
    q = dr_unit(8, 3, 4, 0, 0, 0.0f);
    assert(strcmp(dr_s, "GmapdfXRCCCsvs") == 0 && q->unknown_00_bits_0_2 == 0); /* 8: interrupt, destroyed */
    q = dr_unit(8, 3, 3, 0, 0, 0.0f);
    assert(strcmp(dr_s, "GmapdffXRCCC") == 0 && q->unknown_00_bits_0_2 == 0);
    q = dr_unit(8, 4, 4, 0, 0, 0.0f);
    assert(strcmp(dr_s, "GmapdffXRCCC") == 0 && q->unknown_00_bits_0_2 == 0);

    /* answers 2, 3, 5, 6 with an animation other than 3, 4, 6: interrupt to the motion (3 as 4) */
    dr_unit(2, 3, 0, -1, 0, 0.0f);
    assert(strcmp(dr_s, "Gmapdfsis") == 0 && dr_intr == 4 && dr_speed[0] == 1.0f);
    dr_unit(2, 5, 0, -1, 0, 0.0f);
    assert(strcmp(dr_s, "Gmapdfsis") == 0 && dr_intr == 5);
    dr_unit(5, 4, 0, -1, 0, 0.0f);
    assert(strcmp(dr_s, "GmapdfSsis") == 0 && dr_stone == 1 && dr_intr == 4); /* motion 4, answer 5: stone */
    dr_unit(5, 3, 3, -1, 0, 0.0f);
    assert(strcmp(dr_s, "Gmapdf") == 0);
    dr_unit(6, 4, 0, 0, 0, 0.0f);
    assert(strcmp(dr_s, "Gmapdfsvs") == 0);
    /* answer 7: animations 3 and 4 only */
    dr_unit(7, 0, 3, 0, 0, 0.0f);
    assert(strcmp(dr_s, "Gmapdfsvs") == 0 && dr_speed[0] == 0.0f);
    dr_unit(7, 0, 4, -1, 0, 0.0f);
    assert(strcmp(dr_s, "GmapdfsPs") == 0);
    dr_unit(7, 0, 6, 0, 0, 0.0f);
    assert(strcmp(dr_s, "Gmapdf") == 0);
    dr_unit(7, 0, 2, 0, 0, 0.0f);
    assert(strcmp(dr_s, "Gmapdf") == 0);
    ec_reset(0);
}

int main(void)
{
    check_stop();
    check_drop();
    check_status();
    check_small_entries();
    check_reset();
    check_entry_flag_and_block();
    check_filtered_flag();
    check_cues();
    check_block_angles();
    check_block_matrix();
    check_block_turn();
    check_load_sectl_callback();
    check_load_overlay_callback();
    check_entry_callback();
    check_draw();
    puts("pool_table: ok");
    return 0;
}
