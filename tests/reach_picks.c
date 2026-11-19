/* Synthetic host tests for the reach_picks group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/reach_picks.h"

typedef int (*reach_fn)(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);

static int here_answer, cell_args[2];
static int side_arg, side_answer;
static int score[8][8], score_calls, score_args[6];
static int pick_answer, pick_args[5], pick_calls;
static int tail_args[2], tail_calls;
static struct DotrRuleOrder *tail_order, *done_order, *scan_order;
static int done_mode, done_calls;
static int dfw_answer, rule_answer, rule_args[3];
static int bit_answer, bit_args[3];
static int kind_seq[2], kind_calls;
static int set_args[4], set_calls;
static int edge_answer, edge_calls, edge_args[3];
static int ok[8][8], scan_calls, scan_args[3];
static int blocked, blocked_calls, blocked_last;
static int taken[8][8], item_dummy;
static int score_mode = 3, away_flag, last_flag, no_gate, no_side, zero_side;
static int total_answer, count_calls, count_answer;

int SzDuel_GetUnitLoc(int a, int b) { cell_args[0] = a; cell_args[1] = b; return here_answer; }
static int side_stub(int side) { side_arg = side; return side_answer; }
int TaLogic_GetUnitWarning(int a, int b, int c, int d, int e, unsigned char f)
{
    score_calls++;
    score_args[0] = a, score_args[1] = b, score_args[2] = c, score_args[3] = d, score_args[4] = e, score_args[5] = f;
    return score[a][b];
}
int TaLogic_IsDefStyleUnit(int a, int b, unsigned short c, unsigned char d, int e)
{
    pick_calls++;
    pick_args[0] = a, pick_args[1] = b, pick_args[2] = c, pick_args[3] = d, pick_args[4] = e;
    return pick_answer;
}
static int tail_answer = 0x12345678;
static int tail_stub(int a, int b, struct DotrRuleOrder *order)
{
    tail_calls++;
    tail_args[0] = a, tail_args[1] = b, tail_order = order;
    return tail_answer;
}
void func_0027b1b0_mode(struct DotrRuleOrder *order, unsigned char mode) { done_calls++; done_order = order; done_mode = mode; }
int func_001df540_word(int a, int b) { assert(a == cell_args[0] && b == cell_args[1]); return dfw_answer; }
int func_002464f0_rule(int item, int x, int y) { rule_args[0] = item, rule_args[1] = x, rule_args[2] = y; return rule_answer; }
int SzDuel_GetUnitStatus(int a, int b, int bit) { bit_args[0] = a, bit_args[1] = b, bit_args[2] = bit; return bit_answer; }
int SzDuel_GetUnitKind(int a, int b) { assert(a == cell_args[0] && b == cell_args[1]); return kind_seq[kind_calls++ & 1]; }
int SzDuel_SetUnitStatus(int a, int b, int bit, int value)
{
    set_calls++;
    set_args[0] = a, set_args[1] = b, set_args[2] = bit, set_args[3] = value;
    return 0;
}
/* small_misc's GetCap with the limit of 0 every caller passes: the larger of the two. */
int GetCap(int limit, int value) { return value < limit ? limit : value; }
int TaLogic_IsAttackedNow(int a, int b, int c) { edge_calls++; edge_args[0] = a, edge_args[1] = b, edge_args[2] = c; return edge_answer; }
int TaLogic_IsRunPassRoot(int a, int b, int c, int d, int e, int f, struct DotrRuleOrder *g, unsigned char h)
{
    assert(a == cell_args[0] && b == cell_args[1]);
    scan_calls++;
    scan_args[0] = c, scan_args[1] = d, scan_args[2] = h, scan_order = g;
    return ok[e][f];
}
int TaLogic_IsAttackedNow2(int row, int column, int a, int b, int c)
{
    assert(row == cell_args[0] && column == cell_args[1] && c == last_flag);
    blocked_calls++;
    blocked_last = b << 8 | a;
    return blocked;
}
struct DotrStateItem *SzDuel_GetMapUnit(int x, int y) { return taken[x][y] ? (struct DotrStateItem *)&item_dummy : NULL; }

/* The per-copy callees (0x14b330 and 0x14ae60 for 0x149690) share one stub each. */
int TaLogic_GetWarnHideLvl__0014b330(int side) { return side_stub(side); }
int TaLogic_IsOpenUnit__0014ae60(int a, int b, struct DotrRuleOrder *order) { return tail_stub(a, b, order); }
int TaLogic_GetWarnHideLvl__0015d120(int side) { return side_stub(side); }
int TaLogic_IsOpenUnit__0015cbb0(int a, int b, struct DotrRuleOrder *order) { return tail_stub(a, b, order); }
int TaLogic_GetWarnHideLvl__00158780(int side) { return side_stub(side); }
int TaLogic_IsOpenUnit__00158360(int a, int b, struct DotrRuleOrder *order) { return tail_stub(a, b, order); }
int TaLogic_GetWarnHideLvl__0014f9b0(int side) { return side_stub(side); }
int TaLogic_IsOpenUnit__0014f490(int a, int b, struct DotrRuleOrder *order) { return tail_stub(a, b, order); }
int TaLogic_GetWarnHideLvl__00153ed0(int side) { return side_stub(side); }
int TaLogic_IsOpenUnit__00153ae0(int a, int b, struct DotrRuleOrder *order) { return tail_stub(a, b, order); }
int TaLogic_GetWarnHideLvl__00162580(int side) { return side_stub(side); }
int TaLogic_IsOpenUnit__001620b0(int a, int b, struct DotrRuleOrder *order) { return tail_stub(a, b, order); }
int TaLogic_GetWarnHideLvl__00172310(int side) { return side_stub(side); }
int TaLogicCom_IsOpenUnit__0016b610(int a, int b, struct DotrRuleOrder *order) { return tail_stub(a, b, order); }
int TaLogic_IsOpenUnit__0013d3d0(int a, int b, struct DotrRuleOrder *order) { return tail_stub(a, b, order); }
int TaLogic_GetWarnHideLvl__0013da70(int side) { return side_stub(side); }
int TaLogic_IsOpenUnit__00117870(int a, int b, struct DotrRuleOrder *order) { return tail_stub(a, b, order); }
int TaLogic_GetWarnHideLvl__00117c30(int side) { return side_stub(side); }
int TaLogic_IsOpenUnit__001670a0(int a, int b, struct DotrRuleOrder *order) { return tail_stub(a, b, order); }
int TaLogic_GetWarnHideLvl__001676d0(int side) { return side_stub(side); }
int func_001254a0_free(int side) { return side_stub(side); }
int TaLogic_IsOpenUnit__00126380(int a, int b, struct DotrRuleOrder *order) { return tail_stub(a, b, order); }
void func_00126940_mode(struct DotrRuleOrder *order, unsigned char mode) { func_0027b1b0_mode(order, mode); }
int TaLogic_GetWarnHideLvl__00134ae0(int side) { return side_stub(side); }
int TaLogic_IsOpenUnit__00134b40(int a, int b, struct DotrRuleOrder *order) { return tail_stub(a, b, order); }
int TaLogic_GetWarnHideLvl__001461c0(int side) { return side_stub(side); }
int TaLogic_IsOpenUnit__00146220(int a, int b, struct DotrRuleOrder *order) { return tail_stub(a, b, order); }
int TaLogic_GetWarnHideLvl__00141b30(int side) { return side_stub(side); }
int TaLogic_IsOpenUnit__00141b90(int a, int b, struct DotrRuleOrder *order) { return tail_stub(a, b, order); }
int TaLogic_GetWarnHideLvl__0012b1b0(int side) { return side_stub(side); }
int TaLogic_IsOpenUnit__0012b210(int a, int b, struct DotrRuleOrder *order) { return tail_stub(a, b, order); }
void func_0012b830_mode(struct DotrRuleOrder *order, unsigned char mode) { func_0027b1b0_mode(order, mode); }
int TaLogic_GetWarnHideLvl__0011aa10(int side) { return side_stub(side); }
int func_0011c140_order(int a, int b, struct DotrRuleOrder *order) { return tail_stub(a, b, order); }
void func_0011c3e0_mode(struct DotrRuleOrder *order, unsigned char mode) { func_0027b1b0_mode(order, mode); }
int TaLogic_GetWarnHideLvl__0011fa80(int side) { return side_stub(side); }
int TaLogic_IsOpenUnit__001211b0(int x, int y, struct DotrRuleOrder *unused) { return tail_stub(x, y, unused); }
void func_001215e0_mode(struct DotrRuleOrder *order, unsigned char mode) { func_0027b1b0_mode(order, mode); }
int TaLogic_GetWarnHideLvl__00130780(int side) { return side_stub(side); }
static int tail_flag_seen;
int func_00130940_order(int a, int b, struct DotrRuleOrder *order, unsigned char flag)
{
    tail_flag_seen = flag;
    return tail_stub(a, b, order);
}
/* The five-argument copy behind the common four-argument shape: every tail call must carry the byte. */
static int reach_0012e820(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode)
{
    int value;

    tail_flag_seen = -1;
    value = TaLogic_GetMinimumTask_RunMission__0012e820(order, side, result, mode, 0x5a);
    assert(tail_calls == 0 || tail_flag_seen == 0x5a);
    return value;
}
int TaLogic_IsOpenUnit__00139180(int a, int b, struct DotrRuleOrder *order) { return tail_stub(a, b, order); }
int func_0016ae30_pick(int a, int b, struct DotrRuleOrder *order) { return tail_stub(a, b, order); }
int func_0016b0e0_free(int side) { return side_stub(side); }
/* 0x10fa60 asks these two itself; whatever they answer, its fifth argument of
 * TaLogic_GetUnitWarning is 0. */
int func_001df470_side(int side) { side_arg = side; return total_answer; }
int SzDuel_GetUnitMonsterCount(int side) { assert(side == side_arg); count_calls++; return count_answer; }
int func_0010ee00_order(int x, int y, struct DotrRuleOrder *order) { return tail_stub(x, y, order); }

static int found_answer = -1, found_arg, word_args[3], word_answer;
int TaLogic007_IsRevivePlay(int side) { found_arg = side; return found_answer; }
int TaLogic_GetGraveEntryAfterScreen(int index, int target, int mode)
{
    word_args[0] = index, word_args[1] = target, word_args[2] = mode;
    return word_answer;
}

static int packed_answer, packed_args[2];
int TaLogic_GetMaxAPGraveCard(int side, int mask) { packed_args[0] = side, packed_args[1] = mask; return packed_answer; }

static struct DotrRuleOrder order;
static struct DotrRuleResult result;

static void fill(int value)
{
    int i, j;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            score[i][j] = value;
            ok[i][j] = 1;
            taken[i][j] = 0;
        }
    }
}

static int run(reach_fn f, int position, int here, int rule)
{
    memset(&result, 0xee, sizeof(result));
    score_calls = pick_calls = tail_calls = done_calls = kind_calls = set_calls = edge_calls = scan_calls = blocked_calls = 0;
    order.position = position;
    here_answer = here;
    rule_answer = rule;
    return f(&order, 9, &result, 7);
}

/* The result as every path but the kind-14 one leaves it. */
static void filled(int position, int word, int flag)
{
    assert(result.position == position && result.kind == 6 && result.loc == word);
    assert(result.atkDefPosition == 0x5a && result.shouldFlipUp == 0x12345678);
    assert(pick_calls == 1 && pick_args[0] == position >> 8 && pick_args[1] == (position & 0xff));
    assert(pick_args[2] == word && pick_args[3] == (flag | away_flag) && pick_args[4] == last_flag);
    assert(tail_calls == 1 && tail_args[0] == position >> 8 && tail_args[1] == (position & 0xff) && tail_order == &order);
    assert(done_calls == 1 && done_order == &order && done_mode == 7);
}

/* Branch coverage, shared by the copies: first score zero; rule word zero; bit 4 set with
 * the kind 1 on the first read, 5 on the second, neither; the scan with the reach 1 and
 * 2, both lower and upper clamps, the distance filter, a refused cell, a blocked cell
 * with a non-zero and a zero position low byte, a lower score, an equal score with each
 * outcome of the two item reads, a pick equal to and different from the item's own cell,
 * and no pick. */
/* score_mode is the fourth argument of TaLogic_GetUnitWarning (0x276cc0; 3 in all copies but
 * 0x13be60, which passes 2); away_flag is the fourth argument of 0x27e360 for a pick away from
 * the item's own cell (0 in all copies but 0x115850, which passes 1 there too). */
static void check_reach_as(reach_fn f, int mode, int away)
{
    score_mode = mode;
    away_flag = away;
    pick_answer = 0x5a;
    side_answer = 0x1abcd;
    dfw_answer = 0x77;
    bit_answer = 0;
    blocked = 0;
    side_arg = -1;

    fill(0);
    assert(run(f, 0x0203, 0x0302, 0x11) == 0);
    /* no_side: the copy that never asks for the side passes 0 as the fifth argument of
     * TaLogic_GetUnitWarning. */
    assert(cell_args[0] == 2 && cell_args[1] == 3 && side_arg == (no_side ? -1 : 9));
    assert(score_calls == 1 && score_args[0] == 2 && score_args[1] == 3 && score_args[2] == 0x0203);
    assert(score_args[3] == score_mode && score_args[4] == (no_side || zero_side ? 0 : 0xabcd) && score_args[5] == last_flag);
    filled(0x0203, 0x0302, 1);
    assert(scan_calls == 0);

    fill(5);
    assert(run(f, 0x0203, 0x0302, 0) == 0);
    assert(rule_args[0] == 0x77 && rule_args[1] == 2 && rule_args[2] == 3);
    filled(0x0203, 0x0302, 1);
    assert(scan_calls == 0);

    bit_answer = 1;
    if (!no_gate) {
    kind_seq[0] = 1 << 5, kind_seq[1] = 0;
    assert(run(f, 0x0203, 0x0302, 0x11) == 0);
    assert(bit_args[0] == 2 && bit_args[1] == 3 && bit_args[2] == 4 && kind_calls == 1);
    assert(set_calls == 1 && set_args[0] == 2 && set_args[1] == 3 && set_args[2] == 0 && set_args[3] == 1);
    assert(result.position == 0x0203 && result.kind == 14 && result.loc == 0xeeee);
    assert(pick_calls == 0 && tail_calls == 0 && done_calls == 0 && scan_calls == 0);
    kind_seq[0] = 0, kind_seq[1] = 5 << 5;
    assert(run(f, 0x0203, 0x0302, 0x11) == 0);
    assert(kind_calls == 2 && set_calls == 1 && result.kind == 14 && done_calls == 0);
    }

    /* Neither kind: the scan runs. Reach 1 about (2,3): five cells pass the distance
     * filter; (1,3) scores 4 first, (2,4) ties and is free while (1,3) is taken. */
    kind_seq[0] = 0, kind_seq[1] = 0;
    score[1][3] = 4, score[2][2] = 9, score[2][4] = 4, score[3][3] = 7;
    taken[1][3] = 1;
    assert(run(f, 0x0203, 0x0302, 0x11) == 0);
    /* no_gate: the copies without the 0x1e2100 gate never ask for the kind. */
    assert(kind_calls == (no_gate ? 0 : 2) && set_calls == 0 && scan_calls == 5 && blocked_calls == 5 && score_calls == 6);
    assert(scan_args[0] == 2 && scan_args[1] == 3 && scan_args[2] == 0 && scan_order == &order && edge_calls == 0);
    assert(score_args[2] == 0x0203 && score_args[3] == score_mode && score_args[4] == (no_side || zero_side ? 0 : 0xabcd) && score_args[5] == last_flag);
    filled(0x0203, 0x0402, 0);
    bit_answer = 0;
    assert(run(f, 0x0203, 0x0302, 0x11) == 0 && kind_calls == 0);
    filled(0x0203, 0x0402, 0);
    /* The tying cell is taken, then both are free: the first pick stays. */
    taken[2][4] = 1;
    assert(run(f, 0x0203, 0x0302, 0x11) == 0);
    filled(0x0203, 0x0301, 0);
    taken[2][4] = 0, taken[1][3] = 0;
    assert(run(f, 0x0203, 0x0302, 0x11) == 0);
    filled(0x0203, 0x0301, 0);
    /* Only the item's own cell is allowed: picked, flag 1. A refused scan: no pick. */
    memset(ok, 0, sizeof(ok));
    ok[2][3] = 1;
    assert(run(f, 0x0203, 0x0302, 0x11) == 0 && scan_calls == 5 && blocked_calls == 1 && score_calls == 2);
    filled(0x0203, 0x0302, 1);
    ok[2][3] = 0;
    assert(run(f, 0x0203, 0x0302, 0x11) == 0 && scan_calls == 5 && blocked_calls == 0 && score_calls == 1);
    filled(0x0203, 0x0302, 1);

    /* Blocked cells count only when the position's low byte is zero; that position also
     * asks 0x27bbc0 for the byte passed to the scan. */
    fill(5);
    blocked = 1;
    assert(run(f, 0x0203, 0x0302, 0x11) == 0 && blocked_calls == 5 && score_calls == 1 && edge_calls == 0);
    filled(0x0203, 0x0302, 1);
    edge_answer = 0x1c3;
    score[3][3] = 2;
    assert(run(f, 0x0200, 0x0302, 0x11) == 0 && blocked_calls == 5 && score_calls == 6);
    assert(edge_calls == 1 && edge_args[0] == 2 && edge_args[1] == 0 && edge_args[2] == last_flag && scan_args[2] == 0xc3);
    assert(blocked_last == 0x0303);
    filled(0x0200, 0x0303, 0);
    blocked = 0;

    /* Clamps: reach 2 at (6,6) and (0,0) leaves six cells each; at (5,3) the column 7 is
     * cut from the thirteen; at (4,3) nothing is cut. */
    assert(run(f, 0x0203, 0x0606, 0x12) == 0 && scan_calls == 6);
    assert(run(f, 0x0203, 0x0000, 0x12) == 0 && scan_calls == 6);
    assert(run(f, 0x0203, 0x0305, 0x12) == 0 && scan_calls == 12);
    assert(run(f, 0x0203, 0x0304, 0x12) == 0 && scan_calls == 13);
    assert(run(f, 0x0203, 0x0503, 0x12) == 0 && scan_calls == 12);
    assert(run(f, 0x0203, 0x0403, 0x12) == 0 && scan_calls == 13);
}

/* 0x12ed70 behind the common shape: its two early exits are held off here (checked on their own
 * in main) and its constant 2 at +0x0c is asserted, then reported as the tail call the shared
 * checks expect. */
static int reach_0012ed70(struct DotrRuleOrder *o, int side, struct DotrRuleResult *r, unsigned char mode)
{
    int value;
    int bit = bit_answer;

    bit_answer = 0;
    found_answer = -1;
    value = TaLogic_GetMinimumTask_ReviveMission__0012ed70(o, side, r, mode);
    bit_answer = bit;
    assert(found_arg == side && tail_calls == 0 && set_calls == 0 && r->kind == 6 && r->shouldFlipUp == 2);
    r->shouldFlipUp = 0x12345678;
    tail_calls = 1, tail_args[0] = o->position >> 8, tail_args[1] = o->position & 0xff, tail_order = o;
    return value;
}

/* 0x129700 behind the common shape: 0x12b210 is asked first (answer other than 1 here; the 1 exit
 * and the bit 4 exit are checked on their own in main) and the constant 2 at +0x0c is asserted. */
static int reach_00129700(struct DotrRuleOrder *o, int side, struct DotrRuleResult *r, unsigned char mode)
{
    int value;
    int bit = bit_answer;

    bit_answer = 0;
    value = TaLogic_GetMinimumTask_ReviveMission__00129700(o, side, r, mode);
    bit_answer = bit;
    assert(tail_calls == 1 && tail_args[0] == o->position >> 8 && tail_args[1] == (o->position & 0xff) && tail_order == o);
    assert(set_calls == 0 && r->kind == 6 && r->shouldFlipUp == 2);
    r->shouldFlipUp = 0x12345678;
    return value;
}

static void check_reach(reach_fn f)
{
    check_reach_as(f, 3, 0);
}

static int field[8][8], virus_answer, virus_calls, virus_args[3];
int SzDuel_GetField(int x, int y)
{
    assert(x >= 0 && x < 7 && y >= 0 && y < 7);
    return field[x][y];
}
int func_0024fe40_rule(int item, int x, int y)
{
    virus_calls++;
    virus_args[0] = item, virus_args[1] = x, virus_args[2] = y;
    return virus_answer;
}

/* TaLogic_IsUnitReadyMove (branch and loop coverage). */
static void ready_around(int x, int y, int value_taken, int value_field)
{
    static const int d[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    int k;

    for (k = 0; k < 4; k++) {
        if (x + d[k][0] >= 0 && x + d[k][0] < 7 && y + d[k][1] >= 0 && y + d[k][1] < 7) {
            taken[x + d[k][0]][y + d[k][1]] = value_taken;
            field[x + d[k][0]][y + d[k][1]] = value_field;
        }
    }
}

static void check_ready_move(void)
{
    int saved_here = here_answer, saved_bit = bit_answer, saved_dfw = dfw_answer, saved_rule = rule_answer;
    int k;

    memset(taken, 0, sizeof(taken));
    memset(field, 0, sizeof(field));
    dfw_answer = 0x1234;
    rule_answer = 0;
    /* no location: 0, nothing else asked */
    here_answer = -1;
    bit_answer = 0;
    bit_args[0] = -1;
    assert(TaLogic_IsUnitReadyMove(0x0203) == 0 && cell_args[0] == 2 && cell_args[1] == 3 && bit_args[0] == -1);
    /* status bit 0 set: 0 */
    here_answer = 0x0403;
    bit_answer = 1;
    rule_args[0] = -1;
    assert(TaLogic_IsUnitReadyMove(0x0203) == 0 && bit_args[0] == 2 && bit_args[1] == 3 && bit_args[2] == 0 && rule_args[0] == -1);
    /* free neighbour: 1; the move word is asked with (low byte, high byte) of the location */
    bit_answer = 0;
    assert(TaLogic_IsUnitReadyMove(0x0203) == 1 && rule_args[0] == 0x1234 && rule_args[1] == 3 && rule_args[2] == 4);
    /* all four orthogonal neighbours taken, diagonals and the own cell free: 0 */
    ready_around(3, 4, 1, 0);
    assert(TaLogic_IsUnitReadyMove(0x0203) == 0);
    /* each neighbour alone decides */
    for (k = 0; k < 4; k++) {
        static const int at[4][2] = {{2, 4}, {4, 4}, {3, 3}, {3, 5}};

        taken[at[k][0]][at[k][1]] = 0;
        assert(TaLogic_IsUnitReadyMove(0x0203) == 1);
        taken[at[k][0]][at[k][1]] = 1;
    }
    /* field 8 counts only with bit 4 of the move word */
    ready_around(3, 4, 0, 8);
    rule_answer = 0xef;
    assert(TaLogic_IsUnitReadyMove(0x0203) == 0);
    rule_answer = 0x10;
    assert(TaLogic_IsUnitReadyMove(0x0203) == 1);
    /* field 9: 0x24fe40 answering 1 rejects the cell, another answer accepts it */
    ready_around(3, 4, 0, 9);
    virus_answer = 1;
    virus_calls = 0;
    assert(TaLogic_IsUnitReadyMove(0x0203) == 0 && virus_calls == 4);
    assert(virus_args[0] == 0x1234 && virus_args[1] == 4 && virus_args[2] == 4);
    virus_answer = 2;
    virus_calls = 0;
    assert(TaLogic_IsUnitReadyMove(0x0203) == 1 && virus_calls == 1 && virus_args[1] == 2 && virus_args[2] == 4);
    ready_around(3, 4, 0, 0);
    /* corners: the scan stays inside 0..6 */
    here_answer = 0;
    ready_around(0, 0, 1, 0);
    assert(TaLogic_IsUnitReadyMove(0x0203) == 0);
    taken[0][1] = 0;
    assert(TaLogic_IsUnitReadyMove(0x0203) == 1);
    ready_around(0, 0, 0, 0);
    here_answer = 0x0606;
    ready_around(6, 6, 1, 0);
    assert(TaLogic_IsUnitReadyMove(0x0203) == 0);
    taken[6][5] = 0;
    assert(TaLogic_IsUnitReadyMove(0x0203) == 1);
    memset(taken, 0, sizeof(taken));
    here_answer = saved_here, bit_answer = saved_bit, dfw_answer = saved_dfw, rule_answer = saved_rule;
}

int main(void)
{
    check_reach(TaLogic_GetMinimumTask_RunMission__00149690);
    check_reach(TaLogic_GetMinimumTask_RunMission__0015b250);
    check_reach(TaLogic_GetMinimumTask_RunMission__00157480);
    check_reach(TaLogic_GetMinimumTask_RunMission__0014df80);
    check_reach(TaLogic_GetMinimumTask_RunMission__00152100);
    check_reach(TaLogic_GetMinimumTask_RunMission__001607d0);
    check_reach(TaLogic_GetMinimumTask_RunMission__00170940);
    check_reach_as(TaLogic_GetMinimumTask_RunMission__0013be60, 2, 0);
    check_reach_as(TaLogic_GetMinimumTask_RunMission__00115850, 3, 1);
    /* last_flag: the final argument of TaLogic_GetUnitWarning, TaLogic_IsDefStyleUnit,
     * TaLogic_IsAttackedNow and TaLogic_IsAttackedNow2 (1 in these two). */
    last_flag = 1;
    check_reach_as(TaLogic_GetMinimumTask_RunMission__001658f0, 2, 0);
    check_reach_as(TaLogic_GetMinimumTask_RunMission__00169a10, 2, 0);
    last_flag = 0;
    no_gate = 1;
    check_reach_as(TaLogic_GetMinimumTask_RunMission__00124f60, 2, 0);
    check_reach_as(TaLogic_GetMinimumTask_RunMission__001336a0, 3, 0);
    check_reach_as(TaLogic_GetMinimumTask_RunMission__00144ec0, 3, 0);
    check_reach_as(TaLogic_GetMinimumTask_RunMission__001402f0, 3, 0);
    check_reach_as(TaLogic_GetMinimumTask_RunMission__001291c0, 3, 0);
    check_reach_as(TaLogic_GetMinimumTask_RunMission__0011a4d0, 3, 1);
    check_reach_as(TaLogic_GetMinimumTask_RunMission__0011f540, 3, 0);
    check_reach_as(reach_0012e820, 3, 0);
    last_flag = no_side = 1;
    check_reach_as(TaLogic_GetMinimumTask_RunMission__001374d0, 3, 0);
    last_flag = no_side = 0;
    /* 0x10fa60: total 1500 does not ask for the count, 1501 does; count 3 and 4 both give 0. */
    zero_side = 1;
    total_answer = 1500;
    check_reach_as(TaLogic_GetMinimumTask_RunMission__0010fa60, 3, 1);
    assert(count_calls == 0);
    total_answer = 1501, count_answer = 3;
    check_reach_as(TaLogic_GetMinimumTask_RunMission__0010fa60, 3, 1);
    assert(count_calls != 0);
    count_answer = 4;
    check_reach_as(TaLogic_GetMinimumTask_RunMission__0010fa60, 3, 1);
    zero_side = 0;
    check_reach_as(reach_0012ed70, 3, 0);
    /* 0x12ed70's own exits: found 0 and above (kind 12), then bit 4 (kind 14), then neither. */
    fill(0);
    bit_answer = 1;
    word_answer = 0x21;
    found_answer = 0x00050007;
    assert(run(TaLogic_GetMinimumTask_ReviveMission__0012ed70, 0x0203, 0x0302, 0x11) == 0 && found_arg == 9);
    assert(word_args[0] == 5 && word_args[1] == 7 && word_args[2] == 30);
    assert(result.position == 0x0203 && result.kind == 12 && result.loc == 0x0521);
    assert(result.atkDefPosition == 0x5a && result.shouldFlipUp == 1 && pick_calls == 1 && pick_args[0] == 2 && pick_args[1] == 3);
    assert(pick_args[2] == 0xeeee && pick_args[3] == 1 && pick_args[4] == 0);
    assert(done_calls == 1 && done_order == &order && done_mode == 7 && score_calls == 0 && set_calls == 0);
    found_answer = 0;
    assert(run(TaLogic_GetMinimumTask_ReviveMission__0012ed70, 0x0203, 0x0302, 0x11) == 0 && result.kind == 12 && result.loc == 0x21);
    found_answer = -1;
    assert(run(TaLogic_GetMinimumTask_ReviveMission__0012ed70, 0x0203, 0x0302, 0x11) == 0);
    assert(bit_args[0] == 2 && bit_args[1] == 3 && bit_args[2] == 4 && kind_calls == 0);
    assert(set_calls == 1 && set_args[0] == 2 && set_args[1] == 3 && set_args[2] == 0 && set_args[3] == 1);
    assert(result.position == 0x0203 && result.kind == 14 && result.loc == 0xeeee);
    assert(pick_calls == 0 && done_calls == 0 && score_calls == 0);
    bit_answer = 0;
    assert(run(TaLogic_GetMinimumTask_ReviveMission__0012ed70, 0x0203, 0x0302, 0x11) == 0 && set_calls == 0 && score_calls == 1 && result.kind == 6);
    assert(result.shouldFlipUp == 2 && done_calls == 1);
    check_reach_as(reach_00129700, 3, 0);
    /* 0x129700's own exits: 0x12b210 answering 1 (kind 12), then bit 4 (kind 14), then neither. */
    fill(0);
    bit_answer = 1;
    word_answer = 0x21;
    tail_answer = 1;
    packed_answer = 0x35079999;
    assert(run(TaLogic_GetMinimumTask_ReviveMission__00129700, 0x0203, 0x0302, 0x11) == 0 && tail_calls == 1 && tail_args[0] == 2 && tail_args[1] == 3);
    assert(tail_order == &order && packed_args[0] == 9 && packed_args[1] == 3);
    assert(word_args[0] == 0x35 && word_args[1] == 7 && word_args[2] == 29);
    assert(result.position == 0x0203 && result.kind == 12 && result.loc == 0x0521);
    assert(result.atkDefPosition == 0x5a && result.shouldFlipUp == 1 && pick_calls == 1 && pick_args[0] == 2 && pick_args[1] == 3);
    assert(pick_args[2] == 0xeeee && pick_args[3] == 1 && pick_args[4] == 0);
    assert(done_calls == 1 && done_order == &order && done_mode == 7 && score_calls == 0 && set_calls == 0);
    tail_answer = 2;
    assert(run(TaLogic_GetMinimumTask_ReviveMission__00129700, 0x0203, 0x0302, 0x11) == 0);
    assert(bit_args[0] == 2 && bit_args[1] == 3 && bit_args[2] == 4 && kind_calls == 0);
    assert(set_calls == 1 && set_args[0] == 2 && set_args[1] == 3 && set_args[2] == 0 && set_args[3] == 1);
    assert(result.position == 0x0203 && result.kind == 14 && result.loc == 0xeeee);
    assert(pick_calls == 0 && done_calls == 0 && score_calls == 0);
    bit_answer = 0;
    assert(run(TaLogic_GetMinimumTask_ReviveMission__00129700, 0x0203, 0x0302, 0x11) == 0 && set_calls == 0 && score_calls == 1 && result.kind == 6);
    assert(result.shouldFlipUp == 2 && done_calls == 1);
    tail_answer = 0x12345678;
    no_gate = 0;
    check_ready_move();
    return 0;
}
