/* Synthetic host tests for the score_scans group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/score_scans.h"

/* Every callee is a stub. Answers are keyed by the second (column) argument or by call order. */
static int cell_by[32], cell_calls, cell_first[2];
static int busy_answer, busy_calls;
static int set_word[4], set_calls, set_args[4][6];
static int first_answer, first_calls;
static unsigned short list[22];
static int list_count, fill_calls, fill_args[3], length_calls;
static int bit_by[32], bit_calls, e15_by[32], e15_calls, e16_by[32], e16_calls;
static int score_by[32], score_calls, score_args[4];
static int entry_of[8], entry_calls;
static int count_of[8], pair_by[32];
/* 0x112600 family: the (.., 1, 52, 18) sets answer 0x100 + 7 * x + y and hold entries 0xa00 | n */
static int grid_calls, grid_side, grid_count[49], bit4_by[32], e07_by[32], e07_calls;
static unsigned short other_list[22];
static int other_calls, other_side, pick_answer[2], pick_calls, join_answer, join_calls, join_args[4];

static int score_first[4], both_mode, both_side, slot_by[16], slot_calls, slot_side;

int SzDuel_GetUnitLoc(int a, int b)
{
    if (cell_calls++ == 0) {
        cell_first[0] = a, cell_first[1] = b;
    }
    return cell_by[b & 31];
}
int Ta_SearchUncTypeNaturalEffectUserNum(int kind) { assert(kind == 57); busy_calls++; return busy_answer; }
int SearchUnitByRange(int a, int b, int c, int d, int e, int f)
{
    int v[6] = {a, b, c, d, e, f};
    if (f == 18) {
        assert(a >= 0 && a < 7 && b >= 0 && b < 7 && c == grid_side && d == 1 && e == 52);
        grid_calls++;
        return 0x100 + 7 * a + b;
    }
    assert(set_calls < 4);
    memcpy(set_args[set_calls], v, sizeof(v));
    return set_word[set_calls++];
}
int Ta_RemoveObjFromFlagLst(int word, int side) { assert(word == set_word[0] && side == set_args[0][2]); first_calls++; return first_answer; }
void TaLogic_GetUnitCtlLst(int a, unsigned short *words, int b, int c)
{
    int i;
    for (i = 0; i < 22; i++) {
        assert(words[i] == 0xffff);
    }
    fill_args[0] = a, fill_args[1] = b, fill_args[2] = c;
    fill_calls++;
    memcpy(words, list, sizeof(list));
}
int TaLogic_GetUnitCtlLstLen(unsigned short *words) { assert(memcmp(words, list, sizeof(list)) == 0); length_calls++; return list_count; }
int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    bit_calls++;
    if (bit == 4) {
        assert(a == 10);
        return bit4_by[b & 31];
    }
    assert(a == 9 && bit == 1);
    return bit_by[b & 31];
}
int SzDuel_GetUnitCurseRest(int a, int b) { assert(a == 9); e15_calls++; return e15_by[b & 31]; }
int SzDuel_GetUnitSpellRest(int a, int b) { assert(a == 9); e16_calls++; return e16_by[b & 31]; }
int TaUtil_GetUnitAP(int a, int b, int c, int d)
{
    score_args[0] = a, score_args[1] = b, score_args[2] = c, score_args[3] = d;
    if (score_calls == 0) {
        memcpy(score_first, score_args, sizeof(score_first));
    }
    score_calls++;
    return score_by[b & 31];
}
int Ta_GetUnitOwnerIDFromLstCount(int word, int n)
{
    entry_calls++;
    return word >= 0x100 ? 0xa00 | (n + 1) : entry_of[n & 7];
}
int Ta_GetUnitNumFrom22bitFlagList(unsigned value) { return value >= 0x100 ? grid_count[value - 0x100] : count_of[value & 7]; }
int func_00250360_pair(int a, int b) { (void)a; return pair_by[b & 31]; }
void TaLogic_GetUnitCtlLst2(int a, unsigned short *words, int b, int c, int d)
{
    int i;
    for (i = 0; i < 22; i++) {
        assert(words[i] == 0xffff);
    }
    if (both_mode && other_calls == 1) {
        /* 0x12b000's second call */
        assert(b == 29 && c == 1 && d == 1);
        both_side = a;
        other_calls++;
        memcpy(words, list, sizeof(list));
        return;
    }
    assert(b == 29 && c == 0 && d == 0);
    other_side = a;
    other_calls++;
    memcpy(words, other_list, sizeof(other_list));
}
int TaLogic_GetMaxApUnit(unsigned short *words)
{
    assert(pick_calls < 2);
    assert(memcmp(words, pick_calls == 0 ? other_list : list, sizeof(list)) == 0);
    return pick_answer[pick_calls++];
}
int TaLogic_IsUnitWin(int packed, int b, int c, int d)
{
    join_args[0] = packed, join_args[1] = b, join_args[2] = c, join_args[3] = d;
    join_calls++;
    return join_answer;
}
int SzDuel_GetUnitKind(int a, int b) { assert(a == 10); e07_calls++; return e07_by[b & 31]; }

static void reset(void)
{
    memset(cell_by, 0, sizeof(cell_by));
    memset(bit_by, 0, sizeof(bit_by));
    memset(e15_by, 0, sizeof(e15_by));
    memset(e16_by, 0, sizeof(e16_by));
    memset(score_by, 0, sizeof(score_by));
    memset(entry_of, 0, sizeof(entry_of));
    memset(count_of, 0, sizeof(count_of));
    memset(pair_by, 0, sizeof(pair_by));
    memset(set_word, 0, sizeof(set_word));
    memset(list, 0xff, sizeof(list));
    list_count = busy_answer = first_answer = 0;
}

static int run_step(int (*f)(int), int side)
{
    cell_calls = busy_calls = set_calls = first_calls = fill_calls = length_calls = 0;
    bit_calls = e15_calls = e16_calls = score_calls = entry_calls = 0;
    return f(side);
}

/* TaLogicCom_GetRevoltObj__001110d0, TaLogicCom_GetRevoltObj__0016b3c0, TaLogic009_GetRevoltObj
 * (branch and loop coverage). List entries are 0x900 | n, so the stubs answer by n; the home cell
 * is row 3, column 3. */
static void check_step(int (*f)(int), int flag)
{
    int n;

    reset();
    cell_by[0] = 0x303;
    set_word[0] = 77;
    first_answer = 88;

    /* a positive 0x266df0 answer: -1 before any set or list call */
    busy_answer = 1;
    assert(run_step(f, 0) == -1);
    assert(cell_calls == 1 && cell_first[0] == 1 && cell_first[1] == 0 && busy_calls == 1);
    assert(set_calls == 0 && fill_calls == 0 && length_calls == 0);

    /* an empty list: -1 after the set, first-entry and fill calls */
    busy_answer = 0;
    assert(run_step(f, 1) == -1);
    assert(cell_first[0] == 0 && set_calls == 1 && first_calls == 1 && fill_calls == 1 && length_calls == 1);
    assert(set_args[0][0] == 0 && set_args[0][1] == 0 && set_args[0][2] == 1 && set_args[0][3] == 2);
    assert(set_args[0][4] == 33 && set_args[0][5] == 14);
    assert(fill_args[0] == 0 && fill_args[1] == 52 && fill_args[2] == flag && cell_calls == 1);
    assert(run_step(f, 0) == -1);
    assert(fill_args[0] == 1 && set_args[0][2] == 0);

    /* entries passed over: two steps away, 0 steps away, and one step away in
     * each direction with a non-zero 0x1e2100, 0x1e1580 or 0x1e1640 answer or a
     * score below 1 */
    for (n = 0; n < 22; n++) {
        list[n] = 0x900 | (n + 1);
    }
    list_count = 6;
    cell_by[1] = 0x305;
    cell_by[2] = 0x303;
    cell_by[3] = 0x403, bit_by[3] = 1;
    cell_by[4] = 0x203, e15_by[4] = 1;
    cell_by[5] = 0x304, e16_by[5] = 1;
    cell_by[6] = 0x302, score_by[6] = 0;
    assert(run_step(f, 0) == -1);
    assert(cell_calls == 7 && bit_calls == 4 && e15_calls == 3 && e16_calls == 2 && score_calls == 1);
    assert(score_args[0] == 9 && score_args[1] == 6 && score_args[2] == 2 && score_args[3] == 3);
    assert(entry_calls == 0 && length_calls == 7);

    /* a score of 1 is kept; it is not the first entry of the set: low halfword 0xffff */
    score_by[6] = 1;
    entry_of[0] = 0x999;
    assert(run_step(f, 0) == (int)((0x906u << 16) | 0xffff));
    assert(entry_calls == 1);

    /* a later equal score replaces it, a later lower score does not; the kept
     * entry equal to the set's first entry fills the low halfword */
    list_count = 8;
    cell_by[7] = 0x403, score_by[7] = 1;
    cell_by[8] = 0x203, score_by[8] = 0;
    entry_of[0] = 0x907;
    assert(run_step(f, 0) == (int)((0x907u << 16) | 0x907));
    assert(score_calls == 3 && entry_calls == 2 && length_calls == 9);

    /* a still later higher score that is not the first entry keeps the low halfword */
    score_by[8] = 5;
    assert(run_step(f, 0) == (int)((0x908u << 16) | 0x907));
    assert(entry_calls == 3);
}

static int run_weigh(int (*f)(int, int, int), int side, int flags, int kind)
{
    set_calls = score_calls = cell_calls = 0;
    return f(side, flags, kind);
}

/* TaLogicCom_IsUseObjEffect__001123c0 and TaLogicCom_IsUseObjEffect__0016c6b0 (branch and loop
 * coverage). Set words 1..3 answer in call order; entries are 0x900 | n and side 1 owns those with
 * pair_by[n] == 1. */
static void check_weigh(int (*f)(int, int, int), int limit)
{
    int n;

    reset();
    set_word[0] = 1, set_word[1] = 2, set_word[2] = 3;
    for (n = 0; n < 8; n++) {
        entry_of[n] = 0x900 | (n + 1);
        cell_by[n + 1] = 0x200 | (n + 1);
    }

    /* an empty set and no flags: 0 after one set call */
    assert(run_weigh(f, 1, 0, 40) == 0);
    assert(set_calls == 1 && set_args[0][0] == 0 && set_args[0][1] == 0 && set_args[0][2] == 1);
    assert(set_args[0][3] == 0 && set_args[0][4] == 40 && set_args[0][5] == 14 && score_calls == 0);

    /* the others' best above the limit and above the side's best: 1 */
    count_of[1] = 4;
    pair_by[1] = 1, pair_by[2] = 0, pair_by[3] = 1, pair_by[4] = 0;
    score_by[1] = 100, score_by[2] = limit + 1, score_by[3] = 50, score_by[4] = 7;
    assert(run_weigh(f, 1, 0, 40) == 1);
    assert(score_calls == 4 && cell_calls == 8 && set_calls == 1);
    assert(score_args[0] == 9 && score_args[1] == 4 && score_args[2] == 4 && score_args[3] == 2);
    /* at the limit: not enough */
    score_by[2] = limit;
    assert(run_weigh(f, 1, 0, 40) == 0);
    /* a later higher entry of the others counts */
    score_by[4] = limit + 1;
    assert(run_weigh(f, 1, 0, 40) == 1);
    /* the side's best equal to the others' (first or later entry): no */
    score_by[3] = limit + 1;
    assert(run_weigh(f, 1, 0, 40) == 0);
    score_by[3] = 50, score_by[1] = limit + 1;
    assert(run_weigh(f, 1, 0, 40) == 0);
    /* for the other side the roles swap */
    score_by[1] = limit + 2;
    assert(run_weigh(f, 0, 0, 40) == 1 && set_args[0][2] == 0);

    /* flags bit 0: 0 when the (1, kind, 14) set is not empty, else on to bit 1 */
    count_of[1] = 0;
    count_of[2] = 1;
    assert(run_weigh(f, 1, 1, 41) == 0);
    assert(set_calls == 2 && set_args[0][3] == 1 && set_args[1][2] == 1 && set_args[1][3] == 1);
    assert(set_args[1][0] == 0 && set_args[1][1] == 0 && set_args[1][4] == 41 && set_args[1][5] == 14);
    count_of[2] = 0;
    assert(run_weigh(f, 1, 1, 41) == 0 && set_calls == 2);

    /* flags >> 1: 1 when the (2, kind, 14) set holds more than 3 */
    count_of[2] = 4;
    assert(run_weigh(f, 1, 2, 42) == 1);
    assert(set_calls == 2 && set_args[1][2] == 1 && set_args[1][3] == 2 && set_args[1][4] == 42 && set_args[1][5] == 14);
    count_of[2] = 3;
    assert(run_weigh(f, 1, 2, 42) == 0);

    /* both: a non-empty first set answers 0 without the third call; an empty one goes on */
    count_of[2] = 1, count_of[3] = 4;
    assert(run_weigh(f, 1, 3, 43) == 0 && set_calls == 2);
    count_of[2] = 0;
    assert(run_weigh(f, 1, 3, 43) == 1 && set_calls == 3 && set_args[2][3] == 2);
    count_of[3] = 3;
    assert(run_weigh(f, 1, 3, 43) == 0 && set_calls == 3);
}

static int run_record(int (*f)(int, unsigned *), int side, unsigned word)
{
    unsigned record[2];

    record[0] = word, record[1] = 0x1d;
    grid_side = side;
    grid_calls = other_calls = pick_calls = join_calls = fill_calls = cell_calls = 0;
    bit_calls = e07_calls = entry_calls = 0;
    return f(side, record);
}

/* TaLogicCom_GetUseXYRangeSpell__00112600 and TaLogicCom_GetUseXYRangeSpell__0016cbb0 (branch and
 * loop coverage). */
static void check_record(int (*f)(int, unsigned *))
{
    int n;

    reset();
    memset(grid_count, 0, sizeof(grid_count));
    memset(bit4_by, 0, sizeof(bit4_by));
    memset(e07_by, 0, sizeof(e07_by));
    for (n = 0; n < 22; n++) {
        other_list[n] = 0x500 | n;
        list[n] = 0x600 | n;
    }

    /* neither 29 nor 30 in the low six bits: -1 with no call */
    assert(run_record(f, 1, 28) == -1 && run_record(f, 1, 31) == -1 && run_record(f, 1, 0x40 | 0) == -1);
    assert(other_calls == 0 && grid_calls == 0 && fill_calls == 0);

    /* 29 (bits above the low six ignored): both lists picked, then the cell is returned */
    pick_answer[0] = 0x903, pick_answer[1] = 0x77;
    cell_by[3] = 0x1234;
    join_answer = 0;
    assert(run_record(f, 0, 0xffffff40 | 29) == 0x1234);
    assert(other_calls == 1 && other_side == 1 && fill_calls == 1 && pick_calls == 2);
    assert(fill_args[0] == 0 && fill_args[1] == 29 && fill_args[2] == 1);
    assert(cell_calls == 1 && cell_first[0] == 9 && cell_first[1] == 3 && join_calls == 1);
    assert(join_args[0] == 0x903 && join_args[1] == 0x77 && join_args[2] == 0x34 && join_args[3] == 0x12);
    assert(grid_calls == 0);
    /* the other side */
    assert(run_record(f, 1, 29) == 0x1234 && other_side == 0 && fill_args[0] == 1);
    /* a negative 0x2781f0 answer: -1 */
    join_answer = -1;
    assert(run_record(f, 1, 29) == -1 && join_calls == 1);
    /* a negative first or second pick: -1 without the cell call; 0 is accepted */
    join_answer = 0;
    pick_answer[0] = -1;
    assert(run_record(f, 1, 29) == -1 && pick_calls == 2 && cell_calls == 0 && join_calls == 0);
    pick_answer[0] = 0, pick_answer[1] = -1;
    assert(run_record(f, 1, 29) == -1 && pick_calls == 2 && cell_calls == 0 && join_calls == 0);
    pick_answer[1] = 0;
    cell_by[0] = 0x21;
    assert(run_record(f, 1, 29) == 0x21 && cell_first[0] == 0 && cell_first[1] == 0);

    /* 30: every set empty: -1 after 49 set calls */
    assert(run_record(f, 1, 30) == -1);
    assert(grid_calls == 49 && entry_calls == 0 && other_calls == 0 && fill_calls == 0);
    /* entries 1 and 4 count by a zero SzDuel_GetUnitStatus(.., 4) answer, entry 2 by a
     * 0x1e0700 answer of 32, entry 3 (answer 31) does not: three entries give
     * 2 (not enough), four give 3 */
    bit4_by[2] = 1, e07_by[2] = 32;
    bit4_by[3] = 1, e07_by[3] = 31;
    for (n = 0; n < 49; n++) {
        grid_count[n] = 3;
    }
    assert(run_record(f, 0, 0x80 | 30) == -1);
    assert(grid_calls == 49 && entry_calls == 147 && bit_calls == 147 && e07_calls == 98);
    grid_count[7 * 2 + 5] = 4;
    grid_count[7 * 2 + 6] = 4;
    grid_count[7 * 3 + 0] = 4;
    assert(run_record(f, 0, 30) == (5 << 8 | 2));
    assert(grid_calls == 20 && entry_calls == 61);
    /* the corners */
    grid_count[0] = 4;
    assert(run_record(f, 1, 30) == 0 && grid_calls == 1);
    memset(grid_count, 0, sizeof(grid_count));
    grid_count[48] = 4;
    assert(run_record(f, 1, 30) == (6 << 8 | 6) && grid_calls == 49);
}

int func_001df540_word(int a, int b)
{
    assert(a == slot_side && b == slot_calls + 1 && b < 11);
    slot_calls++;
    return slot_by[b];
}

static int run_slots(int side, int first, int second)
{
    score_by[3] = first, score_by[5] = second;
    slot_side = side;
    both_mode = 1;
    other_calls = pick_calls = cell_calls = score_calls = slot_calls = 0;
    side = TaLogic006_IsBlackHoleCondition(side);
    both_mode = 0;
    return side;
}

/* TaLogic006_IsBlackHoleCondition (branch and loop coverage). */
static void check_slots(void)
{
    int n;

    reset();
    for (n = 0; n < 22; n++) {
        other_list[n] = 0x500 | n;
        list[n] = 0x600 | n;
    }
    memset(slot_by, 0, sizeof(slot_by));
    pick_answer[0] = 0x903, pick_answer[1] = 0xa05;
    cell_by[3] = 0x1234, cell_by[5] = 0x5678;

    /* second score equal to the first: no slot is read, 1 */
    assert(run_slots(0, 3000, 3000) == 1 && slot_calls == 0);
    assert(other_calls == 2 && other_side == 1 && both_side == 0 && pick_calls == 2 && cell_calls == 4);
    assert(cell_first[0] == 9 && cell_first[1] == 3 && score_calls == 2);
    assert(score_first[0] == 9 && score_first[1] == 3 && score_first[2] == 0x34 && score_first[3] == 0x12);
    assert(score_args[0] == 10 && score_args[1] == 5 && score_args[2] == 0x78 && score_args[3] == 0x56);
    /* second above the first */
    assert(run_slots(1, 3000, 3001) == 1 && slot_calls == 0 && other_side == 0 && both_side == 1);
    /* second below, first 2999: no slot is read */
    assert(run_slots(1, 2999, 0) == 1 && slot_calls == 0);
    /* first 3000: ten slots read; none, two and three non-zero answers */
    assert(run_slots(1, 3000, 2999) == 1 && slot_calls == 10);
    slot_by[1] = 7, slot_by[10] = -1;
    assert(run_slots(0, 3000, 2999) == 1 && slot_calls == 10);
    slot_by[4] = 1;
    assert(run_slots(0, 3000, 2999) == 0 && slot_calls == 10);
    for (n = 1; n < 11; n++) {
        slot_by[n] = 1;
    }
    assert(run_slots(1, 5000, -1) == 0 && slot_calls == 10);
}

int main(void)
{
    check_record(TaLogicCom_GetUseXYRangeSpell__00112600);
    check_record(TaLogicCom_GetUseXYRangeSpell__0016cbb0);
    check_step(TaLogicCom_GetRevoltObj__001110d0, 0);
    check_step(TaLogicCom_GetRevoltObj__0016b3c0, 0);
    check_step(TaLogic019_IsUseRevolt, 0);
    check_step(TaLogic009_GetRevoltObj, 1);
    check_weigh(TaLogicCom_IsUseObjEffect__001123c0, 3000);
    check_weigh(TaLogicCom_IsUseObjEffect__0016c6b0, 2500);
    check_slots();
    return 0;
}
