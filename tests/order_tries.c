/* Synthetic host tests for the order_tries group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/order_tries.h"

#define POSITION 0x0305
#define HERE 0x0204
#define THERE 0x0a0b

static int word_calls;
static int first_word;
static int side_answer;
static int side_calls;
static int cell_calls;
static void *buf_seen;
static int buf_calls;
static int test_answer;
static int test_calls;
static int test_args[9];
static int callee_id;
static int callee_arg;
static int callee_calls;
static int answers[8];
static int tries;
static int try_id;
static int try_count;
static struct DotrTryOrder *try_order;
static int last_id;
static int last_calls;
static struct DotrTryOrder *last_order;
static int here_answer = HERE;
static int there_answer = THERE;
static int record_calls;
static int pick_answer;
static int pick_calls;
static int pair_calls;
static int scan_answer;
static int scan_calls;
/* Grid for 0x11d7b0: the cell for line y is row y + 1, column y + 10. */
static int grid_mode;
static int grid_x;
static int grid_cell[8];
static int grid_owner[8];
static int grid_word[8];
static int grid_calls;
static int grid_last;

int func_001df540_word(int a, int b)
{
    assert(a == (POSITION >> 8) && b == (POSITION & 0xff));
    return word_calls++ == 0 ? first_word : 0x1234;
}

int func_0024de70_pair(int a, int b)
{
    assert(a == (POSITION >> 8) && b == (POSITION & 0xff));
    side_calls++;
    return side_answer;
}

int SzDuel_GetUnitLoc(int a, int b)
{
    cell_calls++;
    if (a == (POSITION >> 8) && b == (POSITION & 0xff)) {
        return here_answer;
    }
    assert(a == (side_answer == 0 ? 1 : 0) && b == 0);
    return there_answer;
}

void func_0027e770_buf(void *buf)
{
    buf_seen = buf;
    buf_calls++;
}

int TaLogic_IsRootOpen(int a, int b, int c, int d, int e, int f, int g, int h, unsigned char i)
{
    test_args[0] = a, test_args[1] = b, test_args[2] = c, test_args[3] = d;
    test_args[4] = e, test_args[5] = f, test_args[6] = g, test_args[7] = h, test_args[8] = i;
    test_calls++;
    return test_answer;
}

static int side_callee(int id, int side)
{
    callee_id = id;
    callee_arg = side;
    callee_calls++;
    return 77;
}

int TaLogic_GetWarnHideLvl__0011fa80(int side) { return side_callee(1, side); }
int TaLogic_GetWarnHideLvl__0012b1b0(int side) { return side_callee(2, side); }
int TaLogic_GetWarnHideLvl__00141b30(int side) { return side_callee(3, side); }
int func_0016b0e0_arg(int side) { return side_callee(4, side); }
int TaLogic_GetWarnHideLvl__00162580(int side) { return side_callee(5, side); }

/* Set for 0x135d50, which has no side callee and passes 0 instead. */
static int callee_free;

static int forwarder(int id, struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5,
                     int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a,
                     unsigned short *b, int n, void *buf)
{
    int i;

    try_id = id;
    try_order = order;
    try_count = n;
    assert(r1 == 0 && r2 == 0x1234 && r3 == (here_answer & 0xff) && r4 == (here_answer >> 8));
    assert(r5 == (side_answer == 0 ? 1 : 0));
    assert(r6 == tries + 1);
    if (callee_free) {
        assert(r7 == 0 && callee_calls == 0);
    } else {
        assert(r7 == 77 && callee_calls == tries + 1 && callee_arg == side_answer);
    }
    for (i = 0; i < 22; i++) {
        assert(cells[i].id == 0xffff && cells[i].kind == 0xff);
    }
    assert(s1 == 0 && *a == 0 && *b == 0 && a != b);
    assert(buf == buf_seen && buf_calls == 1);
    return answers[tries++];
}

int func_0011d380_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf)
{
    return forwarder(1, order, r1, r2, r3, r4, r5, r6, r7, cells, s1, a, b, n, buf);
}

int func_00127a30_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf)
{
    return forwarder(2, order, r1, r2, r3, r4, r5, r6, r7, cells, s1, a, b, n, buf);
}

int func_0013ea20_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf)
{
    return forwarder(3, order, r1, r2, r3, r4, r5, r6, r7, cells, s1, a, b, n, buf);
}

int func_001684d0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf)
{
    return forwarder(4, order, r1, r2, r3, r4, r5, r6, r7, cells, s1, a, b, n, buf);
}

static int deep_answer;
static int kind_answer;
static int kind_calls;

int TaLogic015_IsActiveMode(void)
{
    return deep_answer;
}

int SzDuel_GetUnitCardNo(int a, int b)
{
    assert(a == (POSITION >> 8) && b == (POSITION & 0xff));
    kind_calls++;
    return kind_answer;
}

int TaLogic_GetWarnHideLvl__00153ed0(int side) { return side_callee(6, side); }

int func_00150be0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf)
{
    return forwarder(6, order, r1, r2, r3, r4, r5, r6, r7, cells, s1, a, b, n, buf);
}

int TaLogic_GetWarnHideLvl__0015d120(int side) { return side_callee(8, side); }
int TaLogic_GetWarnHideLvl__00130780(int side) { return side_callee(9, side); }

int func_0012cb80_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf)
{
    return forwarder(9, order, r1, r2, r3, r4, r5, r6, r7, cells, s1, a, b, n, buf);
}

int func_00135fb0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf)
{
    return forwarder(7, order, r1, r2, r3, r4, r5, r6, r7, cells, s1, a, b, n, buf);
}

int func_00159f80_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf)
{
    return forwarder(8, order, r1, r2, r3, r4, r5, r6, r7, cells, s1, a, b, n, buf);
}

int func_0015e8f0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf)
{
    return forwarder(5, order, r1, r2, r3, r4, r5, r6, r7, cells, s1, a, b, n, buf);
}

static void fallback(int id, struct DotrTryOrder *order)
{
    last_id = id;
    last_order = order;
    last_calls++;
}

/* 0x11d3e0, the first copy's fallback, is not stubbed: its first call is
 * 0x1de540, counted here. */
static int record_answer;

int SzDuel_GetTurnOwner(void)
{
    record_calls++;
    return record_answer;
}

void TaLogic004_GetNearAtkLineEdge(int word, int column, int row, struct DotrTryPick *out)
{
    assert(word == 0x4321 && column == (here_answer & 0xff) && row == (here_answer >> 8));
    out->word = pick_answer;
    pick_calls++;
}

void func_00127a90_order(struct DotrTryOrder *order) { fallback(2, order); }
/* 0x13ea80, the third copy's fallback, is not stubbed either. */
int func_00250360_pair(int a, int b)
{
    if (grid_mode) {
        assert(a >= 1 && a <= 8 && b == a + 9);
        return grid_owner[a - 1];
    }
    assert(a == (POSITION >> 8) && b == (POSITION & 0xff));
    pair_calls++;
    return 0x55;
}

int TaLogic011_GetNearMountain(int word, int column, int row)
{
    assert(word == 0x4321 || word == 0x1234);
    assert(column == (here_answer & 0xff) && row == (here_answer >> 8));
    scan_calls++;
    return scan_answer;
}

void TaLogic021_CommandInvadeUnit(struct DotrTryOrder *order) { fallback(4, order); }

/* Callees of the five later copies (ids 10-14). */
int TaLogic_GetWarnHideLvl__001676d0(int side) { return side_callee(10, side); }
int TaLogic_GetWarnHideLvl__001254a0(int side) { return side_callee(11, side); }
int TaLogic_GetWarnHideLvl__0011aa10(int side) { return side_callee(12, side); }
int TaLogic_GetWarnHideLvl__001461c0(int side) { return side_callee(13, side); }
int TaLogic_GetWarnHideLvl__00172310(int side) { return side_callee(14, side); }
int func_00163ec0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf)
{
    return forwarder(10, order, r1, r2, r3, r4, r5, r6, r7, cells, s1, a, b, n, buf);
}
int func_001224d0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf)
{
    return forwarder(11, order, r1, r2, r3, r4, r5, r6, r7, cells, s1, a, b, n, buf);
}
int func_00118b60_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf)
{
    return forwarder(12, order, r1, r2, r3, r4, r5, r6, r7, cells, s1, a, b, n, buf);
}
int func_00143790_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf)
{
    return forwarder(13, order, r1, r2, r3, r4, r5, r6, r7, cells, s1, a, b, n, buf);
}
int func_0016e4b0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf)
{
    return forwarder(14, order, r1, r2, r3, r4, r5, r6, r7, cells, s1, a, b, n, buf);
}
void func_00163f20_order(struct DotrTryOrder *order) { fallback(10, order); }
void func_00122530_order(struct DotrTryOrder *order) { fallback(11, order); }
void func_00118bc0_order(struct DotrTryOrder *order) { fallback(12, order); }

static int amount_answer;
static int amount_calls;

int func_001df470_side(int side)
{
    assert(side == side_answer);
    amount_calls++;
    return amount_answer;
}

int TaLogic_GetWarnHideLvl__00134ae0(int side) { return side_callee(15, side); }
int func_00131de0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf)
{
    return forwarder(15, order, r1, r2, r3, r4, r5, r6, r7, cells, s1, a, b, n, buf);
}
void func_00131e40_order(struct DotrTryOrder *order) { fallback(15, order); }


static int last_deep;

int TaLogic_GetWarnHideLvl__00117c30(int side) { return side_callee(16, side); }
int func_00113a00_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf)
{
    return forwarder(16, order, r1, r2, r3, r4, r5, r6, r7, cells, s1, a, b, n, buf);
}
void func_00113a60_order(struct DotrTryOrder *order, int deep)
{
    last_deep = deep;
    fallback(16, order);
}

int TaLogic_GetWarnHideLvl__0013da70(int side) { return side_callee(17, side); }
int func_0013a6b0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf)
{
    return forwarder(17, order, r1, r2, r3, r4, r5, r6, r7, cells, s1, a, b, n, buf);
}
void func_0013a710_order(struct DotrTryOrder *order, int deep)
{
    last_deep = deep;
    fallback(17, order);
}

static int column_answer;
static int column_calls;
static int free_answer;
static int free_calls;
static int near_calls;

int TaLogic_GetWarnHideLvl__0014b330(int side) { return side_callee(18, side); }
int func_001477d0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf)
{
    return forwarder(18, order, r1, r2, r3, r4, r5, r6, r7, cells, s1, a, b, n, buf);
}

int TaLogic013_IsDefensePoint(int column, int row)
{
    assert(column == (HERE & 0xff) && row == (HERE >> 8));
    column_calls++;
    return column_answer;
}

int TaLogic013_GetEmptyLine(void)
{
    free_calls++;
    return free_answer;
}

int TaLogic013_GetInvadePoint(int a, int b, int c)
{
    assert(a == side_answer && b == (HERE & 0xff) && c == (HERE >> 8));
    near_calls++;
    return 0x0607;
}

static int alone_answer;
static int alone_calls;

int TaLogic_GetWarnHideLvl__0014f9b0(int side) { return side_callee(19, side); }
int func_0014c6c0_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf)
{
    return forwarder(19, order, r1, r2, r3, r4, r5, r6, r7, cells, s1, a, b, n, buf);
}

int TaLogic014_IsLinePoint(int column, int row)
{
    assert(column == (HERE & 0xff) && row == (HERE >> 8));
    column_calls++;
    return column_answer;
}

int TaLogic014_IsActiveMode(void)
{
    alone_calls++;
    return alone_answer;
}

int TaLogic014_GetEmptyLine(void)
{
    free_calls++;
    return free_answer;
}

/* Per-copy differences for check_tries (all 0 for the earlier copies): 0x1188f0
 * calls 0x27e770 after the 0x27b200 test; 0x122270 asks 0x27b200 with 2 while
 * trying three times, and stores a bare 0x2ff for answer 3. */
static int buf_late;
static int ask_count;
static int three_plain;
/* Set for 0x13a400, where answer 0 stores as answer 2 does. */
static int zero_here;

typedef void (*Tries)(struct DotrTryOrder *order, int flag);

static struct DotrTryOrder order;

static void run(Tries f, int flag, int word, int side, int test)
{
    memset(&order, 0, sizeof(order));
    order.position = POSITION;
    order.unknown_04 = 0x0f11;
    order.flags.word = 0x7777abcd;
    word_calls = side_calls = cell_calls = buf_calls = test_calls = 0;
    callee_calls = tries = last_calls = record_calls = 0;
    pair_calls = scan_calls = 0;
    scan_answer = -1;
    callee_id = try_id = last_id = 0;
    first_word = word;
    side_answer = side;
    test_answer = test;
    f(&order, flag);
}

static void set_answers(int a0, int a1, int a2, int a3, int a4, int a5)
{
    answers[0] = a0, answers[1] = a1, answers[2] = a2;
    answers[3] = a3, answers[4] = a4, answers[5] = a5;
}

static void check_tries(Tries f, int id, int n)
{
    int k;

    /* A zero cell word: nothing else is asked or stored. */
    set_answers(0, 0, 0, 0, 0, 0);
    run(f, 1, 0, 0, 1);
    assert(word_calls == 1 && side_calls == 0 && cell_calls == 0 && buf_calls == 0);
    assert(test_calls == 0 && tries == 0 && last_calls == 0);
    assert(order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);

    /* Flag set and a positive 0x27b200 answer, side 0: other is row 1. */
    run(f, 1, 9, 0, 1);
    assert(word_calls == 2 && side_calls == 1 && cell_calls == 2 && buf_calls == (buf_late ? 0 : 1));
    assert(test_calls == 1 && tries == 0 && last_calls == 0);
    assert(test_args[0] == (POSITION >> 8) && test_args[1] == (POSITION & 0xff));
    assert(test_args[2] == (HERE & 0xff) && test_args[3] == (HERE >> 8));
    assert(test_args[4] == 1 && test_args[5] == 0);
    assert(test_args[6] == (THERE & 0xff) && test_args[7] == (THERE >> 8) && test_args[8] == (ask_count ? ask_count : n));
    assert(order.unknown_04 == 0x2ff && order.flags.word == 0x0100abcd);
    /* The same with a non-zero side: other is row 0. */
    run(f, 1, 9, 2, 1);
    assert(test_args[4] == 0 && test_args[5] == 0);
    assert(order.unknown_04 == 0x2ff && order.flags.word == 0x0000abcd);

    /* Flag set and a zero 0x27b200 answer: the first try is made (answer 0). */
    run(f, 1, 9, 0, 0);
    assert(test_calls == 1 && tries == 1 && try_id == id && try_order == &order && try_count == n);
    assert((callee_free ? callee_calls == 0 : callee_id == id) && side_calls == 2 && last_calls == 0);
    assert(order.unknown_04 == (zero_here ? 0x3ff : 0x5ff));
    assert(order.flags.word == (zero_here ? ((HERE << 16) | 0xabcd) : 0x7777abcd));
    /* Flag clear: 0x27b200 is not asked, whatever it would answer. */
    run(f, 0, 9, 3, 1);
    assert(test_calls == 0 && tries == 1);
    assert(order.unknown_04 == (zero_here ? 0x3ff : 0x5ff));
    assert(order.flags.word == (zero_here ? ((HERE << 16) | 0xabcd) : 0x7777abcd));

    /* Answer 1 stores nothing. */
    set_answers(1, 0, 0, 0, 0, 0);
    run(f, 0, 9, 0, 0);
    assert(tries == 1 && last_calls == 0);
    assert(order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    /* Answer 2. */
    set_answers(2, 0, 0, 0, 0, 0);
    run(f, 0, 9, 0, 0);
    assert(tries == 1 && last_calls == 0);
    assert(order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    /* Answer 3. */
    set_answers(3, 0, 0, 0, 0, 0);
    run(f, 0, 9, 0, 0);
    assert(tries == 1 && last_calls == 0);
    if (three_plain) {
        assert(order.unknown_04 == 0x2ff && order.flags.word == 0x7777abcd);
    } else {
        assert(order.unknown_04 == 0x6ff && order.flags.word == ((HERE << 16) | 0xabcd));
    }

    /* Other answers go on to the next try; a listed answer on try N ends it. */
    set_answers(4, -1, 4, -1, 4, -1);
    answers[n - 1] = 2;
    run(f, 0, 9, 0, 0);
    assert(tries == n && side_calls == n + 1 && last_calls == 0);
    assert(order.unknown_04 == 0x3ff);
    /* N other answers: the fallback and no further try. The stubs store
     * nothing, the real 0x11d3e0 leaves a +4 high byte of 15 and the real
     * 0x13ea80 stores 0x5ff for a negative 0x13eb20 answer. */
    set_answers(4, -1, 4, -1, 4, -1);
    for (k = n; k < 6; k++) {
        answers[k] = 0;
    }
    run(f, 0, 9, 0, 0);
    assert(tries == n);
    if (id == 7 || id == 9) {
        /* 0x135d50 and 0x12c7f0 have no fallback callee: they store 0x5ff themselves. */
        assert(record_calls == 0 && pair_calls == 0 && last_calls == 0);
    } else if (id == 1) {
        assert(record_calls == 1 && last_calls == 0);
    } else if (id == 3) {
        assert(pair_calls == 1 && scan_calls == 1 && last_calls == 0);
    } else {
        assert(record_calls == 0 && last_calls == 1 && last_id == id && last_order == &order);
    }
    assert(order.unknown_04 == (id == 3 || id == 7 || id == 9 ? 0x5ff : 0x0f11) && order.flags.word == 0x7777abcd);
}

/* The real 0x11d6d0 answers from the same 0x1e1090 halfword: an odd column
 * makes it negative, column 4 with rows 2..4 gives 1. */
static void fall(int half, int here, int pick)
{
    memset(&order, 0, sizeof(order));
    order.position = POSITION;
    order.unknown_04 = half;
    order.flags.word = 0x7777abcd;
    word_calls = record_calls = pick_calls = 0;
    first_word = 0x4321;
    here_answer = here;
    pick_answer = pick;
    TaLogic004_CommandAtkLineUnit(&order);
    here_answer = HERE;
    assert(record_calls == 1);
}

static void check_fallback(void)
{
    /* Negative finder: the pick word decides (0 is the boundary). */
    fall(0x1111, 0x0205, 0x21);
    assert(word_calls == 1 && pick_calls == 1);
    assert(order.unknown_04 == 0x6ff && order.flags.word == 0x0021abcd);
    fall(0x1111, 0x0205, 0);
    assert(order.unknown_04 == 0x6ff && order.flags.word == 0x0000abcd);
    fall(0x1111, 0x0205, -1);
    assert(pick_calls == 1);
    assert(order.unknown_04 == 0x5ff && order.flags.word == 0x7777abcd);
    /* Finder 0 or more: a +4 high byte of 15 or 16 is left alone. */
    fall(0x0f22, 0x0202, 0x21);
    assert(word_calls == 0 && pick_calls == 0);
    assert(order.unknown_04 == 0x0f22 && order.flags.word == 0x7777abcd);
    fall(0x1022, HERE, 0x21);
    assert(order.unknown_04 == 0x1022 && order.flags.word == 0x7777abcd);
    /* Otherwise the row of the halfword picks 15 (below 4) or 16. */
    fall(0x0e22, 0x0304, 0x21);
    assert(pick_calls == 0);
    assert(order.unknown_04 == 0x0fff && order.flags.word == (int)0xffffabcd);
    fall(0x1122, 0x0404, 0x21);
    assert(order.unknown_04 == 0x10ff && order.flags.word == (int)0xffffabcd);
}

static void check_column_rows(void)
{
    static const struct {
        int here;
        int want;
    } cases[] = {
        {-0x100, -1}, {0x0000, 3}, {0x0600, 3}, {0x0700, -1},
        {0x0102, -1}, {0x0202, 0}, {0x0402, 0}, {0x0502, -1},
        {0x0104, -1}, {0x0204, 1}, {0x0404, 1}, {0x0504, -1},
        {-0x100 + 6, -1}, {0x0006, 4}, {0x0606, 4}, {0x0706, -1},
        {0x0201, -1}, {0x0208, -1},
    };
    size_t i;

    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        here_answer = cases[i].here;
        cell_calls = 0;
        assert(TaLogic004_IsUnitOnLine(POSITION >> 8, POSITION & 0xff) == cases[i].want);
        assert(cell_calls == 1);
    }
    here_answer = HERE;
}

static int position_bits;
static int position_calls;

static int below_mode;
static int below_answer;
static int below_calls;

int SzDuel_GetMapUnitID(int x, int y)
{
    if (below_mode) {
        assert(x == (here_answer & 0xff) && y == (here_answer >> 8) + 1);
        below_calls++;
        return below_answer;
    }
    assert(x == grid_x && y >= 0 && y < 8);
    assert(grid_calls == 0 || y == grid_last + 1);
    grid_last = y;
    grid_calls++;
    return grid_cell[y];
}

int SzDuel_GetUnitKind(int a, int b)
{
    /* 0x150850 asks about the order's position, which is not on the line grid. */
    if (a == (POSITION >> 8) && b == (POSITION & 0xff)) {
        position_calls++;
        return position_bits;
    }
    assert(a >= 1 && a <= 8 && b == a + 9);
    return grid_word[a - 1];
}

static int line(int kind, int at, int owner, int word)
{
    int y;
    int r;

    for (y = 0; y < 8; y++) {
        grid_cell[y] = -1;
        grid_owner[y] = 7;
        grid_word[y] = 0;
    }
    if (at >= 0) {
        grid_cell[at] = ((at + 1) << 8) | (at + 10);
        grid_owner[at] = owner;
        grid_word[at] = word;
    }
    grid_mode = 1;
    grid_calls = 0;
    r = TaLogic004_IsEmptyAtkLine(7, kind);
    grid_mode = 0;
    return r;
}

static void check_lines(void)
{
    static const struct {
        int kind;
        int x;
        int lo;
        int hi;
    } kinds[] = {
        {0, 2, 2, 4}, {1, 4, 2, 4}, {2, 0, 1, 5}, {3, 6, 1, 5}, {9, 6, 1, 5}, {0x102, 0, 1, 5},
    };
    size_t i;

    for (i = 0; i < sizeof(kinds) / sizeof(kinds[0]); i++) {
        int lo = kinds[i].lo;
        int hi = kinds[i].hi;
        int n = hi - lo + 1;

        grid_x = kinds[i].x;
        /* No cell: every line from lo to hi is asked once. */
        assert(line(kinds[i].kind, -1, 7, 0) == 1);
        assert(grid_calls == n && grid_last == hi);
        /* A cell of this side with a zero word >> 5, on the first and last line. */
        assert(line(kinds[i].kind, lo, 7, 0) == 0);
        assert(grid_calls == 1);
        assert(line(kinds[i].kind, hi, 7, 31) == 0);
        assert(grid_calls == n);
        /* Lines outside the range are not asked. */
        assert(line(kinds[i].kind, lo - 1, 7, 0) == 1);
        assert(line(kinds[i].kind, hi + 1, 7, 0) == 1);
        assert(grid_calls == n);
        /* Another side, or a word of 32 or more: the scan goes on. */
        assert(line(kinds[i].kind, lo, 6, 0) == 1);
        assert(grid_calls == n);
        assert(line(kinds[i].kind, lo, 7, 32) == 1);
        assert(grid_calls == n);
    }
}

static void scan(int answer)
{
    memset(&order, 0, sizeof(order));
    order.position = POSITION;
    order.unknown_04 = 0x1111;
    order.flags.word = 0x7777abcd;
    word_calls = pair_calls = scan_calls = 0;
    first_word = 0x4321;
    scan_answer = answer;
    TaLogic011_CommandDefenseUnit(&order);
    assert(word_calls == 1 && pair_calls == 1 && scan_calls == 1);
}

static void check_scan_fallback(void)
{
    scan(0x21);
    assert(order.unknown_04 == 0x6ff && order.flags.word == 0x0021abcd);
    scan(0);
    assert(order.unknown_04 == 0x6ff && order.flags.word == 0x0000abcd);
    scan(-1);
    assert(order.unknown_04 == 0x5ff && order.flags.word == 0x7777abcd);
}

/* 0x15e030 takes no flag, always asks TaLogic_IsRootOpen (count 5), asks 0x24de70 once
 * and has no fallback callee: after five other answers it stores by the two
 * 0x1e1090 rows. */
static void plain(int word, int side, int test, int here, int there)
{
    memset(&order, 0, sizeof(order));
    order.position = POSITION;
    order.unknown_04 = 0x0f11;
    order.flags.word = 0x7777abcd;
    word_calls = side_calls = cell_calls = buf_calls = test_calls = 0;
    callee_calls = tries = last_calls = 0;
    callee_id = try_id = 0;
    first_word = word;
    side_answer = side;
    test_answer = test;
    here_answer = here;
    there_answer = there;
    TaLogic019_CommandAtkUnit(&order);
    here_answer = HERE;
    there_answer = THERE;
}

static void check_plain(void)
{
    static const int rows[4][3] = {
        /* here, there, word stored above the +8 halfword */
        {0x0304, 0x030b, 0x030b}, /* both rows below 4: there */
        {0x0404, 0x040b, 0x040b}, /* both rows 4 or more: there */
        {0x0304, 0x040b, 0x0304}, /* only here below 4: 3 above here's column */
        {0x0409, 0x030b, 0x0309}, /* only there below 4 */
    };
    int k;

    /* A zero cell word: nothing else is asked or stored. */
    set_answers(0, 0, 0, 0, 0, 0);
    plain(0, 0, 1, HERE, THERE);
    assert(word_calls == 1 && side_calls == 0 && cell_calls == 0 && buf_calls == 0);
    assert(test_calls == 0 && tries == 0);
    assert(order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);

    /* A positive 0x27b200 answer, side 0: other is row 1. */
    plain(9, 0, 1, HERE, THERE);
    assert(word_calls == 2 && side_calls == 1 && cell_calls == 2 && buf_calls == 1);
    assert(test_calls == 1 && tries == 0);
    assert(test_args[0] == (POSITION >> 8) && test_args[1] == (POSITION & 0xff));
    assert(test_args[2] == (HERE & 0xff) && test_args[3] == (HERE >> 8));
    assert(test_args[4] == 1 && test_args[5] == 0);
    assert(test_args[6] == (THERE & 0xff) && test_args[7] == (THERE >> 8) && test_args[8] == 5);
    assert(order.unknown_04 == 0x2ff && order.flags.word == 0x0100abcd);
    /* The same with a non-zero side: other is row 0. */
    plain(9, 2, 1, HERE, THERE);
    assert(test_args[4] == 0 && test_args[5] == 0);
    assert(order.unknown_04 == 0x2ff && order.flags.word == 0x0000abcd);

    /* A zero and a negative 0x27b200 answer: the first try is made. Answer 0. */
    plain(9, 0, 0, HERE, THERE);
    assert(test_calls == 1 && tries == 1 && try_id == 5 && try_order == &order && try_count == 5);
    assert(callee_id == 5 && side_calls == 1);
    assert(order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    plain(9, 3, -1, HERE, THERE);
    assert(tries == 1 && order.unknown_04 == 0x3ff);
    /* Answer 1 stores nothing. */
    set_answers(1, 0, 0, 0, 0, 0);
    plain(9, 0, 0, HERE, THERE);
    assert(tries == 1 && order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    /* Answer 2. */
    set_answers(2, 0, 0, 0, 0, 0);
    plain(9, 0, 0, HERE, THERE);
    assert(tries == 1 && order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    /* Answer 3. */
    set_answers(3, 0, 0, 0, 0, 0);
    plain(9, 0, 0, HERE, THERE);
    assert(tries == 1 && order.unknown_04 == 0x6ff && order.flags.word == ((HERE << 16) | 0xabcd));
    /* Other answers go on; a listed answer on the fifth try ends it. */
    set_answers(4, -1, 4, -1, 2, -1);
    plain(9, 0, 0, HERE, THERE);
    assert(tries == 5 && order.unknown_04 == 0x3ff);
    /* Five other answers: no sixth try, 0x6ff and a word by the two rows. */
    for (k = 0; k < 4; k++) {
        set_answers(4, -1, 4, -1, 4, 0);
        plain(9, 0, 0, rows[k][0], rows[k][1]);
        assert(tries == 5 && order.unknown_04 == 0x6ff);
        assert(order.flags.word == ((rows[k][2] << 16) | 0xabcd));
    }
}

/* 0x150850: 0x15e030's shape with a try count from 0x153ab0, a 0x1e1030 stop,
 * 0x27b200 asked with 2, 0x24de70 asked again on every try and 0x1e0700 >> 5
 * choosing 0xbff. */
static void deep(int flag, int kind, int word, int side, int test, int here, int there, int bits)
{
    memset(&order, 0, sizeof(order));
    order.position = POSITION;
    order.unknown_04 = 0x0f11;
    order.flags.word = 0x7777abcd;
    word_calls = side_calls = cell_calls = buf_calls = test_calls = 0;
    callee_calls = tries = last_calls = 0;
    callee_id = try_id = 0;
    kind_calls = position_calls = 0;
    deep_answer = flag;
    kind_answer = kind;
    position_bits = bits;
    first_word = word;
    side_answer = side;
    test_answer = test;
    here_answer = here;
    there_answer = there;
    TaLogic015_CommandBtlUnit(&order);
    here_answer = HERE;
    there_answer = THERE;
}

static void check_deep(void)
{
    static const int rows[4][3] = {
        {0x0304, 0x030b, 0x030b},
        {0x0404, 0x040b, 0x040b},
        {0x0304, 0x040b, 0x0304},
        {0x0409, 0x030b, 0x0309},
    };
    int k;

    /* A zero cell word: nothing else is asked or stored. */
    set_answers(0, 0, 0, 0, 0, 0);
    deep(1, 0, 0, 0, 1, HERE, THERE, 0);
    assert(word_calls == 1 && side_calls == 0 && cell_calls == 0 && buf_calls == 0);
    assert(kind_calls == 0 && test_calls == 0 && tries == 0);
    assert(order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);

    /* 0x1e1030 answering 686: 0xcff, before 0x27b200 is asked. 685 and 687 go on. */
    deep(1, 686, 9, 0, 1, HERE, THERE, 0);
    assert(word_calls == 2 && side_calls == 1 && cell_calls == 2 && buf_calls == 1);
    assert(kind_calls == 1 && test_calls == 0 && tries == 0 && position_calls == 0);
    assert(order.unknown_04 == 0xcff && order.flags.word == 0x7777abcd);
    deep(1, 685, 9, 0, 1, HERE, THERE, 0);
    assert(test_calls == 1 && order.unknown_04 == 0x2ff);
    deep(1, 687, 9, 0, 1, HERE, THERE, 0);
    assert(test_calls == 1 && order.unknown_04 == 0x2ff);

    /* A positive 0x27b200 answer, side 0: other is row 1; 0x2ff while 0x1e0700 >> 5 is 0. */
    deep(1, 0, 9, 0, 1, HERE, THERE, 31);
    assert(test_calls == 1 && tries == 0 && position_calls == 1);
    assert(test_args[0] == (POSITION >> 8) && test_args[1] == (POSITION & 0xff));
    assert(test_args[2] == (HERE & 0xff) && test_args[3] == (HERE >> 8));
    assert(test_args[4] == 1 && test_args[5] == 0);
    assert(test_args[6] == (THERE & 0xff) && test_args[7] == (THERE >> 8) && test_args[8] == 2);
    assert(order.unknown_04 == 0x2ff && order.flags.word == 0x0100abcd);
    /* A non-zero side (other is row 0) and 0x1e0700 >> 5 non-zero: 0xbff. */
    deep(1, 0, 9, 2, 1, HERE, THERE, 32);
    assert(test_args[4] == 0 && test_args[5] == 0 && position_calls == 1);
    assert(order.unknown_04 == 0xbff && order.flags.word == 0x0000abcd);

    /* A zero and a negative 0x27b200 answer: the first try is made. Answer 0. */
    deep(1, 0, 9, 0, 0, HERE, THERE, 0);
    assert(test_calls == 1 && tries == 1 && try_id == 6 && try_order == &order && try_count == 5);
    assert(callee_id == 6 && side_calls == 2 && position_calls == 0);
    assert(order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    deep(0, 0, 9, 3, -1, HERE, THERE, 0);
    assert(tries == 1 && try_count == 2 && order.unknown_04 == 0x3ff);
    /* Answer 1 stores nothing, or 0xbff when 0x1e0700 >> 5 is non-zero. */
    set_answers(1, 0, 0, 0, 0, 0);
    deep(1, 0, 9, 0, 0, HERE, THERE, 31);
    assert(tries == 1 && position_calls == 1);
    assert(order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    deep(1, 0, 9, 0, 0, HERE, THERE, 32);
    assert(tries == 1 && position_calls == 1);
    assert(order.unknown_04 == 0xbff && order.flags.word == 0x7777abcd);
    /* Answer 2. */
    set_answers(2, 0, 0, 0, 0, 0);
    deep(1, 0, 9, 0, 0, HERE, THERE, 0);
    assert(tries == 1 && order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    /* Answer 3. */
    set_answers(3, 0, 0, 0, 0, 0);
    deep(1, 0, 9, 0, 0, HERE, THERE, 0);
    assert(tries == 1 && order.unknown_04 == 0x6ff && order.flags.word == ((HERE << 16) | 0xabcd));
    /* Other answers go on; a listed answer on the last try ends it (five tries, or two). */
    set_answers(4, -1, 4, -1, 2, -1);
    deep(1, 0, 9, 0, 0, HERE, THERE, 0);
    assert(tries == 5 && side_calls == 6 && order.unknown_04 == 0x3ff);
    set_answers(4, 2, 0, 0, 0, 0);
    deep(0, 0, 9, 0, 0, HERE, THERE, 0);
    assert(tries == 2 && side_calls == 3 && order.unknown_04 == 0x3ff);
    /* Only other answers: no further try, 0x6ff and a word by the two rows. Only the
     * low byte of the 0x153ab0 answer counts (0x100 gives two tries). */
    for (k = 0; k < 4; k++) {
        set_answers(4, -1, 4, -1, 4, 0);
        deep(1, 0, 9, 0, 0, rows[k][0], rows[k][1], 0);
        assert(tries == 5 && try_count == 5 && order.unknown_04 == 0x6ff);
        assert(order.flags.word == ((rows[k][2] << 16) | 0xabcd));
        set_answers(4, -1, 0, 0, 0, 0);
        deep(k & 1 ? 0x100 : 0, 0, 9, 0, 0, rows[k][0], rows[k][1], 0);
        assert(tries == 2 && try_count == 2 && order.unknown_04 == 0x6ff);
        assert(order.flags.word == ((rows[k][2] << 16) | 0xabcd));
    }
}

/* 0x159d00: 0x15e030's shape with three tries, 0x24de70 asked again on each,
 * and other's halfword stored after three other answers. */
static void brief(int word, int side, int test)
{
    memset(&order, 0, sizeof(order));
    order.position = POSITION;
    order.unknown_04 = 0x0f11;
    order.flags.word = 0x7777abcd;
    word_calls = side_calls = cell_calls = buf_calls = test_calls = 0;
    callee_calls = tries = last_calls = 0;
    callee_id = try_id = 0;
    first_word = word;
    side_answer = side;
    test_answer = test;
    TaLogic018_CommandBtlUnit(&order);
}

static void check_short(void)
{
    /* A zero cell word: nothing else is asked or stored. */
    set_answers(0, 0, 0, 0, 0, 0);
    brief(0, 0, 1);
    assert(word_calls == 1 && side_calls == 0 && cell_calls == 0 && buf_calls == 0);
    assert(test_calls == 0 && tries == 0);
    assert(order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    /* A positive 0x27b200 answer, side 0 (other is row 1) and non-zero (row 0). */
    brief(9, 0, 1);
    assert(word_calls == 2 && side_calls == 1 && cell_calls == 2 && buf_calls == 1);
    assert(test_calls == 1 && tries == 0);
    assert(test_args[0] == (POSITION >> 8) && test_args[1] == (POSITION & 0xff));
    assert(test_args[2] == (HERE & 0xff) && test_args[3] == (HERE >> 8));
    assert(test_args[4] == 1 && test_args[5] == 0);
    assert(test_args[6] == (THERE & 0xff) && test_args[7] == (THERE >> 8) && test_args[8] == 3);
    assert(order.unknown_04 == 0x2ff && order.flags.word == 0x0100abcd);
    brief(9, 2, 1);
    assert(test_args[4] == 0 && order.unknown_04 == 0x2ff && order.flags.word == 0x0000abcd);
    /* A zero and a negative answer: the first try. Answers 0 and 2 store 0x3ff. */
    brief(9, 0, 0);
    assert(tries == 1 && try_id == 8 && try_order == &order && try_count == 3);
    assert(callee_id == 8 && side_calls == 2);
    assert(order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    brief(9, 3, -1);
    assert(tries == 1 && order.unknown_04 == 0x3ff);
    set_answers(2, 0, 0, 0, 0, 0);
    brief(9, 0, 0);
    assert(tries == 1 && order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    /* Answer 1 stores nothing; answer 3 stores 0x6ff. */
    set_answers(1, 0, 0, 0, 0, 0);
    brief(9, 0, 0);
    assert(tries == 1 && order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    set_answers(3, 0, 0, 0, 0, 0);
    brief(9, 0, 0);
    assert(tries == 1 && order.unknown_04 == 0x6ff && order.flags.word == ((HERE << 16) | 0xabcd));
    /* A listed answer on the third try ends it; three other answers store
     * 0x6ff with other's halfword and make no fourth try. */
    set_answers(4, -1, 1, 0, 0, 0);
    brief(9, 0, 0);
    assert(tries == 3 && side_calls == 4 && order.unknown_04 == 0x0f11);
    set_answers(4, -1, 4, 0, 0, 0);
    brief(9, 0, 0);
    assert(tries == 3 && side_calls == 4);
    assert(order.unknown_04 == 0x6ff && order.flags.word == ((THERE << 16) | 0xabcd));
}

/* 0x12c7f0 with a zero third argument behaves as a two-try copy. */
static void shallow(struct DotrTryOrder *o, int flag)
{
    TaLogic007_CommandBtlUnit(o, flag, 0);
}

static void deep_byte(int flag, int byte, int test)
{
    memset(&order, 0, sizeof(order));
    order.position = POSITION;
    order.unknown_04 = 0x0f11;
    order.flags.word = 0x7777abcd;
    word_calls = side_calls = cell_calls = buf_calls = test_calls = 0;
    callee_calls = tries = last_calls = 0;
    callee_id = try_id = 0;
    first_word = 9;
    side_answer = 0;
    test_answer = test;
    TaLogic007_CommandBtlUnit(&order, flag, (unsigned char)byte);
}

/* A non-zero third argument: four tries passing 5; 0x27b200 is asked with 5
 * only when the byte is exactly 1. */
static void check_deep_byte(void)
{
    set_answers(0, 0, 0, 0, 0, 0);
    deep_byte(1, 1, 1);
    assert(test_calls == 1 && test_args[8] == 5 && tries == 0);
    assert(order.unknown_04 == 0x2ff && order.flags.word == 0x0100abcd);
    deep_byte(1, 2, 1);
    assert(test_calls == 1 && test_args[8] == 2 && tries == 0 && order.unknown_04 == 0x2ff);
    /* Flag clear: not asked. Answers 0 to 3 on the first try. */
    deep_byte(0, 1, 1);
    assert(test_calls == 0 && tries == 1 && try_id == 9 && try_count == 5 && callee_id == 9);
    assert(order.unknown_04 == 0x5ff && order.flags.word == 0x7777abcd);
    set_answers(1, 0, 0, 0, 0, 0);
    deep_byte(1, 2, 0);
    assert(tries == 1 && try_count == 5 && order.unknown_04 == 0x0f11);
    set_answers(2, 0, 0, 0, 0, 0);
    deep_byte(0, 1, 0);
    assert(tries == 1 && order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    set_answers(3, 0, 0, 0, 0, 0);
    deep_byte(0, 1, 0);
    assert(tries == 1 && order.unknown_04 == 0x6ff && order.flags.word == ((HERE << 16) | 0xabcd));
    /* A listed answer on the fourth try ends it; four other answers store
     * 0x5ff and make no fifth try. */
    set_answers(4, -1, 4, 2, 0, 0);
    deep_byte(0, 1, 0);
    assert(tries == 4 && side_calls == 5 && order.unknown_04 == 0x3ff);
    set_answers(4, -1, 4, -1, 3, 3);
    deep_byte(0, 255, 0);
    assert(tries == 4 && side_calls == 5 && try_count == 5);
    assert(order.unknown_04 == 0x5ff && order.flags.word == 0x7777abcd);
}

static int last_one;
static int last_hit;

int TaLogic_GetWarnHideLvl__0010eda0(int side) { return side_callee(20, side); }
int func_0010d820_try(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, void *buf)
{
    return forwarder(20, order, r1, r2, r3, r4, r5, r6, r7, cells, s1, a, b, n, buf);
}
void func_0010db00_order(struct DotrTryOrder *order, int one, int hit)
{
    last_one = one;
    last_hit = hit;
    fallback(20, order);
}

static void byte_tries(int flag, int byte, int test)
{
    memset(&order, 0, sizeof(order));
    order.position = POSITION;
    order.unknown_04 = 0x0f11;
    order.flags.word = 0x7777abcd;
    word_calls = side_calls = cell_calls = buf_calls = test_calls = 0;
    callee_calls = tries = last_calls = 0;
    callee_id = try_id = last_id = 0;
    last_one = last_hit = -1;
    first_word = 9;
    side_answer = 0;
    test_answer = test;
    TaLogic_CommandBtlUnit__0010d490(&order, flag, (unsigned char)byte);
}

/* 0x10d490: 0x27b200 is asked with 2 whatever the byte; a zero byte makes two
 * tries passing 2, any other byte five passing 6; answer 0 and running out of
 * tries go to 0x10db00 with (1, 1) and (1, 0). */
static void check_byte_tries(void)
{
    /* A zero cell word: nothing else is asked or stored. */
    set_answers(0, 0, 0, 0, 0, 0);
    memset(&order, 0, sizeof(order));
    order.position = POSITION;
    order.unknown_04 = 0x0f11;
    word_calls = side_calls = buf_calls = test_calls = tries = last_calls = 0;
    first_word = 0;
    TaLogic_CommandBtlUnit__0010d490(&order, 1, 1);
    assert(word_calls == 1 && side_calls == 0 && buf_calls == 0 && test_calls == 0);
    assert(tries == 0 && last_calls == 0 && order.unknown_04 == 0x0f11);
    /* Flag set and a positive answer: 0x2ff with other's row word. */
    byte_tries(1, 1, 1);
    assert(test_calls == 1 && test_args[8] == 2 && tries == 0 && last_calls == 0);
    assert(order.unknown_04 == 0x2ff && order.flags.word == 0x0100abcd);
    /* Flag set and no positive answer: the tries follow. Answer 0. */
    byte_tries(1, 0, 0);
    assert(test_calls == 1 && test_args[8] == 2 && tries == 1 && try_id == 20 && try_count == 2);
    assert(callee_id == 20 && last_calls == 1 && last_id == 20 && last_order == &order);
    assert(last_one == 1 && last_hit == 1);
    assert(order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    /* Flag clear: not asked. Answers 1 to 3 on the first try. */
    set_answers(1, 0, 0, 0, 0, 0);
    byte_tries(0, 0, 1);
    assert(test_calls == 0 && tries == 1 && last_calls == 0 && order.unknown_04 == 0x0f11);
    set_answers(2, 0, 0, 0, 0, 0);
    byte_tries(0, 0, 1);
    assert(tries == 1 && last_calls == 0);
    assert(order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    set_answers(3, 0, 0, 0, 0, 0);
    byte_tries(0, 0, 1);
    assert(tries == 1 && last_calls == 0);
    assert(order.unknown_04 == 0x6ff && order.flags.word == ((HERE << 16) | 0xabcd));
    /* A listed answer on the second try ends it; two other answers go to
     * 0x10db00 with (1, 0) and make no third try. */
    set_answers(4, 2, 0, 0, 0, 0);
    byte_tries(0, 0, 0);
    assert(tries == 2 && side_calls == 3 && last_calls == 0 && order.unknown_04 == 0x3ff);
    set_answers(4, -1, 3, 3, 3, 3);
    byte_tries(0, 0, 0);
    assert(tries == 2 && side_calls == 3 && try_count == 2);
    assert(last_calls == 1 && last_one == 1 && last_hit == 0 && order.unknown_04 == 0x0f11);
    /* A non-zero byte: the same answers on five tries passing 6. */
    set_answers(0, 0, 0, 0, 0, 0);
    byte_tries(0, 1, 0);
    assert(tries == 1 && try_count == 6 && last_calls == 1 && last_one == 1 && last_hit == 1);
    set_answers(1, 0, 0, 0, 0, 0);
    byte_tries(0, 1, 0);
    assert(tries == 1 && last_calls == 0 && order.unknown_04 == 0x0f11);
    set_answers(2, 0, 0, 0, 0, 0);
    byte_tries(0, 255, 0);
    assert(tries == 1 && order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    set_answers(4, -1, 4, -1, 3, 0);
    byte_tries(0, 1, 0);
    assert(tries == 5 && side_calls == 6 && last_calls == 0);
    assert(order.unknown_04 == 0x6ff && order.flags.word == ((HERE << 16) | 0xabcd));
    set_answers(4, -1, 4, -1, 5, 3);
    byte_tries(0, 2, 0);
    assert(tries == 5 && side_calls == 6 && try_count == 6);
    assert(last_calls == 1 && last_one == 1 && last_hit == 0);
    assert(order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
}

static int random_answer;
static int random_calls;

int func_0019a5c8(void)
{
    random_calls++;
    return random_answer;
}

typedef void (*RowTries)(struct DotrTryOrder *order, unsigned char deep);

static void row_run(RowTries f, int byte, int word, int test, int here, int high)
{
    memset(&order, 0, sizeof(order));
    order.position = POSITION;
    order.unknown_04 = (high << 8) | 0x11;
    order.flags.word = 0x7777abcd;
    word_calls = side_calls = cell_calls = buf_calls = test_calls = 0;
    callee_calls = tries = last_calls = random_calls = 0;
    callee_id = try_id = 0;
    first_word = word;
    side_answer = 0;
    test_answer = test;
    here_answer = here;
    f(&order, (unsigned char)byte);
    here_answer = HERE;
}

/* 0x16e0e0 and 0x15e520: the byte picks row 3 (byte 1) or 4; the closing
 * store depends on the position's row and column and the high byte at +4. */
static void check_row_tries(RowTries f, int id)
{
    /* A zero cell word: only the first 0x1e1090 question is made. */
    set_answers(0, 0, 0, 0, 0, 0);
    row_run(f, 1, 0, 1, HERE, 0x0f);
    assert(word_calls == 1 && cell_calls == 1 && side_calls == 0 && buf_calls == 0);
    assert(test_calls == 0 && tries == 0 && order.unknown_04 == 0x0f11);
    /* A positive 0x27b200 answer (asked with 2): 0x2ff with other's row word. */
    row_run(f, 1, 9, 1, HERE, 0x0f);
    assert(word_calls == 2 && cell_calls == 3 && side_calls == 1 && buf_calls == 1);
    assert(test_calls == 1 && test_args[8] == 2 && tries == 0);
    assert(test_args[0] == (POSITION >> 8) && test_args[1] == (POSITION & 0xff));
    assert(test_args[2] == (HERE & 0xff) && test_args[3] == (HERE >> 8));
    assert(test_args[4] == 1 && test_args[5] == 0);
    assert(test_args[6] == (THERE & 0xff) && test_args[7] == (THERE >> 8));
    assert(order.unknown_04 == 0x2ff && order.flags.word == 0x0100abcd);
    /* Answers 0 to 3 on the first try. */
    row_run(f, 1, 9, 0, HERE, 0x0f);
    assert(tries == 1 && try_id == id && try_count == 2 && callee_id == id);
    assert(order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    set_answers(1, 0, 0, 0, 0, 0);
    row_run(f, 1, 9, -1, HERE, 0x0f);
    assert(tries == 1 && order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    set_answers(2, 0, 0, 0, 0, 0);
    row_run(f, 1, 9, 0, HERE, 0x0f);
    assert(tries == 1 && order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    set_answers(3, 0, 0, 0, 0, 0);
    row_run(f, 1, 9, 0, HERE, 0x0f);
    assert(tries == 1 && order.unknown_04 == 0x6ff && order.flags.word == ((HERE << 16) | 0xabcd));
    /* A listed answer on the second try ends it. */
    set_answers(4, 3, 0, 0, 0, 0);
    row_run(f, 1, 9, 0, HERE, 0x0f);
    assert(tries == 2 && side_calls == 3 && order.unknown_04 == 0x6ff);
    /* Two other answers, row not the picked one: 0x6ff with the picked row
     * and the position's column. Byte 1 picks 3, any other byte 4. */
    set_answers(4, -1, 3, 3, 3, 3);
    row_run(f, 1, 9, 0, 0x0407, 0x0f);
    assert(tries == 2 && random_calls == 0);
    assert(order.unknown_04 == 0x6ff && order.flags.word == 0x0307abcd);
    row_run(f, 0, 9, 0, 0x0307, 0x0f);
    assert(tries == 2 && order.unknown_04 == 0x6ff && order.flags.word == 0x0407abcd);
    row_run(f, 2, 9, 0, 0x0307, 0x10);
    assert(order.unknown_04 == 0x6ff && order.flags.word == 0x0407abcd);
    /* The picked row, high byte 16: column 2 and 1. */
    row_run(f, 1, 9, 0, 0x0302, 0x10);
    assert(random_calls == 0 && order.unknown_04 == 0x1011 && order.flags.word == 0x0300abcd);
    row_run(f, 1, 9, 0, 0x0301, 0x10);
    assert(random_calls == 0 && order.unknown_04 == 0xfff && order.flags.word == 0x0305abcd);
    /* High byte 15: column 4 and 5. */
    row_run(f, 0, 9, 0, 0x0404, 0x0f);
    assert(random_calls == 0 && order.unknown_04 == 0x0f11 && order.flags.word == 0x0405abcd);
    row_run(f, 0, 9, 0, 0x0405, 0x0f);
    assert(random_calls == 0 && order.unknown_04 == 0x10ff && order.flags.word == 0x0401abcd);
    /* Any other high byte: 0x19a5c8's parity (even, odd, negative odd). */
    random_answer = 6;
    row_run(f, 1, 9, 0, 0x0302, 0x0e);
    assert(random_calls == 1 && order.unknown_04 == 0x10ff && order.flags.word == 0x0301abcd);
    random_answer = 7;
    row_run(f, 1, 9, 0, 0x0302, 0x11);
    assert(random_calls == 1 && order.unknown_04 == 0xfff && order.flags.word == 0x0305abcd);
    random_answer = -7;
    row_run(f, 0, 9, 0, 0x0409, 0x00);
    assert(random_calls == 1 && order.unknown_04 == 0xfff && order.flags.word == 0x0405abcd);
}

static int third_answer;
static int third_calls;
static int spot_calls;

int CheckLandAgreement2(int a, int b, int c)
{
    assert(a == 3 && b == (POSITION >> 8) && c == (POSITION & 0xff));
    third_calls++;
    return third_answer;
}

int TaLogic001_GetNextGrassPoint(int a, int b)
{
    assert(a == (here_answer & 0xff) && b == (here_answer >> 8));
    spot_calls++;
    return 0x0607;
}

static void spot(int third, int here)
{
    memset(&order, 0, sizeof(order));
    order.position = POSITION;
    order.unknown_04 = 0x0f11;
    order.flags.word = 0x7777abcd;
    pair_calls = cell_calls = third_calls = spot_calls = 0;
    third_answer = third;
    here_answer = here;
    TaLogic001_CommandInvadeUnit(&order);
    here_answer = HERE;
    assert(pair_calls == 1 && cell_calls == 1 && third_calls == 1);
    assert(order.unknown_04 == 0x6ff);
}

/* 0x113a60: 0x24df00's answer 1 takes 0x113b60's result; otherwise the
 * column and row bounds at 3 and their order pick one of three constants. */
static void check_spot_fallback(void)
{
    spot(1, 0x0102);
    assert(spot_calls == 1 && order.flags.word == 0x0607abcd);
    spot(0, 0x0202);
    assert(spot_calls == 0 && order.flags.word == 0x0303abcd);
    spot(2, 0x0203);
    assert(spot_calls == 0 && order.flags.word == 0x0305abcd);
    spot(0, 0x0302);
    assert(order.flags.word == 0x0503abcd);
    spot(0, 0x0304);
    assert(order.flags.word == 0x0305abcd);
    spot(0, 0x0404);
    assert(order.flags.word == 0x0305abcd);
    spot(0, 0x0504);
    assert(order.flags.word == 0x0503abcd);
}

static int level_answer;
static int near_answer;
static int near_calls;

int func_0013a9a0_side(int side)
{
    assert(side == 0x55);
    return level_answer;
}

int func_0013a870_side(int side)
{
    assert(side == 0x55);
    near_calls++;
    return near_answer;
}

static void below(int deep, int here)
{
    memset(&order, 0, sizeof(order));
    order.position = POSITION;
    order.unknown_04 = 0x0f11;
    order.flags.word = 0x7777abcd;
    pair_calls = cell_calls = below_calls = near_calls = 0;
    here_answer = here;
    below_mode = 1;
    TaLogic010_CommandInvadeUnit(&order, deep);
    below_mode = 0;
    here_answer = HERE;
    assert(pair_calls == 1 && cell_calls == 1);
}

/* 0x13a710: a low byte of 1 probes the cell below (non-negative, zero and
 * negative answers); otherwise the row compare, then 0x13a870's sign. */
static void check_below_fallback(void)
{
    below_answer = 0x0405;
    below(0x101, 0x0203);
    assert(below_calls == 1 && near_calls == 0);
    assert(order.unknown_04 == 0x2ff && order.flags.word == 0x0405abcd);
    below_answer = 0;
    below(1, 0x0203);
    assert(below_calls == 1 && order.unknown_04 == 0x2ff && order.flags.word == 0x0000abcd);
    below_answer = -1;
    below(1, 0x0203);
    assert(below_calls == 1 && order.unknown_04 == 0x6ff && order.flags.word == 0x0303abcd);
    /* Not 1: the halfword's row equals 0x13a9a0's answer. */
    level_answer = 2;
    below(0, 0x0203);
    assert(below_calls == 0 && near_calls == 0);
    assert(order.unknown_04 == 0x6ff && order.flags.word == 0x0203abcd);
    /* Another row: 0x13a870 non-negative, zero and negative. */
    level_answer = 3;
    near_answer = 0x0506;
    below(2, 0x0203);
    assert(below_calls == 0 && near_calls == 1);
    assert(order.unknown_04 == 0x6ff && order.flags.word == 0x0506abcd);
    near_answer = 0;
    below(0, 0x0203);
    assert(near_calls == 1 && order.unknown_04 == 0x6ff && order.flags.word == 0x0000abcd);
    near_answer = -1;
    below(0, 0x0203);
    assert(near_calls == 1 && order.unknown_04 == 0x6ff && order.flags.word == 0x0303abcd);
}

static char column_full[8];
static int column_calls;

void *func_001e36e0_try(int x, int y)
{
    assert(x >= 1 && x <= 5 && y == (level_answer & 0xff));
    column_calls++;
    return column_full[x] ? (void *)column_full : 0;
}

/* 0x13a870: all five columns full, then each empty in the order 3, 4, 2, 5,
 * 1; only the low byte of 0x13a9a0's answer is the row. */
static void check_row_columns(void)
{
    static const int columns[5] = {3, 4, 2, 5, 1};
    int i;

    level_answer = 0x706;
    memset(column_full, 1, sizeof(column_full));
    column_calls = 0;
    assert(TaLogic010_GetEmptyLine(0x55) == -1 && column_calls == 5);
    for (i = 4; i >= 0; i--) {
        column_full[columns[i]] = 0;
        column_calls = 0;
        assert(TaLogic010_GetEmptyLine(0x55) == (0x600 | columns[i]) && column_calls == i + 1);
    }
}

static int weight_answer;
static int ring_answer;
static int ring_arg;
static int out_answer;
static int weight_calls;
static int ring_calls;

int func_001e0290_try(int a, int b, int flag)
{
    assert(a == (POSITION >> 8) && b == (POSITION & 0xff) && flag == 0);
    weight_calls++;
    return weight_answer;
}

int func_00122680_try(int a, int b)
{
    assert(a == (POSITION >> 8) && b == (POSITION & 0xff));
    ring_calls++;
    return ring_answer;
}

int func_00122970_try(int n)
{
    ring_arg = n;
    ring_calls += 0x10;
    return 0x0102;
}

int func_00122810_try(int column, int row)
{
    assert(column == (here_answer & 0xff) && row == (here_answer >> 8));
    ring_calls += 0x100;
    return 0x0304;
}

void TaLogic005_GetNearDefensePoint(int word, int column, int row, int *out)
{
    assert(word == 0x1234);
    assert(column == (here_answer & 0xff) && row == (here_answer >> 8));
    ring_calls += 0x1000;
    out[0] = out_answer;
}

static void weigh(int word, int weight, int ring, int out)
{
    memset(&order, 0, sizeof(order));
    order.position = POSITION;
    order.unknown_04 = 0x0f11;
    order.flags.word = 0x7777abcd;
    word_calls = side_calls = cell_calls = weight_calls = ring_calls = 0;
    first_word = word;
    weight_answer = weight;
    ring_answer = ring;
    out_answer = out;
    ring_arg = -1;
    TaLogic005_CommandRoutineUnit(&order);
}

/* 0x122530: the zero cell word, the bound at 1500 on both sides, the sign of
 * 0x122680's answer with the wrap of n + 1 at 8, and the sign of 0x123500's
 * output word. */
static void check_weight_fallback(void)
{
    weigh(0, 1500, 0, 0);
    assert(word_calls == 1 && side_calls == 0 && cell_calls == 0 && weight_calls == 0);
    assert(order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    weigh(0x4321, 1500, 0, 0);
    assert(word_calls == 1 && side_calls == 1 && cell_calls == 1 && weight_calls == 1);
    assert(ring_calls == 0x11 && ring_arg == 1);
    assert(order.unknown_04 == 0x6ff && order.flags.word == 0x0102abcd);
    weigh(0x4321, 2000, 7, 0);
    assert(ring_calls == 0x11 && ring_arg == 0 && order.flags.word == 0x0102abcd);
    weigh(0x4321, 2000, 6, 0);
    assert(ring_calls == 0x11 && ring_arg == 7);
    weigh(0x4321, 1500, -1, 0);
    assert(ring_calls == 0x101);
    assert(order.unknown_04 == 0x6ff && order.flags.word == 0x0304abcd);
    /* Below 1500: 0x123500's output word. */
    weigh(0x4321, 1499, 0, 0x0506);
    assert(word_calls == 2 && ring_calls == 0x1000);
    assert(order.unknown_04 == 0x6ff && order.flags.word == 0x0506abcd);
    weigh(0x4321, 1499, 0, 0);
    assert(ring_calls == 0x1000 && order.unknown_04 == 0x6ff && order.flags.word == 0x0000abcd);
    weigh(0x4321, 1499, 0, -1);
    assert(ring_calls == 0x1000 && order.unknown_04 == 0x5ff && order.flags.word == 0x7777abcd);
}

static int edge_calls;

int TaLogic003_GetEmptyAtkLineStart(int a)
{
    assert(a == 0x21);
    edge_calls++;
    return 0x0003;
}

int TaLogic003_GetEmptyAtkLineEnd(int a)
{
    assert(a == 0x21);
    edge_calls += 0x10;
    return 0x0603;
}

static void edge(int here, int high)
{
    memset(&order, 0, sizeof(order));
    order.position = POSITION;
    order.unknown_04 = (high << 8) | 0x11;
    order.flags.word = 0x7777abcd;
    record_calls = cell_calls = edge_calls = 0;
    record_answer = 0x21;
    here_answer = here;
    TaLogic003_CommandAtkLineUnit(&order);
    here_answer = HERE;
    record_answer = 0;
    assert(record_calls == 1 && cell_calls == 1 && edge_calls == 0x11);
}

/* 0x118bc0: rows 0 and 6 compare the halfword with one callee's answer;
 * other rows depend on the high byte at +4 and on row < 4. */
static void check_edge_fallback(void)
{
    edge(0x0003, 0x02);
    assert(order.unknown_04 == 0xfff && order.flags.word == 0x7777abcd);
    edge(0x0004, 0x02);
    assert(order.unknown_04 == 0x6ff && order.flags.word == 0x0003abcd);
    edge(0x0603, 0x02);
    assert(order.unknown_04 == 0x10ff && order.flags.word == 0x7777abcd);
    edge(0x0602, 0x02);
    assert(order.unknown_04 == 0x6ff && order.flags.word == 0x0603abcd);
    /* Other rows: high byte 15 or 16 leaves everything. */
    edge(0x0303, 0x0f);
    assert(order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    edge(0x0303, 0x10);
    assert(order.unknown_04 == 0x1011 && order.flags.word == 0x7777abcd);
    edge(0x0303, 0x0e);
    assert(order.unknown_04 == 0xfff && order.flags.word == (int)0xffffabcd);
    edge(0x0403, 0x11);
    assert(order.unknown_04 == 0x10ff && order.flags.word == (int)0xffffabcd);
    edge(0x0103, 0x00);
    assert(order.unknown_04 == 0xfff && order.flags.word == (int)0xffffabcd);
    edge(0x0503, 0x00);
    assert(order.unknown_04 == 0x10ff && order.flags.word == (int)0xffffabcd);
}

/* The two copies without a flag argument. */
static void flagless_two(struct DotrTryOrder *o, int flag)
{
    (void)flag;
    TaLogic012_CommandBtlUnit(o);
}

static void flagless_five(struct DotrTryOrder *o, int flag)
{
    (void)flag;
    TaLogic022_CommandAtkUnit(o);
}

/* `five` selects 0x16dda0: 0x27b200 asked with 5 (else 2), 0x24de70 asked
 * once (else again on every try), answer 0 stores 0x3ff with the halfword
 * (else a bare 0x5ff), answer 3 ends (else goes on) and five other answers
 * store 0x6ff with other's halfword (else a bare 0x5ff). */
static void check_flagless(Tries f, int id, int five)
{
    /* A zero cell word: nothing else is asked or stored. */
    set_answers(0, 0, 0, 0, 0, 0);
    run(f, 0, 0, 0, 1);
    assert(word_calls == 1 && side_calls == 0 && cell_calls == 0 && buf_calls == 0);
    assert(test_calls == 0 && tries == 0);
    assert(order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    /* A positive 0x27b200 answer, side 0 (other is row 1) and non-zero (row 0). */
    run(f, 0, 9, 0, 1);
    assert(word_calls == 2 && side_calls == 1 && cell_calls == 2 && buf_calls == 1);
    assert(test_calls == 1 && tries == 0);
    assert(test_args[0] == (POSITION >> 8) && test_args[1] == (POSITION & 0xff));
    assert(test_args[2] == (HERE & 0xff) && test_args[3] == (HERE >> 8));
    assert(test_args[4] == 1 && test_args[5] == 0);
    assert(test_args[6] == (THERE & 0xff) && test_args[7] == (THERE >> 8));
    assert(test_args[8] == (five ? 5 : 2));
    assert(order.unknown_04 == 0x2ff && order.flags.word == 0x0100abcd);
    run(f, 0, 9, 2, 1);
    assert(test_args[4] == 0 && order.unknown_04 == 0x2ff && order.flags.word == 0x0000abcd);
    /* A zero and a negative answer: the first try (answer 0). */
    run(f, 0, 9, 0, 0);
    assert(test_calls == 1 && tries == 1 && try_id == id && try_order == &order && try_count == 5);
    assert(callee_id == id && side_calls == (five ? 1 : 2));
    if (five) {
        assert(order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    } else {
        assert(order.unknown_04 == 0x5ff && order.flags.word == 0x7777abcd);
    }
    run(f, 0, 9, 3, -1);
    assert(tries == 1 && order.unknown_04 == (five ? 0x3ff : 0x5ff));
    /* Answer 1 stores nothing; answer 2 stores 0x3ff. */
    set_answers(1, 0, 0, 0, 0, 0);
    run(f, 0, 9, 0, 0);
    assert(tries == 1 && order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    set_answers(2, 0, 0, 0, 0, 0);
    run(f, 0, 9, 0, 0);
    assert(tries == 1 && order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    /* Answer 3 stores 0x6ff; 0x143540 then makes the next try (answer 1 here). */
    set_answers(3, 1, 0, 0, 0, 0);
    run(f, 0, 9, 0, 0);
    assert(tries == (five ? 1 : 2));
    assert(order.unknown_04 == 0x6ff && order.flags.word == ((HERE << 16) | 0xabcd));
    /* A listed answer on the fifth try ends it. */
    set_answers(4, -1, 4, -1, 1, 0);
    run(f, 0, 9, 0, 0);
    assert(tries == 5 && side_calls == (five ? 1 : 6));
    assert(order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    /* Five other answers: the closing store and no sixth try. */
    set_answers(4, -1, 4, -1, 4, 0);
    run(f, 0, 9, 0, 0);
    assert(tries == 5 && last_calls == 0);
    if (five) {
        assert(order.unknown_04 == 0x6ff && order.flags.word == ((THERE << 16) | 0xabcd));
    } else {
        assert(order.unknown_04 == 0x5ff && order.flags.word == 0x7777abcd);
        /* Answer 3 on every try: 0x5ff replaces 0x6ff, the +8 word stays. */
        set_answers(3, 3, 3, 3, 3, 0);
        run(f, 0, 9, 0, 0);
        assert(tries == 5);
        assert(order.unknown_04 == 0x5ff && order.flags.word == ((HERE << 16) | 0xabcd));
    }
}

/* 0x131b30's early store: kind 408 with an amount of at most 1000 stores 0x3ff
 * and the position's halfword, and the function goes on (answer 1 keeps it,
 * a positive 0x27b200 answer overwrites it). */
static void check_early_store(void)
{
    set_answers(1, 0, 0, 0, 0, 0);
    kind_answer = 408, amount_answer = 1000, amount_calls = 0;
    run(TaLogic008_CommandBtlUnit, 0, 9, 2, 0);
    assert(amount_calls == 1 && tries == 1 && test_calls == 0);
    assert(order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    run(TaLogic008_CommandBtlUnit, 1, 9, 2, 1);
    assert(amount_calls == 2 && tries == 0 && test_calls == 1);
    assert(order.unknown_04 == 0x2ff && order.flags.word == 0x0000abcd);
    /* One more than 1000: nothing stored. */
    amount_answer = 1001;
    run(TaLogic008_CommandBtlUnit, 0, 9, 2, 0);
    assert(amount_calls == 3 && tries == 1);
    assert(order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    /* Another kind: the amount is not asked. */
    kind_answer = 407, amount_answer = 0;
    run(TaLogic008_CommandBtlUnit, 0, 9, 2, 0);
    assert(amount_calls == 3 && tries == 1);
    assert(order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    kind_answer = 0;
}

static void third_clear(struct DotrTryOrder *o, int flag)
{
    TaLogic001_CommandBtlUnit(o, flag, 0);
}

static void third_set(struct DotrTryOrder *o, int flag)
{
    TaLogic001_CommandBtlUnit(o, flag, 0x100);
}

static void one_or_three_clear(struct DotrTryOrder *o, int flag)
{
    TaLogic010_CommandBtlUnit(o, flag, 0);
}

static void one_or_three_set(struct DotrTryOrder *o, int flag)
{
    TaLogic010_CommandBtlUnit(o, flag, -1);
}

int TaLogic_GetNearUnit(struct DotrTryOrder *order, int r1, int r2, int r3, int r4, int r5, int r6, int r7, struct DotrTryCell *cells, int s1, unsigned short *a, unsigned short *b, int n, int s5, int s6, void *buf)
{
    assert(s5 == 0 && s6 == 2400);
    return forwarder(19, order, r1, r2, r3, r4, r5, r6, r7, cells, s1, a, b, n, buf);
}

static void invade_call(struct DotrTryOrder *o, int flag)
{
    (void)flag;
    TaLogic013_CommandInvadeUnit(o);
}

static void invade(int word, int side, int test)
{
    near_calls = 0;
    run(invade_call, 0, word, side, test);
}

/* TaLogic013_CommandInvadeUnit (branch and loop coverage): zero cell word, positive 0x27b200
 * answer for both sides, each listed answer on the first try, a listed answer on a later
 * try, and five unlisted answers (the closing store from 0x147e00). */
static void check_invade(void)
{
    set_answers(0, 0, 0, 0, 0, 0);
    invade(0, 0, 1);
    assert(word_calls == 1 && side_calls == 0 && cell_calls == 0 && buf_calls == 0 && test_calls == 0 && tries == 0);
    assert(order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    invade(9, 0, 1);
    assert(word_calls == 2 && side_calls == 1 && cell_calls == 2 && buf_calls == 1 && test_calls == 1 && tries == 0);
    assert(test_args[4] == 1 && test_args[5] == 0 && test_args[8] == 2);
    assert(order.unknown_04 == 0x2ff && order.flags.word == 0x0100abcd);
    invade(9, 2, 1);
    assert(test_args[4] == 0 && order.unknown_04 == 0x2ff && order.flags.word == 0x0000abcd);
    invade(9, 0, 0);
    assert(tries == 1 && try_id == 19 && try_order == &order && try_count == 5 && callee_id == 18 && side_calls == 2);
    assert(order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    set_answers(1, 0, 0, 0, 0, 0);
    invade(9, 0, -1);
    assert(tries == 1 && order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    set_answers(2, 0, 0, 0, 0, 0);
    invade(9, 0, 0);
    assert(tries == 1 && order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    set_answers(3, 0, 0, 0, 0, 0);
    invade(9, 0, 0);
    assert(tries == 1 && order.unknown_04 == 0x6ff && order.flags.word == ((HERE << 16) | 0xabcd));
    set_answers(4, -1, 5, 2, 0, 0);
    invade(9, 0, 0);
    assert(tries == 4 && side_calls == 5 && near_calls == 0);
    assert(order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    set_answers(4, -1, 5, 6, 7, 0);
    invade(9, 2, 0);
    assert(tries == 5 && side_calls == 6 && near_calls == 1);
    assert(order.unknown_04 == 0x6ff && order.flags.word == ((0x0607 << 16) | 0xabcd));
    set_answers(0, 0, 0, 0, 0, 0);
}

static void two_tries_call(struct DotrTryOrder *o, int flag)
{
    (void)flag;
    TaLogic013_CommandBtlUnit(o);
}

static void two_tries(int word, int side, int test)
{
    column_calls = free_calls = near_calls = 0;
    run(two_tries_call, 0, word, side, test);
}

/* 0x1474f0: the flagless shape with two tries and a three-way closing store. */
static void check_two_tries(void)
{
    /* A zero cell word. */
    set_answers(0, 0, 0, 0, 0, 0);
    two_tries(0, 0, 1);
    assert(word_calls == 1 && side_calls == 0 && cell_calls == 0 && buf_calls == 0);
    assert(test_calls == 0 && tries == 0 && column_calls == 0);
    assert(order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    /* A positive 0x27b200 answer, for both sides. */
    two_tries(9, 0, 1);
    assert(word_calls == 2 && side_calls == 1 && cell_calls == 2 && buf_calls == 1);
    assert(test_calls == 1 && tries == 0 && column_calls == 0);
    assert(test_args[0] == (POSITION >> 8) && test_args[1] == (POSITION & 0xff));
    assert(test_args[2] == (HERE & 0xff) && test_args[3] == (HERE >> 8));
    assert(test_args[4] == 1 && test_args[5] == 0);
    assert(test_args[6] == (THERE & 0xff) && test_args[7] == (THERE >> 8) && test_args[8] == 2);
    assert(order.unknown_04 == 0x2ff && order.flags.word == 0x0100abcd);
    two_tries(9, 2, 1);
    assert(test_args[4] == 0 && order.unknown_04 == 0x2ff && order.flags.word == 0x0000abcd);
    /* Zero and negative 0x27b200 answers: the first try; answer 0. */
    two_tries(9, 0, 0);
    assert(test_calls == 1 && tries == 1 && try_id == 18 && try_order == &order && try_count == 2);
    assert(callee_id == 18 && side_calls == 2 && column_calls == 0);
    assert(order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    two_tries(9, 3, -1);
    assert(tries == 1 && order.unknown_04 == 0x3ff);
    /* Answers 1, 2 and 3. */
    set_answers(1, 0, 0, 0, 0, 0);
    two_tries(9, 0, 0);
    assert(tries == 1 && order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    set_answers(2, 0, 0, 0, 0, 0);
    two_tries(9, 0, 0);
    assert(tries == 1 && order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    set_answers(3, 0, 0, 0, 0, 0);
    two_tries(9, 0, 0);
    assert(tries == 1 && order.unknown_04 == 0x6ff && order.flags.word == ((HERE << 16) | 0xabcd));
    /* A listed answer on the second try ends it. */
    set_answers(4, 1, 0, 0, 0, 0);
    two_tries(9, 0, 0);
    assert(tries == 2 && side_calls == 3 && column_calls == 0);
    assert(order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    /* Two other answers, 0x147e50 answering 1: the position's halfword. */
    set_answers(4, -1, 0, 0, 0, 0);
    column_answer = 1, free_answer = 0x0102;
    two_tries(9, 0, 0);
    assert(tries == 2 && column_calls == 1 && free_calls == 0 && near_calls == 0);
    assert(order.unknown_04 == 0x6ff && order.flags.word == ((HERE << 16) | 0xabcd));
    /* 0x147e50 answering otherwise: 0x147f40's answer when zero or more. */
    column_answer = 2;
    two_tries(9, 0, 0);
    assert(tries == 2 && column_calls == 1 && free_calls == 1 && near_calls == 0);
    assert(order.unknown_04 == 0x6ff && order.flags.word == 0x0102abcd);
    free_answer = 0;
    two_tries(9, 0, 0);
    assert(free_calls == 1 && near_calls == 0);
    assert(order.unknown_04 == 0x6ff && order.flags.word == 0x0000abcd);
    /* A negative 0x147f40 answer: 0x147e00's, for both sides. */
    free_answer = -1;
    two_tries(9, 0, 0);
    assert(free_calls == 1 && near_calls == 1);
    assert(order.unknown_04 == 0x6ff && order.flags.word == 0x0607abcd);
    two_tries(9, 2, 0);
    assert(near_calls == 1 && order.flags.word == 0x0607abcd);
}

static void three_tries_call(struct DotrTryOrder *o, int flag)
{
    (void)flag;
    TaLogic014_CommandBtlUnit(o);
}

static void three_tries(int word, int side, int test)
{
    column_calls = free_calls = alone_calls = 0;
    run(three_tries_call, 0, word, side, test);
}

/* 0x14c3b0: the flagless shape with three tries and a four-way closing store. */
static void check_three_tries(void)
{
    /* A zero cell word. */
    set_answers(0, 0, 0, 0, 0, 0);
    three_tries(0, 0, 1);
    assert(word_calls == 1 && side_calls == 0 && cell_calls == 0 && buf_calls == 0);
    assert(test_calls == 0 && tries == 0 && column_calls == 0);
    assert(order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    /* A positive 0x27b200 answer, for both sides. */
    three_tries(9, 0, 1);
    assert(word_calls == 2 && side_calls == 1 && cell_calls == 2 && buf_calls == 1);
    assert(test_calls == 1 && tries == 0 && column_calls == 0);
    assert(test_args[0] == (POSITION >> 8) && test_args[1] == (POSITION & 0xff));
    assert(test_args[2] == (HERE & 0xff) && test_args[3] == (HERE >> 8));
    assert(test_args[4] == 1 && test_args[5] == 0);
    assert(test_args[6] == (THERE & 0xff) && test_args[7] == (THERE >> 8) && test_args[8] == 3);
    assert(order.unknown_04 == 0x2ff && order.flags.word == 0x0100abcd);
    three_tries(9, 2, 1);
    assert(test_args[4] == 0 && order.unknown_04 == 0x2ff && order.flags.word == 0x0000abcd);
    /* Zero and negative 0x27b200 answers: the first try; answer 0. */
    three_tries(9, 0, 0);
    assert(test_calls == 1 && tries == 1 && try_id == 19 && try_order == &order && try_count == 3);
    assert(callee_id == 19 && side_calls == 2 && column_calls == 0);
    assert(order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    three_tries(9, 3, -1);
    assert(tries == 1 && order.unknown_04 == 0x3ff);
    /* Answers 1, 2 and 3. */
    set_answers(1, 0, 0, 0, 0, 0);
    three_tries(9, 0, 0);
    assert(tries == 1 && order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    set_answers(2, 0, 0, 0, 0, 0);
    three_tries(9, 0, 0);
    assert(tries == 1 && order.unknown_04 == 0x3ff && order.flags.word == ((HERE << 16) | 0xabcd));
    set_answers(3, 0, 0, 0, 0, 0);
    three_tries(9, 0, 0);
    assert(tries == 1 && order.unknown_04 == 0x6ff && order.flags.word == ((HERE << 16) | 0xabcd));
    /* A listed answer on the third try ends it. */
    set_answers(4, -1, 1, 0, 0, 0);
    three_tries(9, 0, 0);
    assert(tries == 3 && side_calls == 4 && column_calls == 0);
    assert(order.unknown_04 == 0x0f11 && order.flags.word == 0x7777abcd);
    /* Three other answers, 0x14c970 answering 1: 0x14f3a0 chooses. */
    set_answers(4, -1, 4, 0, 0, 0);
    column_answer = 1, alone_answer = 1, free_answer = 0x0102;
    three_tries(9, 0, 0);
    assert(tries == 3 && column_calls == 1 && alone_calls == 1 && free_calls == 0);
    assert(order.unknown_04 == 0x2ff && order.flags.word == 0x0100abcd);
    alone_answer = 2;
    three_tries(9, 0, 0);
    assert(column_calls == 1 && alone_calls == 1 && free_calls == 0);
    assert(order.unknown_04 == 0x6ff && order.flags.word == ((HERE << 16) | 0xabcd));
    /* 0x14c970 answering otherwise: 0x14ca60's answer when zero or more. */
    column_answer = 0, alone_answer = 1;
    three_tries(9, 0, 0);
    assert(tries == 3 && column_calls == 1 && alone_calls == 0 && free_calls == 1);
    assert(order.unknown_04 == 0x6ff && order.flags.word == 0x0102abcd);
    free_answer = 0;
    three_tries(9, 0, 0);
    assert(order.unknown_04 == 0x6ff && order.flags.word == 0x0000abcd);
    /* A negative 0x14ca60 answer: 0x2ff and other's row, for both sides. */
    free_answer = -1;
    three_tries(9, 0, 0);
    assert(alone_calls == 0 && free_calls == 1);
    assert(order.unknown_04 == 0x2ff && order.flags.word == 0x0100abcd);
    three_tries(9, 2, 0);
    assert(order.unknown_04 == 0x2ff && order.flags.word == 0x0000abcd);
}

int main(void)
{
    check_deep();
    check_plain();
    check_fallback();
    check_column_rows();
    check_lines();
    check_scan_fallback();
    check_tries(TaLogic_CommandBtlUnit__0011d110, 1, 4);
    check_tries(TaLogic006_CommandBtlUnit, 2, 5);
    check_tries(TaLogic011_CommandBtlUnit, 3, 2);
    check_tries(TaLogic021_CommandBtlUnit, 4, 4);
    callee_free = 1;
    check_tries(TaLogic009_CommandBtlUnit, 7, 2);
    callee_free = 0;
    check_short();
    check_tries(shallow, 9, 2);
    check_deep_byte();
    check_byte_tries();
    check_edge_fallback();
    check_spot_fallback();
    check_below_fallback();
    check_row_columns();
    check_weight_fallback();
    check_row_tries(TaLogic022_CommandLineUnit, 14);
    check_row_tries(TaLogic019_CommandLineUnit, 5);
    check_tries(TaLogic020_CommandBtlUnit, 10, 5);
    ask_count = 2, three_plain = 1;
    check_tries(TaLogic_CommandBtlUnit__00122270, 11, 3);
    ask_count = 0, three_plain = 0;
    buf_late = 1;
    check_tries(TaLogic_CommandBtlUnit__001188f0, 12, 3);
    buf_late = 0;
    kind_answer = 0;
    check_tries(TaLogic008_CommandBtlUnit, 15, 4);
    check_early_store();
    ask_count = 2;
    check_tries(third_clear, 16, 2);
    assert(last_deep == 0);
    check_tries(third_set, 16, 5);
    assert(last_deep == 0x100);
    zero_here = 1;
    check_tries(one_or_three_clear, 17, 3);
    assert(last_deep == 0);
    check_tries(one_or_three_set, 17, 1);
    assert(last_deep == -1);
    ask_count = 0, zero_here = 0;
    check_two_tries();
    check_invade();
    check_three_tries();
    check_flagless(flagless_two, 13, 0);
    check_flagless(flagless_five, 14, 1);
    return 0;
}
