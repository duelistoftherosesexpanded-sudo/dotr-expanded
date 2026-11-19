/* Synthetic host tests for the logic012_line group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/logic012_line.h"

#define POSITION 0x0105
#define WARN 0x12345

static struct DotrLineOrder order, at_btl;
static int unit_answer, unit_calls, side_answer, here_answer, there_answer;
static int root_args[9], root_calls, root_answer;
static int land_answer, land_calls, agree_answer, btl_calls;
static unsigned short line_ids[7];
static int line_loc[7], win_by[7], invade_by[7];
static int lst_calls, lst_count[3], lst_y[3], lst_mask[3];
static int win_calls, invade_calls, cap_answer, cap_calls, empty_answer, empty_calls;
static int random_answer, random_calls;

static int line_index(int id)
{
    int i;

    for (i = 0; i < 7; i++) {
        if (line_ids[i] == id) {
            return i;
        }
    }
    assert(0);
    return -1;
}

struct jp_9a533 *SzDuel_GetUnit(int a, int b)
{
    assert(a == (POSITION >> 8) && b == (POSITION & 0xff));
    unit_calls++;
    return (struct jp_9a533 *)(size_t)unit_answer;
}

int Ta_GetCurrentSide(int a, int b)
{
    assert(a == (POSITION >> 8) && b == (POSITION & 0xff));
    return side_answer;
}

int SzDuel_GetUnitLoc(int a, int b)
{
    if (a == (POSITION >> 8) && b == (POSITION & 0xff)) {
        return here_answer;
    }
    if (a == (side_answer == 0 ? 1 : 0) && b == 0) {
        return there_answer;
    }
    return line_loc[line_index((a << 8) | b)];
}

int TaLogic_IsRootOpen(int a, int b, int c, int d, int e, int f, int g, int h, unsigned char i)
{
    root_args[0] = a, root_args[1] = b, root_args[2] = c, root_args[3] = d, root_args[4] = e;
    root_args[5] = f, root_args[6] = g, root_args[7] = h, root_args[8] = i;
    root_calls++;
    return root_answer;
}

/* TaLogic012_GetEmptyLine runs for real. For the caller's tests (grid_mode 0) the grid is laid
 * out so that it answers empty_answer; check_empty_line (grid_mode 1) sets every cell. */
static int grid_mode, in_empty, map_id[7][7], ctl_by[4], agree_by[7], ctl_calls;

int Ta_IsUnitControl2(struct jp_9a533 *vInfo)
{
    assert((int)(size_t)vInfo == unit_answer);
    empty_calls++;
    in_empty = 1;
    return 5;
}

int SzDuel_GetMapUnitID(int x, int y)
{
    assert(in_empty && x >= 0 && x < 7 && y >= 0 && y < 7);
    if (!grid_mode) {
        return y == empty_answer ? -1 : 0x0190 + x;
    }
    return map_id[y][x];
}

int Ta_IsUnitControl(int a, int b)
{
    assert(in_empty && a >= 0 && a < 4 && b >= 0x90 && b < 0x97);
    ctl_calls++;
    return grid_mode ? ctl_by[a] : 5;
}

int SzDuel_GetField(int x, int y)
{
    if (in_empty) {
        assert(x == 0 && y >= 0 && y < 7);
        return 100 + y;
    }
    assert(x == (here_answer & 0xff) && y == here_answer >> 8);
    return 77;
}

int CheckLandAgreement(int a, struct jp_9a533 *item)
{
    if (in_empty) {
        assert(a >= 100 && a < 107 && (int)(size_t)item == unit_answer);
        return grid_mode ? agree_by[a - 100] : 1;
    }
    assert(a == 77 && (int)(size_t)item == unit_answer);
    land_calls++;
    return land_answer;
}

int TaLogic012_GetAgreeLineZ(struct jp_9a533 *item)
{
    assert((int)(size_t)item == unit_answer);
    return agree_answer;
}

void TaLogic012_CommandBtlUnit(struct DotrLineOrder *o)
{
    assert(o == &order);
    at_btl = *o;
    btl_calls++;
}

int TaLogic012_GetLineUnitLst(unsigned short *out, int y, int side, int mask)
{
    int i;

    assert(lst_calls < 3 && side == side_answer);
    for (i = 0; i < 7; i++) {
        assert(out[i] == 0xffff || lst_calls > 0);
        out[i] = line_ids[i];
    }
    lst_y[lst_calls] = y;
    lst_mask[lst_calls] = mask;
    return lst_count[lst_calls++] | 0x100; /* only the low byte is the count */
}

int TaLogic_GetWarnHideLvl__001461c0(int side)
{
    assert(side == side_answer);
    return WARN;
}

int TaLogic_IsUnitWin4(struct jp_9a533 *item, int packed, int c, int d, int e, unsigned char skip)
{
    int i = line_index(packed);

    assert((int)(size_t)item == unit_answer && c == WARN && d == (line_loc[i] & 0xff) && e == d && skip == 0);
    win_calls++;
    return win_by[i];
}

int TaLogic_GetInvadeRoot2(struct jp_9a533 *pVarInfo, struct jp_9b2bd *pMovePiece, int nX, int nZ, int gX, int gZ,
                           int gUnitID, int fView, unsigned char bAbort, unsigned short bWarnLvl)
{
    int i = line_index(gUnitID);
    int k;

    assert((int)(size_t)pVarInfo == unit_answer);
    for (k = 0; k < 12; k++) {
        assert(((unsigned char *)pMovePiece)[k] == 0xff);
    }
    assert(nX == (here_answer & 0xff) && nZ == here_answer >> 8);
    assert(gX == (line_loc[i] & 0xff) && gZ == line_loc[i] >> 8);
    assert(fView == 0 && bAbort == 5 && bWarnLvl == (WARN & 0xffff));
    invade_calls++;
    return invade_by[i];
}

int GetCap(int limit, int value)
{
    assert(limit == 6 && value == (here_answer >> 8) - 1);
    cap_calls++;
    return cap_answer;
}


int func_0019a5c8(void)
{
    random_calls++;
    return random_answer;
}

/* Defaults: unit present, side 0, cell x 3 / y 4, root closed, land agrees, empty lists, one
 * unit on the line for the last list (so the command switch is reached). */
static void reset(void)
{
    int i;

    memset(&order, 0, sizeof(order));
    order.position = POSITION;
    order.unknown_04 = 0x0101;
    order.flags.word = 0x7eee4321;
    unit_answer = 0x1234; side_answer = 0; here_answer = 0x0403; there_answer = 0x0806;
    root_answer = 0; land_answer = 1; agree_answer = -1; cap_answer = 9; empty_answer = -1;
    random_answer = 0;
    for (i = 0; i < 7; i++) {
        line_ids[i] = 0x0611 + i;
        line_loc[i] = 0x0909;
        win_by[i] = -1;
        invade_by[i] = 0;
    }
    lst_count[0] = 0; lst_count[1] = 0; lst_count[2] = 1;
}

static void run(void)
{
    unit_calls = root_calls = land_calls = btl_calls = lst_calls = win_calls = 0;
    invade_calls = cap_calls = empty_calls = random_calls = 0;
    grid_mode = in_empty = 0;
    TaLogic012_CommandLineUnit(&order);
}

static int result(int command, int high)
{
    return order.unknown_04 == command && order.flags.word == (unsigned int)((high << 16) | 0x4321) && order.position == POSITION;
}

static void check_early(void)
{
    /* no unit: nothing is asked and nothing stored */
    reset(); unit_answer = 0; run();
    assert(unit_calls == 1 && root_calls == 0 && result(0x0101, 0x7eee));
    /* root open: command 0x2ff toward the other leader's unit (1, 0); arguments as packed */
    reset(); root_answer = 1; run();
    assert(result(0x2ff, 0x0100) && land_calls == 0 && unit_calls == 2);
    assert(root_args[0] == 1 && root_args[1] == 5 && root_args[2] == 3 && root_args[3] == 4);
    assert(root_args[4] == 1 && root_args[5] == 0 && root_args[6] == 6 && root_args[7] == 8 && root_args[8] == 2);
    reset(); side_answer = 1; root_answer = 5; run();
    assert(result(0x2ff, 0) && root_args[4] == 0);
    /* 0 is not open (boundary) */
    reset(); run();
    assert(root_calls == 1 && land_calls == 1);
}

static void check_land(void)
{
    /* land does not agree: 0x6ff with the agreeing line (0 is taken), else the battle command */
    reset(); land_answer = 0; agree_answer = 0; run();
    assert(result(0x6ff, 0x0003) && btl_calls == 0 && lst_calls == 0);
    reset(); land_answer = 2; agree_answer = 5; run();
    assert(result(0x6ff, 0x0503) && btl_calls == 0);
    reset(); land_answer = 0; run();
    assert(btl_calls == 1 && result(0x6ff, 0x7eee) && lst_calls == 0);
}

static void check_first_walk(void)
{
    /* a unit that is not beaten within distance 2: 0x3ff at the own cell */
    reset(); lst_count[0] = 1; line_loc[0] = 0x0504; run();      /* x 4, y 5: 1 + 1 */
    assert(result(0x3ff, 0x0403) && lst_calls == 1 && lst_y[0] == 4 && lst_mask[0] == 2 && invade_calls == 0);
    reset(); lst_count[0] = 1; line_loc[0] = 0x0401; run();      /* x 1: 2 + 0 */
    assert(result(0x3ff, 0x0403));
    reset(); lst_count[0] = 1; line_loc[0] = 0x0203; run();      /* y 2: 0 + 2 */
    assert(result(0x3ff, 0x0403));
    reset(); lst_count[0] = 1; line_loc[0] = 0x0703; run();      /* y 7: distance 3 is too far */
    assert(lst_calls == 3 && !result(0x3ff, 0x0403));
    reset(); lst_count[0] = 1; line_loc[0] = 0x0305; run();      /* 2 + 1 */
    assert(lst_calls == 3);
    /* count bound (low byte only): the second entry is not read */
    reset(); lst_count[0] = 1; line_loc[1] = 0x0403; run();
    assert(lst_calls == 3 && win_calls == 1);
    /* later entries are read up to the count */
    reset(); lst_count[0] = 3; line_loc[2] = 0x0403; run();
    assert(result(0x3ff, 0x0403) && win_calls == 3);
    /* beaten units: the strictly longest invade root above 7 is kept (first on ties) and
     * stored as 0x2ff before the later stages, seen here by the battle command */
    reset(); lst_count[0] = 4; lst_count[2] = 2;
    win_by[0] = 0; invade_by[0] = 7;                             /* not above 7 */
    win_by[1] = 3; invade_by[1] = 9;
    win_by[2] = 3; invade_by[2] = 9;                             /* tie: first stays */
    win_by[3] = 3; invade_by[3] = -4;                            /* not positive */
    run();
    assert(invade_calls == 4 && btl_calls == 1 && at_btl.unknown_04 == 0x2ff && at_btl.flags.word == 0x06124321);
    reset(); lst_count[0] = 3; lst_count[2] = 2;
    win_by[0] = 0; invade_by[0] = 8; win_by[1] = 0; invade_by[1] = 10; win_by[2] = 0; invade_by[2] = 0;
    run();
    assert(btl_calls == 1 && at_btl.unknown_04 == 0x2ff && at_btl.flags.word == 0x06124321);
    /* nothing kept: the command is untouched at that point */
    reset(); lst_count[0] = 1; lst_count[2] = 2; win_by[0] = 0; invade_by[0] = 7; run();
    assert(btl_calls == 1 && at_btl.unknown_04 == 0x0101 && at_btl.flags.word == 0x7eee4321);
}

static void check_later_stages(void)
{
    /* second list: capped previous line, mask 2; each entry is asked once, result unused */
    reset(); lst_count[1] = 3; win_by[0] = 4; win_by[1] = 9; run();
    assert(cap_calls == 1 && lst_y[1] == 9 && lst_mask[1] == 2 && win_calls == 3);
    assert(lst_y[2] == 4 && lst_mask[2] == 1);
    /* two or more own units on the line: 0x6ff with the empty line, or the battle command */
    reset(); lst_count[2] = 2; empty_answer = 0; run();
    assert(result(0x6ff, 0x0003) && empty_calls == 1 && btl_calls == 0 && random_calls == 0);
    reset(); lst_count[2] = 2; run();
    assert(btl_calls == 1 && result(0x0101, 0x7eee));
    /* one unit: the switch on the command's high byte */
    reset(); order.unknown_04 = 0x10aa; run();                   /* 16, x > 0 */
    assert(result(0x10aa, 0x0400) && empty_calls == 0 && random_calls == 0);
    reset(); order.unknown_04 = 0x10aa; here_answer = 0x0400; run();   /* 16, x 0 */
    assert(result(0xfff, 0x0406));
    reset(); order.unknown_04 = 0x0faa; here_answer = 0x0405; run();   /* 15, x < 6 */
    assert(result(0x0faa, 0x0406));
    reset(); order.unknown_04 = 0x0faa; here_answer = 0x0406; run();   /* 15, x 6 */
    assert(result(0x10ff, 0x0400));
    reset(); random_answer = 6; run();                           /* other: even */
    assert(result(0x10ff, 0x0400) && random_calls == 1);
    reset(); random_answer = 7; run();
    assert(result(0xfff, 0x0406));
    reset(); random_answer = -3; run();                          /* negative odd remainder */
    assert(result(0xfff, 0x0406));
}

static int empty_line(void)
{
    in_empty = 0;
    ctl_calls = 0;
    return TaLogic012_GetEmptyLine((struct jp_9a533 *)(size_t)unit_answer);
}

/* 0x143970, branch coverage: every line outcome, the `id >= 0` and controller tests, both ends. */
static void check_empty_line(void)
{
    reset();
    grid_mode = 1;
    memset(map_id, 0xff, sizeof(map_id));
    memset(agree_by, 0, sizeof(agree_by));
    ctl_by[0] = 5; ctl_by[1] = 5; ctl_by[2] = 6; ctl_by[3] = 6;
    /* no line agrees */
    assert(empty_line() == -1 && ctl_calls == 0);
    /* the first agreeing line; an answer other than 1 does not agree */
    agree_by[2] = 1; agree_by[4] = 1;
    assert(empty_line() == 2);
    agree_by[2] = 2;
    assert(empty_line() == 4);
    agree_by[2] = 1;
    /* a unit of another controller leaves the line empty; one of the handle's does not */
    map_id[2][3] = 0x0293;
    assert(empty_line() == 2 && ctl_calls == 1);
    map_id[2][6] = 0x0196;
    assert(empty_line() == 4 && ctl_calls == 2);
    map_id[2][6] = -1; map_id[2][0] = 0x0090; /* id high byte 0: still a unit */
    assert(empty_line() == 4 && ctl_calls == 2);
    /* both ends */
    memset(agree_by, 0, sizeof(agree_by));
    agree_by[6] = 1;
    assert(empty_line() == 6);
    agree_by[0] = 1;
    assert(empty_line() == 0);
    grid_mode = in_empty = 0;
}

int main(void)
{
    check_early();
    check_land();
    check_first_walk();
    check_later_stages();
    check_empty_line();
    return 0;
}
