/* Synthetic host tests for the atk_missions group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/atk_missions.h"

typedef int (*atk_fn)(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode);

static int unit_by[2], unit_calls, unit_args[8][2], unit_zero_at;
static int loc_by[2], loc_calls, loc_args[2][2];
static struct DotrRuleOrder *done_order, *open_order;
static int done_calls, done_mode;
static int open_answer, open_args[2];
static int rev_answer, rev_args[3];
static int side_answer, side_calls, side_arg;
static int warn_by[12], warn_calls, warn_args[12][6];
static int ap_answer, ap_args[3];
static int def_answer, def_calls, def_args[5];
static int root_args[11], root_cells[12][2], goal_answer;
static int move_at, move_calls, move_args[6];
static int root_answer, move_root;

int func_001df540_word(int a, int b) { unit_args[unit_calls][0] = a, unit_args[unit_calls][1] = b; if (unit_calls == unit_zero_at) { unit_calls++; return 0; } return unit_by[unit_calls++ & 1]; }
void func_0027b1b0_mode(struct DotrRuleOrder *order, unsigned char mode) { done_calls++; done_order = order; done_mode = mode; }
void func_0012b830_mode(struct DotrRuleOrder *order, unsigned char mode) { func_0027b1b0_mode(order, mode); }
int SzDuel_GetUnitLoc(int a, int b) { loc_args[loc_calls][0] = a, loc_args[loc_calls][1] = b; return loc_by[loc_calls++ & 1]; }
int func_002464f0_rule(int item, int x, int y) { rev_args[0] = item, rev_args[1] = x, rev_args[2] = y; return rev_answer; }
int TaLogic_GetUnitWarning(int a, int b, int c, int d, int e, unsigned char f)
{
    int *w = warn_args[warn_calls];
    w[0] = a, w[1] = b, w[2] = c, w[3] = d, w[4] = e, w[5] = f;
    return warn_by[warn_calls++];
}
int SzDuel_GetUnitAP(int a, int b, int flag) { ap_args[0] = a, ap_args[1] = b, ap_args[2] = flag; return ap_answer; }
int TaLogic_IsDefStyleUnit(int a, int b, unsigned short c, unsigned char d, int e)
{
    def_calls++;
    def_args[0] = a, def_args[1] = b, def_args[2] = c, def_args[3] = d, def_args[4] = e;
    return def_answer;
}
int func_0027a090_rule(int a, int b, struct DotrRuleNibbles *cells, int d, int e, int f, int g, int h, int i,
                       unsigned char j, unsigned short k)
{
    int n;
    root_args[0] = a, root_args[1] = b, root_args[2] = d, root_args[3] = e, root_args[4] = f, root_args[5] = g;
    root_args[6] = h, root_args[7] = i, root_args[8] = j, root_args[9] = k;
    for (n = 0; n < 12; n++) {
        assert(cells[n].low == 15 && cells[n].high == 15);
        cells[n].low = root_cells[n][0];
        cells[n].high = root_cells[n][1];
    }
    return root_answer;
}
int func_0027afc0_rule(struct DotrRuleNibbles *cells) { (void)cells; return goal_answer; }
static int side_stub(int side) { side_calls++; side_arg = side; return side_answer; }
static int open_stub(int a, int b, struct DotrRuleOrder *order) { open_args[0] = a, open_args[1] = b; open_order = order; return open_answer; }
static int move_stub(int a, int b, int x1, int y1, int x2, int y2)
{
    move_args[0] = a, move_args[1] = b, move_args[2] = x1, move_args[3] = y1, move_args[4] = x2, move_args[5] = y2;
    return move_calls++ == move_at;
}
/* One line per copy: its own unit's IsOpenUnit, GetWarnHideLvl and IsOpenMoveUnit. */
#define COPY_STUBS(open, warn, move) \
    int open(int a, int b, struct DotrRuleOrder *order) { return open_stub(a, b, order); } \
    int warn(int side) { return side_stub(side); } \
    int move(int a, int b, int x1, int y1, int x2, int y2) { return move_stub(a, b, x1, y1, x2, y2); }
COPY_STUBS(TaLogic_IsOpenUnit__0012b210, TaLogic_GetWarnHideLvl__0012b1b0, TaLogic006_IsOpenMoveUnit)
COPY_STUBS(TaLogic_IsOpenUnit__00134b40, TaLogic_GetWarnHideLvl__00134ae0, TaLogic008_IsOpenMoveUnit)
COPY_STUBS(TaLogicCom_IsOpenUnit__0016b610, TaLogic_GetWarnHideLvl__00172310, TaLogic022_IsOpenMoveUnit)
COPY_STUBS(TaLogic_IsOpenUnit__001620b0, TaLogic_GetWarnHideLvl__00162580, TaLogic019_IsOpenMoveUnit)
COPY_STUBS(TaLogic_IsOpenUnit__0014f490, TaLogic_GetWarnHideLvl__0014f9b0, TaLogic014_IsOpenMoveUnit)
COPY_STUBS(TaLogic_IsOpenUnit__0015cbb0, TaLogic_GetWarnHideLvl__0015d120, TaLogic018_IsOpenMoveUnit)
COPY_STUBS(TaLogic_IsOpenUnit__00141b90, TaLogic_GetWarnHideLvl__00141b30, TaLogic011_IsOpenMoveUnit)
COPY_STUBS(TaLogic_IsOpenUnit__00146220, TaLogic_GetWarnHideLvl__001461c0, TaLogic012_IsOpenMoveUnit)
COPY_STUBS(TaLogic_IsOpenUnit__0014ae60, TaLogic_GetWarnHideLvl__0014b330, TaLogic013_IsOpenMoveUnit)
COPY_STUBS(TaLogic_IsOpenUnit__0013d3d0, TaLogic_GetWarnHideLvl__0013da70, TaLogic010_IsOpenMoveUnit)
COPY_STUBS(TaLogic_IsOpenUnit__001670a0, TaLogic_GetWarnHideLvl__001676d0, TaLogic020_IsOpenMoveUnit)
COPY_STUBS(TaLogic_IsOpenUnit__0016ae30, TaLogic_GetWarnHideLvl__0016b0e0, TaLogic021_IsOpenMoveUnit)
COPY_STUBS(TaLogic_IsOpenUnit__00153ae0, TaLogic_GetWarnHideLvl__00153ed0, TaLogic015_IsOpenMoveUnit)
int TaLogic_IsOpenUnit__00117870(int a, int b, struct DotrRuleOrder *order) { return open_stub(a, b, order); }
int TaLogic_GetWarnHideLvl__00117c30(int side) { return side_stub(side); }
int TaLogic_GetWarnHideLvl__00158780(int side) { return side_stub(side); }
static int active_answer, active_calls;
int TaLogic015_IsActiveMode(void) { active_calls++; return active_answer; }
int TaLogic_IsOpenUnit__00139180(int a, int b, struct DotrRuleOrder *order) { return open_stub(a, b, order); }
int TaLogic009_IsOpenMoveUnit(int a, int b, int x1, int y1, int x2, int y2) { return move_stub(a, b, x1, y1, x2, y2); }
/* 0 for the copy that passes a constant 0 instead of a warning level */
static int level = 0x1234;

/* The copies with the level body expanded in place: the life-point and monster-count answers,
 * and the unit's own order reset (counted apart; `own_init`, `own_found`, `own_last` say where a
 * copy is expected to use it). */
static int lp_answer, count_answer, own_calls, own_init, own_found, own_last, found_resets;
int func_001df470_side(int side) { assert(side == 1); side_arg = side; return lp_answer; }
int SzDuel_GetUnitMonsterCount(int side) { assert(side == 1); return count_answer; }
int func_0011c140_order(int a, int b, struct DotrRuleOrder *order) { return open_stub(a, b, order); }
int TaLogic_IsOpenUnit__00126380(int a, int b, struct DotrRuleOrder *order) { return open_stub(a, b, order); }
int TaLogic003_IsOpenMoveUnit(int a, int b, int x1, int y1, int x2, int y2) { return move_stub(a, b, x1, y1, x2, y2); }
int TaLogic005_IsOpenMoveUnit(int a, int b, int x1, int y1, int x2, int y2) { return move_stub(a, b, x1, y1, x2, y2); }
void func_0011c3e0_mode(struct DotrRuleOrder *order, unsigned char mode) { own_calls++; func_0027b1b0_mode(order, mode); }
void func_00126940_mode(struct DotrRuleOrder *order, unsigned char mode) { own_calls++; func_0027b1b0_mode(order, mode); }
int TaLogic_IsOpenUnit__001211b0(int a, int b, struct DotrRuleOrder *order) { return open_stub(a, b, order); }
int TaLogic004_IsOpenMoveUnit(int a, int b, int x1, int y1, int x2, int y2, int kind) { move_root = kind; return move_stub(a, b, x1, y1, x2, y2); }
void func_001215e0_mode(struct DotrRuleOrder *order, unsigned char mode) { own_calls++; func_0027b1b0_mode(order, mode); }

/* 0x12f350: its IsOpenUnit takes the function's fifth argument as well. */
static int open_flag;
int TaLogic_IsOpenUnit__00130940(int a, int b, struct DotrRuleOrder *o, int flag) { open_flag = flag; return open_stub(a, b, o); }
int TaLogic_GetWarnHideLvl__00130780(int side) { return side_stub(side); }
int TaLogic007_IsOpenMoveUnit(int a, int b, int x1, int y1, int x2, int y2) { return move_stub(a, b, x1, y1, x2, y2); }
static int atk_0012f350(struct DotrRuleOrder *o, int side, struct DotrRuleResult *r, unsigned char mode)
{
    return TaLogic_GetMinimumTask_AtkMission__0012f350(o, side, r, mode, 0x55);
}

static struct DotrRuleOrder order;
static struct DotrRuleResult result;

static void reset(void)
{
    memset(&result, 0, sizeof result);
    memset(warn_by, 0, sizeof warn_by);
    memset(root_cells, 0, sizeof root_cells);
    unit_calls = loc_calls = done_calls = own_calls = side_calls = warn_calls = def_calls = move_calls = 0;
    done_mode = -1;
    done_order = open_order = NULL;
    open_args[0] = open_args[1] = 0;
    order.position = 0x0305;
    order.flags.word = 0x0102;
    unit_by[0] = 1, unit_by[1] = 77;
    loc_by[0] = 0x0204, loc_by[1] = 0x0603;
    open_answer = 9, rev_answer = 1, side_answer = 0x1234, ap_answer = 50, def_answer = 0x5a;
    goal_answer = 3, move_at = -1, unit_zero_at = -1;
}

/* The loop over the goal cells: `kind` is the copy's fourth GetUnitWarning argument there;
 * `kind1` is that argument of the first call and `flag` the fifth argument of both; `style` is
 * the last IsDefStyleUnit argument of every call and `zero` the ninth root-search argument. */
#define check_atk(fn, kind) check_atk_full(fn, 3, kind, 1, 0, 0)
static void check_atk_full(atk_fn fn, int kind1, int kind, int flag, int style, int zero)
{
    /* No unit at the target cell: the order is reset with mode 1 and the answer is -1. Mode 1
     * reads the target from the high half of the flags word, other modes from the low half. */
    reset();
    unit_by[0] = 0;
    order.flags.word = 0x0a0b0102;
    assert(fn(&order, 1, &result, 1) == -1);
    assert(unit_args[0][0] == 0x0a && unit_args[0][1] == 0x0b && done_calls == 1 && done_order == &order && done_mode == 1 && own_calls == 0);
    reset();
    unit_by[0] = 0;
    order.flags.word = 0x0a0b0102;
    assert(fn(&order, 1, &result, 0) == -1);
    assert(unit_args[0][0] == 0x01 && unit_args[0][1] == 0x02 && done_calls == 1 && done_mode == 1);

    /* The unit cannot move: it stays at its own cell; the style flag is 0 when the cell's
     * warning is below the unit's attack and 1 otherwise (boundary: equal). */
    reset();
    rev_answer = 0, warn_by[0] = 49;
    assert(fn(&order, 1, &result, 2) == 0);
    assert(open_args[0] == 3 && open_args[1] == 5 && open_order == &order);
    assert(loc_args[0][0] == 3 && loc_args[0][1] == 5 && loc_args[1][0] == 1 && loc_args[1][1] == 2);
    assert(rev_args[0] == 77 && rev_args[1] == 4 && rev_args[2] == 2);
    assert(result.position == 0x0305 && result.kind == 6 && result.loc == 0x0204 && result.atkDefPosition == 0x5a);
    assert(warn_args[0][0] == 4 && warn_args[0][1] == 2 && warn_args[0][2] == 1 && warn_args[0][3] == kind1 &&
           warn_args[0][4] == flag && warn_args[0][5] == (level & 0xff) && (level ? side_arg == 1 : side_calls == 0));
    assert(ap_args[0] == 3 && ap_args[1] == 5 && ap_args[2] == 0);
    assert(def_args[0] == 3 && def_args[1] == 5 && def_args[2] == 0x0204 && def_args[3] == 0 && def_args[4] == style);
    assert(done_calls == 1 && done_order == &order && done_mode == 2 && own_calls == own_init);
    reset();
    rev_answer = 0, warn_by[0] = 50;
    assert(fn(&order, 1, &result, 2) == 0);
    assert(def_args[3] == 1 && def_args[4] == style && done_calls == 1);

    /* No goal cell: own cell, IsOpenUnit's answer in unknown_0c, order reset with the mode. */
    reset();
    goal_answer = 0;
    assert(fn(&order, 1, &result, 0) == 0);
    assert(root_args[0] == 3 && root_args[1] == 5 && root_args[2] == 4 && root_args[3] == 2 && root_args[4] == 3 &&
           root_args[5] == 6 && root_args[6] == 0x0102 && root_args[7] == zero && root_args[8] == 30 && root_args[9] == level);
    assert(result.position == 0x0305 && result.kind == 6 && result.loc == 0x0204 && result.shouldFlipUp == 9);
    assert(def_args[2] == 0x0204 && def_args[3] == 0 && def_args[4] == style && done_calls == 1 && done_mode == 0 && move_calls == 0 && own_calls == 0);

    /* The lowest warning wins; the first of two equal ones is kept when it is not farther. */
    reset();
    root_cells[0][0] = 1, root_cells[0][1] = 1;
    root_cells[1][0] = 2, root_cells[1][1] = 6;
    root_cells[2][0] = 4, root_cells[2][1] = 6;
    warn_by[0] = 30, warn_by[1] = 20, warn_by[2] = 20;
    assert(fn(&order, 1, &result, 0) == 0);
    assert(move_calls == 3 && move_args[0] == 3 && move_args[1] == 5 && move_args[2] == 4 && move_args[3] == 2 &&
           move_args[4] == 3 && move_args[5] == 6);
    assert(warn_calls == 3 && warn_args[1][0] == 2 && warn_args[1][1] == 6 && warn_args[1][2] == 1 &&
           warn_args[1][3] == kind && warn_args[1][4] == flag && warn_args[1][5] == (level & 0xff));
    assert(result.position == 0x0305 && result.kind == 6 && result.loc == 0x0602 && result.shouldFlipUp == 0);
    assert(def_args[2] == 0x0602 && def_args[3] == 0 && def_args[4] == style && result.atkDefPosition == 0x5a && done_calls == found_resets && own_calls == own_found);
    /* An equal warning nearer to the target cell (3, 6) replaces the kept one. */
    reset();
    root_cells[0][0] = 1, root_cells[0][1] = 6;
    root_cells[1][0] = 2, root_cells[1][1] = 6;
    root_cells[2][0] = 5, root_cells[2][1] = 7;
    warn_by[0] = 20, warn_by[1] = 20, warn_by[2] = 20;
    assert(fn(&order, 1, &result, 0) == 0);
    assert(result.loc == 0x0602 && done_calls == found_resets && own_calls == own_found);

    /* An open move from a goal cell ends the loop: that entry becomes the target cell and the
     * found flag goes to unknown_0c. */
    reset();
    root_cells[0][0] = 1, root_cells[0][1] = 1;
    root_cells[1][0] = 2, root_cells[1][1] = 2;
    warn_by[0] = 5;
    move_at = 1;
    assert(fn(&order, 1, &result, 0) == 0);
    assert(move_calls == 2 && warn_calls == 1);
    assert(result.loc == 0x0603 && result.shouldFlipUp == 1 && def_args[2] == 0x0603 && done_calls == found_resets && own_calls == own_found);

    /* No goal cell under the starting bound 0xffff: own cell, found flag 0, order reset. */
    reset();
    goal_answer = 1;
    warn_by[0] = 0x10000;
    assert(fn(&order, 1, &result, 3) == 0);
    assert(result.position == 0x0305 && result.kind == 6 && result.loc == 0x0204 && result.shouldFlipUp == 0);
    assert(def_args[2] == 0x0204 && def_args[4] == style && done_calls == 1 && done_mode == 3 && own_calls == own_last);
}

static void check_inlined(atk_fn fn, int kind1, int kind, int bound, int low, int few, int many)
{
    lp_answer = bound, count_answer = 3, level = low;
    check_atk_full(fn, kind1, kind, 1, 0, 0);
    lp_answer = bound + 1, count_answer = 2, level = few;
    check_atk_full(fn, kind1, kind, 1, 0, 0);
    lp_answer = bound + 1, count_answer = 3, level = many;
    check_atk_full(fn, kind1, kind, 1, 0, 0);
}

/* 0x115e20 and 0x156fc0: no style or open-move callee; one run per branch outcome. `opens` says
 * whether the copy calls its IsOpenUnit. */
static void check_plain(atk_fn fn, int opens)
{
    int n;
    /* no unit at the spot: order reset with mode 1 */
    reset();
    unit_by[0] = 0;
    assert(fn(&order, 1, &result, 0) == -1);
    assert(unit_args[0][0] == 1 && unit_args[0][1] == 2 && done_calls == 1 && done_order == &order && done_mode == 1);
    assert(loc_calls == 0 && result.kind == 0);
    /* mode 1 reads the spot from the upper half of the word */
    reset();
    order.flags.word = 0x07080000;
    unit_by[0] = 0;
    assert(fn(&order, 1, &result, 1) == -1 && unit_args[0][0] == 7 && unit_args[0][1] == 8);
    /* cannot move: stays, flag by warning against attack points (at and below the bound) */
    for (n = 0; n < 2; n++) {
        reset();
        rev_answer = 0;
        warn_by[0] = 50 - n;
        assert(fn(&order, 1, &result, 4) == 0);
        assert(opens ? open_args[0] == 3 && open_args[1] == 5 && open_order == &order : open_order == NULL);
        assert(loc_calls == 2 && loc_args[0][0] == 3 && loc_args[0][1] == 5 && loc_args[1][0] == 1 && loc_args[1][1] == 2);
        assert(rev_args[0] == 77 && rev_args[1] == 4 && rev_args[2] == 2);
        assert(result.position == 0x0305 && result.kind == 6 && result.loc == 0x0204);
        assert(warn_calls == 1 && warn_args[0][0] == 4 && warn_args[0][1] == 2 && warn_args[0][2] == 1);
        assert(warn_args[0][3] == 3 && warn_args[0][4] == 1 && warn_args[0][5] == 0x34 && side_arg == 1);
        assert(ap_args[0] == 3 && ap_args[1] == 5 && ap_args[2] == 0);
        assert(result.atkDefPosition == (n == 0) && done_calls == 1 && done_mode == 4);
    }
    /* no goal cell */
    reset();
    goal_answer = 0;
    assert(fn(&order, 1, &result, 4) == -1);
    assert(root_args[0] == 3 && root_args[1] == 5 && root_args[2] == 4 && root_args[3] == 2 && root_args[4] == 3);
    assert(root_args[5] == 6 && root_args[6] == 0x0102 && root_args[7] == 0 && root_args[8] == 30 && root_args[9] == 0x1234);
    assert(result.position == 0x0305 && result.kind == 6 && result.loc == 0x0204);
    assert(result.shouldFlipUp == 2 && result.atkDefPosition == 2 && done_calls == 1 && done_mode == 4 && warn_calls == 0);
    /* lowest warning wins; a tie goes to the cell nearer the spot (3, 6), else the earlier one stays */
    reset();
    root_cells[0][0] = 9, root_cells[0][1] = 9;
    root_cells[1][0] = 1, root_cells[1][1] = 1;
    root_cells[2][0] = 3, root_cells[2][1] = 5;
    warn_by[0] = 8, warn_by[1] = 5, warn_by[2] = 5;
    assert(fn(&order, 1, &result, 4) == 0);
    assert(warn_calls == 3 && warn_args[1][0] == 1 && warn_args[1][1] == 1 && warn_args[1][2] == 1);
    assert(warn_args[1][3] == 2 && warn_args[1][4] == 1 && warn_args[1][5] == 0x34);
    assert(result.position == 0x0305 && result.kind == 6 && result.loc == 0x0503);
    assert(result.shouldFlipUp == 2 && result.atkDefPosition == 2 && done_calls == 0);
    reset();
    root_cells[0][0] = 3, root_cells[0][1] = 7;
    root_cells[1][0] = 1, root_cells[1][1] = 1;
    root_cells[2][0] = 4, root_cells[2][1] = 6;
    warn_by[0] = 5, warn_by[1] = 5, warn_by[2] = 5;
    assert(fn(&order, 1, &result, 4) == 0 && result.loc == 0x0703);
    /* every warning above the starting bound: nothing chosen */
    reset();
    warn_by[0] = warn_by[1] = warn_by[2] = 0x10000;
    assert(fn(&order, 1, &result, 4) == -1);
    assert(result.position == 0x0305 && result.kind == 6 && result.loc == 0x0204);
    assert(result.shouldFlipUp == 2 && result.atkDefPosition == 2 && done_calls == 1 && done_mode == 4);
}

/* 0x10fff0: stubs of the callees only this copy has, and one run per branch outcome. */
static int ap4_answer, ap4_args[4], card_answer, card_args[2], status_answer, status_args[3];
static int field_answer, field_args[2], land_answer, land_args[3], land_calls;
int TaLogic_IsOpenUnit__0010ee00(int x, int y, unsigned *record) { return open_stub(x, y, (struct DotrRuleOrder *)record); }
int TaUtil_GetUnitAP(int a, int b, int c, int d) { ap4_args[0] = a, ap4_args[1] = b, ap4_args[2] = c, ap4_args[3] = d; return ap4_answer; }
int SzDuel_GetUnitCardNo(int a, int b) { card_args[0] = a, card_args[1] = b; return card_answer; }
int SzDuel_GetUnitStatus(int a, int b, int bit) { status_args[0] = a, status_args[1] = b, status_args[2] = bit; return status_answer; }
int SzDuel_GetField(int x, int y) { field_args[0] = x, field_args[1] = y; return field_answer; }
int CheckLandAgreement2(int a, int b, int c) { land_calls++; land_args[0] = a, land_args[1] = b, land_args[2] = c; return land_answer; }
static void reset_0010fff0(void)
{
    reset();
    ap4_answer = 50, card_answer = 416, status_answer = 0, field_answer = 7, land_answer = 1, land_calls = 0;
    lp_answer = 1501, count_answer = 4;
}
static void check_0010fff0(void)
{
    atk_fn fn = TaLogic_GetMinimumTask_AtkMission__0010fff0;
    int n;
    /* no unit at the spot: order reset with mode 1 */
    reset_0010fff0();
    unit_by[0] = 0;
    assert(fn(&order, 1, &result, 0) == -1);
    assert(unit_args[0][0] == 1 && unit_args[0][1] == 2 && done_calls == 1 && done_order == &order && done_mode == 1);
    assert(loc_calls == 0 && result.kind == 0);
    /* mode 1 reads the spot from the upper half of the word */
    reset_0010fff0();
    order.flags.word = 0x07080000;
    unit_by[0] = 0;
    assert(fn(&order, 1, &result, 1) == -1 && unit_args[0][0] == 7 && unit_args[0][1] == 8);
    /* cannot move: stays, flag by warning against attack points (at and below the bound); the level
     * is 0 for every outcome of the expanded level body */
    for (n = 0; n < 6; n++) {
        reset_0010fff0();
        lp_answer = n < 2 ? 1500 : 1501, count_answer = n < 4 ? 3 : 4;
        rev_answer = 0;
        warn_by[0] = 50 - (n & 1);
        assert(fn(&order, 1, &result, 4) == 0);
        assert(open_args[0] == 3 && open_args[1] == 5 && open_order == &order);
        assert(loc_calls == 2 && loc_args[0][0] == 3 && loc_args[0][1] == 5 && loc_args[1][0] == 1 && loc_args[1][1] == 2);
        assert(rev_args[0] == 77 && rev_args[1] == 4 && rev_args[2] == 2);
        assert(result.position == 0x0305 && result.kind == 6 && result.loc == 0x0204);
        assert(warn_calls == 1 && warn_args[0][0] == 4 && warn_args[0][1] == 2 && warn_args[0][2] == 1);
        assert(warn_args[0][3] == 3 && warn_args[0][4] == 1 && warn_args[0][5] == 0 && side_arg == 1);
        assert(ap4_args[0] == 3 && ap4_args[1] == 5 && ap4_args[2] == 4 && ap4_args[3] == 2);
        assert(result.atkDefPosition == !(n & 1) && done_calls == 0 && def_calls == 0);
    }
    /* no goal cell: stays, with IsOpenUnit's answer and the style answer */
    reset_0010fff0();
    goal_answer = 0;
    assert(fn(&order, 1, &result, 4) == 0);
    assert(root_args[0] == 3 && root_args[1] == 5 && root_args[2] == 4 && root_args[3] == 2 && root_args[4] == 3);
    assert(root_args[5] == 6 && root_args[6] == 0x0102 && root_args[7] == 0 && root_args[8] == 30 && root_args[9] == 0);
    assert(result.position == 0x0305 && result.kind == 6 && result.loc == 0x0204);
    assert(result.shouldFlipUp == 9 && result.atkDefPosition == 0x5a && done_calls == 0 && warn_calls == 0);
    assert(def_calls == 1 && def_args[0] == 3 && def_args[1] == 5 && def_args[2] == 0x0204 && def_args[3] == 0 && def_args[4] == 0);
    /* spot not two steps away: no open-move calls; lowest warning wins; a tie goes to the cell nearer
     * the spot (3, 6), else the earlier one stays */
    reset_0010fff0();
    root_cells[0][0] = 9, root_cells[0][1] = 9;
    root_cells[1][0] = 1, root_cells[1][1] = 1;
    root_cells[2][0] = 3, root_cells[2][1] = 5;
    warn_by[0] = 8, warn_by[1] = 5, warn_by[2] = 5;
    assert(fn(&order, 1, &result, 4) == 0);
    assert(unit_calls == 2 && land_calls == 0);
    assert(warn_calls == 3 && warn_args[1][0] == 1 && warn_args[1][1] == 1 && warn_args[1][2] == 1);
    assert(warn_args[1][3] == 2 && warn_args[1][4] == 1 && warn_args[1][5] == 0);
    assert(result.position == 0x0305 && result.kind == 6 && result.loc == 0x0503);
    assert(result.shouldFlipUp == 0 && result.atkDefPosition == 0x5a && done_calls == 0);
    assert(def_calls == 1 && def_args[2] == 0x0503 && def_args[3] == 0 && def_args[4] == 0);
    reset_0010fff0();
    root_cells[0][0] = 3, root_cells[0][1] = 7;
    root_cells[1][0] = 1, root_cells[1][1] = 1;
    root_cells[2][0] = 4, root_cells[2][1] = 6;
    warn_by[0] = 5, warn_by[1] = 5, warn_by[2] = 5;
    assert(fn(&order, 1, &result, 4) == 0 && result.loc == 0x0703);
    /* every warning above the starting bound: nothing chosen, stays */
    reset_0010fff0();
    warn_by[0] = warn_by[1] = warn_by[2] = 0x10000;
    assert(fn(&order, 1, &result, 4) == 0);
    assert(result.position == 0x0305 && result.kind == 6 && result.loc == 0x0204);
    assert(result.shouldFlipUp == 0 && result.atkDefPosition == 0x5a && done_calls == 0 && def_args[2] == 0x0204);
    /* spot (3, 3) two steps from (4, 2) and every open-move test passes: the spot is taken at once */
    reset_0010fff0();
    loc_by[1] = 0x0303;
    assert(fn(&order, 1, &result, 4) == 0);
    assert(unit_calls == 3 && unit_args[2][0] == 3 && unit_args[2][1] == 5 && card_args[0] == 3 && card_args[1] == 5);
    assert(status_args[0] == 3 && status_args[1] == 5 && status_args[2] == 0 && field_args[0] == 4 && field_args[1] == 2);
    assert(land_calls == 1 && land_args[0] == 7 && land_args[1] == 3 && land_args[2] == 5);
    assert(warn_calls == 0 && result.loc == 0x0303 && result.shouldFlipUp == 1 && result.atkDefPosition == 0x5a);
    /* each failing open-move test in turn (no unit, card 417, a set status bit, land disagreement):
     * the cell goes through the warning comparison instead */
    for (n = 0; n < 4; n++) {
        reset_0010fff0();
        loc_by[1] = 0x0303;
        root_cells[0][0] = 9, root_cells[0][1] = 9;
        if (n == 0) unit_zero_at = 2;
        if (n == 1) card_answer = 417;
        if (n == 2) status_answer = 1;
        if (n == 3) land_answer = 2;
        warn_by[0] = 1, warn_by[1] = 2, warn_by[2] = 3;
        assert(fn(&order, 1, &result, 4) == 0);
        if (n == 0) {
            /* the unit is missing for the first cell only: that cell is compared, the next takes the spot */
            assert(warn_calls == 1 && land_calls == 1 && result.loc == 0x0303 && result.shouldFlipUp == 1);
        } else {
            assert(warn_calls == 3 && land_calls == (n == 3 ? 3 : 0));
            assert(result.loc == 0x0909 && result.shouldFlipUp == 0);
        }
    }
}

int main(void)
{
    check_0010fff0();
    check_plain(TaLogic_GetMinimumTask_AtkMission__00115e20, 1);
    check_plain(TaLogic_GetMinimumTask_AtkMission__00156fc0, 0);
    check_atk(TaLogic_GetMinimumTask_AtkMission__00129d00, 5);
    check_atk(TaLogic_GetMinimumTask_AtkMission__00133be0, 4);
    check_atk(TaLogic_GetMinimumTask_AtkMission__00170f10, 2);
    check_atk(TaLogic_GetMinimumTask_AtkMission__00160da0, 2);
    check_atk(TaLogic_GetMinimumTask_AtkMission__0014e550, 3);
    check_atk(TaLogic_GetMinimumTask_AtkMission__0015b820, 3);
    check_atk(TaLogic_GetMinimumTask_AtkMission__00140830, 3);
    check_atk(TaLogic_GetMinimumTask_AtkMission__00145400, 3);
    check_atk(TaLogic_GetMinimumTask_AtkMission__00149c60, 2);
    check_atk_full(TaLogic_GetMinimumTask_AtkMission__0013c430, 2, 2, 0, 0, 0);
    check_atk_full(TaLogic_GetMinimumTask_AtkMission__00165ec0, 2, 5, 0, 1, 1);
    check_atk_full(TaLogic_GetMinimumTask_AtkMission__00169fe0, 2, 4, 0, 1, 1);
    open_flag = 0;
    check_atk(atk_0012f350, 3);
    assert(open_flag == 0x55);
    active_answer = 0;
    check_atk(TaLogic_GetMinimumTask_AtkMission__001526d0, 2);
    active_answer = 0x100; /* kept as a byte: reads as zero */
    check_atk(TaLogic_GetMinimumTask_AtkMission__001526d0, 2);
    active_answer = 1;
    active_calls = 0;
    check_atk(TaLogic_GetMinimumTask_AtkMission__001526d0, 5);
    assert(active_calls > 0);
    level = 0;
    check_atk_full(TaLogic_GetMinimumTask_AtkMission__001379f0, 3, 2, 0, 1, 1);
    level = 0x1234;
    /* Level body expanded in place: one run per outcome (life points at and above the bound,
     * monster count below and at 3). 0x11aa70 answers 0 on every path. */
    own_init = own_found = found_resets = 1;
    own_last = 1;
    check_inlined(TaLogic_GetMinimumTask_AtkMission__0011aa70, 3, 3, 1500, 0, 0, 0);
    own_last = 0;
    check_inlined(TaLogic_GetMinimumTask_AtkMission__00125500, 2, 2, 1000, 2000, 1000, 500);
    root_answer = 0x4321, move_root = 0;
    check_inlined(TaLogic_GetMinimumTask_AtkMission__0011fae0, 3, 4, 1000, 2000, 1000, 500);
    assert(move_root == 0x4321);
    own_init = own_found = found_resets = 0;
    level = 0x1234;
    return 0;
}
