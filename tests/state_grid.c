/* Synthetic tests of the grid/item helpers. Dependencies outside the group
 * are stubs with assumed semantics; table contents are fixtures, not game data. */
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/state_grid.h"
#include "dotr/ee/object_table.h"

struct DotrStateEntry20 gSzDuel_CardConstInfo[854];
struct DotrStateItem gSzDuel_aaVarInfo[3][11];
unsigned char EnchMatch[7 * 4];
struct DotrStateCell gSzDuel_aaMapInfo[7][7];
struct DotrStateEntry8 EffIdx_L[854][5];
struct DotrStateEntry8 EffIdx_M[16][5];
struct DotrStateEntry8 EffIdx_S[16];

static unsigned probe_result;
static int probe_calls, probe_x, probe_y;
static struct DotrStateItem *probe_item;

unsigned GetMoveRev(struct DotrStateItem *item, int x, int y)
{
    probe_calls++;
    probe_item = item;
    probe_x = x;
    probe_y = y;
    return probe_result;
}

struct DotrStateControl data_00317490_control[1];
int gSzBattle_bl[3];

static int fill_calls, gate_result, gate_argument, follow_calls, follow_argument;

/* Assumed fill semantics (destination, value, byte count). */
void RhMem_FillMemory(void *destination, int value, int size)
{
    fill_calls++;
    assert(destination == (void *)gSzBattle_bl && size == 12);
    memset(destination, value, (size_t)size);
}

int SzModel_GetDrawFlag(int unknown)
{
    gate_argument = unknown;
    return gate_result;
}

int lSzDuel_DrawStage_Pipe(int unknown)
{
    follow_calls++;
    follow_argument = unknown;
    return 0x2468;
}

int lSzBattle_DrawStage_Pipe(int unknown)
{
    follow_calls++;
    follow_argument = unknown;
    return 0x1357;
}

static int sweep_calls, sweep_extra[4], sweep_last[6];
static int cell_log[64][4], cell_count;

void SzDuel_SetMapColor2(int a, int b, int c, int d, int e, int f)
{
    /* During a grid sweep the first two arguments are column and row. */
    if (sweep_calls >= 0) {
        assert(a == sweep_calls % 7 && b == sweep_calls / 7);
        assert(c == sweep_extra[0] && d == sweep_extra[1] && e == sweep_extra[2] && f == sweep_extra[3]);
        sweep_calls++;
    }
    if (cell_count >= 0 && cell_count < 64) {
        cell_log[cell_count][0] = a;
        cell_log[cell_count][1] = b;
        cell_log[cell_count][2] = c;
        cell_log[cell_count][3] = e;
        cell_count++;
    }
    sweep_last[0] = a;
    sweep_last[1] = b;
    sweep_last[2] = c;
    sweep_last[3] = d;
    sweep_last[4] = e;
    sweep_last[5] = f;
}

static int tail_calls, tail_args[3];
static struct DotrStateItem *tail_item;

int SzDuel_GetUnitLeaderAbility3(struct DotrStateItem *item, int c, int d, int e)
{
    tail_calls++;
    tail_item = item;
    tail_args[0] = c;
    tail_args[1] = d;
    tail_args[2] = e;
    return 0x4567;
}

static int kind_result, kind_calls, kind_argument;
static int visit_calls;
static int pair_calls, pair_args[2][4], pair_results[2];

static int cached_calls, cached_a, cached_b, cached_first, cached_second;
static int cached_input, cached_result;
static struct DotrStateItem *cached_item;

int SzDuel_DelUnit(int a, int b)
{
    assert(cached_calls++ == 0);
    assert(a == cached_a && b == cached_b);
    /* Later arguments must retain the bytes read before this dependency. */
    cached_item->locCol = cached_item->locRow = 0;
    cached_item->team = cached_item->slot = 255;
    return 1;
}

void SzDuel_InitCardInfo(struct DotrStateBlock *block, int unknown)
{
    int i;
    assert(cached_calls++ == 1 && unknown == cached_input);
    for (i = 0; i < 24; i++) block->unknown_words[i] = i + 100;
}

int SzDuel_AddUnit2(int a, int b, int c, struct DotrStateBlock *block, int flag)
{
    int i;
    assert(cached_calls++ == 2);
    assert(a == cached_a && b == cached_first && c == cached_second);
    assert(flag == (cached_b == 0));
    for (i = 0; i < 24; i++) assert(block->unknown_words[i] == i + 100);
    return cached_result;
}

/* SzDuel_SetUnitStatus (branch coverage: null item, +0x45 of 10 and 11, every selector
 * 0..5 with value 0, 1 and 2 over both prior states, selectors -1 and 6).
 * 0x1dd130 is the real function. */
static void check_set_selected_bit(void)
{
    /* bit of +0x57 written per selector, second bit (or -1), pair shift in +0x56 (or -1) */
    static const signed char first[6] = { 0, 1, 2, 3, 4, 5 };
    static const signed char second[6] = { -1, -1, -1, -1, 2, 3 };
    static const signed char pair[6] = { -1, -1, 4, 6, 4, 6 };
    static const int values[3] = { 0, 1, 2 };
    struct DotrStateItem *item = &gSzDuel_aaVarInfo[1][4];
    struct DotrStateItem expected;
    int bit, v, fill;

    for (bit = 0; bit < 6; bit++) {
        for (v = 0; v < 3; v++) {
            for (fill = 0; fill < 256; fill += 255) {
                unsigned flags = (unsigned)fill, pairs = (unsigned)fill;
                unsigned low = (unsigned)values[v] & 1;

                item->slot = 10;
                *(unsigned char *)&item->status = (unsigned char)fill;
                *(unsigned char *)&item->unknown_56 = (unsigned char)fill;
                memcpy(&expected, item, sizeof(expected));
                assert(SzDuel_SetUnitStatus(1, 4, bit, values[v]) == 1);
                /* the low bit of value goes into the flag bits */
                flags = (flags & ~(1u << first[bit])) | (low << first[bit]);
                if (second[bit] >= 0) {
                    flags = (flags & ~(1u << second[bit])) | (low << second[bit]);
                }
                /* the pair becomes 1 for any non-zero value, else 3 */
                if (pair[bit] >= 0) {
                    pairs = (pairs & ~(3u << pair[bit])) | ((values[v] ? 1u : 3u) << pair[bit]);
                }
                *(unsigned char *)&expected.status = (unsigned char)flags;
                *(unsigned char *)&expected.unknown_56 = (unsigned char)pairs;
                assert(memcmp(&expected, item, sizeof(expected)) == 0);
            }
        }
    }
    memcpy(&expected, item, sizeof(expected));
    assert(SzDuel_SetUnitStatus(1, 4, -1, 1) == 0 && SzDuel_SetUnitStatus(1, 4, 6, 1) == 0);
    assert(SzDuel_SetUnitStatus(3, 0, 0, 1) == 0 && SzDuel_SetUnitStatus(0, 11, 0, 1) == 0);
    assert(memcmp(&expected, item, sizeof(expected)) == 0);
    item->slot = 11;
    memcpy(&expected, item, sizeof(expected));
    assert(SzDuel_SetUnitStatus(1, 4, 0, 0) == 0 && memcmp(&expected, item, sizeof(expected)) == 0);
    item->slot = 0;
}

static void check_selected_bit(void)
{
    int a, b, bits, bit;
    for (a = 0; a < 3; a++) {
        for (b = 0; b < 11; b++) {
            struct DotrStateItem *item = &gSzDuel_aaVarInfo[a][b];
            item->slot = 0;
            for (bits = 0; bits < 256; bits++) {
                struct DotrStateItem expected;
                *(unsigned char *)&item->status = (unsigned char)bits;
                memcpy(&expected, item, sizeof(expected));
                for (bit = -1; bit <= 6; bit++) {
                    int result = bit >= 0 && bit < 6 ? (bits >> bit) & 1 : 0;
                    assert(SzDuel_GetUnitStatus(a, b, bit) == result);
                    assert(memcmp(&expected, item, sizeof(expected)) == 0);
                }
            }
            assert(SzDuel_GetUnitStatus(a, b, INT_MIN) == 0);
            assert(SzDuel_GetUnitStatus(a, b, INT_MAX) == 0);
        }
    }
    assert(SzDuel_GetUnitStatus(3, 0, 0) == 0);
    assert(SzDuel_GetUnitStatus(0, 11, 0) == 0);
    gSzDuel_aaVarInfo[0][0].slot = 11;
    assert(SzDuel_GetUnitStatus(0, 0, 0) == 0);
}

static void check_cached_dispatch(void)
{
    static const int inputs[] = {INT_MIN, -1, 0, INT_MAX};
    int a, b, i;
    cached_calls = 0;
    assert(SzDuel_PutUnit(3, 0, 7) == 0 && cached_calls == 0);
    assert(SzDuel_PutUnit(0, 11, 7) == 0 && cached_calls == 0);
    gSzDuel_aaVarInfo[0][0].slot = 11;
    assert(SzDuel_PutUnit(0, 0, 7) == 0 && cached_calls == 0);
    for (a = 0; a < 3; a++) {
        for (b = 0; b < 11; b++) {
            for (i = 0; i < 4; i++) {
                cached_item = &gSzDuel_aaVarInfo[a][b];
                cached_a = (a * 100 + b * 15) & 255;
                cached_b = b;
                cached_first = 255 - b;
                cached_second = b * 23;
                cached_item->team = (unsigned char)cached_a;
                cached_item->slot = (unsigned char)cached_b;
                cached_item->locCol = (unsigned char)cached_first;
                cached_item->locRow = (unsigned char)cached_second;
                cached_input = inputs[i];
                cached_result = inputs[3 - i];
                cached_calls = 0;
                assert(SzDuel_PutUnit(a, b, cached_input) == cached_result);
                assert(cached_calls == 3);
            }
        }
    }
}

int SzModel_GetModelType(int unknown)
{
    kind_calls++;
    kind_argument = unknown;
    return kind_result;
}

void lSzDuel_DoUnitRequest(struct DotrStateCell *cell, DotrStateWord unknown)
{
    /* Row-major order: the row index is the outer loop. */
    assert(cell == &gSzDuel_aaMapInfo[visit_calls / 7][visit_calls % 7]);
    assert(unknown == cell->unit);
    visit_calls++;
}

int SzDuel_TrimUnitDir2(int a, int b, int c, int d)
{
    assert(pair_calls < 2);
    pair_args[pair_calls][0] = a;
    pair_args[pair_calls][1] = b;
    pair_args[pair_calls][2] = c;
    pair_args[pair_calls][3] = d;
    return pair_results[pair_calls++];
}

static void check_cells(void)
{
    static const int bad[] = {INT_MIN, -1, 7, INT_MAX};
    struct DotrStateCell cell, expected;
    int x, y;
    size_t i;
    /* Host offsets differ from the target because the first word is
     * pointer-sized here; only the relative layout is checked. */
    assert(offsetof(struct DotrStateCell, mapAlpha) - offsetof(struct DotrStateCell, unknown_04) == 0x16);
    assert(offsetof(struct DotrStateCell, fieldRate) - offsetof(struct DotrStateCell, unknown_04) == 0x10);
    memset(&cell, 0xa5, sizeof(cell));
    memcpy(&expected, &cell, sizeof(cell));
    memset(&expected, 0, offsetof(struct DotrStateCell, mapAlpha));
    expected.mapAlpha = 128;
    SzDuel_InitMapInfo(&cell);
    /* The trailing padding byte at +0x1b is not written. */
    assert(memcmp(&cell, &expected, sizeof(cell)) == 0);

    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            assert(SzDuel_GetMapInfo(x, y) == &gSzDuel_aaMapInfo[y][x]);
        }
    }
    for (i = 0; i < sizeof(bad) / sizeof(bad[0]); i++) {
        assert(SzDuel_GetMapInfo(bad[i], 0) == 0);
        assert(SzDuel_GetMapInfo(0, bad[i]) == 0);
    }

    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) gSzDuel_aaMapInfo[y][x].unit = y * 100 + x;
    }
    visit_calls = 0;
    SzDuel_DoUnitRequest();
    assert(visit_calls == 49);
}

static void check_entries(void)
{
    static const int bad[] = {INT_MIN, -1, 671, 854, 999, INT_MAX};
    int id;
    size_t i;
    for (id = 0; id < 854; id++) {
        if (id == 671) continue;
        assert(SzDuel_GetCardConstInfo(id) == &gSzDuel_CardConstInfo[id]);
    }
    for (i = 0; i < sizeof(bad) / sizeof(bad[0]); i++) assert(SzDuel_GetCardConstInfo(bad[i]) == 0);
}

static void check_items(void)
{
    static const int bad_kinds[] = {INT_MIN, -1, 0, 3, 6, INT_MAX};
    int a, b;
    size_t i;
    kind_calls = 0;
    for (a = 0; a < 3; a++) {
        for (b = 0; b < 11; b++) assert(SzDuel_GetCardInfo(a, b) == &gSzDuel_aaVarInfo[a][b]);
        assert(SzDuel_GetCardInfo(a, -1) == 0 && SzDuel_GetCardInfo(a, 11) == 0);
    }
    assert(SzDuel_GetCardInfo(3, 0) == 0 && SzDuel_GetCardInfo(INT_MAX, 0) == 0);
    assert(kind_calls == 0);

    /* Negative first argument: the second is classified by the dependency. */
    for (i = 0; i < sizeof(bad_kinds) / sizeof(bad_kinds[0]); i++) {
        kind_result = bad_kinds[i];
        assert(SzDuel_GetCardInfo(-1, 20) == 0 && kind_argument == 20);
    }
    kind_result = 4;
    for (b = 20; b < 30; b++) {
        assert(SzDuel_GetCardInfo(-1, b) == &gSzDuel_aaVarInfo[b < 25 ? 0 : 1][(b - 20) % 5 + 1]);
    }
    /* No upper guard in this arm: any value of 25 or more lands in row 1.
     * Below 20 the signed remainder decides: 19 gives column 0, 11 gives -3. */
    assert(SzDuel_GetCardInfo(-1, 34) == &gSzDuel_aaVarInfo[1][5]);
    assert(SzDuel_GetCardInfo(-1, 1000) == &gSzDuel_aaVarInfo[1][1]);
    assert(SzDuel_GetCardInfo(-1, 19) == &gSzDuel_aaVarInfo[0][0]);
    assert(SzDuel_GetCardInfo(-1, 11) == 0);
    kind_result = 5;
    for (b = 30; b < 33; b++) assert(SzDuel_GetCardInfo(INT_MIN, b) == &gSzDuel_aaVarInfo[b - 30][0]);
    assert(SzDuel_GetCardInfo(-1, 29) == 0 && SzDuel_GetCardInfo(-1, 33) == 0);
}

static int step_once(int start, int flag)
{
    struct DotrStateItem *item = &gSzDuel_aaVarInfo[1][2];
    memset(gSzDuel_aaVarInfo, 0, sizeof(gSzDuel_aaVarInfo));
    item->slot = 10;
    item->unknown_30 = start;
    item->status.unknown_bit_1 = flag ? -1 : 0;
    assert(SzDuel_TrimUnitDir1(1, 2) == 1);
    return item->unknown_30;
}

static void check_step(void)
{
    struct DotrStateItem expected[3][11];
    /* Starts stay within -0x8000..0x8000, where host and target remainders
     * agree; negative sums, where they differ, are not tested. */
    assert(step_once(0, 0) == 0);
    assert(step_once(0x8000, 1) == 0x8000);
    assert(step_once(100, 0) == 0);
    assert(step_once(-100, 0) == 0);
    assert(step_once(2048, 0) == 0);
    assert(step_once(2049, 0) == 1);
    assert(step_once(-2049, 0) == -1);
    assert(step_once(0x4000, 0) == 0x4000 - 2048);
    assert(step_once(-0x4000, 0) == -0x4000 + 2048);
    /* Exactly half a turn away: the difference normalizes to +0x8000, so the
     * step is positive and the stored value wraps to the negative end. */
    assert(step_once(0x8000, 0) == -0x7800);
    /* -0x8000 is half a turn from zero; the step is positive. */
    assert(step_once(-0x8000, 0) == -0x8000 + 2048);
    assert(step_once(0, 1) == 2048);
    assert(step_once(0x7000, 1) == 0x7800);
    assert(step_once(0x7900, 1) == 0x8000);
    /* Stepping away from -0x7000 toward half a turn wraps to the positive end. */
    assert(step_once(-0x7000, 1) == -0x7800);
    assert(step_once(-0x7900, 1) == 0x8000);
    assert(step_once(-0x8000, 1) == -0x8000);

    /* Rejected lookups return 0 and store nothing. */
    memset(gSzDuel_aaVarInfo, 0x11, sizeof(gSzDuel_aaVarInfo));
    memcpy(expected, gSzDuel_aaVarInfo, sizeof(expected));
    assert(SzDuel_TrimUnitDir1(3, 0) == 0);
    assert(SzDuel_TrimUnitDir1(0, 0) == 0); /* +0x45 is 0x11, which is 11 or more */
    assert(memcmp(expected, gSzDuel_aaVarInfo, sizeof(expected)) == 0);
}

static void check_pair(void)
{
    struct DotrStateItem *p = &gSzDuel_aaVarInfo[0][1], *q = &gSzDuel_aaVarInfo[2][3];
    memset(gSzDuel_aaVarInfo, 0, sizeof(gSzDuel_aaVarInfo));
    p->locCol = 4;
    p->locRow = 5;
    q->locCol = 6;
    q->locRow = 7;
    /* Two distinct items: each position is paired with the other's bytes. */
    pair_calls = 0;
    pair_results[0] = 2;
    pair_results[1] = 8;
    assert(SzDuel_TrimUnitDir(0, 1, 2, 3) == 10);
    assert(pair_calls == 2);
    assert(pair_args[0][0] == 0 && pair_args[0][1] == 1 && pair_args[0][2] == 6 && pair_args[0][3] == 7);
    assert(pair_args[1][0] == 2 && pair_args[1][1] == 3 && pair_args[1][2] == 4 && pair_args[1][3] == 5);
    /* Same item twice, or one rejected lookup: the single-item step runs. */
    pair_calls = 0;
    p->unknown_30 = 100;
    assert(SzDuel_TrimUnitDir(0, 1, 0, 1) == 1 && pair_calls == 0 && p->unknown_30 == 0);
    p->unknown_30 = 100;
    q->unknown_30 = 300;
    assert(SzDuel_TrimUnitDir(0, 1, 3, 0) == 1 && p->unknown_30 == 0 && q->unknown_30 == 300);
    assert(SzDuel_TrimUnitDir(3, 0, 2, 3) == 1 && q->unknown_30 == 0);
    assert(SzDuel_TrimUnitDir(3, 0, 0, 11) == 0 && pair_calls == 0);
}

static void check_values(void)
{
    static const int raw[] = {0, 1, 0x1fff, 0x2000, 0xe123, 0xffff};
    static const int adjust[] = {-32768, -9000, -1, 0, 1, 2000, 32767};
    struct DotrStateItemSource source;
    struct DotrStateItem *item = &gSzDuel_aaVarInfo[2][4];
    size_t r, a;
    int flag, want;
    assert(SzDuel_GetUnitAP2(0, 0) == -1 && SzDuel_GetUnitDP2(0, 1) == -1);
    memset(gSzDuel_aaVarInfo, 0, sizeof(gSzDuel_aaVarInfo));
    assert(SzDuel_GetUnitAP2(item, 0) == -1 && SzDuel_GetUnitDP2(item, 0) == -1);
    item->constInfo = &source;
    for (r = 0; r < sizeof(raw) / sizeof(raw[0]); r++) {
        for (a = 0; a < sizeof(adjust) / sizeof(adjust[0]); a++) {
            for (flag = 0; flag < 2; flag++) {
                /* Each routine reads only its own halfword and adjustment. */
                source.apWithFlags = (unsigned short)raw[r];
                source.dpWithFlags = 0x1234;
                item->apBoost = (short)adjust[a];
                item->dpBoost = 77;
                want = (raw[r] & 0x1fff) + (flag ? 0 : adjust[a]);
                want = want < 0 ? 0 : want > 9999 ? 9999 : want;
                assert(SzDuel_GetUnitAP2(item, flag) == want);
                assert(SzDuel_GetUnitAP(2, 4, flag) == want);
                source.apWithFlags = 0x1234;
                source.dpWithFlags = (unsigned short)raw[r];
                item->apBoost = 77;
                item->dpBoost = (short)adjust[a];
                assert(SzDuel_GetUnitDP2(item, flag) == want);
                assert(SzDuel_GetUnitDP(2, 4, flag) == want);
            }
        }
    }
    /* The wrappers pass a rejected lookup through as null. */
    item->slot = 11;
    assert(SzDuel_GetUnitAP(2, 4, 0) == -1 && SzDuel_GetUnitDP(2, 4, 0) == -1);
    assert(SzDuel_GetUnitAP(3, 0, 0) == -1 && SzDuel_GetUnitDP(0, 11, 1) == -1);
}

static void check_counts(void)
{
    int row, column, low, high;
    /* +0x45 of 11 or more rejects an item; columns 1-5 and 6-10 are counted
     * separately and column 0 is never counted. */
    for (row = 0; row < 3; row++) {
        memset(gSzDuel_aaVarInfo, 0, sizeof(gSzDuel_aaVarInfo));
        for (column = 0; column < 11; column++) gSzDuel_aaVarInfo[row][column].slot = 11;
        assert(SzDuel_GetUnitMonsterCount(row) == 0 && SzDuel_GetUnitSpellCount(row) == 0 && SzDuel_GetUnitCount(row) == 0);
        low = high = 0;
        gSzDuel_aaVarInfo[row][0].slot = 0;
        assert(SzDuel_GetUnitCount(row) == 0);
        for (column = 1; column < 11; column += 2) {
            gSzDuel_aaVarInfo[row][column].slot = 10;
            if (column < 6) low++; else high++;
            assert(SzDuel_GetUnitMonsterCount(row) == low && SzDuel_GetUnitSpellCount(row) == high);
            assert(SzDuel_GetUnitCount(row) == low + high);
        }
    }
    assert(SzDuel_GetUnitCount(3) == 0);
}

static void check_accessors(void)
{
    struct DotrStateItemSource source;
    struct DotrStateItem *item = &gSzDuel_aaVarInfo[1][7];
    int value, flag, want;
    memset(gSzDuel_aaVarInfo, 0, sizeof(gSzDuel_aaVarInfo));
    memset(&source, 0, sizeof(source));
    /* Rejected lookups. */
    assert(SzDuel_GetUnitConstKind(3, 0) == -1 && SzDuel_GetUnitKindOfs(3, 0) == -1);
    assert(SzDuel_GetUnitKind(3, 0) == -1 && SzDuel_GetUnitAttr(3, 0) == -1);
    assert(SzDuel_SetUnitKind(3, 0, 1) == 0 && SzDuel_SetUnitAttr(3, 0, 1) == 0);
    assert(SzDuel_GetUnitLevel(3, 0, 0) == -1 && SzDuel_GetUnitEffect(3, 0, 0) == 0);
    /* Null source word. */
    assert(SzDuel_GetUnitConstKind(1, 7) == -1 && SzDuel_GetUnitKindOfs(1, 7) == -1 && SzDuel_GetUnitLevel2(item, 0) == -1);
    assert(SzDuel_GetUnitLevel2(0, 0) == -1);
    item->constInfo = &source;
    source.kind = 0xfe;
    source.kindOfs = 0x81;
    assert(SzDuel_GetUnitConstKind(1, 7) == 0xfe && SzDuel_GetUnitKindOfs(1, 7) == 0x81);
    for (value = 0; value < 256; value += 15) {
        assert(SzDuel_SetUnitKind(1, 7, value + 256) == 1 && item->kind == value);
        assert(SzDuel_GetUnitKind(1, 7) == value);
        assert(SzDuel_SetUnitAttr(1, 7, value - 512) == 1 && item->attr == value);
        assert(SzDuel_GetUnitAttr(1, 7) == value);
    }
    /* High nibble of source byte 2, less item byte +0x48 unless flagged,
     * clamped to 0-12. */
    for (value = 0; value < 16; value++) {
        for (flag = 0; flag < 2; flag++) {
            source.unknown_02_low = 0xf;
            source.unknown_02_high = (unsigned char)value;
            item->lvRev = 3;
            want = value - (flag ? 0 : 3);
            want = want < 0 ? 0 : want > 12 ? 12 : want;
            assert(SzDuel_GetUnitLevel2(item, flag) == want && SzDuel_GetUnitLevel(1, 7, flag) == want);
        }
    }
    item->lvRev = 255;
    assert(SzDuel_GetUnitLevel2(item, 0) == 0);
    /* Embedded eight-byte entries. */
    for (value = 0; value < 5; value++) assert(SzDuel_GetUnitEffect(1, 7, value) == &item->cardEffects[value]);
    assert(SzDuel_GetUnitEffect(1, 7, -1) == 0 && SzDuel_GetUnitEffect(1, 7, 5) == 0);
    /* Tail wrappers forward the checked item and append a zero argument. */
    tail_calls = 0;
    assert(SzDuel_GetUnitLeaderAbility2(item, 5, -6) == 0x4567);
    assert(tail_calls == 1 && tail_item == item && tail_args[0] == 5 && tail_args[1] == -6 && tail_args[2] == 0);
    assert(SzDuel_GetUnitLeaderAbility(1, 7, 8, 9) == 0x4567 && tail_item == item && tail_args[0] == 8 && tail_args[1] == 9);
    assert(SzDuel_GetUnitLeaderAbility(3, 0, 1, 2) == 0x4567 && tail_item == 0 && tail_calls == 3);
}

static void check_item_arithmetic(void)
{
    static const int stored[] = {-9999, -1, 0, 7, 9999};
    static const int operands[] = {-20000, -3, 0, 1, 2, 5000, 20000};
    static const int bad_operations[] = {INT_MIN, -1, 5, INT_MAX};
    struct DotrStateItem *item = &gSzDuel_aaVarInfo[0][3];
    size_t n, o, b;
    int operation, r;
    memset(gSzDuel_aaVarInfo, 0, sizeof(gSzDuel_aaVarInfo));
    assert(SzDuel_CalcUnitAP2(0, 0, 1) == 0 && SzDuel_CalcUnitDP2(0, 4, 1) == 0);
    assert(SzDuel_CalcUnitAP(3, 0, 4, 1) == 0 && SzDuel_CalcUnitDP(0, 11, 4, 1) == 0);
    for (n = 0; n < sizeof(stored) / sizeof(stored[0]); n++) {
        for (o = 0; o < sizeof(operands) / sizeof(operands[0]); o++) {
            for (b = 0; b < sizeof(bad_operations) / sizeof(bad_operations[0]); b++) {
                item->apBoost = item->dpBoost = (short)stored[n];
                assert(SzDuel_CalcUnitAP2(item, bad_operations[b], operands[o]) == 0);
                assert(SzDuel_CalcUnitDP2(item, bad_operations[b], operands[o]) == 0);
                assert(item->apBoost == stored[n] && item->dpBoost == stored[n]);
            }
            for (operation = 0; operation < 5; operation++) {
                if (operation == 3 && operands[o] == 0) continue;
                r = operation == 0 ? stored[n] + operands[o] : operation == 1 ? stored[n] - operands[o]
                  : operation == 2 ? stored[n] * operands[o] : operation == 3 ? stored[n] / operands[o] : operands[o];
                r = r < -9999 ? -9999 : r > 9999 ? 9999 : r;
                /* Each routine writes only its own halfword. */
                item->apBoost = (short)stored[n];
                item->dpBoost = 1234;
                assert(SzDuel_CalcUnitAP2(item, operation, operands[o]) == 1);
                assert(item->apBoost == r && item->dpBoost == 1234);
                item->apBoost = (short)stored[n];
                assert(SzDuel_CalcUnitAP(0, 3, operation, operands[o]) == 1 && item->apBoost == r);
                item->dpBoost = (short)stored[n];
                item->apBoost = 4321;
                assert(SzDuel_CalcUnitDP2(item, operation, operands[o]) == 1);
                assert(item->dpBoost == r && item->apBoost == 4321);
                item->dpBoost = (short)stored[n];
                assert(SzDuel_CalcUnitDP(0, 3, operation, operands[o]) == 1 && item->dpBoost == r);
            }
        }
    }
}

static void check_selection(void)
{
    struct DotrStateItemSource source;
    struct DotrStateItem *item = &gSzDuel_aaVarInfo[2][9], *other = &gSzDuel_aaVarInfo[0][0];
    struct DotrStateCell *home = &gSzDuel_aaMapInfo[4][2];
    int n;
    memset(gSzDuel_aaVarInfo, 0, sizeof(gSzDuel_aaVarInfo));
    memset(gSzDuel_aaMapInfo, 0, sizeof(gSzDuel_aaMapInfo));
    memset(&source, 0, sizeof(source));
    assert(SzDuel_GetUnitConstEffect2(0, 0) == 0);
    assert(SzDuel_GetUnitConstEffect2(item, 0) == 0); /* null source word */
    item->constInfo = &source;
    assert(SzDuel_GetUnitConstEffect2(item, -1) == 0 && SzDuel_GetUnitConstEffect2(item, 5) == 0);
    item->locCol = 2;
    item->locRow = 4;
    item->cardNo = 853;
    source.effectId = 9;
    for (n = 0; n < 5; n++) {
        /* The cell at the item's own position refers back to it and byte
         * +0x45 is zero: the table indexed by item halfword +0x3c is used. */
        home->unit = (DotrStateWord)item;
        item->slot = 0;
        item->cardNo = 853;
        assert(SzDuel_GetUnitConstEffect2(item, n) == &EffIdx_L[853][n]);
        item->cardNo = 854;
        assert(SzDuel_GetUnitConstEffect2(item, n) == 0);
        item->cardNo = 671;
        assert(SzDuel_GetUnitConstEffect2(item, n) == 0);
        /* Otherwise the source halfword +4 selects by byte +0x45. */
        item->slot = 5;
        assert(SzDuel_GetUnitConstEffect2(item, n) == &EffIdx_M[9][n]);
        item->slot = 6;
        assert(SzDuel_GetUnitConstEffect2(item, n) == &EffIdx_S[9]);
        item->slot = 0;
        home->unit = (DotrStateWord)other;
        assert(SzDuel_GetUnitConstEffect2(item, n) == &EffIdx_M[9][n]);
        /* A position outside the grid behaves like a non-matching cell. */
        item->locCol = 7;
        home->unit = (DotrStateWord)item;
        assert(SzDuel_GetUnitConstEffect2(item, n) == &EffIdx_M[9][n]);
        item->locCol = 2;
    }
    source.effectId = 0xffff;
    item->slot = 6;
    assert(SzDuel_GetUnitConstEffect2(item, 0) == 0);
}

typedef int (*Getter)(int, int);
typedef int (*Setter)(int, int, int);

static void check_byte_table(void)
{
    static const struct { Getter get; Setter set; size_t offset; } bytes[] = {
        {SzDuel_GetUnitModelID, 0, offsetof(struct DotrStateItem, modelId)},
        {SzDuel_GetUnitSummonOrder, 0, offsetof(struct DotrStateItem, summonOrder)},
        {SzDuel_GetUnitCurseRest, SzDuel_SetUnitCurseRest, offsetof(struct DotrStateItem, curseRest)},
        {SzDuel_GetUnitSpellRest, SzDuel_SetUnitSpellRest, offsetof(struct DotrStateItem, spellRest)},
        {SzDuel_GetUnitDelayCount, SzDuel_SetUnitDelayCount, offsetof(struct DotrStateItem, delayCount)},
        {0, SzDuel_SetUnitCtlModeC, offsetof(struct DotrStateItem, ctlModeC)},
        {0, SzDuel_SetUnitCtlModeM, offsetof(struct DotrStateItem, unknown_4f)},
        {SzDuel_GetUnitCtlGoalC, SzDuel_SetUnitCtlGoalC, offsetof(struct DotrStateItem, ctlGoalC)},
        {SzDuel_GetUnitCtlGoalM, SzDuel_SetUnitCtlGoalM, offsetof(struct DotrStateItem, ctlGoalM)},
        {SzDuel_GetUnitCtlRateC, 0, offsetof(struct DotrStateItem, ctlRateC)},
        {SzDuel_GetUnitCtlRateM, 0, offsetof(struct DotrStateItem, ctlRateM)},
        {SzDuel_GetUnitOpenRate, 0, offsetof(struct DotrStateItem, openRate)},
    };
    struct DotrStateItem *item = &gSzDuel_aaVarInfo[1][2];
    struct DotrStateItem expected;
    size_t i;
    int value;
    for (i = 0; i < sizeof(bytes) / sizeof(bytes[0]); i++) {
        unsigned char *field = (unsigned char *)item + bytes[i].offset;
        memset(gSzDuel_aaVarInfo, 0, sizeof(gSzDuel_aaVarInfo));
        for (value = 0; value < 256; value += 17) {
            if (bytes[i].set) {
                /* Only the low byte is stored, and only in this field. */
                memcpy(&expected, item, sizeof(expected));
                ((unsigned char *)&expected)[bytes[i].offset] = (unsigned char)value;
                assert(bytes[i].set(1, 2, value + 0x300) == 1);
                assert(memcmp(&expected, item, sizeof(expected)) == 0);
            } else {
                *field = (unsigned char)value;
            }
            if (bytes[i].get) assert(bytes[i].get(1, 2) == value);
        }
        if (bytes[i].get) assert(bytes[i].get(3, 0) == -1);
        if (bytes[i].set) assert(bytes[i].set(3, 0, 1) == 0);
    }
    /* Halfword getter and the packed position. */
    item->cardNo = 0xfedc;
    item->locCol = 0x12;
    item->locRow = 0xab;
    assert(SzDuel_GetUnitCardNo(1, 2) == 0xfedc && SzDuel_GetUnitCardNo(3, 0) == -1);
    assert(SzDuel_GetUnitLoc(1, 2) == 0xab12 && SzDuel_GetUnitLoc(3, 0) == -1);
    /* Both-setter wrappers succeed or fail together with the lookup. */
    assert(SzDuel_SetUnitCtlGoal(1, 2, 0x77) == 1 && item->ctlGoalC == 0x77 && item->ctlGoalM == 0x77);
    assert(SzDuel_SetUnitCtlGoal(3, 0, 1) == 0);
}

static void check_bit_fields(void)
{
    struct DotrStateItem *item = &gSzDuel_aaVarInfo[0][5];
    struct DotrStateItem expected;
    int pair, flag;
    memset(gSzDuel_aaVarInfo, 0, sizeof(gSzDuel_aaVarInfo));
    for (flag = -2; flag < 3; flag++) {
        memset(item, 0xff, sizeof(*item));
        item->slot = 0;
        memcpy(&expected, item, sizeof(expected));
        expected.modelStatusFlags.unknown_bit_3 = flag ? 1 : 0;
        assert(SzDuel_SetUnitCurseFlag(0, 5, flag) == 1);
        assert(memcmp(&expected, item, sizeof(expected)) == 0);
        assert(SzDuel_GetUnitCurseFlag(0, 5) == (flag ? 1 : 0));
    }
    assert(SzDuel_SetUnitCurseFlag(3, 0, 1) == 0 && SzDuel_GetUnitCurseFlag(3, 0) == 0);
    for (pair = -1; pair < 9; pair++) {
        /* Each routine replaces one two-bit field and, for a non-negative
         * value, one byte; a negative value leaves the byte alone. */
        memset(item, 0x5a, sizeof(*item));
        item->slot = 0;
        memcpy(&expected, item, sizeof(expected));
        expected.unknown_56.unknown_bits_0_1 = pair & 3;
        expected.ctlRateC = 0x21;
        assert(SzDuel_SetUnitCtlRateC(0, 5, pair, 0x121) == 1);
        assert(memcmp(&expected, item, sizeof(expected)) == 0);
        assert(SzDuel_SetUnitCtlRateC(0, 5, pair, -1) == 1);
        assert(memcmp(&expected, item, sizeof(expected)) == 0);
        expected.unknown_56.unknown_bits_2_3 = pair & 3;
        expected.ctlRateM = 0x22;
        assert(SzDuel_SetUnitCtlRateM(0, 5, pair, 0x22) == 1);
        assert(memcmp(&expected, item, sizeof(expected)) == 0);
        expected.unknown_56.unknown_bits_4_5 = pair & 3;
        expected.openRate = 0;
        assert(SzDuel_SetUnitOpenRate(0, 5, pair, 0) == 1);
        assert(memcmp(&expected, item, sizeof(expected)) == 0);
        assert(SzDuel_SetUnitOpenRate(0, 5, pair, INT_MIN) == 1);
        assert(memcmp(&expected, item, sizeof(expected)) == 0);
        /* The combined routine writes the first two fields and bytes. */
        expected.unknown_56.unknown_bits_0_1 = (pair + 1) & 3;
        expected.unknown_56.unknown_bits_2_3 = (pair + 1) & 3;
        expected.ctlRateC = expected.ctlRateM = 9;
        assert(SzDuel_SetUnitCtlRate(0, 5, pair + 1, 9) == 1);
        assert(memcmp(&expected, item, sizeof(expected)) == 0);
    }
    assert(SzDuel_SetUnitCtlRateC(3, 0, 1, 1) == 0 && SzDuel_SetUnitCtlRateM(3, 0, 1, 1) == 0);
    assert(SzDuel_SetUnitOpenRate(3, 0, 1, 1) == 0 && SzDuel_SetUnitCtlRate(3, 0, 1, 1) == 0);
}

static void check_probe_and_vector(void)
{
    static const unsigned results[] = {0, 1, 0xf, 0x10, 0x1f, 0xabc7, 0x80000000u, 0xffffffffu};
    struct DotrStateItem *item = &gSzDuel_aaVarInfo[2][1];
    struct DotrStateVector v;
    size_t i;
    int x, y, z;
    memset(gSzDuel_aaVarInfo, 0, sizeof(gSzDuel_aaVarInfo));
    item->locCol = 6;
    item->locRow = 3;
    probe_calls = 0;
    assert(SzDuel_CalcUnitMovable(3, 0) == 0 && SzDuel_IsUnitMovableOnLabyrinth(3, 0) == 0 && probe_calls == 0);
    for (i = 0; i < sizeof(results) / sizeof(results[0]); i++) {
        probe_result = results[i];
        assert(SzDuel_CalcUnitMovable(2, 1) == (int)(results[i] & 0xf));
        assert(probe_item == item && probe_x == 6 && probe_y == 3);
        assert(SzDuel_IsUnitMovableOnLabyrinth(2, 1) == ((results[i] >> 4) ? 1 : 0));
    }
    for (x = -1; x < 8; x++) {
        for (y = -1; y < 8; y++) {
            for (z = -2; z < 5; z++) {
                memset(&v, 0x7f, sizeof(v));
                SzDuel_CalcUnitPos(x, y, z, &v);
                assert(v.x == (float)((x >= 0 && x < 7) ? (x - 3) * 50 : 0));
                assert(v.z == (float)((y >= 0 && y < 7) ? (y - 3) * 50 : 0));
                assert(v.y == (float)(z * 35) / 128.0f && v.unknown_w == 1.0f);
            }
        }
    }
}

static void check_cell_fields(void)
{
    static const int bad[][2] = {{-1, 0}, {7, 0}, {0, -1}, {0, 7}, {INT_MIN, INT_MAX}};
    struct DotrStateCell expected[7][7];
    struct DotrStateItem *item = &gSzDuel_aaVarInfo[1][4];
    size_t i;
    int x, y, value, want;
    for (i = 0; i < sizeof(bad) / sizeof(bad[0]); i++) {
        memset(gSzDuel_aaMapInfo, 0x33, sizeof(gSzDuel_aaMapInfo));
        memcpy(expected, gSzDuel_aaMapInfo, sizeof(expected));
        assert(SzDuel_SetField(bad[i][0], bad[i][1], 1) == 0 && SzDuel_GetField(bad[i][0], bad[i][1]) == -1);
        assert(SzDuel_SetFieldRate(bad[i][0], bad[i][1], 1) == -1 && SzDuel_GetFieldRate(bad[i][0], bad[i][1]) == 0);
        assert(SzDuel_GetMapMovable(bad[i][0], bad[i][1]) == 255 && SzDuel_SetMapAlpha(bad[i][0], bad[i][1], 1) == 0);
        assert(SzDuel_GetMapUnit(bad[i][0], bad[i][1]) == 0 && SzDuel_GetMapUnitID(bad[i][0], bad[i][1]) == -1);
        assert(memcmp(expected, gSzDuel_aaMapInfo, sizeof(expected)) == 0);
    }
    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            struct DotrStateCell *c = &gSzDuel_aaMapInfo[y][x];
            memset(gSzDuel_aaMapInfo, 0, sizeof(gSzDuel_aaMapInfo));
            memset(gSzDuel_aaVarInfo, 0, sizeof(gSzDuel_aaVarInfo));
            memcpy(expected, gSzDuel_aaMapInfo, sizeof(expected));
            expected[y][x].field = 0x9c;
            assert(SzDuel_SetField(x, y, 0x19c) == 1 && SzDuel_GetField(x, y) == 0x9c);
            expected[y][x].mapAlpha = 0x81;
            assert(SzDuel_SetMapAlpha(x, y, -127) == 1);
            assert(memcmp(expected, gSzDuel_aaMapInfo, sizeof(expected)) == 0);
            c->mapMovable = 0xfe;
            assert(SzDuel_GetMapMovable(x, y) == 0xfe);
            for (value = -300; value <= 300; value += 43) {
                want = value < -128 ? -128 : value > 128 ? 128 : value;
                assert(SzDuel_SetFieldRate(x, y, value) == want && c->fieldRate == want);
                assert(SzDuel_GetFieldRate(x, y) == want);
            }
            /* Item referenced by the first cell word. */
            assert(SzDuel_GetMapUnit(x, y) == 0 && SzDuel_GetMapUnitID(x, y) == -1);
            c->unit = (DotrStateWord)item;
            item->team = 0xa1;
            item->slot = 10;
            assert(SzDuel_GetMapUnit(x, y) == item && SzDuel_GetMapUnitID(x, y) == 0xa10a);
            item->slot = 11;
            assert(SzDuel_GetMapUnit(x, y) == 0 && SzDuel_GetMapUnitID(x, y) == -1);
        }
    }
    /* The reset sweep stores zero in the halfword of every cell. */
    memset(gSzDuel_aaMapInfo, 0x33, sizeof(gSzDuel_aaMapInfo));
    memcpy(expected, gSzDuel_aaMapInfo, sizeof(expected));
    for (y = 0; y < 7; y++) for (x = 0; x < 7; x++) expected[y][x].fieldRate = 0;
    SzDuel_InitFieldRate();
    assert(memcmp(expected, gSzDuel_aaMapInfo, sizeof(expected)) == 0);

    /* Sweeps and thunks around SzDuel_SetMapColor2 (stubbed here). */
    sweep_calls = 0;
    sweep_extra[0] = 11;
    sweep_extra[1] = -12;
    sweep_extra[2] = 13;
    sweep_extra[3] = -14;
    SzDuel_InitMapColor2(11, -12, 13, -14);
    assert(sweep_calls == 49);
    sweep_calls = 0;
    sweep_extra[3] = 1;
    SzDuel_InitMapColor(11, -12, 13);
    assert(sweep_calls == 49);
    sweep_calls = -1;
    SzDuel_SetMapColor(21, 22, 23, 24, 25);
    assert(sweep_last[0] == 21 && sweep_last[1] == 22 && sweep_last[2] == 23);
    assert(sweep_last[3] == 24 && sweep_last[4] == 25 && sweep_last[5] == 1);
}

static void check_item_flags_and_object(void)
{
    struct DotrStateItem *item = &gSzDuel_aaVarInfo[2][10];
    struct DotrStateItem expected;
    struct DotrStateItemObject object, want;
    struct DotrStateVector in = {1.5f, -2.5f, 3.25f, 4.0f}, out;
    struct DotrStateCellQuad quad_in = {1, 2, 3, 4}, quad_out;
    int flag, fill;
    memset(gSzDuel_aaVarInfo, 0, sizeof(gSzDuel_aaVarInfo));
    for (fill = 0; fill < 2; fill++) {
        for (flag = -1; flag < 2; flag++) {
            /* Each routine changes only its named bits. */
            memset(item, fill ? 0xff : 0, sizeof(*item));
            item->slot = 0;
            memcpy(&expected, item, sizeof(expected));
            expected.status.unknown_bit_7 = flag ? 1 : 0;
            assert(SzDuel_SetCardBusyFlag(2, 10, flag) == 1 && memcmp(&expected, item, sizeof(expected)) == 0);
            expected.modelStatusFlags.unknown_bit_1 = flag ? 1 : 0;
            assert(SzDuel_SetCardColBusyFlag(2, 10, flag) == 1 && memcmp(&expected, item, sizeof(expected)) == 0);
            expected.modelStatusFlags.unknown_bit_0 = flag ? 1 : 0;
            assert(SzDuel_SetModelBusyFlag(2, 10, flag) == 1 && memcmp(&expected, item, sizeof(expected)) == 0);
            expected.status.unknown_bit_7 = flag ? 0 : 1;
            expected.modelStatusFlags.unknown_bit_0 = flag ? 0 : 1;
            assert(SzDuel_SetBusyFlag(2, 10, !flag) == 1 && memcmp(&expected, item, sizeof(expected)) == 0);
        }
    }
    assert(SzDuel_SetBusyFlag(3, 0, 1) == 0 && SzDuel_SetCardBusyFlag(3, 0, 1) == 0);
    assert(SzDuel_SetCardColBusyFlag(3, 0, 1) == 0 && SzDuel_SetModelBusyFlag(3, 0, 1) == 0);

    /* Object accessors: rejected lookup or null object word returns 0. */
    memset(item, 0, sizeof(*item));
    assert(SzDuel_SetCardPos(3, 0, &in) == 0 && SzDuel_GetCardPos(2, 10, &out) == 0);
    assert(SzDuel_SetCardRgba(2, 10, &quad_in) == 0 && SzDuel_GetCardRgba(3, 0, &quad_out) == 0);
    item->cardModel = &object;
    memset(&object, 0x6b, sizeof(object));
    memcpy(&want, &object, sizeof(want));
    /* A null vector pointer succeeds without copying. */
    assert(SzDuel_SetCardPos(2, 10, 0) == 1 && SzDuel_SetCardRot(2, 10, 0) == 1 && SzDuel_SetCardScale(2, 10, 0) == 1);
    assert(SzDuel_GetCardPos(2, 10, 0) == 1 && SzDuel_GetCardRot(2, 10, 0) == 1 && SzDuel_GetCardScale(2, 10, 0) == 1);
    assert(SzDuel_SetCardRgba(2, 10, 0) == 1 && SzDuel_GetCardRgba(2, 10, 0) == 1);
    assert(memcmp(&want, &object, sizeof(want)) == 0);
    want.unknown_00 = in;
    assert(SzDuel_SetCardPos(2, 10, &in) == 1 && memcmp(&want, &object, sizeof(want)) == 0);
    assert(SzDuel_GetCardPos(2, 10, &out) == 1 && memcmp(&out, &in, sizeof(in)) == 0);
    in.x = 9.0f;
    want.unknown_10 = in;
    assert(SzDuel_SetCardRot(2, 10, &in) == 1 && memcmp(&want, &object, sizeof(want)) == 0);
    assert(SzDuel_GetCardRot(2, 10, &out) == 1 && memcmp(&out, &in, sizeof(in)) == 0);
    in.unknown_w = -7.0f;
    want.unknown_30 = in;
    assert(SzDuel_SetCardScale(2, 10, &in) == 1 && memcmp(&want, &object, sizeof(want)) == 0);
    assert(SzDuel_GetCardScale(2, 10, &out) == 1 && memcmp(&out, &in, sizeof(in)) == 0);
    want.unknown_40 = quad_in;
    assert(SzDuel_SetCardRgba(2, 10, &quad_in) == 1 && memcmp(&want, &object, sizeof(want)) == 0);
    assert(SzDuel_GetCardRgba(2, 10, &quad_out) == 1 && memcmp(&quad_out, &quad_in, sizeof(quad_in)) == 0);
}

static void check_control_and_triple(void)
{
    struct DotrStateControl expected[1];
    int flag;
    assert(offsetof(struct DotrStateControl, unknown_ca2) == 0xca2);
    assert(offsetof(struct DotrStateControl, unknown_cae) == 0xcae);
    assert(offsetof(struct DotrStateControl, unknown_cb2) == 0xcb2);
    assert(offsetof(struct DotrStateControl, unknown_cb5) == 0xcb5);
    assert(offsetof(struct DotrStateControl, unknown_d53) == 0xd53);
    assert(offsetof(struct DotrStateControl, unknown_d67) == 0xd67);
    memset(data_00317490_control, 0x42, sizeof(data_00317490_control));
    memcpy(expected, data_00317490_control, sizeof(expected));
    expected[0].unknown_cb4 = 18;
    expected[0].unknown_cae = (short)0x8123;
    expected[0].unknown_ca2 = -2;
    expected[0].unknown_ca4 = 3;
    SzDuel_OpenHandPopup(0x18123, -2, 3);
    assert(memcmp(expected, data_00317490_control, sizeof(expected)) == 0);
    expected[0].unknown_cb4 = 23;
    expected[0].unknown_cae = 4;
    expected[0].unknown_ca2 = 5;
    expected[0].unknown_ca4 = -6;
    SzDuel_OpenHandCursor(4, 5, -6);
    assert(memcmp(expected, data_00317490_control, sizeof(expected)) == 0);
    expected[0].unknown_cb4 = 24;
    SzDuel_CloseHandPopup();
    assert(memcmp(expected, data_00317490_control, sizeof(expected)) == 0);
    data_00317490_control[0].unknown_cb4 = 0;
    SzDuel_CloseHandCursor();
    assert(memcmp(expected, data_00317490_control, sizeof(expected)) == 0);
    expected[0].unknown_cb2 = 24;
    SzDuel_CloseOptionPopup();
    assert(memcmp(expected, data_00317490_control, sizeof(expected)) == 0);
    expected[0].unknown_cb5 = 24;
    SzDuel_CloseMsgPopup();
    assert(memcmp(expected, data_00317490_control, sizeof(expected)) == 0);
    for (flag = 0; flag < 4; flag++) {
        /* Only the low bit of the argument reaches the one-bit field. */
        expected[0].unknown_d67.unknown_bit_5 = flag;
        SzDuel_SetIgnoreJudge(flag);
        assert(memcmp(expected, data_00317490_control, sizeof(expected)) == 0);
    }

    gSzBattle_bl[0] = gSzBattle_bl[1] = gSzBattle_bl[2] = 0x55;
    fill_calls = 0;
    SzBattle_Initialize();
    assert(fill_calls == 1);
    assert(gSzBattle_bl[0] == -1 && gSzBattle_bl[1] == -1 && gSzBattle_bl[2] == 0);
    gSzBattle_bl[0] = 10;
    gSzBattle_bl[1] = 20;
    gSzBattle_bl[2] = 30;
    assert(SzBattle_InqCamera() == 30);
    assert(SzBattle_InqEntry(0) == 10 && SzBattle_InqEntry(1) == 20 && SzBattle_InqEntry(2) == 30);

    follow_calls = 0;
    gate_result = 0;
    assert(SzBattle_DrawStage_Pipe(77) == 77 && gate_argument == 8 && follow_calls == 0);
    gate_result = -1;
    assert(SzBattle_DrawStage_Pipe(78) == 0x1357 && follow_calls == 1 && follow_argument == 78);

    follow_calls = 0;
    gate_result = 0;
    assert(SzDuel_DrawStage_Pipe(61) == 61 && gate_argument == 0 && follow_calls == 0);
    gate_result = 1;
    assert(SzDuel_DrawStage_Pipe(62) == 0x2468 && gate_argument == 0 && follow_calls == 1 && follow_argument == 62);
}

static int flow_count, flow_log[16][5];
static int flow_ids[2], flow_pad_bits, flow_pad_argument, flow_gate_bits, flow_busy;

static void flow_record(int tag, int a, int b, int c, int d)
{
    assert(flow_count < 16);
    flow_log[flow_count][0] = tag;
    flow_log[flow_count][1] = a;
    flow_log[flow_count][2] = b;
    flow_log[flow_count][3] = c;
    flow_log[flow_count][4] = d;
    flow_count++;
}

static int flow_is(int n, int tag, int a, int b, int c, int d)
{
    const int *e = flow_log[n];
    return n < flow_count && e[0] == tag && e[1] == a && e[2] == b && e[3] == c && e[4] == d;
}

/* Recording stubs; their semantics are not modelled. */
void SzEffect_LoadModule(int a, int b, int c) { flow_record(1, a, b, c, 0); }
int SzModel_LoadMerge(int a, int b, int c) { flow_record(2, a, b, c, 0); return 1; }
int SzData_GetCurrentDeck(int index) { return 0x40 + index; }
int SzData_GetLeader(int index, int unknown)
{
    assert(unknown == 0x40 + index);
    return flow_ids[index];
}
int SzModel_Load(int a, int b, int c, int d) { flow_record(4, a, b, c, d); return 1; }
int SzPad_Check(int unknown, int zero, int mask)
{
    assert(unknown == flow_pad_argument && zero == 0);
    return flow_pad_bits & mask;
}
void SzSound_PlaySE(int zero, int code) { flow_record(5, zero, code, 0, 0); }
int SzGame_GetMenuNum(int unknown)
{
    assert(unknown == 15);
    return flow_busy;
}
int SzData_AddBag(int side, int key, int id) { flow_record(6, side, key, id, 0); return 1; }
int SzData_RollSpecial(int side, int id) { flow_record(7, side, id, 0, 0); return 1; }
int SzData_GetOption(int unknown_0, int unknown_1)
{
    assert(unknown_0 == 0);
    return ((flow_gate_bits >> unknown_1) & 1) * 77;
}
int SzModel_DrawWithOutCalc_Pipe(int cursor, int index, int c, void *d, int e, int f, int g)
{
    assert(c == 0 && e == 0 && f == 0 && g == 0);
    flow_record(8, cursor, index, d != 0, d ? ((int *)d)[1] : 0);
    if (d) {
        const float *v = (const float *)d;
        assert(v[0] == 1.0f && v[2] == 3.0f && v[3] == 4.0f);
    }
    return cursor + 0x100;
}
int SzModel_GetRot(int index, struct DotrObjectVector4 *out)
{
    flow_record(10, index, 0, 0, 0);
    out->x = 1.0f;
    out->y = 2.0f;
    out->z = 3.0f;
    out->unknown_w = 4.0f;
    return 1;
}
int lSzBattle_DrawFakeShadow(int cursor, int kind, int unknown, float x, float y)
{
    assert(unknown == -1 && x == -25.0f && y == 800.0f);
    flow_record(9, cursor, kind, 0, 0);
    return cursor + 0x1000;
}

static void check_flow(void)
{
    static const unsigned char codes[9] = {19, 15, 12, 13, 24, 20, 21, 22, 16};
    struct DotrStateControl expected;
    struct DotrStateControl *control = data_00317490_control;
    int operation, busy;

    /* 0x1ddd80: identifier 671 is remapped out of range and skipped. */
    memset(gSzDuel_CardConstInfo, 0, sizeof(gSzDuel_CardConstInfo));
    ((unsigned char *)&gSzDuel_CardConstInfo[5])[0] = 0xff;
    ((unsigned char *)&gSzDuel_CardConstInfo[853])[0] = 0x23;
    flow_ids[0] = 671;
    flow_ids[1] = 5;
    flow_count = 0;
    SzDuel_SetupDuel();
    assert(flow_count == 4 && flow_is(0, 1, 0, 620, 0, 0) && flow_is(1, 2, 6, 0, 0, 0));
    assert(flow_is(2, 2, 0, 0, 0, 0) && flow_is(3, 4, 6, -1, 0x1f + 100, 1));
    flow_ids[0] = 853;
    flow_ids[1] = -1;
    flow_count = 0;
    SzDuel_SetupDuel();
    assert(flow_count == 4 && flow_is(3, 4, 5, -1, 3, 1));

    /* 0x1e3ff0 */
    memset(control, 0xa5, sizeof(*control));
    expected = *control;
    flow_gate_bits = 7;
    SzDuel_OpenOptionPopup(-1, 0x12345, -2);
    expected.unknown_cb2 = 7;
    expected.unknown_caa = 0;
    expected.unknown_c9a = 0x2345;
    expected.unknown_c9c = -2;
    assert(memcmp(control, &expected, sizeof(expected)) == 0);
    flow_gate_bits = 5;
    SzDuel_OpenOptionPopup(0x103, 1, 2);
    expected.unknown_c9a = 1;
    expected.unknown_c9c = 2;
    expected.unknown_d54 = 3;
    expected.unknown_d35 = 0;
    expected.unknown_d36 = 5;
    assert(memcmp(control, &expected, sizeof(expected)) == 0);

    /* 0x1e42c0 */
    for (operation = -1; operation <= 9; operation++) {
        for (busy = 0; busy < 2; busy++) {
            static const int values[] = {-1, 0x0123, 0x8123};
            size_t v;
            for (v = 0; v < sizeof(values) / sizeof(values[0]); v++) {
                int value = values[v], stored = value;
                int valid = operation >= 0 && operation <= 8 && operation != 4;
                memset(control, 0xa5, sizeof(*control));
                control->unknown_d50 = 9;
                expected = *control;
                flow_busy = busy;
                flow_count = 0;
                SzDuel_OpenMsgPopup(operation, -3, 0x10004, value);
                expected.unknown_cb5 = valid ? codes[operation] : 24;
                if (operation == 7 && value >= 0) {
                    expected.unknown_d35 = (value & 0x8000) ? 1 : 0;
                    stored = value & 0x7fff;
                }
                if (valid) {
                    if (stored >= 0) expected.unknown_cb0 = (short)stored;
                    expected.unknown_ca6 = -3;
                    expected.unknown_ca8 = 4;
                }
                assert(memcmp(control, &expected, sizeof(expected)) == 0);
                if (operation == 2 && !busy) {
                    assert(flow_count == 1 && flow_is(0, 6, 9, 0, value, 0));
                } else if (operation == 3 && !busy) {
                    assert(flow_count == 2 && flow_is(0, 6, 9, 0, value, 0) && flow_is(1, 7, 9, value, 0, 0));
                } else {
                    assert(flow_count == 0);
                }
            }
        }
    }

    /* 0x1e4430 */
    flow_pad_argument = 0x55;
    memset(control, 0xa5, sizeof(*control));
    control->unknown_d35 = 0;
    expected = *control;
    flow_pad_bits = 0x8000;
    flow_count = 0;
    assert(SzDuel_ActMsgPopup(0x55) == 0 && flow_count == 1 && flow_is(0, 5, 0, 3, 0, 0));
    expected.unknown_d35 = 1;
    assert(memcmp(control, &expected, sizeof(expected)) == 0);
    flow_pad_bits = 0xa0c0;
    flow_count = 0;
    assert(SzDuel_ActMsgPopup(0x55) == 0 && flow_count == 0);
    flow_pad_bits = 0x2020;
    assert(SzDuel_ActMsgPopup(0x55) == 0 && flow_count == 1 && flow_is(0, 5, 0, 3, 0, 0));
    expected.unknown_d35 = 0;
    assert(memcmp(control, &expected, sizeof(expected)) == 0);
    flow_pad_bits = 0x60;
    flow_count = 0;
    assert(SzDuel_ActMsgPopup(0x55) == 2 && flow_count == 1 && flow_is(0, 5, 0, 4, 0, 0));
    control->unknown_d35 = 3;
    flow_pad_bits = 0x80;
    assert(SzDuel_ActMsgPopup(0x55) == 1 && flow_count == 2 && flow_is(1, 5, 0, 4, 0, 0));
    flow_pad_bits = 0x20;
    flow_count = 0;
    assert(SzDuel_ActMsgPopup(0x55) == 2 && flow_count == 1 && flow_is(0, 5, 0, 5, 0, 0));
    flow_pad_bits = 0x1f1f;
    flow_count = 0;
    assert(SzDuel_ActMsgPopup(0x55) == 0 && flow_count == 0);

    /* 0x1e5d30 */
    flow_count = 0;
    assert(lSzBattle_DrawWasteland_Pipe(0x1000, 12, 9) == 0x1100 && flow_count == 1 && flow_is(0, 8, 0x1000, 12, 0, 0));
    flow_count = 0;
    assert(lSzBattle_DrawWasteland_Pipe(0x1000, 12, 8) == 0x1200 && flow_count == 3);
    assert(flow_is(0, 8, 0x1000, 12, 0, 0) && flow_is(1, 10, 12, 0, 0, 0) && flow_is(2, 8, 0x1100, 12, 1, 0x8000));

    /* 0x1e5dd0 */
    memset(gSzDuel_aaVarInfo, 0, sizeof(gSzDuel_aaVarInfo));
    gSzDuel_aaVarInfo[1][2].modelId = 9;
    gSzDuel_aaVarInfo[2][10].slot = 11; /* rejected by the checked lookup */
    gSzBattle_bl[0] = -1;
    gSzBattle_bl[1] = (1 << 8) | 2;
    flow_count = 0;
    assert(lSzBattle_DrawGrass_Pipe(0x10, 4, 1) == 0x110 && flow_count == 1 && flow_is(0, 8, 0x10, 4, 0, 0));
    flow_count = 0;
    assert(lSzBattle_DrawGrass_Pipe(0x10, 4, 0) == 0x2110 && flow_count == 3);
    assert(flow_is(1, 9, 0x110, 7, 0, 0) && flow_is(2, 9, 0x1110, 9, 0, 0));
    gSzBattle_bl[0] = (2 << 8) | 10;
    gSzBattle_bl[1] = (3 << 8) | 0;
    flow_count = 0;
    assert(lSzBattle_DrawGrass_Pipe(0x10, 4, 0) == 0x1110 && flow_count == 2);
}

union DotrStateBlock16 data_002921c0[1];
static int seq_count, seq_log[16][3];
static float seq_vectors[2][4];
static int seq_words[4];

static int seq_record(int tag, int a, int b)
{
    assert(seq_count < 16);
    seq_log[seq_count][0] = tag;
    seq_log[seq_count][1] = a;
    seq_log[seq_count][2] = b;
    seq_count++;
    return 1;
}

/* Object-table routines (defined in their own group), stubbed as recorders here. */
int SzModel_GetModelNo(int index) { seq_record(1, index, 0); return 0x61; }
void SzSound_TransferSEGrp(int a, int b, int c) { seq_record(2, a, b); assert(c == 0); }
int SzModel_SetRot(int index, struct DotrObjectVector4 *in)
{
    memcpy(seq_words, in, sizeof(seq_words));
    return seq_record(3, index, 0);
}
int SzModel_GetPos(int index, struct DotrObjectVector4 *out)
{
    out->x = 11.0f;
    out->y = 12.0f;
    out->z = 13.0f;
    out->unknown_w = 14.0f;
    return seq_record(4, index, 0);
}
int SzModel_SetPos(int index, struct DotrObjectVector4 *in)
{
    memcpy(seq_vectors[1], in, sizeof(seq_vectors[1]));
    return seq_record(5, index, 0);
}
int SzModel_SetDrawFlag(int index, int value) { return seq_record(6, index, value); }
int SzModel_SetDeform(int index, int value) { return seq_record(7, index, value); }
int SzModel_SetDeformRatio(int index, int value) { return seq_record(8, index, value); }
int SzModel_SetAnimDirect(int index, int slot) { return seq_record(9, index, slot); }
int SzModel_SetAnimDir(int index, int value) { return seq_record(10, index, value); }

static void check_side_setup(void)
{
    static const int tags[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    struct DotrStateItemSource source;
    struct DotrStateItem *item = &gSzDuel_aaVarInfo[2][10];
    int side, n;
    memset(&source, 0, sizeof(source));
    memset(data_002921c0, 0, sizeof(data_002921c0));
    for (side = 0; side < 2; side++) {
        memset(gSzDuel_aaVarInfo, 0, sizeof(gSzDuel_aaVarInfo));
        gSzBattle_bl[0] = gSzBattle_bl[1] = gSzBattle_bl[2] = -7;
        /* Null first word: nothing happens. */
        item->modelId = 33;
        seq_count = 0;
        SzBattle_Entry(side, (2 << 8) | 10);
        assert(seq_count == 0 && gSzBattle_bl[side] == -7);
        /* Rejected lookup. */
        SzBattle_Entry(side, (3 << 8) | 10);
        assert(seq_count == 0);
        item->constInfo = &source;
        SzBattle_Entry(side, (2 << 8) | 10);
        assert(seq_count == 10);
        for (n = 0; n < 9; n++) {
            assert(seq_log[n][0] == tags[n]);
        }
        assert(seq_log[0][1] == 33 && seq_log[1][1] == side + 2 && seq_log[1][2] == 0x61);
        assert(seq_words[0] == 0 && seq_words[1] == (side ? 0 : 0x8000) && seq_words[2] == 0 && seq_words[3] == 0);
        assert(seq_vectors[1][0] == 0.0f && seq_vectors[1][1] == 12.0f);
        assert(seq_vectors[1][2] == (side ? -80.0f : 80.0f) && seq_vectors[1][3] == 14.0f);
        assert(seq_log[5][1] == 33 && seq_log[5][2] == 1 && seq_log[6][2] == 0 && seq_log[7][2] == 0);
        assert(seq_log[8][2] == 0 && seq_log[9][0] == 10 && seq_log[9][2] == 1);
        assert(gSzBattle_bl[side] == ((2 << 8) | 10) && gSzBattle_bl[!side] == -7 && gSzBattle_bl[2] == -7);
    }
}

static void check_ticker(void)
{
    static const unsigned char periods[] = {1, 2, 3, 5, 255};
    struct DotrGridTicker ticker;
    struct DotrStateControl *control = data_00317490_control;
    unsigned i, period;
    int tick;

    memset(&ticker, 0, sizeof(ticker));
    for (i = 0; i < sizeof(periods); i++) {
        period = periods[i];
        for (tick = 0; tick <= (int)period + 1 && tick < 8; tick++) {
            int next = tick + 1 > (int)period ? 0 : tick + 1;
            memset(control, 0, sizeof(*control));
            control[0].unknown_cb4 = 99;
            ticker.tick = (unsigned short)tick;
            flow_count = 0;
            TaTuto_DrawFinger(&ticker, (unsigned char)period, 4, 5, -6);
            assert(ticker.tick == next);
            /* Sound call (0, 19) precedes the branch only on a multiple of the period. */
            if (tick % period == 0) {
                assert(flow_count == 1 && flow_is(0, 5, 0, 19, 0, 0));
            } else {
                assert(flow_count == 0);
            }
            if (tick % period < period / 2) { /* 0x1e4540 */
                assert(control[0].unknown_cb4 == 23 && control[0].unknown_cae == 4);
                assert(control[0].unknown_ca2 == 5 && control[0].unknown_ca4 == -6);
            } else { /* 0x1e4e50 */
                assert(control[0].unknown_cb4 == 24 && control[0].unknown_cae == 0);
            }
        }
    }
    /* A tick above the period wraps to 0; equal to the period it does not (tick 5 -> 6 > 5). */
    ticker.tick = 4;
    TaTuto_DrawFinger(&ticker, 5, 0, 0, 0);
    assert(ticker.tick == 5);
    TaTuto_DrawFinger(&ticker, 5, 0, 0, 0);
    assert(ticker.tick == 0);
    /* Period 255 keeps a large tick: 254 -> 255 is not above the period. */
    ticker.tick = 254;
    TaTuto_DrawFinger(&ticker, 255, 0, 0, 0);
    assert(ticker.tick == 255);
}

static int count_cells(int x, int y)
{
    struct DotrGridTicker2 t;
    int n = 0, i, j;

    memset(&t, 0, sizeof(t));
    cell_count = 0;
    TaTuto_DrawTrapArea(&t, 1, x, y);
    assert(t.tick == 1); /* 0 -> 1 is not above period 1 */
    for (i = 0; i <= 6; i++) {
        for (j = 0; j <= 6; j++) {
            int d = (x > i ? x - i : i - x) + (y > j ? y - j : j - y);
            if (d < 2) {
                assert(cell_log[n][0] == i && cell_log[n][1] == j);
                n++;
            }
        }
    }
    assert(cell_count == n);
    return n;
}

static void check_ticker2(void)
{
    struct DotrGridTicker2 t;
    int k;

    memset(&t, 0, sizeof(t));
    /* tick % period != 0: no cells, tick incremented. */
    t.tick = 1;
    cell_count = 0;
    TaTuto_DrawTrapArea(&t, 3, 3, 3);
    assert(cell_count == 0 && t.tick == 2);
    /* tick % period == 0: five cells around (3,3) with c=0, e=1. */
    t.tick = 3;
    cell_count = 0;
    TaTuto_DrawTrapArea(&t, 3, 3, 3);
    assert(cell_count == 5 && t.tick == 0);
    for (k = 0; k < 5; k++) {
        assert(cell_log[k][2] == 0 && cell_log[k][3] == 1);
    }
    assert(cell_log[0][0] == 2 && cell_log[0][1] == 3 && cell_log[2][0] == 3 && cell_log[2][1] == 3);
    /* Corner gives 3 cells; far outside gives none. */
    cell_count = 0;
    t.tick = 0;
    TaTuto_DrawTrapArea(&t, 2, 0, 0);
    assert(cell_count == 3 && t.tick == 1);
    cell_count = 0;
    t.tick = 0;
    TaTuto_DrawTrapArea(&t, 2, -5, -5);
    assert(cell_count == 0);
    /* Tick above the period wraps to 0; equal to the period it stays. */
    t.tick = 4;
    TaTuto_DrawTrapArea(&t, 5, -5, -5);
    assert(t.tick == 5);
    TaTuto_DrawTrapArea(&t, 5, -5, -5);
    assert(t.tick == 0);
    assert(count_cells(6, 6) == 3);
    cell_count = -1;
}

static void check_flag_query(void)
{
    struct DotrStateItem *item = &gSzDuel_aaVarInfo[2][5];
    int value;

    memset(gSzDuel_aaVarInfo, 0, sizeof(gSzDuel_aaVarInfo));
    memset(EnchMatch, 0, sizeof(EnchMatch));
    /* a < 2 with b == 0 short-circuits before any lookup; a == 2 does not */
    item->kind = 2;
    gSzDuel_aaVarInfo[1][0].kind = 2;
    gSzDuel_aaVarInfo[2][0].kind = 2;
    assert(Do_ChkEncMatch(1, 0, 772) == 0 && Do_ChkEncMatch(0, 0, 772) == 0);
    assert(Do_ChkEncMatch(2, 0, 772) == 1);
    /* c == 772: only kind 2 with no high bits */
    for (value = 0; value < 256; value++) {
        item->kind = value;
        assert(Do_ChkEncMatch(2, 5, 772) == (value == 2));
    }
    item->kind = 2;
    item->slot = 11;
    assert(Do_ChkEncMatch(2, 5, 772) == 0);
    item->slot = 0;
    /* other c: bit (c - 752) of row unknown_3c * 7 */
    item->cardNo = 2;
    EnchMatch[14] = 0x01;
    EnchMatch[15] = 0x81;
    EnchMatch[16] = 0x08;
    assert(Do_ChkEncMatch(2, 5, 752) == 1 && Do_ChkEncMatch(2, 5, 753) == 0 && Do_ChkEncMatch(2, 5, 759) == 0);
    assert(Do_ChkEncMatch(2, 5, 760) == 1 && Do_ChkEncMatch(2, 5, 761) == 0 && Do_ChkEncMatch(2, 5, 767) == 1);
    assert(Do_ChkEncMatch(2, 5, 771) == 1 && Do_ChkEncMatch(2, 5, 768) == 0);
    item->cardNo = 0;
    EnchMatch[0] = 0x80;
    assert(Do_ChkEncMatch(2, 5, 759) == 1 && Do_ChkEncMatch(2, 5, 752) == 0);
    item->cardNo = 3;
    EnchMatch[21] = 0x10;
    assert(Do_ChkEncMatch(2, 5, 756) == 1 && Do_ChkEncMatch(2, 5, 755) == 0);
}

/* Sets the 0x250ca0 fixture: the counter reads base + adjust (flag 0) or base (flag 1). */
static struct DotrStateItemSource counter_source;
static struct DotrStateItem *counter_item(int base, int adjust)
{
    struct DotrStateItem *item = &gSzDuel_aaVarInfo[0][3];

    memset(gSzDuel_aaVarInfo, 0, sizeof(gSzDuel_aaVarInfo));
    memset(&counter_source, 0, sizeof(counter_source));
    item->constInfo = &counter_source;
    counter_source.apWithFlags = base;
    item->apBoost = adjust;
    return item;
}

static void check_counter_update(void)
{
    struct DotrStateItem *item;

    /* add: below the limit stores the sum; at/over it pins the counter at 9999 */
    item = counter_item(100, 50);
    assert(TaUtil_CalcUnitAP(0, 3, 0, 100) == 1 && item->apBoost == 150);
    item = counter_item(100, 50);
    assert(TaUtil_CalcUnitAP(0, 3, 0, 9849) == 1 && item->apBoost == 9899);
    item = counter_item(100, 50);
    assert(TaUtil_CalcUnitAP(0, 3, 0, 9900) == 1 && item->apBoost == 9899);
    item = counter_item(8000, 5000); /* read clamps to 9999 before the sum */
    assert(TaUtil_CalcUnitAP(0, 3, 0, 1) == 1 && item->apBoost == 1999);
    /* subtract: non-negative result subtracts; negative pins the counter at 0 */
    item = counter_item(100, 50);
    assert(TaUtil_CalcUnitAP(0, 3, 1, 150) == 1 && item->apBoost == -100);
    item = counter_item(100, 50);
    assert(TaUtil_CalcUnitAP(0, 3, 1, 151) == 1 && item->apBoost == -100);
    item = counter_item(100, -500); /* read clamps to 0 before the difference */
    assert(TaUtil_CalcUnitAP(0, 3, 1, 1) == 1 && item->apBoost == -100);
    /* multiply: the product of the counter and val is compared with the limit */
    item = counter_item(10, 20);
    assert(TaUtil_CalcUnitAP(0, 3, 2, 333) == 1 && item->apBoost == 6660);
    item = counter_item(10, 20);
    assert(TaUtil_CalcUnitAP(0, 3, 2, 334) == 1 && item->apBoost == 9989);
    item = counter_item(10, 20);
    assert(TaUtil_CalcUnitAP(0, 3, 2, 0) == 1 && item->apBoost == 0);
    item = counter_item(10, 20);
    assert(TaUtil_CalcUnitAP(0, 3, 2, -1) == 1 && item->apBoost == -20);
    /* divide (callee operation 3): zero divisor is rejected without a write */
    item = counter_item(10, 100);
    assert(TaUtil_CalcUnitAP(0, 3, 3, 0) == -1 && item->apBoost == 100);
    assert(TaUtil_CalcUnitAP(0, 3, 3, 7) == 1 && item->apBoost == 14);
    item->apBoost = 100;
    assert(TaUtil_CalcUnitAP(0, 3, 3, -7) == 1 && item->apBoost == -14);
    /* set, clamped by the writer; unknown operations do nothing */
    item = counter_item(10, 100);
    assert(TaUtil_CalcUnitAP(0, 3, 4, 12345) == 1 && item->apBoost == 9999);
    assert(TaUtil_CalcUnitAP(0, 3, 4, -3) == 1 && item->apBoost == -3);
    assert(TaUtil_CalcUnitAP(0, 3, 5, 9) == 1 && TaUtil_CalcUnitAP(0, 3, -1, 9) == 1 && item->apBoost == -3);
}

static struct DotrStateItem *second_counter_item(int base, int adjust)
{
    struct DotrStateItem *item = &gSzDuel_aaVarInfo[0][3];

    memset(gSzDuel_aaVarInfo, 0, sizeof(gSzDuel_aaVarInfo));
    memset(&counter_source, 0, sizeof(counter_source));
    item->constInfo = &counter_source;
    counter_source.dpWithFlags = base;
    item->dpBoost = adjust;
    return item;
}

static void check_second_counter_update(void)
{
    struct DotrStateItem *item;

    /* add: below the limit stores the sum; at/over it pins the counter at 9999 */
    item = second_counter_item(100, 50);
    assert(TaUtil_CalcUnitDP(0, 3, 0, 100) == 1 && item->dpBoost == 150);
    item = second_counter_item(100, 50);
    assert(TaUtil_CalcUnitDP(0, 3, 0, 9849) == 1 && item->dpBoost == 9899);
    item = second_counter_item(100, 50);
    assert(TaUtil_CalcUnitDP(0, 3, 0, 9900) == 1 && item->dpBoost == 9899);
    item = second_counter_item(8000, 5000); /* read clamps to 9999 before the sum */
    assert(TaUtil_CalcUnitDP(0, 3, 0, 1) == 1 && item->dpBoost == 1999);
    /* subtract: non-negative result subtracts; negative pins the counter at 0 */
    item = second_counter_item(100, 50);
    assert(TaUtil_CalcUnitDP(0, 3, 1, 150) == 1 && item->dpBoost == -100);
    item = second_counter_item(100, 50);
    assert(TaUtil_CalcUnitDP(0, 3, 1, 151) == 1 && item->dpBoost == -100);
    item = second_counter_item(100, -500); /* read clamps to 0 before the difference */
    assert(TaUtil_CalcUnitDP(0, 3, 1, 1) == 1 && item->dpBoost == -100);
    /* multiply: the product of the counter and val is compared with the limit */
    item = second_counter_item(10, 20);
    assert(TaUtil_CalcUnitDP(0, 3, 2, 333) == 1 && item->dpBoost == 6660);
    item = second_counter_item(10, 20);
    assert(TaUtil_CalcUnitDP(0, 3, 2, 334) == 1 && item->dpBoost == 9989);
    item = second_counter_item(10, 20);
    assert(TaUtil_CalcUnitDP(0, 3, 2, 0) == 1 && item->dpBoost == 0);
    item = second_counter_item(10, 20);
    assert(TaUtil_CalcUnitDP(0, 3, 2, -1) == 1 && item->dpBoost == -20);
    /* divide (callee operation 3): zero divisor is rejected without a write */
    item = second_counter_item(10, 100);
    assert(TaUtil_CalcUnitDP(0, 3, 3, 0) == -1 && item->dpBoost == 100);
    assert(TaUtil_CalcUnitDP(0, 3, 3, 7) == 1 && item->dpBoost == 14);
    item->dpBoost = 100;
    assert(TaUtil_CalcUnitDP(0, 3, 3, -7) == 1 && item->dpBoost == -14);
    /* set, clamped by the writer; unknown operations do nothing */
    item = second_counter_item(10, 100);
    assert(TaUtil_CalcUnitDP(0, 3, 4, 12345) == 1 && item->dpBoost == 9999);
    assert(TaUtil_CalcUnitDP(0, 3, 4, -3) == 1 && item->dpBoost == -3);
    assert(TaUtil_CalcUnitDP(0, 3, 5, 9) == 1 && TaUtil_CalcUnitDP(0, 3, -1, 9) == 1 && item->dpBoost == -3);
}

static int slide(struct DotrSlideObject *o, int dir, int x, int mode)
{
    memset(data_00317490_control, 0, sizeof(data_00317490_control));
    return TaEff_PopHand(99, o, dir, x, mode);
}

static void check_slide(void)
{
    struct DotrSlideObject o;

    memset(&o, 0, sizeof(o));
    /* start: pos 0 with counter 0 jumps to 0 (dir 1) or 960; afterwards the counter is 1 */
    assert(slide(&o, 1, 7, 1) == 0 && o.counter == 1 && o.pos == 30);
    assert(data_00317490_control[0].unknown_cae == 7 && data_00317490_control[0].unknown_ca2 == 0);
    assert(data_00317490_control[0].unknown_ca4 == -480 && data_00317490_control[0].unknown_cb4 == 18);
    memset(&o, 0, sizeof(o));
    assert(slide(&o, 2, 8, 1) == 0 && o.counter == 1 && o.pos == 930);
    assert(data_00317490_control[0].unknown_ca4 == 480);
    /* pos 0 with a nonzero counter is left alone */
    memset(&o, 0, sizeof(o));
    o.counter = 5;
    assert(slide(&o, 2, 8, 1) == 0 && o.counter == 6 && o.pos == 0 - 30 + 65536);
    /* mode 1: move 30 per call toward 480 from either side; arrival returns 1 and does not move */
    memset(&o, 0, sizeof(o));
    o.counter = 1; o.pos = 100;
    assert(slide(&o, 1, 3, 1) == 0 && o.pos == 130 && data_00317490_control[0].unknown_ca4 == -380);
    o.pos = 100;
    assert(slide(&o, 2, 3, 1) == 0 && o.pos == 70);
    o.pos = 480;
    assert(slide(&o, 1, 3, 1) == 1 && o.pos == 480 && data_00317490_control[0].unknown_ca4 == 0);
    assert(slide(&o, 2, 3, 1) == 1 && o.pos == 480);
    /* other modes: move away from the mode-1 target by dir; arrival at 0 (dir 1) or 960 (otherwise) */
    o.counter = 1; o.pos = 100;
    assert(slide(&o, 1, 3, 0) == 0 && o.pos == 70);
    o.pos = 100;
    assert(slide(&o, 2, 3, 0) == 0 && o.pos == 130);
    o.pos = 960;
    assert(slide(&o, 2, 3, 2) == 1 && o.pos == 960 && data_00317490_control[0].unknown_ca4 == 480);
    o.pos = 960;
    assert(slide(&o, 1, 3, 0) == 0 && o.pos == 930);
    o.pos = 0;
    assert(slide(&o, 1, 3, 0) == 1 && o.pos == 0 && o.counter == 2);
}

/* 0x1e11b0: branch coverage for the lookup and for the values reachable without a null cell. */
static void check_item_place(void)
{
    struct DotrStateItem *item = &gSzDuel_aaVarInfo[2][4];
    float out[4];
    short offsets[2];

    memset(gSzDuel_aaVarInfo, 0, sizeof(gSzDuel_aaVarInfo));
    memset(gSzDuel_aaMapInfo, 0, sizeof(gSzDuel_aaMapInfo));
    out[0] = out[1] = out[2] = out[3] = 9.0f;
    /* no item, and an item whose +0x45 byte is 11: nothing is written */
    assert(SzDuel_GetUnitPos(3, 0, out) == 0);
    item->slot = 11;
    assert(SzDuel_GetUnitPos(2, 4, out) == 0);
    assert(out[0] == 9.0f && out[1] == 9.0f && out[2] == 9.0f && out[3] == 9.0f);
    /* +0x45 of 10 passes */
    item->slot = 10;
    item->locCol = 5;
    item->locRow = 1;
    offsets[0] = -7, offsets[1] = 9;
    memcpy(item->unknown_34, offsets, sizeof(offsets));
    gSzDuel_aaMapInfo[1][5].fieldRate = 128;
    assert(SzDuel_GetUnitPos(2, 4, out) == 1);
    assert(out[0] == 93.0f && out[1] == 35.0f && out[2] == -91.0f && out[3] == 1.0f);
    /* both ends of the 0..6 range, a negative height and zero offsets */
    item->locCol = 0;
    item->locRow = 6;
    offsets[0] = 0, offsets[1] = 0;
    memcpy(item->unknown_34, offsets, sizeof(offsets));
    gSzDuel_aaMapInfo[6][0].fieldRate = -64;
    assert(SzDuel_GetUnitPos(2, 4, out) == 1);
    assert(out[0] == -150.0f && out[1] == -17.5f && out[2] == 150.0f && out[3] == 1.0f);
    item->locCol = 6;
    item->locRow = 0;
    gSzDuel_aaMapInfo[0][6].fieldRate = 1;
    assert(SzDuel_GetUnitPos(2, 4, out) == 1);
    assert(out[0] == 150.0f && out[1] == 35.0f / 128.0f && out[2] == -150.0f);
    memset(gSzDuel_aaVarInfo, 0, sizeof(gSzDuel_aaVarInfo));
    memset(gSzDuel_aaMapInfo, 0, sizeof(gSzDuel_aaMapInfo));
}

/* lSzDuel_CalcMapSummonEnable (branch and loop coverage). 0x1e36e0 is the real function; the
 * 0x250360 stub answers ring_side for the item bytes (0xa1, 10). */
static int ring_side, ring_calls;

static int pair_free, pair_last[2];
int func_00250360_pair(int a, int b)
{
    if (!pair_free) {
        assert(a == 0xa1 && b == 10);
    }
    pair_last[0] = a;
    pair_last[1] = b;
    ring_calls++;
    return ring_side;
}

static void ring_expect(int cx, int cy, int skip_x, int skip_y)
{
    int x, y;

    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            int dx = x - cx, dy = y - cy;
            int near = dx >= -1 && dx <= 1 && dy >= -1 && dy <= 1 && !(dx == 0 && dy == 0);

            if (x == skip_x && y == skip_y) {
                near = 0;
            }
            assert(gSzDuel_aaMapInfo[y][x].mapMovable == (near ? 0 : 0xff));
        }
    }
}

static void ring_reset(void)
{
    int x, y;

    memset(gSzDuel_aaMapInfo, 0, sizeof(gSzDuel_aaMapInfo));
    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            gSzDuel_aaMapInfo[y][x].mapMovable = 0x55;
            gSzDuel_aaMapInfo[y][x].mapAlpha = 0x66;
        }
    }
    ring_calls = 0;
}

static void check_ring(void)
{
    static struct DotrStateItem ring_item;
    static const int centres[6][2] = {{3, 3}, {0, 0}, {6, 6}, {0, 6}, {-1, 3}, {-2, 9}};
    struct DotrStateCell *c = &gSzDuel_aaMapInfo[2][4];
    int n, x, y;

    /* no items, no 8: the in-range ring is cleared, everything else 0xff, nothing else stored */
    for (n = 0; n < 6; n++) {
        ring_reset();
        lSzDuel_CalcMapSummonEnable(3, centres[n][0], centres[n][1]);
        ring_expect(centres[n][0], centres[n][1], -1, -1);
        assert(ring_calls == 0);
        for (y = 0; y < 7; y++) {
            for (x = 0; x < 7; x++) {
                assert(gSzDuel_aaMapInfo[y][x].mapAlpha == 0x66 && gSzDuel_aaMapInfo[y][x].field == 0);
            }
        }
    }
    /* unknown_16 of 8 keeps 0xff; 7 and 9 do not */
    ring_reset();
    c->field = 8;
    lSzDuel_CalcMapSummonEnable(3, 3, 3);
    ring_expect(3, 3, 4, 2);
    for (n = 7; n <= 9; n += 2) {
        ring_reset();
        c->field = n;
        lSzDuel_CalcMapSummonEnable(3, 3, 3);
        ring_expect(3, 3, -1, -1);
    }
    /* an item of another side keeps 0xff; of this side it does not */
    ring_item.team = 0xa1;
    ring_item.slot = 10;
    ring_reset();
    c->unit = (DotrStateWord)&ring_item;
    ring_side = 4;
    lSzDuel_CalcMapSummonEnable(3, 3, 3);
    ring_expect(3, 3, 4, 2);
    assert(ring_calls == 1);
    ring_reset();
    c->unit = (DotrStateWord)&ring_item;
    ring_side = 3;
    lSzDuel_CalcMapSummonEnable(3, 3, 3);
    ring_expect(3, 3, -1, -1);
    assert(ring_calls == 1);
    /* 8 and an item of this side: the item is still asked, 0xff stays */
    ring_reset();
    c->unit = (DotrStateWord)&ring_item;
    c->field = 8;
    lSzDuel_CalcMapSummonEnable(3, 3, 3);
    ring_expect(3, 3, 4, 2);
    assert(ring_calls == 1);
    /* an item on the centre cell or outside the ring is not asked */
    ring_reset();
    gSzDuel_aaMapInfo[3][3].unit = (DotrStateWord)&ring_item;
    gSzDuel_aaMapInfo[5][3].unit = (DotrStateWord)&ring_item;
    ring_side = 4;
    lSzDuel_CalcMapSummonEnable(3, 3, 3);
    ring_expect(3, 3, -1, -1);
    assert(ring_calls == 0);
    /* an item 0x1e36e0 rejects (unknown_45 of 11) counts as none */
    ring_reset();
    ring_item.slot = 11;
    c->unit = (DotrStateWord)&ring_item;
    lSzDuel_CalcMapSummonEnable(3, 3, 3);
    ring_expect(3, 3, -1, -1);
    assert(ring_calls == 0);
    memset(gSzDuel_aaMapInfo, 0, sizeof(gSzDuel_aaMapInfo));
}

/* Branch coverage: completed and not completed. */
static void check_camera_callback(void)
{
    struct DotrStateLoadRecord record = { { 1, 2, 3, 4, 5 }, 77 };

    gSzBattle_bl[0] = 10;
    gSzBattle_bl[1] = 20;
    gSzBattle_bl[2] = 30;
    lSzBattle_CameraCB(&record, 1);
    assert(gSzBattle_bl[0] == 10 && gSzBattle_bl[1] == 20 && gSzBattle_bl[2] == 77);
    lSzBattle_CameraCB(&record, 0);
    assert(gSzBattle_bl[0] == 10 && gSzBattle_bl[1] == 20 && gSzBattle_bl[2] == 0);
}

/* TaEnc_GetEnchantLst (branch coverage): empty row, the three skipped codes, the 49-bit scan
 * bound and the 12-entry cap. */
static void check_enchant_list(void)
{
    unsigned short list[14];
    int i;

    memset(EnchMatch, 0, sizeof(EnchMatch));
    memset(list, 0xee, sizeof(list));
    assert(TaEnc_GetEnchantLst(1, list) == 0 && list[0] == 0xeeee);
    /* row 2: bits 0, 28 (780), 37 (789), 44 (796), 48 and 49 (beyond the scan) */
    EnchMatch[14] = 0x01;
    EnchMatch[17] = 0x10;
    EnchMatch[18] = 0x20;
    EnchMatch[19] = 0x10;
    EnchMatch[20] = 0x03;
    assert(TaEnc_GetEnchantLst(2, list) == 2);
    assert(list[0] == 752 && list[1] == 800 && list[2] == 0xeeee);
    assert(TaEnc_GetEnchantLst(1, list) == 0 && TaEnc_GetEnchantLst(3, list) == 0);
    /* row 3 full: stops after twelve entries */
    memset(&EnchMatch[21], 0xff, 7);
    memset(list, 0xee, sizeof(list));
    assert(TaEnc_GetEnchantLst(3, list) == 12);
    for (i = 0; i < 12; i++) {
        assert(list[i] == 752 + i);
    }
    assert(list[12] == 0xeeee);
}

/* lSzDuel_CalcMapMovableRecursive and lSzDuel_CalcMapMovable (branch and loop coverage). 0x1e36e0
 * is the real function; the 0x250360 stub answers ring_side and records its arguments. */
static unsigned char reach_px[64], reach_py[64];

static void reach_reset(void)
{
    int x, y;

    memset(gSzDuel_aaMapInfo, 0, sizeof(gSzDuel_aaMapInfo));
    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            gSzDuel_aaMapInfo[y][x].mapMovable = 0xff;
        }
    }
    memset(reach_px, 0xee, sizeof(reach_px));
    memset(reach_py, 0xee, sizeof(reach_py));
    ring_calls = 0;
    pair_free = 1;
}

static int reach_dist(int x, int y, int cx, int cy)
{
    return (x > cx ? x - cx : cx - x) + (y > cy ? y - cy : cy - y);
}

/* every cell within `limit` steps of (cx, cy) holds base + distance, all others 0xff,
 * apart from the cells listed in skip (x, y pairs, -1 terminated) */
static void reach_expect(int cx, int cy, int limit, int base, const int *skip)
{
    int x, y, d, k, skipped;

    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            skipped = 0;
            for (k = 0; skip != 0 && skip[k] >= 0; k += 2) {
                if (skip[k] == x && skip[k + 1] == y) {
                    skipped = 1;
                }
            }
            if (skipped) {
                continue;
            }
            d = reach_dist(x, y, cx, cy);
            assert(gSzDuel_aaMapInfo[y][x].mapMovable == (d <= limit ? base + d : 0xff));
        }
    }
}

static void check_reach(void)
{
    static struct DotrStateItem reach_item;
    static const int out[4][2] = {{-1, 3}, {7, 3}, {3, -1}, {3, 7}};
    static const int skip_east[] = {4, 3, 5, 3, -1};
    static const int skip_one[] = {4, 3, -1};
    static const int skip_far[] = {5, 3, -1};
    unsigned char xs[4], ys[4];
    struct DotrStateCell *east = &gSzDuel_aaMapInfo[3][4];
    int n, x, y;

    /* out of range on each side: nothing stored, the item lookup not reached */
    for (n = 0; n < 4; n++) {
        reach_reset();
        lSzDuel_CalcMapMovableRecursive(0, 0, 3, out[n][0], out[n][1], 5, xs, ys, 0, reach_px, reach_py, 0, 1);
        reach_expect(3, 3, -1, 0, 0);
        assert(reach_px[0] == 0xee && reach_py[0] == 0xee);
    }
    /* limit 0: the start cell only, recorded in the work arrays; limit 2 and 3: distance */
    reach_reset();
    lSzDuel_CalcMapMovableRecursive(0, 0, 3, 3, 2, 0, xs, ys, 0, reach_px, reach_py, 0, 1);
    reach_expect(3, 2, 0, 0, 0);
    assert(reach_px[0] == 3 && reach_py[0] == 2 && reach_px[1] == 0xee && reach_py[1] == 0xee);
    for (n = 2; n <= 3; n++) {
        reach_reset();
        lSzDuel_CalcMapMovableRecursive(0, 0, 3, 3, 3, n, xs, ys, 0, reach_px, reach_py, 0, 1);
        reach_expect(3, 3, n, 0, 0);
        assert(ring_calls == 0);
    }
    /* a corner start (neighbours out of range), n added to the stored value, a lower value kept */
    reach_reset();
    gSzDuel_aaMapInfo[1][0].mapMovable = 1;
    lSzDuel_CalcMapMovableRecursive(0, 0, 3, 0, 0, 2, xs, ys, 2, reach_px, reach_py, 0, 1);
    gSzDuel_aaMapInfo[1][0].mapMovable += 2;
    reach_expect(0, 0, 2, 2, 0);
    /* a listed (xs, ys) cell is left below the first level; same x with another y is not; the
     * start cell is not checked against the list */
    reach_reset();
    xs[0] = 4; ys[0] = 3; xs[1] = 3; ys[1] = 3; xs[2] = 3; ys[2] = 9;
    lSzDuel_CalcMapMovableRecursive(0, 0, 3, 3, 3, 2, xs, ys, 3, reach_px, reach_py, 0, 1);
    reach_expect(3, 3, 2, 3, skip_east);
    assert(east->mapMovable == 0xff && gSzDuel_aaMapInfo[3][5].mapMovable == 0xff);
    /* unknown_16 of 8 without an item and pass 0: not marked and the budget of 1 ends there */
    reach_reset();
    east->field = 8;
    lSzDuel_CalcMapMovableRecursive(0, 0, 3, 3, 3, 2, xs, ys, 0, reach_px, reach_py, 0, 1);
    reach_expect(3, 3, 2, 0, skip_east);
    assert(east->mapMovable == 0xff && gSzDuel_aaMapInfo[3][5].mapMovable == 0xff);
    /* a budget of 2 lets the search pass the 8 cell, still unmarked */
    reach_reset();
    east->field = 8;
    lSzDuel_CalcMapMovableRecursive(0, 0, 3, 3, 3, 2, xs, ys, 0, reach_px, reach_py, 0, 2);
    reach_expect(3, 3, 2, 0, skip_one);
    assert(east->mapMovable == 0xff);
    /* pass set: the 8 cell is an ordinary cell; 7 is ordinary either way */
    reach_reset();
    east->field = 8;
    lSzDuel_CalcMapMovableRecursive(1, 0, 3, 3, 3, 2, xs, ys, 0, reach_px, reach_py, 0, 1);
    reach_expect(3, 3, 2, 0, 0);
    reach_reset();
    east->field = 7;
    lSzDuel_CalcMapMovableRecursive(0, 0, 3, 3, 3, 2, xs, ys, 0, reach_px, reach_py, 0, 1);
    reach_expect(3, 3, 2, 0, 0);
    /* an 8 start cell with n + depth 0: not marked, no budget spent */
    reach_reset();
    gSzDuel_aaMapInfo[3][3].field = 8;
    lSzDuel_CalcMapMovableRecursive(0, 0, 3, 3, 3, 1, xs, ys, 0, reach_px, reach_py, 0, 1);
    assert(gSzDuel_aaMapInfo[3][3].mapMovable == 0xff && east->mapMovable == 1);
    /* an item of another side: left unmarked with check set; with check clear it is marked
     * and the budget ends the search there */
    reach_item.team = 0xa1;
    reach_item.slot = 10;
    reach_reset();
    east->unit = (DotrStateWord)&reach_item;
    ring_side = 4;
    lSzDuel_CalcMapMovableRecursive(0, 1, 3, 3, 3, 2, xs, ys, 0, reach_px, reach_py, 0, 1);
    reach_expect(3, 3, 2, 0, skip_east);
    assert(east->mapMovable == 0xff && gSzDuel_aaMapInfo[3][5].mapMovable == 0xff);
    assert(ring_calls == 1 && pair_last[0] == 0xa1 && pair_last[1] == 10);
    reach_reset();
    east->unit = (DotrStateWord)&reach_item;
    lSzDuel_CalcMapMovableRecursive(0, 0, 3, 3, 3, 2, xs, ys, 0, reach_px, reach_py, 0, 1);
    reach_expect(3, 3, 2, 0, skip_far);
    assert(gSzDuel_aaMapInfo[3][5].mapMovable == 0xff && ring_calls == 1);
    /* an item of this side with check set: marked, budget spent */
    reach_reset();
    east->unit = (DotrStateWord)&reach_item;
    ring_side = 3;
    lSzDuel_CalcMapMovableRecursive(0, 1, 3, 3, 3, 2, xs, ys, 0, reach_px, reach_py, 0, 1);
    reach_expect(3, 3, 2, 0, skip_far);
    assert(gSzDuel_aaMapInfo[3][5].mapMovable == 0xff);
    /* unknown_45 0: unknown_44 equal to side leaves unmarked, another value does not */
    reach_item.slot = 0;
    reach_item.team = 3;
    reach_reset();
    east->unit = (DotrStateWord)&reach_item;
    lSzDuel_CalcMapMovableRecursive(0, 0, 3, 3, 3, 2, xs, ys, 0, reach_px, reach_py, 0, 1);
    reach_expect(3, 3, 2, 0, skip_east);
    assert(east->mapMovable == 0xff && pair_last[0] == 3 && pair_last[1] == 0);
    reach_item.team = 2;
    reach_reset();
    east->unit = (DotrStateWord)&reach_item;
    lSzDuel_CalcMapMovableRecursive(0, 0, 3, 3, 3, 2, xs, ys, 0, reach_px, reach_py, 0, 1);
    assert(east->mapMovable == 1);
    /* an item on the start cell: not asked at the first level and, as the neighbours do not
     * step back onto a cell of their own path, never asked at all; with n 1 its budget ends
     * the search on the start cell */
    reach_item.team = 0xa1;
    reach_item.slot = 10;
    reach_reset();
    gSzDuel_aaMapInfo[3][3].unit = (DotrStateWord)&reach_item;
    lSzDuel_CalcMapMovableRecursive(0, 1, 3, 3, 3, 3, xs, ys, 0, reach_px, reach_py, 0, 1);
    reach_expect(3, 3, 3, 0, 0);
    assert(ring_calls == 0);
    reach_reset();
    gSzDuel_aaMapInfo[3][3].unit = (DotrStateWord)&reach_item;
    xs[0] = 0; ys[0] = 0;
    lSzDuel_CalcMapMovableRecursive(0, 1, 3, 3, 3, 3, xs, ys, 1, reach_px, reach_py, 0, 1);
    reach_expect(3, 3, 0, 1, 0);

    /* 0x1e27d0, reset set: every cell 0xff first, the search from (x0, y0) with n added, then
     * the index in each in-range listed cell (the first pair is out of range) */
    reach_reset();
    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            gSzDuel_aaMapInfo[y][x].mapMovable = 3;
        }
    }
    xs[0] = 7; ys[0] = 0; xs[1] = 6; ys[1] = 6; xs[2] = 0; ys[2] = 6;
    lSzDuel_CalcMapMovable(1, 0, 0, 3, 3, 3, 2, xs, ys, 3);
    {
        static const int listed[] = {6, 6, 0, 6, -1};
        reach_expect(3, 3, 2, 3, listed);
    }
    assert(gSzDuel_aaMapInfo[6][6].mapMovable == 1 && gSzDuel_aaMapInfo[6][0].mapMovable == 2);
    /* reset clear: cells below 0xff become 0xfe, 0xff stays, reached cells get the distance;
     * pass, check and side reach the search (the 8 cell is marked only with pass set) */
    for (n = 0; n < 2; n++) {
        reach_reset();
        gSzDuel_aaMapInfo[0][0].mapMovable = 9;
        gSzDuel_aaMapInfo[3][2].mapMovable = 0xfe;
        east->field = 8;
        lSzDuel_CalcMapMovable(0, n, 0, 3, 3, 3, 1, xs, ys, 0);
        assert(gSzDuel_aaMapInfo[0][0].mapMovable == 0xfe && gSzDuel_aaMapInfo[0][1].mapMovable == 0xff);
        assert(gSzDuel_aaMapInfo[3][3].mapMovable == 0 && gSzDuel_aaMapInfo[3][2].mapMovable == 1);
        assert(gSzDuel_aaMapInfo[2][3].mapMovable == 1 && gSzDuel_aaMapInfo[3][5].mapMovable == 0xff);
        assert(east->mapMovable == (n ? 1 : 0xff));
    }
    reach_reset();
    east->unit = (DotrStateWord)&reach_item;
    ring_side = 4;
    lSzDuel_CalcMapMovable(0, 0, 1, 3, 3, 3, 1, xs, ys, 0);
    assert(east->mapMovable == 0xff && ring_calls == 1);
    ring_side = 3;
    lSzDuel_CalcMapMovable(0, 0, 1, 3, 3, 3, 1, xs, ys, 0);
    assert(east->mapMovable == 1);
    pair_free = 0;
    ring_calls = 0;
    memset(gSzDuel_aaMapInfo, 0, sizeof(gSzDuel_aaMapInfo));
}

static int line_calls;
static struct { int cursor, alpha, single; float pos[3], dx, dz; unsigned char colour[4]; } line_log[4];
int SzDuel_DrawCursor_Pipe(int a, float *centre, float dx, float dz, int alpha, unsigned char *colour, int single)
{
    assert(line_calls < 4);
    line_log[line_calls].cursor = a;
    line_log[line_calls].alpha = alpha;
    line_log[line_calls].single = single;
    memcpy(line_log[line_calls].pos, centre, sizeof(line_log[0].pos));
    line_log[line_calls].dx = dx;
    line_log[line_calls].dz = dz;
    memcpy(line_log[line_calls].colour, colour, 4);
    line_calls++;
    return a + 0x10;
}

/* Gate closed and open; cells with both, only the first, only the second and neither quad
 * visible, at the first and last cell of the walk. */
static void check_draw_line(void)
{
    static const struct DotrStateCellQuad q0 = {200, 100, 50, 255};
    static const struct DotrStateCellQuad q1 = {64, 32, 16, 1};
    struct DotrStateCell saved[7][7];
    struct DotrStateCell *cell;

    memcpy(saved, gSzDuel_aaMapInfo, sizeof(saved));
    memset(gSzDuel_aaMapInfo, 0, sizeof(saved));
    cell = &gSzDuel_aaMapInfo[0][0];
    cell->unknown_04[1] = q0;
    cell->unknown_04[3] = q1;
    cell->selectionHighlightType = 77;
    cell->mapAlpha = 64;
    cell = &gSzDuel_aaMapInfo[2][5];
    cell->unknown_04[1] = q0;
    cell->unknown_04[3] = q1;
    cell->unknown_04[3].unknown_3 = 0;
    cell->selectionHighlightType = 9;
    cell->mapAlpha = 128;
    cell = &gSzDuel_aaMapInfo[6][6];
    cell->unknown_04[1] = q0;
    cell->unknown_04[1].unknown_3 = 0;
    cell->unknown_04[3] = q1;
    cell->mapAlpha = 255;

    line_calls = 0;
    gate_result = 0;
    gate_argument = -1;
    assert(SzDuel_DrawLine_Pipe(0x500) == 0x500 && line_calls == 0 && gate_argument == 0);
    gate_result = 1;
    assert(SzDuel_DrawLine_Pipe(0x500) == 0x540 && line_calls == 4);
    gate_result = 0;

    assert(line_log[0].cursor == 0x500 && line_log[0].alpha == 77 && line_log[0].single == 1);
    assert(line_log[0].pos[0] == -150.0f && line_log[0].pos[1] == -0.2f && line_log[0].pos[2] == -150.0f);
    assert(line_log[0].dx == 24.0f && line_log[0].dz == 24.0f);
    assert(line_log[0].colour[0] == 100 && line_log[0].colour[1] == 50 && line_log[0].colour[2] == 25);
    assert(line_log[0].colour[3] == 255);
    assert(line_log[1].cursor == 0x510 && line_log[1].alpha == 1 && line_log[1].single == 0);
    assert(line_log[1].pos[0] == -150.0f && line_log[1].pos[1] == -0.1f && line_log[1].pos[2] == -150.0f);
    assert(line_log[1].dx == 24.0f && line_log[1].dz == 24.0f);
    assert(line_log[1].colour[0] == 32 && line_log[1].colour[1] == 16 && line_log[1].colour[2] == 8);
    assert(line_log[1].colour[3] == 1);
    assert(line_log[2].cursor == 0x520 && line_log[2].alpha == 9 && line_log[2].single == 1);
    assert(line_log[2].pos[0] == 100.0f && line_log[2].pos[1] == -0.2f && line_log[2].pos[2] == -50.0f);
    assert(line_log[2].colour[0] == 200 && line_log[2].colour[1] == 100 && line_log[2].colour[2] == 50);
    assert(line_log[3].cursor == 0x530 && line_log[3].alpha == 1 && line_log[3].single == 0);
    assert(line_log[3].pos[0] == 150.0f && line_log[3].pos[1] == -0.1f && line_log[3].pos[2] == 150.0f);
    assert(line_log[3].colour[0] == (64 * 255) >> 7 && line_log[3].colour[1] == (32 * 255) >> 7);
    assert(line_log[3].colour[2] == (16 * 255) >> 7 && line_log[3].colour[3] == 1);
    memcpy(gSzDuel_aaMapInfo, saved, sizeof(saved));
}

int main(void)
{
    check_draw_line();
    check_ring();
    check_cells();
    check_entries();
    check_items();
    check_step();
    check_pair();
    check_values();
    check_counts();
    check_accessors();
    check_item_arithmetic();
    check_selection();
    check_byte_table();
    check_bit_fields();
    check_probe_and_vector();
    check_cell_fields();
    check_item_flags_and_object();
    check_control_and_triple();
    check_cached_dispatch();
    check_set_selected_bit();
    check_selected_bit();
    check_flow();
    check_ticker();
    check_ticker2();
    check_side_setup();
    check_flag_query();
    check_counter_update();
    check_second_counter_update();
    check_slide();
    check_item_place();
    check_camera_callback();
    check_reach();
    puts("state_grid: ok");
    check_enchant_list();
    return 0;
}
