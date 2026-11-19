/* Synthetic host tests for the lower_rules group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/lower_rules.h"

/* Every callee is a stub: each records its arguments in a call log and answers from a table set by
 * the test. */
enum { PAIR, CELL, WORD, KIND, DFW, BIT, SET, COUNT, ENTRY, GUARD, FILL, IDS, IDKIND, IDWORD, MODE, MARK, ID54,
       SLOT, PLACE, TOTAL, LEVEL, SEND_A, SEND_B, SEND_C, BYTE_OF, FILL_A, FILL_B, PACK, COMPARE, OBJ_KIND, OBJ_LIVE, OBJ_SEND,
       FIND_A, FIND_B, AROUND, PICK, DONE, SETBIT, REST_A, REST_B, NEAR, TRY, FIND_C, REST_C, FIND_D, WORD_D, FIND_E, REST_E, REST_F,
       LIST_PICK, OK_A, OK_B, SLOT_OF, PREP, LIVE, TRY_STEP, PAIR_AT, KEY_OF };

static int log_id[512], log_arg[512][6], log_n;
static int pair_answer, cell_by[16], word_answer, kind_answer, dfw_by[128], bit_answer;
static int set_by[4], set_calls, count_of[256], entry_by[8], guard_answer;
static int ids[8], id_count, id_kind[64], id_word, mode_by[8], id54_answer, total_answer, level_answer;
static int byte_answer, pack_by[2], pack_calls, compare_answer;
static int obj_kind[84], obj_live[84];
static void *mark_block;
static struct DotrRuleOrder *send_order;

static int note(int id, int a, int b, int c, int d, int e, int f)
{
    assert(log_n < 512);
    log_id[log_n] = id;
    log_arg[log_n][0] = a; log_arg[log_n][1] = b; log_arg[log_n][2] = c;
    log_arg[log_n][3] = d; log_arg[log_n][4] = e; log_arg[log_n][5] = f;
    return log_n++;
}

static int was(int at, int id, int a, int b, int c, int d, int e, int f)
{
    return at < log_n && log_id[at] == id && log_arg[at][0] == a && log_arg[at][1] == b && log_arg[at][2] == c
        && log_arg[at][3] == d && log_arg[at][4] == e && log_arg[at][5] == f;
}

static int calls_of(int id)
{
    int i, n = 0;

    for (i = 0; i < log_n; i++) {
        n += log_id[i] == id;
    }
    return n;
}

static void set_cells(int value)
{
    int i;

    for (i = 0; i < 16; i++) {
        cell_by[i] = value;
    }
}

/* col_mode (0 unless a test sets it) makes the 0x250360, 0x1e2100 and 0x24de70 stubs answer per
 * first argument and column. */
static int col_mode, near_answer, pair_for[2][16], bit_for[2][16];
int func_00250360_pair(int a, int b) { note(PAIR, a, b, 0, 0, 0, 0); return col_mode ? pair_for[a & 1][b & 15] : pair_answer; }
int SzDuel_GetUnitLoc(int a, int b) { note(CELL, a, b, 0, 0, 0, 0); return cell_by[b & 15]; }
int SzDuel_GetUnitCardNo(int a, int b) { note(WORD, a, b, 0, 0, 0, 0); return word_answer; }
/* list_mode (0 unless a test sets it): 0x1e0700 answers per column and 0x27c070 copies the next
 * prepared list. */
static int list_mode, kind_for[16], list_calls;
/* kind_seq_n (0 for the other tests): the next answers of 0x1e0700, last first. */
static int kind_seq[2], kind_seq_n;
static unsigned short list_src[4][22];
int SzDuel_GetUnitKind(int a, int b) { note(KIND, a, b, 0, 0, 0, 0); return kind_seq_n ? kind_seq[--kind_seq_n] : list_mode ? kind_for[b & 15] : kind_answer; }
int func_001df540_word(int a, int b) { note(DFW, a, b, 0, 0, 0, 0); return dfw_by[b & 127]; }
int SzDuel_GetUnitStatus(int a, int b, int bit) { note(BIT, a, b, bit, 0, 0, 0); return col_mode ? bit_for[a & 1][b & 15] : bit_answer; }
int SearchUnitByRange(int a, int b, int c, int d, int e, int f)
{
    note(SET, a, b, c, d, e, f);
    return set_by[set_calls++ & 3];
}
int Ta_GetUnitNumFrom22bitFlagList(unsigned value) { note(COUNT, (int)value, 0, 0, 0, 0, 0); return count_of[value & 255]; }
int Ta_GetUnitOwnerIDFromLstCount(int word, int n) { note(ENTRY, word, n, 0, 0, 0, 0); return entry_by[n & 7]; }
unsigned char TaLogic013_IsActiveMode(int a, int b, int c) { note(GUARD, a, b, c, 0, 0, 0); return (unsigned char)guard_answer; }
/* ids_copy: bytes copied to the caller (TaLogic_GetEnchantQue has a five-entry buffer). */
static size_t ids_copy = sizeof ids;
void TaHand_GetNextHand(int index, int *out) { note(IDS, index, 0, 0, 0, 0, 0); memcpy(out, ids, ids_copy); }
int TaHand_GetNextHandNum(int index) { note(FILL, index, 0, 0, 0, 0, 0); return id_count; }
int TaHand_GetCardKind(int id) { note(IDKIND, id, 0, 0, 0, 0, 0); return id_kind[id & 63]; }
int TaHand_GetCardAPDP(int id) { note(IDWORD, id, 0, 0, 0, 0, 0); return id_word; }
int SzDuel_Kind2Type(int value) { note(MODE, value, 0, 0, 0, 0, 0); return mode_by[value & 7]; }
void SzDuel_InitCardInfo(void *block, int id) { note(MARK, id, 0, 0, 0, 0, 0); mark_block = block; }
int TaLogic017_IsExzParts(int id) { note(ID54, id, 0, 0, 0, 0, 0); return id54_answer; }
int SzDuel_GetLP(void) { note(TOTAL, 0, 0, 0, 0, 0, 0); return total_answer; }
/* side_quiet (0x11b0e0/0x120150 tests): the total and level answers are counted but not logged. */
static int side_quiet, side_total, side_calls, level_quiet_calls;
int func_001df470_side(int side) { assert(side_quiet && side == 1); side_calls++; return side_total; }
int SzDuel_GetUnitMonsterCount(int side)
{
    if (side_quiet) {
        assert(side == 1);
        level_quiet_calls++;
        return level_answer;
    }
    note(LEVEL, side, 0, 0, 0, 0, 0);
    return level_answer;
}
void TaLogic_CommandStdLDUnit(struct DotrRuleOrder *order, int a, int b, int c) { note(SEND_A, a, b, c, 0, 0, 0); send_order = order; }
void TaLogic021_CommandBtlUnit(struct DotrRuleOrder *order, int a) { note(SEND_B, a, 0, 0, 0, 0, 0); send_order = order; }
void TaLogic_CommandEncUnit(struct DotrRuleOrder *order, int a, int b, int c) { note(SEND_C, a, b, c, 0, 0, 0); send_order = order; }
int func_0016b0e0_arg(int value) { note(BYTE_OF, value, 0, 0, 0, 0, 0); return byte_answer; }

/* Both fillers require a list of 22 empty halfwords and leave a mark in it. */
static void fill(unsigned short *words, int mark)
{
    int i;

    for (i = 0; i < 22; i++) {
        assert(words[i] == 0xFFFF);
    }
    words[0] = (unsigned short)mark;
    words[21] = (unsigned short)mark;
}
void TaLogic_GetUnitCtlLst2(int a, unsigned short *words, int b, int c, int d) { note(FILL_A, a, b, c, d, 0, 0); fill(words, 0x111); }
void TaLogic_GetUnitCtlLst(int a, unsigned short *words, int b, int c)
{
    note(FILL_B, a, b, c, 0, 0, 0);
    fill(words, 0x222);
    if (list_mode) {
        assert(list_calls < 4);
        memcpy(words, list_src[list_calls++], sizeof list_src[0]);
    }
}
int TaLogic_GetMaxApUnit(unsigned short *words) { note(PACK, words[0], words[21], 0, 0, 0, 0); return pack_by[pack_calls++ & 1]; }
int TaLogic_IsUnitWin(int packed, int b, int c, int d) { note(COMPARE, packed, b, c, d, 0, 0); return compare_answer; }
int SzModel_GetModelType(int index) { note(OBJ_KIND, index, 0, 0, 0, 0, 0); return obj_kind[index]; }
int SzModel_IsModelReady(int index) { note(OBJ_LIVE, index, 0, 0, 0, 0, 0); return obj_live[index]; }
int SzModel_Load(int a, int b, int c, int d) { note(OBJ_SEND, a, b, c, d, 0, 0); return 1; }

static unsigned slot_word, *slot_answer;
static int place_answer;
unsigned *func_00250740_word(int a, int b, int n) { note(SLOT, a, b, n, 0, 0, 0); return slot_answer; }
int TaLogic_GetLandChange(int a, int b, int c, int d, int e, int f) { note(PLACE, a, b, c, d, e, f); return place_answer; }

static void reset(void)
{
    log_n = 0;
    set_calls = 0;
    pack_calls = 0;
    send_order = NULL;
    mark_block = NULL;
}

/* lMain_LoadModel (loop coverage): each outcome of the three tests, and both ends of the index range. */
static void check_objects(void)
{
    int i, at;

    memset(obj_kind, 0, sizeof obj_kind);
    memset(obj_live, 0, sizeof obj_live);
    memset(dfw_by, 0, sizeof dfw_by);
    obj_live[0] = 1; obj_kind[0] = 3;
    obj_live[1] = 1; obj_kind[1] = 4;
    obj_live[2] = 1; obj_kind[2] = 4; dfw_by[2] = 1;
    obj_live[3] = 1; obj_kind[3] = 5;
    obj_live[4] = 1; obj_kind[4] = 6;
    obj_kind[5] = 3;
    obj_live[83] = 7;
    reset();
    lMain_LoadModel();
    assert(calls_of(OBJ_KIND) == 84 && calls_of(OBJ_LIVE) == 84 && calls_of(DFW) == 3 && calls_of(OBJ_SEND) == 4);
    for (i = 0, at = 0; i < log_n; i++) {
        static const int sent[4] = {0, 2, 4, 83};

        if (log_id[i] == OBJ_SEND) {
            assert(was(i, OBJ_SEND, sent[at], -1, -1, 1, 0, 0));
            at++;
        }
        if (log_id[i] == DFW) {
            assert(log_arg[i][0] == -1 && log_arg[i][1] >= 1 && log_arg[i][1] <= 3);
        }
    }
    assert(was(0, OBJ_KIND, 0, 0, 0, 0, 0, 0) && was(1, OBJ_LIVE, 0, 0, 0, 0, 0, 0));
}

/* TaLogic001_IsUseBraveHourGlass (branch coverage): the 1500 and 4 boundaries and a zero sum. */
static int run_levels(int total, int s0, int s1, int level)
{
    reset();
    total_answer = total; set_by[0] = s0; set_by[1] = s1; level_answer = level;
    return TaLogic001_IsUseBraveHourGlass(6);
}

static void check_levels(void)
{
    assert(run_levels(1499, 1, 1, 4) == 0 && calls_of(SET) == 0);
    assert(run_levels(1500, 0, 0, 4) == 0 && calls_of(SET) == 2 && calls_of(LEVEL) == 0);
    assert(was(1, SET, 0, 0, 6, 1, 0x2001C, 14) && was(2, SET, 0, 0, 6, 1, 0x2001C, 14));
    assert(run_levels(1500, 5, -5, 4) == 0 && calls_of(LEVEL) == 0);
    assert(run_levels(1500, 0, 1, 3) == 0 && was(3, LEVEL, 6, 0, 0, 0, 0, 0));
    assert(run_levels(1500, 1, 0, 4) == 1);
}

/* TaLogic006_CommandSpellUnit (branch coverage): the four word answers and the row boundary of the
 * 745 arm. */
static void run_order(struct DotrRuleOrder *order, int word, int cell)
{
    memset(order, 0, sizeof *order);
    order->position = 0x1234;
    order->summonOrder = 0x5555;
    order->missions = 0x6666;
    order->unknown_06 = 0x7777;
    order->flags.word = 0x1111ABCD;
    reset();
    word_answer = word;
    set_cells(cell);
    TaLogic006_CommandSpellUnit(order);
    assert(was(0, PAIR, 0x12, 0x34, 0, 0, 0, 0) && was(1, CELL, 0x12, 0x34, 0, 0, 0, 0)
        && was(2, WORD, 0x12, 0x34, 0, 0, 0, 0));
    assert(order->position == 0x1234 && order->summonOrder == 0x5555 && order->unknown_06 == 0x7777);
}

static void check_order_words(void)
{
    struct DotrRuleOrder order;

    run_order(&order, 745, 0x2FF);
    assert(log_n == 4 && was(3, CELL, 0x12, 0x34, 0, 0, 0, 0));
    assert(order.missions == 0x6FF && order.flags.word == 0x0301ABCD);
    run_order(&order, 745, 0x300);
    assert(order.missions == 0x6FF && order.flags.word == 0x0502ABCD);
    run_order(&order, 690, 0);
    assert(log_n == 3 && order.missions == 0x6FF && order.flags.word == 0x0405ABCD);
    run_order(&order, 685, 0);
    assert(log_n == 3 && order.missions == 0xCFF && order.flags.word == (int)0xFFFFABCD);
    run_order(&order, 744, 0);
    assert(log_n == 3 && order.missions == 0x5FF && order.flags.word == 0x1111ABCD);
    run_order(&order, 0, 0);
    assert(order.missions == 0x5FF && order.flags.word == 0x1111ABCD);
}

/* TaLogic009_IsUseCurseDragonPot (branch coverage): the count boundary, the side comparison and
 * both halves of the 0x1e0700 word. */
static int run_first_entry(int count, int pair, int kind)
{
    reset();
    set_by[0] = 0x51; set_by[1] = 0x52;
    count_of[0x51] = count;
    entry_by[0] = 0x0903;
    pair_answer = pair;
    kind_answer = kind;
    return TaLogic009_IsUseCurseDragonPot(4);
}

/* TaLogic011_IsUseReversalGoddess (branch coverage): the count boundary 2 / 3 and the forwarded arguments. */
static void check_count_three(void)
{
    reset();
    set_by[0] = 0x51; count_of[0x51] = 2;
    assert(TaLogic011_IsUseReversalGoddess(4) == 0 && was(0, SET, 0, 0, 4, 1, 0x3E80025, 14) && was(1, COUNT, 0x51, 0, 0, 0, 0, 0) && log_n == 2);
    reset();
    set_by[0] = 0x51; count_of[0x51] = 3;
    assert(TaLogic011_IsUseReversalGoddess(4) == 1 && log_n == 2);
}

static void check_first_entry(void)
{
    assert(run_first_entry(2, 0, 0) == 1 && log_n == 2);
    assert(was(0, SET, 0, 0, 4, 2, 0, 14) && was(1, COUNT, 0x51, 0, 0, 0, 0, 0));
    assert(run_first_entry(1, 4, 0) == 0 && log_n == 5 && calls_of(KIND) == 0);
    assert(was(2, SET, 0, 0, 4, 3, 33, 14) && was(3, ENTRY, 0x52, 0, 0, 0, 0, 0) && was(4, PAIR, 9, 3, 0, 0, 0, 0));
    assert(run_first_entry(1, 5, 0) == 1 && calls_of(KIND) == 2 && was(5, KIND, 9, 3, 0, 0, 0, 0)
        && was(6, KIND, 9, 3, 0, 0, 0, 0));
    assert(run_first_entry(1, 5, 0x20) == 0 && calls_of(KIND) == 1);
    assert(run_first_entry(1, 5, 0x01) == 0 && calls_of(KIND) == 2);
    assert(run_first_entry(1, 5, 0x1F) == 0);
}

/* TaLogic019_GetUseThunderBolt and TaLogic010_GetUseThunderBolt (branch and loop coverage): both
 * fills (the fill stubs require 22 empty halfwords each time), both sign tests at 0 and -1, the
 * comparison at -1, 0 and 1, and its argument order. */
static int run_packed(int (*f)(int), int side, int first, int second, int compare)
{
    reset();
    pack_by[0] = first; pack_by[1] = second;
    compare_answer = compare;
    set_cells(0x0705);
    return f(side);
}

static void check_packed(int (*f)(int), int swapped, int sign)
{
    assert(run_packed(f, 0, -1, 0x304, sign) == -1 && log_n == 4);
    assert(was(0, FILL_A, 1, 29, 0, 0, 0, 0) && was(1, PACK, 0x111, 0x111, 0, 0, 0, 0));
    assert(was(2, FILL_B, 0, 29, 1, 0, 0, 0) && was(3, PACK, 0x222, 0x222, 0, 0, 0, 0));
    assert(run_packed(f, 3, 0x203, -1, sign) == -1 && log_n == 4 && was(0, FILL_A, 0, 29, 0, 0, 0, 0)
        && was(2, FILL_B, 3, 29, 1, 0, 0, 0));
    assert(run_packed(f, 3, 0x203, 0x304, sign) == 0x203 && log_n == 6 && was(4, CELL, 2, 3, 0, 0, 0, 0));
    if (swapped) {
        assert(was(5, COMPARE, 0x304, 0x203, 5, 7, 0, 0));
    } else {
        assert(was(5, COMPARE, 0x203, 0x304, 5, 7, 0, 0));
    }
    assert(run_packed(f, 3, 0x203, 0x304, 0) == -1 && log_n == 6);
    assert(run_packed(f, 3, 0x203, 0x304, -sign) == -1 && log_n == 6);
    assert(run_packed(f, 3, 0, 0, sign) == 0 && log_n == 6);
}

/* TaLogic012_IsPinch (branch and loop coverage): both count boundaries, no entry, an entry at each
 * end, the row boundary. */
static int run_rows(int first, int second, int row_at, int row)
{
    int i;

    reset();
    set_by[0] = 0x61; set_by[1] = 0x62;
    count_of[0x61] = first; count_of[0x62] = second;
    set_cells(0x300);
    for (i = 0; i < 8; i++) {
        entry_by[i] = 0x0400 + i;
    }
    if (row_at >= 0) {
        cell_by[row_at] = row;
    }
    return TaLogic012_IsPinch(8);
}

static void check_rows(void)
{
    assert(run_rows(3, 5, 0, 0) == 0 && log_n == 2 && was(0, SET, 0, 0, 8, 1, 52, 14));
    assert(run_rows(2, 4, 0, 0) == 0 && log_n == 4 && was(2, SET, 0, 0, 8, 2, 52, 14)
        && was(3, COUNT, 0x62, 0, 0, 0, 0, 0));
    assert(run_rows(2, 5, -1, 0) == 0 && calls_of(CELL) == 5 && calls_of(ENTRY) == 5);
    assert(run_rows(2, 5, 0, 0x2FF) == 1 && calls_of(CELL) == 1);
    assert(was(5, ENTRY, 0x62, 0, 0, 0, 0, 0) && was(6, CELL, 4, 0, 0, 0, 0, 0));
    assert(run_rows(2, 5, 4, 0x2FF) == 1 && calls_of(CELL) == 5);
    assert(run_rows(2, 5, 5, 0x2FF) == 0 && calls_of(CELL) == 5);
    assert(run_rows(0, 6, 5, 0) == 1 && calls_of(CELL) == 6);
}

/* TaLogic013_GetInvadePoint (branch coverage): both boundaries at 3 and both values of the forwarded flag. */
static void check_corner(void)
{
    reset();
    set_cells(0x77);
    assert(TaLogic013_GetInvadePoint(0, 2, 2) == 0x300 && TaLogic013_GetInvadePoint(0, 3, 2) == 0x306 && TaLogic013_GetInvadePoint(9, 3, -1) == 0x306);
    assert(log_n == 0);
    assert(TaLogic013_GetInvadePoint(0, 3, 3) == 0x77 && log_n == 1 && was(0, CELL, 1, 0, 0, 0, 0, 0));
    assert(TaLogic013_GetInvadePoint(5, 0, 4) == 0x77 && log_n == 2 && was(1, CELL, 0, 0, 0, 0, 0, 0));
}

/* TaLogic013_IsUnitInvader (branch coverage): each accepted word and a neighbour of each. */
static void check_words(void)
{
    static const int words[7] = {273, 30, 11, 272, 31, 10, 0};
    int i;

    for (i = 0; i < 7; i++) {
        reset();
        word_answer = words[i];
        assert(TaLogic013_IsUnitInvader(3, 4) == (i < 3) && log_n == 1 && was(0, WORD, 3, 4, 0, 0, 0, 0));
    }
}

/* TaLogic013_IsUseHarpySweaper (branch coverage): each of the three answers both ways. */
static int run_guarded(int guard, int s0, int s1)
{
    reset();
    guard_answer = guard; set_by[0] = s0; set_by[1] = s1;
    return TaLogic013_IsUseHarpySweaper(1, 2, 3);
}

static void check_guarded(void)
{
    assert(run_guarded(0, 0, 1) == 0 && log_n == 1 && was(0, GUARD, 1, 2, 3, 0, 0, 0));
    assert(run_guarded(0x100, 0, 1) == 0 && log_n == 1);
    assert(run_guarded(1, 1, 1) == 0 && log_n == 2 && was(1, SET, 2, 3, 1, 1, 30, 18));
    assert(run_guarded(1, 0, 0) == 0 && log_n == 3 && was(2, SET, 2, 3, 1, 2, 30, 18));
    assert(run_guarded(0x80, 0, 9) == 1 && log_n == 3);
}

/* TaLogic017_IsRefuseEmptyCombo (branch coverage): the kind boundary at 0x20, the 4500 boundary,
 * and the 0x1587e0 answers 0, 1, 2. */
static int run_id(int kind, int word, int answer)
{
    reset();
    id_kind[9] = kind; id_word = word; id54_answer = answer;
    return TaLogic017_IsRefuseEmptyCombo(9);
}

static void check_id(void)
{
    assert(run_id(0x20, 0, 0) == 1 && log_n == 2 && was(0, IDWORD, 9, 0, 0, 0, 0, 0) && was(1, IDKIND, 9, 0, 0, 0, 0, 0));
    assert(run_id(0x1F, (2 << 16) | 4497, 0) == 1 && log_n == 2);
    assert(run_id(0x1F, (2 << 16) | 4496, 0) == 0 && log_n == 3 && was(2, ID54, 9, 0, 0, 0, 0, 0));
    assert(run_id(0, 4500, 1) == 1 && log_n == 3);
    assert(run_id(0, 4501, 0) == 1 && log_n == 2);
    assert(run_id(0, (int)0xFFFF0000 | 4502, 0) == 0);
    assert(run_id(0, 0, 2) == 0);
}

/* TaLogic020_GetTrapCardPlay (loop coverage): empty list, no match, a match at each end, a match
 * past the count. */
static int run_ids(unsigned char *item, int count, int match)
{
    int i;

    reset();
    memset(item, 0xEE, 8);
    memset(id_kind, 0, sizeof id_kind);
    memset(mode_by, 0, sizeof mode_by);
    mode_by[3] = 2;
    for (i = 0; i < 8; i++) {
        ids[i] = 10 + i;
        id_kind[10 + i] = (i == match ? 3 : 1) << 5 | i;
    }
    id_count = count;
    return TaLogic020_GetTrapCardPlay(5, item, &id_count);
}

static void check_ids(void)
{
    unsigned char item[8];

    assert(run_ids(item, 0, 0) == -1 && log_n == 2 && was(0, IDS, 5, 0, 0, 0, 0, 0) && was(1, FILL, 5, 0, 0, 0, 0, 0));
    assert(run_ids(item, 4, -1) == -1 && calls_of(MODE) == 4 && calls_of(MARK) == 0 && item[6] == 0xEE);
    assert(run_ids(item, 4, 4) == -1 && calls_of(MODE) == 4 && item[6] == 0xEE);
    assert(run_ids(item, 4, 0) == 3 && item[6] == 0 && calls_of(MARK) == 1 && mark_block == &id_count);
    assert(was(2, IDKIND, 10, 0, 0, 0, 0, 0) && was(3, MODE, 3, 0, 0, 0, 0, 0) && was(4, MARK, 10, 0, 0, 0, 0, 0)
        && was(5, IDKIND, 10, 0, 0, 0, 0, 0) && log_n == 6);
    assert(run_ids(item, 4, 3) == 3 && item[6] == 3 && calls_of(MODE) == 4 && item[5] == 0xEE && item[7] == 0xEE);
}

/* TaLogic_CommandUnit__00167d30 (branch coverage): each rejection, the column-0 arm both ways, and
 * the three kinds. */
static int run_send(struct DotrRuleOrder *order, int position, int word, int dfw, int bit, int kind)
{
    memset(order, 0, sizeof *order);
    order->position = (unsigned short)position;
    order->missions = 0x6666;
    order->flags.word = word;
    reset();
    memset(dfw_by, 0, sizeof dfw_by);
    dfw_by[position & 127] = dfw;
    bit_answer = bit;
    kind_answer = kind;
    pair_answer = 0x44;
    byte_answer = 0x1A5;
    return TaLogic_CommandUnit__00167d30(order);
}

static void check_send(void)
{
    struct DotrRuleOrder order;

    assert(run_send(&order, 0x0203, 0, 0, 0, 0) == 0 && log_n == 3);
    assert(was(0, CELL, 2, 3, 0, 0, 0, 0) && was(1, PAIR, 2, 3, 0, 0, 0, 0) && was(2, DFW, 2, 3, 0, 0, 0, 0));
    assert(run_send(&order, 0x0203, 0, 1, 1, 0) == 0 && log_n == 4 && was(3, BIT, 2, 3, 0, 0, 0, 0));
    assert(run_send(&order, 0xFFFF, 0, 1, 0, 0) == 0 && log_n == 4 && order.missions == 0x6666);
    assert(run_send(&order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 5 && was(4, SEND_A, 3, 1, 1, 0, 0, 0)
        && send_order == &order);
    assert(run_send(&order, 0x0200, -2, 1, 0, 0) == 1 && log_n == 4);
    assert(run_send(&order, 0x0203, 0, 1, 0, 0x1F) == 1 && log_n == 6 && was(4, KIND, 2, 3, 0, 0, 0, 0)
        && was(5, SEND_B, 1, 0, 0, 0, 0, 0) && send_order == &order);
    assert(run_send(&order, 0x0203, 0, 1, 0, 0x40) == 1 && log_n == 7 && was(5, BYTE_OF, 0x44, 0, 0, 0, 0, 0)
        && was(6, SEND_C, 4, 1, 0xA5, 0, 0, 0) && send_order == &order && order.missions == 0x6666);
    assert(run_send(&order, 0x0203, 0, 1, 0, 0x20) == 1 && log_n == 5 && order.missions == 0x5FF
        && order.flags.word == 0 && send_order == NULL);
    assert(run_send(&order, 0x0203, 0, 1, 0, 0x60) == 1 && order.missions == 0x5FF);
}

/* TaLogic002_CommandMagicUnit (branch coverage): both rejections, entry kinds 1, 2, 3, 12, 13, 14.
 * The arm taken when the masked 0x27c770 halfword is negative cannot be reached and is not
 * host-tested. */
static void run_place(struct DotrRuleOrder *order, int dfw, int has_entry, int kind)
{
    memset(order, 0, sizeof *order);
    order->position = 0x0203;
    order->missions = 0x6666;
    order->flags.word = 0x1111ABCD;
    reset();
    memset(dfw_by, 0, sizeof dfw_by);
    dfw_by[3] = dfw;
    slot_word = (unsigned)kind << 8 | 0x7F;
    slot_answer = has_entry ? &slot_word : NULL;
    set_cells((int)0xFFFF0705);
    place_answer = (int)0xFFFF8123;
    TaLogic002_CommandMagicUnit(order);
    assert(was(0, DFW, 2, 3, 0, 0, 0, 0));
}

static void check_place(void)
{
    static const int kinds[4] = {1, 3, 12, 14};
    struct DotrRuleOrder order;
    int i;

    run_place(&order, 0, 1, 2);
    assert(log_n == 1 && order.missions == 0x6666 && order.flags.word == 0x1111ABCD);
    run_place(&order, 9, 0, 2);
    assert(log_n == 2 && was(1, SLOT, 2, 3, 0, 0, 0, 0) && order.missions == 0x6666);
    for (i = 0; i < 4; i++) {
        run_place(&order, 9, 1, kinds[i]);
        assert(log_n == 2 && order.missions == 0x6666 && order.flags.word == 0x1111ABCD);
    }
    run_place(&order, 9, 1, 13);
    assert(log_n == 2 && order.missions == 0x5FF && order.flags.word == 0x1111ABCD);
    run_place(&order, 9, 1, 2);
    assert(log_n == 5 && was(2, CELL, 2, 3, 0, 0, 0, 0) && was(3, DFW, 2, 3, 0, 0, 0, 0)
        && was(4, PLACE, 9, 5, 7, 0, 6, 2));
    assert(order.missions == 0x6FF && order.flags.word == (int)0x8123ABCD && order.position == 0x0203);
}

static int grid_by[7][7], grid_calls, item_by[7][7], item_calls;
static int item_dummy;
static int item_null_at = -1;

int Ta_GetCurrentSide(int a, int b) { note(NEAR, a, b, 0, 0, 0, 0); return col_mode ? near_answer : 0; }
int SzDuel_GetField(int x, int y)
{
    assert(x >= 0 && x < 7 && y >= 0 && y < 7);
    grid_calls++;
    return grid_by[x][y];
}
struct DotrStateItem *SzDuel_GetMapUnit(int x, int y)
{
    assert(grid_by[x][y] == 1);
    item_calls++;
    if (item_calls == item_null_at) {
        return NULL;
    }
    return item_by[x][y] ? (struct DotrStateItem *)&item_dummy : NULL;
}

/* TaLogic005_CommandTrapBUnit (branch and loop coverage). */
static void run_word(struct DotrRuleOrder *order, int dfw, int word)
{
    memset(order, 0, sizeof *order);
    order->position = 0x0203;
    order->missions = 0x6666;
    order->flags.word = 0x1010ABCD;
    reset();
    memset(dfw_by, 0, sizeof dfw_by);
    dfw_by[3] = dfw;
    word_answer = word;
    set_cells(0x0304);
    grid_calls = item_calls = 0;
    TaLogic005_CommandTrapBUnit(order);
    assert(was(0, DFW, 2, 3, 0, 0, 0, 0));
}

static void check_word(void)
{
    struct DotrRuleOrder order;

    memset(grid_by, 0, sizeof grid_by);
    memset(item_by, 0, sizeof item_by);
    /* a zero 0x1df540 answer: nothing else is called or stored */
    run_word(&order, 0, 829);
    assert(log_n == 1 && order.missions == 0x6666 && order.flags.word == 0x1010ABCD);
    /* 829: the flag word keeps its low halfword only */
    run_word(&order, 9, 829);
    assert(log_n == 4 && was(1, NEAR, 2, 3, 0, 0, 0, 0) && was(2, CELL, 2, 3, 0, 0, 0, 0) && was(3, WORD, 2, 3, 0, 0, 0, 0));
    assert(order.missions == 0x5FF && order.flags.word == 0x0000ABCD && order.position == 0x0203 && grid_calls == 0);
    /* 818: also bit 24 */
    run_word(&order, 9, 818);
    assert(log_n == 4 && order.missions == 0x5FF && order.flags.word == 0x0100ABCD && grid_calls == 0);
    /* neighbouring and other words: only +4 */
    run_word(&order, 9, 819);
    assert(log_n == 4 && order.missions == 0x5FF && order.flags.word == 0x1010ABCD && grid_calls == 0);
    run_word(&order, 9, 0);
    assert(order.missions == 0x5FF && order.flags.word == 0x1010ABCD && grid_calls == 0);
    /* 820: no cell answers 1 (2 does not count) */
    grid_by[3][3] = 2;
    run_word(&order, 9, 820);
    assert(log_n == 4 && grid_calls == 49 && item_calls == 0);
    assert(order.missions == 0x5FF && order.flags.word == 0x1010ABCD);
    /* a cell answering 1 with an item does not count */
    grid_by[0][0] = 1, item_by[0][0] = 1;
    run_word(&order, 9, 820);
    assert(grid_calls == 49 && item_calls == 1 && order.missions == 0x5FF && order.flags.word == 0x1010ABCD);
    /* one without an item leaves the low halfword and bit 24, as 818 does: first, last and the
     * 0x1e1090 cell itself */
    item_by[0][0] = 0;
    run_word(&order, 9, 820);
    assert(grid_calls == 49 && item_calls == 1 && order.missions == 0x5FF && order.flags.word == 0x0100ABCD);
    grid_by[0][0] = 0, grid_by[6][6] = 1;
    run_word(&order, 9, 820);
    assert(item_calls == 1 && order.flags.word == 0x0100ABCD && order.missions == 0x5FF);
    grid_by[6][6] = 0, grid_by[4][3] = 1;
    run_word(&order, 9, 820);
    assert(item_calls == 1 && order.flags.word == 0x0100ABCD);
    grid_by[4][3] = 0, grid_by[3][3] = 0;
}

/* TaLogic021_CommandInvadeUnit (branch coverage). */
static void run_rewrite(struct DotrRuleOrder *order, int kind, int cell)
{
    memset(order, 0, sizeof *order);
    order->position = 0x0203;
    order->missions = kind << 8 | 0x66;
    order->flags.word = 0x1010ABCD;
    reset();
    set_cells(cell);
    cell_by[0] = 0x1234;
    item_calls = 0;
    TaLogic021_CommandInvadeUnit(order);
    assert(was(0, PAIR, 2, 3, 0, 0, 0, 0) && was(1, CELL, 2, 3, 0, 0, 0, 0) && order->position == 0x0203);
}

static void check_rewrite(void)
{
    struct DotrRuleOrder order;

    memset(grid_by, 0, sizeof grid_by);
    memset(item_by, 0, sizeof item_by);
    grid_by[3][3] = grid_by[2][3] = 1;
    pair_answer = 0;
    /* 16: high byte 6 or not */
    run_rewrite(&order, 16, 0x0645);
    assert(log_n == 2 && order.missions == 0xFFF && order.flags.word == 0x0045ABCD);
    run_rewrite(&order, 16, 0x0545);
    assert(log_n == 2 && order.missions == 0x10FF && order.flags.word == 0x0645ABCD);
    /* 15: high byte 0 or not */
    run_rewrite(&order, 15, 0x0045);
    assert(log_n == 2 && order.missions == 0x10FF && order.flags.word == 0x0645ABCD);
    run_rewrite(&order, 15, 0x0145);
    assert(log_n == 2 && order.missions == 0xFFF && order.flags.word == 0x0045ABCD);
    /* other kinds, high byte below 4 */
    run_rewrite(&order, 14, 0x0045);
    assert(log_n == 2 && order.missions == 0x10FF && order.flags.word == 0x0045ABCD);
    run_rewrite(&order, 17, 0x0303);
    assert(log_n == 2 && order.missions == 0x6FF && order.flags.word == 0x0003ABCD);
    run_rewrite(&order, 0, 0x0304);
    assert(log_n == 2 && order.missions == 0x6FF && order.flags.word == 0x0006ABCD);
    run_rewrite(&order, 0, 0x0102);
    assert(log_n == 2 && order.missions == 0x6FF && order.flags.word == 0x0000ABCD && item_calls == 0);
    /* high byte 4: the three 0x1e36e0 questions in order */
    run_rewrite(&order, 0, 0x0403);
    assert(log_n == 2 && item_calls == 1 && order.missions == 0x6FF && order.flags.word == 0x0303ABCD);
    item_by[3][3] = 1;
    run_rewrite(&order, 0, 0x0403);
    assert(log_n == 2 && item_calls == 2 && order.missions == 0x6FF && order.flags.word == 0x0302ABCD);
    item_by[2][3] = 1;
    item_null_at = 3;
    run_rewrite(&order, 0, 0x0403);
    assert(log_n == 2 && item_calls == 3 && order.missions == 0x6FF && order.flags.word == 0x0304ABCD);
    item_null_at = -1;
    /* none null: SzDuel_GetUnitLoc(pair == 0, 0) << 16 joins the low halfword */
    run_rewrite(&order, 0, 0x0403);
    assert(log_n == 3 && item_calls == 3 && was(2, CELL, 1, 0, 0, 0, 0, 0));
    assert(order.missions == 0x6FF && order.flags.word == 0x1234ABCD);
    pair_answer = 7;
    run_rewrite(&order, 0, 0x0403);
    assert(log_n == 3 && was(2, CELL, 0, 0, 0, 0, 0, 0) && order.flags.word == 0x1234ABCD);
    pair_answer = 0;
    memset(grid_by, 0, sizeof grid_by);
    memset(item_by, 0, sizeof item_by);
}

static int gate_b_answer, gate_b_calls, have_by[3], have_ids[3], have_calls;
static int hand_count[2], hand_ids[2][8], hand_reads;

int SzDuel_GetUnitSpellCount(int side) { (void)side; gate_b_calls++; return gate_b_answer; }
int TaLogic_GetGraveCardCount(int index, int flags, int id)
{
    assert(have_calls < 3 && flags == 3 && id == have_ids[have_calls]);
    (void)index;
    return have_by[have_calls++];
}
int SzDuel_GetGraveCount(int index) { assert(index == 0 || index == 1); return hand_count[index]; }
int SzDuel_InqGrave(int index, int position)
{
    assert((index == 0 || index == 1) && position >= 0 && position < hand_count[index]);
    hand_reads++;
    return hand_ids[index][position];
}

static int run_hand(int (*f)(int), int side)
{
    reset();
    gate_b_calls = have_calls = hand_reads = 0;
    return f(side);
}

/* TaLogic019_IsUseRevive and TaLogic019_IsUseSaint (branch and loop coverage). a, b, c are the ids
 * in 0x27dd20 call order; the search order is first, second, third. */
static void check_hand(int (*f)(int), int second_gate, int a, int b, int c, int first, int second, int third)
{
    int *gate = second_gate ? &gate_b_answer : &level_answer;
    int side, other, n;

    have_ids[0] = a, have_ids[1] = b, have_ids[2] = c;
    memset(hand_ids, 0, sizeof hand_ids);
    hand_count[0] = hand_count[1] = 0;

    /* gate at 5: -1 before any other call; 4 goes on */
    level_answer = gate_b_answer = 0;
    *gate = 5;
    have_by[0] = have_by[1] = have_by[2] = 1;
    assert(run_hand(f, 1) == -1 && have_calls == 0);
    assert(gate_b_calls == second_gate && calls_of(LEVEL) == !second_gate);
    *gate = 4;
    assert(run_hand(f, 1) == -1 && have_calls == 1 && hand_reads == 0);
    /* none of the three ids present (0 and negative answers): -1 without a walk */
    have_by[0] = 0, have_by[1] = -1, have_by[2] = 0;
    hand_count[0] = hand_count[1] = 8;
    assert(run_hand(f, 1) == -1 && have_calls == 3 && hand_reads == 0);
    /* each id alone lets the walks run; empty of the ids: -1 after six walks */
    for (n = 0; n < 3; n++) {
        have_by[0] = have_by[1] = have_by[2] = 0;
        have_by[n] = 1;
        assert(run_hand(f, 1) == -1 && have_calls == n + 1 && hand_reads == 48);
    }
    for (side = 0; side < 2; side++) {
        other = !side;
        memset(hand_ids, 0, sizeof hand_ids);
        /* the last-searched id in the other side's last position */
        hand_ids[other][7] = third;
        assert(run_hand(f, side) == (other << 16 | 7) && hand_reads == 48);
        /* on the own side it is found before the other side's */
        hand_ids[side][2] = third;
        assert(run_hand(f, side) == (side << 16 | 2) && hand_reads == 35);
        /* the second id comes first, the other side's after the own side's */
        hand_ids[other][5] = second;
        assert(run_hand(f, side) == (other << 16 | 5) && hand_reads == 30);
        hand_ids[side][7] = second;
        assert(run_hand(f, side) == (side << 16 | 7) && hand_reads == 24);
        /* and the first id before both */
        hand_ids[other][0] = first;
        assert(run_hand(f, side) == (other << 16) && hand_reads == 9);
        hand_ids[side][0] = first;
        assert(run_hand(f, side) == (side << 16) && hand_reads == 1);
        /* the count bounds each walk */
        hand_ids[side][0] = 0, hand_ids[other][0] = 0;
        hand_count[side] = 7, hand_count[other] = 5;
        assert(run_hand(f, side) == (side << 16 | 2) && hand_reads == 7 + 5 + 7 + 5 + 3);
        hand_count[0] = hand_count[1] = 8;
    }
    level_answer = gate_b_answer = 0;
}

static int find_answer, around_answer, pick_answer, rest_answer;
static struct DotrRuleOrder *done_order, *rest_order;
static struct DotrRuleResult *rest_result;

int TaLogic019_IsUseRevolt(int side) { note(FIND_A, side, 0, 0, 0, 0, 0); return find_answer; }
int TaLogicCom_GetRevoltObj__0016b3c0(int side) { note(FIND_B, side, 0, 0, 0, 0, 0); return find_answer; }
int func_00138b60_find(int side) { note(FIND_C, side, 0, 0, 0, 0, 0); return find_answer; }
int TaLogic_GetActionReaction(int a, int b, int c, int d, int e, unsigned char f) { note(AROUND, a, b, c, d, e, f); return around_answer; }
int TaLogic_IsDefStyleUnit(int a, int b, unsigned short c, unsigned char d, int e) { note(PICK, a, b, c, d, e, 0); return pick_answer; }
void func_0027b1b0_order(struct DotrRuleOrder *order, int mode) { note(DONE, mode, 0, 0, 0, 0, 0); done_order = order; }
int SzDuel_SetUnitStatus(int a, int b, int bit, int value) { note(SETBIT, a, b, bit, value, 0, 0); return 0; }
int func_001607d0_rest(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode)
{
    note(REST_A, side, mode, 0, 0, 0, 0);
    rest_order = order, rest_result = result;
    return rest_answer;
}
int func_00170940_rest(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode)
{
    note(REST_B, side, mode, 0, 0, 0, 0);
    rest_order = order, rest_result = result;
    return rest_answer;
}
int func_001374d0_rest(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode)
{
    note(REST_C, side, mode, 0, 0, 0, 0);
    rest_order = order, rest_result = result;
    return rest_answer;
}

/* found_last: the last argument the copy under test passes to TaLogic_GetActionReaction and
 * TaLogic_IsDefStyleUnit (1 for 0x138950, 0 for 0x161ea0 and 0x172000). */
static int found_last;
static struct DotrRuleOrder found_order;
static struct DotrRuleResult found_result;

static int run_found(int (*f)(struct DotrRuleOrder *, int, struct DotrRuleResult *, int), int side, int find,
                     int around, int bit)
{
    memset(&found_order, 0, sizeof found_order);
    memset(&found_result, 0x77, sizeof found_result);
    memset(dfw_by, 0, sizeof dfw_by);
    found_order.position = 0x0305;
    find_answer = find, around_answer = around, bit_answer = bit;
    pick_answer = 0x1c4, rest_answer = 99;
    done_order = rest_order = NULL;
    rest_result = NULL;
    log_n = 0;
    return f(&found_order, side, &found_result, 6);
}

static void check_found(int (*f)(struct DotrRuleOrder *, int, struct DotrRuleResult *, int), int find_id, int rest_id)
{
    /* a high halfword of 3: three cell words asked for the other side, none set, count - 1 = 0xffff */
    assert(run_found(f, 0, 0x30000, 0, 0) == 0);
    assert(was(0, CELL, 3, 5, 0, 0, 0, 0) && was(1, find_id, 0, 0, 0, 0, 0, 0) && was(2, AROUND, 3, 5, 0, 0, 6, found_last));
    assert(calls_of(DFW) == 3 && was(3, DFW, 1, 1, 0, 0, 0, 0) && was(5, DFW, 1, 3, 0, 0, 0, 0));
    assert(was(6, PICK, 3, 5, 0x7777, 1, found_last, 0) && was(7, DONE, 6, 0, 0, 0, 0, 0) && log_n == 8);
    assert(done_order == &found_order && found_result.position == 0x0305 && found_result.kind == 13);
    assert(found_result.atkDefPosition == 0xc4 && found_result.shouldFlipUp == 1 && found_result.loc == 0xffff);
    assert(found_result.unknown_03 == 0x77 && found_result.unknown_06 == 0x7777 && found_result.unknown_08 == 0x77777777);
    /* a high halfword of 1 stops after one; of 10, 0 or more than 10 after ten; set words are counted */
    assert(run_found(f, 1, 0x1ffff, 4, 0) == 0 && calls_of(DFW) == 1 && was(3, DFW, 0, 1, 0, 0, 0, 0));
    assert(run_found(f, 1, 0xa0000, 6, 0) == 0 && calls_of(DFW) == 10);
    assert(run_found(f, 1, 0, 0, 0) == 0 && calls_of(DFW) == 10);
    memset(&found_order, 0, sizeof found_order);
    memset(&found_result, 0x77, sizeof found_result);
    found_order.position = 0x0305;
    dfw_by[1] = 1, dfw_by[2] = 0, dfw_by[3] = 7, dfw_by[10] = 1, dfw_by[11] = 1;
    find_answer = 0x7ffe0000;
    log_n = 0;
    assert(f(&found_order, 1, &found_result, 6) == 0 && calls_of(DFW) == 10 && found_result.loc == 2);
    /* an 0x27d830 answer of 5 or a negative finder answer: the bit test decides */
    assert(run_found(f, 0, 0x30000, 5, 1) == 0 && calls_of(DFW) == 0 && calls_of(PICK) == 0 && calls_of(DONE) == 0);
    assert(was(3, BIT, 3, 5, 4, 0, 0, 0) && was(4, SETBIT, 3, 5, 0, 1, 0, 0) && log_n == 5);
    assert(found_result.position == 0x0305 && found_result.kind == 14 && found_result.loc == 0x7777);
    assert(run_found(f, 2, -1, 0, 0) == 99 && calls_of(DFW) == 0 && calls_of(SETBIT) == 0);
    assert(was(3, BIT, 3, 5, 4, 0, 0, 0) && was(4, rest_id, 2, 6, 0, 0, 0, 0) && log_n == 5);
    assert(rest_order == &found_order && rest_result == &found_result && found_result.position == 0x7777);
}

/* 0x138780: finder 0x138ea0 and word lookup 0x27dbc0 are stubs. */
int TaLogic009_IsUseCopyCat(int side) { note(FIND_D, side, 0, 0, 0, 0, 0); return find_answer; }
int TaLogic_GetGraveEntryAfterScreen(int index, int target, int mode) { note(WORD_D, index, target, mode, 0, 0, 0); return word_answer; }

int TaLogic020_IsUseRevive(int side) { note(FIND_E, side, 0, 0, 0, 0, 0); return find_answer; }
int func_001664a0_ints(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode, int flag)
{
    note(REST_E, side, mode, flag, 0, 0, 0);
    rest_order = order, rest_result = result;
    return rest_answer;
}

/* word_mode is the third 0x27dbc0 argument; rest_flag the fallback's fifth argument (0: it takes four). */
static void check_found_word(int (*f)(struct DotrRuleOrder *, int, struct DotrRuleResult *, int), int find_id,
                             int rest_id, int word_mode, int rest_flag)
{
    /* found: high halfword 0x12 and low byte 0x34 go to 0x27dbc0; kind 12, word (0x12 << 8) | answer */
    word_answer = 0x56;
    assert(run_found(f, 1, 0x12ff34, 0, 0) == 0);
    assert(was(0, CELL, 3, 5, 0, 0, 0, 0) && was(1, find_id, 1, 0, 0, 0, 0, 0) && was(2, AROUND, 3, 5, 0, 0, 6, 1));
    assert(was(3, WORD_D, 0x12, 0x34, word_mode, 0, 0, 0) && was(4, PICK, 3, 5, 0x7777, 1, 1, 0));
    assert(was(5, DONE, 6, 0, 0, 0, 0, 0) && log_n == 6);
    assert(done_order == &found_order && found_result.position == 0x0305 && found_result.kind == 12);
    assert(found_result.atkDefPosition == 0xc4 && found_result.shouldFlipUp == 1 && found_result.loc == 0x1256);
    assert(found_result.unknown_03 == 0x77 && found_result.unknown_06 == 0x7777 && found_result.unknown_08 == 0x77777777);
    /* a zero answer is still found */
    assert(run_found(f, 0, 0, 4, 0) == 0 && was(3, WORD_D, 0, 0, word_mode, 0, 0, 0) && found_result.loc == 0x56);
    /* an 0x27d830 answer of 5: the bit test decides; set bit stores kind 14 */
    assert(run_found(f, 0, 0x30000, 5, 1) == 0 && calls_of(WORD_D) == 0 && calls_of(PICK) == 0);
    assert(calls_of(DONE) == 0 && was(3, BIT, 3, 5, 4, 0, 0, 0) && was(4, SETBIT, 3, 5, 0, 1, 0, 0) && log_n == 5);
    assert(found_result.position == 0x0305 && found_result.kind == 14 && found_result.loc == 0x7777);
    /* a negative finder answer with the bit clear: the fallback's result is returned */
    assert(run_found(f, 2, -1, 0, 0) == 99 && calls_of(WORD_D) == 0 && calls_of(SETBIT) == 0);
    assert(was(3, BIT, 3, 5, 4, 0, 0, 0) && was(4, rest_id, 2, 6, rest_flag, 0, 0, 0) && log_n == 5);
    assert(rest_order == &found_order && rest_result == &found_result && found_result.position == 0x7777);
}

int func_00149690_rest(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode)
{
    note(REST_F, side, mode, 0, 0, 0, 0);
    rest_order = order, rest_result = result;
    return rest_answer;
}

/* TaLogic_GetMinimumTask_ReviveMission__0014aab0 (branch coverage). The finder is the real
 * TaLogic013_IsUseRevive, driven through its own stubs: grave entry 3 of side 1 holds id 11, so its
 * answer is 0x10003 unless the unit count gate (5) makes it -1. Outcomes: found; found but the
 * field is 9; found but 0x27d830 gives 5 (bit set: kind 14); nothing found and the bit clear
 * (fallback). */
static int run_revive(int gate, int field, int around, int bit)
{
    reset();
    gate_b_calls = have_calls = hand_reads = 0;
    memset(&found_order, 0, sizeof found_order);
    memset(&found_result, 0x77, sizeof found_result);
    memset(hand_ids, 0, sizeof hand_ids);
    memset(grid_by, 0, sizeof grid_by);
    found_order.position = 0x0305;
    level_answer = gate;
    have_by[0] = 1, have_by[1] = have_by[2] = 0;
    hand_count[0] = hand_count[1] = 8;
    hand_ids[1][3] = 11;
    cell_by[5] = 0x0204;
    grid_by[4][2] = field;
    grid_calls = 0;
    around_answer = around, bit_answer = bit;
    pick_answer = 0x1c4, rest_answer = 99, word_answer = 0x56;
    done_order = rest_order = NULL;
    rest_result = NULL;
    return TaLogic_GetMinimumTask_ReviveMission__0014aab0(&found_order, 1, &found_result, 6);
}

static void check_revive_field(void)
{
    int at;

    /* found: 1 and 3 go to 0x27dbc0 with mode 1; kind 12, word (1 << 8) | answer */
    assert(run_revive(0, 3, 0, 0) == 0 && grid_calls == 1);
    assert(was(0, CELL, 3, 5, 0, 0, 0, 0) && calls_of(AROUND) == 1 && calls_of(WORD_D) == 1 && calls_of(PICK) == 1);
    at = log_n - 4;
    assert(was(at, AROUND, 3, 5, 0, 0, 6, 0) && was(at + 1, WORD_D, 1, 3, 1, 0, 0, 0));
    assert(was(at + 2, PICK, 3, 5, 0x7777, 1, 0, 0) && was(at + 3, DONE, 6, 0, 0, 0, 0, 0));
    assert(done_order == &found_order && found_result.position == 0x0305 && found_result.kind == 12);
    assert(found_result.atkDefPosition == 0xc4 && found_result.shouldFlipUp == 1 && found_result.loc == 0x156);
    assert(calls_of(BIT) == 0 && calls_of(REST_F) == 0);
    /* field 9 drops the answer; the bit is clear, so the fallback's result is returned */
    assert(run_revive(0, 9, 0, 0) == 99 && grid_calls == 1 && calls_of(WORD_D) == 0 && calls_of(DONE) == 0);
    at = log_n - 2;
    assert(was(at, BIT, 3, 5, 4, 0, 0, 0) && was(at + 1, REST_F, 1, 6, 0, 0, 0, 0) && calls_of(SETBIT) == 0);
    assert(rest_order == &found_order && rest_result == &found_result && found_result.position == 0x7777);
    /* an 0x27d830 answer of 5 drops it too; a set bit is replaced and stores kind 14 */
    assert(run_revive(0, 3, 5, 1) == 0 && calls_of(WORD_D) == 0 && calls_of(REST_F) == 0);
    at = log_n - 2;
    assert(was(at, BIT, 3, 5, 4, 0, 0, 0) && was(at + 1, SETBIT, 3, 5, 0, 1, 0, 0));
    assert(found_result.position == 0x0305 && found_result.kind == 14 && found_result.loc == 0x7777);
    /* the finder's own -1 (unit count gate) with the bit clear: fallback */
    assert(run_revive(5, 3, 0, 0) == 99 && calls_of(WORD_D) == 0 && calls_of(REST_F) == 1 && hand_reads == 0);
    level_answer = 0;
    have_by[0] = 0;
    memset(hand_ids, 0, sizeof hand_ids);
    memset(grid_by, 0, sizeof grid_by);
    cell_by[5] = 0;
}

/* 0x11c430 pair: the three callees are stubs; the placing stub writes the set as the test asks. */
static struct DotrRuleSet step_set;
static int step_prep_calls, step_find_calls[21], step_find_answer, step_place_calls, step_place_answer, step_place_mode;
static int step_record_ok;
/* Set for 0x172370: no null test and the slot bytes are not filled. */
static int step_bare;

int func_00275e80_set(struct DotrRuleSet *set)
{
    assert(set == &step_set);
    step_prep_calls++;
    return 0;
}

static int step_find(int which, struct DotrRuleSet *set, struct DotrRuleStep *step, int side)
{
    static const unsigned char ones[5] = {0xff, 0xff, 0xff, 0xff, 0xff};

    assert(set == &step_set && side == 1 && step_prep_calls == 1);
    step_record_ok = step->position == 0xffff && step->kind == 0xff && step->loc == 0xffff
        && (step_bare || memcmp(step->slots, ones, 5) == 0) && step->shouldFlipUp == 0 && step->atkDefPosition == 0xff;
    step_find_calls[which]++;
    step->position = 0x1234;
    return step_find_answer;
}

int TaLogic_GetMinimumTask__00117e60(struct DotrRuleSet *set, struct DotrRuleStep *step, int side) { return step_find(0, set, step, side); }
int TaLogic_GetMinimumTask__0011c600(struct DotrRuleSet *set, struct DotrRuleStep *step, int side) { return step_find(1, set, step, side); }
int TaLogic_GetMinimumTask__00112b90(struct DotrRuleSet *set, struct DotrRuleStep *step, int side) { return step_find(2, set, step, side); }
int TaLogic_GetMinimumTask__00158c80(struct DotrRuleSet *set, struct DotrRuleStep *step, int side) { return step_find(3, set, step, side); }
int TaLogic_GetMinimumTask__00121800(struct DotrRuleSet *set, struct DotrRuleStep *step, int side) { return step_find(4, set, step, side); }
int TaLogic_GetMinimumTask__00126c80(struct DotrRuleSet *set, struct DotrRuleStep *step, int side) { return step_find(5, set, step, side); }
int TaLogic_GetMinimumTask__001424b0(struct DotrRuleSet *set, struct DotrRuleStep *step, int side) { return step_find(6, set, step, side); }
int TaLogic_GetMinimumTask__00139890(struct DotrRuleSet *set, struct DotrRuleStep *step, int side) { return step_find(7, set, step, side); }
int TaLogic_GetMinimumTask__00163130(struct DotrRuleSet *set, struct DotrRuleStep *step, int side) { return step_find(8, set, step, side); }
int TaLogic_GetMinimumTask__0016d140(struct DotrRuleSet *set, struct DotrRuleStep *step, int side) { return step_find(9, set, step, side); }
int TaLogic_GetMinimumTask__001678b0(struct DotrRuleSet *set, struct DotrRuleStep *step, int side) { return step_find(12, set, step, side); }
int TaLogic_GetMinimumTask__0014fd40(struct DotrRuleSet *set, struct DotrRuleStep *step, int side) { return step_find(13, set, step, side); }
int TaLogic_GetMinimumTask__00131060(struct DotrRuleSet *set, struct DotrRuleStep *step, int side) { return step_find(10, set, step, side); }
int TaLogic_GetMinimumTask__0013dca0(struct DotrRuleSet *set, struct DotrRuleStep *step, int side) { return step_find(11, set, step, side); }
int TaLogic_GetMinimumTask__0010c940(struct DotrRuleSet *set, struct DotrRuleStep *step, int side) { return step_find(14, set, step, side); }
int TaLogic_GetMinimumTask__00135230(struct DotrRuleSet *set, struct DotrRuleStep *step, int side) { return step_find(15, set, step, side); }
int TaLogic_GetMinimumTask__0014b900(struct DotrRuleSet *set, struct DotrRuleStep *step, int side) { return step_find(16, set, step, side); }
int TaLogic_GetMinimumTask__0012bb70(struct DotrRuleSet *set, struct DotrRuleStep *step, int side) { return step_find(17, set, step, side); }
int TaLogic_GetMinimumTask__00146960(struct DotrRuleSet *set, struct DotrRuleStep *step, int side) { return step_find(18, set, step, side); }
int TaLogic_GetMinimumTask__00154220(struct DotrRuleSet *set, struct DotrRuleStep *step, int side) { return step_find(19, set, step, side); }
int TaLogic_GetMinimumTask__0015d350(struct DotrRuleSet *set, struct DotrRuleStep *step, int side) { return step_find(20, set, step, side); }

static int step_find_total(void)
{
    return step_find_calls[0] + step_find_calls[1] + step_find_calls[2] + step_find_calls[3]
        + step_find_calls[4] + step_find_calls[5] + step_find_calls[6]
        + step_find_calls[7] + step_find_calls[8] + step_find_calls[9]
        + step_find_calls[10] + step_find_calls[11] + step_find_calls[12] + step_find_calls[13]
        + step_find_calls[14] + step_find_calls[15] + step_find_calls[16] + step_find_calls[17]
        + step_find_calls[18] + step_find_calls[19] + step_find_calls[20];
}

int TaLogic_GetInputQueFromMinimumTask(struct DotrRuleSet *set, struct DotrRuleStep *step, int side)
{
    assert(set == &step_set && side == 1 && step->position == 0x1234);
    step_place_calls++;
    set->mode = step_place_mode;
    return step_place_answer;
}

static int run_step(int (*f)(int, struct DotrRuleSet *), int count, int mode)
{
    int i;

    for (i = 0; i < 32; i++) {
        step_set.ids[i] = (unsigned short)(100 + i);
    }
    step_set.inputCount = (unsigned short)count;
    step_set.mode = mode;
    step_prep_calls = step_place_calls = 0;
    memset(step_find_calls, 0, sizeof(step_find_calls));
    step_record_ok = 0;
    return f(1, &step_set);
}

static void check_step(int (*f)(int, struct DotrRuleSet *), int which)
{
    int i;

    /* a null set: 0x800 before anything */
    if (!step_bare) {
        assert(f(1, 0) == 0x800);
    }

    /* the counter after the increment: 600, 601, 602 -> 0x20; 603 and above -> 0x800; nothing else happens */
    for (i = 599; i <= 601; i++) {
        assert(run_step(f, i, 3) == 0x20 && step_set.inputCount == i + 1 && step_set.mode == 3);
    }
    assert(run_step(f, 602, 3) == 0x800 && step_set.inputCount == 603);
    assert(run_step(f, 0xfffe, 3) == 0x800 && step_set.inputCount == 0xffff);
    assert(step_prep_calls == 0);

    /* a nonzero mode, counter 599 after the increment: no callee; ids[mode - 1] returned and the
     * mode stepped */
    assert(run_step(f, 598, 3) == 102 && step_set.inputCount == 599 && step_set.mode == 4);
    assert(step_prep_calls == 0 && step_find_total() == 0 && step_place_calls == 0);
    assert(step_set.ids[2] == 102 && step_set.ids[3] == 103);
    /* the counter wraps to 0 */
    assert(run_step(f, 0xffff, 3) == 102 && step_set.inputCount == 0);

    /* ids[mode - 1] == 0xffff: 0, mode unchanged */
    step_set.mode = 0;
    run_step(f, 0, 3);
    step_set.ids[2] = 0xffff; step_set.mode = 3; step_set.inputCount = 0;
    assert(f(1, &step_set) == 0 && step_set.mode == 3 && step_set.ids[3] == 103);

    /* ids[mode] == 0xffff: the id is returned, the mode cleared and every id set to 0xffff */
    run_step(f, 0, 3);
    step_set.ids[3] = 0xffff; step_set.mode = 3; step_set.inputCount = 0;
    assert(f(1, &step_set) == 102 && step_set.mode == 0);
    for (i = 0; i < 32; i++) {
        assert(step_set.ids[i] == 0xffff);
    }
    assert(step_set.inputCount == 1);

    /* mode 31: the last id pair; the 5-bit mode wraps to 0 */
    assert(run_step(f, 0, 31) == 130 && step_set.mode == 0 && step_set.ids[31] == 131);

    /* mode 0: prepare, then the finder; a finder answer of 0 -> 0x800 without placing */
    step_find_answer = 0;
    assert(run_step(f, 0, 0) == 0x800 && step_prep_calls == 1 && step_find_calls[which] == 1
           && step_find_total() == 1 && step_record_ok && step_place_calls == 0);
    /* placing answers below zero -> 0 */
    step_find_answer = 1; step_place_answer = -1; step_place_mode = 2;
    assert(run_step(f, 0, 0) == 0 && step_place_calls == 1 && step_set.mode == 2);
    /* placing answers zero: the walk continues from the mode the placing stub left */
    step_place_answer = 0;
    assert(run_step(f, 0, 0) == 101 && step_place_calls == 1 && step_set.mode == 3);
}

/* 0x117c30 pair (branch coverage): the 1200 boundary of the total and the 4 or 5 boundary of the level. */
static void check_total_level(int (*f)(int), int low, int level)
{
    reset();
    total_answer = 1200; level_answer = 0;
    assert(f(6) == low && calls_of(TOTAL) == 1 && calls_of(LEVEL) == 0);
    reset();
    total_answer = 1201; level_answer = level - 1;
    assert(f(6) == 1500 && was(1, LEVEL, 6, 0, 0, 0, 0, 0));
    reset();
    total_answer = 1201; level_answer = level;
    assert(f(6) == 750 && calls_of(LEVEL) == 1);
}

/* TaLogic_CommandEfcMonster__001229f0 (branch and loop coverage). */
void func_00122270_order(struct DotrRuleOrder *order, int flag) { note(TRY, flag, 0, 0, 0, 0, 0); send_order = order; }

static void run_columns(struct DotrRuleOrder *order, int dfw, int word, int side)
{
    memset(order, 0, sizeof *order);
    order->position = 0x0203;
    order->missions = 0x6666;
    order->flags.word = 0x1010ABCD;
    reset();
    memset(dfw_by, 0, sizeof dfw_by);
    dfw_by[3] = dfw;
    word_answer = word;
    near_answer = side;
    set_cells(0x0304);
    col_mode = 1;
    TaLogic_CommandEfcMonster__001229f0(order);
    col_mode = 0;
    assert(was(0, DFW, 2, 3, 0, 0, 0, 0));
}

static void check_columns(void)
{
    struct DotrRuleOrder order;
    int i;

    memset(pair_for, 0, sizeof pair_for);
    memset(bit_for, 0, sizeof bit_for);
    /* a zero 0x1df540 answer: nothing else is called or stored */
    run_columns(&order, 0, 565, 0);
    assert(log_n == 1 && send_order == NULL && order.missions == 0x6666 && order.flags.word == 0x1010ABCD);
    /* another word (and its neighbours): no scan, the closing call only */
    run_columns(&order, 9, 564, 0);
    assert(log_n == 5 && was(1, NEAR, 2, 3, 0, 0, 0, 0) && was(2, CELL, 2, 3, 0, 0, 0, 0) && was(3, WORD, 2, 3, 0, 0, 0, 0));
    assert(was(4, TRY, 0, 0, 0, 0, 0, 0) && send_order == &order && order.missions == 0x6666 && order.flags.word == 0x1010ABCD);
    run_columns(&order, 9, 566, 0);
    assert(log_n == 5 && calls_of(BIT) == 0 && order.missions == 0x6666);
    /* 565, every 0x1e2100 answer 0: columns 6..10 asked for both sides, no 0x250360 call, nothing stored */
    run_columns(&order, 9, 565, 0);
    assert(log_n == 15 && calls_of(BIT) == 10 && calls_of(PAIR) == 0 && was(14, TRY, 0, 0, 0, 0, 0, 0));
    for (i = 0; i < 5; i++) {
        assert(was(4 + 2 * i, BIT, 0, 6 + i, 4, 0, 0, 0) && was(5 + 2 * i, BIT, 1, 6 + i, 4, 0, 0, 0));
    }
    assert(order.missions == 0x6666 && order.flags.word == 0x1010ABCD && send_order == &order);
    /* side 1: the second question of each column goes to side 0 */
    run_columns(&order, 9, 565, 1);
    assert(calls_of(BIT) == 10 && was(4, BIT, 1, 6, 4, 0, 0, 0) && was(5, BIT, 0, 6, 4, 0, 0, 0) && was(13, BIT, 0, 10, 4, 0, 0, 0));
    /* a set bit whose 0x250360 answer is not the side does not count (first and second question) */
    bit_for[1][6] = 1; pair_for[1][6] = 0;
    bit_for[0][10] = 1; pair_for[0][10] = 0;
    run_columns(&order, 9, 565, 1);
    assert(calls_of(PAIR) == 2 && was(5, PAIR, 1, 6, 0, 0, 0, 0) && was(15, PAIR, 0, 10, 0, 0, 0, 0));
    assert(order.missions == 0x6666 && order.flags.word == 0x1010ABCD && send_order == &order);
    /* one counted in the first question at the first column */
    pair_for[1][6] = 1;
    run_columns(&order, 9, 565, 1);
    assert(order.missions == 0x3FF && order.flags.word == 0x0304ABCD && order.position == 0x0203);
    assert(was(log_n - 1, TRY, 0, 0, 0, 0, 0, 0) && send_order == &order);
    /* one counted in the second question at the last column */
    pair_for[1][6] = 0; pair_for[0][10] = 1;
    run_columns(&order, 9, 565, 1);
    assert(order.missions == 0x3FF && order.flags.word == 0x0304ABCD);
    /* columns 5 and 11 are not asked */
    memset(bit_for, 0, sizeof bit_for);
    bit_for[1][5] = bit_for[0][5] = bit_for[1][11] = bit_for[0][11] = 1;
    pair_for[1][5] = pair_for[0][5] = pair_for[1][11] = pair_for[0][11] = 1;
    run_columns(&order, 9, 565, 1);
    assert(calls_of(PAIR) == 0 && order.missions == 0x6666 && order.flags.word == 0x1010ABCD);
    /* all ten counted */
    for (i = 6; i < 11; i++) {
        bit_for[0][i] = bit_for[1][i] = 1;
        pair_for[0][i] = pair_for[1][i] = 0;
    }
    run_columns(&order, 9, 565, 0);
    assert(calls_of(PAIR) == 10 && order.missions == 0x3FF && order.flags.word == 0x0304ABCD);
}

/* 0x118510 and its copy TaLogic_GetNextActiveUnit__0011cd30 (branch and loop coverage). Stand-ins:
 * 0x27c670 counts the leading halfwords that are not 0xffff; the two acceptance callees and
 * 0x1e2100 answer per low nibble of the packed position (0 for the side's own position, side << 8).
 * */
static struct DotrRuleSet list_set;
static struct DotrRuleStep list_step;
static struct DotrRuleEntry *prep_entry;
static int pick_list_answer, oka_for[16], okb_for[16], list_slot_answer, live_answer, try_answer;

int TaLogic_GetAtkUnitNow(int side, unsigned short *words, struct DotrRuleSet *set)
{
    assert(set == &list_set);
    note(LIST_PICK, side, words[0], 0, 0, 0, 0);
    return pick_list_answer;
}
int TaLogic_GetUnitCtlLstLen(unsigned short *words)
{
    int n = 0;

    while (n < 22 && words[n] != 0xFFFF) {
        n++;
    }
    return n;
}
int TaLogic_IsUnitReadyMove(unsigned short packed) { note(OK_A, packed, 0, 0, 0, 0, 0); return oka_for[packed & 15]; }
int TaLogic_IsPickUp(int packed) { note(OK_B, packed, 0, 0, 0, 0, 0); return okb_for[packed & 15]; }
int func_00276c80_set(struct DotrRuleSet *set, int value)
{
    assert(set == &list_set);
    note(SLOT_OF, value, 0, 0, 0, 0, 0);
    return list_slot_answer;
}
void func_00118330_entry(struct DotrRuleEntry *entry) { note(PREP, 0, 0, 0, 0, 0, 0); prep_entry = entry; }
void func_0011cb20_entry(struct DotrRuleEntry *entry) { note(PREP, 1, 0, 0, 0, 0, 0); prep_entry = entry; }
void func_00121ce0_entry(struct DotrRuleEntry *entry) { note(PREP, 2, 0, 0, 0, 0, 0); prep_entry = entry; }
void func_001271f0_entry(struct DotrRuleEntry *entry) { note(PREP, 3, 0, 0, 0, 0, 0); prep_entry = entry; }
void func_00131540_entry(struct DotrRuleEntry *entry) { note(PREP, 4, 0, 0, 0, 0, 0); prep_entry = entry; }
int SzDuel_InqHandPhase(int index) { note(LIVE, index, 0, 0, 0, 0, 0); return live_answer; }
int TaLogic_GetMinimumTask_Summon__00119300(int position, int side, struct DotrRuleStep *step)
{
    assert(step == &list_step);
    note(TRY_STEP, 0, position, side, 0, 0, 0);
    return try_answer;
}
int TaLogic_GetMinimumTask_Summon__0011dfb0(int position, int side, struct DotrRuleStep *step)
{
    assert(step == &list_step);
    note(TRY_STEP, 1, position, side, 0, 0, 0);
    return try_answer;
}
int TaLogic_GetMinimumTask_Summon__00122d30(int position, int side, struct DotrRuleStep *step)
{
    assert(step == &list_step);
    note(TRY_STEP, 2, position, side, 0, 0, 0);
    return try_answer;
}
int func_00127e30_step(int position, int side, struct DotrRuleStep *step)
{
    assert(step == &list_step);
    note(TRY_STEP, 3, position, side, 0, 0, 0);
    return try_answer;
}
int func_00132460_step(int position, int side, struct DotrRuleStep *step)
{
    assert(step == &list_step);
    note(TRY_STEP, 4, position, side, 0, 0, 0);
    return try_answer;
}
void func_00135790_entry(struct DotrRuleEntry *entry) { note(PREP, 5, 0, 0, 0, 0, 0); prep_entry = entry; }
int TaLogic_GetMinimumTask_Summon__00136210(int position, int side, struct DotrRuleStep *step)
{
    assert(step == &list_step);
    note(TRY_STEP, 5, position, side, 0, 0, 0);
    return try_answer;
}
void func_0013e1d0_entry(struct DotrRuleEntry *entry) { note(PREP, 6, 0, 0, 0, 0, 0); prep_entry = entry; }
int TaLogic_GetMinimumTask_Summon__0013eee0(int position, int side, struct DotrRuleStep *step)
{
    assert(step == &list_step);
    note(TRY_STEP, 6, position, side, 0, 0, 0);
    return try_answer;
}
void func_001429b0_entry(struct DotrRuleEntry *entry) { note(PREP, 7, 0, 0, 0, 0, 0); prep_entry = entry; }
int TaLogic_GetMinimumTask_Summon__00143ba0(int position, int side, struct DotrRuleStep *step)
{
    assert(step == &list_step);
    note(TRY_STEP, 7, position, side, 0, 0, 0);
    return try_answer;
}
void func_0014bde0_entry(struct DotrRuleEntry *entry) { note(PREP, 8, 0, 0, 0, 0, 0); prep_entry = entry; }
int func_0014cba0_step(int position, int side, struct DotrRuleStep *step)
{
    assert(step == &list_step);
    note(TRY_STEP, 8, position, side, 0, 0, 0);
    return try_answer;
}
void func_00150270_entry(struct DotrRuleEntry *entry) { note(PREP, 9, 0, 0, 0, 0, 0); prep_entry = entry; }
int TaLogic_GetMinimumTask_Summon__00150c40(int position, int side, struct DotrRuleStep *step)
{
    assert(step == &list_step);
    note(TRY_STEP, 9, position, side, 0, 0, 0);
    return try_answer;
}
void func_001591b0_entry(struct DotrRuleEntry *entry) { note(PREP, 10, 0, 0, 0, 0, 0); prep_entry = entry; }
int func_0015a360_step(int position, int side, struct DotrRuleStep *step)
{
    assert(step == &list_step);
    note(TRY_STEP, 10, position, side, 0, 0, 0);
    return try_answer;
}
void func_0015d8e0_entry(struct DotrRuleEntry *entry) { note(PREP, 11, 0, 0, 0, 0, 0); prep_entry = entry; }
int TaLogic_GetMinimumTask_Summon__0015f300(int position, int side, struct DotrRuleStep *step)
{
    assert(step == &list_step);
    note(TRY_STEP, 11, position, side, 0, 0, 0);
    return try_answer;
}
void func_00163660_entry(struct DotrRuleEntry *entry) { note(PREP, 12, 0, 0, 0, 0, 0); prep_entry = entry; }
int func_001643f0_step(int position, int side, struct DotrRuleStep *step)
{
    assert(step == &list_step);
    note(TRY_STEP, 12, position, side, 0, 0, 0);
    return try_answer;
}
void func_0016d6a0_entry(struct DotrRuleEntry *entry) { note(PREP, 13, 0, 0, 0, 0, 0); prep_entry = entry; }
int func_0016f4d0_step(int position, int side, struct DotrRuleStep *step)
{
    assert(step == &list_step);
    note(TRY_STEP, 13, position, side, 0, 0, 0);
    return try_answer;
}

static int prep_kind, limit_calls, kind_twice;
void func_0012c1a0_entry(struct DotrRuleEntry *entry, int kind) { note(PREP, 19, 0, 0, 0, 0, 0); prep_entry = entry; prep_kind = kind; }
int TaLogic007_IsActiveMode(void) { limit_calls++; return 0x4321; }
int TaLogic_GetMinimumTask_Summon__0012d270(int position, int side, struct DotrRuleStep *step, int limit)
{
    assert(step == &list_step && limit == 0x4321);
    note(TRY_STEP, 19, position, side, 0, 0, 0);
    return try_answer;
}
void func_0010ce20_entry(struct DotrRuleSet *set, struct DotrRuleEntry *entry)
{
    assert(set == &list_set);
    note(PREP, 20, 0, 0, 0, 0, 0);
    prep_entry = entry;
}
int TaLogic_GetMinimumTask_Summon__0010dd50(int position, int side, struct DotrRuleStep *step)
{
    assert(step == &list_step);
    note(TRY_STEP, 20, position, side, 0, 0, 0);
    return try_answer;
}

typedef int (*ListFn)(struct DotrRuleSet *, int, struct DotrRuleStep *);

static void list_defaults(void)
{
    int i;

    memset(list_src, 0xFF, sizeof list_src);
    memset(&list_set, 0, sizeof list_set);
    memset(bit_for, 0, sizeof bit_for);
    for (i = 0; i < 16; i++) {
        kind_for[i] = 0, oka_for[i] = 0, okb_for[i] = 0;
    }
    pick_list_answer = -1, list_slot_answer = 7, live_answer = 0, try_answer = -1;
    /* by default the middle stage takes the 0x1e2100 arm and declines: bit set, not live */
    bit_for[1][0] = bit_for[0][0] = 1;
}

static int run_list(ListFn f, int side)
{
    int r;

    reset();
    list_calls = 0;
    prep_entry = NULL;
    list_mode = col_mode = 1;
    r = f(&list_set, side, &list_step);
    list_mode = col_mode = 0;
    return r;
}

static void check_lists(ListFn f, int id)
{
    /* the first list's pick is accepted (0 counts): one fill, nothing else */
    list_defaults();
    list_src[0][0] = 0x0109;
    pick_list_answer = 0;
    assert(run_list(f, 1) == 0 && log_n == 2 && was(0, FILL_B, 1, 29, 1, 0, 0, 0) && was(1, LIST_PICK, 1, 0x0109, 0, 0, 0, 0));
    pick_list_answer = 0x0456;
    assert(run_list(f, 1) == 0x0456 && log_n == 2);
    /* everything declines: four fills (29, 30, 31, 31), the side's slot prepared, -1 */
    list_defaults();
    assert(run_list(f, 1) == -1 && list_calls == 4 && calls_of(FILL_B) == 4);
    assert(was(2, FILL_B, 1, 30, 1, 0, 0, 0) && was(3, SLOT_OF, 0x100, 0, 0, 0, 0, 0) && was(4, PREP, id, 0, 0, 0, 0, 0));
    assert(prep_entry == &list_set.entries[7] && was(5, BIT, 1, 0, 0, 0, 0, 0) && was(6, LIVE, 1, 0, 0, 0, 0, 0));
    assert(was(7, FILL_B, 1, 31, 1, 0, 0, 0) && was(8, FILL_B, 1, 31, 1, 0, 0, 0) && log_n == 9);
    assert(calls_of(KIND) == 0 && calls_of(OK_A) == 0 && calls_of(OK_B) == 0 && calls_of(TRY_STEP) == 0);
    /* second list: an entry needs 0x1e0700 >> 5 == 3 and both acceptances; each refusal in turn,
     * then the fourth entry */
    list_src[1][0] = 0x0103, list_src[1][1] = 0x0104, list_src[1][2] = 0x0105, list_src[1][3] = 0x0106;
    kind_for[3] = 2 << 5;
    kind_for[4] = 3 << 5 | 31, oka_for[4] = 0, okb_for[4] = 1;
    kind_for[5] = 3 << 5, oka_for[5] = 1, okb_for[5] = 2;
    kind_for[6] = 3 << 5, oka_for[6] = 1, okb_for[6] = 1;
    /* kind_twice (0x10d080): an answer other than 3 is asked for again, and 4 counts as well */
    assert(run_list(f, 1) == 0x0106 && list_calls == 2 && calls_of(KIND) == 4 + kind_twice && calls_of(OK_A) == 3 && calls_of(OK_B) == 2);
    assert(was(3, KIND, 1, 3, 0, 0, 0, 0) && was(5 + kind_twice, OK_A, 0x0104, 0, 0, 0, 0, 0) && was(8 + kind_twice, OK_B, 0x0105, 0, 0, 0, 0, 0));
    assert(calls_of(SLOT_OF) == 0);
    /* 0x1e0700 >> 5 of 4 does not count; the first entry can win */
    kind_for[6] = 4 << 5;
    if (kind_twice) {
        assert(run_list(f, 1) == 0x0106 && calls_of(KIND) == 6 && calls_of(OK_A) == 3);
        kind_for[6] = 5 << 5;
        assert(run_list(f, 1) == -1 && calls_of(KIND) == 6 && calls_of(OK_A) == 2);
    } else {
        assert(run_list(f, 1) == -1 && calls_of(KIND) == 4 && calls_of(OK_A) == 2);
    }
    kind_for[3] = 3 << 5, oka_for[3] = okb_for[3] = 1;
    assert(run_list(f, 1) == 0x0103 && calls_of(KIND) == 1);
    /* middle stage, entry kind 10 (high byte of +4): live and the try accepted */
    list_defaults();
    list_set.entries[7].kinds = 0x0A33;
    live_answer = 1, try_answer = 0;
    assert(run_list(f, 1) == 0x10100 && was(5, LIVE, 1, 0, 0, 0, 0, 0) && was(6, TRY_STEP, id, 0x100, 1, 0, 0, 0) && log_n == 7);
    assert(calls_of(BIT) == 0);
    assert(run_list(f, 0) == 0x10000 && was(3, SLOT_OF, 0, 0, 0, 0, 0, 0) && was(6, TRY_STEP, id, 0, 0, 0, 0, 0));
    /* try refused: the first acceptance of the position decides */
    try_answer = -1, oka_for[0] = 1;
    assert(run_list(f, 1) == 0x100 && was(7, OK_A, 0x100, 0, 0, 0, 0, 0) && log_n == 8);
    oka_for[0] = 2;
    assert(run_list(f, 1) == -1 && list_calls == 4);
    /* not live: no try */
    live_answer = 2, try_answer = 0, oka_for[0] = 1;
    assert(run_list(f, 1) == 0x100 && calls_of(TRY_STEP) == 0 && was(6, OK_A, 0x100, 0, 0, 0, 0, 0) && log_n == 7);
    /* another slot and a neighbouring kind (11) take the 0x1e2100 arm */
    list_defaults();
    list_slot_answer = 21;
    list_set.entries[21].kinds = 0x0B00;
    bit_for[1][0] = 0, oka_for[0] = 1;
    assert(run_list(f, 1) == 0x100 && prep_entry == &list_set.entries[21] && was(5, BIT, 1, 0, 0, 0, 0, 0));
    assert(was(6, OK_A, 0x100, 0, 0, 0, 0, 0) && log_n == 7 && calls_of(LIVE) == 0);
    oka_for[0] = 0;
    assert(run_list(f, 1) == -1 && calls_of(LIVE) == 0 && list_calls == 4);
    /* bit set: live and try accepted, live and try refused (the acceptance is not asked) */
    bit_for[1][0] = 1, live_answer = 1, try_answer = 5, oka_for[0] = 1;
    assert(run_list(f, 1) == 0x10100 && was(7, TRY_STEP, id, 0x100, 1, 0, 0, 0) && log_n == 8);
    try_answer = -1;
    assert(run_list(f, 1) == -1 && calls_of(TRY_STEP) == 1 && calls_of(OK_A) == 0 && list_calls == 4);
    /* third list: both acceptances; refusals of each, then the third entry */
    list_defaults();
    list_src[2][0] = 0x0103, list_src[2][1] = 0x0104, list_src[2][2] = 0x0105;
    oka_for[3] = 0, okb_for[3] = 1;
    oka_for[4] = 1, okb_for[4] = 0;
    oka_for[5] = 1, okb_for[5] = 1;
    assert(run_list(f, 1) == 0x0105 && list_calls == 3 && calls_of(OK_A) == 3 && calls_of(OK_B) == 2 && calls_of(KIND) == 0);
    okb_for[5] = 0;
    assert(run_list(f, 1) == -1 && list_calls == 4 && calls_of(OK_A) == 3);
    /* fourth list: a clear 0x1e2100 bit and the second acceptance */
    list_defaults();
    list_src[3][0] = 0x0103, list_src[3][1] = 0x0104, list_src[3][2] = 0x0105;
    bit_for[1][3] = 1, okb_for[3] = 1;
    bit_for[1][4] = 0, okb_for[4] = 0;
    bit_for[1][5] = 0, okb_for[5] = 1;
    assert(run_list(f, 1) == 0x0105 && list_calls == 4 && calls_of(OK_B) == 2 && calls_of(OK_A) == 0);
    assert(was(log_n - 1, OK_B, 0x0105, 0, 0, 0, 0, 0) && was(log_n - 2, BIT, 1, 5, 0, 0, 0, 0));
    okb_for[5] = 2;
    assert(run_list(f, 1) == -1 && calls_of(OK_B) == 2);
    /* a full list of 22 is walked to its end */
    {
        int i;

        for (i = 0; i < 22; i++) {
            list_src[3][i] = 0x0104;
        }
        assert(run_list(f, 1) == -1 && calls_of(OK_B) == 22);
    }
}

/* TaLogic_GetNextActiveUnit__0012c3f0 and TaLogic_GetNextActiveUnit__0010d080 (branch and loop
 * coverage): the check_lists cases; 0x12c3f0 also has the low four bits of the set's byte +0x1a1
 * reach its prepare stub and one 0x130cb0 answer reach each try; 0x10d080 runs with kind_twice.
 * Every callee is a stub. */
static void check_lists_more(void)
{
    limit_calls = 0;
    check_lists(TaLogic_GetNextActiveUnit__0012c3f0, 19);
    assert(limit_calls > 0);
    list_defaults();
    list_set.aiActiveRelated = 0xA7;
    limit_calls = 0;
    assert(run_list(TaLogic_GetNextActiveUnit__0012c3f0, 1) == -1 && prep_kind == 7 && limit_calls == 0);
    list_set.entries[7].kinds = 0x0A00;
    live_answer = 1, try_answer = 0;
    assert(run_list(TaLogic_GetNextActiveUnit__0012c3f0, 1) == 0x10100 && limit_calls == 1 && prep_kind == 7);
    kind_twice = 1;
    check_lists(TaLogic_GetNextActiveUnit__0010d080, 20);
    kind_twice = 0;
}

/* TaLogic_GetNextActiveUnit__00113370, TaLogic_GetNextActiveUnit__0013a010 and
 * TaLogic_GetNextActiveUnit__00147100 (branch and loop coverage): the check_lists cases through
 * three-argument wrappers that pass the key 0x5A17; each prepare stub checks that key. Every callee
 * is a stub, including the `_step` views of 0x114080, 0x13ab70 and 0x148020. */
#define KEYED_LIST(fn, prep, try_, id) \
    void prep(struct DotrRuleEntry *entry, int key) { assert(key == 0x5A17); note(PREP, id, 0, 0, 0, 0, 0); prep_entry = entry; } \
    int try_(int position, int side, struct DotrRuleStep *step) \
    { \
        assert(step == &list_step); \
        note(TRY_STEP, id, position, side, 0, 0, 0); \
        return try_answer; \
    } \
    static int keyed_##fn(struct DotrRuleSet *set, int side, struct DotrRuleStep *step) { return fn(set, side, 0x5A17, step); }
KEYED_LIST(TaLogic_GetNextActiveUnit__00113370, func_00113140_entry, func_00114080_step, 16)
KEYED_LIST(TaLogic_GetNextActiveUnit__0013a010, func_00139d90_entry, func_0013ab70_step, 17)
KEYED_LIST(TaLogic_GetNextActiveUnit__00147100, func_00146ec0_entry, func_00148020_step, 18)

/* TaLogic_GetNextActiveUnit__00154ad0 and TaLogic_GetNextActiveUnit__00167e80 (branch and loop
 * coverage): the check_lists cases with the third list walked before the side's slot stage (a
 * third-list win never reaches the slot). Every callee is a stub, including the `_entry` view of
 * 0x167d30 and the `_step` view of 0x1687a0. */
void func_001547a0_entry(struct DotrRuleEntry *entry) { note(PREP, 14, 0, 0, 0, 0, 0); prep_entry = entry; }
int TaLogic_GetMinimumTask_Summon__00155770(int position, int side, struct DotrRuleStep *step)
{
    assert(step == &list_step);
    note(TRY_STEP, 14, position, side, 0, 0, 0);
    return try_answer;
}
void func_00167d30_entry(struct DotrRuleEntry *entry) { note(PREP, 15, 0, 0, 0, 0, 0); prep_entry = entry; }
int func_001687a0_step(int position, int side, struct DotrRuleStep *step)
{
    assert(step == &list_step);
    note(TRY_STEP, 15, position, side, 0, 0, 0);
    return try_answer;
}

static void check_lists_late(ListFn f, int id)
{
    /* the first list's pick is accepted (0 counts): one fill, nothing else */
    list_defaults();
    list_src[0][0] = 0x0109;
    pick_list_answer = 0;
    assert(run_list(f, 1) == 0 && log_n == 2 && was(0, FILL_B, 1, 29, 1, 0, 0, 0) && was(1, LIST_PICK, 1, 0x0109, 0, 0, 0, 0));
    pick_list_answer = 0x0456;
    assert(run_list(f, 1) == 0x0456 && log_n == 2);
    /* everything declines: four fills (29, 30, 31, 31), the side's slot prepared after the third, -1 */
    list_defaults();
    assert(run_list(f, 1) == -1 && list_calls == 4 && calls_of(FILL_B) == 4);
    assert(was(2, FILL_B, 1, 30, 1, 0, 0, 0) && was(4, SLOT_OF, 0x100, 0, 0, 0, 0, 0) && was(5, PREP, id, 0, 0, 0, 0, 0));
    assert(prep_entry == &list_set.entries[7] && was(6, BIT, 1, 0, 0, 0, 0, 0) && was(7, LIVE, 1, 0, 0, 0, 0, 0));
    assert(was(3, FILL_B, 1, 31, 1, 0, 0, 0) && was(8, FILL_B, 1, 31, 1, 0, 0, 0) && log_n == 9);
    assert(calls_of(KIND) == 0 && calls_of(OK_A) == 0 && calls_of(OK_B) == 0 && calls_of(TRY_STEP) == 0);
    /* second list: an entry needs 0x1e0700 >> 5 == 3 and both acceptances; each refusal in turn,
     * then the fourth entry */
    list_src[1][0] = 0x0103, list_src[1][1] = 0x0104, list_src[1][2] = 0x0105, list_src[1][3] = 0x0106;
    kind_for[3] = 2 << 5;
    kind_for[4] = 3 << 5 | 31, oka_for[4] = 0, okb_for[4] = 1;
    kind_for[5] = 3 << 5, oka_for[5] = 1, okb_for[5] = 2;
    kind_for[6] = 3 << 5, oka_for[6] = 1, okb_for[6] = 1;
    assert(run_list(f, 1) == 0x0106 && list_calls == 2 && calls_of(KIND) == 4 && calls_of(OK_A) == 3 && calls_of(OK_B) == 2);
    assert(was(3, KIND, 1, 3, 0, 0, 0, 0) && was(5, OK_A, 0x0104, 0, 0, 0, 0, 0) && was(8, OK_B, 0x0105, 0, 0, 0, 0, 0));
    assert(calls_of(SLOT_OF) == 0);
    /* 0x1e0700 >> 5 of 4 does not count; the first entry can win */
    kind_for[6] = 4 << 5;
    assert(run_list(f, 1) == -1 && calls_of(KIND) == 4 && calls_of(OK_A) == 2);
    kind_for[3] = 3 << 5, oka_for[3] = okb_for[3] = 1;
    assert(run_list(f, 1) == 0x0103 && calls_of(KIND) == 1);
    /* middle stage, entry kind 10 (high byte of +4): live and the try accepted */
    list_defaults();
    list_set.entries[7].kinds = 0x0A33;
    live_answer = 1, try_answer = 0;
    assert(run_list(f, 1) == 0x10100 && was(6, LIVE, 1, 0, 0, 0, 0, 0) && was(7, TRY_STEP, id, 0x100, 1, 0, 0, 0) && log_n == 8);
    assert(calls_of(BIT) == 0);
    assert(run_list(f, 0) == 0x10000 && was(4, SLOT_OF, 0, 0, 0, 0, 0, 0) && was(7, TRY_STEP, id, 0, 0, 0, 0, 0));
    /* try refused: the first acceptance of the position decides */
    try_answer = -1, oka_for[0] = 1;
    assert(run_list(f, 1) == 0x100 && was(8, OK_A, 0x100, 0, 0, 0, 0, 0) && log_n == 9);
    oka_for[0] = 2;
    assert(run_list(f, 1) == -1 && list_calls == 4);
    /* not live: no try */
    live_answer = 2, try_answer = 0, oka_for[0] = 1;
    assert(run_list(f, 1) == 0x100 && calls_of(TRY_STEP) == 0 && was(7, OK_A, 0x100, 0, 0, 0, 0, 0) && log_n == 8);
    /* another slot and a neighbouring kind (11) take the 0x1e2100 arm */
    list_defaults();
    list_slot_answer = 21;
    list_set.entries[21].kinds = 0x0B00;
    bit_for[1][0] = 0, oka_for[0] = 1;
    assert(run_list(f, 1) == 0x100 && prep_entry == &list_set.entries[21] && was(6, BIT, 1, 0, 0, 0, 0, 0));
    assert(was(7, OK_A, 0x100, 0, 0, 0, 0, 0) && log_n == 8 && calls_of(LIVE) == 0);
    oka_for[0] = 0;
    assert(run_list(f, 1) == -1 && calls_of(LIVE) == 0 && list_calls == 4);
    /* bit set: live and try accepted, live and try refused (the acceptance is not asked) */
    bit_for[1][0] = 1, live_answer = 1, try_answer = 5, oka_for[0] = 1;
    assert(run_list(f, 1) == 0x10100 && was(8, TRY_STEP, id, 0x100, 1, 0, 0, 0) && log_n == 9);
    try_answer = -1;
    assert(run_list(f, 1) == -1 && calls_of(TRY_STEP) == 1 && calls_of(OK_A) == 0 && list_calls == 4);
    /* third list: both acceptances; refusals of each, then the third entry */
    list_defaults();
    list_src[2][0] = 0x0103, list_src[2][1] = 0x0104, list_src[2][2] = 0x0105;
    oka_for[3] = 0, okb_for[3] = 1;
    oka_for[4] = 1, okb_for[4] = 0;
    oka_for[5] = 1, okb_for[5] = 1;
    assert(run_list(f, 1) == 0x0105 && list_calls == 3 && calls_of(OK_A) == 3 && calls_of(OK_B) == 2 && calls_of(KIND) == 0 && calls_of(SLOT_OF) == 0);
    okb_for[5] = 0;
    assert(run_list(f, 1) == -1 && list_calls == 4 && calls_of(OK_A) == 3);
    /* fourth list: a clear 0x1e2100 bit and the second acceptance */
    list_defaults();
    list_src[3][0] = 0x0103, list_src[3][1] = 0x0104, list_src[3][2] = 0x0105;
    bit_for[1][3] = 1, okb_for[3] = 1;
    bit_for[1][4] = 0, okb_for[4] = 0;
    bit_for[1][5] = 0, okb_for[5] = 1;
    assert(run_list(f, 1) == 0x0105 && list_calls == 4 && calls_of(OK_B) == 2 && calls_of(OK_A) == 0);
    assert(was(log_n - 1, OK_B, 0x0105, 0, 0, 0, 0, 0) && was(log_n - 2, BIT, 1, 5, 0, 0, 0, 0));
    okb_for[5] = 2;
    assert(run_list(f, 1) == -1 && calls_of(OK_B) == 2);
    /* a full list of 22 is walked to its end */
    {
        int i;

        for (i = 0; i < 22; i++) {
            list_src[3][i] = 0x0104;
        }
        assert(run_list(f, 1) == -1 && calls_of(OK_B) == 22);
    }
}

/* TaLogic_CommandUnit__00121ce0 (branch coverage): each rejection, the column-0 arm both ways, and
 * kinds 0 (both ways), 1, 2, 3, 4 and 5. 0x1229f0 and 0x122b50 are the real functions over this
 * file's stubs: the first is recognised by its closing TaLogic_CommandBtlUnit__00122270(order, 0),
 * the second by its word-829 flag-word store. */
static int pair_at_answer, key_answer;
int func_002507d0_pair(int a, int b) { note(PAIR_AT, a, b, 0, 0, 0, 0); return pair_at_answer; }
int func_001254a0_key(int key) { note(KEY_OF, key, 0, 0, 0, 0, 0); return key_answer; }

static int run_prepare(struct DotrRuleOrder *order, int position, int word, int dfw, int bit, int kind)
{
    memset(order, 0, sizeof *order);
    order->position = (unsigned short)position;
    order->missions = 0x6666;
    order->flags.word = word;
    reset();
    memset(dfw_by, 0, sizeof dfw_by);
    dfw_by[position & 127] = dfw;
    bit_answer = bit;
    kind_answer = kind;
    pair_answer = 0x44;
    key_answer = 0x1A5;
    set_cells(0x0304);
    return TaLogic_CommandUnit__00121ce0(order);
}

static void check_prepare(void)
{
    struct DotrRuleOrder order;

    assert(run_prepare(&order, 0x0203, 0, 0, 0, 0) == 0 && log_n == 2);
    assert(was(0, PAIR, 2, 3, 0, 0, 0, 0) && was(1, DFW, 2, 3, 0, 0, 0, 0));
    assert(run_prepare(&order, 0x0203, 0, 1, 1, 0) == 0 && log_n == 3 && was(2, BIT, 2, 3, 0, 0, 0, 0));
    assert(run_prepare(&order, 0xFFFF, 0, 1, 0, 0) == 0 && log_n == 3 && order.missions == 0x6666);
    assert(run_prepare(&order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 4 && was(3, SEND_A, 3, 2, 0, 0, 0, 0)
        && send_order == &order);
    assert(run_prepare(&order, 0x0200, -2, 1, 0, 0) == 1 && log_n == 3);
    /* kind 0, 0x2507d0 not 1: TaLogic_CommandBtlUnit__00122270(order, 1) directly */
    pair_at_answer = 0;
    assert(run_prepare(&order, 0x0203, 7, 1, 0, 0x1F) == 1 && log_n == 6 && was(3, KIND, 2, 3, 0, 0, 0, 0)
        && was(4, PAIR_AT, 2, 3, 0, 0, 0, 0) && was(5, TRY, 1, 0, 0, 0, 0, 0) && send_order == &order);
    /* kind 0, 0x2507d0 == 1: the real TaLogic_CommandEfcMonster__001229f0 (word 564: its closing
     * call only) */
    pair_at_answer = 1;
    word_answer = 564;
    assert(run_prepare(&order, 0x0203, 7, 1, 0, 0) == 1 && was(4, PAIR_AT, 2, 3, 0, 0, 0, 0) && was(5, DFW, 2, 3, 0, 0, 0, 0)
        && was(log_n - 1, TRY, 0, 0, 0, 0, 0, 0) && send_order == &order && order.missions == 0x6666);
    pair_at_answer = 0;
    /* kinds 1, 5 and 3 (default): the +4 store only */
    assert(run_prepare(&order, 0x0203, 7, 1, 0, 0x20) == 1 && log_n == 4 && order.missions == 0x5FF
        && order.flags.word == 7 && send_order == NULL);
    assert(run_prepare(&order, 0x0203, 7, 1, 0, 0xA0) == 1 && log_n == 4 && order.missions == 0x5FF);
    assert(run_prepare(&order, 0x0203, 7, 1, 0, 0x60) == 1 && log_n == 4 && order.missions == 0x5FF);
    /* kind 2 */
    assert(run_prepare(&order, 0x0203, 7, 1, 0, 0x40) == 1 && log_n == 6 && was(4, KEY_OF, 0x44, 0, 0, 0, 0, 0)
        && was(5, SEND_C, 2, 0, 0xA5, 0, 0, 0) && send_order == &order && order.missions == 0x6666);
    /* kind 4: the real TaLogic005_CommandTrapBUnit (word 829 keeps only the low flag halfword) */
    word_answer = 829;
    grid_calls = item_calls = 0;
    assert(run_prepare(&order, 0x0203, 0x1010ABCD, 1, 0, 0x80) == 1 && was(4, DFW, 2, 3, 0, 0, 0, 0)
        && calls_of(TRY) == 0 && log_n == 8 && order.missions == 0x5FF && order.flags.word == 0xABCD);
}

/* TaLogic_CommandUnit__001429b0 (branch coverage): each rejection, column 0 with the flag word
 * other than -1 and the four 0x27bbc0 / 0x1de6d0 outcomes, kind 0 both ways, kind 2, and kinds 1
 * and 3. Every callee is a stub. */
enum { OK_C = 100, KEY_B, ORDER_A, ORDER_B };
static int okc_answer;
int TaLogic_IsAttackedNow(int a, int b, int c) { note(OK_C, a, b, c, 0, 0, 0); return okc_answer; }
int func_001461c0_key(int key) { note(KEY_B, key, 0, 0, 0, 0, 0); return key_answer; }
void func_00143540_order(struct DotrRuleOrder *order) { note(ORDER_A, 0, 0, 0, 0, 0, 0); send_order = order; }
void TaLogic012_CommandLineUnit(struct DotrRuleOrder *order) { note(ORDER_B, 0, 0, 0, 0, 0, 0); send_order = order; }

static int run_prepare_b(struct DotrRuleOrder *order, int position, int word, int dfw, int bit, int kind)
{
    memset(order, 0, sizeof *order);
    order->position = (unsigned short)position;
    order->missions = 0x6666;
    order->flags.word = word;
    reset();
    memset(dfw_by, 0, sizeof dfw_by);
    dfw_by[position & 127] = dfw;
    bit_answer = bit;
    kind_answer = kind;
    pair_answer = 0x44;
    key_answer = 0x1A5;
    send_order = NULL;
    return TaLogic_CommandUnit__001429b0(order);
}

static void check_prepare_b(void)
{
    struct DotrRuleOrder order;

    assert(run_prepare_b(&order, 0x0203, 0, 0, 0, 0) == 0 && log_n == 2);
    assert(was(0, PAIR, 2, 3, 0, 0, 0, 0) && was(1, DFW, 2, 3, 0, 0, 0, 0));
    assert(run_prepare_b(&order, 0x0203, 0, 1, 1, 0) == 0 && log_n == 3 && was(2, BIT, 2, 3, 0, 0, 0, 0));
    assert(run_prepare_b(&order, 0xFFFF, 0, 1, 0, 0) == 0 && log_n == 3 && order.missions == 0x6666);
    /* column 0 */
    assert(run_prepare_b(&order, 0x0200, -2, 1, 0, 0) == 1 && log_n == 3 && order.missions == 0x6666);
    okc_answer = 1, live_answer = 1;
    assert(run_prepare_b(&order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 5 && was(3, OK_C, 2, 0, 0, 0, 0, 0)
        && was(4, LIVE, 2, 0, 0, 0, 0, 0) && order.missions == 0x50A && order.flags.word == -1);
    live_answer = 0;
    assert(run_prepare_b(&order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 5 && order.missions == 0x5FF);
    okc_answer = 2, live_answer = 1;
    assert(run_prepare_b(&order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 5 && was(4, LIVE, 2, 0, 0, 0, 0, 0)
        && order.missions == 0xA05);
    live_answer = 2;
    assert(run_prepare_b(&order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 5 && order.missions == 0x5FF);
    live_answer = 0;
    /* kind 0: word 352 or not */
    word_answer = 352;
    assert(run_prepare_b(&order, 0x0203, 7, 1, 0, 0x1F) == 1 && log_n == 6 && was(3, KIND, 2, 3, 0, 0, 0, 0)
        && was(4, WORD, 2, 3, 0, 0, 0, 0) && was(5, ORDER_A, 0, 0, 0, 0, 0, 0) && send_order == &order
        && order.missions == 0x6666);
    word_answer = 353;
    assert(run_prepare_b(&order, 0x0203, 7, 1, 0, 0) == 1 && log_n == 6 && was(5, ORDER_B, 0, 0, 0, 0, 0, 0)
        && send_order == &order && order.missions == 0x6666);
    /* kind 2 */
    assert(run_prepare_b(&order, 0x0203, 7, 1, 0, 0x40) == 1 && log_n == 6 && was(4, KEY_B, 0x44, 0, 0, 0, 0, 0)
        && was(5, SEND_C, 2, 0, 0xA5, 0, 0, 0) && send_order == &order);
    /* kinds 1 and 3 (default): the +4 store only */
    assert(run_prepare_b(&order, 0x0203, 7, 1, 0, 0x20) == 1 && log_n == 4 && order.missions == 0x5FF
        && order.flags.word == 7 && send_order == NULL);
    assert(run_prepare_b(&order, 0x0203, 7, 1, 0, 0x60) == 1 && log_n == 4 && order.missions == 0x5FF);
}

/* TaLogic_CommandUnit__001591b0 (branch coverage): each rejection, column 0 with the flag word -1
 * and not, kind 0 both ways, kind 2 through the real TaLogic_GetWarnHideLvl__0015d120 (total 1200:
 * 2000), kind 5 with the high byte of +4 2 and not, and kinds 1 and 3. Every other callee is a
 * stub. */
enum { COL_C = 110, ORDER_C, ORDER_D };
static int order_d_04;
void TaLogic018_CommandLDUnit(struct DotrRuleOrder *order) { note(COL_C, 0, 0, 0, 0, 0, 0); send_order = order; }
void TaLogic018_CommandEfcMonster(struct DotrRuleOrder *order) { note(ORDER_C, 0, 0, 0, 0, 0, 0); send_order = order; }
void func_00159d00_order(struct DotrRuleOrder *order)
{
    note(ORDER_D, 0, 0, 0, 0, 0, 0);
    send_order = order;
    order->missions = (unsigned short)order_d_04;
}

static int run_prepare_c(struct DotrRuleOrder *order, int position, int word, int dfw, int bit, int kind)
{
    memset(order, 0, sizeof *order);
    order->position = (unsigned short)position;
    order->missions = 0x6666;
    order->flags.word = word;
    reset();
    memset(dfw_by, 0, sizeof dfw_by);
    dfw_by[position & 127] = dfw;
    bit_answer = bit;
    kind_answer = kind;
    pair_answer = 0x44;
    total_answer = 1200;
    set_cells(0x0304);
    cell_by[9] = 0x7B5A;
    send_order = NULL;
    return TaLogic_CommandUnit__001591b0(order);
}

static void check_prepare_c(void)
{
    struct DotrRuleOrder order;

    assert(run_prepare_c(&order, 0x0203, 0, 0, 0, 0) == 0 && log_n == 3);
    assert(was(0, CELL, 2, 3, 0, 0, 0, 0) && was(1, PAIR, 2, 3, 0, 0, 0, 0) && was(2, DFW, 2, 3, 0, 0, 0, 0));
    assert(run_prepare_c(&order, 0x0203, 0, 1, 1, 0) == 0 && log_n == 4 && was(3, BIT, 2, 3, 0, 0, 0, 0));
    assert(run_prepare_c(&order, 0xFFFF, 0, 1, 0, 0) == 0 && log_n == 4 && order.missions == 0x6666);
    /* column 0 */
    assert(run_prepare_c(&order, 0x0200, -2, 1, 0, 0) == 1 && log_n == 4 && send_order == NULL);
    assert(run_prepare_c(&order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 5 && was(4, COL_C, 0, 0, 0, 0, 0, 0)
        && send_order == &order && order.missions == 0x6666);
    /* kind 0: 0x2507d0 == 1 or not */
    order_d_04 = 0x6666;
    pair_at_answer = 1;
    assert(run_prepare_c(&order, 0x0203, 7, 1, 0, 0x1F) == 1 && log_n == 7 && was(4, KIND, 2, 3, 0, 0, 0, 0)
        && was(5, PAIR_AT, 2, 3, 0, 0, 0, 0) && was(6, ORDER_C, 0, 0, 0, 0, 0, 0) && send_order == &order);
    pair_at_answer = 0;
    assert(run_prepare_c(&order, 0x0203, 7, 1, 0, 0) == 1 && log_n == 7 && was(6, ORDER_D, 0, 0, 0, 0, 0, 0)
        && send_order == &order && order.flags.word == 7);
    /* kind 2 */
    assert(run_prepare_c(&order, 0x0203, 7, 1, 0, 0x40) == 1 && log_n == 7 && was(5, TOTAL, 0, 0, 0, 0, 0, 0)
        && was(6, SEND_C, 3, 0, 2000 & 0xFF, 0, 0, 0) && send_order == &order && order.missions == 0x6666);
    /* kind 5: the high byte of +4 after 0x159d00 is 2 or not */
    order_d_04 = 0x02AB;
    assert(run_prepare_c(&order, 0x0203, 0x8109ABCD, 1, 0, 0xA0) == 1 && log_n == 7 && was(5, ORDER_D, 0, 0, 0, 0, 0, 0)
        && was(6, CELL, 0x81, 9, 0, 0, 0, 0) && order.missions == 0x6FF && order.flags.word == 0x7B5AABCD);
    order_d_04 = 0x03AB;
    assert(run_prepare_c(&order, 0x0203, 0x8109ABCD, 1, 0, 0xA0) == 1 && log_n == 6 && order.missions == 0x03AB
        && order.flags.word == (int)0x8109ABCD);
    /* kinds 1 and 3 (default): the +4 store only */
    assert(run_prepare_c(&order, 0x0203, 7, 1, 0, 0x20) == 1 && log_n == 5 && order.missions == 0x5FF
        && order.flags.word == 7 && send_order == NULL);
    assert(run_prepare_c(&order, 0x0203, 7, 1, 0, 0x60) == 1 && log_n == 5 && order.missions == 0x5FF);
}

/* TaLogic_CommandUnit__0015d8e0 (branch coverage): each rejection, column 0 with the flag word
 * other than -1 and the four 0x27bbc0 / 0x1de6d0 outcomes, kind 0 both ways, kind 2, kinds 1, 3 and
 * 4, and kind 5 (default). Every callee is a stub. */
enum { KEY_D = 120, ORDER_E, ORDER_F, ORDER_G };
void TaLogic019_CommandEfcMonster(struct DotrRuleOrder *order) { note(ORDER_E, 0, 0, 0, 0, 0, 0); send_order = order; }
void TaLogic019_CommandBtlUnit(struct DotrRuleOrder *order) { note(ORDER_F, 0, 0, 0, 0, 0, 0); send_order = order; }
void TaLogic019_CommandSpellUnit(struct DotrRuleOrder *order) { note(ORDER_G, 0, 0, 0, 0, 0, 0); send_order = order; }
int func_00162580_key(int key) { note(KEY_D, key, 0, 0, 0, 0, 0); return key_answer; }

static int run_prepare_d(struct DotrRuleOrder *order, int position, int word, int dfw, int bit, int kind)
{
    memset(order, 0, sizeof *order);
    order->position = (unsigned short)position;
    order->missions = 0x6666;
    order->flags.word = word;
    reset();
    memset(dfw_by, 0, sizeof dfw_by);
    dfw_by[position & 127] = dfw;
    bit_answer = bit;
    kind_answer = kind;
    pair_answer = 0x44;
    key_answer = 0x1A5;
    send_order = NULL;
    return TaLogic_CommandUnit__0015d8e0(order);
}

static void check_prepare_d(void)
{
    struct DotrRuleOrder order;
    int kind;

    assert(run_prepare_d(&order, 0x0203, 0, 0, 0, 0) == 0 && log_n == 3);
    assert(was(0, CELL, 2, 3, 0, 0, 0, 0) && was(1, PAIR, 2, 3, 0, 0, 0, 0) && was(2, DFW, 2, 3, 0, 0, 0, 0));
    assert(run_prepare_d(&order, 0x0203, 0, 1, 1, 0) == 0 && log_n == 4 && was(3, BIT, 2, 3, 0, 0, 0, 0));
    assert(run_prepare_d(&order, 0xFFFF, 0, 1, 0, 0) == 0 && log_n == 4 && order.missions == 0x6666);
    /* column 0 */
    assert(run_prepare_d(&order, 0x0200, -2, 1, 0, 0) == 1 && log_n == 4 && order.missions == 0x6666);
    okc_answer = 1, live_answer = 1;
    assert(run_prepare_d(&order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 6 && was(4, OK_C, 2, 0, 0, 0, 0, 0)
        && was(5, LIVE, 2, 0, 0, 0, 0, 0) && order.missions == 0x50A && order.flags.word == -1);
    live_answer = 0;
    assert(run_prepare_d(&order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 6 && order.missions == 0x5FF);
    okc_answer = 2, live_answer = 1;
    assert(run_prepare_d(&order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 6 && was(5, LIVE, 2, 0, 0, 0, 0, 0)
        && order.missions == 0xA05);
    live_answer = 2;
    assert(run_prepare_d(&order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 6 && order.missions == 0x5FF);
    live_answer = 0;
    /* kind 0: 0x2507d0 == 1 or not */
    pair_at_answer = 1;
    assert(run_prepare_d(&order, 0x0203, 7, 1, 0, 0x1F) == 1 && log_n == 7 && was(4, KIND, 2, 3, 0, 0, 0, 0)
        && was(5, PAIR_AT, 2, 3, 0, 0, 0, 0) && was(6, ORDER_E, 0, 0, 0, 0, 0, 0) && send_order == &order
        && order.missions == 0x6666);
    pair_at_answer = 0;
    assert(run_prepare_d(&order, 0x0203, 7, 1, 0, 0) == 1 && log_n == 7 && was(6, ORDER_F, 0, 0, 0, 0, 0, 0)
        && send_order == &order && order.missions == 0x6666);
    /* kind 2 */
    assert(run_prepare_d(&order, 0x0203, 7, 1, 0, 0x40) == 1 && log_n == 7 && was(5, KEY_D, 0x44, 0, 0, 0, 0, 0)
        && was(6, SEND_C, 5, 0, 0xA5, 0, 0, 0) && send_order == &order);
    /* kinds 1, 3 and 4 */
    for (kind = 0x20; kind <= 0x80; kind += kind == 0x20 ? 0x40 : 0x20) {
        assert(run_prepare_d(&order, 0x0203, 7, 1, 0, kind) == 1 && log_n == 6 && was(5, ORDER_G, 0, 0, 0, 0, 0, 0)
            && send_order == &order && order.missions == 0x6666);
    }
    /* kind 5 (default): the +4 store only */
    assert(run_prepare_d(&order, 0x0203, 7, 1, 0, 0xA0) == 1 && log_n == 5 && order.missions == 0x5FF
        && order.flags.word == 7 && send_order == NULL);
}

/* TaLogic_CommandUnit__0014bde0, TaLogic_CommandUnit__00150270, TaLogic_CommandUnit__00135790,
 * TaLogic_CommandUnit__0013e1d0, TaLogic_CommandUnit__00118330, TaLogic_CommandUnit__0011cb20 and
 * TaLogic_CommandUnit__00131540 (branch coverage): each rejection, column 0 with the flag word
 * other than -1 and every 0x27bbc0 / 0x1de6d0 outcome (and for 0x150270 the 0x153ab0 answer, for
 * 0x131540 both key outcomes), then every kind 0-5 with both outcomes of a kind's own test. Every
 * callee is a stub. */
enum { SUB = 130, KEY_E };
static int sub_answer, flag_answer, pair_e = 0x44;
#define SUB_ORDER(name, at) void name(struct DotrRuleOrder *order) { note(SUB, at, -1, 0, 0, 0, 0); send_order = order; }
#define SUB_FLAG(name, at) void name(struct DotrRuleOrder *order, int flag) { note(SUB, at, flag, 0, 0, 0, 0); send_order = order; }
#define SUB_KEY(name, at) int name(int key) { note(KEY_E, at, key, 0, 0, 0, 0); return key_answer; }
SUB_ORDER(TaLogic014_CommandEfcMonster, 0x14c720)
SUB_ORDER(func_0014c3b0_order, 0x14c3b0)
SUB_ORDER(TaLogic014_CommandSpellUnit, 0x14c870)
SUB_KEY(func_0014f9b0_key, 0x14f9b0)
/* flag_quiet (0x153550's tests): 1 answers 0 and 2 answers 0x101 (low byte 1), counted but not logged. */
static int flag_quiet, flag_quiet_calls;
int func_00153ab0_flag(void)
{
    if (flag_quiet) {
        flag_quiet_calls++;
        return flag_quiet == 1 ? 0 : 0x101;
    }
    note(SUB, 0x153ab0, 0, 0, 0, 0, 0);
    return flag_answer;
}
SUB_ORDER(func_00150850_order, 0x150850)
SUB_KEY(func_00153ed0_key, 0x153ed0)
SUB_ORDER(func_00136010_order, 0x136010)
SUB_FLAG(func_00135d50_order, 0x135d50)
SUB_ORDER(TaLogic009_CommandSpellUnit, 0x1360e0)
SUB_ORDER(TaLogic011_CommandEfcMonster, 0x13ecf0)
SUB_FLAG(func_0013e7b0_order, 0x13e7b0)
SUB_KEY(func_00141b30_key, 0x141b30)
SUB_KEY(func_00130780_key, 0x130780)
SUB_KEY(func_0012b1b0_key, 0x12b1b0)
int TaLogic_CommandGunnerUnit(struct DotrRuleOrder *order, int a) { note(SUB, 0x27d720, a, 0, 0, 0, 0); send_order = order; return sub_answer; }
SUB_FLAG(func_001188f0_order, 0x1188f0)
SUB_ORDER(TaLogic_CommandTrapUnit__00118f50, 0x118f50)
SUB_KEY(func_0011aa10_key, 0x11aa10)
SUB_ORDER(TaLogic_CommandEfcMonster__0011d9b0, 0x11d9b0)
SUB_FLAG(func_0011d110_order, 0x11d110)
SUB_ORDER(TaLogic_CommandTrapUnit__0011dc00, 0x11dc00)
SUB_KEY(func_0011fa80_key, 0x11fa80)
SUB_FLAG(func_00131b30_order, 0x131b30)
SUB_ORDER(TaLogic008_CommandSpellUnit, 0x132280)
SUB_KEY(func_00134ae0_key, 0x134ae0)

/* 0x10ce20's callees; 0x10f030 only counts its calls so the shared log checks still fit. */
static int low_calls;
static struct DotrRuleSet low_set;
int TaLogic002_IsActiveMode(void) { low_calls++; return 0x37; }
SUB_KEY(TaLogic_GetWarnHideLvl__0010eda0, 0x10eda0)
void func_0010d880_rule(struct DotrRuleOrder *order, int flag, int deep)
{
    assert(flag == 1 && deep == 7);
    note(SUB, 0x10d880, -1, 0, 0, 0, 0);
    send_order = order;
}
void func_0010d490_rule(struct DotrRuleOrder *order, int flag, int deep)
{
    assert(deep == 7);
    note(SUB, 0x10d490, flag, 0, 0, 0, 0);
    send_order = order;
}
void func_0010db00_rule(struct DotrRuleOrder *order, int one, int hit)
{
    assert(hit == 0);
    note(SUB, 0x10db00, one, 0, 0, 0, 0);
    send_order = order;
}
/* Every call asks 0x10f030 once and replaces only the low nibble of the set's byte 0x1a1. */
static int set_0010ce20(struct DotrRuleOrder *order)
{
    int r;

    memset(&low_set, 0x5A, sizeof low_set);
    low_set.aiActiveRelated = 0xA0;
    low_calls = 0;
    r = TaLogic_CommandUnit__0010ce20(&low_set, order);
    assert(low_calls == 1 && low_set.aiActiveRelated == 0xA7 && low_set.inputCount == 0x5A5A
        && low_set.ids[31] == 0x5A5A);
    return r;
}

/* 0x1271f0's callees. */
SUB_ORDER(func_00127b00_order, 0x127b00)
SUB_FLAG(func_001277c0_rule, 0x1277c0)

/* 0x163660's callees. */
SUB_ORDER(TaLogic020_CommandEfcMonster, 0x164020)
SUB_FLAG(func_00163c50_order, 0x163c50)
SUB_ORDER(func_00164260_order, 0x164260)
SUB_KEY(TaLogic_GetWarnHideLvl__001676d0, 0x1676d0)

/* 0x113140's callees: each checks the passed-on second argument 0x77. */
void TaLogic001_CommandEfcMonster(struct DotrRuleOrder *order, int deep) { assert(deep == 0x77); note(SUB, 0x113bb0, -1, 0, 0, 0, 0); send_order = order; }
void func_00113760_order(struct DotrRuleOrder *order, int flag, int deep)
{
    assert(deep == 0x77);
    note(SUB, 0x113760, flag, 0, 0, 0, 0);
    send_order = order;
}
static int deep_00113140(struct DotrRuleOrder *order) { return TaLogic_CommandUnit__00113140(order, 0x77); }

/* 0x12c1a0's callees, checking the passed-on second argument 0x77 where they take it. */
void TaLogic007_CommandEfcMonster(struct DotrRuleOrder *order, int deep) { assert(deep == 0x77); note(SUB, 0x12cbe0, -1, 0, 0, 0, 0); send_order = order; }
void func_0012c7f0_order(struct DotrRuleOrder *order, int flag, int deep)
{
    assert(deep == 0x77);
    note(SUB, 0x12c7f0, flag, 0, 0, 0, 0);
    send_order = order;
}
SUB_ORDER(TaLogic_CommandTrapUnit__0012ce50, 0x12ce50)
static int deep_0012c1a0(struct DotrRuleOrder *order) { return TaLogic_CommandUnit__0012c1a0(order, 0x77); }

/* 0x146ec0's and 0x139d90's callees, checking the passed-on second argument 0x77 where they take it. */
SUB_ORDER(TaLogic013_CommandInvadeUnit, 0x147830)
SUB_ORDER(func_001474f0_order, 0x1474f0)
SUB_KEY(TaLogic_GetWarnHideLvl__0014b330, 0x14b330)
SUB_KEY(TaLogic_GetWarnHideLvl__0013da70, 0x13da70)
void TaLogic013_CommandSpellUnit(struct DotrRuleOrder *order, int deep) { assert(deep == 0x77); note(SUB, 0x147b90, -1, 0, 0, 0, 0); send_order = order; }
static int spot_answer;
int TaLogicCom_GetUseXYRangeSpell__00112600(int side, unsigned *record)
{
    assert(record == slot_answer);
    note(KEY_E, 0x112600, side, 0, 0, 0, 0);
    return spot_answer;
}
void func_0013a400_order(struct DotrRuleOrder *order, int flag, int deep)
{
    assert(deep == 0x77);
    note(SUB, 0x13a400, flag, 0, 0, 0, 0);
    send_order = order;
}
static int deep_00146ec0(struct DotrRuleOrder *order) { return TaLogic_CommandUnit__00146ec0(order, 0x77); }
static int deep_00139d90(struct DotrRuleOrder *order) { return TaLogic_CommandUnit__00139d90(order, 0x77); }
static int one_00139d90(struct DotrRuleOrder *order) { return TaLogic_CommandUnit__00139d90(order, 0x301); }

static int run_prepare_e(int (*fn)(struct DotrRuleOrder *), struct DotrRuleOrder *order, int position, int word,
    int dfw, int bit, int kind)
{
    memset(order, 0, sizeof *order);
    order->position = (unsigned short)position;
    order->missions = 0x6666;
    order->flags.word = word;
    reset();
    memset(dfw_by, 0, sizeof dfw_by);
    dfw_by[position & 127] = dfw;
    bit_answer = bit;
    kind_answer = kind;
    pair_answer = pair_e;
    key_answer = 0x1A5;
    send_order = NULL;
    return fn(order);
}

/* The rejections and column 0. lead: the function starts with 0x1e1090; pre: 0x153ab0 is asked
 * first (answering 0 here); c: the third 0x27bbc0 argument; last: the store when neither answer
 * is 1; plain: the 0x27bbc0 != 1 stores are the bare 0xA05 / last pair. Returns the log index
 * after the kind call. */
static int check_prepare_e(int (*fn)(struct DotrRuleOrder *), int lead, int pre, int c, int last, int plain)
{
    struct DotrRuleOrder order;
    int n = lead + 3;

    assert(run_prepare_e(fn, &order, 0x0203, 0, 0, 0, 0) == 0 && log_n == lead + 2);
    assert((!lead || was(0, CELL, 2, 3, 0, 0, 0, 0)) && was(lead, PAIR, 2, 3, 0, 0, 0, 0)
        && was(lead + 1, DFW, 2, 3, 0, 0, 0, 0));
    assert(run_prepare_e(fn, &order, 0x0203, 0, 1, 1, 0) == 0 && log_n == n && was(lead + 2, BIT, 2, 3, 0, 0, 0, 0));
    assert(run_prepare_e(fn, &order, 0xFFFF, 0, 1, 0, 0) == 0 && log_n == n && order.missions == 0x6666);
    assert(run_prepare_e(fn, &order, 0x0200, -2, 1, 0, 0) == 1 && log_n == n && order.missions == 0x6666);
    flag_answer = 0;
    okc_answer = 1, live_answer = 1;
    assert(run_prepare_e(fn, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == n + pre + 2
        && (!pre || was(n, SUB, 0x153ab0, 0, 0, 0, 0, 0)) && was(n + pre, OK_C, 2, 0, c, 0, 0, 0)
        && was(n + pre + 1, LIVE, 2, 0, 0, 0, 0, 0) && order.missions == 0x50A && order.flags.word == -1);
    live_answer = 0;
    assert(run_prepare_e(fn, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == n + pre + 2 && order.missions == 0x5FF);
    if (plain) {
        okc_answer = 2, live_answer = 1;
        assert(run_prepare_e(fn, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == n + pre + 2
            && was(n + pre + 1, LIVE, 2, 0, 0, 0, 0, 0) && order.missions == 0xA05 && order.flags.word == -1);
        live_answer = 2;
        assert(run_prepare_e(fn, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == n + pre + 2
            && order.missions == last && order.flags.word == -1);
    }
    okc_answer = 0, live_answer = 0;
    /* an unlisted kind stores 0x5FF only */
    assert(run_prepare_e(fn, &order, 0x0203, 7, 1, 0, 0xC0) == 1 && log_n == n + 1 && was(n, KIND, 2, 3, 0, 0, 0, 0)
        && order.missions == 0x5FF && order.flags.word == 7 && send_order == NULL);
    return n + 1;
}

/* One kind: the result is 1, the log holds `more` entries after the kind call. */
static int kind_e(int (*fn)(struct DotrRuleOrder *), struct DotrRuleOrder *order, int n, int kind, int more)
{
    return run_prepare_e(fn, order, 0x0203, 7, 1, 0, kind << 5 | 0x1F) == 1 && log_n == n + more
        && order->flags.word == 7;
}

/* A kind that calls one callee with the order: nothing stored. */
static int sub_e(int (*fn)(struct DotrRuleOrder *), int n, int kind, int at, int flag)
{
    struct DotrRuleOrder order;

    return kind_e(fn, &order, n, kind, 1) && was(n, SUB, at, flag, 0, 0, 0, 0) && send_order == &order
        && order.missions == 0x6666;
}

/* A kind that only stores 0x5FF. */
static int store_e(int (*fn)(struct DotrRuleOrder *), int n, int kind)
{
    struct DotrRuleOrder order;

    return kind_e(fn, &order, n, kind, 0) && order.missions == 0x5FF && send_order == NULL;
}

/* Kind 0 by the 0x2507d0 answer: 1 or not. */
static int pair_e_kind(int (*fn)(struct DotrRuleOrder *), int n, int at_one, int at_other, int flag)
{
    struct DotrRuleOrder order;
    int ok;

    pair_at_answer = 1;
    ok = kind_e(fn, &order, n, 0, 2) && was(n, PAIR_AT, 2, 3, 0, 0, 0, 0) && was(n + 1, SUB, at_one, -1, 0, 0, 0, 0)
        && send_order == &order && order.missions == 0x6666;
    pair_at_answer = 0;
    return ok && kind_e(fn, &order, n, 0, 2) && was(n + 1, SUB, at_other, flag, 0, 0, 0, 0) && send_order == &order
        && order.missions == 0x6666;
}

/* Kind 2 sending TaLogic_CommandEncUnit(order, a, 0, key callee & 0xFF). */
static int key_e(int (*fn)(struct DotrRuleOrder *), int n, int at, int a)
{
    struct DotrRuleOrder order;

    return kind_e(fn, &order, n, 2, 2) && was(n, KEY_E, at, 0x44, 0, 0, 0, 0)
        && was(n + 1, SEND_C, a, 0, 0xA5, 0, 0, 0) && send_order == &order && order.missions == 0x6666;
}

/* TaLogic013_CommandEfcMonster, TaLogic010_CommandSpellUnit and TaLogic001_CommandSpellUnit (branch
 * coverage), each on an order at 0x0205 holding 0x6666 and the flag word 0x1234ABCD. 0x13aa40 runs
 * on the real 0x13d2c0; every other callee is a stub. */
static void sub_start(struct DotrRuleOrder *order, int kind)
{
    memset(order, 0, sizeof *order);
    order->position = 0x0205;
    order->missions = 0x6666;
    order->flags.word = 0x1234ABCD;
    reset();
    set_cells(0);
    cell_by[5] = 0x7E1, cell_by[3] = 0x1C7, cell_by[0] = 0x5A9;
    pair_answer = 0x44;
    kind_answer = kind << 5 | 0x1F;
    send_order = NULL;
}

static int sub_is(struct DotrRuleOrder *order, int n, int half, int word)
{
    return log_n == n && order->missions == half && order->flags.word == word && order->position == 0x0205;
}

static void check_prepare_subs(void)
{
    static unsigned rec;
    struct DotrRuleOrder order;
    int i;

    /* 0x147ac0: word 290 and 291, count 2 and 3 */
    set_by[0] = 0x61;
    for (i = 0; i < 3; i++) {
        sub_start(&order, 0);
        word_answer = i == 0 ? 290 : 291;
        count_of[0x61] = i == 2 ? 3 : 2;
        TaLogic013_CommandEfcMonster(&order);
        assert(was(0, PAIR, 2, 5, 0, 0, 0, 0) && was(1, WORD, 2, 5, 0, 0, 0, 0));
        if (i == 0) {
            assert(sub_is(&order, 3, 0x6666, 0x1234ABCD) && was(2, SUB, 0x1474f0, -1, 0, 0, 0, 0) && send_order == &order);
        } else {
            assert(was(2, SET, 0, 0, 0x44, 1, 6, 14) && was(3, COUNT, 0x61, 0, 0, 0, 0, 0));
            if (i == 1) {
                assert(sub_is(&order, 5, 0x6666, 0x1234ABCD) && was(4, SUB, 0x1474f0, -1, 0, 0, 0, 0)
                    && send_order == &order);
            } else {
                assert(sub_is(&order, 5, 0x3FF, 0x07E1ABCD) && was(4, CELL, 2, 5, 0, 0, 0, 0) && send_order == NULL);
            }
        }
    }
    word_answer = 0;
    count_of[0x61] = 0;

    /* 0x13aa40: kinds 0 and 2 (neither listed), 3, and 1 with the 0x13d2c0 answer -1, 0 and 0x203 */
    for (i = 0; i < 3; i += 2) {
        sub_start(&order, i);
        TaLogic010_CommandSpellUnit(&order, 0x99);
        assert(sub_is(&order, 3, 0x5FF, 0x1234ABCD) && was(0, PAIR, 2, 5, 0, 0, 0, 0) && was(1, CELL, 2, 5, 0, 0, 0, 0)
            && was(2, KIND, 2, 5, 0, 0, 0, 0));
    }
    sub_start(&order, 3);
    TaLogic010_CommandSpellUnit(&order, 0x99);
    assert(sub_is(&order, 4, 0x3FF, 0x05E1ABCD) && was(3, CELL, 0x44, 0, 0, 0, 0, 0));
    sub_start(&order, 1);
    pack_by[0] = -1, pack_by[1] = 0x304, compare_answer = 1;
    TaLogic010_CommandSpellUnit(&order, 0x99);
    assert(sub_is(&order, 7, 0x5FF, 0x1234ABCD));
    sub_start(&order, 1);
    pack_by[0] = 0, pack_by[1] = 0;
    TaLogic010_CommandSpellUnit(&order, 0x99);
    assert(sub_is(&order, 10, 0x6FF, 0x07A9ABCD) && was(9, CELL, 0, 0, 0, 0, 0, 0));
    sub_start(&order, 1);
    pack_by[0] = 0x203, pack_by[1] = 0x304;
    TaLogic010_CommandSpellUnit(&order, 0x99);
    assert(sub_is(&order, 10, 0x6FF, 0x07C7ABCD) && was(9, CELL, 2, 3, 0, 0, 0, 0));
    compare_answer = 0;

    /* 0x113ed0: no record; kinds 0 and 2; kind 1 with record words 2, 20, 3, 4 and a high bit set,
     * the 0x112600 answer -1, 0 and 0x1C7 and the record's low six bits 0 and 1; kind 3 both ways */
    sub_start(&order, 1);
    slot_answer = NULL;
    TaLogic001_CommandSpellUnit(&order, 0x99);
    assert(sub_is(&order, 3, 0x5FF, 0x1234ABCD) && was(0, PAIR, 2, 5, 0, 0, 0, 0) && was(1, CELL, 2, 5, 0, 0, 0, 0)
        && was(2, SLOT, 2, 5, 0, 0, 0, 0));
    slot_answer = &rec;
    for (i = 0; i < 3; i += 2) {
        sub_start(&order, i);
        rec = 2 << 8;
        TaLogic001_CommandSpellUnit(&order, 0x99);
        assert(sub_is(&order, 4, 0x5FF, 0x1234ABCD) && was(3, KIND, 2, 5, 0, 0, 0, 0));
    }
    sub_start(&order, 1);
    rec = 2 << 8 | 0x3F;
    TaLogic001_CommandSpellUnit(&order, 0x99);
    assert(sub_is(&order, 4, 0x6FF, 0x0303ABCD));
    sub_start(&order, 1);
    rec = 0x10000 | 2 << 8;
    TaLogic001_CommandSpellUnit(&order, 0x99);
    assert(sub_is(&order, 4, 0x5FF, 0x1234ABCD));
    sub_start(&order, 1);
    rec = 20 << 8;
    TaLogic001_CommandSpellUnit(&order, 0x99);
    assert(sub_is(&order, 4, 0xCFF, 0x1234ABCD));
    sub_start(&order, 1);
    rec = 3 << 8;
    TaLogic001_CommandSpellUnit(&order, 0x99);
    assert(sub_is(&order, 4, 0x5FF, 0x1234ABCD));
    sub_start(&order, 1);
    rec = 4 << 8, spot_answer = -1;
    TaLogic001_CommandSpellUnit(&order, 0x99);
    assert(sub_is(&order, 5, 0x5FF, 0x1234ABCD) && was(4, KEY_E, 0x112600, 0x44, 0, 0, 0, 0));
    sub_start(&order, 1);
    spot_answer = 0;
    TaLogic001_CommandSpellUnit(&order, 0x99);
    assert(sub_is(&order, 5, 0x6FF, 0x0700ABCD));
    sub_start(&order, 1);
    spot_answer = 0x1C7;
    TaLogic001_CommandSpellUnit(&order, 0x99);
    assert(sub_is(&order, 5, 0x6FF, 0x07C7ABCD));
    sub_start(&order, 1);
    rec = 4 << 8 | 1;
    TaLogic001_CommandSpellUnit(&order, 0x99);
    assert(sub_is(&order, 5, 0x6FF, 0x01C7ABCD));
    sub_start(&order, 3);
    sub_answer = 1;
    TaLogic001_CommandSpellUnit(&order, 0x99);
    assert(sub_is(&order, 5, 0xBFF, 0x1234ABCD) && was(4, SUB, 0x27d720, 0, 0, 0, 0, 0) && send_order == &order);
    sub_start(&order, 3);
    sub_answer = 2;
    TaLogic001_CommandSpellUnit(&order, 0x99);
    assert(sub_is(&order, 5, 0x5FF, 0x1234ABCD));
    sub_answer = 0, spot_answer = 0;
    slot_answer = NULL;
    set_cells(0);
}

/* TaLogic001_IsUseLifeHourGlass (branch and loop coverage): the total at 1499 and 1500; each need
 * of 0 (also 0x100, read as a byte); counts one below and at the need; the level at and above 5 -
 * need; -1 answers are not counted, 0 is. */
static int e300_n[2], e300_calls;
int Ta_GetSummonablePieceWithOrder(int side, int i, int c)
{
    assert(side == 7 && c == 1 && i == (e300_calls & 7) && have_calls >= 1);
    e300_calls++;
    return i < e300_n[have_calls - 1] ? i * 5 : -1;
}

static int run_gate(int total, int need_a, int have_a, int need_b, int have_b, int level)
{
    have_ids[0] = 2, have_ids[1] = 0;
    have_by[0] = need_a, have_by[1] = need_b;
    e300_n[0] = have_a, e300_n[1] = have_b;
    e300_calls = 0;
    total_answer = total;
    level_answer = level;
    return run_hand(TaLogic001_IsUseLifeHourGlass, 7);
}

static void check_gate(void)
{
    assert(run_gate(1499, 3, 8, 3, 8, 0) == 0 && have_calls == 0 && log_n == 1);
    assert(run_gate(1500, 0, 8, 0, 8, 0) == 0 && have_calls == 2 && e300_calls == 0 && calls_of(LEVEL) == 0);
    assert(run_gate(1500, 0x100, 8, 0x100, 8, 0) == 0 && have_calls == 2 && e300_calls == 0);
    assert(run_gate(1500, 3, 3, 0, 0, 2) == 1 && have_calls == 1 && e300_calls == 8 && was(1, LEVEL, 7, 0, 0, 0, 0, 0));
    assert(run_gate(1500, 0x203, 3, 0, 0, 2) == 1 && have_calls == 1);
    assert(run_gate(1500, 3, 2, 0, 0, 2) == 0 && have_calls == 2 && e300_calls == 8 && calls_of(LEVEL) == 0);
    assert(run_gate(1500, 3, 3, 0, 0, 3) == 0 && have_calls == 2 && calls_of(LEVEL) == 1);
    assert(run_gate(1500, 0, 0, 2, 2, 3) == 1 && have_calls == 2 && e300_calls == 8 && calls_of(LEVEL) == 1);
    assert(run_gate(1500, 0, 0, 2, 1, 3) == 0 && e300_calls == 8 && calls_of(LEVEL) == 0);
    assert(run_gate(1500, 0, 0, 2, 2, 4) == 0 && calls_of(LEVEL) == 1);
    /* the first pass fails on its count, the second accepts: the count starts again at 0 */
    assert(run_gate(1500, 5, 4, 4, 4, 1) == 1 && have_calls == 2 && e300_calls == 16 && calls_of(LEVEL) == 1);
    assert(run_gate(1500, 5, 4, 5, 4, 0) == 0 && e300_calls == 16 && calls_of(LEVEL) == 0);
    total_answer = 0, level_answer = 0;
}

static void check_prepare_more(void)
{
    struct DotrRuleOrder order;
    int n;

    n = check_prepare_e(TaLogic_CommandUnit__0014bde0, 1, 0, 0, 0x5FF, 1);
    assert(pair_e_kind(TaLogic_CommandUnit__0014bde0, n, 0x14c720, 0x14c3b0, -1) && sub_e(TaLogic_CommandUnit__0014bde0, n, 1, 0x14c870, -1));
    assert(key_e(TaLogic_CommandUnit__0014bde0, n, 0x14f9b0, 5) && store_e(TaLogic_CommandUnit__0014bde0, n, 3) && store_e(TaLogic_CommandUnit__0014bde0, n, 4)
        && store_e(TaLogic_CommandUnit__0014bde0, n, 5));

    n = check_prepare_e(TaLogic_CommandUnit__00150270, 1, 1, 0, 0x5FF, 1);
    flag_answer = 1;
    assert(run_prepare_e(TaLogic_CommandUnit__00150270, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 6
        && was(4, SUB, 0x153ab0, 0, 0, 0, 0, 0) && was(5, SEND_A, 3, 3, 0, 0, 0, 0) && send_order == &order
        && order.missions == 0x6666 && order.flags.word == -1);
    flag_answer = 0;
    assert(sub_e(TaLogic_CommandUnit__00150270, n, 0, 0x150850, -1) && sub_e(TaLogic_CommandUnit__00150270, n, 1, 0x150850, -1)
        && sub_e(TaLogic_CommandUnit__00150270, n, 3, 0x150850, -1) && sub_e(TaLogic_CommandUnit__00150270, n, 4, 0x150850, -1));
    assert(key_e(TaLogic_CommandUnit__00150270, n, 0x153ed0, 5) && store_e(TaLogic_CommandUnit__00150270, n, 5));

    n = check_prepare_e(TaLogic_CommandUnit__00135790, 0, 0, 1, 0x5FF, 1);
    word_answer = 189;
    assert(kind_e(TaLogic_CommandUnit__00135790, &order, n, 0, 2) && was(n, WORD, 2, 3, 0, 0, 0, 0)
        && was(n + 1, SUB, 0x136010, -1, 0, 0, 0, 0) && send_order == &order && order.missions == 0x6666);
    word_answer = 190;
    assert(kind_e(TaLogic_CommandUnit__00135790, &order, n, 0, 2) && was(n + 1, SUB, 0x135d50, 1, 0, 0, 0, 0)
        && send_order == &order && order.missions == 0x6666);
    word_answer = 0;
    assert(sub_e(TaLogic_CommandUnit__00135790, n, 1, 0x1360e0, -1) && sub_e(TaLogic_CommandUnit__00135790, n, 3, 0x136010, -1));
    assert(kind_e(TaLogic_CommandUnit__00135790, &order, n, 2, 1) && was(n, SEND_C, 2, 1, 0, 0, 0, 0) && send_order == &order
        && order.missions == 0x6666);
    assert(store_e(TaLogic_CommandUnit__00135790, n, 4) && store_e(TaLogic_CommandUnit__00135790, n, 5));

    n = check_prepare_e(TaLogic_CommandUnit__0013e1d0, 0, 0, 0, 0x5FF, 1);
    assert(pair_e_kind(TaLogic_CommandUnit__0013e1d0, n, 0x13ecf0, 0x13e7b0, 1) && key_e(TaLogic_CommandUnit__0013e1d0, n, 0x141b30, 2));
    sub_answer = 1;
    assert(kind_e(TaLogic_CommandUnit__0013e1d0, &order, n, 3, 1) && was(n, SUB, 0x27d720, 0, 0, 0, 0, 0) && send_order == &order
        && order.missions == 0xBFF);
    sub_answer = 2;
    assert(kind_e(TaLogic_CommandUnit__0013e1d0, &order, n, 3, 1) && send_order == &order && order.missions == 0x5FF);
    sub_answer = 0;
    assert(store_e(TaLogic_CommandUnit__0013e1d0, n, 1) && store_e(TaLogic_CommandUnit__0013e1d0, n, 4) && store_e(TaLogic_CommandUnit__0013e1d0, n, 5));

    n = check_prepare_e(TaLogic_CommandUnit__00118330, 0, 0, 0, 0xFF05, 1);
    assert(sub_e(TaLogic_CommandUnit__00118330, n, 0, 0x1188f0, 1) && sub_e(TaLogic_CommandUnit__00118330, n, 3, 0x118f50, -1)
        && key_e(TaLogic_CommandUnit__00118330, n, 0x11aa10, 3));
    assert(store_e(TaLogic_CommandUnit__00118330, n, 1) && store_e(TaLogic_CommandUnit__00118330, n, 4) && store_e(TaLogic_CommandUnit__00118330, n, 5));

    n = check_prepare_e(TaLogic_CommandUnit__0011cb20, 0, 0, 0, 0xFF05, 1);
    assert(pair_e_kind(TaLogic_CommandUnit__0011cb20, n, 0x11d9b0, 0x11d110, 1) && sub_e(TaLogic_CommandUnit__0011cb20, n, 3, 0x11dc00, -1)
        && key_e(TaLogic_CommandUnit__0011cb20, n, 0x11fa80, 4));
    assert(store_e(TaLogic_CommandUnit__0011cb20, n, 1) && store_e(TaLogic_CommandUnit__0011cb20, n, 4) && store_e(TaLogic_CommandUnit__0011cb20, n, 5));

    /* TaLogic_CommandUnit__00113140 (branch coverage) through a one-argument wrapper passing 0x77: the shared
     * rejections and column 0, then its own 0x27bbc0 != 1 stores and its kinds; kind 2 runs on
     * the real TaLogic_GetWarnHideLvl__00117c30 (2500 & 0xFF) */
    n = check_prepare_e(deep_00113140, 1, 0, 0, 0, 0);
    okc_answer = 2, live_answer = 1;
    assert(run_prepare_e(deep_00113140, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 6
        && was(4, OK_C, 2, 0, 0, 0, 0, 0) && was(5, LIVE, 2, 0, 0, 0, 0, 0) && order.missions == 0xA06
        && order.flags.word == (int)0xFFFF0101);
    live_answer = 2;
    assert(run_prepare_e(deep_00113140, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 6 && order.missions == 0x6FF
        && order.flags.word == 0x0101FFFF);
    okc_answer = 0, live_answer = 0;
    assert(pair_e_kind(deep_00113140, n, 0x113bb0, 0x113760, 1));
    /* kinds 1 and 3 run the real 0x113ed0, which stores 0x5FF without a 0x250740 record */
    slot_answer = NULL;
    assert(kind_e(deep_00113140, &order, n, 1, 3) && was(n + 2, SLOT, 2, 3, 0, 0, 0, 0) && order.missions == 0x5FF);
    assert(kind_e(deep_00113140, &order, n, 3, 3) && was(n + 2, SLOT, 2, 3, 0, 0, 0, 0) && order.missions == 0x5FF);
    assert(store_e(deep_00113140, n, 4) && store_e(deep_00113140, n, 5));
    total_answer = 1200;
    assert(run_prepare_e(deep_00113140, &order, 0x0203, 7, 1, 0, 2 << 5 | 0x1F) == 1 && calls_of(TOTAL) == 1
        && was(log_n - 1, SEND_C, 5, 0, 2500 & 0xFF, 0, 0, 0) && send_order == &order && order.missions == 0x6666
        && order.flags.word == 7);
    /* TaLogic_CommandUnit__00163660 (branch coverage): the shared rejections and column 0 with 1 as the third
     * 0x27bbc0 argument, its 0x103 stores, then each of its six kinds and the default */
    n = check_prepare_e(TaLogic_CommandUnit__00163660, 1, 0, 1, 0, 0);
    okc_answer = 2, live_answer = 1;
    assert(run_prepare_e(TaLogic_CommandUnit__00163660, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 6
        && was(4, OK_C, 2, 0, 1, 0, 0, 0) && was(5, LIVE, 2, 0, 0, 0, 0, 0) && order.missions == 0xA06
        && order.flags.word == (int)0xFFFF0103);
    live_answer = 2;
    assert(run_prepare_e(TaLogic_CommandUnit__00163660, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 6 && order.missions == 0x6FF
        && order.flags.word == 0x0103FFFF);
    okc_answer = 0, live_answer = 0;
    assert(pair_e_kind(TaLogic_CommandUnit__00163660, n, 0x164020, 0x163c50, 1));
    pair_at_answer = 1;
    assert(kind_e(TaLogic_CommandUnit__00163660, &order, n, 5, 2) && was(n, PAIR_AT, 2, 3, 0, 0, 0, 0)
        && was(n + 1, SUB, 0x164020, -1, 0, 0, 0, 0) && send_order == &order);
    pair_at_answer = 0;
    assert(kind_e(TaLogic_CommandUnit__00163660, &order, n, 5, 2) && was(n + 1, SUB, 0x163c50, 1, 0, 0, 0, 0) && send_order == &order);
    assert(sub_e(TaLogic_CommandUnit__00163660, n, 1, 0x164260, -1) && sub_e(TaLogic_CommandUnit__00163660, n, 3, 0x164260, -1)
        && sub_e(TaLogic_CommandUnit__00163660, n, 4, 0x164260, -1));
    assert(kind_e(TaLogic_CommandUnit__00163660, &order, n, 2, 2) && was(n, KEY_E, 0x1676d0, 0x44, 0, 0, 0, 0)
        && was(n + 1, SEND_C, 5, 1, 0xA5, 0, 0, 0) && send_order == &order && order.missions == 0x6666);
    assert(store_e(TaLogic_CommandUnit__00163660, n, 7));
    /* TaLogic_CommandUnit__001271f0 (branch coverage): the shared rejections and column 0 with 0 as the third
     * 0x27bbc0 argument, its bare stores, then each of its six kinds and the default; kinds 1, 3,
     * 4 and 5 run the real 0x127d20, which stores 0x5FF for an unlisted 0x1e1030 word */
    n = check_prepare_e(TaLogic_CommandUnit__001271f0, 0, 0, 0, 0, 0);
    okc_answer = 2, live_answer = 1;
    assert(run_prepare_e(TaLogic_CommandUnit__001271f0, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 5
        && was(3, OK_C, 2, 0, 0, 0, 0, 0) && was(4, LIVE, 2, 0, 0, 0, 0, 0) && order.missions == 0xA06
        && order.flags.word == (int)0xFFFF0000);
    live_answer = 2;
    assert(run_prepare_e(TaLogic_CommandUnit__001271f0, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 5 && order.missions == 0x6FF
        && order.flags.word == 0x0000FFFF);
    okc_answer = 0, live_answer = 0;
    assert(pair_e_kind(TaLogic_CommandUnit__001271f0, n, 0x127b00, 0x1277c0, 1));
    assert(key_e(TaLogic_CommandUnit__001271f0, n, 0x12b1b0, 5));
    word_answer = 0;
    assert(kind_e(TaLogic_CommandUnit__001271f0, &order, n, 1, 3) && was(n + 2, WORD, 2, 3, 0, 0, 0, 0) && order.missions == 0x5FF);
    assert(kind_e(TaLogic_CommandUnit__001271f0, &order, n, 3, 3) && was(n + 2, WORD, 2, 3, 0, 0, 0, 0) && order.missions == 0x5FF);
    assert(kind_e(TaLogic_CommandUnit__001271f0, &order, n, 4, 3) && was(n + 2, WORD, 2, 3, 0, 0, 0, 0) && order.missions == 0x5FF);
    assert(kind_e(TaLogic_CommandUnit__001271f0, &order, n, 5, 3) && was(n + 2, WORD, 2, 3, 0, 0, 0, 0) && order.missions == 0x5FF);
    assert(store_e(TaLogic_CommandUnit__001271f0, n, 7));
    /* TaLogic_CommandUnit__0010ce20 (branch coverage) through a wrapper that supplies the set and
     * checks the nibble store: the shared rejections and column 0, its 1 / 0x10000 stores, each of
     * its six kinds and the default; kind 1 runs the real 0x10dc40, which returns without a
     * 0x250740 record */
    n = check_prepare_e(set_0010ce20, 0, 0, 0, 0, 0);
    okc_answer = 2, live_answer = 1;
    assert(run_prepare_e(set_0010ce20, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 5
        && was(3, OK_C, 2, 0, 0, 0, 0, 0) && was(4, LIVE, 2, 0, 0, 0, 0, 0) && order.missions == 0xA06
        && order.flags.word == (int)0xFFFF0001);
    live_answer = 2;
    assert(run_prepare_e(set_0010ce20, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 5 && order.missions == 0x6FF
        && order.flags.word == 0x0001FFFF);
    okc_answer = 0, live_answer = 0;
    assert(pair_e_kind(set_0010ce20, n, 0x10d880, 0x10d490, 1));
    slot_answer = NULL;
    assert(kind_e(set_0010ce20, &order, n, 1, 2) && was(n, DFW, 2, 3, 0, 0, 0, 0) && was(n + 1, SLOT, 2, 3, 0, 0, 0, 0)
        && order.missions == 0x6666);
    assert(key_e(set_0010ce20, n, 0x10eda0, 2));
    assert(sub_e(set_0010ce20, n, 3, 0x10db00, 2) && sub_e(set_0010ce20, n, 4, 0x10db00, 2)
        && sub_e(set_0010ce20, n, 5, 0x10db00, 1) && store_e(set_0010ce20, n, 7));
    /* TaLogic_CommandUnit__0012c1a0 (branch coverage) through a one-argument wrapper passing 0x77: the shared
     * rejections and column 0; with 0x27bbc0 != 1 the 0x1e1090 word's high byte decides (2 keeps
     * the 0x50A / 0x5FF stores, 1 gives 0x60A / 0x6FF with the second 0x1e1090 word in the high
     * half, asked for 1 when the 0x250360 key is 0 and for 0 otherwise); kinds 0, 2, 3, others */
    n = check_prepare_e(deep_0012c1a0, 0, 0, 0, 0, 0);
    okc_answer = 2, live_answer = 1, cell_by[0] = 0x200;
    assert(run_prepare_e(deep_0012c1a0, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 6
        && was(3, OK_C, 2, 0, 0, 0, 0, 0) && was(4, CELL, 2, 0, 0, 0, 0, 0) && was(5, LIVE, 2, 0, 0, 0, 0, 0)
        && order.missions == 0x50A && order.flags.word == -1);
    live_answer = 0;
    assert(run_prepare_e(deep_0012c1a0, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 6 && order.missions == 0x5FF);
    live_answer = 1, cell_by[0] = 0x1FF;
    assert(run_prepare_e(deep_0012c1a0, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 7
        && was(5, LIVE, 2, 0, 0, 0, 0, 0) && was(6, CELL, 0, 0, 0, 0, 0, 0) && order.missions == 0x60A
        && order.flags.word == 0x01FFFFFF);
    live_answer = 2;
    assert(run_prepare_e(deep_0012c1a0, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 7
        && was(6, CELL, 0, 0, 0, 0, 0, 0) && order.missions == 0x6FF && order.flags.word == 0x01FFFFFF);
    pair_e = 0;
    assert(run_prepare_e(deep_0012c1a0, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 7
        && was(6, CELL, 1, 0, 0, 0, 0, 0) && order.missions == 0x6FF);
    live_answer = 1;
    assert(run_prepare_e(deep_0012c1a0, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 7
        && was(6, CELL, 1, 0, 0, 0, 0, 0) && order.missions == 0x60A);
    pair_e = 0x44;
    okc_answer = 0, live_answer = 0, cell_by[0] = 0;
    assert(pair_e_kind(deep_0012c1a0, n, 0x12cbe0, 0x12c7f0, 1));
    assert(kind_e(deep_0012c1a0, &order, n, 2, 2) && was(n, KEY_E, 0x130780, 0x44, 0, 0, 0, 0)
        && was(n + 1, SEND_C, 5, 0, 0xA5, 0, 0, 0) && send_order == &order && order.missions == 0x6666);
    assert(kind_e(deep_0012c1a0, &order, n, 3, 1) && was(n, SUB, 0x12ce50, -1, 0, 0, 0, 0) && send_order == &order
        && order.missions == 0x6666);
    assert(store_e(deep_0012c1a0, n, 1) && store_e(deep_0012c1a0, n, 4) && store_e(deep_0012c1a0, n, 7));
    /* TaLogic_CommandUnit__00146ec0 (branch coverage): the shared rejections and column 0 with the
     * bare 0xA05 / 0x5FF stores; kind 0 on the real TaLogic013_IsUnitInvader (word 273 accepted, 0
     * not) and then by 0x2507d0 */
    n = check_prepare_e(deep_00146ec0, 1, 0, 0, 0x5FF, 1);
    word_answer = 273;
    assert(kind_e(deep_00146ec0, &order, n, 0, 2) && was(n, WORD, 2, 3, 0, 0, 0, 0)
        && was(n + 1, SUB, 0x147830, -1, 0, 0, 0, 0) && send_order == &order && order.missions == 0x6666);
    word_answer = 0, pair_at_answer = 1;
    /* the real TaLogic013_CommandEfcMonster (word not 291) passes the order on to 0x1474f0 after
     * its two reads */
    assert(kind_e(deep_00146ec0, &order, n, 0, 5) && was(n + 1, PAIR_AT, 2, 3, 0, 0, 0, 0)
        && was(n + 3, WORD, 2, 3, 0, 0, 0, 0) && was(n + 4, SUB, 0x1474f0, -1, 0, 0, 0, 0) && send_order == &order
        && order.missions == 0x6666);
    pair_at_answer = 0;
    assert(kind_e(deep_00146ec0, &order, n, 0, 3) && was(n + 1, PAIR_AT, 2, 3, 0, 0, 0, 0)
        && was(n + 2, SUB, 0x1474f0, -1, 0, 0, 0, 0) && send_order == &order && order.missions == 0x6666);
    assert(sub_e(deep_00146ec0, n, 1, 0x147b90, -1) && sub_e(deep_00146ec0, n, 3, 0x147b90, -1)
        && key_e(deep_00146ec0, n, 0x14b330, 5));
    assert(store_e(deep_00146ec0, n, 4) && store_e(deep_00146ec0, n, 5));

    /* TaLogic_CommandUnit__00139d90 (branch coverage): the shared rejections and column 0; 0x27bbc0
     * not 1 puts the leading 0x1e1090 word into the flag word, by 0x1de6d0 and by the low byte of
     * the second argument (0x77, then 0x301) */
    n = check_prepare_e(deep_00139d90, 1, 0, 0, 0, 0);
    cell_by[0] = 0x1C7;
    okc_answer = 2, live_answer = 1;
    assert(run_prepare_e(deep_00139d90, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 6
        && was(0, CELL, 2, 0, 0, 0, 0, 0) && order.missions == 0xA06 && order.flags.word == (int)0xFFFF01C7);
    assert(run_prepare_e(one_00139d90, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 6
        && order.missions == 0xA06 && order.flags.word == (int)0xFFFF02C7);
    live_answer = 2;
    assert(run_prepare_e(deep_00139d90, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 6
        && order.missions == 0x6FF && order.flags.word == 0x01C7FFFF);
    assert(run_prepare_e(one_00139d90, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 6
        && order.missions == 0x6FF && order.flags.word == 0x02C7FFFF);
    cell_by[0] = 0;
    okc_answer = 0, live_answer = 0;
    assert(sub_e(deep_00139d90, n, 0, 0x13a400, 1) && key_e(deep_00139d90, n, 0x13da70, 2));
    /* kinds 1 and 3 run the real 0x13aa40: 0x5FF after a refused real 0x13d2c0, and 0x3FF */
    pack_by[0] = -1, pack_by[1] = -1;
    assert(kind_e(deep_00139d90, &order, n, 1, 7) && log_id[n + 6] == PACK
        && order.missions == 0x5FF);
    assert(kind_e(deep_00139d90, &order, n, 3, 4) && was(n + 3, CELL, 0x44, 0, 0, 0, 0, 0) && order.missions == 0x3FF);
    assert(store_e(deep_00139d90, n, 4) && store_e(deep_00139d90, n, 5));
    n = check_prepare_e(TaLogic_CommandUnit__00131540, 0, 0, 0, 0, 0);
    /* 0x27bbc0 not 1: the SzDuel_GetUnitLoc(key == 0, 0) byte goes into the flag word, by 0x1de6d0
     * and by key */
    cell_by[0] = 0x1C7;
    okc_answer = 2, live_answer = 1;
    assert(run_prepare_e(TaLogic_CommandUnit__00131540, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 6
        && was(4, LIVE, 2, 0, 0, 0, 0, 0) && was(5, CELL, 0, 0, 0, 0, 0, 0) && order.missions == 0xA06
        && order.flags.word == (int)0xFFFF00C7);
    live_answer = 2;
    assert(run_prepare_e(TaLogic_CommandUnit__00131540, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 6
        && was(5, CELL, 0, 0, 0, 0, 0, 0) && order.missions == 0x6FF && order.flags.word == 0x00C7FFFF);
    pair_e = 0;
    assert(run_prepare_e(TaLogic_CommandUnit__00131540, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 6
        && was(5, CELL, 1, 0, 0, 0, 0, 0) && order.missions == 0x6FF && order.flags.word == 0x00C7FFFF);
    live_answer = 1;
    assert(run_prepare_e(TaLogic_CommandUnit__00131540, &order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 6
        && was(5, CELL, 1, 0, 0, 0, 0, 0) && order.missions == 0xA06 && order.flags.word == (int)0xFFFF00C7);
    pair_e = 0x44;
    cell_by[0] = 0;
    okc_answer = 0, live_answer = 0;
    assert(sub_e(TaLogic_CommandUnit__00131540, n, 0, 0x131b30, 1) && sub_e(TaLogic_CommandUnit__00131540, n, 1, 0x132280, -1)
        && sub_e(TaLogic_CommandUnit__00131540, n, 3, 0x132280, -1) && key_e(TaLogic_CommandUnit__00131540, n, 0x134ae0, 4));
    assert(store_e(TaLogic_CommandUnit__00131540, n, 4) && store_e(TaLogic_CommandUnit__00131540, n, 5));
}

/* TaLogic_CommandUnit__0016d6a0 (branch coverage): the three rejections, column 0 with the flag
 * word other than -1 and the four 0x27bbc0 / 0x1de6d0 outcomes, kind 0 with 0x2507d0 == 1, with the
 * 0x1e1030 word at 53, 54, 57 and 58, kind 2, kinds 1, 3 and 4, and kind 5 (default). Every callee
 * is a stub. */
enum { KEY_F = 140, ORDER_H, ORDER_I, ORDER_J };
void TaLogic022_CommandEfcMonster(struct DotrRuleOrder *order) { note(ORDER_H, 0, 0, 0, 0, 0, 0); send_order = order; }
void TaLogic022_CommandBtlUnit(struct DotrRuleOrder *order) { note(ORDER_I, 0, 0, 0, 0, 0, 0); send_order = order; }
void TaLogic022_CommandSpellUnit(struct DotrRuleOrder *order) { note(ORDER_J, 0, 0, 0, 0, 0, 0); send_order = order; }
int func_00172310_key(int key) { note(KEY_F, key, 0, 0, 0, 0, 0); return key_answer; }

static int run_prepare_f(struct DotrRuleOrder *order, int position, int word, int dfw, int bit, int kind)
{
    memset(order, 0, sizeof *order);
    order->position = (unsigned short)position;
    order->missions = 0x6666;
    order->flags.word = word;
    reset();
    memset(dfw_by, 0, sizeof dfw_by);
    dfw_by[position & 127] = dfw;
    bit_answer = bit;
    kind_answer = kind;
    pair_answer = 0x44;
    key_answer = 0x1A5;
    send_order = NULL;
    return TaLogic_CommandUnit__0016d6a0(order);
}

static void check_prepare_f(void)
{
    struct DotrRuleOrder order;
    int kind;
    int word;

    word_answer = 0;
    assert(run_prepare_f(&order, 0x0203, 0, 0, 0, 0) == 0 && log_n == 4);
    assert(was(0, WORD, 2, 3, 0, 0, 0, 0) && was(1, CELL, 2, 3, 0, 0, 0, 0) && was(2, PAIR, 2, 3, 0, 0, 0, 0)
        && was(3, DFW, 2, 3, 0, 0, 0, 0));
    assert(run_prepare_f(&order, 0x0203, 0, 1, 1, 0) == 0 && log_n == 5 && was(4, BIT, 2, 3, 0, 0, 0, 0));
    assert(run_prepare_f(&order, 0xFFFF, 0, 1, 0, 0) == 0 && log_n == 5 && order.missions == 0x6666);
    /* column 0 */
    assert(run_prepare_f(&order, 0x0200, -2, 1, 0, 0) == 1 && log_n == 5 && order.missions == 0x6666);
    okc_answer = 1, live_answer = 1;
    assert(run_prepare_f(&order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 7 && was(5, OK_C, 2, 0, 0, 0, 0, 0)
        && was(6, LIVE, 2, 0, 0, 0, 0, 0) && order.missions == 0x50A && order.flags.word == -1);
    live_answer = 0;
    assert(run_prepare_f(&order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 7 && order.missions == 0x5FF);
    okc_answer = 2, live_answer = 1;
    assert(run_prepare_f(&order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 7 && was(6, LIVE, 2, 0, 0, 0, 0, 0)
        && order.missions == 0xA05);
    live_answer = 2;
    assert(run_prepare_f(&order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 7 && order.missions == 0x5FF);
    live_answer = 0;
    /* kind 0: 0x2507d0 == 1, or the 0x1e1030 word in 54..57, or neither */
    pair_at_answer = 1;
    assert(run_prepare_f(&order, 0x0203, 7, 1, 0, 0x1F) == 1 && log_n == 8 && was(5, KIND, 2, 3, 0, 0, 0, 0)
        && was(6, PAIR_AT, 2, 3, 0, 0, 0, 0) && was(7, ORDER_H, 0, 0, 0, 0, 0, 0) && send_order == &order
        && order.missions == 0x6666);
    pair_at_answer = 0;
    for (word = 53; word <= 58; word++) {
        word_answer = word;
        assert(run_prepare_f(&order, 0x0203, 7, 1, 0, 0) == 1 && log_n == 8
            && was(7, word >= 54 && word <= 57 ? ORDER_H : ORDER_I, 0, 0, 0, 0, 0, 0)
            && send_order == &order && order.missions == 0x6666);
    }
    word_answer = 0;
    /* kind 2 */
    assert(run_prepare_f(&order, 0x0203, 7, 1, 0, 0x40) == 1 && log_n == 8 && was(6, KEY_F, 0x44, 0, 0, 0, 0, 0)
        && was(7, SEND_C, 5, 0, 0xA5, 0, 0, 0) && send_order == &order);
    /* kinds 1, 3 and 4 */
    for (kind = 0x20; kind <= 0x80; kind += kind == 0x20 ? 0x40 : 0x20) {
        assert(run_prepare_f(&order, 0x0203, 7, 1, 0, kind) == 1 && log_n == 7 && was(6, ORDER_J, 0, 0, 0, 0, 0, 0)
            && send_order == &order && order.missions == 0x6666);
    }
    /* kind 5 (default): the +4 store only */
    assert(run_prepare_f(&order, 0x0203, 7, 1, 0, 0xA0) == 1 && log_n == 6 && order.missions == 0x5FF
        && order.flags.word == 7 && send_order == NULL);
}

/* TaLogic_CommandUnit__001547a0 (branch coverage): the three rejections; column 0 with the flag
 * word other than -1, the two 0x27bbc0 == 1 stores, a negative 0x1e3790 answer, and each refusal of
 * the answer (the 0x1e1090 word, the 0x250360 key, the position itself) and its acceptance, each
 * for both 0x1de6d0 outcomes; kind 0 by 0x2507d0 and by 0x1587e0, kinds 1 to 4, and kind 5
 * (default). Every callee is a stub. */
enum { GRID_G = 150, ORDER_K, ORDER_L, ORDER_M };
static int grid_g;
int SzDuel_GetMapUnitID(int x, int y) { note(GRID_G, x, y, 0, 0, 0, 0); return grid_g; }
void TaLogic017_CommandEfcMonster(struct DotrRuleOrder *order) { note(ORDER_K, 0, 0, 0, 0, 0, 0); send_order = order; }
void TaLogic017_CommandBtlUnit(struct DotrRuleOrder *order) { note(ORDER_L, 0, 0, 0, 0, 0, 0); send_order = order; }
void TaLogic017_CommandSpellUnit(struct DotrRuleOrder *order) { note(ORDER_M, 0, 0, 0, 0, 0, 0); send_order = order; }

static int run_prepare_g(struct DotrRuleOrder *order, int position, int word, int dfw, int bit, int kind)
{
    memset(order, 0, sizeof *order);
    order->position = (unsigned short)position;
    order->missions = 0x6666;
    order->flags.word = word;
    reset();
    memset(dfw_by, 0, sizeof dfw_by);
    dfw_by[position & 127] = dfw;
    bit_answer = bit;
    kind_answer = kind;
    pair_answer = 0x44;
    send_order = NULL;
    return TaLogic_CommandUnit__001547a0(order);
}

/* Column 0 at position 0x0100 with 0x27bbc0 not 1: the stores for one 0x1e3790 answer. */
static int col_g(int grid, int cell_word, int other_key, int live, int n, int half_04, unsigned int flags)
{
    struct DotrRuleOrder order;
    int r;

    col_mode = 1;
    pair_for[1][0] = 0x44, pair_for[0][3] = other_key, bit_for[1][0] = 0;
    cell_by[0] = cell_word;
    grid_g = grid;
    okc_answer = 0, live_answer = live;
    r = run_prepare_g(&order, 0x0100, -1, 1, 0, 0);
    col_mode = 0;
    cell_by[0] = 0;
    return r == 1 && log_n == n && was(4, OK_C, 1, 0, 0, 0, 0, 0) && was(5, GRID_G, 3, 5, 0, 0, 0, 0)
        && was(n - 1, LIVE, 1, 0, 0, 0, 0, 0) && order.missions == half_04 && (unsigned int)order.flags.word == flags;
}

static void check_prepare_g(void)
{
    struct DotrRuleOrder order;
    int kind;

    assert(run_prepare_g(&order, 0x0203, 0, 0, 0, 0) == 0 && log_n == 3);
    assert(was(0, CELL, 2, 3, 0, 0, 0, 0) && was(1, PAIR, 2, 3, 0, 0, 0, 0) && was(2, DFW, 2, 3, 0, 0, 0, 0));
    assert(run_prepare_g(&order, 0x0203, 0, 1, 1, 0) == 0 && log_n == 4 && was(3, BIT, 2, 3, 0, 0, 0, 0));
    assert(run_prepare_g(&order, 0xFFFF, 0, 1, 0, 0) == 0 && log_n == 4 && order.missions == 0x6666);
    /* column 0 */
    assert(run_prepare_g(&order, 0x0200, -2, 1, 0, 0) == 1 && log_n == 4 && order.missions == 0x6666);
    okc_answer = 1, live_answer = 1;
    assert(run_prepare_g(&order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 6 && was(4, OK_C, 2, 0, 0, 0, 0, 0)
        && was(5, LIVE, 2, 0, 0, 0, 0, 0) && order.missions == 0x50A && order.flags.word == -1);
    live_answer = 0;
    assert(run_prepare_g(&order, 0x0200, -1, 1, 0, 0) == 1 && log_n == 6 && order.missions == 0x5FF
        && order.flags.word == -1);
    /* negative 0x1e3790 answer */
    assert(col_g(-1, 0x403, 0x44, 1, 7, 0xA06, 0xFFFF0403) && col_g(-1, 0x403, 0x44, 0, 7, 0x6FF, 0x0403FFFF));
    /* answer 0 counts; the position's word is not 0x403 */
    assert(col_g(0, 0x402, 0x44, 1, 8, 0xA06, 0xFFFF0403) && col_g(0x0003, 0x404, 0x44, 0, 8, 0x6FF, 0x0403FFFF));
    assert(was(6, CELL, 1, 0, 0, 0, 0, 0));
    /* another key */
    assert(col_g(0x0003, 0x403, 0x45, 1, 9, 0xA06, 0xFFFF0403) && col_g(0x0003, 0x403, 0x43, 0, 9, 0x6FF, 0x0403FFFF));
    assert(was(7, PAIR, 0, 3, 0, 0, 0, 0));
    /* the position itself */
    assert(col_g(0x0100, 0x403, 0x44, 1, 9, 0xA06, 0xFFFF0403) && col_g(0x0100, 0x403, 0x44, 0, 9, 0x6FF, 0x0403FFFF));
    assert(was(7, PAIR, 1, 0, 0, 0, 0, 0));
    /* accepted */
    assert(col_g(0x0003, 0x403, 0x44, 1, 9, 0x20A, 0x0003FFFF) && col_g(0x0003, 0x403, 0x44, 0, 9, 0x2FF, 0x0003FFFF));
    live_answer = 0;
    /* kind 0: 0x2507d0 == 1, else 0x1587e0 of the 0x1e1030 word == 1 or not */
    word_answer = 0x155;
    pair_at_answer = 1, id54_answer = 0;
    assert(run_prepare_g(&order, 0x0203, 7, 1, 0, 0x1F) == 1 && log_n == 8 && was(4, KIND, 2, 3, 0, 0, 0, 0)
        && was(5, WORD, 2, 3, 0, 0, 0, 0) && was(6, PAIR_AT, 2, 3, 0, 0, 0, 0) && was(7, ORDER_K, 0, 0, 0, 0, 0, 0)
        && send_order == &order && order.missions == 0x6666);
    pair_at_answer = 0, id54_answer = 1;
    assert(run_prepare_g(&order, 0x0203, 7, 1, 0, 0) == 1 && log_n == 9 && was(7, ID54, 0x155, 0, 0, 0, 0, 0)
        && was(8, ORDER_K, 0, 0, 0, 0, 0, 0) && send_order == &order);
    pair_at_answer = 2, id54_answer = 0;
    assert(run_prepare_g(&order, 0x0203, 7, 1, 0, 0) == 1 && log_n == 9 && was(7, ID54, 0x155, 0, 0, 0, 0, 0)
        && was(8, ORDER_L, 0, 0, 0, 0, 0, 0) && send_order == &order && order.missions == 0x6666);
    pair_at_answer = 0;
    word_answer = 0;
    /* kinds 1 to 4 */
    for (kind = 0x20; kind <= 0x80; kind += 0x20) {
        assert(run_prepare_g(&order, 0x0203, 7, 1, 0, kind) == 1 && log_n == 7 && was(6, ORDER_M, 0, 0, 0, 0, 0, 0)
            && send_order == &order && order.missions == 0x6666);
    }
    /* kind 5 (default): the +4 store only */
    assert(run_prepare_g(&order, 0x0203, 7, 1, 0, 0xA0) == 1 && log_n == 6 && order.missions == 0x5FF
        && order.flags.word == 7 && send_order == NULL);
}

/* 0x116b60: the pair filler, counter, scorer and 0x2464f0 are stubs; the key
 * callee is the real one of the copy under test. */
static struct DotrRuleNibbles near_fill[12];
static int near_count, near_scores[12], near_score_n, near_live, near_side, near_key, near_spot, near_fills;
int func_002464f0_rule(int item, int x, int y)
{
    assert(item == 0x51 && x == 0x03 && y == 0x04);
    return near_live;
}
int func_0027a090_rule(int a, int b, struct DotrRuleNibbles *cells, int d, int e, int f, int g, int h, int i,
                       unsigned char j, unsigned short k)
{
    int n;

    assert(a == 3 && b == 5 && d == 0x03 && e == 0x04 && f == 0x02 && g == 0x03 && h == near_spot && i == 0);
    assert(j == 30 && k == near_key);
    for (n = 0; n < 12; n++) {
        assert(cells[n].low == 15 && cells[n].high == 15);
        cells[n] = near_fill[n];
    }
    near_fills++;
    return 0;
}
int func_0027afc0_rule(struct DotrRuleNibbles *cells)
{
    assert(cells[0].low == near_fill[0].low && cells[11].high == near_fill[11].high);
    return near_count;
}
static int near_fourth = 3;
int TaLogic_GetUnitWarning(int a, int b, int c, int d, int e, unsigned char f)
{
    assert(near_score_n < near_count && c == near_side && d == near_fourth && e == 1 && f == (near_key & 0xff));
    while (item_by[near_fill[near_score_n].low][near_fill[near_score_n].high]) {
        near_score_n++;
    }
    assert(a == near_fill[near_score_n].low && b == near_fill[near_score_n].high);
    return near_scores[near_score_n++];
}

typedef int near_fn(struct DotrRuleOrder *, int, struct DotrRuleResult *, unsigned char);

static int run_near(near_fn *f, int mode, int spot, int live, int okc, int count)
{
    memset(&found_order, 0, sizeof found_order);
    memset(&found_result, 0x77, sizeof found_result);
    memset(dfw_by, 0, sizeof dfw_by);
    found_order.position = 0x0305;
    found_order.flags.word = mode == 1 ? (int)((unsigned)spot << 16 | 0x0666) : (0x06660000 | spot);
    dfw_by[5] = 0x51, dfw_by[2] = 1, dfw_by[127] = 1;
    set_cells(0x0302);
    cell_by[5] = 0x0403;
    near_spot = spot, near_live = live, okc_answer = okc, near_count = count;
    near_score_n = near_fills = 0;
    near_side = 1;
    total_answer = 0;
    pick_answer = 0x1c4;
    done_order = NULL;
    item_calls = 0;
    log_n = 0;
    return f(&found_order, near_side, &found_result, (unsigned char)mode);
}

/* near_none_flag: the flag passed when no entry was chosen (0 for 0x120150 only). */
static int near_none_flag = 1;
static void near_fallback_flag(int flag)
{
    assert(found_result.position == 0x0305 && found_result.kind == 6 && found_result.loc == 0x0403);
    assert(found_result.atkDefPosition == 0xc4 && calls_of(PICK) == 1 && was(log_n - 1, PICK, 3, 5, 0x0403, flag, 0, 0));
    assert(found_result.unknown_03 == 0x77 && found_result.unknown_06 == 0x7777 && calls_of(DONE) == 0);
}

static void near_fallback(void)
{
    near_fallback_flag(1);
}

static void near_set(int n, const int (*at)[3])
{
    int i;

    memset(near_fill, 0xff, sizeof near_fill);
    memset(grid_by, 0, sizeof grid_by);
    memset(item_by, 0, sizeof item_by);
    for (i = 0; i < n; i++) {
        near_fill[i].low = at[i][0];
        near_fill[i].high = at[i][1];
        near_scores[i] = at[i][2];
        grid_by[at[i][0]][at[i][1]] = 1;
    }
}

static void near_picks(near_fn *f, int n, const int (*at)[3], int want)
{
    near_set(n, at);
    assert(run_near(f, 0, 0x0402, 1, 0, n) == 0 && near_fills == 1 && item_calls == n && near_score_n == n);
    assert(found_result.position == 0x0305 && found_result.kind == 6 && found_result.loc == want);
    assert(found_result.atkDefPosition == 0xc4 && calls_of(PICK) == 1 && was(log_n - 1, PICK, 3, 5, want, 0, 0, 0));
}

static void check_near(near_fn *f, int key)
{
    /* the spot's cell is (low 2, high 3); entries are { low, high, score } */
    static const int one[1][3] = { { 6, 1, 1 } };
    static const int higher[3][3] = { { 1, 1, 2 }, { 2, 3, 3 }, { 6, 6, 1 } };
    static const int nearer_high[2][3] = { { 2, 5, 4 }, { 2, 2, 4 } };
    static const int nearer_low[2][3] = { { 0, 3, 4 }, { 3, 3, 4 } };
    static const int farther[3][3] = { { 3, 3, 4 }, { 0, 3, 4 }, { 2, 6, 4 } };
    static const int level[2][3] = { { 1, 3, 4 }, { 2, 4, 4 } };
    static const int below[2][3] = { { 5, 5, 4 }, { 2, 3, -1 } };
    int mode;

    near_key = key;
    near_set(1, one);
    /* no word at the spot: the upper half for mode 1, the low halfword otherwise */
    for (mode = 0; mode < 3; mode++) {
        assert(run_near(f, mode, 0x0402, 1, 0, 1) == 0 && was(0, DFW, 4, 2, 0, 0, 0, 0));
        dfw_by[2] = 0;
        log_n = 0;
        memset(&found_result, 0x77, sizeof found_result);
        assert(f(&found_order, 1, &found_result, (unsigned char)mode) == -1 && log_n == 2 && was(0, DFW, 4, 2, 0, 0, 0, 0));
        assert(was(1, DONE, 1, 0, 0, 0, 0, 0) && done_order == &found_order && found_result.kind == 0x77);
    }
    /* a spot of 0xffff */
    for (mode = 0; mode < 2; mode++) {
        assert(run_near(f, mode, 0xffff, 1, 0, 1) == 0 && was(0, DFW, 0xff, 0xff, 0, 0, 0, 0) && near_fills == 0);
        near_fallback();
        assert(calls_of(DFW) == 1 && calls_of(CELL) == 2);
    }
    /* 0x2464f0 answers 0; 0x27bbc0 answers 1 */
    assert(run_near(f, 0, 0x0402, 0, 0, 1) == 0 && near_fills == 0 && calls_of(OK_C) == 0);
    near_fallback();
    assert(run_near(f, 0, 0x0402, 1, 1, 1) == 0 && near_fills == 0 && was(log_n - 2, OK_C, 3, 5, 0, 0, 0, 0));
    near_fallback();
    /* no entries counted (0 and negative) */
    assert(run_near(f, 0, 0x0402, 1, 2, 0) == 0 && near_fills == 1 && item_calls == 0);
    near_fallback();
    assert(run_near(f, 0, 0x0402, 1, 0, -1) == 0 && near_fills == 1 && item_calls == 0);
    near_fallback();
    /* every entry holds an item: nothing chosen */
    item_by[6][1] = 1;
    assert(run_near(f, 0, 0x0402, 1, 0, 1) == 0 && item_calls == 1 && near_score_n == 0);
    near_fallback_flag(near_none_flag);
    /* one free entry; mode 1 passes the upper half as the spot */
    near_picks(f, 1, one, 0x0106);
    assert(run_near(f, 1, 0x0402, 1, 0, 1) == 0 && found_result.loc == 0x0106);
    /* a higher score wins and a lower one after it does not */
    near_picks(f, 3, higher, 0x0302);
    /* equal scores: the nearer entry wins on either axis, a farther or equally far one does not */
    near_picks(f, 2, nearer_high, 0x0202);
    near_picks(f, 2, nearer_low, 0x0303);
    near_picks(f, 3, farther, 0x0303);
    near_picks(f, 2, level, 0x0301);
    /* a score below the best, and an occupied entry between two free ones */
    near_picks(f, 2, below, 0x0505);
    near_set(3, higher);
    item_by[2][3] = 1;
    assert(run_near(f, 0, 0x0402, 1, 0, 3) == 0 && item_calls == 3 && found_result.loc == 0x0101);
    memset(grid_by, 0, sizeof grid_by);
    memset(item_by, 0, sizeof item_by);
}

/* TaLogic_GetMinimumTask_ReviveMission__00171d60 (branch coverage). */
enum { ENTRY_Z = 160, FIND_Z };
static unsigned entry_z_word;
static int entry_z_null;
unsigned *SzDuel_GetUnitEffect(int a, int b, int n) { note(ENTRY_Z, a, b, n, 0, 0, 0); return entry_z_null ? NULL : &entry_z_word; }
int TaLogicCom_IsUseRevive(int side, int a, int b) { note(FIND_Z, side, a, b, 0, 0, 0); return find_answer; }

static int run_entry_rule(struct DotrRuleOrder *order, struct DotrRuleResult *result)
{
    memset(result, 0, sizeof(*result));
    result->loc = 0x4321;
    log_n = 0, grid_calls = 0;
    done_order = NULL, rest_order = NULL, rest_result = NULL;
    return TaLogic_GetMinimumTask_ReviveMission__00171d60(order, 5, result, 9);
}

static void check_entry_rule(void)
{
    struct DotrRuleOrder order;
    struct DotrRuleResult result;

    memset(&order, 0, sizeof(order));
    order.position = 0x0703;
    list_mode = 0, col_mode = 0;
    memset(grid_by, 0, sizeof(grid_by));
    cell_by[3] = 0x0204;
    rest_answer = 77, pick_answer = 0x5a, word_answer = 0x21, around_answer = 4, bit_answer = 0;
    /* kind 0, no entry: straight to TaLogic_GetMinimumTask_RunMission__00170940 (entry 3 asked, no
     * bit query) */
    kind_answer = 0x1f, entry_z_null = 1;
    assert(run_entry_rule(&order, &result) == 77 && log_n == 4);
    assert(was(0, KIND, 7, 3, 0, 0, 0, 0) && was(1, ENTRY_Z, 7, 3, 3, 0, 0, 0) && was(2, CELL, 7, 3, 0, 0, 0, 0));
    assert(was(3, REST_B, 5, 9, 0, 0, 0, 0) && rest_order == &order && rest_result == &result && result.kind == 0);
    /* kind 0, entry, negative find: 0x170940; neither 0x1e22b0 nor 0x27d830 asked */
    entry_z_null = 0, entry_z_word = 0xffffff00 | 2 << 6 | 0x2b, find_answer = -1;
    assert(run_entry_rule(&order, &result) == 77 && log_n == 5 && grid_calls == 0);
    assert(was(3, FIND_Z, 5, 2, 0x2b, 0, 0, 0) && was(4, REST_B, 5, 9, 0, 0, 0, 0));
    /* kind 0, find 0 (boundary): kind-12 result with the side in loc */
    find_answer = 0x00030000 | 0x1ee;
    assert(run_entry_rule(&order, &result) == 0 && log_n == 7 && grid_calls == 0);
    assert(was(4, WORD_D, 3, 0xee, 0x2b, 0, 0, 0) && was(5, PICK, 7, 3, 0x4321, 1, 0, 0) && was(6, DONE, 9, 0, 0, 0, 0, 0));
    assert(result.position == 0x0703 && result.kind == 12 && result.atkDefPosition == 0x5a && result.shouldFlipUp == 1);
    assert(result.loc == (5 << 8 | 0x21) && done_order == &order);
    find_answer = 0;
    assert(run_entry_rule(&order, &result) == 0 && was(4, WORD_D, 0, 0, 0x2b, 0, 0, 0));
    /* entry bits 6-7 of 3: the found index instead of the side */
    entry_z_word = 3 << 6 | 0x01, find_answer = 0x00030000;
    assert(run_entry_rule(&order, &result) == 0 && was(3, FIND_Z, 5, 3, 1, 0, 0, 0));
    assert(result.loc == (3 << 8 | 0x21));
    /* non-zero kind: entry 0; both gates pass */
    kind_answer = 1 << 5;
    assert(run_entry_rule(&order, &result) == 0 && was(1, ENTRY_Z, 7, 3, 0, 0, 0, 0) && grid_calls == 1);
    assert(was(4, AROUND, 7, 3, 0, 0, 6, 0) && was(5, WORD_D, 3, 0, 1, 0, 0, 0) && result.kind == 12);
    /* SzDuel_GetField(low byte, rest) answers 9: find dropped, bit clear: 0x170940 */
    grid_by[4][2] = 9;
    assert(run_entry_rule(&order, &result) == 77 && calls_of(WORD_D) == 0 && calls_of(AROUND) == 1);
    assert(was(5, BIT, 7, 3, 4, 0, 0, 0) && was(6, REST_B, 5, 9, 0, 0, 0, 0) && log_n == 7);
    grid_by[4][2] = 8;
    /* 0x27d830 answers 5: find dropped; bit set: kind-14 result */
    around_answer = 5, bit_answer = 1;
    assert(run_entry_rule(&order, &result) == 0 && calls_of(WORD_D) == 0 && calls_of(REST_B) == 0);
    assert(was(5, BIT, 7, 3, 4, 0, 0, 0) && was(6, SETBIT, 7, 3, 0, 1, 0, 0) && log_n == 7);
    assert(result.position == 0x0703 && result.kind == 14 && result.loc == 0x4321);
    /* non-zero kind, no entry, bit set: kind 14 without the finder */
    around_answer = 4, entry_z_null = 1;
    assert(run_entry_rule(&order, &result) == 0 && calls_of(FIND_Z) == 0 && result.kind == 14);
    /* kind 0 never asks the bit even when it is set */
    kind_answer = 0;
    assert(run_entry_rule(&order, &result) == 77 && calls_of(BIT) == 0);
    entry_z_null = 0, bit_answer = 0;
}

/* TaLogic_GetMinimumTask_ReviveMission__001170a0 (branch coverage). 0x1e0700 is asked twice:
 * kind_seq[0] picks the entry number, kind_seq[1] gates the vetoes. */
enum { FIND_Y = 170, REST_Y };
int TaLogic001_IsUseRevive(int side) { note(FIND_Y, side, 0, 0, 0, 0, 0); return find_answer; }
int func_00115850_rest(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode)
{
    note(REST_Y, side, mode, 0, 0, 0, 0);
    rest_order = order, rest_result = result;
    return rest_answer;
}

static int run_side_rule(struct DotrRuleOrder *order, struct DotrRuleResult *result, int first, int second)
{
    memset(result, 0, sizeof(*result));
    result->loc = 0x4321;
    log_n = 0, grid_calls = 0;
    done_order = NULL, rest_order = NULL, rest_result = NULL;
    list_mode = 0;
    kind_seq[1] = first, kind_seq[0] = second, kind_seq_n = 2;
    return TaLogic_GetMinimumTask_ReviveMission__001170a0(order, 5, result, 9);
}

static void check_side_rule(void)
{
    struct DotrRuleOrder order;
    struct DotrRuleResult result;

    memset(&order, 0, sizeof(order));
    order.position = 0x0703;
    col_mode = 0;
    memset(grid_by, 0, sizeof(grid_by));
    cell_by[3] = 0x0204;
    rest_answer = 77, pick_answer = 0x5a, word_answer = 0x21, around_answer = 4, bit_answer = 0;
    /* kind 0, no entry, bit clear: entry 3 asked, then 0x115850 */
    entry_z_null = 1;
    assert(run_side_rule(&order, &result, 0x1f, 0x1f) == 77 && log_n == 6);
    assert(was(0, KIND, 7, 3, 0, 0, 0, 0) && was(1, ENTRY_Z, 7, 3, 3, 0, 0, 0) && was(2, CELL, 7, 3, 0, 0, 0, 0));
    assert(was(3, KIND, 7, 3, 0, 0, 0, 0) && was(4, BIT, 7, 3, 4, 0, 0, 0) && was(5, REST_Y, 5, 9, 0, 0, 0, 0));
    assert(rest_order == &order && rest_result == &result && result.kind == 0);
    /* no entry, bit set: kind 14 even with kind 0 */
    bit_answer = 1;
    assert(run_side_rule(&order, &result, 0, 0) == 0 && was(5, SETBIT, 7, 3, 0, 1, 0, 0) && log_n == 6);
    assert(result.position == 0x0703 && result.kind == 14 && result.loc == 0x4321 && calls_of(FIND_Y) == 0);
    bit_answer = 0;
    /* entry, negative find: no veto queries, 0x115850 */
    entry_z_null = 0, entry_z_word = 0xffffff00 | 2 << 6 | 0x2b, find_answer = -1;
    assert(run_side_rule(&order, &result, 0, 0) == 77 && log_n == 7 && grid_calls == 0);
    assert(was(4, FIND_Y, 5, 0, 0, 0, 0, 0) && was(6, REST_Y, 5, 9, 0, 0, 0, 0));
    /* find 0 (boundary) and a larger find: kind-12 result with the side */
    find_answer = 0;
    assert(run_side_rule(&order, &result, 0, 0) == 0 && was(5, WORD_D, 0, 0, 0x2b, 0, 0, 0));
    find_answer = 0x00030000 | 0x1ee;
    assert(run_side_rule(&order, &result, 0, 0) == 0 && log_n == 8 && grid_calls == 0);
    assert(was(5, WORD_D, 3, 0xee, 0x2b, 0, 0, 0) && was(6, PICK, 7, 3, 0x4321, 1, 0, 0) && was(7, DONE, 9, 0, 0, 0, 0, 0));
    assert(result.position == 0x0703 && result.kind == 12 && result.atkDefPosition == 0x5a && result.shouldFlipUp == 1);
    assert(result.loc == (5 << 8 | 0x21) && done_order == &order);
    /* entry bits 6-7 of 3: the found index instead of the side */
    entry_z_word = 3 << 6 | 0x01, find_answer = 0x00030000;
    assert(run_side_rule(&order, &result, 0, 0) == 0 && result.loc == (3 << 8 | 0x21));
    /* first answer non-zero: entry 0; the second answer alone gates the vetoes */
    assert(run_side_rule(&order, &result, 1 << 5, 0) == 0 && was(1, ENTRY_Z, 7, 3, 0, 0, 0, 0) && grid_calls == 0);
    assert(run_side_rule(&order, &result, 0, 1 << 5) == 0 && was(1, ENTRY_Z, 7, 3, 3, 0, 0, 0) && grid_calls == 1);
    assert(was(5, AROUND, 7, 3, 0, 0, 6, 0) && was(6, WORD_D, 3, 0, 1, 0, 0, 0) && result.kind == 12);
    /* SzDuel_GetField(low byte, rest) answers 9: find dropped */
    grid_by[4][2] = 9;
    assert(run_side_rule(&order, &result, 0, 1 << 5) == 77 && calls_of(WORD_D) == 0 && calls_of(AROUND) == 1);
    assert(calls_of(REST_Y) == 1);
    grid_by[4][2] = 8;
    /* 0x27d830 answers 5: find dropped */
    around_answer = 5;
    assert(run_side_rule(&order, &result, 0, 1 << 5) == 77 && calls_of(WORD_D) == 0 && calls_of(REST_Y) == 1);
    around_answer = 4;
}

/* TaLogic021_IsUseBlackHole (branch and loop coverage). */
enum { SCORE_Y = 172 };
static int score_y_answer;
int TaUtil_GetUnitAP(int a, int b, int c, int d) { note(SCORE_Y, a, b, c, d, 0, 0); return score_y_answer; }

static int run_cell_count(int owner, int score, int live)
{
    int i;

    log_n = 0, set_calls = 0;
    list_mode = 0, col_mode = 0;
    set_by[0] = 0x6600, entry_by[0] = 0x0703, cell_by[3] = 0x0204;
    pair_answer = owner, score_y_answer = score;
    memset(dfw_by, 0, sizeof(dfw_by));
    for (i = 1; i <= live; i++) {
        dfw_by[i] = 1;
    }
    dfw_by[0] = dfw_by[11] = 1;
    return TaLogic021_IsUseBlackHole(5);
}

static void check_cell_count(void)
{
    /* own cell: nothing else asked */
    assert(run_cell_count(5, 9000, 10) == 1 && log_n == 3);
    assert(was(0, SET, 0, 0, 5, 3, 33, 14) && was(1, ENTRY, 0x6600, 0, 0, 0, 0, 0) && was(2, PAIR, 7, 3, 0, 0, 0, 0));
    /* another side below 3000: no count */
    assert(run_cell_count(4, 2999, 10) == 1 && log_n == 6 && calls_of(DFW) == 0);
    assert(was(3, CELL, 7, 3, 0, 0, 0, 0) && was(4, CELL, 7, 3, 0, 0, 0, 0) && was(5, SCORE_Y, 7, 3, 0x04, 0x02, 0, 0));
    /* at 3000: cells 1..10 counted; two is under the bound, three is not */
    assert(run_cell_count(4, 3000, 2) == 1 && calls_of(DFW) == 10);
    assert(was(6, DFW, 5, 1, 0, 0, 0, 0) && was(15, DFW, 5, 10, 0, 0, 0, 0) && log_n == 16);
    assert(run_cell_count(4, 3000, 3) == 0);
    assert(run_cell_count(4, 3000, 10) == 0);
    /* cells 0 and 11 are outside the walk */
    assert(run_cell_count(4, 3000, 0) == 1);
    pair_answer = 0, memset(dfw_by, 0, sizeof(dfw_by));
}

/* TaLogic_GetMinimumTask_ReviveMission__00161bf0 (branch coverage). Its finders 0x1627d0 and
 * 0x162a70 are the real functions, driven through the check_hand stubs: the wanted id sits in the
 * other side's hand at position 2, so a find is 2 (index 0, target 2). */
static int run_hand_rule(struct DotrRuleOrder *order, struct DotrRuleResult *result)
{
    memset(result, 0, sizeof(*result));
    result->loc = 0x4321;
    log_n = 0, grid_calls = 0;
    gate_b_calls = have_calls = hand_reads = 0;
    done_order = NULL, rest_order = NULL, rest_result = NULL;
    return TaLogic_GetMinimumTask_ReviveMission__00161bf0(order, 1, result, 9);
}

static void check_hand_rule(void)
{
    struct DotrRuleOrder order;
    struct DotrRuleResult result;

    memset(&order, 0, sizeof(order));
    order.position = 0x0703;
    list_mode = 0, col_mode = 0;
    memset(grid_by, 0, sizeof(grid_by));
    memset(hand_ids, 0, sizeof hand_ids);
    hand_count[0] = hand_count[1] = 8;
    have_by[0] = have_by[1] = have_by[2] = 1;
    level_answer = gate_b_answer = 0;
    cell_by[3] = 0x0204;
    rest_answer = 77, pick_answer = 0x5a, word_answer = 0x21, around_answer = 4, bit_answer = 0;
    /* kind 0, no entry: 0x1607d0 without a finder or the bit query */
    kind_answer = 0x1f, entry_z_null = 1;
    assert(run_hand_rule(&order, &result) == 77 && log_n == 4 && gate_b_calls == 0);
    assert(was(1, ENTRY_Z, 7, 3, 3, 0, 0, 0) && was(3, REST_A, 1, 9, 0, 0, 0, 0));
    assert(rest_order == &order && rest_result == &result);
    /* kind 0, entry: TaLogic019_IsUseSaint (second gate), found: side form of loc */
    entry_z_null = 0, entry_z_word = 2 << 6 | 0x2b;
    have_ids[0] = 700, have_ids[1] = 715, have_ids[2] = 827;
    hand_ids[0][2] = 827;
    assert(run_hand_rule(&order, &result) == 0 && gate_b_calls == 1 && calls_of(LEVEL) == 0 && grid_calls == 0);
    assert(calls_of(AROUND) == 0 && was(3, WORD_D, 0, 2, 0x2b, 0, 0, 0) && was(4, PICK, 7, 3, 0x4321, 1, 0, 0));
    assert(was(5, DONE, 9, 0, 0, 0, 0, 0) && done_order == &order && log_n == 6);
    assert(result.position == 0x0703 && result.kind == 12 && result.atkDefPosition == 0x5a && result.shouldFlipUp == 1);
    assert(result.loc == (1 << 8 | 0x21));
    /* entry bits 6-7 of 3: the found index (0) instead of the side */
    entry_z_word = 3 << 6 | 0x2b;
    assert(run_hand_rule(&order, &result) == 0 && result.loc == 0x21);
    /* kind 0, finder -1 (gate at 5): 0x1607d0 */
    gate_b_answer = 5;
    assert(run_hand_rule(&order, &result) == 77 && gate_b_calls == 1 && calls_of(REST_A) == 1 && calls_of(BIT) == 0);
    /* non-zero kind: entry 0, TaLogic019_IsUseRevive (first gate), both vetoes pass */
    kind_answer = 1 << 5;
    have_ids[0] = 60, have_ids[1] = 87, have_ids[2] = 43;
    hand_ids[0][2] = 87;
    assert(run_hand_rule(&order, &result) == 0 && was(1, ENTRY_Z, 7, 3, 0, 0, 0, 0));
    assert(gate_b_calls == 0 && calls_of(LEVEL) == 1 && grid_calls == 1 && calls_of(AROUND) == 1);
    assert(calls_of(WORD_D) == 1 && result.kind == 12 && result.loc == 0x21);
    /* 0x1e22b0 answers 9: find dropped; bit clear: 0x1607d0 */
    grid_by[4][2] = 9;
    assert(run_hand_rule(&order, &result) == 77 && calls_of(WORD_D) == 0 && calls_of(AROUND) == 1);
    assert(calls_of(BIT) == 1 && calls_of(REST_A) == 1);
    grid_by[4][2] = 8;
    /* 0x27d830 answers 5: find dropped; bit set: kind-14 result */
    around_answer = 5, bit_answer = 1;
    assert(run_hand_rule(&order, &result) == 0 && calls_of(WORD_D) == 0 && calls_of(REST_A) == 0);
    assert(calls_of(SETBIT) == 1 && result.position == 0x0703 && result.kind == 14 && result.loc == 0x4321);
    /* non-zero kind, finder -1 (gate at 5), bit clear: 0x1607d0 */
    around_answer = 4, bit_answer = 0, level_answer = 5;
    assert(run_hand_rule(&order, &result) == 77 && calls_of(LEVEL) == 1 && calls_of(BIT) == 1);
    /* non-zero kind, no entry: no finder */
    entry_z_null = 1;
    assert(run_hand_rule(&order, &result) == 77 && calls_of(LEVEL) == 0 && calls_of(BIT) == 1);
    entry_z_null = 0, level_answer = 0;
    memset(hand_ids, 0, sizeof hand_ids);
    hand_count[0] = hand_count[1] = 0;
}

/* TaLogic_IsOpenUnit__0016ae30 (branch coverage). 0x1de5c0 answers low_count; 0x16b0f0 is the
 * real function: it answers 1 when the 0x250360 stub answers its side, and 0
 * when that differs, 0x2509e0 answers 3000 and three 0x1df540 words are set. */
static int low_count, low_count_side;
int SzDuel_GetSummonPower(int index) { low_count_side = index; return low_count; }

static int rule_logged(int id, int a, int b, int c, int d, int e, int f)
{
    int i;
    for (i = 0; i < log_n; i++) {
        if (log_id[i] == id && log_arg[i][0] == a && log_arg[i][1] == b && log_arg[i][2] == c
            && log_arg[i][3] == d && log_arg[i][4] == e && log_arg[i][5] == f) {
            return 1;
        }
    }
    return 0;
}

static int run_kind_rule(int bit, int kind, int word)
{
    col_mode = 0, list_mode = 0, kind_seq_n = 0;
    bit_answer = bit, kind_answer = kind, word_answer = word;
    log_n = 0;
    low_count_side = -1;
    return TaLogic_IsOpenUnit__0016ae30(1, 9, 0);
}

static void check_kind_rule(void)
{
    memset(dfw_by, 0, sizeof(dfw_by));
    pair_answer = 1, around_answer = 4, entry_z_null = 0, entry_z_word = 0, low_count = 3;

    assert(run_kind_rule(1, 0, 364) == 0);
    assert(rule_logged(WORD, 1, 9, 0, 0, 0, 0) && rule_logged(PAIR, 1, 9, 0, 0, 0, 0));
    assert(rule_logged(ENTRY_Z, 1, 9, 0, 0, 0, 0) && rule_logged(CELL, 1, 9, 0, 0, 0, 0));
    assert(rule_logged(BIT, 1, 9, 4, 0, 0, 0) && !rule_logged(KIND, 1, 9, 0, 0, 0, 0));

    /* kind >> 5 is neither 0 nor 1 */
    assert(run_kind_rule(0, 0x40, 364) == 2 && low_count_side == -1);

    /* kind >> 5 == 0: ids 224 and 364 with the count below 4 */
    assert(run_kind_rule(0, 0x1f, 364) == 1 && low_count_side == 1);
    assert(run_kind_rule(0, 0x1f, 224) == 1 && low_count_side == 1);
    assert(run_kind_rule(0, 0x1f, 363) == 2 && low_count_side == -1);
    assert(run_kind_rule(0, 0x1f, 225) == 2 && low_count_side == -1);
    low_count = 4;
    assert(run_kind_rule(0, 0x1f, 364) == 2 && low_count_side == 1);
    assert(run_kind_rule(0, 0x1f, 224) == 2);

    /* kind >> 5 == 1 */
    low_count = 3;
    around_answer = 5;
    entry_z_word = 39 << 8;
    assert(run_kind_rule(0, 0x3f, 364) == 2 && low_count_side == -1);
    assert(rule_logged(AROUND, 1, 9, 0, 0, 6, 1));
    around_answer = 4;
    entry_z_null = 1;
    assert(run_kind_rule(0, 0x3f, 364) == 2 && low_count_side == -1);
    entry_z_null = 0;
    entry_z_word = 39 << 8 | 0xff;
    assert(run_kind_rule(0, 0x3f, 0) == 1 && low_count_side == 1);
    low_count = 4;
    assert(run_kind_rule(0, 0x3f, 0) == 2 && low_count_side == 1);
    low_count = 3;
    entry_z_word = 40 << 8;
    assert(run_kind_rule(0, 0x3f, 364) == 2 && low_count_side == -1);
    entry_z_word = 4 << 8 | 0xff;
    assert(run_kind_rule(0, 0x3f, 0) == 1 && low_count_side == -1);
    pair_answer = 0, score_y_answer = 3000;
    dfw_by[1] = dfw_by[2] = dfw_by[3] = 1;
    assert(run_kind_rule(0, 0x3f, 0) == 2);
    memset(dfw_by, 0, sizeof(dfw_by));
    pair_answer = 0, score_y_answer = 0;
}

/* Branch coverage: answers 1, 2 and another value. */
static void check_task_atk_or_move(void)
{
    struct jp_9add8 task;
    int answer;

    for (answer = 0; answer <= 3; answer++) {
        memset(&task, 0, sizeof(task));
        task.wUnit = 0x0305;
        task.wOrder = 0x7777;
        task.dwGoal = 0xaaaa1234u;
        cell_by[7] = 0x0409;
        around_answer = answer;
        log_n = 0;
        TaLogic_SetTaskAtkOrMove(&task, 0x0207, 1);
        assert(log_n == 2 && was(0, CELL, 2, 7, 0, 0, 0, 0) && was(1, AROUND, 3, 5, 9, 4, 0, 1));
        assert(task.wUnit == 0x0305 && task.wOrder == 0x7777);
        if (answer == 1 || answer == 2) {
            assert(task.wPurpose == 0x6ff && task.dwGoal == 0x04091234u);
        } else {
            assert(task.wPurpose == 0x2ff && task.dwGoal == 0x02071234u);
        }
    }
}

static int pass_root_args[7];
static int pass_root_calls;
static int pass_root_answer;
int TaLogic_IsPassRoot(int a, int b, int c, int d, int e, int f, unsigned short *g)
{
    int v[7] = { a, b, c, d, e, f, *g };
    memcpy(pass_root_args, v, sizeof v);
    pass_root_calls++;
    return pass_root_answer;
}

/* TaLogic_GetQueueXY_Move1 (branch coverage): adjacent cell, x distance 0, 1 (route found, not
 * found), 2 and 3. */
static void check_queue_xy_move1(void)
{
    static const int want[7] = { 1, 9, 3, 2, 4, 2, 0x1234 };
    grid_g = 0x1234;
    pass_root_calls = 0;
    pass_root_answer = 0;
    assert(TaLogic_GetQueueXY_Move1(1, 9, 3, 2, 3, 3) == 1);
    assert(TaLogic_GetQueueXY_Move1(1, 9, 3, 2, 4, 2) == 1);
    assert(TaLogic_GetQueueXY_Move1(1, 9, 3, 2, 3, 4) == 1);
    assert(TaLogic_GetQueueXY_Move1(1, 9, 3, 2, 5, 2) == 1);
    assert(TaLogic_GetQueueXY_Move1(1, 9, 3, 2, 5, 3) == 1);
    assert(TaLogic_GetQueueXY_Move1(1, 9, 3, 2, 6, 2) == 1);
    assert(pass_root_calls == 0);
    assert(TaLogic_GetQueueXY_Move1(1, 9, 3, 2, 4, 3) == 0 && pass_root_calls == 1);
    assert(memcmp(pass_root_args, want, sizeof want) == 0);
    pass_root_answer = 1;
    assert(TaLogic_GetQueueXY_Move1(1, 9, 3, 2, 2, 0) == 1 && pass_root_calls == 2);
    assert(pass_root_args[4] == 2 && pass_root_args[5] == 2);
    pass_root_answer = 2;
    assert(TaLogic_GetQueueXY_Move1(1, 9, 3, 2, 2, 0) == 0 && pass_root_calls == 3);
}

/* TaLogic_IsUnitCategory (branch coverage): each kind, both sides of every bound. */
static void check_unit_category(void)
{
    assert(TaLogic_IsUnitCategory(7, 0, 34) == 1 && TaLogic_IsUnitCategory(7, 1, 34) == 0);
    assert(TaLogic_IsUnitCategory(7, 0, 29) == 0 && TaLogic_IsUnitCategory(7, 1, 29) == 1);
    assert(TaLogic_IsUnitCategory(7, 5, 29) == 1 && TaLogic_IsUnitCategory(7, 6, 29) == 0);
    assert(TaLogic_IsUnitCategory(7, 5, 30) == 0 && TaLogic_IsUnitCategory(7, 6, 30) == 1);
    assert(TaLogic_IsUnitCategory(7, 11, 30) == 1 && TaLogic_IsUnitCategory(7, 12, 30) == 0);
    assert(TaLogic_IsUnitCategory(7, 0, 31) == 1 && TaLogic_IsUnitCategory(7, 9, 31) == 1);
    assert(TaLogic_IsUnitCategory(7, 0, 52) == 0 && TaLogic_IsUnitCategory(7, 3, 52) == 1);
    assert(TaLogic_IsUnitCategory(7, 0, 33) == 0 && TaLogic_IsUnitCategory(7, 3, 33) == 1);
}

static int trap_answer, trap_args[2], used_by[8], used_calls, ench_by[64], ench_root, encdo_by[64];
static int keep_n, keep_id[8], keep_root[8], len_calls, len_answer, score_by[64];
static struct jp_9c054 *keep_rule[8];
static unsigned char *used_list, *len_list;
int Ta_IsActiveTrapB(int a, int b) { trap_args[0] = a; trap_args[1] = b; return trap_answer; }
int TaLogic_IsUseHandInCombo(unsigned char value, unsigned char *p) { used_calls++; used_list = p; return used_by[value & 7]; }
int Ta_GetHandEnchant(int a, int b) { ench_root = a; return ench_by[b & 63]; }
int TaEnc_IsEncDo(int eNO) { return encdo_by[eNO & 63]; }
int TaLogic_IsKeepEnchant(struct jp_9c054 *rule, int id, int value)
{
    keep_rule[keep_n] = rule;
    keep_id[keep_n] = id;
    keep_root[keep_n] = value;
    keep_n++;
    return 0;
}
int TaLogic_GetArrayLength(unsigned char *bytes) { len_list = bytes; len_calls++; return len_answer++; }
int TaEnc_GetEncScore(int index) { return score_by[index & 63]; }

/* TaLogic_GetEnchantQue (branch coverage): active trap 43 with and without its status bit, empty
 * hand, each of the six skips, the store branch (narrowed length, 16-bit DP) and the keep loop
 * with depth 0 and 2. */
static void check_enchant_que(void)
{
    struct TreeTag tree;
    struct jp_9c054 keep_a, keep_b, *keep[2] = { &keep_a, &keep_b };
    int i;

    memset(&tree, 0, sizeof tree);
    tree.nRoot = 0x155;
    ids_copy = 5 * sizeof(int);
    col_mode = 0;
    id_word = 0x0012fff0;
    for (i = 0; i < 64; i++) {
        id_kind[i] = 0x40;
        ench_by[i] = 0x10000;
        encdo_by[i] = 1;
        score_by[i] = 0;
    }
    /* trap found and its status bit set: 0, the hand is not walked */
    log_n = 0;
    trap_answer = 0x0305;
    bit_answer = 1;
    id_count = 5;
    assert(TaLogic_GetEnchantQue(1, &tree, 0, 0) == 0);
    assert(trap_args[0] == 1 && trap_args[1] == 43 && calls_of(FILL) == 0);
    assert(calls_of(BIT) == 1 && calls_of(IDWORD) == 2 && was(0, IDWORD, 0x155, 0, 0, 0, 0, 0));
    assert(was(2, IDS, 1, 0, 0, 0, 0, 0) && was(3, BIT, 3, 5, 4, 0, 0, 0));
    /* trap found, bit clear, empty hand: the packed word unchanged */
    log_n = 0;
    bit_answer = 0;
    id_count = 0;
    assert(TaLogic_GetEnchantQue(1, &tree, 0, 0) == 0x0012fff0);
    assert(calls_of(BIT) == 1 && calls_of(FILL) == 1 && used_calls == 0);
    /* no trap: no status question; five skipped entries */
    log_n = 0;
    trap_answer = -1;
    id_count = 5;
    ids[0] = 10; ids[1] = 11; ids[2] = 796; ids[3] = 795; ids[4] = 12;
    memset(used_by, 0, sizeof used_by);
    used_by[0] = 1;
    id_kind[11] = 0x20;
    ench_by[12] = 0x20000;
    assert(TaLogic_GetEnchantQue(1, &tree, 0, 0) == 0x0012fff0);
    assert(calls_of(BIT) == 0 && used_calls == 5 && used_list == tree.nComboLink);
    assert(calls_of(IDKIND) == 4 && ench_root == 0x155 && len_calls == 0 && keep_n == 0);
    /* store branch: entry 0 fails TaEnc_IsEncDo, entries 1 and 2 are stored; a set bit above
     * the low byte of the combo answer and of the length is ignored */
    log_n = 0;
    id_count = 3;
    ids[0] = 13; ids[1] = 14; ids[2] = 15;
    used_by[0] = 0x100;
    encdo_by[13] = 0;
    score_by[14] = 0x20;
    score_by[15] = 0x30;
    len_answer = 0x101;
    assert(TaLogic_GetEnchantQue(1, &tree, 0, 2) == 0x00620040);
    assert(len_calls == 2 && len_list == tree.nComboLink && keep_n == 0);
    assert(tree.nComboLink[0] == 0 && tree.nComboLink[1] == 1 && tree.nComboLink[2] == 2);
    /* keep list: depth 0 asks nothing, depth 2 asks both entries for each accepted card */
    assert(TaLogic_GetEnchantQue(1, &tree, keep, 0) == 0x0012fff0 && keep_n == 0 && len_calls == 2);
    assert(TaLogic_GetEnchantQue(1, &tree, keep, 2) == 0x0012fff0 && keep_n == 4 && len_calls == 2);
    assert(keep_rule[0] == &keep_a && keep_rule[1] == &keep_b && keep_id[0] == 14 && keep_id[1] == 14);
    assert(keep_rule[2] == &keep_a && keep_rule[3] == &keep_b && keep_id[2] == 15 && keep_root[3] == 0x155);
    ids_copy = sizeof ids;
}

int main(void)
{
    check_prepare_g();
    check_prepare_f();
    check_prepare_more();
    check_prepare_subs();
    check_gate();
    check_prepare_d();
    check_prepare_c();
    check_prepare_b();
    check_prepare();
    check_lists(TaLogic_GetNextActiveUnit__00118510, 0);
    check_lists(TaLogic_GetNextActiveUnit__0011cd30, 1);
    check_lists(TaLogic_GetNextActiveUnit__00121e90, 2);
    check_lists(TaLogic_GetNextActiveUnit__001273e0, 3);
    check_lists(TaLogic_GetNextActiveUnit__00131750, 4);
    check_lists(TaLogic_GetNextActiveUnit__00135970, 5);
    check_lists(TaLogic_GetNextActiveUnit__0013e3d0, 6);
    check_lists(TaLogic_GetNextActiveUnit__00142b70, 7);
    check_lists(TaLogic_GetNextActiveUnit__0014bfd0, 8);
    check_lists(TaLogic_GetNextActiveUnit__00150470, 9);
    check_lists(TaLogic_GetNextActiveUnit__00159380, 10);
    check_lists(TaLogic_GetNextActiveUnit__0015daf0, 11);
    check_lists(TaLogic_GetNextActiveUnit__00163870, 12);
    check_lists(TaLogic_GetNextActiveUnit__0016d8c0, 13);
    check_lists_more();
    check_lists_late(TaLogic_GetNextActiveUnit__00154ad0, 14);
    check_lists_late(TaLogic_GetNextActiveUnit__00167e80, 15);
    check_lists(keyed_TaLogic_GetNextActiveUnit__00113370, 16);
    check_lists(keyed_TaLogic_GetNextActiveUnit__0013a010, 17);
    check_lists(keyed_TaLogic_GetNextActiveUnit__00147100, 18);
    check_columns();
    check_hand(TaLogic019_IsUseRevive, 0, 60, 87, 43, 87, 43, 60);
    check_hand(TaLogic019_IsUseSaint, 1, 700, 715, 827, 827, 700, 715);
    check_hand(TaLogic013_IsUseRevive, 0, 273, 30, 11, 11, 30, 273);
    check_word();
    check_rewrite();
    check_total_level(TaLogic_GetWarnHideLvl__00117c30, 2500, 4);
    check_total_level(TaLogic_GetWarnHideLvl__0015d120, 2000, 5);
    check_step(L001, 2);
    check_step(L018, 3);
    check_step(L003, 0);
    check_step(L004, 1);
    check_step(L005, 4);
    check_step(L006, 5);
    check_step(L012, 6);
    check_step(L010, 7);
    check_step(L020, 8);
    check_step(L021, 12);
    check_step(L015, 13);
    check_step(L002, 14);
    check_step(L009, 15);
    check_step(L014, 16);
    check_step(L007, 17);
    check_step(L013, 18);
    check_step(L017, 19);
    check_step(L019, 20);
    check_cell_count();
    check_step(L008, 10);
    check_step(L011, 11);
    check_count_three();
    step_bare = 1;
    check_step(L022, 9);
    /* L016 (forwarder): reaches 0x172370 with both arguments, same checks. */
    check_step(L016, 9);
    step_bare = 0;
    check_found(TaLogic_GetMinimumTask_RevoltMission__00161ea0, FIND_A, REST_A);
    check_found(TaLogic_GetMinimumTask_RevoltMission__00172000, FIND_B, REST_B);
    found_last = 1;
    check_found(TaLogic_GetMinimumTask_RevoltMission__00138950, FIND_C, REST_C);
    check_found_word(TaLogic_GetMinimumTask_ReviveMission__00138780, FIND_D, REST_C, 0, 0);
    check_found_word(TaLogic_GetMinimumTask_ReviveMission__00166d10, FIND_E, REST_E, 30, 1);
    check_revive_field();
    found_last = 0;
    check_place();
    check_objects();
    check_levels();
    check_order_words();
    check_first_entry();
    check_packed(TaLogic019_GetUseThunderBolt, 1, -1);
    check_packed(TaLogic010_GetUseThunderBolt, 0, 1);
    check_rows();
    check_corner();
    check_words();
    check_guarded();
    check_id();
    check_ids();
    check_send();
    check_near(TaLogic_GetMinimumTask_TrapMission__00116b60, 2500);
    key_answer = 0x1A5;
    check_near(TaLogic_GetMinimumTask_TrapMission__00130120, 0x1A5);
    check_near(TaLogic_GetMinimumTask_TrapMission__001415f0, 0x1A5);
    check_near(TaLogic_GetMinimumTask_TrapMission__0015c670, 2000);
    /* 0x153550: the low byte of TaLogic015_IsActiveMode() chooses the fourth 0x276cc0 argument, 2
     * for zero and 5 otherwise */
    flag_quiet = 1;
    near_fourth = 2;
    check_near(TaLogic_GetMinimumTask_TrapMission__00153550, key_answer);
    assert(flag_quiet_calls > 20);
    flag_quiet = 2;
    near_fourth = 5;
    flag_quiet_calls = 0;
    check_near(TaLogic_GetMinimumTask_TrapMission__00153550, key_answer);
    assert(flag_quiet_calls > 20);
    flag_quiet = 0;
    near_fourth = 3;
    /* expanded key: total at the threshold, then above it with the level below and at 3 */
    side_quiet = 1;
    side_total = 1500, level_answer = 3;
    check_near(TaLogic_GetMinimumTask_TrapMission__0011b0e0, 0);
    assert(side_calls > 20 && level_quiet_calls == 0);
    side_total = 1501, level_answer = 2;
    check_near(TaLogic_GetMinimumTask_TrapMission__0011b0e0, 0);
    assert(level_quiet_calls > 20);
    level_answer = 3;
    check_near(TaLogic_GetMinimumTask_TrapMission__0011b0e0, 0);
    near_fourth = 4;
    side_calls = level_quiet_calls = 0;
    near_none_flag = 0;
    side_total = 1000, level_answer = 3;
    check_near(TaLogic_GetMinimumTask_TrapMission__00120150, 2000);
    assert(side_calls > 20 && level_quiet_calls == 0);
    side_total = 1001, level_answer = 2;
    check_near(TaLogic_GetMinimumTask_TrapMission__00120150, 1000);
    assert(level_quiet_calls > 20);
    level_answer = 3;
    check_near(TaLogic_GetMinimumTask_TrapMission__00120150, 500);
    side_quiet = 0;
    near_none_flag = 1;
    level_answer = 0;
    near_fourth = 5;
    check_near(TaLogic_GetMinimumTask_TrapMission__0012aac0, 0x1A5);
    near_fourth = 3;
    check_entry_rule();
    check_side_rule();
    check_hand_rule();
    check_kind_rule();
    check_task_atk_or_move();
    check_queue_xy_move1();
    check_unit_category();
    check_enchant_que();
    return 0;
}
