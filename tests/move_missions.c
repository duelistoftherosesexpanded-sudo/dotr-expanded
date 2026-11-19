/* Synthetic host tests for the move_missions group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/move_missions.h"

typedef int (*move_fn)(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag);

static struct DotrRuleOrder order, *done_order, *open_order;
static struct DotrRuleResult result;
static int loc_calls, loc_args[2], loc_answer;
static int def_calls, def_args[5], def_answer;
static int open_calls, open_args[2], open_answer;
static int done_calls, done_mode;
static int id_calls, id_args[2], id_answer;
static int root_calls, root_args[8], root_cells[12][2];
static int goal_answer;
static int root_raw;
static int unit_calls, unit_args[12][2];
static int warn_calls, warn_args[12][6], warn_by[12];
/* What the copy under test hands on: IsDefStyleUnit's last argument and GetUnitWarning's last three. */
static int want_last, want_warn[3];
static int lvl_calls, lvl_side;

int SzDuel_GetUnitLoc(int a, int b) { loc_calls++; loc_args[0] = a, loc_args[1] = b; return loc_answer; }
int TaLogic_IsDefStyleUnit(int a, int b, unsigned short c, unsigned char d, int e)
{
    def_calls++;
    def_args[0] = a, def_args[1] = b, def_args[2] = c, def_args[3] = d, def_args[4] = e;
    return def_answer;
}
static int open_stub(int a, int b, struct DotrRuleOrder *o) { open_calls++; open_args[0] = a, open_args[1] = b; open_order = o; return open_answer; }
int TaLogic_IsOpenUnit__00139180(int a, int b, struct DotrRuleOrder *o) { return open_stub(a, b, o); }
void func_0027b1b0_mode(struct DotrRuleOrder *o, unsigned char mode) { done_calls++; done_order = o; done_mode = mode; }
int SzDuel_GetMapUnitID(int x, int y) { id_calls++; id_args[0] = x, id_args[1] = y; return id_answer; }
int TaLogic_GetRoot(int a, int b, struct DotrRuleNibbles *cells, int d, int e, int f, int g, int h, unsigned char i)
{
    int n;
    root_calls++;
    root_args[0] = a, root_args[1] = b, root_args[2] = d, root_args[3] = e, root_args[4] = f, root_args[5] = g;
    root_args[6] = h, root_args[7] = i;
    for (n = 0; n < 12; n++) {
        assert(root_raw || (cells[n].low == 15 && cells[n].high == 15));
        cells[n].low = root_cells[n][0];
        cells[n].high = root_cells[n][1];
    }
    return 0;
}
int func_0027afc0_rule(struct DotrRuleNibbles *cells) { (void)cells; return goal_answer; }
/* A cell whose low nibble is 7 is occupied. */
struct DotrStateItem *SzDuel_GetMapUnit(int x, int y)
{
    unit_args[unit_calls][0] = x, unit_args[unit_calls][1] = y;
    unit_calls++;
    return x == 7 ? (struct DotrStateItem *)&order : NULL;
}
int TaLogic_GetUnitWarning(int a, int b, int c, int d, int e, unsigned char f)
{
    int *w = warn_args[warn_calls];
    w[0] = a, w[1] = b, w[2] = c, w[3] = d, w[4] = e, w[5] = f;
    return warn_by[warn_calls++];
}

/* 0x1459e0's unit: the hide level (low byte) is GetUnitWarning's last argument. */
int TaLogic_GetWarnHideLvl__001461c0(int side) { lvl_calls++; lvl_side = side; return 0x135; }
int TaLogic_IsOpenUnit__00146220(int a, int b, struct DotrRuleOrder *o) { return TaLogic_IsOpenUnit__00139180(a, b, o); }

int TaLogic_GetWarnHideLvl__00141b30(int side) { return TaLogic_GetWarnHideLvl__001461c0(side); }
int TaLogic_GetWarnHideLvl__00134ae0(int side) { return TaLogic_GetWarnHideLvl__001461c0(side); }
int TaLogic_GetWarnHideLvl__0012b1b0(int side) { return TaLogic_GetWarnHideLvl__001461c0(side); }
int TaLogic_GetWarnHideLvl__00130780(int side) { return TaLogic_GetWarnHideLvl__001461c0(side); }
int TaLogic_IsOpenUnit__00141b90(int a, int b, struct DotrRuleOrder *o) { return open_stub(a, b, o); }
int TaLogic_IsOpenUnit__00134b40(int a, int b, struct DotrRuleOrder *o) { return open_stub(a, b, o); }
int TaLogic_IsOpenUnit__0012b210(int a, int b, struct DotrRuleOrder *o) { return open_stub(a, b, o); }
/* 0x12a2e0's unit resets the order through its own InitRogicControl. */
static int unit_done_calls;
void func_0012b830_mode(struct DotrRuleOrder *o, unsigned char mode) { unit_done_calls++; func_0027b1b0_mode(o, mode); }
/* 0x12f930's unit: IsOpenUnit takes a fourth byte, the copy's sixth parameter. */
static int open_flag_calls, open_flag_bad;
int func_00130940_order(int a, int b, struct DotrRuleOrder *o, unsigned char flag)
{
    open_flag_calls++;
    open_flag_bad |= flag != 0x77;
    return open_stub(a, b, o);
}
static int move_12f930(struct DotrRuleOrder *o, int side, struct DotrRuleResult *r, unsigned char mode, unsigned char flag)
{
    return TaLogic_GetMinimumTask_MoveMission__0012f930(o, side, r, mode, flag, 0x77);
}

/* 0x1106f0's unit: IsOpenUnit at 0x10ee00, and the hide level expanded in place from these two. */
static int lp_calls, lp_side, lp_answer, mon_calls, mon_side, mon_answer;
int func_0010ee00_order(int a, int b, struct DotrRuleOrder *o) { return open_stub(a, b, o); }
int func_001df470_side(int side) { lp_calls++; lp_side = side; return lp_answer; }
int SzDuel_GetUnitMonsterCount(int side) { mon_calls++; mon_side = side; return mon_answer; }

/* 0x125b70's unit: IsOpenUnit at 0x126380, InitRogicControl at 0x126940, hide level expanded in place. */
static int inline_lvl, unit126_done_calls;
int TaLogic_IsOpenUnit__00126380(int a, int b, struct DotrRuleOrder *o) { return open_stub(a, b, o); }
void func_00126940_mode(struct DotrRuleOrder *o, unsigned char mode) { unit126_done_calls++; func_0027b1b0_mode(o, mode); }

/* The 0x868-byte copies: each unit's IsOpenUnit and GetWarnHideLvl, and the status exit ahead of
 * the search. */
int TaLogic_IsOpenUnit__0014f490(int a, int b, struct DotrRuleOrder *o) { return open_stub(a, b, o); }
int TaLogic_IsOpenUnit__001670a0(int a, int b, struct DotrRuleOrder *o) { return open_stub(a, b, o); }
int TaLogic_IsOpenUnit__0015cbb0(int a, int b, struct DotrRuleOrder *o) { return open_stub(a, b, o); }
int TaLogic_IsOpenUnit__001620b0(int a, int b, struct DotrRuleOrder *o) { return open_stub(a, b, o); }
int TaLogic_IsOpenUnit__0014ae60(int a, int b, struct DotrRuleOrder *o) { return open_stub(a, b, o); }
int TaLogic_IsOpenUnit__00117870(int a, int b, struct DotrRuleOrder *o) { return open_stub(a, b, o); }
int TaLogicCom_IsOpenUnit__0016b610(int a, int b, struct DotrRuleOrder *o) { return open_stub(a, b, o); }
int func_0016ae30_pick(int a, int b, struct DotrRuleOrder *o) { return open_stub(a, b, o); }
int TaLogic_IsOpenUnit__0013d3d0(int a, int b, struct DotrRuleOrder *o) { return open_stub(a, b, o); }
int TaLogic_GetWarnHideLvl__0014f9b0(int side) { return TaLogic_GetWarnHideLvl__001461c0(side); }
int TaLogic_GetWarnHideLvl__001676d0(int side) { return TaLogic_GetWarnHideLvl__001461c0(side); }
int TaLogic_GetWarnHideLvl__0015d120(int side) { return TaLogic_GetWarnHideLvl__001461c0(side); }
int TaLogic_GetWarnHideLvl__00162580(int side) { return TaLogic_GetWarnHideLvl__001461c0(side); }
int TaLogic_GetWarnHideLvl__0014b330(int side) { return TaLogic_GetWarnHideLvl__001461c0(side); }
int TaLogic_GetWarnHideLvl__00117c30(int side) { return TaLogic_GetWarnHideLvl__001461c0(side); }
int TaLogic_GetWarnHideLvl__00172310(int side) { return TaLogic_GetWarnHideLvl__001461c0(side); }
int func_0016b0e0_free(int side) { return TaLogic_GetWarnHideLvl__001461c0(side); }
int TaLogic_GetWarnHideLvl__0013da70(int side) { return TaLogic_GetWarnHideLvl__001461c0(side); }
static int status_calls, status_args[3], status_answer, kind_calls, kind_bad, kind_by[2], set_calls, set_args[4];
int SzDuel_GetUnitStatus(int a, int b, int bit) { status_calls++; status_args[0] = a, status_args[1] = b, status_args[2] = bit; return status_answer; }
int SzDuel_GetUnitKind(int a, int b) { kind_bad |= a != 3 || b != 5; return kind_by[kind_calls++]; }
int SzDuel_SetUnitStatus(int a, int b, int bit, int value) { set_calls++; set_args[0] = a, set_args[1] = b, set_args[2] = bit, set_args[3] = value; return 0; }

static void reset(void)
{
    memset(&order, 0, sizeof order);
    memset(&result, 0, sizeof result);
    memset(root_cells, 0, sizeof root_cells);
    memset(warn_by, 0, sizeof warn_by);
    order.position = 0x0305;
    order.flags.half = 0x0102;
    loc_calls = def_calls = open_calls = done_calls = id_calls = root_calls = unit_calls = warn_calls = lvl_calls = 0;
    done_order = open_order = NULL;
    loc_answer = 0x0204, def_answer = 0x109, open_answer = 0x1234567, id_answer = 0x10042, goal_answer = 6;
    done_mode = -1;
}

/* The unit stays where it is: result filled from its own cell, order reset with the caller's mode. */
static void stays(int mode, int flag)
{
    assert(result.position == 0x0305 && result.kind == 6 && result.loc == loc_answer);
    assert(def_calls == 1 && def_args[0] == 3 && def_args[1] == 5 && def_args[2] == loc_answer && def_args[3] == flag && def_args[4] == want_last);
    assert(result.atkDefPosition == 9 && result.shouldFlipUp == 0x1234567);
    assert(open_calls == 1 && open_args[0] == 3 && open_args[1] == 5 && open_order == &order);
    assert(done_calls == 1 && done_order == &order && done_mode == mode);
    assert(loc_calls == 1 && loc_args[0] == 3 && loc_args[1] == 5);
}

/* Branch coverage: one run per branch outcome and boundary. */
static void check_move(move_fn fn)
{
    /* no goal (0xffff) and the unit's own cell reads the same: stays, nothing searched; both modes */
    reset();
    order.flags.half = 0xffff, loc_answer = 0xffff;
    assert(fn(&order, 1, &result, 0, 4) == 0);
    stays(0, 4);
    assert(id_calls == 0 && root_calls == 0);
    reset();
    order.flags.word = (int)0xffff0000, loc_answer = 0xffff;
    assert(fn(&order, 1, &result, 1, 0x80) == 0);
    stays(1, 0x80);
    assert(root_calls == 0);
    /* no goal but the unit is elsewhere: searched; no unit at the goal gives 0xffff; no goal cell
     * (0): stays */
    reset();
    order.flags.half = 0xffff, id_answer = -1, goal_answer = 0;
    assert(fn(&order, 1, &result, 0, 4) == 0);
    stays(0, 4);
    assert(id_calls == 1 && id_args[0] == 0xff && id_args[1] == 0xff);
    assert(root_calls == 1 && root_args[0] == 3 && root_args[1] == 5 && root_args[2] == 4 && root_args[3] == 2);
    assert(root_args[4] == 0xff && root_args[5] == 0xff && root_args[6] == 0xffff && root_args[7] == 7 && unit_calls == 0);
    /* mode 1 reads the goal from the upper half; the unit number is cut to 16 bits; one goal cell,
     * occupied: stays */
    reset();
    order.flags.word = 0x06080000, goal_answer = 1;
    root_cells[0][0] = 7, root_cells[0][1] = 3;
    assert(fn(&order, 1, &result, 1, 4) == 0);
    stays(1, 4);
    assert(id_args[0] == 8 && id_args[1] == 6 && root_args[4] == 8 && root_args[5] == 6 && root_args[6] == 0x42);
    assert(unit_calls == 1 && unit_args[0][0] == 7 && unit_args[0][1] == 3 && warn_calls == 0);
    /* one free goal cell is taken */
    reset();
    goal_answer = 1;
    root_cells[0][0] = 9, root_cells[0][1] = 8;
    warn_by[0] = 0xfffe;
    assert(fn(&order, 2, &result, 0, 4) == 0);
    assert(id_args[0] == 2 && id_args[1] == 1 && root_args[4] == 2 && root_args[5] == 1);
    assert(warn_calls == 1 && warn_args[0][0] == 9 && warn_args[0][1] == 8 && warn_args[0][2] == 2);
    assert(warn_args[0][3] == want_warn[0] && warn_args[0][4] == want_warn[1] && warn_args[0][5] == want_warn[2]);
    assert(lvl_calls == (want_warn[2] != 0 && !inline_lvl) && (lvl_calls == 0 || lvl_side == 2));
    assert(result.position == 0x0305 && result.kind == 6 && result.loc == 0x0809);
    assert(def_calls == 1 && def_args[0] == 3 && def_args[1] == 5 && def_args[2] == 0x0809 && def_args[3] == 4 && def_args[4] == want_last);
    assert(result.atkDefPosition == 9 && result.shouldFlipUp == 0x1234567 && open_calls == 1 && open_order == &order);
    assert(done_calls == 1 && done_order == &order && done_mode == 0);
    /* lowest warning wins; on a tie only a cell strictly nearer the goal (2, 1) replaces the choice;
     * occupied cells are skipped; cells past the count are not read */
    reset();
    root_cells[0][0] = 9, root_cells[0][1] = 9; /* 8 */
    root_cells[1][0] = 1, root_cells[1][1] = 1; /* 5: lower */
    root_cells[2][0] = 3, root_cells[2][1] = 1; /* 5: as near, stays */
    root_cells[3][0] = 7, root_cells[3][1] = 1; /* occupied */
    root_cells[4][0] = 2, root_cells[4][1] = 2; /* 9: higher */
    root_cells[5][0] = 2, root_cells[5][1] = 1; /* (not reached with count 5) */
    warn_by[0] = 8, warn_by[1] = 5, warn_by[2] = 5, warn_by[3] = 9, warn_by[4] = 5;
    goal_answer = 5;
    assert(fn(&order, 1, &result, 0, 4) == 0);
    assert(unit_calls == 5 && warn_calls == 4 && result.loc == 0x0101 && done_calls == 1);
    reset();
    root_cells[0][0] = 1, root_cells[0][1] = 1;
    root_cells[1][0] = 2, root_cells[1][1] = 3; /* farther: stays */
    root_cells[2][0] = 2, root_cells[2][1] = 1; /* nearer: taken */
    root_cells[3][0] = 4, root_cells[3][1] = 1; /* farther in x: stays */
    root_cells[4][0] = 2, root_cells[4][1] = 0; /* farther in y from below: stays */
    root_cells[5][0] = 0, root_cells[5][1] = 0; /* higher warning */
    warn_by[0] = 5, warn_by[1] = 5, warn_by[2] = 5, warn_by[3] = 5, warn_by[4] = 5, warn_by[5] = 6;
    assert(fn(&order, 1, &result, 0, 4) == 0);
    assert(unit_calls == 6 && warn_calls == 6 && result.loc == 0x0102 && def_args[2] == 0x0102);
    /* the same tie under mode 1 */
    reset();
    order.flags.word = 0x01020000;
    root_cells[0][0] = 1, root_cells[0][1] = 1;
    root_cells[1][0] = 2, root_cells[1][1] = 1;
    goal_answer = 2;
    assert(fn(&order, 1, &result, 1, 4) == 0);
    assert(result.loc == 0x0102 && done_mode == 1);
}

/* The status exit of the 0x868-byte copies, one run per outcome and boundary of the kind test (kind
 * >> 5 is 1 or 5). `leaves` says whether the unit is released (status cleared, result kind 14,
 * nothing searched). */
static void status_run(move_fn fn, int kind0, int kind1, int kinds, int leaves)
{
    reset();
    status_calls = kind_calls = kind_bad = set_calls = 0;
    status_answer = 1, kind_by[0] = kind0, kind_by[1] = kind1;
    goal_answer = 0;
    assert(fn(&order, 1, &result, 0, 4) == 0);
    assert(status_calls == 1 && status_args[0] == 3 && status_args[1] == 5 && status_args[2] == 4);
    assert(kind_calls == kinds && !kind_bad && set_calls == leaves);
    if (leaves) {
        assert(set_args[0] == 3 && set_args[1] == 5 && set_args[2] == 0 && set_args[3] == 1);
        assert(result.position == 0x0305 && result.kind == 14 && root_calls == 0 && done_calls == 0 && def_calls == 0);
    } else {
        stays(0, 4);
        assert(root_calls == 1);
    }
    status_answer = 0;
}
static void check_status(move_fn fn)
{
    /* status clear: the kind is not read (check_move runs with it clear throughout) */
    reset();
    status_calls = kind_calls = set_calls = 0;
    goal_answer = 0;
    assert(fn(&order, 1, &result, 0, 4) == 0);
    assert(status_calls == 1 && kind_calls == 0 && set_calls == 0 && root_calls == 1);
    status_run(fn, 0x20, 0, 1, 1);    /* kind 1, lowest */
    status_run(fn, 0x3f, 0, 1, 1);    /* kind 1, highest */
    status_run(fn, 0x1f, 0xa0, 2, 1); /* below 1 on the first read, 5 on the second */
    status_run(fn, 0x40, 0xbf, 2, 1); /* 2, then 5 highest */
    status_run(fn, 0x40, 0xc0, 2, 0); /* 2, then 6: searched */
    status_run(fn, 0x1f, 0x9f, 2, 0); /* 0, then 4: searched */
}

/* 0x152cd0's unit: the active-mode answer picks GetUnitWarning's fourth argument. */
static int active_calls, active_answer;
int TaLogic015_IsActiveMode(void) { active_calls++; return active_answer; }
int TaLogic_IsOpenUnit__00153ae0(int a, int b, struct DotrRuleOrder *o) { return open_stub(a, b, o); }
int TaLogic_GetWarnHideLvl__00153ed0(int side) { return TaLogic_GetWarnHideLvl__001461c0(side); }
/* 0x157a50's unit: the sixth argument picks the route finder. */
static int route_calls, route_args[10];
int TaLogic_IsOpenUnit__00158360(int a, int b, struct DotrRuleOrder *o) { return open_stub(a, b, o); }
int TaLogic_GetWarnHideLvl__00158780(int side) { return TaLogic_GetWarnHideLvl__001461c0(side); }
int func_0027a090_rule(int a, int b, struct DotrRuleNibbles *cells, int d, int e, int f, int g, int h, int i, unsigned char j, unsigned short k)
{
    int n;
    route_calls++;
    route_args[0] = a, route_args[1] = b, route_args[2] = d, route_args[3] = e, route_args[4] = f, route_args[5] = g;
    route_args[6] = h, route_args[7] = i, route_args[8] = j, route_args[9] = k;
    for (n = 0; n < 12; n++) {
        assert(root_raw || (cells[n].low == 15 && cells[n].high == 15));
        cells[n].low = root_cells[n][0];
        cells[n].high = root_cells[n][1];
    }
    return 0;
}
static int move_157a50(struct DotrRuleOrder *o, int side, struct DotrRuleResult *r, unsigned char mode, unsigned char flag)
{
    return TaLogic_GetMinimumTask_MoveMission__00157a50(o, side, r, mode, flag, 0);
}
/* Branch coverage for the sixth argument: 1 takes the other route finder, 2 does not. */
static void check_route(void)
{
    reset();
    route_calls = 0;
    goal_answer = 1;
    root_cells[0][0] = 9, root_cells[0][1] = 8;
    assert(TaLogic_GetMinimumTask_MoveMission__00157a50(&order, 2, &result, 0, 4, 1) == 0);
    assert(root_calls == 0 && route_calls == 1 && id_calls == 1 && lvl_calls == 2 && lvl_side == 2);
    assert(route_args[0] == 3 && route_args[1] == 5 && route_args[2] == 4 && route_args[3] == 2 && route_args[4] == 2);
    assert(route_args[5] == 1 && route_args[6] == 0xffff && route_args[7] == 0 && route_args[8] == 30 && route_args[9] == 0x135);
    assert(warn_calls == 1 && warn_args[0][3] == 3 && warn_args[0][4] == 1 && warn_args[0][5] == 0x35);
    assert(result.kind == 6 && result.loc == 0x0809 && done_calls == 1);
    reset();
    route_calls = 0;
    goal_answer = 1;
    root_cells[0][0] = 9, root_cells[0][1] = 8;
    assert(TaLogic_GetMinimumTask_MoveMission__00157a50(&order, 2, &result, 0, 4, 2) == 0);
    assert(root_calls == 1 && route_calls == 0 && root_args[6] == 0x42 && result.loc == 0x0809);
}

/* 0x27d020: GetUnitWarning through its byte/halfword view. */
int func_00276cc0_enc(int x, int z, int side, unsigned char d, unsigned short e, unsigned char f)
{
    return TaLogic_GetUnitWarning(x, z, side, d, e, f);
}
/* Branch coverage: goal 0xffff with the unit there or not (both modes), no goal cell, every cell occupied,
 * and per cell: occupied, lower warning, equal warning nearer / farther, higher warning. */
static void check_enc(void)
{
    static const int cells[6][2] = { { 7, 0 }, { 5, 5 }, { 3, 1 }, { 6, 6 }, { 2, 2 }, { 9, 8 } };
    static const int warns[5] = { 10, 10, 10, 11, 4 };
    int n, mode;
    for (mode = 0; mode < 2; mode++) {
        reset();
        route_calls = 0;
        if (mode) {
            order.flags.word = 0xffff0000u;
        } else {
            order.flags.half = 0xffff;
        }
        loc_answer = 0xffff;
        assert(TaLogic_GetMinimumTask_EncMission(&order, 2, &result, mode, 0x77, 0x12345, 0, 0x66) == 0);
        assert(loc_calls == 2 && route_calls == 0 && done_calls == 1 && done_order == &order && done_mode == mode);
        assert(result.position == 0x0305 && result.kind == 6 && result.loc == 0xffff);
        assert(result.atkDefPosition == 2 && result.shouldFlipUp == 2);
        /* goal 0xffff, unit elsewhere: the search runs and finds no goal cell */
        reset();
        route_calls = 0;
        if (mode) {
            order.flags.word = 0xffff0000u;
        } else {
            order.flags.half = 0xffff;
        }
        goal_answer = 0;
        assert(TaLogic_GetMinimumTask_EncMission(&order, 2, &result, mode, 0x77, 0x12345, 0, 0x66) == 0);
        assert(route_calls == 1 && unit_calls == 0 && done_calls == 1 && result.loc == 0x0204);
        assert(route_args[0] == 3 && route_args[1] == 5 && route_args[2] == 4 && route_args[3] == 2 && route_args[4] == 4);
        assert(route_args[5] == 2 && route_args[6] == (int)order.flags.word && route_args[7] == 0x77);
        assert(route_args[8] == 3 && route_args[9] == 0x2345);
        assert(result.position == 0x0305 && result.kind == 6 && result.atkDefPosition == 2 && result.shouldFlipUp == 2);
    }
    for (n = 1; n <= 6; n++) {
        int k;
        reset();
        route_calls = 0;
        goal_answer = n;
        for (k = 0; k < 6; k++) {
            root_cells[k][0] = cells[k][0], root_cells[k][1] = cells[k][1];
        }
        memcpy(warn_by, warns, sizeof warns);
        assert(TaLogic_GetMinimumTask_EncMission(&order, 2, &result, 0, 0x77, 0x12345, 0, 0x66) == 0);
        assert(unit_calls == n && warn_calls == n - 1 && done_calls == 1 && done_mode == 0);
        assert(result.position == 0x0305 && result.kind == 6 && result.atkDefPosition == 2 && result.shouldFlipUp == 2);
        /* all occupied; first free; equal and nearer; equal and farther; higher; lower */
        assert(result.loc == (n == 1 ? 0x0204 : n == 2 ? 0x0505 : n < 6 ? 0x0103 : 0x0809));
        if (n > 2) {
            assert(warn_args[0][0] == 5 && warn_args[0][1] == 5 && warn_args[0][2] == 2 && warn_args[0][3] == 0xff);
            assert(warn_args[0][4] == 0x66 && warn_args[0][5] == 0xff && warn_args[1][3] == 10 && warn_args[1][5] == 10);
        }
    }
}

/* The 0xabc-byte copies: the units' IsOpenUnit / InitRogicControl and the callees of the two edge exits. */
static int unit11c_done_calls, unit121_done_calls, edge_unit_calls, rev_calls, rev_args[3], cap_calls, cap_args[2];
int func_0011c140_order(int a, int b, struct DotrRuleOrder *o) { return open_stub(a, b, o); }
void func_0011c3e0_mode(struct DotrRuleOrder *o, unsigned char mode) { unit11c_done_calls++; func_0027b1b0_mode(o, mode); }
int TaLogic_IsOpenUnit__001211b0(int a, int b, struct DotrRuleOrder *o) { return open_stub(a, b, o); }
void func_001215e0_mode(struct DotrRuleOrder *o, unsigned char mode) { unit121_done_calls++; func_0027b1b0_mode(o, mode); }
int func_001df540_word(int a, int b) { edge_unit_calls += a == 3 && b == 5; return 0x777; }
int func_002464f0_rule(int item, int x, int y) { rev_calls++; rev_args[0] = item, rev_args[1] = x, rev_args[2] = y; return 0x13; }
int GetCap(int limit, int value) { cap_calls++; cap_args[0] = limit, cap_args[1] = value; return 6; }

/* Branch coverage: the order's kind byte 14/15/16/17 (both sides of each exit), goal cell found or
 * not, both modes. */
static void check_edge(move_fn fn)
{
    int kind;
    for (kind = 15; kind <= 16; kind++) {
        reset();
        root_raw = 1, edge_unit_calls = rev_calls = cap_calls = 0;
        order.missions = kind << 8 | 0x55;
        assert(fn(&order, 1, &result, 0, 4) == 0);
        assert(edge_unit_calls == 1 && rev_calls == 1 && rev_args[0] == 0x777 && rev_args[1] == 4 && rev_args[2] == 2);
        assert(cap_calls == 1 && cap_args[0] == (kind == 15 ? 7 : 0) && cap_args[1] == (kind == 15 ? 5 : -1));
        assert(id_calls == 1 && id_args[0] == 2 && id_args[1] == 1);
        assert(root_calls == 1 && root_args[0] == 3 && root_args[1] == 5 && root_args[2] == 4 && root_args[3] == 2);
        assert(root_args[4] == 4 && root_args[5] == 6 && root_args[6] == 0x42 && root_args[7] == 2);
        assert(result.position == 0x0305 && result.kind == 6 && result.loc == 0x0604);
        assert(def_calls == 1 && def_args[0] == 3 && def_args[1] == 5 && def_args[2] == 0x0604 && def_args[3] == 4 && def_args[4] == want_last);
        assert(result.atkDefPosition == 9 && result.shouldFlipUp == 0x1234567 && open_calls == 1 && open_order == &order);
        assert(done_calls == 1 && done_order == &order && done_mode == 0 && unit_calls == 0 && warn_calls == 0);
        /* no goal cell: the unit's own cell; mode 1 reads the goal from the upper half; no unit
         * there gives 0xffff */
        reset();
        order.missions = kind << 8;
        order.flags.word = 0x06080000, goal_answer = 0, id_answer = -1;
        assert(fn(&order, 1, &result, 1, 0x80) == 0);
        stays(1, 0x80);
        assert(id_args[0] == 8 && id_args[1] == 6 && root_calls == 1 && root_args[6] == 0xffff && root_args[7] == 2);
        root_raw = 0;
    }
    /* 14 and 17 take the ordinary search (cells preset, TaLogic_GetRoot asked with 7) */
    for (kind = 14; kind <= 17; kind += 3) {
        reset();
        cap_calls = 0;
        order.missions = kind << 8, goal_answer = 0;
        assert(fn(&order, 1, &result, 0, 4) == 0);
        stays(0, 4);
        assert(cap_calls == 0 && root_calls == 1 && root_args[7] == 7);
    }
}

int main(void)
{
    want_last = 1, want_warn[0] = 2, want_warn[1] = 0, want_warn[2] = 0;
    check_move(TaLogic_GetMinimumTask_MoveMission__00137fb0);
    want_last = 0, want_warn[0] = 3, want_warn[1] = 1, want_warn[2] = 0x35;
    check_move(TaLogic_GetMinimumTask_MoveMission__001459e0);
    want_last = 0, want_warn[0] = 3, want_warn[1] = 1, want_warn[2] = 0x35;
    check_move(TaLogic_GetMinimumTask_MoveMission__00140e10);
    want_warn[0] = 4;
    check_move(TaLogic_GetMinimumTask_MoveMission__001341c0);
    want_warn[0] = 5;
    assert(unit_done_calls == 0);
    check_move(TaLogic_GetMinimumTask_MoveMission__0012a2e0);
    assert(unit_done_calls > 0);
    want_warn[0] = 3;
    assert(open_flag_calls == 0);
    check_move(move_12f930);
    assert(open_flag_calls > 0 && !open_flag_bad);
    /* 0x1106f0: the level is 0 on each path of the expanded test (1500/1501 life points, 3/4 monsters). */
    want_warn[0] = 2, want_warn[2] = 0;
    lp_answer = 1500, mon_answer = 4;
    check_move(TaLogic_GetMinimumTask_MoveMission__001106f0);
    assert(lp_calls > 0 && lp_side == 1 && mon_calls == 0);
    lp_answer = 1501, mon_answer = 3;
    check_move(TaLogic_GetMinimumTask_MoveMission__001106f0);
    assert(mon_calls > 0 && mon_side == 1);
    mon_answer = 4;
    check_move(TaLogic_GetMinimumTask_MoveMission__001106f0);
    /* 0x125b70: 2000 up to 1000 life points; above, 500 with three monsters and 1000 with two (low bytes). */
    inline_lvl = 1;
    lp_calls = mon_calls = 0;
    lp_answer = 1000, mon_answer = 3, want_warn[2] = 2000 & 0xff;
    check_move(TaLogic_GetMinimumTask_MoveMission__00125b70);
    assert(lp_calls > 0 && mon_calls == 0 && unit126_done_calls > 0);
    lp_answer = 1001, want_warn[2] = 500 & 0xff;
    check_move(TaLogic_GetMinimumTask_MoveMission__00125b70);
    assert(mon_calls > 0 && mon_side == 1);
    mon_answer = 2, want_warn[2] = 1000 & 0xff;
    check_move(TaLogic_GetMinimumTask_MoveMission__00125b70);
    inline_lvl = 0;
    /* the 0x868-byte copies: per copy IsDefStyleUnit's last argument and GetUnitWarning's fourth and fifth */
    want_warn[2] = 0x35;
    want_last = 0, want_warn[0] = 3, want_warn[1] = 1;
    check_move(TaLogic_GetMinimumTask_MoveMission__0014eb30);
    check_status(TaLogic_GetMinimumTask_MoveMission__0014eb30);
    want_last = 0, want_warn[0] = 3, want_warn[1] = 1;
    check_move(TaLogic_GetMinimumTask_MoveMission__0015be00);
    check_status(TaLogic_GetMinimumTask_MoveMission__0015be00);
    want_last = 1, want_warn[0] = 5, want_warn[1] = 0;
    check_move(TaLogic_GetMinimumTask_MoveMission__001664a0);
    check_status(TaLogic_GetMinimumTask_MoveMission__001664a0);
    want_last = 0, want_warn[0] = 2, want_warn[1] = 1;
    check_move(TaLogic_GetMinimumTask_MoveMission__00161380);
    check_status(TaLogic_GetMinimumTask_MoveMission__00161380);
    want_last = 0, want_warn[0] = 2, want_warn[1] = 1;
    check_move(TaLogic_GetMinimumTask_MoveMission__0014a240);
    check_status(TaLogic_GetMinimumTask_MoveMission__0014a240);
    want_last = 0, want_warn[0] = 2, want_warn[1] = 1;
    check_move(TaLogic_GetMinimumTask_MoveMission__001162f0);
    check_status(TaLogic_GetMinimumTask_MoveMission__001162f0);
    want_last = 0, want_warn[0] = 2, want_warn[1] = 1;
    check_move(TaLogic_GetMinimumTask_MoveMission__001714f0);
    check_status(TaLogic_GetMinimumTask_MoveMission__001714f0);
    want_last = 1, want_warn[0] = 4, want_warn[1] = 0;
    check_move(TaLogic_GetMinimumTask_MoveMission__0016a5c0);
    check_status(TaLogic_GetMinimumTask_MoveMission__0016a5c0);
    want_last = 0, want_warn[0] = 2, want_warn[1] = 0;
    check_move(TaLogic_GetMinimumTask_MoveMission__0013ca10);
    check_status(TaLogic_GetMinimumTask_MoveMission__0013ca10);
    /* 0x152cd0: the active-mode byte gives 5 or 2; only its low byte counts (0x100 reads as clear) */
    want_last = 0, want_warn[0] = 2, want_warn[1] = 1;
    active_answer = 0;
    check_move(TaLogic_GetMinimumTask_MoveMission__00152cd0);
    check_status(TaLogic_GetMinimumTask_MoveMission__00152cd0);
    assert(active_calls > 0);
    active_answer = 0x100;
    check_move(TaLogic_GetMinimumTask_MoveMission__00152cd0);
    active_answer = 1, want_warn[0] = 5;
    check_move(TaLogic_GetMinimumTask_MoveMission__00152cd0);
    /* 0x157a50: sixth argument 0 behaves as the other copies; 1 and 2 in check_route */
    want_last = 0, want_warn[0] = 3, want_warn[1] = 1;
    check_move(move_157a50);
    check_status(move_157a50);
    assert(route_calls == 0);
    check_route();
    /* 0x11b680: level 0 on each path (1500/1501 life points, 2/3 monsters); 0x1206f0: as 0x125b70 */
    want_last = 0, want_warn[0] = 3, want_warn[1] = 1, want_warn[2] = 0;
    lp_calls = mon_calls = 0;
    lp_answer = 1500, mon_answer = 3;
    check_move(TaLogic_GetMinimumTask_MoveMission__0011b680);
    assert(lp_calls > 0 && lp_side == 1 && mon_calls == 0 && unit11c_done_calls > 0);
    lp_answer = 1501, mon_answer = 2;
    check_move(TaLogic_GetMinimumTask_MoveMission__0011b680);
    assert(mon_calls > 0 && mon_side == 1);
    mon_answer = 3;
    check_move(TaLogic_GetMinimumTask_MoveMission__0011b680);
    check_edge(TaLogic_GetMinimumTask_MoveMission__0011b680);
    inline_lvl = 1;
    lp_calls = mon_calls = 0;
    want_warn[0] = 4;
    lp_answer = 1000, mon_answer = 3, want_warn[2] = 2000 & 0xff;
    check_move(TaLogic_GetMinimumTask_MoveMission__001206f0);
    assert(lp_calls > 0 && mon_calls == 0 && unit121_done_calls > 0);
    lp_answer = 1001, want_warn[2] = 500 & 0xff;
    check_move(TaLogic_GetMinimumTask_MoveMission__001206f0);
    assert(mon_calls > 0 && mon_side == 1);
    mon_answer = 2, want_warn[2] = 1000 & 0xff;
    check_move(TaLogic_GetMinimumTask_MoveMission__001206f0);
    check_edge(TaLogic_GetMinimumTask_MoveMission__001206f0);
    inline_lvl = 0;
    check_enc();
    return 0;
}
