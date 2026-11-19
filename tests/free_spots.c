/* Synthetic host tests for the free_spots group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/free_spots.h"

#define SIDE 1
#define PACKED 0x0305
#define HERE 0x0204
#define KEY 0x12345

typedef int (*free_fn)(int side, unsigned char kind, struct DotrFreeBlock *block, int packed);

static struct DotrFreeBlock block;
static int spots[8];
static int kinds[8];
static int held[8];
static int filled[8];
static int ids[8];
static int costs[8];
static int reach[8];
static int gate_answer;
static int key_id;
static int here_calls;
static int key_calls;
static int gate_calls;
static int long_calls;
static int short_calls;
static int reach_calls;
static int id_calls;
static int last;
static int fixed; /* 1 while 0x137120 runs: fixed arguments, no key callee; 2 for 0x169610 */
static int packed_now = PACKED;
/* 0x11e270: the side-position call, the packed answer and the count 4 */
#define CORNER 0x0607
static int corner_mode;
static int corner_calls;
static int pack_answer;
static int n_now;
/* 0x12e3f0: the long costing's own count, and the other side's position call */
static int n_long;
static int enemy_mode;
static int enemy_calls;
/* fixed == 3 (0x115340): the later stage's answers and records */
static int to_id;
static int to_at;
static int to_calls;
static int deb_answer;
static int deb_calls;
static int near_calls;
static int near_costs[8];
static int near_tos[8];
/* fixed == 4 (0x1652f0): what each costing was handed, and the one place that answers */
static int near_ids[8];
static int id_seen[8];
static int ids_again[8];
static int round_tos[5];
static int want_to = -1;
/* fixed == 5 (0x10e890): the line callee's rounds and the one round whose costs answer */
static int line_calls;
static int line_mode;
static int line_to;
static int hit_round;

/* 0x144a60: the row search's answers */
static int row_mode;
static int home_at;
static int row_hits;
/* 0x151cf0 / 0x1491b0: the type callee and 0x1491b0's five target places */
static int type_answer;
static int type_calls;
static int type_arg;
static int def_mode;
static int def_filled[5];
static int def_calls;

int TaLogic_GetWarnHideLvl__00130780(int side)
{
    assert(side == SIDE);
    key_calls++;
    return KEY;
}

int TaLogic_GetWarnHideLvl__001461c0(int side)
{
    assert(side == SIDE);
    key_calls++;
    return KEY;
}

static int find(int x, int y)
{
    int i;

    for (i = 0; i < 8; i++) {
        if (spots[i] == ((y << 8) | x)) {
            return i;
        }
    }
    assert(0);
    return 0;
}

/* the count 0x27a130 and 0x2792c0 are handed, per function under test */
static int want_n(void)
{
    if (n_now) {
        return n_now;
    }
    return fixed == 0 ? 3 : fixed == 1 ? 2 : fixed == 2 ? 4 : 5;
}

int SzDuel_GetUnitLoc(int a, int b)
{
    if (enemy_mode && b == 0) {
        /* 0x12e3f0's second stage: the other side's position */
        assert(a == (SIDE == 0 ? 1 : 0));
        enemy_calls++;
        return CORNER;
    }
    if (corner_mode && b == 0) {
        /* 0x11e270's first call: the side's own position */
        assert(a == SIDE);
        corner_calls++;
        return CORNER;
    }
    assert(a == (packed_now >> 8) && b == (packed_now & 0xff));
    here_calls++;
    return HERE;
}

int Ta_GetSommonPiecePreference(int a, int i)
{
    assert(a == SIDE && i >= 0 && i < 8);
    return spots[i];
}

int SzDuel_GetField(int x, int y)
{
    if (row_mode && x == 0) {
        /* 0x144a60's row search: column 0 of rows 1..5, answered with a marker */
        assert(y >= 1 && y <= 5);
        return 0x50 + y;
    }
    return kinds[find(x, y)];
}

int func_0024fe40_free(struct DotrFreeBlock *b, int x, int y)
{
    assert(b == &block);
    return held[find(x, y)];
}

struct DotrStateItem *SzDuel_GetMapUnit(int x, int y)
{
    if (def_mode && y == 0x0a) {
        assert(x >= 1 && x <= 5);
        return def_filled[x - 1] ? (struct DotrStateItem *)&block : NULL;
    }
    return filled[find(x, y)] ? (struct DotrStateItem *)&block : NULL;
}

int SzDuel_GetMapUnitID(int x, int y)
{
    if (fixed >= 3) {
        int i;

        for (i = 0; i < 8; i++) {
            if (spots[i] == ((y << 8) | x)) {
                id_calls++;
                if ((fixed == 4 || fixed == 5) && (id_seen[i]++ & 1)) {
                    /* 0x1652f0's last stage asks a free position twice */
                    return ids_again[i];
                }
                return ids[i];
            }
        }
        /* not a free position: the place the later stage costs towards */
        to_calls++;
        to_at = (y << 8) | x;
        return to_id;
    }
    id_calls++;
    return ids[find(x, y)];
}

static int key(int id, int s)
{
    assert(id == key_id && s == SIDE);
    key_calls++;
    return KEY;
}

int func_0015d120_free(int s) { return key(1, s); }
int func_00162580_free(int s) { return key(2, s); }
int func_00172310_free(int s) { return key(3, s); }
int func_0016b0e0_free(int s) { return key(4, s); }
int TaLogic_GetWarnHideLvl__00117c30(int s) { return key(5, s); }
int TaLogic_GetWarnHideLvl__001676d0(int s) { return key(6, s); }

int CheckLandAgreement(int a, struct DotrStateItem *item)
{
    if (row_mode) {
        assert(a == 0x51 + deb_calls % 5 && item == (struct DotrStateItem *)&block);
        deb_calls++;
        if (a - 0x50 == home_at) {
            deb_calls = 0;
            row_hits++;
            return 1;
        }
        return a - 0x50 == 2 ? 2 : 0;
    }
    assert(a == 3 && item == (struct DotrStateItem *)&block);
    deb_calls++;
    return deb_answer;
}

int func_00278510_free(struct DotrFreeBlock *b, int packed, int c, int d, int e, int skip)
{
    assert(b == &block && packed == PACKED && c == (fixed == 1 ? 0 : KEY) && d == (HERE & 0xff) && e == (HERE >> 8) && skip == (fixed != 0 && fixed != 3 && fixed != 5));
    gate_calls++;
    return gate_answer;
}

static void check_list(struct DotrFreeNibbles *bytes)
{
    int i;

    for (i = 0; i < 12; i++) {
        assert(bytes[i].low == 0xf && bytes[i].high == 0xf);
    }
}

int func_0027a130_free(struct DotrFreeBlock *b, struct DotrFreeNibbles *bytes, int column, int row, int x, int y,
                       int g, int h, unsigned char i, unsigned short k)
{
    assert(b == &block && x == (HERE & 0xff) && y == (HERE >> 8) && g == PACKED && h == (fixed != 0 && fixed != 3 && fixed != 5) && i == (n_long ? n_long : want_n()) && k == (fixed == 1 ? 0 : (KEY & 0xffff)));
    check_list(bytes);
    long_calls++;
    last = find(column, row);
    return costs[last];
}

int func_002792c0_free(struct DotrFreeBlock *b, struct DotrFreeNibbles *bytes, int column, int row, int x, int y,
                       unsigned short id, int n)
{
    if (fixed == 4 && n == 7) {
        assert(b == &block);
        check_list(bytes);
        last = find(column, row);
        near_tos[last] = (y << 8) | x;
        near_ids[last] = id;
        if (near_calls < 40) {
            round_tos[near_calls / 8] = (y << 8) | x;
        }
        near_calls++;
        if (want_to >= 0 && ((y << 8) | x) != want_to) {
            return 14;
        }
        return near_costs[last];
    }
    if (fixed == 5 && n == 7) {
        last = find(column, row);
        assert(b == &block && ((y << 8) | x) == line_to && id == (ids_again[last] < 0 ? 0xffff : ids_again[last]));
        check_list(bytes);
        near_calls++;
        return line_calls - 1 == hit_round ? near_costs[last] : 14;
    }
    if (fixed == 3 && n == 7) {
        assert(b == &block && ((y << 8) | x) == to_at && id == (to_id < 0 ? 0xffff : to_id));
        check_list(bytes);
        near_calls++;
        last = find(column, row);
        near_tos[last] = to_at;
        return near_costs[last];
    }
    assert(b == &block && x == (HERE & 0xff) && y == (HERE >> 8) && id == PACKED && n == want_n());
    check_list(bytes);
    short_calls++;
    last = find(column, row);
    return costs[last];
}

int func_0027afc0_free(struct DotrFreeNibbles *bytes)
{
    check_list(bytes);
    reach_calls++;
    if (fixed == 5) {
        /* 0x10e890 sets the twelve bytes again for every position: the next costing checks them */
        bytes[3].low = 1;
    }
    return reach[last];
}

/* 0x10e890's own callees */
int TaLogic_GetWarnHideLvl__0010eda0(int side)
{
    assert(side == SIDE);
    key_calls++;
    return KEY;
}

int TaLogic_GetSommonPiecePreference__0010ed90(int a, int i)
{
    return Ta_GetSommonPiecePreference(a, i);
}

int TaLogic002_GetLinePiece(unsigned char mode, unsigned char index)
{
    assert(mode == line_mode && index == line_calls);
    line_calls++;
    memset(id_seen, 0, sizeof(id_seen));
    line_to = 0x0403 + index;
    /* only the low 16 bits are the place */
    return 0x10000 | line_to;
}

static void reset(int id)
{
    int i;

    memset(&block, 0, sizeof(block));
    for (i = 0; i < 8; i++) {
        spots[i] = ((i + 1) << 8) | (i + 2);
        kinds[i] = 0;
        held[i] = 0;
        filled[i] = 0;
        ids[i] = 5;
        costs[i] = 100;
        reach[i] = 1;
    }
    gate_answer = 0;
    key_id = id;
    here_calls = key_calls = gate_calls = long_calls = short_calls = reach_calls = id_calls = 0;
    to_id = 0x77;
    to_at = -1;
    deb_answer = 0;
    to_calls = deb_calls = near_calls = 0;
    for (i = 0; i < 8; i++) {
        near_costs[i] = 14;
        near_tos[i] = -1;
    }
}

static void check_copy(free_fn fn, int id)
{
    /* null block: nothing is asked */
    reset(id);
    assert(fn(SIDE, 0, NULL, PACKED) == -1);
    assert(here_calls == 0 && id_calls == 0);

    /* first stage, short cost path: strictly lower cost wins, earliest on a tie */
    reset(id);
    costs[2] = 40;
    costs[5] = 40;
    costs[6] = 41;
    assert(fn(SIDE, 0, &block, PACKED) == spots[2]);
    assert(here_calls == 1 && gate_calls == 8 && key_calls == 8 && short_calls == 8 && long_calls == 0);
    assert(reach_calls == 8 && id_calls == 0);

    /* long cost path when the gate answer is positive: the key is asked twice */
    reset(id);
    gate_answer = 1;
    costs[7] = 3;
    assert(fn(SIDE, 0, &block, PACKED) == spots[7]);
    assert(long_calls == 8 && short_calls == 0 && key_calls == 16);

    /* positions skipped by the first stage: negative, kind 8, kind 9 held, filled */
    reset(id);
    spots[0] = -1;
    kinds[1] = 8;
    kinds[2] = 9;
    held[2] = 1;
    filled[3] = 1;
    kinds[4] = 9;
    held[4] = 2;
    costs[4] = 9;
    costs[0] = costs[1] = costs[2] = costs[3] = 1;
    spots[1] = 0x0102;
    assert(fn(SIDE, 0, &block, PACKED) == spots[4]);
    assert(gate_calls == 4 && short_calls == 4);

    /* an unreachable position or a cost of 255 and above is not taken */
    reset(id);
    costs[0] = 10;
    reach[0] = 0;
    costs[1] = 255;
    for (last = 2; last < 8; last++) {
        costs[last] = 254 + last;
    }
    costs[3] = 254;
    assert(fn(SIDE, 0, &block, PACKED) == spots[3]);

    /* nothing taken: the second stage returns the first position with a negative id */
    reset(id);
    for (last = 0; last < 8; last++) {
        reach[last] = 0;
    }
    spots[0] = -1;
    kinds[1] = 8;
    kinds[2] = 9;
    held[2] = 1;
    kinds[3] = 9;
    ids[3] = 0;
    ids[5] = -1;
    ids[6] = -1;
    assert(fn(SIDE, 0, &block, PACKED) == spots[5]);
    assert(reach_calls == 5 && id_calls == 3);

    /* second stage only: byte +0x3e at 0x20, and a negative packed value */
    reset(id);
    block.kind = 0x20;
    costs[0] = 1;
    ids[4] = -1;
    assert(fn(SIDE, 0, &block, PACKED) == spots[4]);
    assert(here_calls == 0 && gate_calls == 0 && id_calls == 5);
    reset(id);
    block.kind = 0x1f;
    ids[7] = -3;
    assert(fn(SIDE, 0, &block, -1) == spots[7]);
    assert(here_calls == 0 && gate_calls == 0 && id_calls == 8);

    /* byte +0x3e at 0x1f still runs the first stage */
    reset(id);
    block.kind = 0x1f;
    assert(fn(SIDE, 0, &block, PACKED) == spots[0]);
    assert(here_calls == 1);

    /* no position at all */
    reset(id);
    block.kind = 0xff;
    assert(fn(SIDE, 0, &block, PACKED) == -1);
    assert(id_calls == 8);
}

/* 0x137120: fixed arguments, and a second stage that keeps the last hit */
static void check_fixed(void)
{
    fixed = 1;
    /* null block */
    reset(0);
    assert(TaLogic009_GetSummonPiece(SIDE, 0, NULL, PACKED) == -1);
    assert(here_calls == 0 && id_calls == 0);

    /* first stage, short path: lowest cost, earliest on a tie; no key callee */
    reset(0);
    costs[2] = 40;
    costs[5] = 40;
    assert(TaLogic009_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[2]);
    assert(here_calls == 1 && gate_calls == 8 && key_calls == 0 && short_calls == 8 && long_calls == 0 && id_calls == 0);

    /* long path on a positive gate answer */
    reset(0);
    gate_answer = 1;
    costs[7] = 3;
    assert(TaLogic009_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[7]);
    assert(long_calls == 8 && short_calls == 0 && key_calls == 0);

    /* first-stage skips: negative, kind 8, kind 9 held, filled; kind 9 not held counts */
    reset(0);
    spots[0] = -1;
    kinds[1] = 8;
    kinds[2] = 9;
    held[2] = 1;
    filled[3] = 1;
    kinds[4] = 9;
    held[4] = 2;
    costs[4] = 9;
    costs[1] = costs[2] = costs[3] = 1;
    assert(TaLogic009_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[4]);
    assert(gate_calls == 4 && short_calls == 4);

    /* unreachable or cost 255: not taken; 254 is */
    reset(0);
    costs[0] = 10;
    reach[0] = 0;
    costs[1] = 255;
    costs[3] = 254;
    for (last = 4; last < 8; last++) {
        costs[last] = 300;
    }
    costs[2] = 256;
    assert(TaLogic009_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[3]);

    /* nothing taken: the second stage returns the LAST position with a negative id */
    reset(0);
    for (last = 0; last < 8; last++) {
        reach[last] = 0;
    }
    spots[0] = -1;
    kinds[1] = 8;
    ids[1] = -1;
    kinds[2] = 9;
    held[2] = 1;
    ids[2] = -1;
    kinds[3] = 9;
    ids[3] = 0;
    ids[5] = -1;
    ids[6] = -1;
    kinds[7] = 8;
    ids[7] = -1;
    assert(TaLogic009_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[6]);
    assert(reach_calls == 4 && id_calls == 4);

    /* second stage only: byte +0x3e at 0x20, or a negative packed value */
    reset(0);
    block.kind = 0x20;
    ids[4] = -1;
    ids[1] = -1;
    assert(TaLogic009_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[4]);
    assert(here_calls == 0 && gate_calls == 0 && id_calls == 8);
    reset(0);
    block.kind = 0x1f;
    ids[0] = -3;
    assert(TaLogic009_GetSummonPiece(SIDE, 0, &block, -1) == spots[0]);
    assert(here_calls == 0 && gate_calls == 0 && id_calls == 8);

    /* byte +0x3e at 0x1f still runs the first stage */
    reset(0);
    block.kind = 0x1f;
    assert(TaLogic009_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[0]);
    assert(here_calls == 1);

    /* no position at all, in both second-stage copies */
    reset(0);
    block.kind = 0xff;
    assert(TaLogic009_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);
    assert(id_calls == 8);
    reset(0);
    assert(TaLogic009_GetSummonPiece(SIDE, 0, &block, -1) == -1);
    assert(id_calls == 8);
    fixed = 0;
}

/* 0x169610: level 0 or 5 runs the first stage and returns the first hit; other levels the last low hit */
static void check_levels(void)
{
    static const unsigned char first_levels[2] = { 0x00, 0xa0 };
    static const unsigned char last_levels[4] = { 0x20, 0x80, 0xc0, 0xe0 };
    int n;

    fixed = 2;
    reset(4);
    assert(TaLogic021_GetSummonPiece(SIDE, 0, NULL, PACKED) == -1);
    assert(here_calls == 0 && id_calls == 0);

    for (n = 0; n < 2; n++) {
        /* short cost path: lowest cost, earliest on a tie */
        reset(4);
        block.kind = first_levels[n] | 0x1f;
        costs[2] = 40;
        costs[5] = 40;
        assert(TaLogic021_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[2]);
        assert(here_calls == 1 && gate_calls == 8 && key_calls == 8 && short_calls == 8 && long_calls == 0 && id_calls == 0);

        /* long cost path */
        reset(4);
        block.kind = first_levels[n];
        gate_answer = 1;
        costs[7] = 3;
        assert(TaLogic021_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[7]);
        assert(long_calls == 8 && short_calls == 0 && key_calls == 16);
    }

    /* first-stage skips */
    reset(4);
    spots[0] = -1;
    kinds[1] = 8;
    kinds[2] = 9;
    held[2] = 1;
    filled[3] = 1;
    kinds[4] = 9;
    held[4] = 2;
    costs[4] = 9;
    costs[1] = costs[2] = costs[3] = 1;
    assert(TaLogic021_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[4]);
    assert(gate_calls == 4 && short_calls == 4);

    /* cost 254 is taken, 255 and unreachable are not */
    reset(4);
    for (last = 0; last < 8; last++) {
        costs[last] = 255;
    }
    costs[0] = 10;
    reach[0] = 0;
    costs[3] = 254;
    assert(TaLogic021_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[3]);

    /* nothing taken: 0x1e1090 is asked again, then the FIRST position with a negative id */
    reset(4);
    for (last = 0; last < 8; last++) {
        costs[last] = 255;
    }
    spots[0] = -1;
    kinds[1] = 8;
    ids[1] = -1;
    kinds[2] = 9;
    held[2] = 1;
    ids[2] = -1;
    kinds[3] = 9;
    ids[3] = 0;
    ids[5] = -1;
    ids[6] = -1;
    assert(TaLogic021_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[5]);
    assert(here_calls == 2 && id_calls == 3);

    /* kind 9 not held counts in the first-hit stage; no hit gives -1 */
    reset(4);
    for (last = 0; last < 8; last++) {
        reach[last] = 0;
    }
    kinds[3] = 9;
    ids[3] = -1;
    assert(TaLogic021_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[3]);
    reset(4);
    for (last = 0; last < 8; last++) {
        reach[last] = 0;
    }
    assert(TaLogic021_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);
    assert(here_calls == 2 && id_calls == 8);

    /* negative packed: no first stage, 0x1e1090 still asked once */
    reset(4);
    packed_now = -1;
    ids[3] = -1;
    assert(TaLogic021_GetSummonPiece(SIDE, 0, &block, -1) == spots[3]);
    assert(here_calls == 1 && gate_calls == 0 && id_calls == 4);
    packed_now = PACKED;

    for (n = 0; n < 4; n++) {
        /* other levels: the LAST hit whose row is below 8 (spots[7] has row 8) */
        reset(4);
        block.kind = last_levels[n];
        ids[2] = -1;
        ids[6] = -1;
        ids[7] = -1;
        assert(TaLogic021_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[6]);
        assert(here_calls == 0 && gate_calls == 0 && id_calls == 8);
    }
    /* skips in the last-hit stage */
    reset(4);
    block.kind = 0x20;
    ids[0] = -1;
    ids[1] = -1;
    ids[2] = -1;
    ids[3] = -1;
    ids[4] = -1;
    spots[4] = -1;
    kinds[3] = 8;
    kinds[2] = 9;
    held[2] = 1;
    kinds[1] = 9;
    assert(TaLogic021_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[1]);
    assert(id_calls == 5);
    /* only the row-8 position answers: nothing */
    reset(4);
    block.kind = 0x20;
    ids[7] = -1;
    assert(TaLogic021_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);
    reset(4);
    block.kind = 0x20;
    assert(TaLogic021_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);
    fixed = 0;
}

/* TaLogic001_GetSummonPiece (branch and loop coverage). */
static void check_near(void)
{
    int n;

    fixed = 3;
    reset(5);
    assert(TaLogic001_GetSummonPiece(SIDE, 0, NULL, PACKED) == -1);
    assert(here_calls == 0 && id_calls == 0);

    /* first stage, short cost path: lowest cost, earliest on a tie; the level bits alone decide */
    reset(5);
    block.kind = 0x1f;
    costs[2] = 40;
    costs[5] = 40;
    assert(TaLogic001_GetSummonPiece(SIDE, 9, &block, PACKED) == spots[2]);
    assert(here_calls == 1 && gate_calls == 8 && key_calls == 8 && short_calls == 8 && long_calls == 0
           && id_calls == 0 && near_calls == 0);
    /* long cost path: the key is asked again */
    reset(5);
    gate_answer = 1;
    costs[7] = 3;
    assert(TaLogic001_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[7]);
    assert(long_calls == 8 && short_calls == 0 && key_calls == 16);
    /* first-stage skips */
    reset(5);
    spots[0] = -1;
    kinds[1] = 8;
    kinds[2] = 9;
    held[2] = 1;
    filled[3] = 1;
    kinds[4] = 9;
    held[4] = 2;
    costs[4] = 9;
    costs[1] = costs[2] = costs[3] = 1;
    assert(TaLogic001_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[4]);
    assert(gate_calls == 4 && short_calls == 4);
    /* cost 254 is taken, 255 and unreachable are not */
    reset(5);
    for (n = 0; n < 8; n++) {
        costs[n] = 255;
    }
    costs[0] = 10;
    reach[0] = 0;
    costs[3] = 254;
    assert(TaLogic001_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[3]);
    assert(near_calls == 0);

    /* nothing taken: 0x1e1090 is not asked again; no position with a negative id gives -1 */
    reset(5);
    for (n = 0; n < 8; n++) {
        reach[n] = 0;
    }
    assert(TaLogic001_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);
    assert(here_calls == 1 && id_calls == 8 && deb_calls == 0 && near_calls == 0);
    /* 0x24deb0 not 1: costed towards (3, 3) with the id there; lowest cost wins, earliest on a tie */
    reset(5);
    for (n = 0; n < 8; n++) {
        reach[n] = 0;
        ids[n] = -1;
    }
    near_costs[2] = 13;
    near_costs[4] = 5;
    near_costs[6] = 5;
    assert(TaLogic001_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[4]);
    assert(deb_calls == 8 && near_calls == 8 && to_calls == 8 && to_at == 0x0303 && id_calls == 8);
    /* 14 is not taken, 13 and 0 are, a negative cost is not; a negative id there is handed on as 0xffff */
    reset(5);
    packed_now = -1;
    to_id = -1;
    ids[1] = ids[2] = ids[3] = -1;
    assert(TaLogic001_GetSummonPiece(SIDE, 0, &block, -1) == -1);
    assert(here_calls == 0 && gate_calls == 0 && near_calls == 3 && id_calls == 8);
    near_costs[1] = 13;
    assert(TaLogic001_GetSummonPiece(SIDE, 0, &block, -1) == spots[1]);
    near_costs[2] = -1;
    assert(TaLogic001_GetSummonPiece(SIDE, 0, &block, -1) == spots[1]);
    near_costs[3] = 0;
    assert(TaLogic001_GetSummonPiece(SIDE, 0, &block, -1) == spots[3]);
    /* skips in this stage */
    reset(5);
    for (n = 0; n < 8; n++) {
        ids[n] = -1;
        near_costs[n] = n;
    }
    spots[0] = -1;
    kinds[1] = 8;
    kinds[2] = 9;
    held[2] = 1;
    ids[3] = 0;
    kinds[4] = 9;
    held[4] = 2;
    assert(TaLogic001_GetSummonPiece(SIDE, 0, &block, -1) == spots[4]);
    assert(near_calls == 4 && deb_calls == 4);
    /* 0x24deb0 answers 1: the corner depends on the position */
    reset(5);
    deb_answer = 1;
    spots[0] = (3 << 8) | 1;
    spots[1] = (2 << 8) | 2;
    spots[2] = (4 << 8) | 5;
    spots[3] = (5 << 8) | 3;
    spots[4] = (4 << 8) | 4;
    for (n = 0; n < 8; n++) {
        ids[n] = -1;
    }
    near_costs[5] = 1;
    assert(TaLogic001_GetSummonPiece(SIDE, 0, &block, -1) == spots[5]);
    assert(near_tos[0] == 0x501 && near_tos[1] == 0x105 && near_tos[2] == 0x206 && near_tos[3] == 0x602
           && near_tos[4] == 0x105 && near_tos[5] == 0x602 && near_calls == 8);
    packed_now = PACKED;

    for (n = 1; n < 8; n++) {
        /* other levels: the LAST hit whose row is below 8 (spots[7] has row 8) */
        reset(5);
        block.kind = (unsigned char)(n << 5);
        ids[2] = -1;
        ids[6] = -1;
        ids[7] = -1;
        assert(TaLogic001_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[6]);
        assert(here_calls == 0 && gate_calls == 0 && id_calls == 8 && deb_calls == 0);
    }
    /* level 0 with the +0x3c word 83 takes the same stage; 82 does not */
    reset(5);
    block.cardNo = 83;
    ids[2] = -1;
    assert(TaLogic001_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[2]);
    assert(here_calls == 0);
    reset(5);
    block.cardNo = 82;
    assert(TaLogic001_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[0]);
    assert(here_calls == 1);
    /* skips in the last-hit stage: kind 9 is skipped whether held or not */
    reset(5);
    block.kind = 0x20;
    ids[0] = ids[1] = ids[2] = ids[3] = ids[4] = -1;
    spots[4] = -1;
    kinds[3] = 8;
    kinds[2] = 9;
    held[2] = 1;
    kinds[1] = 9;
    assert(TaLogic001_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[0]);
    assert(id_calls == 4);
    /* only the row-8 position answers, or none: nothing */
    reset(5);
    block.kind = 0x20;
    ids[7] = -1;
    assert(TaLogic001_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);
    reset(5);
    block.kind = 0x20;
    assert(TaLogic001_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);
    fixed = 0;
}

static void check_rows(void)
{
    int i;

    fixed = 0;
    row_mode = 1;
    home_at = 3;
    /* null block */
    reset(0);
    assert(TaLogic012_GetSummonPiece(SIDE, 0, NULL, PACKED) == -1);
    assert(here_calls == 0 && id_calls == 0);

    /* first stage, short path: lowest cost, earliest on a tie; the key callee feeds the gate */
    reset(0);
    costs[2] = 40;
    costs[5] = 40;
    assert(TaLogic012_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[2]);
    assert(here_calls == 1 && gate_calls == 8 && key_calls == 8 && short_calls == 8 && long_calls == 0 && id_calls == 0);

    /* long path on a positive gate answer: the key callee is called again */
    reset(0);
    gate_answer = 1;
    costs[7] = 3;
    assert(TaLogic012_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[7]);
    assert(long_calls == 8 && short_calls == 0 && key_calls == 16);

    /* byte +0x3e in 0x60..0x7f (value 3) runs the first stage too */
    reset(0);
    block.kind = 0x7f;
    costs[1] = 7;
    assert(TaLogic012_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[1]);
    assert(here_calls == 1);

    /* first-stage skips: negative, kind 8, kind 9 held, filled; kind 9 not held counts */
    reset(0);
    spots[0] = -1;
    kinds[1] = 8;
    kinds[2] = 9;
    held[2] = 1;
    filled[3] = 1;
    kinds[4] = 9;
    held[4] = 2;
    costs[4] = 9;
    costs[1] = costs[2] = costs[3] = 1;
    assert(TaLogic012_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[4]);
    assert(gate_calls == 4 && short_calls == 4);

    /* unreachable or cost 255: not taken; 254 is */
    reset(0);
    costs[0] = 10;
    reach[0] = 0;
    costs[1] = 255;
    costs[3] = 254;
    for (i = 4; i < 8; i++) {
        costs[i] = 300;
    }
    costs[2] = 256;
    assert(TaLogic012_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[3]);

    /* nothing taken in the first stage: -1, and no later stage runs */
    reset(0);
    for (i = 0; i < 8; i++) {
        reach[i] = 0;
        ids[i] = -1;
    }
    assert(TaLogic012_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);
    assert(reach_calls == 8 && id_calls == 0 && row_hits == 0);

    /* negative packed value: the position with a negative id whose row is nearest the found
     * row (3); rows are 1..8, so index 2; skips as before; earliest on a tie (rows 2 and 4) */
    reset(0);
    row_hits = 0;
    for (i = 0; i < 8; i++) {
        ids[i] = -1;
    }
    assert(TaLogic012_GetSummonPiece(SIDE, 0, &block, -1) == spots[2]);
    assert(here_calls == 0 && gate_calls == 0 && id_calls == 8 && row_hits == 8);
    reset(0);
    row_hits = 0;
    ids[0] = -1;
    kinds[1] = 8;
    ids[1] = -1;
    kinds[2] = 9;
    held[2] = 1;
    ids[2] = -1;
    kinds[3] = 9;
    ids[3] = -1;
    spots[4] = -1;
    ids[7] = -1;
    assert(TaLogic012_GetSummonPiece(SIDE, 0, &block, -1) == spots[3]);
    assert(row_hits == 3);
    reset(0);
    ids[1] = -1;
    ids[3] = -1;
    assert(TaLogic012_GetSummonPiece(SIDE, 0, &block, -1) == spots[1]);

    /* distance bound: 13 rows away is taken, 14 is not */
    reset(0);
    spots[0] = (16 << 8) | 2;
    ids[0] = -1;
    assert(TaLogic012_GetSummonPiece(SIDE, 0, &block, -1) == spots[0]);
    reset(0);
    spots[0] = (17 << 8) | 2;
    ids[0] = -1;
    assert(TaLogic012_GetSummonPiece(SIDE, 0, &block, -1) == -1);

    /* no row found: the row counts as -1, so row 1 is two away and row 13 is not taken */
    reset(0);
    home_at = 9;
    deb_calls = 0;
    ids[0] = -1;
    ids[5] = -1;
    assert(TaLogic012_GetSummonPiece(SIDE, 0, &block, -1) == spots[0]);
    reset(0);
    spots[0] = (13 << 8) | 2;
    ids[0] = -1;
    assert(TaLogic012_GetSummonPiece(SIDE, 0, &block, -1) == -1);
    spots[0] = (12 << 8) | 2;
    deb_calls = 0;
    assert(TaLogic012_GetSummonPiece(SIDE, 0, &block, -1) == spots[0]);
    home_at = 3;

    /* no position with a negative id */
    reset(0);
    assert(TaLogic012_GetSummonPiece(SIDE, 0, &block, -1) == -1);
    assert(id_calls == 8);

    /* other values of byte +0x3e: the LAST position with a negative id in a row below 8 */
    reset(0);
    row_hits = 0;
    block.kind = 0x20;
    ids[1] = -1;
    ids[6] = -1;
    ids[7] = -1;
    assert(TaLogic012_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[6]);
    assert(here_calls == 0 && gate_calls == 0 && id_calls == 8 && row_hits == 0);
    reset(0);
    block.kind = 0x5f;
    ids[7] = -1;
    kinds[2] = 8;
    ids[2] = -1;
    kinds[3] = 9;
    held[3] = 1;
    ids[3] = -1;
    assert(TaLogic012_GetSummonPiece(SIDE, 0, &block, -1) == -1);
    reset(0);
    block.kind = 0x80;
    spots[0] = 2;
    ids[0] = -1;
    assert(TaLogic012_GetSummonPiece(SIDE, 0, &block, -1) == spots[0]);
    row_mode = 0;
}

/* 0x123010: its own slot, key and best-cost callees */
static int best_costs[8];
static int best_calls;

int TaLogic_GetSommonPiecePreference__00124f50(int side, int slot)
{
    assert(side == SIDE && slot >= 0 && slot < 8);
    return spots[slot];
}

int func_001254a0_free(int side)
{
    assert(side == SIDE);
    key_calls++;
    return KEY;
}

void func_00123500_free(struct DotrFreeBlock *b, int column, int row, struct DotrFreeSpotBest *best)
{
    assert(b == &block);
    best_calls++;
    best->spot = 0;
    best->cost = (unsigned char)best_costs[find(column, row)];
}

static void check_best(void)
{
    int i;

    fixed = 0;
    /* null block */
    reset(0);
    assert(TaLogic005_GetSummonPiece(SIDE, 0, NULL, PACKED) == -1);
    assert(here_calls == 0 && id_calls == 0);

    /* first stage, short path: lowest cost, earliest on a tie */
    reset(0);
    costs[2] = 40;
    costs[5] = 40;
    assert(TaLogic005_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[2]);
    assert(here_calls == 1 && gate_calls == 8 && key_calls == 8 && short_calls == 8 && long_calls == 0 && id_calls == 0);

    /* long path on a positive gate answer: the key callee is called again */
    reset(0);
    gate_answer = 1;
    costs[7] = 3;
    assert(TaLogic005_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[7]);
    assert(long_calls == 8 && short_calls == 0 && key_calls == 16);

    /* byte +0x3e value 3 runs the first stage too */
    reset(0);
    block.kind = 0x7f;
    costs[1] = 7;
    assert(TaLogic005_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[1]);

    /* first-stage skips: negative, kind 8, kind 9 held, filled; kind 9 not held counts */
    reset(0);
    spots[0] = -1;
    kinds[1] = 8;
    kinds[2] = 9;
    held[2] = 1;
    filled[3] = 1;
    kinds[4] = 9;
    held[4] = 2;
    costs[4] = 9;
    costs[1] = costs[2] = costs[3] = 1;
    assert(TaLogic005_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[4]);
    assert(gate_calls == 4 && short_calls == 4);

    /* unreachable or cost 255: not taken; 254 is */
    reset(0);
    costs[0] = 10;
    reach[0] = 0;
    costs[1] = 255;
    costs[3] = 254;
    for (i = 4; i < 8; i++) {
        costs[i] = 300;
    }
    costs[2] = 256;
    assert(TaLogic005_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[3]);

    /* nothing taken in the first stage: -1, no later stage */
    reset(0);
    for (i = 0; i < 8; i++) {
        reach[i] = 0;
        ids[i] = -1;
    }
    best_calls = 0;
    assert(TaLogic005_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);
    assert(reach_calls == 8 && id_calls == 0 && best_calls == 0);

    /* negative packed value: lowest answer byte among positions with a negative id,
     * earliest on a tie; 13 is taken, 14 is not */
    reset(0);
    best_calls = 0;
    for (i = 0; i < 8; i++) {
        ids[i] = -1;
        best_costs[i] = 20;
    }
    best_costs[3] = 13;
    best_costs[5] = 6;
    best_costs[6] = 6;
    assert(TaLogic005_GetSummonPiece(SIDE, 0, &block, -1) == spots[5]);
    assert(here_calls == 0 && gate_calls == 0 && id_calls == 8 && best_calls == 8);
    best_costs[5] = best_costs[6] = 14;
    assert(TaLogic005_GetSummonPiece(SIDE, 0, &block, -1) == spots[3]);

    /* skips as in the first stage, and a non-negative id */
    reset(0);
    best_calls = 0;
    for (i = 0; i < 8; i++) {
        ids[i] = -1;
        best_costs[i] = 1;
    }
    spots[0] = -1;
    kinds[1] = 8;
    kinds[2] = 9;
    held[2] = 1;
    ids[3] = 0;
    kinds[4] = 9;
    best_costs[4] = 2;
    best_costs[5] = best_costs[6] = best_costs[7] = 9;
    assert(TaLogic005_GetSummonPiece(SIDE, 0, &block, -1) == spots[4]);
    assert(best_calls == 4);

    /* no answer below 14: the first position of kind 4 with a negative id, same skips */
    reset(0);
    for (i = 0; i < 8; i++) {
        ids[i] = -1;
        best_costs[i] = 14;
    }
    spots[0] = -1;
    ids[1] = 3;
    kinds[1] = 4;
    kinds[2] = 8;
    kinds[3] = 9;
    held[3] = 1;
    kinds[5] = 4;
    kinds[6] = 4;
    assert(TaLogic005_GetSummonPiece(SIDE, 0, &block, -1) == spots[5]);

    /* none of kind 4: -1 */
    reset(0);
    for (i = 0; i < 8; i++) {
        ids[i] = -1;
        best_costs[i] = 0xff;
    }
    assert(TaLogic005_GetSummonPiece(SIDE, 0, &block, -1) == -1);
    assert(id_calls == 16);

    /* other values of byte +0x3e: the LAST position with a negative id in a row below 8 */
    reset(0);
    best_calls = 0;
    block.kind = 0x20;
    ids[1] = -1;
    ids[6] = -1;
    ids[7] = -1;
    assert(TaLogic005_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[6]);
    assert(here_calls == 0 && gate_calls == 0 && id_calls == 8 && best_calls == 0);
    reset(0);
    block.kind = 0x5f;
    ids[7] = -1;
    kinds[2] = 8;
    ids[2] = -1;
    kinds[3] = 9;
    held[3] = 1;
    ids[3] = -1;
    assert(TaLogic005_GetSummonPiece(SIDE, 0, &block, -1) == -1);
    reset(0);
    block.kind = 0x80;
    spots[0] = 2;
    ids[0] = -1;
    assert(TaLogic005_GetSummonPiece(SIDE, 0, &block, -1) == spots[0]);
}

/* 0x11e270's callees */
static int side_now = SIDE;

int TaLogic_GetSommonPiecePreference__0011f530(int side, int slot)
{
    assert(side == side_now && slot >= 0 && slot < 8);
    return spots[slot];
}

int func_0011fa80_free(int side)
{
    assert(side == SIDE);
    key_calls++;
    return KEY;
}

void func_0011d4e0_free(struct DotrFreeBlock *b, int column, int row, struct DotrFreeSpotBest *best)
{
    assert(b == &block);
    best_calls++;
    best->spot = 0;
    best->cost = (unsigned char)best_costs[find(column, row)];
}

int func_00276f10_free(struct DotrFreeBlock *b, int x, int y, int c, int d, int e, int f, int g, int h, int i)
{
    assert(b == &block && x == (CORNER & 0xff) && y == (CORNER >> 8) && c == 0 && d == want_n() && e == 31 && f == 2 && g == 2 && h == 0 && i == KEY);
    return pack_answer;
}

static void check_packed(void)
{
    int i;

    fixed = 0;
    corner_mode = 1;
    n_now = 4;
    pack_answer = PACKED;
    /* null block */
    reset(0);
    corner_calls = 0;
    assert(TaLogic004_GetSummonPiece(SIDE, 0, NULL) == -1);
    assert(corner_calls == 0 && here_calls == 0);

    /* first stage, short path: lowest cost, earliest on a tie; one key call for the packed value */
    reset(0);
    costs[2] = 40;
    costs[5] = 40;
    assert(TaLogic004_GetSummonPiece(SIDE, 0, &block) == spots[2]);
    assert(corner_calls == 1 && here_calls == 1 && gate_calls == 8 && key_calls == 9 && short_calls == 8 && long_calls == 0 && id_calls == 0);

    /* long path on a positive gate answer */
    reset(0);
    gate_answer = 1;
    costs[7] = 3;
    assert(TaLogic004_GetSummonPiece(SIDE, 0, &block) == spots[7]);
    assert(long_calls == 8 && short_calls == 0 && key_calls == 17);

    /* first-stage skips; unreachable and cost 255 not taken, 254 is */
    reset(0);
    spots[0] = -1;
    kinds[1] = 8;
    kinds[2] = 9;
    held[2] = 1;
    filled[3] = 1;
    kinds[4] = 9;
    held[4] = 2;
    costs[4] = 254;
    costs[1] = costs[2] = costs[3] = 1;
    costs[5] = 255;
    costs[6] = 10;
    reach[6] = 0;
    costs[7] = 300;
    assert(TaLogic004_GetSummonPiece(SIDE, 0, &block) == spots[4]);
    assert(gate_calls == 4 && short_calls == 4);

    /* nothing taken in the first stage: -1, no later stage */
    reset(0);
    for (i = 0; i < 8; i++) {
        reach[i] = 0;
        ids[i] = -1;
    }
    best_calls = 0;
    assert(TaLogic004_GetSummonPiece(SIDE, 0, &block) == -1);
    assert(reach_calls == 8 && id_calls == 0 && best_calls == 0);

    /* negative packed answer: lowest answer byte below 14, earliest on a tie */
    pack_answer = -1;
    reset(0);
    best_calls = 0;
    for (i = 0; i < 8; i++) {
        ids[i] = -1;
        best_costs[i] = 20;
    }
    best_costs[3] = 13;
    best_costs[5] = 6;
    best_costs[6] = 6;
    assert(TaLogic004_GetSummonPiece(SIDE, 0, &block) == spots[5]);
    assert(here_calls == 0 && gate_calls == 0 && id_calls == 8 && best_calls == 8);
    best_costs[5] = best_costs[6] = 14;
    assert(TaLogic004_GetSummonPiece(SIDE, 0, &block) == spots[3]);

    /* skips, and a non-negative id */
    reset(0);
    best_calls = 0;
    for (i = 0; i < 8; i++) {
        ids[i] = -1;
        best_costs[i] = 1;
    }
    spots[0] = -1;
    kinds[1] = 8;
    kinds[2] = 9;
    held[2] = 1;
    ids[3] = 0;
    kinds[4] = 9;
    best_costs[4] = 2;
    best_costs[5] = best_costs[6] = best_costs[7] = 9;
    assert(TaLogic004_GetSummonPiece(SIDE, 0, &block) == spots[4]);
    assert(best_calls == 4);

    /* no answer below 14: -1, with no kind-4 pass */
    reset(0);
    for (i = 0; i < 8; i++) {
        ids[i] = -1;
        best_costs[i] = 14;
        kinds[i] = 4;
    }
    assert(TaLogic004_GetSummonPiece(SIDE, 0, &block) == -1);
    assert(id_calls == 8);

    /* any other +0x3e value (3 included): the LAST position with a negative id in a row below 8 */
    pack_answer = PACKED;
    reset(0);
    corner_calls = 0;
    block.kind = 0x7f;
    ids[1] = -1;
    ids[6] = -1;
    ids[7] = -1;
    assert(TaLogic004_GetSummonPiece(SIDE, 0, &block) == spots[6]);
    assert(corner_calls == 0 && here_calls == 0 && gate_calls == 0 && id_calls == 8);
    reset(0);
    block.kind = 0x20;
    ids[7] = -1;
    kinds[2] = 8;
    ids[2] = -1;
    kinds[3] = 9;
    held[3] = 1;
    ids[3] = -1;
    assert(TaLogic004_GetSummonPiece(SIDE, 0, &block) == -1);
    reset(0);
    block.kind = 0x80;
    spots[0] = 2;
    ids[0] = -1;
    assert(TaLogic004_GetSummonPiece(SIDE, 0, &block) == spots[0]);
    corner_mode = 0;
    n_now = 0;
}

/* 0x119520's callees */
static int accept_k;
static int accept_calls;

int TaLogic_GetSommonPiecePreference__0011a4c0(int side, int slot)
{
    assert(side == SIDE && slot >= 0 && slot < 8);
    return spots[slot];
}

int func_0011aa10_key(int side)
{
    assert(side == SIDE);
    key_calls++;
    return KEY;
}

int TaLogic003_IsEmptyAtkLine(int k_side, unsigned char k)
{
    assert(k_side == SIDE && k < 4);
    accept_calls++;
    return k == accept_k;
}

static void check_towards(void)
{
    static const int places[5] = { 0x102, 0x104, 0x100, 0x106, -1 };
    int i;
    int k;

    corner_mode = 1;
    n_now = 3;
    fixed = 3;
    pack_answer = PACKED;
    accept_k = 0;
    /* null block */
    reset(0);
    corner_calls = 0;
    assert(TaLogic003_GetSummonPiece(SIDE, 0, NULL) == -1);
    assert(corner_calls == 0 && here_calls == 0);

    /* first stage, short path: lowest cost, earliest on a tie */
    reset(0);
    accept_calls = 0;
    costs[2] = 40;
    costs[5] = 40;
    assert(TaLogic003_GetSummonPiece(SIDE, 0, &block) == spots[2]);
    assert(corner_calls == 1 && here_calls == 1 && gate_calls == 8 && key_calls == 9 && short_calls == 8 && long_calls == 0 && accept_calls == 0);

    /* long path on a positive gate answer */
    reset(0);
    gate_answer = 1;
    costs[7] = 3;
    assert(TaLogic003_GetSummonPiece(SIDE, 0, &block) == spots[7]);
    assert(long_calls == 8 && short_calls == 0 && key_calls == 17);

    /* skips; unreachable, cost 255 and a negative cost not taken, 254 and 0 are */
    reset(0);
    spots[0] = -1;
    kinds[1] = 8;
    kinds[2] = 9;
    held[2] = 1;
    filled[3] = 1;
    kinds[4] = 9;
    held[4] = 2;
    costs[4] = 254;
    costs[1] = costs[2] = costs[3] = 1;
    costs[5] = 255;
    costs[6] = 10;
    reach[6] = 0;
    costs[7] = -1;
    assert(TaLogic003_GetSummonPiece(SIDE, 0, &block) == spots[4]);
    assert(gate_calls == 4 && short_calls == 4);
    costs[5] = 0;
    assert(TaLogic003_GetSummonPiece(SIDE, 0, &block) == spots[5]);

    /* nothing taken in the first stage: -1, no later stage */
    reset(0);
    accept_calls = 0;
    for (i = 0; i < 8; i++) {
        reach[i] = 0;
        ids[i] = -1;
    }
    assert(TaLogic003_GetSummonPiece(SIDE, 0, &block) == -1);
    assert(reach_calls == 8 && id_calls == 0 && accept_calls == 0);

    /* negative packed answer: each accepted key's place, and none; lowest cost
     * below 14, earliest on a tie */
    pack_answer = -1;
    for (k = 0; k < 5; k++) {
        reset(0);
        accept_k = k;
        accept_calls = 0;
        to_id = -1;
        to_at = places[k];
        for (i = 0; i < 8; i++) {
            ids[i] = -1;
            near_costs[i] = 20;
        }
        near_costs[3] = 13;
        near_costs[5] = 6;
        near_costs[6] = 6;
        assert(TaLogic003_GetSummonPiece(SIDE, 0, &block) == spots[5]);
        assert(accept_calls == (k < 4 ? k + 1 : 4));
        assert(here_calls == 0 && gate_calls == 0 && id_calls == 16 && near_calls == 8 && to_calls == 0);
        for (i = 0; i < 8; i++) {
            assert(near_tos[i] == places[k]);
        }
    }
    accept_k = 1;
    to_at = places[1];
    near_costs[5] = near_costs[6] = 14;
    assert(TaLogic003_GetSummonPiece(SIDE, 0, &block) == spots[3]);

    /* skips, a non-negative id, a negative cost and cost 0 */
    reset(0);
    to_id = -1;
    to_at = places[1];
    for (i = 0; i < 8; i++) {
        ids[i] = -1;
        near_costs[i] = 1;
    }
    spots[0] = -1;
    kinds[1] = 8;
    kinds[2] = 9;
    held[2] = 1;
    ids[3] = 0;
    kinds[4] = 9;
    near_costs[4] = 2;
    near_costs[5] = -1;
    near_costs[6] = near_costs[7] = 9;
    assert(TaLogic003_GetSummonPiece(SIDE, 0, &block) == spots[4]);
    assert(near_calls == 4);
    near_costs[7] = 0;
    assert(TaLogic003_GetSummonPiece(SIDE, 0, &block) == spots[7]);

    /* no cost below 14: -1 */
    reset(0);
    to_id = -1;
    to_at = places[1];
    for (i = 0; i < 8; i++) {
        ids[i] = -1;
    }
    assert(TaLogic003_GetSummonPiece(SIDE, 0, &block) == -1);
    assert(near_calls == 8);

    /* any other +0x3e value: the LAST position with a negative id in a row below 8;
     * kind 9 is skipped whatever 0x24fe40 would say */
    pack_answer = PACKED;
    reset(0);
    corner_calls = 0;
    accept_calls = 0;
    block.kind = 0x7f;
    ids[1] = -1;
    ids[5] = -1;
    kinds[5] = 9;
    ids[6] = -1;
    ids[7] = -1;
    assert(TaLogic003_GetSummonPiece(SIDE, 0, &block) == spots[6]);
    assert(corner_calls == 0 && here_calls == 0 && gate_calls == 0 && id_calls == 7 && accept_calls == 0);
    kinds[6] = 9;
    assert(TaLogic003_GetSummonPiece(SIDE, 0, &block) == spots[1]);
    reset(0);
    block.kind = 0x20;
    ids[7] = -1;
    kinds[2] = 8;
    ids[2] = -1;
    assert(TaLogic003_GetSummonPiece(SIDE, 0, &block) == -1);
    reset(0);
    block.kind = 0x80;
    spots[0] = 2;
    ids[0] = -1;
    assert(TaLogic003_GetSummonPiece(SIDE, 0, &block) == spots[0]);
    corner_mode = 0;
    n_now = 0;
    fixed = 0;
}

static void reset_rounds(void)
{
    int n;

    reset(6);
    want_to = -1;
    for (n = 0; n < 8; n++) {
        id_seen[n] = 0;
        ids_again[n] = -1;
        near_ids[n] = -2;
    }
    for (n = 0; n < 5; n++) {
        round_tos[n] = -1;
    }
}

/* 0x1652f0: one assertion per branch outcome and bound, except the test of the
   unsigned place being negative, which no input reaches */
static void check_rounds(void)
{
    static const int corner_spots[8] = { 0x0004, 0x0002, 0x0304, 0x0501, 0x0603, 0x0702, 0x0503, 0x0003 };
    static const int corner_tos[8] = { 0x106, 0x100, 0x605, 0x601, 0x601, 0x605, 0x605, 0x106 };
    int n;

    fixed = 4;
    packed_now = PACKED;
    reset_rounds();
    assert(TaLogic020_GetSummonPiece(SIDE, 0, NULL, PACKED) == -1);
    assert(here_calls == 0 && id_calls == 0);

    /* level 0, short cost path: lowest cost, earliest on a tie; the argument is not the level */
    reset_rounds();
    block.kind = 0x1f;
    costs[2] = 40;
    costs[5] = 40;
    assert(TaLogic020_GetSummonPiece(SIDE, 9, &block, PACKED) == spots[2]);
    assert(here_calls == 1 && gate_calls == 8 && key_calls == 8 && short_calls == 8 && long_calls == 0
           && id_calls == 0 && near_calls == 0);
    /* level 5, long cost path: the key is asked again */
    reset_rounds();
    block.kind = 0xa0;
    gate_answer = 1;
    costs[7] = 3;
    assert(TaLogic020_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[7]);
    assert(long_calls == 8 && short_calls == 0 && key_calls == 16);
    /* first-stage skips */
    reset_rounds();
    spots[0] = -1;
    kinds[1] = 8;
    kinds[2] = 9;
    held[2] = 1;
    filled[3] = 1;
    kinds[4] = 9;
    held[4] = 2;
    costs[4] = 9;
    costs[1] = costs[2] = costs[3] = 1;
    assert(TaLogic020_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[4]);
    assert(gate_calls == 4 && short_calls == 4);
    /* cost 254 is taken, 255 and unreachable are not */
    reset_rounds();
    for (n = 0; n < 8; n++) {
        costs[n] = 255;
    }
    costs[0] = 10;
    reach[0] = 0;
    costs[3] = 254;
    assert(TaLogic020_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[3]);
    assert(near_calls == 0);

    /* nothing taken and no position with a negative id: -1 */
    reset_rounds();
    for (n = 0; n < 8; n++) {
        reach[n] = 0;
    }
    assert(TaLogic020_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);
    assert(here_calls == 1 && id_calls == 8 && near_calls == 0);
    /* the corner each position is costed towards; the id there, 0xffff when negative */
    reset_rounds();
    for (n = 0; n < 8; n++) {
        spots[n] = corner_spots[n];
        reach[n] = 0;
        ids[n] = -1;
    }
    near_costs[2] = 13;
    near_costs[4] = 5;
    near_costs[6] = 5;
    assert(TaLogic020_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[4]);
    assert(near_calls == 8 && to_calls == 8 && id_calls == 8);
    for (n = 0; n < 8; n++) {
        assert(near_tos[n] == corner_tos[n] && near_ids[n] == 0x77);
    }
    /* a negative packed word goes straight to this stage; 14 is not taken, 13 and 0 are, a negative
     * cost is not */
    reset_rounds();
    packed_now = -1;
    to_id = -1;
    ids[1] = ids[2] = ids[3] = -1;
    assert(TaLogic020_GetSummonPiece(SIDE, 0, &block, -1) == -1);
    assert(here_calls == 0 && gate_calls == 0 && near_calls == 3 && id_calls == 8);
    assert(near_ids[1] == 0xffff && near_ids[0] == -2);
    near_costs[1] = 13;
    assert(TaLogic020_GetSummonPiece(SIDE, 0, &block, -1) == spots[1]);
    near_costs[2] = -1;
    assert(TaLogic020_GetSummonPiece(SIDE, 0, &block, -1) == spots[1]);
    near_costs[3] = 0;
    assert(TaLogic020_GetSummonPiece(SIDE, 0, &block, -1) == spots[3]);
    /* skips in this stage */
    reset_rounds();
    for (n = 0; n < 8; n++) {
        ids[n] = -1;
        near_costs[n] = n;
    }
    spots[0] = -1;
    kinds[1] = 8;
    kinds[2] = 9;
    held[2] = 1;
    kinds[3] = 9;
    held[3] = 2;
    assert(TaLogic020_GetSummonPiece(SIDE, 0, &block, -1) == spots[3]);
    assert(near_calls == 5);
    packed_now = PACKED;

    /* other levels: the five places in turn, nothing else asked; none answers */
    reset_rounds();
    block.kind = 0x20;
    for (n = 0; n < 8; n++) {
        ids[n] = -1;
    }
    assert(TaLogic020_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);
    assert(here_calls == 0 && gate_calls == 0 && key_calls == 0 && near_calls == 40 && id_calls == 80 && to_calls == 0);
    assert(round_tos[0] == 0x203 && round_tos[1] == 0x301 && round_tos[2] == 0x305 && round_tos[3] == 0x202
           && round_tos[4] == 0x204);
    assert(near_ids[0] == 0xffff);
    /* the third place answers: lowest cost, earliest on a tie; the second id answer is handed on */
    reset_rounds();
    block.kind = 0xc0;
    for (n = 0; n < 8; n++) {
        ids[n] = -1;
    }
    ids_again[4] = 0x31;
    want_to = 0x305;
    near_costs[2] = 13;
    near_costs[4] = 5;
    near_costs[6] = 5;
    assert(TaLogic020_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[4]);
    assert(near_calls == 24 && near_ids[4] == 0x31 && near_ids[6] == 0xffff);
    /* the last place answers; cost 0 is taken, a negative cost is not */
    reset_rounds();
    block.kind = 0x40;
    for (n = 0; n < 8; n++) {
        ids[n] = -1;
    }
    want_to = 0x204;
    near_costs[1] = -1;
    near_costs[5] = 0;
    assert(TaLogic020_GetSummonPiece(SIDE, 0, &block, -1) == spots[5]);
    assert(near_calls == 40);
    /* skips: no position, kind 8, kind 9 held, an id at the position */
    reset_rounds();
    block.kind = 0x60;
    for (n = 0; n < 8; n++) {
        ids[n] = -1;
        near_costs[n] = n;
    }
    spots[0] = -1;
    kinds[1] = 8;
    kinds[2] = 9;
    held[2] = 1;
    ids[3] = 0;
    kinds[4] = 9;
    held[4] = 2;
    assert(TaLogic020_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[4]);
    assert(near_calls == 4 && near_tos[4] == 0x203);
    want_to = -1;
    fixed = 0;
}

static void l2_reset(int kind)
{
    int i;

    reset(0);
    block.kind = (unsigned char)((kind << 5) | 0x1f);
    corner_calls = line_calls = 0;
    hit_round = -1;
    memset(id_seen, 0, sizeof(id_seen));
    for (i = 0; i < 8; i++) {
        ids[i] = -1;
        ids_again[i] = -1;
    }
}

/* 0x10e890 TaLogic002_GetSummonPiece */
static void check_logic002(void)
{
    int i;

    fixed = 5;
    corner_mode = 1;
    n_now = 2;

    /* null block: nothing is asked */
    l2_reset(0);
    assert(TaLogic002_GetSummonPiece(SIDE, 9, NULL, PACKED) == -1);
    assert(corner_calls == 0 && here_calls == 0 && id_calls == 0);

    /* kind 0, packed given, 0x278510 answers 0: the ten-argument costing; field 8, an
     * occupied place and a held field 9 are skipped, an unheld field 9 is costed, an
     * unreachable or negative cost is not taken, the first of two equal costs wins */
    l2_reset(0);
    kinds[0] = 8;
    filled[1] = 1;
    kinds[2] = 9;
    held[2] = 1;
    kinds[3] = 9;
    costs[3] = 7;
    costs[4] = 5;
    reach[4] = 0;
    costs[5] = -1;
    costs[6] = 6;
    costs[7] = 6;
    assert(TaLogic002_GetSummonPiece(SIDE, 9, &block, PACKED) == spots[6]);
    assert(corner_calls == 1 && here_calls == 1 && gate_calls == 1 && key_calls == 6);
    assert(long_calls == 5 && short_calls == 0 && reach_calls == 5 && line_calls == 0 && id_calls == 0);

    /* kind 3, 0x278510 negative: the eight-argument costing; nothing reachable gives -1 */
    l2_reset(3);
    gate_answer = -1;
    for (i = 0; i < 8; i++) {
        reach[i] = 0;
    }
    assert(TaLogic002_GetSummonPiece(SIDE, 9, &block, PACKED) == -1);
    assert(gate_calls == 1 && key_calls == 1 && long_calls == 0 && short_calls == 8 && reach_calls == 8);

    /* kind 4, same path: a cost of zero is taken */
    l2_reset(4);
    gate_answer = -1;
    costs[2] = 0;
    assert(TaLogic002_GetSummonPiece(SIDE, 9, &block, PACKED) == spots[2]);
    assert(short_calls == 8 && line_calls == 0);

    /* kind 0, negative packed: mode 1, up to six line places; the third answers. Field
     * 8 and 9 and a place with an id are skipped, a negative cost is not taken, the
     * first of two equal costs wins, a second id answer is handed on */
    l2_reset(0);
    line_mode = 1;
    hit_round = 2;
    kinds[0] = 8;
    kinds[1] = 9;
    ids[2] = 0;
    near_costs[3] = 9;
    near_costs[4] = -2;
    near_costs[5] = 4;
    near_costs[6] = 4;
    ids_again[5] = 0x33;
    assert(TaLogic002_GetSummonPiece(SIDE, 9, &block, -1) == spots[5]);
    assert(corner_calls == 1 && here_calls == 0 && gate_calls == 0 && key_calls == 0);
    assert(line_calls == 3 && near_calls == 15);

    /* kind 0, no round below 14: six rounds, then -1 */
    l2_reset(0);
    line_mode = 1;
    assert(TaLogic002_GetSummonPiece(SIDE, 9, &block, -1) == -1);
    assert(line_calls == 6 && near_calls == 48);

    /* kind 3: mode 2 and four rounds */
    l2_reset(3);
    line_mode = 2;
    assert(TaLogic002_GetSummonPiece(SIDE, 9, &block, -1) == -1);
    assert(line_calls == 4 && near_calls == 32);

    /* kind 4: the fourth round still answers; 13 is below the bound */
    l2_reset(4);
    line_mode = 2;
    hit_round = 3;
    near_costs[1] = 13;
    assert(TaLogic002_GetSummonPiece(SIDE, 9, &block, -1) == spots[1]);
    assert(line_calls == 4);

    /* any other kind: the last free position that is not field 8 or 9 and has no id */
    l2_reset(1);
    kinds[7] = 8;
    kinds[6] = 9;
    ids[5] = 0;
    assert(TaLogic002_GetSummonPiece(SIDE, 9, &block, PACKED) == spots[4]);
    assert(corner_calls == 1 && here_calls == 0 && line_calls == 0 && gate_calls == 0 && id_calls == 6);

    /* kinds 2 and 5..7 take the same stage; none free gives -1 */
    l2_reset(2);
    for (i = 0; i < 8; i++) {
        ids[i] = 5;
    }
    assert(TaLogic002_GetSummonPiece(SIDE, 9, &block, -1) == -1);
    for (i = 5; i < 8; i++) {
        l2_reset(i);
        for (last = 1; last < 8; last++) {
            ids[last] = 5;
        }
        assert(TaLogic002_GetSummonPiece(SIDE, 9, &block, -1) == spots[0]);
        assert(line_calls == 0);
    }

    fixed = 0;
    corner_mode = 0;
    n_now = 0;
}

int SzDuel_Kind2Type(int value)
{
    type_calls++;
    type_arg = value;
    return type_answer;
}

int TaLogic_GetWarnHideLvl__00153ed0(int s) { return key(7, s); }
int TaLogic_GetWarnHideLvl__0014b330(int s) { return key(8, s); }

int TaLogic013_GetDefensePoint(int index)
{
    assert(index >= 0 && index < 5);
    def_calls++;
    return 0x0a01 + index;
}

/* the first stage both functions share: level 0 or a level of type 2 */
static void check_typed_first(int (*fn)(int, long, struct DotrFreeBlock *, int), int id)
{
    reset(id);
    assert(fn(SIDE, 0, NULL, PACKED) == -1);
    assert(here_calls == 0 && id_calls == 0);

    /* level 0: the type is not asked; strictly lower cost wins, earliest on a tie */
    reset(id);
    type_calls = 0;
    costs[2] = 40;
    costs[5] = 40;
    costs[6] = 41;
    assert(fn(SIDE, 9, &block, PACKED) == spots[2]);
    assert(type_calls == 0 && here_calls == 1 && gate_calls == 8 && key_calls == 8);
    assert(short_calls == 8 && long_calls == 0 && reach_calls == 8);

    /* positive gate answer: the long cost path, the key asked twice */
    reset(id);
    gate_answer = 1;
    costs[7] = 3;
    assert(fn(SIDE, 0, &block, PACKED) == spots[7]);
    assert(long_calls == 8 && short_calls == 0 && key_calls == 16);

    /* positions the first stage skips: negative, kind 8, kind 9 held, filled */
    reset(id);
    spots[0] = -1;
    kinds[1] = 8;
    kinds[2] = 9;
    held[2] = 1;
    filled[3] = 1;
    kinds[4] = 9;
    held[4] = 2;
    costs[4] = 9;
    costs[1] = costs[2] = costs[3] = 1;
    assert(fn(SIDE, 0, &block, PACKED) == spots[4]);
    assert(gate_calls == 4 && short_calls == 4);

    /* level 1 of type 2 runs the first stage too; the level is what is asked */
    reset(id);
    block.kind = 0x3f;
    type_answer = 2;
    assert(fn(SIDE, 0, &block, PACKED) == spots[0]);
    assert(type_calls == 1 && type_arg == 1 && here_calls == 1);

    /* any other type: the last free position below row 8, without the first stage */
    reset(id);
    block.kind = 0xe0;
    type_answer = 1;
    type_calls = 0;
    ids[2] = ids[6] = ids[7] = -1;
    kinds[5] = 8;
    ids[5] = -1;
    assert(fn(SIDE, 0, &block, PACKED) == spots[6]);
    assert(type_calls == 1 && type_arg == 7 && here_calls == 0 && gate_calls == 0);
    reset(id);
    block.kind = 0x20;
    type_answer = 3;
    ids[7] = -1;
    assert(fn(SIDE, 0, &block, PACKED) == -1);
}

/* 0x151cf0: after the first stage, the first position with a negative id */
static void check_logic015(void)
{
    int i;

    fixed = 0;
    check_typed_first(TaLogic015_GetSummonPiece, 7);

    /* unreachable or costing 255: nothing taken, the place is asked again */
    reset(7);
    for (i = 0; i < 8; i++) {
        reach[i] = i & 1;
        costs[i] = 255;
    }
    spots[0] = -1;
    kinds[1] = 8;
    kinds[2] = 9;
    held[2] = 1;
    kinds[3] = 9;
    ids[3] = 0;
    ids[5] = -1;
    ids[6] = -1;
    assert(TaLogic015_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[5]);
    assert(here_calls == 2 && id_calls == 3);

    /* a negative packed value skips the first stage */
    reset(7);
    packed_now = -1;
    assert(TaLogic015_GetSummonPiece(SIDE, 0, &block, -1) == -1);
    assert(here_calls == 1 && gate_calls == 0 && id_calls == 8);
    packed_now = PACKED;
}

/* 0x1491b0: after the first stage, the free position cheapest towards the first open target place */
static void check_logic013(void)
{
    int i;

    fixed = 3;
    n_now = 3;
    def_mode = 1;
    memset(def_filled, 0, sizeof(def_filled));
    check_typed_first(TaLogic013_GetSummonPiece, 8);

    /* nothing taken: free positions are costed towards the first unfilled place */
    reset(8);
    for (i = 0; i < 8; i++) {
        reach[i] = 0;
    }
    def_filled[0] = 1;
    def_calls = 0;
    spots[0] = -1;
    kinds[2] = 8;
    ids[2] = -1;
    kinds[3] = 9;
    held[3] = 1;
    ids[3] = -1;
    ids[1] = ids[4] = ids[5] = -1;
    near_costs[1] = 9;
    near_costs[4] = 5;
    near_costs[5] = 5;
    assert(TaLogic013_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[4]);
    assert(near_calls == 3 && to_calls == 3 && def_calls == 9 && here_calls == 1);
    assert(to_at == 0x0a02 && near_tos[4] == 0x0a02);

    /* a negative id at the place is passed as 0xffff; negative costs and 14 are not taken */
    reset(8);
    for (i = 0; i < 8; i++) {
        reach[i] = 0;
    }
    to_id = -2;
    ids[1] = ids[4] = ids[5] = -1;
    near_costs[1] = -1;
    assert(TaLogic013_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);
    assert(near_calls == 3);
    near_costs[5] = 13;
    kinds[5] = 9;
    held[5] = 2;
    assert(TaLogic013_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[5]);

    /* every place filled: the place is -1 */
    reset(8);
    for (i = 0; i < 5; i++) {
        def_filled[i] = 1;
    }
    def_calls = 0;
    ids[1] = -1;
    near_costs[1] = 3;
    assert(TaLogic013_GetSummonPiece(SIDE, 0, &block, -1) == spots[1]);
    assert(def_calls == 5 && to_at == -1 && here_calls == 0 && gate_calls == 0);

    def_mode = 0;
    n_now = 0;
    fixed = 0;
}

/* 0x12e3f0: counts 2 and 5, and a second stage that goes for the place nearest the other side */
static void check_logic007(void)
{
    int i;

    fixed = 0;
    n_now = 2;
    n_long = 5;
    enemy_mode = 1;
    /* null block */
    reset(0);
    enemy_calls = 0;
    assert(TaLogic007_GetSummonPiece(SIDE, 0, NULL, PACKED) == -1);
    assert(here_calls == 0 && id_calls == 0 && enemy_calls == 0);

    /* first stage, short path: lowest cost, earliest on a tie */
    reset(0);
    costs[2] = 40;
    costs[5] = 40;
    assert(TaLogic007_GetSummonPiece(SIDE, 9, &block, PACKED) == spots[2]);
    assert(here_calls == 1 && gate_calls == 8 && key_calls == 8 && short_calls == 8 && long_calls == 0 && id_calls == 0);

    /* long path on a positive gate answer: the key callee is called again */
    reset(0);
    gate_answer = 1;
    costs[7] = 3;
    assert(TaLogic007_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[7]);
    assert(long_calls == 8 && short_calls == 0 && key_calls == 16);

    /* byte +0x3e values 3 and 2 run the first stage too */
    reset(0);
    block.kind = 0x7f;
    costs[1] = 7;
    assert(TaLogic007_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[1]);
    assert(here_calls == 1);
    reset(0);
    block.kind = 0x40;
    costs[6] = 7;
    assert(TaLogic007_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[6]);
    assert(here_calls == 1);

    /* first-stage skips: negative, kind 8, kind 9 held, filled; kind 9 not held counts */
    reset(0);
    spots[0] = -1;
    kinds[1] = 8;
    kinds[2] = 9;
    held[2] = 1;
    filled[3] = 1;
    kinds[4] = 9;
    held[4] = 2;
    costs[4] = 9;
    costs[1] = costs[2] = costs[3] = 1;
    assert(TaLogic007_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[4]);
    assert(gate_calls == 4 && short_calls == 4);

    /* unreachable or cost 255: not taken; 254 is */
    reset(0);
    costs[0] = 10;
    reach[0] = 0;
    costs[1] = 255;
    costs[3] = 254;
    for (i = 4; i < 8; i++) {
        costs[i] = 300;
    }
    costs[2] = 256;
    assert(TaLogic007_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[3]);

    /* nothing taken in the first stage: -1, and no later stage runs */
    reset(0);
    enemy_calls = 0;
    for (i = 0; i < 8; i++) {
        reach[i] = 0;
        ids[i] = -1;
    }
    assert(TaLogic007_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);
    assert(reach_calls == 8 && id_calls == 0 && enemy_calls == 0);

    /* negative packed value: the position with a negative id nearest the other side's place
     * (column 7, row 6), columns and rows added; index 5 is on it */
    reset(0);
    for (i = 0; i < 8; i++) {
        ids[i] = -1;
    }
    assert(TaLogic007_GetSummonPiece(SIDE, 0, &block, -1) == spots[5]);
    assert(here_calls == 0 && gate_calls == 0 && id_calls == 8 && enemy_calls == 1);

    /* skips as in the first stage, a non-negative id, and the earliest on a tie (4 and 6) */
    reset(0);
    for (i = 0; i < 8; i++) {
        ids[i] = -1;
    }
    ids[5] = 0;
    assert(TaLogic007_GetSummonPiece(SIDE, 0, &block, -1) == spots[4]);
    kinds[4] = 8;
    assert(TaLogic007_GetSummonPiece(SIDE, 0, &block, -1) == spots[6]);
    kinds[6] = 9;
    held[6] = 1;
    spots[3] = -1;
    assert(TaLogic007_GetSummonPiece(SIDE, 0, &block, -1) == spots[7]);
    kinds[6] = 9;
    held[6] = 0;
    assert(TaLogic007_GetSummonPiece(SIDE, 0, &block, -1) == spots[6]);

    /* each of the four orderings of column and row against the other side's place */
    reset(0);
    spots[0] = (5 << 8) | 9;
    spots[1] = (9 << 8) | 5;
    ids[0] = -1;
    assert(TaLogic007_GetSummonPiece(SIDE, 0, &block, -1) == spots[0]);
    ids[0] = 5;
    ids[1] = -1;
    assert(TaLogic007_GetSummonPiece(SIDE, 0, &block, -1) == spots[1]);

    /* distance bound: 13 away is taken, 14 is not and the answer is -1 */
    reset(0);
    spots[0] = (19 << 8) | 7;
    ids[0] = -1;
    assert(TaLogic007_GetSummonPiece(SIDE, 0, &block, -1) == spots[0]);
    spots[0] = (20 << 8) | 7;
    assert(TaLogic007_GetSummonPiece(SIDE, 0, &block, -1) == -1);

    /* no position with a negative id */
    reset(0);
    assert(TaLogic007_GetSummonPiece(SIDE, 0, &block, -1) == -1);
    assert(id_calls == 8);

    /* other values of byte +0x3e: the LAST position with a negative id in a row below 8 */
    reset(0);
    enemy_calls = 0;
    block.kind = 0x20;
    ids[1] = -1;
    ids[6] = -1;
    ids[7] = -1;
    assert(TaLogic007_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[6]);
    assert(here_calls == 0 && gate_calls == 0 && id_calls == 8 && enemy_calls == 0);
    /* kind 9 is skipped here whether held or not */
    reset(0);
    block.kind = 0x3f;
    ids[7] = -1;
    kinds[2] = 8;
    ids[2] = -1;
    kinds[3] = 9;
    ids[3] = -1;
    assert(TaLogic007_GetSummonPiece(SIDE, 0, &block, -1) == -1);
    reset(0);
    block.kind = 0x80;
    spots[0] = 2;
    ids[0] = -1;
    assert(TaLogic007_GetSummonPiece(SIDE, 0, &block, -1) == spots[0]);

    enemy_mode = 0;
    n_now = 0;
    n_long = 0;
}

int main(void)
{
    check_logic007();
    check_logic015();
    check_logic013();
    check_logic002();
    check_rounds();
    check_towards();
    check_packed();
    check_best();
    check_rows();
    check_near();
    check_levels();
    check_fixed();
    check_copy(TaLogic018_GetSummonPiece, 1);
    check_copy(TaLogic019_GetSummonPiece, 2);
    check_copy(TaLogic022_GetSummonPiece, 3);
    return 0;
}
