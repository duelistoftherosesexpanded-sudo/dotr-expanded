/* Synthetic host tests for the cell_verdicts group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/cell_verdicts.h"

/* Every callee is a stub; the cell is always (0x12, 0x34). */
static int bit_v, bit_calls, kind_v, kind_calls, grade_v, grade_calls;
static unsigned word_v;
static int word_null, word_calls, cell_v, cell_calls, own_v, own_calls, pair_calls, block_calls;
static int pick_v, pick_calls;
static int block_dummy;
/* 0x141b90 is also run with x of 0. */
static int cell_x = 0x12;
/* 0x130940 also asks about (side 0 or 1, index 0..10): answers come from these. */
static int scan_on, scan_picks, scan_pairs;
static unsigned char scan_present[2][11];
static int scan_pair[2][11];

/* 0x146220 asks bit 3 of the cells 0x24ec50 hands out: entry n is (n, 0x40 + n). */
static int bit3_calls;
static unsigned char bit3_by[8];
/* 0x1211b0 asks rows of both sides: (side 0 or 1, row 1..10) answers come from these. */
static int row_on, row_self, row_kind[2][11], row_side[2][11], row_bit[2][11];
int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    if (row_on && b != 0x34) {
        assert((a == 0 || a == 1) && b >= 1 && b < 11 && bit == 4);
        return row_bit[a][b];
    }
    if (bit == 3) {
        assert(a >= 0 && a < 8 && b == 0x40 + a);
        bit3_calls++;
        return bit3_by[a];
    }
    assert(a == cell_x && b == 0x34 && bit == 4);
    bit_calls++;
    return bit_v;
}
int SzDuel_GetUnitKind(int a, int b)
{
    if (row_on && b != 0x34) {
        assert((a == 0 || a == 1) && b >= 1 && b < 5);
        return row_kind[a][b];
    }
    assert(a == cell_x && b == 0x34);
    kind_calls++;
    return kind_v;
}
int TaLogic_GetActionReaction(int a, int b, int c, int d, int e, int f)
{
    assert(a == cell_x && b == 0x34 && c == 0 && d == 0 && e == 6 && f == 0);
    grade_calls++;
    return grade_v;
}
unsigned *func_00250740_word(int a, int b, int n)
{
    assert(a == cell_x && b == 0x34 && n == 0);
    word_calls++;
    return word_null ? NULL : &word_v;
}
static int there_on, there_v, there_calls;
int SzDuel_GetUnitLoc(int a, int b)
{
    if (there_on && b != 0x34) {
        assert(a == 0x21 && b == 0x43);
        there_calls++;
        return there_v;
    }
    assert(a == cell_x && b == 0x34);
    cell_calls++;
    return cell_v;
}
int SzDuel_GetUnitCardNo(int a, int b)
{
    assert(a == cell_x && b == 0x34);
    own_calls++;
    return own_v;
}
int func_00250360_pair(int a, int b)
{
    if (row_on) {
        if (b == 0x34) {
            assert(a == cell_x);
            return row_self;
        }
        assert((a == 0 || a == 1) && b >= 1 && b < 11);
        return row_side[a][b];
    }
    if (scan_on && (a != 0x12 || b != 0x34)) {
        assert((a == 0 || a == 1) && b >= 0 && b < 11 && scan_present[a][b]);
        scan_pairs++;
        return scan_pair[a][b];
    }
    assert(a == cell_x && b == 0x34);
    pair_calls++;
    return 0x55;
}
struct DotrPickBlock *func_001df540_pick(int a, int b)
{
    if (scan_on) {
        assert((a == 0 || a == 1) && b >= 0 && b < 11);
        scan_picks++;
        return scan_present[a][b] ? (struct DotrPickBlock *)&block_dummy : NULL;
    }
    assert(a == cell_x && b == 0x34);
    block_calls++;
    return (struct DotrPickBlock *)&block_dummy;
}
int TaLogic003_IsSpellPlay(int side, unsigned char *item, struct DotrPickBlock *block)
{
    assert(side == 0x55 && item != NULL && block == (struct DotrPickBlock *)&block_dummy);
    assert(own_calls == 1 && pair_calls == 1 && block_calls == 1);
    pick_calls++;
    return pick_v;
}

static int verdict(unsigned high)
{
    unsigned record[3];

    record[0] = record[1] = 0x11111111;
    record[2] = high << 16 | 0xabcd;
    bit_calls = kind_calls = grade_calls = word_calls = cell_calls = own_calls = 0;
    pair_calls = block_calls = pick_calls = 0;
    return TaLogic_IsOpenUnit__0011c140(0x12, 0x34, record);
}

/* TaLogic_IsOpenUnit__0011c140 (branch coverage). */
static void check_verdict(void)
{
    /* the bit-4 query answers non-zero: 0, nothing else asked */
    bit_v = 1, kind_v = 1 << 5;
    assert(verdict(0) == 0 && bit_calls == 1 && kind_calls == 0);
    /* kind >> 5 of 0 and of 2: 2 without the grade */
    bit_v = 0, kind_v = 0x1f;
    assert(verdict(0) == 2 && kind_calls == 1 && grade_calls == 0);
    kind_v = 2 << 5;
    assert(verdict(0) == 2 && kind_calls == 1 && grade_calls == 0);
    /* kind 1 (low bits ignored), grade 5: 2 without the word */
    kind_v = 1 << 5 | 0x1f, grade_v = 5;
    assert(verdict(0) == 2 && grade_calls == 1 && word_calls == 0);
    /* a null word: 2 */
    grade_v = 4, word_null = 1;
    assert(verdict(0) == 2 && word_calls == 1 && cell_calls == 0 && own_calls == 0);
    /* word >> 8 of 13: 1 */
    word_null = 0, word_v = 13 << 8 | 0xff;
    assert(verdict(0) == 1 && cell_calls == 0 && own_calls == 0);
    /* word >> 8 of 2: the record's high halfword against the cell */
    word_v = 2 << 8;
    cell_v = 0x8765;
    assert(verdict(0x8765) == 1 && cell_calls == 1 && own_calls == 0);
    assert(verdict(0x8764) == 2 && cell_calls == 1 && own_calls == 0);
    cell_v = 0xabcd;
    assert(verdict(0x8765) == 2);
    /* any other word: the 0x1e1030 answer against the 0x119f30 answer */
    word_v = 3 << 8, own_v = 721, pick_v = 721;
    assert(verdict(0) == 1 && pick_calls == 1 && cell_calls == 0);
    pick_v = -1;
    assert(verdict(0) == 2 && pick_calls == 1);
    word_v = 0x01000000 | 13 << 8;
    assert(verdict(0) == 2 && pick_calls == 1);
}

static int fit_v, fit_calls, spot_v, spot_calls;

int TaLogic002_IsDoCerem(int pair, int kind)
{
    assert(pair == 0x55 && kind == own_v && pair_calls == 1);
    fit_calls++;
    return fit_v;
}
int SzDuel_GetField(int x, int y)
{
    assert(x == (cell_v & 0xff) && y == cell_v >> 8);
    spot_calls++;
    return spot_v;
}

static int verdict_b(unsigned high)
{
    unsigned record[3];

    record[0] = record[1] = 0x11111111;
    record[2] = high << 16 | 0xabcd;
    bit_calls = kind_calls = grade_calls = word_calls = cell_calls = own_calls = 0;
    pair_calls = fit_calls = spot_calls = 0;
    return TaLogic_IsOpenUnit__0010ee00(0x12, 0x34, record);
}

/* TaLogic_IsOpenUnit__0010ee00 (branch coverage). */
static void check_verdict_b(void)
{
    static const int own_by[] = { 401, 402, 403, 404, 426, 427, 428, 429 };
    int i;

    /* the bit-4 query answers non-zero: 0 after the first cell read only */
    bit_v = 1, kind_v = 0, own_v = 427, cell_v = 0x0709;
    assert(verdict_b(0) == 0 && cell_calls == 1 && bit_calls == 1 && own_calls == 0);
    bit_v = 0;
    /* kind 0: 1 for the four words, 2 for their neighbours */
    for (i = 0; i < 8; i++) {
        own_v = own_by[i], kind_v = 0x1f;
        assert(verdict_b(0) == (i == 1 || i == 2 || i == 5 || i == 6 ? 1 : 2));
        assert(own_calls == 1 && kind_calls == 1 && grade_calls == 0);
    }
    /* kinds 2, 3, 4 and out of range (6, 7, negative): 2 */
    own_v = 427;
    for (i = 2; i < 8; i++) {
        if (i == 5) {
            continue;
        }
        kind_v = i << 5;
        assert(verdict_b(0) == 2 && grade_calls == 0);
    }
    kind_v = -1;
    assert(verdict_b(0) == 2 && grade_calls == 0);
    /* kind 1, grade 5: 2 without the word */
    kind_v = 1 << 5 | 0x1f, grade_v = 5;
    assert(verdict_b(0) == 2 && grade_calls == 1 && word_calls == 0);
    /* a null word: 2 */
    grade_v = 4, word_null = 1;
    assert(verdict_b(0) == 2 && word_calls == 1 && cell_calls == 1);
    /* word >> 8 of 13: 1 */
    word_null = 0, word_v = 13 << 8 | 0xff;
    assert(verdict_b(0) == 1 && cell_calls == 1);
    /* word >> 8 of 2: the record's high halfword against a second cell read */
    word_v = 2 << 8, cell_v = 0x8765;
    assert(verdict_b(0x8765) == 1 && cell_calls == 2);
    assert(verdict_b(0x8764) == 2 && cell_calls == 2);
    /* any other word: 2, nothing more asked */
    word_v = 3 << 8;
    assert(verdict_b(0x8765) == 2 && cell_calls == 1 && pair_calls == 0);
    /* kind 5, grade 5: 2 without the pair */
    kind_v = 5 << 5, grade_v = 5, cell_v = 0x0709;
    assert(verdict_b(0) == 2 && grade_calls == 1 && pair_calls == 0);
    /* fit other than 1: 2 without the spot query */
    grade_v = 6, fit_v = 0;
    assert(verdict_b(0) == 2 && fit_calls == 1 && spot_calls == 0);
    fit_v = 2;
    assert(verdict_b(0) == 2 && fit_calls == 1 && spot_calls == 0);
    /* fit 1: 1 unless the spot query answers 9 */
    fit_v = 1, spot_v = 8;
    assert(verdict_b(0) == 1 && spot_calls == 1 && word_calls == 0);
    spot_v = 9;
    assert(verdict_b(0) == 2 && spot_calls == 1);
    spot_v = 10;
    assert(verdict_b(0) == 1 && spot_calls == 1);
}

static int gate_v, gate_calls;

int TaLogic007_IsDoCerem(int value, int index)
{
    assert(value == 0x55 && index == own_v && pair_calls == 2);
    gate_calls++;
    return gate_v;
}

static int verdict_c(int flag)
{
    bit_calls = kind_calls = grade_calls = cell_calls = own_calls = 0;
    pair_calls = gate_calls = spot_calls = scan_picks = scan_pairs = 0;
    scan_on = 1;
    flag = TaLogic_IsOpenUnit__00130940(0x12, 0x34, (struct DotrRuleOrder *)0, flag);
    scan_on = 0;
    assert(own_calls == 1 && cell_calls == 1 && bit_calls == 1);
    return flag;
}

/* TaLogic_IsOpenUnit__00130940 (branch coverage). */
static void check_verdict_c(void)
{
    int i;

    memset(scan_present, 0, sizeof scan_present);
    /* the bit-4 query answers non-zero: 0 */
    bit_v = 1, kind_v = 0, own_v = 298, cell_v = 0x0403;
    assert(verdict_c(1) == 0 && kind_calls == 0 && pair_calls == 1);
    /* class 0: 298 is 1; 322 follows the flag's low byte; others 2 */
    bit_v = 0, kind_v = 0x1f;
    assert(verdict_c(0) == 1 && kind_calls == 1 && grade_calls == 0);
    own_v = 322;
    assert(verdict_c(0x101) == 1 && verdict_c(1) == 1);
    assert(verdict_c(0) == 2 && verdict_c(0x100) == 2 && verdict_c(2) == 2);
    own_v = 299;
    assert(verdict_c(1) == 2 && scan_picks == 0);
    /* 326: both sides, indices 0..10; nothing present: 2 */
    own_v = 326;
    assert(verdict_c(1) == 2 && scan_picks == 22 && scan_pairs == 0);
    /* three present entries with another answer: 2; four: 1 */
    scan_present[0][0] = scan_present[1][0] = scan_present[0][10] = 1;
    scan_pair[0][0] = scan_pair[1][0] = scan_pair[0][10] = 0x13;
    assert(verdict_c(0) == 2 && scan_picks == 22 && scan_pairs == 3);
    scan_present[1][10] = 1, scan_pair[1][10] = 0x11;
    assert(verdict_c(0) == 1 && scan_pairs == 4);
    /* an entry answering x itself is not counted */
    scan_pair[1][10] = 0x12;
    assert(verdict_c(0) == 2 && scan_pairs == 4);
    /* the count is a byte: 256 + 3 would read as 3 only past 22 entries; all 22: 1 */
    for (i = 0; i < 11; i++) {
        scan_present[0][i] = scan_present[1][i] = 1;
        scan_pair[0][i] = scan_pair[1][i] = 0x34;
    }
    assert(verdict_c(0) == 1 && scan_pairs == 22);
    /* class 1: 2 when the grade is 5, else 1 */
    kind_v = 1 << 5, grade_v = 5;
    assert(verdict_c(0) == 2 && grade_calls == 1 && scan_picks == 0);
    grade_v = 4;
    assert(verdict_c(0) == 1 && grade_calls == 1);
    /* classes 2, 3, 4 and 6: 2 without the grade */
    for (i = 2; i < 7; i++) {
        if (i != 5) {
            kind_v = i << 5;
            assert(verdict_c(0) == 2 && grade_calls == 0 && gate_calls == 0);
        }
    }
    /* class 5: grade 5 is 2; else the gate must be 1 and the spot not 9 */
    kind_v = 5 << 5, grade_v = 5;
    assert(verdict_c(0) == 2 && grade_calls == 1 && gate_calls == 0);
    grade_v = 0, gate_v = 0, spot_v = 1;
    assert(verdict_c(0) == 2 && gate_calls == 1 && spot_calls == 0);
    gate_v = 1, spot_v = 9;
    assert(verdict_c(0) == 2 && gate_calls == 1 && spot_calls == 1);
    spot_v = 8;
    assert(verdict_c(0) == 1 && spot_calls == 1);
}

static int total_by[0x56], total_calls, one_v, one_calls, one_at, set_v, set_e, set_calls;
static int count_v, count_calls, okc_v, okc_calls, set_d = 1;

int func_001df470_side(int side)
{
    assert(side >= 0 && side < 0x56);
    total_calls = total_calls * 0x100 + side + 1;
    return total_by[side];
}
int TaLogic011_IsUseReversalGoddess(int side)
{
    assert(side == 0x55);
    one_calls++, one_at = 0x142280;
    return one_v;
}
int TaLogic011_IsUseBindChain(int side)
{
    assert(side == 0x55);
    one_calls++, one_at = 0x141f30;
    return one_v;
}
/* 0x117870 asks two sets of the pair answer in turn. */
static int alt_on, alt_v[2];
/* 0x15cbb0 asks up to two sets with all six arguments given: k_exp holds them. */
static int k_on, k_n, k_exp[2][6], k_count[2];
int SearchUnitByRange(int a, int b, int c, int d, int e, int f)
{
    if (k_on) {
        assert(k_n < 2 && a == k_exp[k_n][0] && b == k_exp[k_n][1] && c == k_exp[k_n][2]);
        assert(d == k_exp[k_n][3] && e == k_exp[k_n][4] && f == k_exp[k_n][5]);
        return 0x700 + k_n++;
    }
    if (alt_on) {
        assert(a == 0 && b == 0 && c == 0x55 && d == 1 && f == 14);
        assert(e == (set_calls == 0 ? 0x2001c : 28));
        return alt_v[set_calls++];
    }
    assert(a == 0 && b == 0 && c == cell_x && d == set_d && e == set_e && f == 14);
    set_calls++;
    return set_v;
}
int Ta_GetUnitNumFrom22bitFlagList(unsigned value)
{
    if (k_on) {
        assert(k_n > 0 && value == 0x700u + k_n - 1);
        count_calls++;
        return k_count[k_n - 1];
    }
    assert(value == (unsigned)set_v && set_calls == 1);
    count_calls++;
    return count_v;
}
static int okc_x = 0x55, okc_y;
int TaLogic_IsAttackedNow(int a, int b, int c)
{
    assert(a == okc_x && b == okc_y && c == 0);
    okc_calls++;
    return okc_v;
}

static int verdict_d(void)
{
    bit_calls = kind_calls = grade_calls = word_calls = own_calls = pair_calls = 0;
    total_calls = one_calls = one_at = set_calls = count_calls = okc_calls = 0;
    return TaLogic_IsOpenUnit__00141b90(cell_x, 0x34);
}

/* TaLogic_IsOpenUnit__00141b90 (branch coverage). */
static void check_verdict_d(void)
{
    /* the bit-4 query answers non-zero: 0 after the word and the pair */
    bit_v = 1, kind_v = 0, own_v = 368;
    assert(verdict_d() == 0 && own_calls == 1 && pair_calls == 1 && kind_calls == 0);
    /* classes other than 0 and 1: 2 */
    bit_v = 0, kind_v = 2 << 5, grade_v = 4;
    assert(verdict_d() == 2 && kind_calls == 1 && grade_calls == 0 && one_calls == 0);
    /* class 0, word 368 and 386: the 0x142280 / 0x141f30 answer for the pair must be 1 */
    kind_v = 0x1f, one_v = 1;
    assert(verdict_d() == 1 && one_calls == 1 && one_at == 0x142280);
    one_v = 2;
    assert(verdict_d() == 2 && one_calls == 1);
    own_v = 386, one_v = 1;
    assert(verdict_d() == 1 && one_calls == 1 && one_at == 0x141f30);
    one_v = 0;
    assert(verdict_d() == 2 && one_calls == 1);
    /* word 383: the count of the (x, 1, 20, 14) set must reach 3 */
    own_v = 383, set_e = 20, set_v = 0x4321, count_v = 3;
    assert(verdict_d() == 1 && set_calls == 1 && count_calls == 1);
    count_v = 2;
    assert(verdict_d() == 2 && count_calls == 1);
    /* word 388: the total of x below that of side 0 (x non-zero) or 1 (x zero) */
    own_v = 388, total_by[0x12] = 7, total_by[0] = 8, total_by[1] = 7;
    assert(verdict_d() == 1 && total_calls == 0x1301);
    total_by[0] = 7;
    assert(verdict_d() == 2 && total_calls == 0x1301);
    cell_x = 0, total_by[0] = 6;
    assert(verdict_d() == 1 && total_calls == 0x0102);
    total_by[1] = 6;
    assert(verdict_d() == 2 && total_calls == 0x0102);
    cell_x = 0x12;
    /* any other word: 2 */
    own_v = 387;
    assert(verdict_d() == 2 && one_calls == 0 && set_calls == 0 && total_calls == 0);
    /* class 1: grade 5 or a null word is 2 */
    kind_v = 1 << 5, grade_v = 5;
    assert(verdict_d() == 2 && grade_calls == 1 && word_calls == 0);
    grade_v = 4, word_null = 1;
    assert(verdict_d() == 2 && word_calls == 1 && total_calls == 0);
    /* word >> 8 of 13: the pair's total below 2000 */
    word_null = 0, word_v = 13 << 8 | 0xff, total_by[0x55] = 1999;
    assert(verdict_d() == 1 && total_calls == 0x56);
    total_by[0x55] = 2000;
    assert(verdict_d() == 2 && total_calls == 0x56);
    /* 18: the count of the (x, 1, 8, 14) set must reach 4 */
    word_v = 18 << 8, set_e = 8, count_v = 4;
    assert(verdict_d() == 1 && set_calls == 1 && count_calls == 1);
    count_v = 3;
    assert(verdict_d() == 2 && count_calls == 1);
    /* 37: TaLogic_IsAttackedNow(pair, 0, 0) must be 1 */
    word_v = 37 << 8, okc_v = 1;
    assert(verdict_d() == 1 && okc_calls == 1);
    okc_v = 3;
    assert(verdict_d() == 2 && okc_calls == 1);
    /* any other: 2 */
    word_v = 14 << 8;
    assert(verdict_d() == 2 && okc_calls == 0 && set_calls == 0 && total_calls == 0);
}

static int entry_calls, side_v, side_calls, gate_b_v, gate_b_calls;

int Ta_GetUnitOwnerIDFromLstCount(int word, int n)
{
    assert(word == set_v && n == entry_calls && n < 8);
    entry_calls++;
    return n << 8 | (0x40 + n);
}
int TaLogic012_IsPinch(int side)
{
    assert(side == 0x55);
    side_calls++;
    return side_v;
}
int TaLogic012_IsDoCerem(int value, int index)
{
    assert(value == 0x55 && index == 846 && pair_calls == 2);
    gate_b_calls++;
    return gate_b_v;
}

static int verdict_e(void)
{
    int r;

    bit_calls = grade_calls = cell_calls = own_calls = pair_calls = spot_calls = 0;
    set_calls = count_calls = entry_calls = bit3_calls = side_calls = gate_b_calls = 0;
    r = TaLogic_IsOpenUnit__00146220(0x12, 0x34);
    assert(cell_calls == 1 && own_calls == 1 && bit_calls == 1);
    return r;
}

/* TaLogic_IsOpenUnit__00146220 (branch coverage). */
static void check_verdict_e(void)
{
    /* the bit-4 query answers non-zero: 0 */
    bit_v = 1, own_v = 192, cell_v = 0x0403, set_v = 0x4321;
    assert(verdict_e() == 0 && pair_calls == 1 && set_calls == 0);
    /* 192: the count of the (x, 1, 5, 14) set must reach 3 */
    bit_v = 0, set_d = 1, set_e = 5, count_v = 3;
    assert(verdict_e() == 1 && set_calls == 1 && count_calls == 1);
    count_v = 2;
    assert(verdict_e() == 2 && count_calls == 1);
    /* 325: entries of the (x, 2, 52, 14) set with bit 3; an empty set asks nothing */
    own_v = 325, set_d = 2, set_e = 52, count_v = 0;
    assert(verdict_e() == 2 && set_calls == 1 && count_calls == 1 && entry_calls == 0);
    /* five entries, two with the bit: 2; three: 1 */
    count_v = 5;
    memset(bit3_by, 0, sizeof bit3_by);
    bit3_by[0] = bit3_by[4] = 1;
    assert(verdict_e() == 2 && entry_calls == 5 && bit3_calls == 5 && count_calls == 6);
    bit3_by[2] = 1;
    assert(verdict_e() == 1 && entry_calls == 5 && bit3_calls == 5);
    /* an entry past the count is not read */
    bit3_by[2] = 0, bit3_by[5] = 1;
    assert(verdict_e() == 2 && entry_calls == 5);
    set_d = 1;
    /* 731 and 751: grade 5 is 2; else 0x1465a0 of the pair must be 1 */
    own_v = 731, grade_v = 5;
    assert(verdict_e() == 2 && grade_calls == 1 && side_calls == 0);
    grade_v = 4, side_v = 1;
    assert(verdict_e() == 1 && side_calls == 1);
    own_v = 751;
    assert(verdict_e() == 1 && grade_calls == 1 && side_calls == 1);
    side_v = 0;
    assert(verdict_e() == 2 && side_calls == 1);
    grade_v = 5;
    assert(verdict_e() == 2 && side_calls == 0);
    /* 846: grade 5 is 2; else the 0x1466a0 gate must be 1 and the spot not 9 */
    own_v = 846;
    assert(verdict_e() == 2 && grade_calls == 1 && gate_b_calls == 0);
    grade_v = 0, gate_b_v = 0, spot_v = 1;
    assert(verdict_e() == 2 && gate_b_calls == 1 && spot_calls == 0);
    gate_b_v = 1, spot_v = 9;
    assert(verdict_e() == 2 && gate_b_calls == 1 && spot_calls == 1);
    spot_v = 8;
    assert(verdict_e() == 1 && spot_calls == 1);
    /* any other word: 2 with nothing more asked */
    own_v = 847;
    assert(verdict_e() == 2 && grade_calls == 0 && set_calls == 0 && pair_calls == 1);
}

static unsigned slot_word;
static int slot_null;
static int slot_calls, rank_calls, rank_v, gate_f_calls, gate_f_v, pack_calls, pack_v;
static int tail_calls, tail_v;
unsigned *SzDuel_GetUnitEffect(int a, int b, int n)
{
    assert(a == cell_x && b == 0x34 && n == 0);
    slot_calls++;
    return slot_null ? NULL : &slot_word;
}
int TaUtil_GetGraveCardNum(int index, int kind)
{
    assert(index == 0x55 && kind == 0);
    rank_calls++;
    return rank_v;
}
int TaLogic001_IsDoCerem(int value, int index)
{
    assert(value == 0x55 && index == own_v);
    gate_f_calls++;
    return gate_f_v;
}
int TaLogic001_IsLandNearCntr(int packed)
{
    assert(packed == cell_v);
    pack_calls++;
    return pack_v;
}
int TaLogicCom_IsOpenUnit__00111320(int x, int y, int z)
{
    assert(x == cell_x && y == 0x34 && z == 0x77);
    tail_calls++;
    return tail_v;
}

static int verdict_f(void)
{
    bit_calls = kind_calls = grade_calls = cell_calls = own_calls = pair_calls = 0;
    set_calls = spot_calls = slot_calls = rank_calls = gate_f_calls = pack_calls = tail_calls = 0;
    return TaLogic_IsOpenUnit__00117870(cell_x, 0x34, 0x77);
}

/* TaLogic_IsOpenUnit__00117870 (branch coverage). */
static void check_verdict_f(void)
{
    /* the bit-4 query answers non-zero: 0, after the four opening queries */
    bit_v = 1, cell_v = 0x3412, alt_on = 1;
    assert(verdict_f() == 0 && cell_calls == 1 && own_calls == 1 && pair_calls == 1);
    assert(slot_calls == 1 && kind_calls == 0);
    /* class 0, word 83: either set of the pair answer */
    bit_v = 0, kind_v = 0, own_v = 83, alt_v[0] = 1, alt_v[1] = 0;
    assert(verdict_f() == 1 && set_calls == 1);
    alt_v[0] = 0, alt_v[1] = 7;
    assert(verdict_f() == 1 && set_calls == 2);
    alt_v[1] = 0;
    assert(verdict_f() == 2 && set_calls == 2);
    /* word 146: 0x250940 of at least 6 */
    own_v = 146, rank_v = 6;
    assert(verdict_f() == 1 && rank_calls == 1 && set_calls == 0);
    rank_v = 5;
    assert(verdict_f() == 2 && rank_calls == 1);
    /* any other word: 2 */
    own_v = 84;
    assert(verdict_f() == 2 && rank_calls == 0 && set_calls == 0 && grade_calls == 0);
    /* class 5: grade 5 is 2; else the gate must be 1 and the spot not 9 */
    kind_v = 5 << 5, grade_v = 5;
    assert(verdict_f() == 2 && grade_calls == 1 && gate_f_calls == 0);
    grade_v = 4, gate_f_v = 0, spot_v = 8;
    assert(verdict_f() == 2 && gate_f_calls == 1 && pair_calls == 2 && spot_calls == 0);
    gate_f_v = 1, spot_v = 9;
    assert(verdict_f() == 2 && spot_calls == 1);
    spot_v = 8;
    assert(verdict_f() == 1 && spot_calls == 1);
    /* class 1: grade 5 is 2; else by the slot word >> 8 */
    kind_v = 1 << 5, grade_v = 5, slot_word = 13 << 8;
    assert(verdict_f() == 2 && grade_calls == 1);
    grade_v = 0, slot_word = (13 << 8) | 0xff;
    assert(verdict_f() == 1 && tail_calls == 0);
    slot_word = 2 << 8, pack_v = 1;
    assert(verdict_f() == 1 && cell_calls == 2 && pack_calls == 1);
    pack_v = 0;
    assert(verdict_f() == 2 && pack_calls == 1 && tail_calls == 0);
    slot_word = 20 << 8, tail_v = 0x99;
    assert(verdict_f() == 2 && tail_calls == 0);
    /* any other, including a set top bit (unsigned shift): 0x111320 */
    slot_word = 21 << 8;
    assert(verdict_f() == 0x99 && tail_calls == 1);
    slot_word = 0x80000000u | (13 << 8);
    assert(verdict_f() == 0x99 && tail_calls == 1);
    /* other classes: 2 with nothing more asked */
    kind_v = 2 << 5;
    assert(verdict_f() == 2 && grade_calls == 0 && tail_calls == 0 && set_calls == 0);
    alt_on = 0;
}

/* Linked for 0x1211b0's 502 case, which the host tests do not run. */
void TaUtil_GetUnitLst(int kind, int mode, unsigned short *out)
{
    (void)kind, (void)mode, (void)out;
    assert(0);
}
int TaLogic_GetMaxApUnit(unsigned short *words)
{
    (void)words;
    assert(0);
    return 0;
}

static int verdict_g(void)
{
    grade_calls = word_calls = own_calls = 0;
    return TaLogic_IsOpenUnit__001211b0(cell_x, 0x34, NULL);
}

/* TaLogic_IsOpenUnit__001211b0 (branch and loop coverage, except the 502 case: it writes 22
 * entries into a 16-entry frame array, which a host run cannot do safely). */
static void check_verdict_g(void)
{
    int i;

    cell_x = 1, row_on = 1, row_self = 0;
    memset(row_kind, 0, sizeof row_kind);
    memset(row_side, 0, sizeof row_side);
    memset(row_bit, 0, sizeof row_bit);
    /* the bit-4 query answers non-zero: 0 */
    bit_v = 1, kind_v = 0, own_v = 514;
    assert(verdict_g() == 0 && own_calls == 0);
    /* classes 2 to 5 and out-of-range ones: 0 */
    bit_v = 0;
    for (i = 2; i < 8; i++) {
        kind_v = i << 5;
        assert(verdict_g() == 0 && own_calls == 0 && grade_calls == 0);
    }
    /* class 0, word 514: three counted cells; only the low five bits and an answer of x count */
    kind_v = 0;
    row_kind[1][1] = 14 | 0x20, row_side[1][1] = 1;
    row_kind[0][4] = 14, row_side[0][4] = 1;
    row_kind[0][1] = 14, row_side[0][1] = 0;
    row_kind[0][2] = 15, row_side[0][2] = 1;
    assert(verdict_g() == 2 && own_calls == 1);
    row_kind[1][4] = 14, row_side[1][4] = 1;
    assert(verdict_g() == 1);
    /* class 1: grade 5, a null word and another word are 2; 18 goes by the count */
    kind_v = 1 << 5, grade_v = 5, word_null = 0, word_v = 18 << 8;
    assert(verdict_g() == 2 && grade_calls == 1 && word_calls == 0);
    grade_v = 0, word_null = 1;
    assert(verdict_g() == 2 && word_calls == 1);
    word_null = 0;
    assert(verdict_g() == 1 && word_calls == 1);
    word_v = 17 << 8;
    assert(verdict_g() == 2);
    word_v = 0x80000000u | (18 << 8);
    assert(verdict_g() == 2);
    word_v = 18 << 8, row_side[1][4] = 0;
    assert(verdict_g() == 2);
    /* any other word of class 0: 2 */
    kind_v = 0, own_v = 515;
    assert(verdict_g() == 2 && own_calls == 1);
    /* 523: rows 1..10 of both sides with another answer than the pair's and a clear bit 4 */
    own_v = 523;
    memset(row_side, 0, sizeof row_side);
    assert(verdict_g() == 2);
    row_side[0][0] = row_side[1][0] = 1;
    row_side[0][1] = row_side[0][10] = row_side[1][1] = row_side[1][10] = row_side[0][5] = 1;
    assert(verdict_g() == 2);
    row_side[1][6] = 1;
    assert(verdict_g() == 1);
    row_bit[1][10] = 1;
    assert(verdict_g() == 2);
    row_bit[1][10] = 0, row_bit[0][2] = 1;
    assert(verdict_g() == 1);
    cell_x = 0x12, row_on = 0;
}

static int copy_v, copy_calls, try_v, try_calls, near_v, near_calls, word_b_calls;
static int try_d = 2, try_e = 6;

int TaLogic013_IsDoCerem(int value, int index)
{
    assert(value == 0x55 && index == own_v);
    copy_calls++;
    return copy_v;
}

int func_001df540_word(int a, int b)
{
    assert(a == cell_x && b == 0x34);
    word_b_calls++;
    return 0x7654;
}

int TaLogic_GetLandChange(int a, int b, int c, int d, int e, int f)
{
    assert(a == 0x7654 && b == (cell_v & 0xff) && c == cell_v >> 8 && d == try_d && e == try_e && f == 2);
    try_calls++;
    return try_v;
}

int TaLogic013_IsUseHarpySweaper(int a, int b, int c)
{
    assert(a == cell_x && b == (cell_v & 0xff) && c == cell_v >> 8);
    near_calls++;
    return near_v;
}

static int verdict_h(void)
{
    bit_calls = kind_calls = grade_calls = cell_calls = own_calls = pair_calls = 0;
    set_calls = count_calls = spot_calls = slot_calls = 0;
    copy_calls = try_calls = near_calls = word_b_calls = 0;
    return TaLogic_IsOpenUnit__0014ae60(cell_x, 0x34);
}

/* 0x14ae60: one assertion per branch outcome. */
static void check_verdict_h(void)
{
    row_on = scan_on = alt_on = 0;
    cell_x = 0x12, cell_v = 0x0403, set_v = 0x4321, set_d = 1, slot_null = 0;
    /* the bit-4 query answers non-zero: 0, after the four opening queries */
    bit_v = 1, kind_v = 0, own_v = 291;
    assert(verdict_h() == 0 && own_calls == 1 && cell_calls == 1 && pair_calls == 1 && slot_calls == 1);
    assert(bit_calls == 1 && kind_calls == 0);
    /* group 0, 291: the count of the (x, 1, 6, 14) set must reach 3 */
    bit_v = 0, set_e = 6, count_v = 3;
    assert(verdict_h() == 1 && set_calls == 1 && count_calls == 1);
    count_v = 2;
    assert(verdict_h() == 2 && count_calls == 1);
    /* 272 and 273: the count of the (x, 1, 0x1e001c, 14) set must be positive */
    own_v = 272, set_e = 0x1e001c, count_v = 1;
    assert(verdict_h() == 1 && set_calls == 1 && count_calls == 1);
    own_v = 273;
    assert(verdict_h() == 1 && set_calls == 1);
    count_v = 0;
    assert(verdict_h() == 2 && count_calls == 1);
    own_v = 272;
    assert(verdict_h() == 2 && count_calls == 1);
    /* any other word in group 0: 2 with no set asked */
    own_v = 274;
    assert(verdict_h() == 2 && set_calls == 0 && grade_calls == 0);
    /* group 5: grade 5 is 2; else the 0x14b240 gate must be 1 and the spot not 9 */
    kind_v = 5 << 5 | 3, grade_v = 5;
    assert(verdict_h() == 2 && grade_calls == 1 && copy_calls == 0);
    grade_v = 4, copy_v = 0, spot_v = 1;
    assert(verdict_h() == 2 && copy_calls == 1 && pair_calls == 2 && spot_calls == 0);
    copy_v = 1, spot_v = 9;
    assert(verdict_h() == 2 && copy_calls == 1 && spot_calls == 1);
    spot_v = 8;
    assert(verdict_h() == 1 && spot_calls == 1);
    /* group 1: grade 5 is 2, a null word is 2 */
    kind_v = 1 << 5, grade_v = 5, slot_word = 13 << 8;
    assert(verdict_h() == 2 && grade_calls == 1);
    grade_v = 0, slot_null = 1;
    assert(verdict_h() == 2 && grade_calls == 1 && try_calls == 0 && near_calls == 0);
    slot_null = 0;
    /* word >> 8 of 13: 1 */
    assert(verdict_h() == 1 && try_calls == 0 && near_calls == 0);
    /* 2: the 0x27c770 answer must be the cell and not negative */
    slot_word = 2 << 8 | 0x7f, try_v = 0x0403;
    assert(verdict_h() == 1 && word_b_calls == 1 && try_calls == 1);
    try_v = 0x0404;
    assert(verdict_h() == 2 && try_calls == 1);
    try_v = -1;
    assert(verdict_h() == 2 && try_calls == 1);
    cell_v = -1;
    assert(verdict_h() == 2 && try_calls == 1);
    cell_v = 0x0403;
    /* 4: the 0x14b3e0 answer must be 1 */
    slot_word = 4 << 8, near_v = 1;
    assert(verdict_h() == 1 && near_calls == 1 && try_calls == 0);
    near_v = 2;
    assert(verdict_h() == 2 && near_calls == 1);
    /* any other word: 2 */
    slot_word = 5 << 8;
    assert(verdict_h() == 2 && near_calls == 0 && try_calls == 0);
    /* any other group: 2 with nothing more asked */
    kind_v = 2 << 5;
    assert(verdict_h() == 2 && grade_calls == 0 && set_calls == 0);
    kind_v = 6 << 5;
    assert(verdict_h() == 2 && grade_calls == 0);
}

static int list_v, list_calls, copy_b_v, copy_b_calls, spot_i_calls, item_calls, item_at[3];
static int spot_neg, item_free, full_v, full_calls, many_v, many_calls;

int TaLogic014_IsUnitNaturalEffectUse(int side, int kind)
{
    assert(side == 0x55 && kind == own_v);
    list_calls++;
    return list_v;
}

int TaLogic014_IsDoCerem(int value, int index)
{
    assert(value == 0x55 && index == own_v);
    copy_b_calls++;
    return copy_b_v;
}

int TaLogic014_GetLine(int index)
{
    assert(index >= 0 && index < 3);
    spot_i_calls++;
    if (spot_neg && item_free) {
        return -1;
    }
    return 0x100 * (index + 1) + index + 0x10;
}

struct DotrStateItem *SzDuel_GetMapUnit(int x, int y)
{
    assert(x >= 0x10 && x < 0x13 && y == x - 0x10 + 1);
    item_calls++;
    item_free = !item_at[x - 0x10];
    return item_at[x - 0x10] ? (struct DotrStateItem *)&block_dummy : NULL;
}

int SzDuel_GetUnitSpellCount(int side)
{
    assert(side == cell_x);
    full_calls++;
    return full_v;
}

int SzDuel_GetUnitMonsterCount(int side)
{
    assert(side == cell_x);
    many_calls++;
    return many_v;
}

static int verdict_i(void)
{
    bit_calls = kind_calls = grade_calls = cell_calls = own_calls = pair_calls = 0;
    spot_calls = slot_calls = try_calls = word_b_calls = 0;
    list_calls = copy_b_calls = spot_i_calls = item_calls = full_calls = many_calls = 0;
    item_free = 0;
    return TaLogic_IsOpenUnit__0014f490(cell_x, 0x34);
}

/* 0x14f490: one assertion per branch outcome and loop boundary. */
static void check_verdict_i(void)
{
    row_on = scan_on = alt_on = 0;
    cell_x = 0x12, cell_v = 0x0403, slot_null = 0;
    /* the bit-4 query answers non-zero: 0, after the four opening queries */
    bit_v = 1, kind_v = 0, own_v = 551;
    assert(verdict_i() == 0 && own_calls == 1 && cell_calls == 1 && pair_calls == 1 && slot_calls == 1);
    assert(bit_calls == 1 && kind_calls == 0);
    /* group 0, 551 and 612: the 0x14fa10 answer for the pair must be 1 */
    bit_v = 0, list_v = 1;
    assert(verdict_i() == 1 && list_calls == 1);
    own_v = 612;
    assert(verdict_i() == 1 && list_calls == 1);
    list_v = 2;
    assert(verdict_i() == 2 && list_calls == 1);
    own_v = 551;
    assert(verdict_i() == 2 && list_calls == 1);
    own_v = 552;
    assert(verdict_i() == 2 && list_calls == 0 && grade_calls == 0);
    /* group 5: grade 5 is 2; else the 0x14f8c0 gate must be 1 and the spot not 9 */
    kind_v = 5 << 5 | 3, grade_v = 5;
    assert(verdict_i() == 2 && grade_calls == 1 && copy_b_calls == 0);
    grade_v = 4, copy_b_v = 0, spot_v = 1;
    assert(verdict_i() == 2 && copy_b_calls == 1 && pair_calls == 2 && spot_calls == 0);
    copy_b_v = 1, spot_v = 9;
    assert(verdict_i() == 2 && copy_b_calls == 1 && spot_calls == 1);
    spot_v = 8;
    assert(verdict_i() == 1 && spot_calls == 1);
    /* group 1: grade 5 is 2, a null word is 2, word >> 8 of 13 is 1 */
    kind_v = 1 << 5, grade_v = 5, slot_word = 13 << 8;
    assert(verdict_i() == 2 && grade_calls == 1);
    grade_v = 0, slot_null = 1;
    assert(verdict_i() == 2 && grade_calls == 1 && try_calls == 0);
    slot_null = 0;
    assert(verdict_i() == 1 && try_calls == 0);
    /* 2: the TaLogic_GetLandChange(.., 4, 9, 2) answer must be the cell and not negative */
    slot_word = 2 << 8 | 0x7f, try_d = 4, try_e = 9, try_v = 0x0403;
    assert(verdict_i() == 1 && word_b_calls == 1 && try_calls == 1);
    try_v = 0x0404;
    assert(verdict_i() == 2 && try_calls == 1);
    try_v = -1;
    assert(verdict_i() == 2 && try_calls == 1);
    cell_v = -1;
    assert(verdict_i() == 2 && try_calls == 1);
    cell_v = 0x0403, try_d = 2, try_e = 6;
    /* 4: all three spots taken and 0x1e01f0 of 0: 1 */
    slot_word = 4 << 8, full_v = 0;
    item_at[0] = item_at[1] = item_at[2] = 1;
    assert(verdict_i() == 1 && spot_i_calls == 3 && item_calls == 3 && full_calls == 1);
    full_v = 1;
    assert(verdict_i() == 2 && full_calls == 1);
    /* a free spot (first, last) is asked again and ends the search: 2 */
    full_v = 0, item_at[0] = 0;
    assert(verdict_i() == 2 && spot_i_calls == 2 && item_calls == 1 && full_calls == 0);
    item_at[0] = 1, item_at[2] = 0;
    assert(verdict_i() == 2 && spot_i_calls == 4 && item_calls == 3 && full_calls == 0);
    /* the second answer for the free spot is what is tested: negative counts as none */
    spot_neg = 1;
    assert(verdict_i() == 1 && spot_i_calls == 4 && full_calls == 1);
    spot_neg = 0, item_at[2] = 1;
    /* 18: 0x1e0150 of 4 or more */
    slot_word = 18 << 8, many_v = 4;
    assert(verdict_i() == 1 && many_calls == 1 && spot_i_calls == 0);
    many_v = 3;
    assert(verdict_i() == 2 && many_calls == 1);
    /* any other word, any other group: 2 */
    slot_word = 5 << 8;
    assert(verdict_i() == 2 && many_calls == 0 && spot_i_calls == 0 && try_calls == 0);
    kind_v = 2 << 5;
    assert(verdict_i() == 2 && grade_calls == 0);
    kind_v = 6 << 5;
    assert(verdict_i() == 2 && grade_calls == 0 && list_calls == 0);
}

static int copy_c_v, copy_c_calls, far_calls, gate_g_v, gate_g_calls, last_v, last_calls;

int TaLogic019_IsDoCerem(int value, int index)
{
    assert(value == 0x55 && index == own_v);
    copy_c_calls++;
    return copy_c_v;
}

int TaLogic019_GetUseThunderBolt(int side)
{
    assert(side == 0x55);
    far_calls++;
    return 0x2143;
}

int TaLogic019_IsUseGFK(int side)
{
    assert(side == 0x55);
    gate_g_calls++;
    return gate_g_v;
}

int TaLogic019_IsUseRevolt(int side)
{
    assert(side == 0x55);
    last_calls++;
    return last_v;
}

static int verdict_j(void)
{
    bit_calls = kind_calls = grade_calls = cell_calls = own_calls = pair_calls = 0;
    spot_calls = slot_calls = okc_calls = there_calls = 0;
    copy_c_calls = far_calls = gate_g_calls = last_calls = 0;
    return TaLogic_IsOpenUnit__001620b0(cell_x, 0x34);
}

/* 0x1620b0: one assertion per branch outcome. */
static void check_verdict_j(void)
{
    row_on = scan_on = alt_on = 0;
    cell_x = 0x12, cell_v = 0x0403, slot_null = 0, there_on = 1;
    /* the bit-4 query answers non-zero: 0, after the four opening queries */
    bit_v = 1, kind_v = 5 << 5, own_v = 749;
    assert(verdict_j() == 0 && own_calls == 1 && cell_calls == 1 && pair_calls == 1 && slot_calls == 1);
    assert(bit_calls == 1 && kind_calls == 0);
    /* group 5: grade 5 is 2; else the 0x162490 gate must be 1 and the spot not 9 */
    bit_v = 0, grade_v = 5;
    assert(verdict_j() == 2 && grade_calls == 1 && copy_c_calls == 0);
    grade_v = 4, copy_c_v = 0, spot_v = 1;
    assert(verdict_j() == 2 && copy_c_calls == 1 && pair_calls == 2 && spot_calls == 0);
    copy_c_v = 1, spot_v = 9;
    assert(verdict_j() == 2 && copy_c_calls == 1 && spot_calls == 1);
    spot_v = 8;
    assert(verdict_j() == 1 && spot_calls == 1);
    /* group 1: grade 5 is 2, a null word is 2 */
    kind_v = 1 << 5, grade_v = 5, slot_word = 4 << 8;
    assert(verdict_j() == 2 && grade_calls == 1 && far_calls == 0);
    grade_v = 0, slot_null = 1;
    assert(verdict_j() == 2 && grade_calls == 1 && far_calls == 0);
    slot_null = 0;
    /* 4: same low byte, same high part, neither */
    there_v = 0x0903;
    assert(verdict_j() == 1 && far_calls == 1 && cell_calls == 2 && there_calls == 1);
    there_v = 0x0409;
    assert(verdict_j() == 1 && there_calls == 1);
    there_v = 0x0509;
    assert(verdict_j() == 2 && far_calls == 1 && there_calls == 1);
    /* 73: the 0x1626f0 answer must be 1 */
    slot_word = 73 << 8 | 1, gate_g_v = 1;
    assert(verdict_j() == 1 && gate_g_calls == 1 && far_calls == 0);
    gate_g_v = 0;
    assert(verdict_j() == 2 && gate_g_calls == 1);
    /* 37: word 749 asks TaLogic_IsAttackedNow(x, 0, 0), word 746 asks (x, y, 0) */
    slot_word = 37 << 8, okc_x = cell_x, okc_y = 0, okc_v = 1;
    assert(verdict_j() == 1 && okc_calls == 1);
    okc_v = 0;
    assert(verdict_j() == 2 && okc_calls == 1);
    own_v = 746, okc_y = 0x34, okc_v = 1;
    assert(verdict_j() == 1 && okc_calls == 1);
    okc_v = 2;
    assert(verdict_j() == 2 && okc_calls == 1);
    own_v = 747;
    assert(verdict_j() == 2 && okc_calls == 0);
    okc_x = 0x55, okc_y = 0;
    /* 31: only word 733 asks 0x162d10 */
    slot_word = 31 << 8, last_v = 1;
    assert(verdict_j() == 2 && last_calls == 0);
    own_v = 733;
    assert(verdict_j() == 1 && last_calls == 1);
    last_v = 0;
    assert(verdict_j() == 2 && last_calls == 1);
    /* any other word, any other group: 2 */
    slot_word = 5 << 8;
    assert(verdict_j() == 2 && last_calls == 0 && okc_calls == 0 && gate_g_calls == 0);
    kind_v = 0;
    assert(verdict_j() == 2 && grade_calls == 0);
    kind_v = 6 << 5;
    assert(verdict_j() == 2 && grade_calls == 0 && copy_c_calls == 0);
    there_on = 0;
}

static int low_v, low_calls, copy_k_v, copy_k_calls;
int SzDuel_GetSummonPower(int index)
{
    assert(index == 0x55);
    low_calls++;
    return low_v;
}

int TaLogic018_IsDoCerem(int value, int index)
{
    assert(value == 0x55 && index == own_v);
    copy_k_calls++;
    return copy_k_v;
}

static void k_set(int n, int a, int b, int c, int d, int e, int f, int count)
{
    k_exp[n][0] = a, k_exp[n][1] = b, k_exp[n][2] = c;
    k_exp[n][3] = d, k_exp[n][4] = e, k_exp[n][5] = f;
    k_count[n] = count;
}

static int verdict_k(void)
{
    bit_calls = kind_calls = grade_calls = cell_calls = own_calls = pair_calls = 0;
    spot_calls = slot_calls = count_calls = low_calls = copy_k_calls = k_n = 0;
    return TaLogic_IsOpenUnit__0015cbb0(cell_x, 0x34);
}

/* 0x15cbb0: one assertion per branch outcome and per count boundary. */
static void check_verdict_k(void)
{
    row_on = scan_on = alt_on = 0, k_on = 1;
    cell_x = 0x12, cell_v = 0x0403, slot_null = 0;
    /* the bit-4 query answers non-zero: 0, after the four opening queries */
    bit_v = 1, kind_v = 0, own_v = 224;
    assert(verdict_k() == 0 && own_calls == 1 && cell_calls == 1 && pair_calls == 1 && slot_calls == 1);
    assert(bit_calls == 1 && kind_calls == 0);
    /* group 0, word 224: 1 while the 0x1de5c0 answer is under 2 */
    bit_v = 0, low_v = 1;
    assert(verdict_k() == 1 && low_calls == 1 && k_n == 0);
    low_v = 2;
    assert(verdict_k() == 2 && low_calls == 1 && k_n == 0);
    /* word 652: the first count of 2 is 2; else the second count of 6 is 1 */
    own_v = 652;
    k_set(0, 3, 4, 0x55, 1, 52, 20, 2);
    k_set(1, 3, 4, 0x55, 2, 52, 20, 6);
    assert(verdict_k() == 2 && k_n == 1 && count_calls == 1 && low_calls == 0);
    k_count[0] = 1;
    assert(verdict_k() == 1 && k_n == 2 && count_calls == 2);
    k_count[1] = 5;
    assert(verdict_k() == 2 && k_n == 2 && count_calls == 2);
    /* word 655: a count of 6 is 1 */
    own_v = 655;
    k_set(0, 0, 0, 0x12, 2, 52, 14, 6);
    assert(verdict_k() == 1 && k_n == 1 && count_calls == 1);
    k_count[0] = 5;
    assert(verdict_k() == 2 && k_n == 1);
    /* any other word */
    own_v = 225;
    assert(verdict_k() == 2 && k_n == 0 && low_calls == 0 && grade_calls == 0);
    /* group 5: grade 5 is 2; else the 0x15d030 gate must be 1 and the spot not 9 */
    kind_v = 5 << 5, grade_v = 5;
    assert(verdict_k() == 2 && grade_calls == 1 && copy_k_calls == 0);
    grade_v = 4, copy_k_v = 0, spot_v = 1;
    assert(verdict_k() == 2 && copy_k_calls == 1 && pair_calls == 2 && spot_calls == 0);
    copy_k_v = 1, spot_v = 9;
    assert(verdict_k() == 2 && copy_k_calls == 1 && spot_calls == 1);
    spot_v = 8;
    assert(verdict_k() == 1 && spot_calls == 1);
    /* group 1: grade 5 is 2, a null word is 2 */
    kind_v = 1 << 5, grade_v = 5, slot_word = 24 << 8;
    assert(verdict_k() == 2 && grade_calls == 1 && k_n == 0);
    grade_v = 0, slot_null = 1;
    assert(verdict_k() == 2 && grade_calls == 1 && k_n == 0);
    slot_null = 0;
    /* 24: a count of 6 is 1 (the low byte of the word is ignored) */
    slot_word = 24 << 8 | 0xff;
    k_set(0, 0, 0, 0x12, 2, 52, 14, 6);
    assert(verdict_k() == 1 && k_n == 1 && count_calls == 1);
    k_count[0] = 5;
    assert(verdict_k() == 2 && k_n == 1);
    /* 18: a positive first count is 2; else the second count of 3 is 1 */
    slot_word = 18 << 8;
    k_set(0, 3, 4, 0x55, 1, 29, 29, 1);
    k_set(1, 0, 0, 0x55, 1, 19, 14, 3);
    assert(verdict_k() == 2 && k_n == 1 && count_calls == 1);
    k_count[0] = 0;
    assert(verdict_k() == 1 && k_n == 2 && count_calls == 2);
    k_count[1] = 2;
    assert(verdict_k() == 2 && k_n == 2);
    /* any other word, any other group: 2 */
    slot_word = 19 << 8;
    assert(verdict_k() == 2 && k_n == 0);
    kind_v = 6 << 5;
    assert(verdict_k() == 2 && grade_calls == 0 && copy_k_calls == 0);
    k_on = 0;
}

int main(void)
{
    check_verdict();
    check_verdict_b();
    check_verdict_c();
    check_verdict_d();
    check_verdict_e();
    check_verdict_f();
    check_verdict_g();
    check_verdict_h();
    check_verdict_i();
    check_verdict_j();
    check_verdict_k();
    return 0;
}
